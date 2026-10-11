#include <string.h>

#include "../include/database.h"

void database_init(Database *database, const char *name)
{
    strcpy(database->name, name);

    database->table_count = 0;
}

int database_create_table(Database *database, const char *table_name)
{
    if (database->table_count >= MAX_TABLES)
    {
        return -1;
    }

    table_init(
        &database->tables[database->table_count],
        table_name
    );

    database->table_count++;

    return 0;
}

int database_find_table(const Database *database, const char *table_name)
{
    for (int i = 0; i < database->table_count; i++)
    {
        if (strcmp(database->tables[i].name, table_name) == 0)
        {
            return i;
        }
    }

    return -1;
}   