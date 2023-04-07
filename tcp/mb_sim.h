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

typedef struct mb_device_data_map{
    uint8_t start_addr;
    mb_device_dp_value_range_t dp_value_range;
    uint8_t step;
    uint32_t max_value;
    uint32_t min_value;
    uint32_t value;
} mb_device_data_map_t;

int mb_sim(void);
static char *create_row(char *row_addr, char *str, int str_size);
int print_mb_map(void);

#endif