
#include <stdio.h>
#include <string.h>

#include "../include/database.h"
#include "../include/parser.h"

#define INPUT_SIZE MAX_COMMAND_LENGTH

int main(void)
{
    char command[INPUT_SIZE];

    /* Initialize database */
    Database database;
    database_init(&database, "minidb");

    printf("MiniDB started.\n");
    printf("Type .help for available commands.\n\n");

    while (1)
    {
        char *tokens[MAX_TOKENS];

        printf("MiniDB> ");

        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            break;
        }

        int token_count = parser_tokenize(
            command,
            tokens,
            MAX_TOKENS
        );

        if (token_count == 0)
        {
            continue;
        }

        /* Exit command */
        if (strcmp(tokens[0], ".exit") == 0)
        {
            printf("Goodbye!\n");
            break;
        }

        /* Help command */
        else if (strcmp(tokens[0], ".help") == 0)
        {
            printf("\nAvailable commands:\n");
            printf(".help   - Show available commands\n");
            printf(".tokens - Display parsed tokens\n");
            printf("CREATE <table> <columns...> - Create a table\n");
            printf(".exit   - Exit MiniDB\n\n");
        }

        /* Display parsed tokens */
        else if (strcmp(tokens[0], ".tokens") == 0)
        {
            printf("Tokens found: %d\n", token_count);

            for (int i = 0; i < token_count; i++)
            {
                printf("Token %d: %s\n", i + 1, tokens[i]);
            }
        }

        /* Create table command */
        else if (strcmp(tokens[0], "CREATE") == 0)
        {
            if (token_count < 3)
            {
                printf(
                    "Usage: CREATE <table> <column1> <column2> ...\n"
                );
                continue;
            }

            if (database_find_table(&database, tokens[1]) != -1)
            {
                printf("Table already exists.\n");
                continue;
            }

            if (token_count - 2 > MAX_COLUMNS)
            {
                printf(
                    "Too many columns. Maximum allowed: %d\n",
                    MAX_COLUMNS
                );
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

            if (success)
            {
                printf(
                    "Table '%s' created successfully.\n",
                    table->name
                );

                printf("Columns: ");

                for (int i = 0; i < table->column_count; i++)
                {
                    printf("%s", table->columns[i].name);

                    if (i < table->column_count - 1)
                    {
                        printf(", ");
                    }
                }

                printf("\n");
            }
            else
            {
                printf("Could not add columns.\n");
            }
        }

        /* Unknown command */
        else
        {
            printf(
                "Unknown command. Type .help for available commands.\n"
            );
        }
    }

    return 0;
}
