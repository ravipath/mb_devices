#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define DELIMITER "|"
#define PADDING "     "
#define FORMAT "ABCD"
#define MAX_STR_LEN 18

char *table[5] = {NULL};

char *elements[5] = {"NAME", "ADDRESS", "REGISTER", "FORMAT", "UNIT"};

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK";
    if (size)
    {
        --size;
        for (size_t n = 0; n < size; n++)
        {
            int key = rand() % (int)(sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

uint32_t generate_random_number(uint32_t min, uint32_t max)
{
    return (rand() % (max - min + 1)) + min;
}

char *create_row(char *row_addr, char *str, int str_size)
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
}

int main(int argc, char *argv[])
{
    int char_counter = 0;
    int left_indent = 0, right_indent = 0;

    table[0] = (char *)malloc(150);

    for (int n = 0; n < 5; n++)
    {
        int tmp_len = strlen(elements[n]);
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
        memcpy(table[0] + char_counter, elements[n], strlen(elements[n]));
        char_counter += strlen(elements[n]);
        while (right_indent)
        {
            memcpy(table[0] + char_counter, " ", 1);
            char_counter++;
            right_indent--;
        }
    }

    for (int i = 1; i < 5; i++)
    {
        int index_counter = 0;
        int tmp_len = 0;
        table[i] = (char *)malloc(150);

        // copy name
        char tmpstr[15];
        rand_string(tmpstr, 15);
        tmp_len = strlen(tmpstr);
        create_row(table[i] + index_counter, tmpstr, tmp_len);
        index_counter+=21;

        // copy address
        memset(tmpstr, 0, 15);
        int address = generate_random_number(100, 200);
        sprintf(tmpstr, "%d", address);
        tmp_len = strlen(tmpstr);
        create_row(table[i] + index_counter, tmpstr, tmp_len);
        index_counter+=21;

        // copy reg
        memset(tmpstr, 0, 15);
        address = generate_random_number(10, 20);
        sprintf(tmpstr, "%d", address);
        tmp_len = strlen(tmpstr);
        create_row(table[i] + index_counter, tmpstr, tmp_len);
        index_counter+=21;

        // copy format
        memset(tmpstr, 0, 15);
        memcpy(tmpstr, "ABCD", 4);
        tmp_len = strlen(tmpstr);
        create_row(table[i] + index_counter, tmpstr, tmp_len);
        index_counter+=21;

        // copy unit
        memset(tmpstr, 0, 15);
        rand_string(tmpstr, 3);
        tmp_len = strlen(tmpstr);
        create_row(table[i] + index_counter, tmpstr, tmp_len);
        index_counter+=21;
    }
    printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("%s\n", table[0]);
    printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");


    for (int count = 1; count < 5; count++)
    {
        printf("%s\n", table[count]);
    }

    printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");

    return 0;
}