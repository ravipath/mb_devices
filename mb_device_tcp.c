#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "modbus.h"
#include "modbus-rtu.h"
#include "mb_device_tcp.h"
#include "mb_sim.h"

#define UNIT_ID 1

#ifdef DEBUG
#define DEBUG_PRINT(...)              \
    do                                \
    {                                 \
        fprintf(stderr, __VA_ARGS__); \
    } while (false)
#else
#define DEBUG_PRINT(...) \
    do                   \
    {                    \
    } while (false)
#endif

static mb_device_t *HEAD = NULL;
static modbus_mapping_t *modbus_register_map;

mb_device_t *get_head(void)
{
    return HEAD;
}

void print_usage(void)
{
    fprintf(stderr, "Usage: mb_device [-f csv_file_path] [-p port]\n");
    fprintf(stderr, "          [-p] port (e.g., 501)\n");
}

float read_mb_register_f32(uint8_t address, mbdevice_byteorder_t bo, uint8_t length)
{
    float real;
    switch (bo)
    {
    case ABCD:
        real = modbus_get_float_abcd(modbus_register_map->tab_registers + address);
        break;
    case CDAB:
        real = modbus_get_float_cdab(modbus_register_map->tab_registers + address);
        break;
    case BADC:
        real = modbus_get_float_badc(modbus_register_map->tab_registers + address);
        break;
    case DCBA:
        real = modbus_get_float_dcba(modbus_register_map->tab_registers + address);
        break;
    default:
        break;
    }
    return real;
}

uint32_t read_mb_register(uint8_t address, mbdevice_byteorder_t bo, uint8_t length)
{
    uint32_t data = 0;
    uint16_t *mb_reg = modbus_register_map->tab_registers + address;
    switch (length)
    {
    case 1:
        data = *mb_reg;
        break;
    case 2:
        switch (bo)
        {
        case ABCD: // big endian
            data = (data | *mb_reg) << 16;
            data = (data | *(mb_reg + 1));
            break;
        case CDAB:
            break;
        case BADC: // little endian
            data = (data | *mb_reg) << 16;
            data = (data | *(mb_reg + 1));
            data = ((data & (0x0000FFFF)) << 16) | ((data & (0xFFFF0000)) >> 16);
            data = ((data & (0x00FF00FF)) << 8) | ((data & (0xFF00FF00)) >> 8);
            break;
        case DCBA:
            break;
        default:
            break;
        }

        break;
    default:
        break;
    }

    return data;
}

void write_mb_register_float32(uint16_t offset, mbdevice_byteorder_t bo, float data)
{
    uint16_t *mb_reg_offset = modbus_register_map->tab_registers + offset;
    switch (bo)
    {
    case ABCD:
        modbus_set_float_abcd(data, mb_reg_offset);
        break;
    case CDAB:
        modbus_set_float_cdab(data, mb_reg_offset);
        break;
    case BADC:
        modbus_set_float_badc(data, mb_reg_offset);
        break;
    case DCBA:
        modbus_set_float_dcba(data, mb_reg_offset);
        break;
    default:
        break;
    }
}

void write_mb_register(uint16_t offset, mbdevice_byteorder_t bo, mbdevice_datatype_t datatype, uint32_t data)
{
    uint16_t *mb_reg_offset = modbus_register_map->tab_registers + offset;
    switch (datatype)
    {
    case MB_INT16:
        // 16 bit big endian - write code to process 16 bit little endian
        printf("write INT16: %d\n", data);
        *mb_reg_offset = data;
        break;
    case MB_INT32:
        switch (bo)
        {
        case ABCD: // big endian
            printf("write ABCD: %d\n", data);
            *mb_reg_offset = (uint16_t)(data >> 16);
            *(mb_reg_offset + 1) = (uint16_t)(data);
            break;
        case CDAB:
            break;
        case BADC: // little endian
            printf("write BADC\n");
            data = htonl(data);
            *mb_reg_offset = (uint16_t)(data >> 16);
            *(mb_reg_offset + 1) = (uint16_t)(data);
            break;
        case DCBA:
            break;
        }
        // 32 bit big endian - write code to process 32 bit little endian
        *mb_reg_offset = (uint16_t)(data >> 16);
        *(mb_reg_offset + 1) = (uint16_t)(data);
        break;
    default:
        break;
    }
}

uint32_t calc_num_datapoints(void)
{
    uint32_t count = 0;

    mb_device_t *temp = HEAD;
    if (temp == NULL)
    {
        printf("list empty\n");
        return 0;
    }

    while (temp != NULL)
    {
        count += 1;
        temp = temp->nxt;
    }
    return count;
}

uint32_t calc_num_modbus_registers(void)
{

    uint32_t reg_sum = 0;

    mb_device_t *temp = HEAD;
    if (temp == NULL)
    {
        printf("list empty\n");
        return 0;
    }

    while (temp != NULL)
    {

        reg_sum += temp->regs;
        temp = temp->nxt;
    }
    return reg_sum;
}

