#include "parser.h"
#include <stdio.h>
#include <string.h>

static FILE *file = NULL;
static char filename[MAX_FNAME_LENGTH];
static char f_extension[16] = {'\0'};

static char curr_cmd[MAX_CMD_LENGTH] = {'\0'};
static char next_cmd[MAX_CMD_LENGTH] = {'\0'};
static char curr_arg1[MAX_ARG_LENGTH] = {'\0'};
static int curr_arg2 = 0;

void init_file(char *path)
{
    file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file");
        return;
    }
    // process filename for use in codewriter.c
    char *start = strrchr(path, '/');
    start = (start == NULL) ? path : start + 1;

    char *end = strrchr(path, '.');
    if (end == NULL)
    {
        end = &path[strlen(path) - 1];
    }
    else
    {
        strcpy(f_extension, end);
    }
    strncpy(filename, start, end - start);
    filename[end - start] = '\0';
};

void close_file(void)
{
    fclose(file);
};

char *get_filename(void)
{
    return filename;
};

char *get_fextension(void)
{
    return f_extension;
};

void get_next_command(void)
{
    char line[256];
    bool not_eof;
    while ((not_eof = fgets(line, sizeof line, file) != NULL))
    {
        // vars for capturing command values
        char cmd[MAX_CMD_LENGTH],
            arg1[MAX_ARG_LENGTH],
            arg2[MAX_ARG_LENGTH];
        int match_count;

        match_count = sscanf(line, " %s", cmd);
        // skip if empty line
        if (match_count < 1)
        {
            continue;
        }

        // strip comments
        char *result = strstr(line, "//");
        if (result != NULL)
        {
            // if entire line is commented, read next line
            if (result - line == 0)
            {
                continue;
            }
            else
            {
                line[result - line] = '\0';
            }
        }

        match_count = sscanf(line, "%s %s %s", cmd, arg1, arg2);
        if (match_count == 1) // arithmetic
        {
            strcpy(line, cmd);
        }
        else if (match_count == 3) // push-pop
        {
            strcpy(line, cmd);
            strcat(line, " ");
            strcat(line, arg1);
            strcat(line, " ");
            strcat(line, arg2);
        }
        else
        {
            // unknown command, skip
            continue;
        }

        break;
    }

    // if EOF, set next_cmd=NULL and return
    if (!not_eof)
    {
        next_cmd[0] = '\0';
        return;
    }

    // TODO: implement syntax validation
    // validate_syntax(stripped);
    strcpy(next_cmd, line);
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

void advance(void)
{
    strcpy(curr_cmd, next_cmd);
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
    char cmd[MAX_ARG_LENGTH];
    sscanf(curr_cmd, "%s", cmd);
    return str_command_type(cmd);
};

char *arg1(void)
{
    int cmd_type = command_type();
    if (cmd_type == C_ARITHMETIC)
    {
        return curr_cmd;
    }
    else if (cmd_type == C_RETURN || curr_cmd[0] == '\0')
    {
        curr_arg1[0] = '\0'; // reset arg1
        return NULL;
    }
    else
    {
        char arg[MAX_ARG_LENGTH];
        int match_count;
        match_count = sscanf(curr_cmd, "%*s %s", arg);
        if (match_count == 1)
        {
            strcpy(curr_arg1, arg);
            return curr_arg1;
        }
        else
        {
            curr_arg1[0] = '\0'; // reset arg1
            return NULL;
        }
    }
};

int arg2(void)
{
    int cmd_type = command_type();
    if (!(cmd_type == C_PUSH ||
          cmd_type == C_POP ||
          cmd_type == C_CALL ||
          cmd_type == C_FUNCTION))
    {
        curr_arg2 = 0; // reset arg2
    }
    else
    {
        int match_count;
        match_count = sscanf(curr_cmd, "%*s %*s %d", &curr_arg2);
        if (match_count < 1)
        {
            curr_arg2 = 0; // reset arg2
        }
    }
    return curr_arg2;
};
