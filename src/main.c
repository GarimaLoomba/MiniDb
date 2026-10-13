
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../include/database.h"
#include "../include/parser.h"
#include "../include/record.h"
#include "../include/storage.h"
#include "../include/catalog.h"

#define INPUT_SIZE MAX_COMMAND_LENGTH
#define TABLE_DIRECTORY "data/tables/"

/* Validate table names before using them in filenames. */
static int is_valid_table_name(const char *name)
{
    if (name == NULL || name[0] == '\0')
        return 0;

    if (strlen(name) >= MAX_TABLE_NAME)
        return 0;

    for (int i = 0; name[i] != '\0'; i++)
    {
        if (!isalnum((unsigned char)name[i]) && name[i] != '_')
            return 0;
    }

    return 1;
}

/* Save one table to disk. */
static int save_table_to_disk(const Table *table)
{
    if (table == NULL || !is_valid_table_name(table->name))
        return -1;

    char filename[256];

    int written = snprintf(
        filename,
        sizeof(filename),
        "%s%s.dat",
        TABLE_DIRECTORY,
        table->name
    );

    if (written < 0 || (size_t)written >= sizeof(filename))
        return -1;

    return storage_save_table(table, filename);
}

/* Report persistence errors without hiding them. */
static int persist_table(const Table *table)
{
    if (save_table_to_disk(table) != 0)
    {
        printf("Warning: Changes are in memory but could not be saved.\n");
        return -1;
    }

    return 0;
}

/* Display the supported commands. */
static void show_help(void)
{
    printf("\nAvailable commands:\n");
    printf(".help - Show available commands\n");
    printf(".tokens - Display parsed tokens\n");
    printf("CREATE <table> <columns...> - Create a table\n");
    printf("INSERT <table> <values...> - Insert a record\n");
    printf("SELECT <table> - Display all records\n");
    printf("FIND <table> <id> - Find a record by ID\n");
    printf("UPDATE <table> <id> <field> <value> - Update a field\n");
    printf("DELETE <table> <id> - Delete a record by ID\n");
    printf(".exit - Exit MiniDB\n\n");
}

