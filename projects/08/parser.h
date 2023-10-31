#ifndef PARSER_H
#define PARSER_H
#include <stdbool.h>
#define MAX_FNAME_LENGTH 128
#define MAX_CMD_LENGTH 128
#define MAX_ARG_LENGTH 32

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
char *get_filename(void);
char *get_fextension(void);
bool has_more_commands(void);
void advance(void);
int command_type(void);
char *arg1(void);
int arg2(void);

#endif