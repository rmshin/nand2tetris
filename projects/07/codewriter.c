#include <stdio.h>
#include <string.h>
#include "codewriter.h"
#include "parser.h"

FILE *out = NULL;
int bool_count = 0;

void init_out_file(char *path)
{
  out = fopen(path, "w");
};

void close_out_file(void)
{
  fclose(out);
};

void increment_sp()
{
  fputs("@SP\nM=M+1\n", out);
};

void pop_stack_to_d()
{
  fputs("@SP\nAM=M-1\n", out);
  fputs("D=M\n", out);
};

void handle_bool_op(char *op)
{
  pop_stack_to_d();
  fputs("@SP\nA=M-1\n", out);
  // assume true to start
  fputs("D=M-D\nM=-1\n", out);
  // jump label
  char label[16];
  sprintf(label, "@BOOL%d\n", bool_count);
  char jmp_cmd[7];
  sprintf(jmp_cmd, "D;%s\n", op);
  // jump if bool = true
  fputs(label, out);
  fputs(jmp_cmd, out);
  // else set stack[-1] value to 0
  fputs("@SP\nA=M-1\nM=0\n", out);
  sprintf(label, "(BOOL%d)\n", bool_count);
  fputs(label, out);
  bool_count++;
};

void write_arithmetic(char *cmd)
{
  if (strcmp(cmd, "add") == 0)
  {
    pop_stack_to_d();
    fputs("@SP\nA=M-1\n", out);
    // M now represents stack[-1]
    fputs("M=M+D\n", out);
  }
  else if (strcmp(cmd, "sub") == 0)
  {
    pop_stack_to_d();
    fputs("@SP\nA=M-1\n", out);
    fputs("M=M-D\n", out);
  }
  else if (strcmp(cmd, "and") == 0)
  {
    pop_stack_to_d();
    fputs("@SP\nA=M-1\n", out);
    fputs("M=M&D\n", out);
  }
  else if (strcmp(cmd, "or") == 0)
  {
    pop_stack_to_d();
    fputs("@SP\nA=M-1\n", out);
    fputs("M=M|D\n", out);
  }
  else if (strcmp(cmd, "not") == 0)
  {
    fputs("@SP\nA=M-1\n", out);
    fputs("M=!M\n", out);
  }
  else if (strcmp(cmd, "neg") == 0)
  {
    fputs("@SP\nA=M-1\n", out);
    fputs("M=-M\n", out);
  }
  else if (strcmp(cmd, "eq") == 0)
  {
    handle_bool_op("JEQ");
  }
  else if (strcmp(cmd, "gt") == 0)
  {
    handle_bool_op("JGT");
  }
  else if (strcmp(cmd, "lt") == 0)
  {
    handle_bool_op("JLT");
  }
};

// TODO: load seg[idx] address into A reg
void load_seg_to_a(char *seg, int idx){
    // @idx
    // D=A // D <- idx
    // @LCL // R1 (base address local)
    // A=M+D // A <- base_lcl+idx
};

// push seg[idx] to stack
void push_seg_to_stack(char *seg, int idx)
{
  load_seg_to_a(seg, idx);
  // copy seg[idx] value to stack
  fputs("D=M\n@SP\nA=M\nM=D\n", out);
  increment_sp();
};

// pop stack value to seg[idx]
void pop_stack_to_seg(char *seg, int idx)
{
  load_seg_to_a(seg, idx);
  // store seg[idx] address in R13
  fputs("D=A\n@13\nM=D\n", out);
  pop_stack_to_d();
  // load seg[idx] address from R13 and update val with stack value
  fputs("@13\nA=M\nM=D\n", out);
};

// TODO: static, pointer + temp address segments
void write_push_pop(CommandType cmd_type, char *segment, int idx)
{
  switch (cmd_type)
  {
  case C_PUSH:
    if (strcmp(segment, "constant") == 0)
    {
      // load idx into D reg
      char str[16];
      sprintf(str, "@%d\nD=A\n", idx);
      fputs(str, out);
      // copy idx in D to stack
      fputs("@SP\nA=M\nM=D\n", out);
      increment_sp();
    }
    else if (strcmp(segment, "static") == 0)
    {
    }
    else if (strcmp(segment, "argument") == 0)
    {
      push_seg_to_stack("ARG", idx);
    }
    else if (strcmp(segment, "local") == 0)
    {
      push_seg_to_stack("LCL", idx);
    }
    else if (strcmp(segment, "this") == 0)
    {
      push_seg_to_stack("THIS", idx);
    }
    else if (strcmp(segment, "that") == 0)
    {
      push_seg_to_stack("THAT", idx);
    }
    else if (strcmp(segment, "pointer") == 0)
    {
    }
    else if (strcmp(segment, "temp") == 0)
    {
    }
    break;
  case C_POP:
    if (strcmp(segment, "constant") == 0)
    {
    }
    else if (strcmp(segment, "static") == 0)
    {
    }
    else if (strcmp(segment, "argument") == 0)
    {
      pop_stack_to_seg("ARG", idx);
    }
    else if (strcmp(segment, "local") == 0)
    {
      pop_stack_to_seg("LCL", idx);
    }
    else if (strcmp(segment, "this") == 0)
    {
      pop_stack_to_seg("THIS", idx);
    }
    else if (strcmp(segment, "that") == 0)
    {
      pop_stack_to_seg("THAT", idx);
    }
    else if (strcmp(segment, "pointer") == 0)
    {
    }
    else if (strcmp(segment, "temp") == 0)
    {
    }
    break;
  default:
    break;
  }
};