// debug
void printlist(void)
{

    mb_device_t *temp = HEAD;
    if (temp == NULL)
    {
        printf("list empty\n");
        return;
    }
    uint8_t counter = 0;
    while (temp != NULL)
    {
        printf("---------\n");
        printf("element %d\n", counter++);
        printf("---------\n");
        printf("name: %s\n", temp->name);
        printf("byteorder: %d\n", temp->byteorder);
        printf("datatype: %d\n", temp->datatype);
        printf("unit: %s\n", temp->unit);
        printf("fc: %d\n", temp->fc);
        printf("start address: %d\n", temp->start_address);
        printf("regs: %d\n", temp->regs);
        printf("scale_factor: %d\n", temp->scale_factor);
        temp = temp->nxt;
    }
}

void insert_dev(mb_device_t *d)
{
    // if d is the first element
    if (HEAD == NULL)
    {
        printf("*HEAD is NULL\n");
        HEAD = d;
        return;
    }
    mb_device_t *temp;
    temp = HEAD;
    while (temp->nxt != NULL)
        temp = temp->nxt;

    temp->nxt = d;
}

int create_mb_device_elements(mb_device_t **d, mbdevice_element_t e, char *data)
{
    int r = 0;
    mb_device_t *l_dev = *d;
    l_dev->nxt = NULL;
    // printf("element: %d\n", e);
    switch (e)
    {
    case INTEGRATE:
        // char tok = data[0];
        l_dev->integrate = atoi((char *)data);
        // printf("INTEGRATE: %s | Inserted: %d\n", (char *)data, l_dev->integrate);
        break;
    case INTERVAL:
        l_dev->interval = atoi((char *)data);
        // printf("INTERVAL: %s | Inserted: %d\n", (char *)data, l_dev->interval);
        break;
    case KEY:
        l_dev->name = (char *)malloc(strlen((char *)data));
        if (l_dev->name == NULL)
        {
            r = -1;
        }
        else
        {
            strcpy(l_dev->name, (char *)data);
        }
        // printf("KEY: %s | Inserted: %s\n", (char *)data, l_dev->name);
        break;
    case ADDRESS:
        l_dev->start_address = atoi((char *)data);
        // printf("ADDRESS: %s | Inserted: %d\n", (char *)data, l_dev->start_address);
        break;
    case FUNCTION_CODE:
        l_dev->fc = atoi((char *)data);
        // printf("FUNCTION_CODE: %s | Inserted: %d\n", (char *)data, l_dev->fc);
        break;
    case REGISTERS:
        l_dev->regs = atoi((char *)data);
        // printf("REGISTERS: %s | Inserted: %d\n", (char *)data, l_dev->regs);
        break;
    case FORMAT:
        if (strcmp((char *)data, "INT8") == 0)
        {
            l_dev->datatype = MB_INT8;
        }
        else if (strcmp((char *)data, "INT16") == 0)
        {
            l_dev->datatype = MB_INT16;
        }
        else if (strcmp((char *)data, "INT32") == 0)
        {
            l_dev->datatype = MB_INT32;
        }
        else if (strcmp((char *)data, "FLOAT32") == 0)
        {
            l_dev->datatype = MB_FLOAT32;
        }
        else
        {
            l_dev->datatype = MB_UNKNOWN;
        }
        // printf("FORMAT: %s | Inserted: %d\n", (char *)data, l_dev->datatype);
        break;
    case BO:
        if (strcmp((char *)data, "ABCD") == 0)
        {
            l_dev->byteorder = ABCD;
        }
        else if (strcmp((char *)data, "CDAB") == 0)
        {
            l_dev->byteorder = CDAB;
        }
        else if (strcmp((char *)data, "BADC") == 0)
        {
            l_dev->byteorder = BADC;
        }
        else if (strcmp((char *)data, "DCBA") == 0)
        {
            l_dev->byteorder = DCBA;
        }
        else
        {
            l_dev->byteorder = MB_UNKNOWN;
        }
        // printf("BO: %s | Inserted: %d\n", (char *)data, l_dev->byteorder);
        break;
    case UNIT:
        l_dev->unit = (char *)malloc(strlen((char *)data));
        if (l_dev->unit == NULL)
        {
            r = -1;
        }
        else
        {
            strcpy(l_dev->unit, (char *)data);
        }
        // printf("UNIT: %s | Inserted: %s\n", (char *)data, l_dev->unit);
        break;
    case SCALE_FACTOR:
        l_dev->scale_factor = atoi((char *)data);
        // printf("SCALE_FACTOR: %s | Inserted: %d\n", (char *)data, l_dev->scale_factor);
        break;
    default:
        break;
    }
    return r;
}

