#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "code.h"
#include "symtable.h"

void dec_to_binary_str(char str[16], int n)
{
    // reset to 0
    strcpy(str, "000000000000000");
    // counter for binary pos
    int i = 0;
    while (n > 0 && i < 15)
    {
        // storing remainder in binary array
        if (n % 2 == 1)
        {
            str[14 - i] = '1';
        }
        n = n / 2;
        i++;
    }
}

void process_labels(void)
{
    int cmd_type, pc = 0;
    char sym[MAX_SYMBOL_LEN];
    while (has_more_commands())
    {
        advance();
        cmd_type = command_type();
        if (cmd_type == A_COMMAND || cmd_type == C_COMMAND)
        {
            pc++;
        }
        else if (cmd_type == L_COMMAND)
        {
            strcpy(sym, symbol());
            add_entry(sym, pc);
        }
    };

    rewind_file();
};

int main(int argc, char **argv)
{
    char *path = argv[1];
    init_file(path);
    init_symtable();
    process_labels();

    FILE *out = fopen("out.hack", "w");
    int cmd_type, ram_address = 16;
    char line[18], binary_str[16], sym[MAX_SYMBOL_LEN];
    while (has_more_commands())
    {
        advance();
        cmd_type = command_type();
        if (cmd_type == A_COMMAND)
        {
            strcpy(sym, symbol());
            char *end;
            int res, decimal;
            res = strtol(sym, &end, 10);
            if ((int)strlen(sym) != (end - sym)) // non-decimal symbol
            {
                if (contains(sym)) // known symbol
                {
                    decimal = get_address(sym);
                }
                else // new variable
                {
                    add_entry(sym, ram_address);
                    decimal = ram_address;
                    ram_address++;
                }
            }
            else
            {
                decimal = res;
            };
            dec_to_binary_str(binary_str, decimal);
            line[0] = '0';
            strcpy(line + 1, binary_str);
        }
        else if (cmd_type == C_COMMAND)
        {
            char *dest_str = dest();
            char *comp_str = comp();
            char *jump_str = jump();

            char *d = dest_code(dest_str);
            char *c = comp_code(comp_str);
            char *j = jump_code(jump_str);
            strcpy(line, "111");
            strcat(line, c);
            strcat(line, d);
            strcat(line, j);
        }
        else if (cmd_type == L_COMMAND)
        {
            continue; // skip L_COMMANDs
        }
        strcat(line, "\n");
        fputs(line, out);
    };

    fclose(out);
    close_file();
    return 0;
}