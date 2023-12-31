#ifndef PARSER_H
#define PARSER_H
#include "symtable.h"
#include <stdbool.h>
#define MAX_CMD_LENGTH (MAX_SYMBOL_LEN)

typedef enum
{
    A_COMMAND,
    C_COMMAND,
    L_COMMAND,
} CommandType;

void init_file(char *path);
void close_file(void);
void rewind_file(void);
bool has_more_commands(void);
void advance(void);
int command_type(void);
char *symbol(void);
char *dest(void);
char *comp(void);
char *jump(void);

#endif