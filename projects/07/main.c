#include <stdio.h>
#include <string.h>
#include "codewriter.h"

int main(int argc, char **argv)
{
    // file_paths = []
    // add file(s) to array
    init_out_file("out.asm");

    // for each file:
    //  init file
    //  parse file line by line
    //  for each parsed command, write code to out file
    //  close file

    char *path = argv[1];
    init_file(path);
    char line[18];
    while (has_more_commands())
    {
        advance();
        int cmd_type = command_type();
        char cmd_arg1[MAX_ARG_LENGTH];
        switch (cmd_type)
        {
        case C_ARITHMETIC:
            // arg1 represents operation
            strcpy(cmd_arg1, arg1());
            write_arithmetic(cmd_arg1);
            break;
        case C_POP:
        case C_PUSH:
            // arg1 represents segment
            strcpy(cmd_arg1, arg1());
            // arg2 represents idx
            write_push_pop(cmd_type, cmd_arg1, arg2());
            break;
        default:
            break;
        }
    }

    close_file();
    close_out_file();

    return 0;
}