// example usage: mbdevice chp_template.csv /dev/ttyUSB0 115200 N 8 1
int main(int argc, char *argv[])
{
    int opt;
    char buf[1024] = {0};
    char *bufPtr = buf;
    int count = 0;
    int elements_processed = 0;
    uint8_t line_index = 0;

    // pthread
    pthread_t rtu_thread, simulator_thread;
    pthread_mutex_t mutex;
    int r_th = 0;

    mb_tcp_client_config_t *s = (mb_tcp_client_config_t *)malloc(sizeof(mb_tcp_client_config_t));

    // cmdline args
    FILE *csv_file = NULL;
    const char *server_ip = "0.0.0.0";
    int32_t port = -1;

    mbdevice_element_t element;
    mb_device_t *dev = NULL;

    while ((opt = getopt(argc, argv, "s:f:p:")) != EOF)
    {
        switch (opt)
        {
        case 'f':
            csv_file = fopen(optarg, "r");
            if (csv_file == NULL)
            {
                printf("%s: %s\n", optarg, strerror(errno));
                return 0;
            }
            printf("CSV file: %s\n", optarg);
            break;
        case 'p':
            s->port = atoi(optarg);
            printf("port: %d\n", s->port);
            break;
        case 's':
            s->unit_id = atoi(optarg);
            printf("unit_id: %d\n", s->unit_id);
            break;
        default:
            printf("unknown argument: %s\n", optarg);
            break;
        }
    }

    if (csv_file == NULL)
    {
        printf("file error: no csv file was found\n");
        print_usage();
        return 0;
    }
    if (server_ip == NULL)
    {
        printf("server_ip: no server ip was specified\n");
        print_usage();
        return 0;
    }

    s->ip = (char *)malloc(strlen(server_ip) + 1);
    strcpy(s->ip, server_ip);
    printf("\n\n");

    // read and ignore first line since this contains csv headers
    // Appropriate check must be coded later to asscertain consistency with the csv template
    fgets(buf, 1024, csv_file);
    memset(buf, 0, 1024);

    char *tok;
    int device_node_counter = 0;

    // start reading csv line by line
    while (fgets(buf, 1024, csv_file))
    {
        line_index = 0;
        dev = (mb_device_t *)malloc(sizeof(mb_device_t));
        if (dev == NULL)
        {
            printf("cannot allocate memory for device node\n");
            return 0;
        }
        bufPtr = buf;
        while (tok = strsep(&bufPtr, ";"))
        {
            create_mb_device_elements(&dev, line_index, tok);
            line_index++;
        }

        insert_dev(dev);
    }
    fclose(csv_file);
    // printlist();

    uint32_t num_modbus_registers = calc_num_modbus_registers();
    modbus_register_map = modbus_mapping_new_start_address(0,
                                                           0,
                                                           0,
                                                           0,
                                                           0,
                                                           num_modbus_registers,
                                                           0,
                                                           0);

    printf("Creating threads\n");

    r_th = pthread_create(&rtu_thread, NULL, rtu_worker, s);
    r_th = pthread_create(&simulator_thread, NULL, simulator_worker, (void *)NULL);

    pthread_join(rtu_thread, NULL);
    pthread_join(simulator_thread, NULL);
    free_allocated_spaces();
    free(s->ip);
    free(s);
    modbus_mapping_free(modbus_register_map);
    printf("exiting the MAIN application ...\n");
}

void *rtu_worker(void *ptr)
{
    int rc;
    int s = -1;
    int count = 0;
    // int header_length;

    uint8_t *query = NULL;
    modbus_t *ctx;
    mb_tcp_client_config_t *d = ptr;

    printf("connect to %s(unit_id = %d) at %d\n", d->ip, d->unit_id, d->port);

    ctx = modbus_new_tcp(d->ip, d->port);
    query = malloc(MODBUS_TCP_MAX_ADU_LENGTH);
    modbus_set_debug(ctx, TRUE);

    s = modbus_tcp_listen(ctx, d->unit_id);
    modbus_tcp_accept(ctx, &s);

    for (;;)
    {
        do
        {
            rc = modbus_receive(ctx, query);
        } while (rc == 0);

        /* The connection is not closed on errors which require on reply such as
           bad CRC in RTU. */
        if (rc == -1 && errno != EMBBADCRC)
        {
            /* Quit */
            printf("rc returned -1 for RECV\n");
            // break;
        }
        rc = modbus_reply(ctx, query, rc, modbus_register_map);
        if (rc == -1)
        {
            printf("rc returned -1 for REPLY \n");
        }
    }
    modbus_close(ctx);
    modbus_free(ctx);
}

void *simulator_worker(void *ptr)
{
    uint32_t counter = 0;
    for (;;)
    {
        // system("clear");
        mb_sim();
        print_mb_map();
        sleep(5);
    }
}
void free_allocated_spaces(void)
{
    mb_device_t *temp = HEAD;
    mb_device_t *prev = NULL;
    while (temp != NULL)
    {
        HEAD = temp->nxt;
        if (temp->name != NULL)
        {
            free(temp->name);
        }
        free(temp);
        temp = HEAD;
    }
    if (HEAD == NULL)
    {
        free(HEAD);
    }
}