#ifndef PARSER_H
#define PARSER_H
#include <stdbool.h>
#define MAX_CMD_LENGTH 4
#define MAX_SEG_LENGTH 9

typedef enum
{
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL,
} CommandType;

void init_file(char *path);
void close_file(void);
bool has_more_commands(void);
void advance(void);
int command_type(void);
char *arg1(void);
char *arg2(void);

#endif