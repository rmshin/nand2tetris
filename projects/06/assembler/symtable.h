#ifndef SYMTABLE_H
#define SYMTABLE_H
#define MAX_SYMBOL_LEN 128
#define MAX_SYMBOL_COUNT 1000

#include <stdbool.h>

void init_symtable(void);
void add_entry(char *symbol, int address);
bool contains(char *symbol);
int get_address(char *symbol);

#endif