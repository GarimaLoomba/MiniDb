```c
#ifndef PARSER_H
#define PARSER_H

#define MAX_COMMAND_LENGTH 256
#define MAX_TOKENS 10

int parser_tokenize(char *command, char *tokens[], int max_tokens);

#endif
```
