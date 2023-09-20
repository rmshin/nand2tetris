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

void rewind_file(void)
{
    rewind(file);
    curr_cmd[0] = '\0';
    next_cmd[0] = '\0';
}

void close_file(void)
{
    fclose(file);
}

bool validate_syntax(char *cmd)
{
    // TODO
    return true;
};

void get_next_command(void)
{
    char line[256], stripped[256];
    bool not_eof;
    while ((not_eof = fgets(line, sizeof line, file) != NULL))
    {
        // strip spaces
        int li = 0, si = 0;
        while (line[li] != '\n' && line[li] != '\r' && line[li] != EOF)
        {
            if (line[li] != ' ')
            {
                stripped[si] = line[li];
                si++;
            }
            li++;
        }
        stripped[si] = '\0';

        // if empty line, read next line.
        if (stripped[0] == '\0')
            continue;

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

void advance(void)
{
    strcpy(curr_cmd, next_cmd);
};

int command_type(void)
{
    if (curr_cmd[0] == '\0')
    {
        fprintf(stderr, "Cannot return type of null command");
        return -1;
    }

    switch (curr_cmd[0])
    {
    case '@':
        return A_COMMAND;
    case '(':
        return L_COMMAND;
    default:
        return C_COMMAND;
    }
};

static char sym[MAX_SYMBOL_LEN];
char *symbol(void)
{
    int clen = strlen(curr_cmd);
    int curr_cmd_type = command_type();
    switch (curr_cmd_type)
    {
    case A_COMMAND:
        strncpy(sym, curr_cmd + 1, clen - 1);
        sym[clen - 1] = '\0';
        break;
    case L_COMMAND:
        strncpy(sym, curr_cmd + 1, clen - 2);
        sym[clen - 2] = '\0';
        break;
    default:
        fprintf(stderr, "Can only fetch symbol for A or L commands");
        return NULL;
    }

    return sym;
};

static char d[4];
char *dest(void)
{
    int curr_cmd_type = command_type();
    if (curr_cmd_type != C_COMMAND)
    {
        fprintf(stderr, "Dest only exists for C commands");
        return NULL;
    }
    char *result = strchr(curr_cmd, '=');
    if (result == NULL)
        return NULL;
    int idx = 0;
    while (idx < result - curr_cmd)
    {
        d[idx] = curr_cmd[idx];
        idx++;
    }
    d[idx] = '\0';

    return d;
};

static char c[8];
char *comp(void)
{
    int curr_cmd_type = command_type();
    if (curr_cmd_type != C_COMMAND)
    {
        fprintf(stderr, "Comp only exists for C commands");
        return NULL;
    }

    char *result = strchr(curr_cmd, '=');
    if (result == NULL)
    {
        result = curr_cmd;
    }
    else
    {
        result++;
    }
    int idx = 0;
    while (*result != ';' && *result != '\0')
    {
        c[idx++] = *result++;
    }
    c[idx] = '\0';

    return c;
};

static char j[4];
char *jump(void)
{
    int curr_cmd_type = command_type();
    if (curr_cmd_type != C_COMMAND)
    {
        fprintf(stderr, "Jump only exists for C commands");
        return NULL;
    }

    char *result = strchr(curr_cmd, ';');
    if (result == NULL)
        return NULL;

    j[0] = *++result;
    j[1] = *++result;
    j[2] = *++result;
    j[3] = '\0';
    return j;
};
