#include <stdio.h>
#include <string.h>

#include "../include/table.h"

void table_init(Table *table, const char *name)
{
    strcpy(table->name, name);
    table->column_count = 0;
}