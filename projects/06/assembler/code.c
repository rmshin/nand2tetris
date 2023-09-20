#include "code.h"
#include <stdio.h>
#include <string.h>

static char dc[4];
char *dest_code(char *dest_str)
{
    dc[0] = '0';
    dc[1] = '0';
    dc[2] = '0';
    if (dest_str != NULL)
    {
        int idx = 0;
        while (dest_str[idx] != '\0')
        {
            if (dest_str[idx] == 'A')
            {
                dc[0] = '1';
            }
            else if (dest_str[idx] == 'D')
            {
                dc[1] = '1';
            }
            else if (dest_str[idx] == 'M')
            {
                dc[2] = '1';
            }
            idx++;
        }
    }

    return dc;
};

static char cc[8]; // 0 (default)
char *comp_code(char *comp_str)
{
    strcpy(cc, "0101010");
    int clen = strlen(comp_str);

    if (comp_str[0] == 'M' || comp_str[clen - 1] == 'M')
    {
        cc[0] = '1';
    }

    if (clen == 1)
    {
        switch (comp_str[0])
        {
        case '1': // 1
            strcpy(cc + 1, "111111");
            break;
        case 'D': // D
            strcpy(cc + 1, "001100");
            break;
        case 'A': // A
        case 'M': // M
            strcpy(cc + 1, "110000");
            break;
        default:
            break;
        }
    }
    else if (clen == 2)
    {
        switch (comp_str[1])
        {
        case '1': // -1
            strcpy(cc + 1, "111010");
            break;
        case 'D':
            if (comp_str[0] == '!') // !D
            {
                strcpy(cc + 1, "111010");
            }
            else // -D
            {
                strcpy(cc + 1, "001111");
            }
            break;
        case 'A':
        case 'M':
            if (comp_str[0] == '!') // !A || !M
            {
                strcpy(cc + 1, "110001");
            }
            else // -A || -M
            {
                strcpy(cc + 1, "110011");
            }
            break;
        default:
            break;
        }
    }
    else if (clen == 3)
    {
        if (comp_str[1] == '&') // D&A || D&M
        {
            strcpy(cc + 1, "000000");
        }
        else if (comp_str[1] == '|') // D|A || D|M
        {
            strcpy(cc + 1, "010101");
        }
        else if (comp_str[2] == '1')
        {
            if (comp_str[0] == 'D')
            {
                if (comp_str[1] == '+') // D+1
                {
                    strcpy(cc + 1, "011111");
                }
                else // D-1
                {
                    strcpy(cc + 1, "001110");
                }
            }
            else
            {
                if (comp_str[1] == '+') // A+1 || M+1
                {
                    strcpy(cc + 1, "110111");
                }
                else // A-1 || M-1
                {
                    strcpy(cc + 1, "110010");
                }
            }
        }
        else if (comp_str[0] == 'D')
        {
            if (comp_str[1] == '+') // D+A || D+M
            {
                strcpy(cc + 1, "000010");
            }
            else // D-A || D-M
            {
                strcpy(cc + 1, "010011");
            }
        }
        else // A-D
        {
            strcpy(cc + 1, "000111");
        }
    }

    return cc;
};

char jc[4];
char *jump_code(char *jump_str)
{
    // null
    jc[0] = '0';
    jc[1] = '0';
    jc[2] = '0';
    if (jump_str != NULL)
    {
        switch (jump_str[1])
        {
        case 'G':
            jc[2] = '1';            // JGT
            if (jump_str[2] == 'E') // JGE
            {
                jc[1] = '1';
            }
            break;
        case 'L':
            jc[0] = '1';            // JLT
            if (jump_str[2] == 'E') // JLE
            {
                jc[1] = '1';
            }
            break;
        case 'E': // JEQ
            jc[1] = '1';
            break;
        case 'N': // JNE
            jc[0] = '1';
            jc[2] = '1';
            break;
        case 'M': // JMP
            jc[0] = '1';
            jc[1] = '1';
            jc[2] = '1';
            break;
        default:
            break;
        }
    }

    return jc;
};