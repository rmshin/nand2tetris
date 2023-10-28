#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "codewriter.h"

int is_directory(const char *path)
{
    struct stat path_stat;
    if (stat(path, &path_stat) != 0)
        return 0;
    return S_ISDIR(path_stat.st_mode);
}

int process_dir_or_file(char *path, char paths_arr[128][2048])
{
    int count = 0;
    DIR *dir;
    struct dirent *de;
    dir = opendir(path);
    if (dir != NULL)
    {
        char full_path[1024];
        while ((de = readdir(dir)) != NULL)
        {
            // only process files
            if (de->d_type == DT_REG)
            {
                full_path[0] = '\0';
                strcat(full_path, path);
                strcat(full_path, "/");
                strcat(full_path, de->d_name);
                strcpy(paths_arr[count], full_path);
                count++;
            }
        }
        closedir(dir);
    }
    else
    {
        strcpy(paths_arr[0], path);
        count++;
    }
    return count;
};

void get_out_filename(char *path, char *fname)
{
    // NOTE: this doesn't handle trailing slashes in path
    char *start = strrchr(path, '/');
    start = (start == NULL) ? path : start + 1;

    char *end = strrchr(path, '.');
    // HACK: ignore leading period, breaks if more than one
    if (end == NULL || end == path)
    {
        end = &path[strlen(path)];
    }
    strncpy(fname, start, end - start);
    fname[end - start] = '\0';
};

int main(int argc, char **argv)
{
    char *path = argv[1];
    char out_fname[MAX_FNAME_LENGTH];
    get_out_filename(path, out_fname);
    strcat(out_fname, ".asm");
    init_out_file(out_fname);

    // process file path(s)
    char fpaths[128][2048];
    int num_files = process_dir_or_file(path, fpaths);

    for (int i = 0; i < num_files; i++)
    {
        init_file(fpaths[i]);
        char ext[16];
        strcpy(ext, get_fextension());
        if (strcmp(ext, ".vm") != 0)
        {
            continue;
        }
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
    }
    close_out_file();

    return 0;
}