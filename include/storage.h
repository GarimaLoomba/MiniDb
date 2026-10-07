#ifndef STORAGE_H
#define STORAGE_H

#include "table.h"

int storage_save_table(const Table *table, const char *filename);

int storage_load_table(Table *table, const char *filename);

#endif