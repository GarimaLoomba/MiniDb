#include <stdio.h>

#include "../include/storage.h"

int storage_save_table(const Table *table, const char *filename)
{
    FILE *file = fopen(filename, "wb");

    if (file == NULL)
    {
        return -1;
    }

    fwrite(table, sizeof(Table), 1, file);

    fclose(file);

    return 0;
}

int storage_load_table(Table *table, const char *filename)
{
    FILE *file = fopen(filename, "rb");

    if (file == NULL)
    {
        return -1;
    }

    if (fread(table, sizeof(Table), 1, file) != 1)
    {
        fclose(file);
        return -1;
    }

    fclose(file);

    return 0;
}