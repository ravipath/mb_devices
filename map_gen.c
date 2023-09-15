#include <stdio.h>
#include <string.h>
#include <mb_sim.h>
#include "mb_device_tcp.h"

#define MAP_FILE_NAME "mb_data_map.c"
#define START_HEADER "static mb_device_data_map_t mb_data_map[] = {"
#define TAB "    {"

const char *value_ranges[] = {
    {INCREASING, "INCREASING"},
    {CONSTANT, "CONSTANT"},
    {RANGE, "RANGE"},
    {CONFIGURE, "CONFIGURE"}};

const char *start_addr = ".start_addr";//2
const char *datatype = ".datatype";//5
const char *format = ".format";//6
const char *dp_value_range = ".dp_value_range";//
const char *max_value = ".max_value";//
const char *min_value = ".min_value";//
const char *value = ".value";//
const char *step = ".step";//

int main(int argc, char *argv[])
{
    FILE *csv_file = NULL;
    FILE *map_file = NULL;
    char buf[1024] = {0};
    char *bufPtr = buf;
    char single_line[200];
    const char *starttext = START_HEADER;
    mbdevice_element_t e;
    int counter = 0;
    int token_index = 0;
    int next_index[] = {2, 5, 6};
    csv_file = fopen(argv[1], "r");
    map_file = fopen(MAP_FILE_NAME, "wa");

    //uint8_t  = sizeof(next_index) / sizeof(next_index[0]);
    
    //fwrite(starttext, sizeof( char ), strlen( starttext ), map_file);
    fprintf(map_file, "%s\n", starttext);
    fprintf(map_file, "%s\n", TAB);

    fgets(buf, 1024, csv_file);//read and ignore first line
    memset(buf, 0, 1024);

    char *tok;

    // start reading csv line by line
    while (fgets(buf, 1024, csv_file)){
        bufPtr = buf;
        token_index = next_index[counter];
        memset(single_line,0, 200);
        int count = 0;
        while (count < token_index)
        {
            tok = strsep(&bufPtr, ";");
            count++;
        }
    }


}