#ifndef _MB_SIM_H_
#define _MB_SIM_H_

#include <stdint.h>

typedef enum
{
    INCREASING,
    CONSTANT,
    RANGE,
    CONFIGURE
} mb_device_dp_value_range_t;

/*typedef union data_value_ptr
{
    uint8_t *c_data_ptr;
    uint32_t *i_data_ptr;
    float *f_data_ptr;
} mb_device_data_value_ptr_t;*/

typedef union data_value
{
    uint32_t i_data;
    float f_data;
} mb_device_data_value_t;

typedef struct mb_device_data_map
{
    uint8_t start_addr;
    char *datatype;
    char *format;
    mb_device_dp_value_range_t dp_value_range;
    uint8_t step;
    uint32_t max_value;
    uint32_t min_value;
    mb_device_data_value_t value;
} mb_device_data_map_t;

int mb_sim(void);
static char *create_row(char *row_addr, char *str, int str_size);
int print_mb_map(void);

#endif