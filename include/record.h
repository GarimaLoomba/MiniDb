#ifndef RECORD_H
#define RECORD_H

#define MAX_FIELDS 10
#define MAX_FIELD_SIZE 100

typedef struct
{
    char values[MAX_FIELDS][MAX_FIELD_SIZE];

} Record;

void record_init(Record *record);

#endif