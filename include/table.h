#ifdef TABLE_H 
#define TABLE_H 

#define MAX_TABLE_NAME 50 
#define MAX_COLUMNS 10 
#define MAX_COLUMN_NAME 50 

typedef struct{
    char name[MAX_COLUMN_NAME];
} Column ;


typedef struct{
    char name[MAX_TABLE_NAME];
    int column_count ;
    Column columns[MAX_COLUMNS];
} Table ; 


void table_init(Table *table, const char *name);
#endif ; 