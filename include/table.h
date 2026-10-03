#ifndef TABLE_H
#define TABLE_H

#include "record.h"

#define MAX_TABLE_NAME 50
#define MAX_COLUMNS 10
#define MAX_COLUMN_NAME 50
#define MAX_RECORDS 100

typedef struct
{
    char name[MAX_COLUMN_NAME];

} Column;

typedef struct
{
    char name[MAX_TABLE_NAME];

    int column_count;

    Column columns[MAX_COLUMNS];

    int record_count;

    Record records[MAX_RECORDS];

} Table;

void table_init(Table *table, const char *name);

#endif