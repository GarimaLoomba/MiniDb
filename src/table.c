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

int table_update_record(
    Table *table,
    const char *id,
    const char *field,
    const char *new_value
)
{
    int field_index = -1;

    for (int i = 0; i < table->column_count; i++)
    {
        if (strcmp(table->columns[i].name, field) == 0)
        {
            field_index = i;
            break;
        }
    }

    if (field_index == -1)
    {
        return -1;
    }

    for (int i = 0; i < table->record_count; i++)
    {
        if (strcmp(table->records[i].values[0], id) == 0)
        {
            strcpy(
                table->records[i].values[field_index],
                new_value
            );

            return 0;
        }
    }

    return -1;
}

int table_delete_record(Table *table, const char *id)
{
    int record_index = -1;

    for (int i = 0; i < table->record_count; i++)
    {
        if (strcmp(table->records[i].values[0], id) == 0)
        {
            record_index = i;
            break;
        }
    }

    if (record_index == -1)
    {
        return -1;
    }

    for (int i = record_index; i < table->record_count - 1; i++)
    {
        table->records[i] = table->records[i + 1];
    }

    table->record_count--;

    return 0;
}


int table_find_record(const Table *table, const char *id)
{
    for (int i = 0; i < table->record_count; i++)
    {
        if (strcmp(table->records[i].values[0], id) == 0)
        {
            return i;
        }
    }

    return -1;
}

