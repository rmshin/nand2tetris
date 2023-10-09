#ifndef CODEWRITER_H
#define CODEWRITER_H
#include "parser.h"

void init_out_file(char *path);
void close_out_file(void);
void write_arithmetic(char *cmd);
void write_push_pop(CommandType cmd_type, char *segment, int idx);

#endif