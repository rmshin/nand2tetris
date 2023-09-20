#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "code.h"

void dec_to_binary_str(char *str, int n)
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

int main(int argc, char **argv)
{
    char *path = argv[1];
    init_file(path);

    FILE *out = fopen("out.hack", "w");
    int cmd_type;
    char line[18], binary_str[16];
    while (has_more_commands())
    {
        advance();
        cmd_type = command_type();
        if (cmd_type == A_COMMAND)
        {
            char *decimal = symbol();
            dec_to_binary_str(binary_str, atoi(decimal));
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
        strcat(line, "\n");
        fputs(line, out);
    };

    fclose(out);
    close_file();
    return 0;
}