
#ifndef CATALOG_H
#define CATALOG_H

#include "database.h"



int catalog_save(const Database *database);
int catalog_load(Database *database);

#endif
