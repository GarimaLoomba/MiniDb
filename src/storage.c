
#include <stdio.h>

#include "../include/storage.h"

/* Save a complete table to a binary file. */
int storage_save_table(const Table *table, const char *filename)
{
    if (table == NULL || filename == NULL)
        return -1;

    FILE *file = fopen(filename, "wb");

    if (file == NULL)
    {
        perror("Could not open table file for saving");
        return -1;
    }

    if (fwrite(table, sizeof(Table), 1, file) != 1)
    {
        perror("Could not write table data");
        fclose(file);
        return -1;
    }

    if (fclose(file) != 0)
    {
        perror("Could not close table file");
        return -1;
    }

    return 0;
}

/* Load a complete table from a binary file. */
int storage_load_table(Table *table, const char *filename)
{
    if (table == NULL || filename == NULL)
        return -1;

    FILE *file = fopen(filename, "rb");

    if (file == NULL)
        return -1;

    if (fread(table, sizeof(Table), 1, file) != 1)
    {
        fclose(file);
        return -1;
    }

    /* Reject files containing unexpected extra data. */
    if (fgetc(file) != EOF)
    {
        fclose(file);
        return -1;
    }

    if (ferror(file))
    {
        fclose(file);
        return -1;
    }

    if (fclose(file) != 0)
        return -1;

    return 0;
}
