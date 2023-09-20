#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>

void init_symtable(void);
void add_entry(char *symbol, int address);
bool contains(char *symbol);
int get_address(char *symbol);

#endif