int main(void)
{
    char command[INPUT_SIZE];

    Database database;
    database_init(&database, "minidb");

    int load_result = catalog_load(&database);

    if (load_result == 0)
    {
        printf("Saved database loaded successfully.\n");
    }
    else if (load_result == 1)
    {
        printf("Starting with an empty database.\n");
    }
    else
    {
        printf("Warning: Could not load saved database.\n");
        database_init(&database, "minidb");
    }

    printf("MiniDB started.\n");
    printf("Type .help for available commands.\n\n");

    while (1)
    {
        char *tokens[MAX_TOKENS];

        printf("MiniDB> ");
        fflush(stdout);

        if (fgets(command, sizeof(command), stdin) == NULL)
            break;

        /*
         * Detect commands that exceed the input buffer.
         * Consume the remaining characters so they cannot
         * accidentally become a second command.
         */
        if (strchr(command, '\n') == NULL && !feof(stdin))
        {
            int ch;

            while ((ch = getchar()) != '\n' && ch != EOF)
            {
                /* Discard the rest of the oversized input. */
            }

            printf("Error: Command is too long. Try again.\n");
            continue;
        }

        int token_count = parser_tokenize(
            command,
            tokens,
            MAX_TOKENS
        );

        if (token_count < 0)
        {
            printf("Error: Could not parse command.\n");
            continue;
        }

        if (token_count == 0)
            continue;

        /* Exit MiniDB. */
        if (strcmp(tokens[0], ".exit") == 0)
        {
            if (token_count != 1)
            {
                printf("Usage: .exit\n");
                continue;
            }

            printf("Goodbye!\n");
            break;
        }

        /* Display help. */
        else if (strcmp(tokens[0], ".help") == 0)
        {
            if (token_count != 1)
            {
                printf("Usage: .help\n");
                continue;
            }

            show_help();
        }

        /* Display parsed tokens. */
        else if (strcmp(tokens[0], ".tokens") == 0)
        {
            if (token_count != 1)
            {
                printf("Usage: .tokens\n");
                continue;
            }

            printf("Tokens found: %d\n", token_count);

            for (int i = 0; i < token_count; i++)
                printf("Token %d: %s\n", i + 1, tokens[i]);
        }

        /* Create a table. */
        else if (strcmp(tokens[0], "CREATE") == 0)
        {
            if (token_count < 3)
            {
                printf("Usage: CREATE <table> <column1> <column2> ...\n");
                continue;
            }

            if (!is_valid_table_name(tokens[1]))
            {
                printf("Invalid table name. Use letters, digits and underscores only.\n");
                continue;
            }

            if (database_find_table(&database, tokens[1]) != -1)
            {
                printf("Table already exists.\n");
                continue;
            }

            if (database.table_count >= MAX_TABLES)
            {
                printf("Cannot create table. Maximum table limit reached.\n");
                continue;
            }

            if (token_count - 2 > MAX_COLUMNS)
            {
                printf("Too many columns. Maximum allowed: %d\n", MAX_COLUMNS);
                continue;
            }

            if (database_create_table(&database, tokens[1]) != 0)
            {
                printf("Could not create table.\n");
                continue;
            }

            Table *table = &database.tables[database.table_count - 1];
            int success = 1;

            for (int i = 2; i < token_count; i++)
            {
                if (table_add_column(table, tokens[i]) != 0)
                {
                    success = 0;
                    break;
                }
            }

            if (!success)
            {
                database.table_count--;
                printf("Could not add columns. Check their names and limits.\n");
                continue;
            }

            if (save_table_to_disk(table) != 0)
            {
                database.table_count--;
                printf("Error: Could not save table data.\n");
                continue;
            }

            if (catalog_save(&database) != 0)
            {
                printf("Warning: Table data was saved, but the catalog could not be saved.\n");
                printf("The table may not be available after restarting.\n");
                continue;
            }

            printf("Table '%s' created successfully.\n", table->name);
            printf("Columns: ");

            for (int i = 0; i < table->column_count; i++)
            {
                printf("%s", table->columns[i].name);

                if (i < table->column_count - 1)
                    printf(", ");
            }

            printf("\n");
        }

        /* Insert a record. */
        else if (strcmp(tokens[0], "INSERT") == 0)
        {
            if (token_count < 3)
            {
                printf("Usage: INSERT <table> <value1> <value2> ...\n");
                continue;
            }

            int table_index = database_find_table(&database, tokens[1]);

            if (table_index == -1)
            {
                printf("Table not found.\n");
                continue;
            }

            Table *table = &database.tables[table_index];

            if (token_count - 2 != table->column_count)
            {
                printf("Incorrect number of values.\n");
                printf("Expected: %d values, received: %d\n",
                       table->column_count, token_count - 2);
                continue;
            }

            int valid = 1;

            for (int i = 0; i < table->column_count; i++)
            {
                if (strlen(tokens[i + 2]) >= MAX_FIELD_SIZE)
                {
                    printf("Value for column '%s' is too long.\n",
                           table->columns[i].name);
                    valid = 0;
                    break;
                }
            }

            if (!valid)
                continue;

            /* The first column is the record ID. */
            if (table_find_record(table, tokens[2]) != -1)
            {
                printf("A record with this ID already exists.\n");
                continue;
            }

            Record record;
            record_init(&record);

            for (int i = 0; i < table->column_count; i++)
                strcpy(record.values[i], tokens[i + 2]);

            if (table_insert_record(table, &record) != 0)
            {
                printf("Could not insert record. Table may be full.\n");
                continue;
            }

            if (persist_table(table) == 0)
                printf("Record inserted and saved successfully.\n");
        }

        /* Display all records. */
        else if (strcmp(tokens[0], "SELECT") == 0)
        {
            if (token_count != 2)
            {
                printf("Usage: SELECT <table>\n");
                continue;
            }

            int table_index = database_find_table(&database, tokens[1]);

            if (table_index == -1)
            {
                printf("Table not found.\n");
                continue;
            }

            Table *table = &database.tables[table_index];

            if (table->column_count == 0)
            {
                printf("Table has no columns.\n");
                continue;
            }

            for (int i = 0; i < table->column_count; i++)
                printf("%-15s", table->columns[i].name);

            printf("\n");

            for (int i = 0; i < table->column_count; i++)
                printf("---------------");

            printf("\n");

            for (int i = 0; i < table->record_count; i++)
            {
                for (int j = 0; j < table->column_count; j++)
                    printf("%-15s", table->records[i].values[j]);

                printf("\n");
            }

            if (table->record_count == 0)
                printf("No records found.\n");
        }

        /* Find a record by ID. */
        else if (strcmp(tokens[0], "FIND") == 0)
        {
            if (token_count != 3)
            {
                printf("Usage: FIND <table> <id>\n");
                continue;
            }

            int table_index = database_find_table(&database, tokens[1]);

            if (table_index == -1)
            {
                printf("Table not found.\n");
                continue;
            }

            Table *table = &database.tables[table_index];
            int record_index = table_find_record(table, tokens[2]);

            if (record_index == -1)
            {
                printf("Record not found.\n");
                continue;
            }

            printf("Record found:\n");

            for (int i = 0; i < table->column_count; i++)
            {
                printf("%s: %s\n",
                       table->columns[i].name,
                       table->records[record_index].values[i]);
            }
        }

        /* Update a record field. */
        else if (strcmp(tokens[0], "UPDATE") == 0)
        {
            if (token_count != 5)
            {
                printf("Usage: UPDATE <table> <id> <field> <new_value>\n");
                continue;
            }

            if (strlen(tokens[4]) >= MAX_FIELD_SIZE)
            {
                printf("New value is too long.\n");
                continue;
            }

            int table_index = database_find_table(&database, tokens[1]);

            if (table_index == -1)
            {
                printf("Table not found.\n");
                continue;
            }

            Table *table = &database.tables[table_index];

            int result = table_update_record(
                table,
                tokens[2],
                tokens[3],
                tokens[4]
            );

            if (result != 0)
            {
                printf("Update failed. Check the record ID and field name.\n");
                continue;
            }

            if (persist_table(table) == 0)
                printf("Record updated and saved successfully.\n");
        }

        /* Delete a record by ID. */
        else if (strcmp(tokens[0], "DELETE") == 0)
        {
            if (token_count != 3)
            {
                printf("Usage: DELETE <table> <id>\n");
                continue;
            }

            int table_index = database_find_table(&database, tokens[1]);

            if (table_index == -1)
            {
                printf("Table not found.\n");
                continue;
            }

            Table *table = &database.tables[table_index];
            int result = table_delete_record(table, tokens[2]);

            if (result != 0)
            {
                printf("Record not found.\n");
                continue;
            }

            if (persist_table(table) == 0)
                printf("Record deleted and saved successfully.\n");
        }

        /* Handle unsupported commands. */
        else
        {
            printf("Unknown command. Type .help for available commands.\n");
        }
    }

    return 0;
}
