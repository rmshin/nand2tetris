#include "symtable.h"
#include "uthash.h"

struct symaddress
{
    char symbol[MAX_SYMBOL_LEN];
    int address;
    UT_hash_handle hh;
};

static struct symaddress predef_symbols[23] = {
    {"SP", 0},
    {"LCL", 1},
    {"ARG", 2},
    {"THIS", 3},
    {"THAT", 4},
    {"R0", 0},
    {"R1", 1},
    {"R2", 2},
    {"R3", 3},
    {"R4", 4},
    {"R5", 5},
    {"R6", 6},
    {"R7", 7},
    {"R8", 8},
    {"R9", 9},
    {"R10", 10},
    {"R11", 11},
    {"R12", 12},
    {"R13", 13},
    {"R14", 14},
    {"R15", 15},
    {"SCREEN", 16384},
    {"KBD", 24576},
};

static struct symaddress *entries = NULL;
static struct symaddress usr_symbols[MAX_SYMBOL_COUNT];
static int sym_idx = 0;

void init_symtable(void)
{
    for (int i = 0; i < 23; i++)
    {
        HASH_ADD_STR(entries, symbol, &predef_symbols[i]);
    }
};

void add_entry(char *sym, int address)
{
    strcpy(usr_symbols[sym_idx].symbol, sym);
    usr_symbols[sym_idx].address = address;
    HASH_ADD_STR(entries, symbol, &usr_symbols[sym_idx]);
    sym_idx++;
};

bool contains(char *sym)
{
    struct symaddress *sa = NULL;
    HASH_FIND_STR(entries, sym, sa);
    return sa != NULL;
};

int get_address(char *sym)
{
    struct symaddress *sa;
    HASH_FIND_STR(entries, sym, sa);
    return sa->address;
};