#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "mb_sim.h"
#include "mb_device_tcp.h"

#define DELIMITER "|"
#define PADDING "     "
#define FORMAT "ABCD"
#define MAX_STR_LEN 18

static char *table[20] = {NULL};
static char *elements[5] = {"NAME", "ADDRESS", "REGISTER", "FORMAT", "UNIT"};

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
}

int print_mb_map(void)
{
    int char_counter = 0;
    int left_indent = 0, right_indent = 0;
            mb_device_t *head = get_head();
        mb_device_t *temp = head;
        uint8_t count = calc_num_datapoints();

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

    while (temp != NULL)
    {
        int index_counter = 0;
        int row_counter = 0;
        int tmp_len = 0;

        //start from 1
        row_counter++;
        table[row_counter] = (char *)malloc(150);

        // copy name
        create_row(table[row_counter] + index_counter, temp->name, strlen(temp->name));
        index_counter += 21;

        // copy address
        char tmpstr[10] = {0};
        sprintf(tmpstr, "%d", temp->start_address);
        create_row(table[row_counter] + index_counter, tmpstr, tmp_len);
        index_counter += 21;

        // copy reg
        memset(tmpstr, 0, 10);
        sprintf(tmpstr, "%d", temp->regs);
        tmp_len = strlen(tmpstr);
        create_row(table[row_counter] + index_counter, tmpstr, tmp_len);
        index_counter += 21;

        // copy format
        create_row(table[row_counter] + index_counter, temp->byteorder, strlen(temp->byteorder));
        index_counter += 21;

        // copy unit
        create_row(table[row_counter] + index_counter, temp->unit, strlen(temp->unit));
        index_counter += 21;

        temp = temp->nxt;
    }
    printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("%s\n", table[0]);
    printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int count = 1; count <= calc_num_datapoints(); count++)
    {
        printf("%s\n", table[count]);
    }

    printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");

    return 0;
}