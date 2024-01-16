static mb_device_data_map_t mb_data_map[] = {
    
{.start_addr = 0, .datatype = MB_INT16, .bo = ABCD, .dp_value_range = CONSTANT, .max_value = -1, .min_value = -1, .value.i_data = 100, .step = -1},
{.start_addr = 1, .datatype = MB_INT32, .bo = ABCD, .dp_value_range = CONSTANT, .max_value = -1, .min_value = -1, .value.i_data = 100, .step = -1},
{.start_addr = 3, .datatype = MB_INT32, .bo = DCBA, .dp_value_range = CONSTANT, .max_value = -1, .min_value = -1, .value.i_data = 100, .step = -1},
{.start_addr = 5, .datatype = MB_FLOAT32, .bo = ABCD, .dp_value_range = CONSTANT, .max_value = -1, .min_value = -1, .value.f_data = 1234.56, .step = -1},
{.start_addr = 7, .datatype = MB_FLOAT32, .bo = DCBA, .dp_value_range = CONSTANT, .max_value = -1, .min_value = -1, .value.f_data = 1234.56, .step = -1},
};
