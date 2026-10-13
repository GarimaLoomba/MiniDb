
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../include/catalog.h"
#include "../include/storage.h"

#define CATALOG_FILE "data/metadata/catalog.dat"
#define TABLE_DIRECTORY "data/tables/"

/* Allow only letters, digits and underscores in table names. */
static int is_valid_table_name(const char *name)
{
    if (name == NULL || name[0] == '\0')
        return 0;

    for (int i = 0; name[i] != '\0'; i++)
    {
        if (!isalnum((unsigned char)name[i]) && name[i] != '_')
            return 0;
    }

    return 1;
}

/* Save all table names in the catalog file. */
int catalog_save(const Database *database)
{
    if (database == NULL ||
        database->table_count < 0 ||
        database->table_count > MAX_TABLES)
    {
        return -1;
    }

    FILE *file = fopen(CATALOG_FILE, "wb");

    if (file == NULL)
    {
        perror("Could not save catalog");
        return -1;
    }

    if (fwrite(&database->table_count,
               sizeof(database->table_count), 1, file) != 1)
    {
        fclose(file);
        return -1;
    }

    for (int i = 0; i < database->table_count; i++)
    {
        const char *name = database->tables[i].name;

        if (!is_valid_table_name(name))
        {
            fclose(file);
            return -1;
        }

        size_t name_length = strlen(name) + 1;

        if (name_length > MAX_TABLE_NAME)
        {
            fclose(file);
            return -1;
        }

        if (fwrite(&name_length, sizeof(name_length), 1, file) != 1 ||
            fwrite(name, 1, name_length, file) != name_length)
        {
            fclose(file);
            return -1;
        }
    }

    if (fclose(file) != 0)
        return -1;

    return 0;
}

/* Load table names and their saved table files. */
int catalog_load(Database *database)
{
    if (database == NULL)
        return -1;

    FILE *file = fopen(CATALOG_FILE, "rb");

    /* No catalog means this is the first run. */
    if (file == NULL)
        return 1;

    int table_count;

    if (fread(&table_count, sizeof(table_count), 1, file) != 1 ||
        table_count < 0 || table_count > MAX_TABLES)
    {
        fclose(file);
        database->table_count = 0;
        return -1;
    }

    database->table_count = 0;

    for (int i = 0; i < table_count; i++)
    {
        size_t name_length;
        char table_name[MAX_TABLE_NAME];

        if (fread(&name_length, sizeof(name_length), 1, file) != 1 ||
            name_length < 2 || name_length > MAX_TABLE_NAME)
        {
            fclose(file);
            database->table_count = 0;
            return -1;
        }

        if (fread(table_name, 1, name_length, file) != name_length ||
            table_name[name_length - 1] != '\0')
        {
            fclose(file);
            database->table_count = 0;
            return -1;
        }

        if (!is_valid_table_name(table_name))
        {
            fclose(file);
            database->table_count = 0;
            return -1;
        }

        if (database_create_table(database, table_name) != 0)
        {
            fclose(file);
            database->table_count = 0;
            return -1;
        }

        char filename[256];

        int written = snprintf(filename, sizeof(filename),
                               "%s%s.dat",
                               TABLE_DIRECTORY, table_name);

        if (written < 0 ||
            (size_t)written >= sizeof(filename) ||
            storage_load_table(&database->tables[i], filename) != 0 ||
            strcmp(database->tables[i].name, table_name) != 0)
        {
            fclose(file);
            database->table_count = 0;
            return -1;
        }
    }

    fclose(file);
    return 0;
}
