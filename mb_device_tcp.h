#ifndef _MB_DEVICE_H_
#define _MB_DEVICE_H_
#include "modbus.h"

typedef enum
{
    ABCD,
    CDAB,
    BADC,
    DCBA
} mbdevice_byteorder_t;

typedef enum
{
    MB_INT8,
    MB_INT16,
    MB_INT32,
    MB_FLOAT32,
    MB_UNKNOWN
} mbdevice_datatype_t;

typedef enum
{
    INTEGRATE,
    KEY,
    ADDRESS,
    FUNCTION_CODE,
    REGISTERS,
    FORMAT,
    BO,
    UNIT,
    SCALE_FACTOR,
    INTERVAL
} mbdevice_element_t;
 
typedef struct mb_tcp_client_config
{
    char *ip;
    uint32_t unit_id;
    uint32_t port;
} mb_tcp_client_config_t;

typedef struct mb_device
{
    char *name;
    char *unit;
    uint8_t fc;
    uint8_t regs;
    uint8_t start_address;
    uint8_t integrate;
    uint8_t interval;
    uint16_t scale_factor;
    mbdevice_byteorder_t byteorder;
    mbdevice_datatype_t datatype;
    struct mb_device *nxt;
} mb_device_t;

void insert_dev(mb_device_t *d);
void printlist(void);
mb_device_t *get_head(void);
uint32_t calc_num_datapoints(void);
int create_mb_device_elements(mb_device_t *d, mbdevice_element_t e, void *data);
modbus_mapping_t *create_modbus_mapping(uint16_t bytes);
uint32_t calc_num_modbus_registers(void);
void free_allocated_spaces(void);
uint8_t get_num_dp(void);
uint32_t read_mb_register(uint8_t address, mbdevice_byteorder_t bo, uint8_t length);
float read_mb_register_f32(uint8_t address, mbdevice_byteorder_t bo, uint8_t length);
void write_mb_register(uint16_t offset, mbdevice_byteorder_t bo, uint8_t num_regs, uint32_t data);
void write_mb_register_float32(uint16_t offset, mbdevice_byteorder_t bo, float data);
void *simulator_worker(void *ptr);
void *rtu_worker(void *ptr);

#endif