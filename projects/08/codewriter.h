#ifndef CODEWRITER_H
#define CODEWRITER_H
#include "parser.h"

void init_out_file(char *path);
void close_out_file(void);
void write_arithmetic(char *cmd);
void write_push_pop(CommandType cmd_type, char *segment, int idx);
void write_init(void);
void write_label(char *label);
void write_goto(char *label);
void write_if(char *label);
void write_call(char *func_name, int num_args);
void write_return(void);
void write_function(char *func_name, int num_locals);

#endif