#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "mb_device_tcp.h"
#include "mb_sim.h"

#define DELIMITER "|"
#define PADDING "     "
#define FORMAT "ABCD"
#define MAX_STR_LEN 18

static char table[15][150] = {0, 0};
static char *elements[] = {"NAME", "ADDRESS", "REGISTER", "FORMAT", "VALUE", "UNIT"};

//static mb_device_data_map_t mb_data_map2[15] = {0};

static mb_device_data_map_t mb_data_map[] = {
    {.start_addr = 0, .datatype = "INT16", .format = "ABCD", .dp_value_range = CONSTANT, .max_value = -1, .min_value = -1, .value.i_data = 101, .step = -1},
    {.start_addr = 1, .datatype = "INT16", .format = "ABCD", .dp_value_range = CONSTANT, .max_value = -1, .min_value = -1, .value.i_data = 102, .step = -1},
};

size_t string_lengh(const char *s)
{
    size_t length = 0;
    while (*s++ != '\0')
        length++;
    return length;
}

static char *create_row(char *row_addr, char *str, int str_size)
{
    int char_counter = 0;
    int left_indent = 0, right_indent = 0;

    int tmp_len = str_size;
    if (tmp_len >= MAX_STR_LEN)
    {
        char tmp_str[16] = {0};
        memcpy(tmp_str, str, 15);
        left_indent = 2;
        right_indent = 2;
    }
    else
    {
        switch (tmp_len % 2)
        {
        case 0:
            left_indent = right_indent = (20 - tmp_len) / 2;
            break;
        case 1:
            left_indent = (20 - tmp_len) / 2;
            right_indent = left_indent + 1;
        }
    }
    memcpy(row_addr + char_counter, DELIMITER, 1);
    char_counter++;
    while (left_indent)
    {
        memcpy(row_addr + char_counter, " ", 1);
        char_counter++;
        left_indent--;
    }
    memcpy(row_addr + char_counter, str, str_size);
    char_counter += str_size;
    while (right_indent)
    {
        memcpy(row_addr + char_counter, " ", 1);
        char_counter++;
        right_indent--;
    }
    memcpy(row_addr + char_counter, DELIMITER, 1);
}

