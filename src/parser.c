
#include <string.h>

#include "../include/parser.h"

int parser_tokenize(char *command, char *tokens[], int max_tokens)
{
    int count = 0;

    char *token = strtok(command, " \t\n");

    while (token != NULL && count < max_tokens)
    {
        tokens[count] = token;
        count++;

        token = strtok(NULL, " \t\n");
    }

    return count;
}

