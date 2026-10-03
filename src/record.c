#include <string.h>

#include "../include/record.h"

void record_init(Record *record)
{
    for (int i = 0; i < MAX_FIELDS; i++)
    {
        record->values[i][0] = '\0';
    }
}