int print_mb_map(void)
{
    int char_counter = 0;
    int left_indent = 0, right_indent = 0;
    mb_device_t *head = get_head();
    mb_device_t *temp = head;
    uint8_t count = calc_num_datapoints();
    uint8_t elements_length = sizeof(elements) / sizeof(elements[0]);

    for (int n = 0; n < elements_length; n++)
    {
        int tmp_len = string_lengh(elements[n]);
        if (tmp_len > MAX_STR_LEN)
        {
            char tmp_str[18] = {0};
            memcpy(tmp_str, elements[n], 16);
            memcpy(tmp_str + 16, "~", 1);
            left_indent = 1;
            right_indent = 1;
        }
        else
        {
            switch (tmp_len % 2)
            {
            case 0:
                left_indent = right_indent = (20 - tmp_len) / 2;
                break;
            case 1:
                left_indent = (20 - tmp_len) / 2;
                right_indent = left_indent + 1;
                break;
            default:
                break;
            }
        }
        memcpy(table[0] + char_counter, DELIMITER, 1);
        char_counter++;
        while (left_indent)
        {
            memcpy(table[0] + char_counter, " ", 1);
            char_counter++;
            left_indent--;
        }
        memcpy(table[0] + char_counter, elements[n], string_lengh(elements[n]));
        char_counter += string_lengh(elements[n]);
        while (right_indent)
        {
            memcpy(table[0] + char_counter, " ", 1);
            char_counter++;
            right_indent--;
        }
        memcpy(table[0] + char_counter, DELIMITER, 1);
    }
    int row_counter = 0;
    while (temp != NULL)
    {
        int index_counter = 0;
        int tmp_len = 0;

        // start from 1
        row_counter++;

        // copy name
        create_row(table[row_counter] + index_counter, temp->name, string_lengh(temp->name));
        index_counter += 21;

        // copy address
        char tmpstr[10] = {0};
        sprintf(tmpstr, "%d", temp->start_address);
        tmp_len = string_lengh(tmpstr);
        create_row(table[row_counter] + index_counter, tmpstr, tmp_len);
        index_counter += 21;

        // copy reg
        memset(tmpstr, 0, 10);
        sprintf(tmpstr, "%d", temp->regs);
        tmp_len = string_lengh(tmpstr);
        create_row(table[row_counter] + index_counter, tmpstr, tmp_len);
        index_counter += 21;

        // copy format
        create_row(table[row_counter] + index_counter, temp->byteorder, string_lengh(temp->byteorder));
        index_counter += 21;

        // copy value
        memset(tmpstr, 0, 10);
        if (strncmp(temp->datatype, "INT16", 5) == 0 || strncmp(temp->datatype, "INT32", 5) == 0)
        {
            uint32_t value = read_mb_register(temp->start_address, temp->regs);
            sprintf(tmpstr, "%d", value);
            tmp_len = string_lengh(tmpstr);
            create_row(table[row_counter] + index_counter, tmpstr, tmp_len);
        }
        if (strncmp(temp->datatype, "FLOAT32", 7) == 0)
        {
            float real = read_mb_register_f32(temp->start_address, temp->regs);
            int len = snprintf(NULL, 0, "%f", real);
            char *tmp_str = malloc(len + 1);
            snprintf(tmp_str, len + 1, "%f", real);
            tmp_len = string_lengh(tmp_str);
            create_row(table[row_counter] + index_counter, tmp_str, tmp_len);
            free(tmp_str);
        }
        index_counter += 21;

        // copy unit
        create_row(table[row_counter] + index_counter, temp->unit, string_lengh(temp->unit));
        index_counter += 21;

        temp = temp->nxt;
    }

    printf("-------------------------------------------------------------------------------------------------------------------------------\n");
    printf("%s\n", table[0]);
    printf("-------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 1; i <= count; i++)
    {
        printf("%s\n", table[i]);
    }
    printf("-------------------------------------------------------------------------------------------------------------------------------\n");

    return 0;
}

uint32_t generate_random_number(uint32_t min, uint32_t max)
{
    return (rand() % (max - min + 1)) + min;
}

int mb_sim(void)
{
    mb_device_t *head = get_head();
    mb_device_t *temp = head;
    uint8_t count = 0;
    float f_data;
    uint32_t i_data;
    int i = 0;
    while (temp != NULL)
    {
        uint8_t index = temp->start_address;
        if (mb_data_map[count].dp_value_range == INCREASING)
        {
            
            if (mb_data_map[count].datatype == "FLOAT32")
            {
                 write_mb_register_float32(temp->start_address, mb_data_map[count].value.f_data);
                f_data = mb_data_map[count].value.f_data;
                mb_data_map[count].value.f_data = f_data + mb_data_map[count].step;
            }
            else
            {
                write_mb_register(temp->start_address, temp->regs, mb_data_map[count].value.i_data);
                i_data = mb_data_map[count].value.i_data;
                mb_data_map[count].value.i_data = i_data + mb_data_map[count].step;
            }
        }
        else if (mb_data_map[count].dp_value_range == RANGE)
        {
            i_data = generate_random_number(mb_data_map[count].min_value, mb_data_map[count].max_value);
            write_mb_register(temp->start_address, temp->regs, i_data);
        }
        else if (mb_data_map[count].dp_value_range == CONSTANT)
        {
            if (mb_data_map[count].datatype == "FLOAT32")
            {
                write_mb_register_float32(temp->start_address, mb_data_map[count].value.f_data);
            }
            else
            {
                write_mb_register(temp->start_address, temp->regs, mb_data_map[count].value.i_data);
            }
        }
        // write_mb_register_float32
        temp = temp->nxt;
        count++;
    }
}