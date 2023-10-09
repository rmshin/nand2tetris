#include "parser.h"
#include <stdio.h>
#include <string.h>

static FILE *file = NULL;
static char curr_cmd[MAX_CMD_LENGTH] = {'\0'};
static char next_cmd[MAX_CMD_LENGTH] = {'\0'};

void init_file(char *path)
{
    file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file");
        return;
    }
};

void close_file(void)
{
    fclose(file);
};

void get_next_command(void)
{
    char line[256], stripped[256];
    bool not_eof;
    while ((not_eof = fgets(line, sizeof line, file) != NULL))
    {
        // vars for capturing command values
        char cmd[MAX_CMD_LENGTH];
        int match_count;

        match_count = sscanf(line, " %s", cmd);
        // skip if empty line
        if (match_count < 1)
        {
            continue;
        }
        else
        {
        }
        // strip comments
        char *result = strstr(stripped, "//");
        if (result != NULL)
        {
            // if entire line is commented, read next line
            if (result - stripped == 0)
            {
                continue;
            }
            else
            {
                stripped[result - stripped] = '\0';
            }
        }
        break;
    }

    // 5. if EOF, set next_cmd=NULL and return
    if (!not_eof)
    {
        next_cmd[0] = '\0';
        return;
    }

    // TODO: implement syntax validation
    // validate_syntax(stripped);
    strcpy(next_cmd, stripped);
    return;
};

bool has_more_commands(void)
{
    get_next_command();
    if (next_cmd[0] == '\0')
    {
        return false;
    }
    return true;
};

void advance(void){

};

int str_command_type(char *cmd)
{
    if (strcmp(cmd, "add") == 0 ||
        strcmp(cmd, "sub") == 0 ||
        strcmp(cmd, "neg") == 0 ||
        strcmp(cmd, "eq") == 0 ||
        strcmp(cmd, "gt") == 0 ||
        strcmp(cmd, "lt") == 0 ||
        strcmp(cmd, "and") == 0 ||
        strcmp(cmd, "or") == 0 ||
        strcmp(cmd, "not") == 0)
    {
        return C_ARITHMETIC;
    }
    else if (strcmp(cmd, "pop") == 0)
    {
        return C_POP;
    }
    else if (strcmp(cmd, "push") == 0)
    {
        return C_PUSH;
    }
    else if (strcmp(cmd, "label") == 0)
    {
        return C_LABEL;
    }
    else if (strcmp(cmd, "goto") == 0)
    {
        return C_GOTO;
    }
    else if (strcmp(cmd, "if-goto") == 0)
    {
        return C_IF;
    }
    else if (strcmp(cmd, "function") == 0)
    {
        return C_FUNCTION;
    }
    else if (strcmp(cmd, "call") == 0)
    {
        return C_CALL;
    }
    else if (strcmp(cmd, "return") == 0)
    {
        return C_RETURN;
    }
    else
    {
        return -1;
    }
};

int command_type(void)
{
    str_command_type(curr_cmd);
};

char *arg1(void){

};

char *arg2(void){

};
