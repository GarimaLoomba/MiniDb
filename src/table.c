#include <string.h>

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