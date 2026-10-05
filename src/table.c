#include <string.h>
#include <stdio.h>

#include "../include/table.h"

void table_init(Table *table, const char *name)
{
    strcpy(table->name, name);

    table->column_count = 0;

    table->record_count = 0;
}

int table_insert_record(Table *table, const Record *record)
{
    if (table->record_count >= MAX_RECORDS)
    {
        return -1;
    }

    table->records[table->record_count] = *record;

    table->record_count++;

    return 0;
}

void table_print_records(const Table *table)
{
    for (int i = 0; i < table->record_count; i++)
    {
        printf("Record %d: ", i + 1);

        for (int j = 0; j < MAX_FIELDS; j++)
        {
            if (table->records[i].values[j][0] == '\0')
            {
                break;
            }

            printf("%s", table->records[i].values[j]);

            if (j < MAX_FIELDS - 1 &&
                table->records[i].values[j + 1][0] != '\0')
            {
                printf(" | ");
            }
        }

        printf("\n");
    }
}