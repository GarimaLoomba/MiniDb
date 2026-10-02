#ifndef DATABASE_H
#define DATABASE_H

#include "table.h"

#define MAX_DATABASE_NAME 50
#define MAX_TABLES 20

typedef struct
{
    char name[MAX_DATABASE_NAME];

    int table_count;

    Table tables[MAX_TABLES];

} Database;

void database_init(Database *database, const char *name);

int database_create_table(Database *database, const char *table_name);

#endif