#include <stdio.h>
#include <string.h>

#define MAP_FILE_NAME "mb_data_map.c"
#define START_HEADER "static mb_device_data_map_t mb_data_map[] = {"
#define END_HEADER "};"
#define TAB "    "

const char *start_addr = "{.start_addr = ";
const char *datatype = ", .datatype = ";
const char *bo = ", .bo = ";
const char *dp_value_range = ", .dp_value_range = ";
const char *max_value = ", .max_value = ";
const char *min_value = ", .min_value = ";
const char *i_data = ", .value.i_data = ";
const char *f_data = ", .value.f_data = ";
const char *step = ", .step = ";
const char *endl_delimiter = "},\n";

int main(int argc, char *argv[])
{
    FILE *csv_file = NULL;
    FILE *map_file = NULL;
    char buf[1024] = {0};
    char *bufPtr = buf;
    char single_line[200];
    const char *starttext = START_HEADER;
    int counter = 0;
    int curser = 0;
    int token_index = 0;
    int next_index[] = {2, 5, 6};
    csv_file = fopen(argv[1], "r");
    map_file = fopen(MAP_FILE_NAME, "wa");

    fprintf(map_file, "%s\n", starttext);
    fprintf(map_file, "%s\n", TAB);

    fgets(buf, 1024, csv_file); // read and ignore first line
    memset(buf, 0, 1024);

    char *tok;
    while (fgets(buf, 1024, csv_file))
    {
        bufPtr = buf;
        token_index = next_index[counter++];
        memset(single_line, 0, 200);
        strcpy(single_line + curser, start_addr);
        curser += strlen(start_addr);
        int count = 0;
        while (count <= token_index)
        {
            tok = strsep(&bufPtr, ";");
            count++;
        }
        strcpy(single_line + curser, tok);
        curser += strlen(tok);
        strcpy(single_line + curser, datatype);
        curser += strlen(datatype);
        token_index = next_index[counter++];
        while (count <= token_index)
        {
            tok = strsep(&bufPtr, ";");
            count++;
        }
        if (strcmp(tok, "INT16") == 0)
        {
            strcpy(single_line + curser, "MB_INT16");
            curser += strlen("MB_INT16");
        }
        else if (strcmp(tok, "INT32") == 0)
        {
            strcpy(single_line + curser, "MB_INT32");
            curser += strlen("MB_INT32");
        }
        if (strcmp(tok, "FLOAT32") == 0)
        {
            strcpy(single_line + curser, "MB_FLOAT32");
            curser += strlen("MB_FLOAT32");
        }

        strcpy(single_line + curser, bo);
        curser += strlen(bo);
        token_index = next_index[counter++];
        while (count <= token_index)
        {
            tok = strsep(&bufPtr, ";");
            count++;
        }
        strcpy(single_line + curser, tok);
        curser += strlen(tok);
        strcpy(single_line + curser, dp_value_range);
        curser += strlen(dp_value_range);
        strcpy(single_line + curser, "CONSTANT");
        curser += strlen("CONSTANT");
        strcpy(single_line + curser, max_value);
        curser += strlen(max_value);
        strcpy(single_line + curser, "-1");
        curser += strlen("-1");
        strcpy(single_line + curser, min_value);
        curser += strlen(min_value);
        strcpy(single_line + curser, "-1");
        curser += strlen("-1");
        strcpy(single_line + curser, i_data);
        curser += strlen(i_data);
        strcpy(single_line + curser, "23");
        curser += strlen("23");
        strcpy(single_line + curser, step);
        curser += strlen(step);
        strcpy(single_line + curser, "-1");
        curser += strlen("-1");
        strcpy(single_line + curser, endl_delimiter);
        curser += strlen(endl_delimiter);
        fprintf(map_file, "%s", single_line);
        token_index = 0;
        counter = 0;
        curser = 0;
    }
    fprintf(map_file, "%s\n", END_HEADER);
    fclose(csv_file);
    return 0;
}