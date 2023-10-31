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
  // load stack[-1] into A reg & decrement sp
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

// load segment[idx] address into A reg
void load_seg_to_a(char *seg, int idx)
{
  char str[16];
  if (idx == 0)
  {
    sprintf(str, "@%s\nA=M\n", seg);
    fputs(str, out);
  }
  else if (idx == 1)
  {
    sprintf(str, "@%s\nA=M+1\n", seg);
    fputs(str, out);
  }
  else
  {
    // load idx into D reg
    sprintf(str, "@%d\nD=A\n", idx);
    fputs(str, out);
    // load seg[idx] into A reg
    sprintf(str, "@%s\nA=M+D\n", seg);
    fputs(str, out);
  }
};

// push seg[idx] to stack
void push_seg_to_stack(char *seg, int idx)
{
  load_seg_to_a(seg, idx);
  // push seg[idx] value in D to stack
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
  // load seg[idx] address from R13 and update val with popped stack value
  fputs("@13\nA=M\nM=D\n", out);
};

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
      char str[MAX_FNAME_LENGTH + 8];
      sprintf(str, "@%s.%d\n", get_filename(), idx);
      fputs(str, out);
      fputs("D=M\n@SP\nA=M\nM=D\n", out);
      increment_sp();
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
      // pointer segment maps to R3 & R4
      if (idx == 0)
      {
        fputs("@3\n", out);
      }
      else if (idx == 1)
      {
        fputs("@4\n", out);
      }
      // same as other segments
      fputs("D=M\n@SP\nA=M\nM=D\n", out);
      increment_sp();
    }
    else if (strcmp(segment, "temp") == 0)
    {
      // temp segment maps to R5 - R12
      char str[5];
      sprintf(str, "@%d\n", idx + 5);
      fputs(str, out);
      // same as other segments
      fputs("D=M\n@SP\nA=M\nM=D\n", out);
      increment_sp();
    }
    break;
  case C_POP:
    if (strcmp(segment, "constant") == 0)
    {
      // invalid scenario
    }
    else if (strcmp(segment, "static") == 0)
    {
      char str[MAX_FNAME_LENGTH + 8];
      sprintf(str, "@%s.%d\n", get_filename(), idx);
      fputs(str, out);
      fputs("D=A\n@13\nM=D\n", out);
      pop_stack_to_d();
      fputs("@13\nA=M\nM=D\n", out);
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
      // pointer segment maps to R3 & R4
      if (idx == 0)
      {
        fputs("@3\n", out);
      }
      else if (idx == 1)
      {
        fputs("@4\n", out);
      }
      // same as other segments
      fputs("D=A\n@13\nM=D\n", out);
      pop_stack_to_d();
      fputs("@13\nA=M\nM=D\n", out);
    }
    else if (strcmp(segment, "temp") == 0)
    {
      // temp segment maps to R5 - R12
      char str[5];
      sprintf(str, "@%d\n", idx + 5);
      fputs(str, out);
      // same as other segments
      fputs("D=A\n@13\nM=D\n", out);
      pop_stack_to_d();
      fputs("@13\nA=M\nM=D\n", out);
    }
    break;
  default:
    break;
  }
};

void write_init(void)
{
  // init sp to RAM[256]
  fputs("@256\nD=A\n@SP\nM=D\n", out);
  write_call("Sys.init", 0);
};

void write_label(char *label)
{
  char str[MAX_ARG_LENGTH * 2 + 3];
  sprintf(str, "(%s)\n", label);
  fputs(str, out);
};

void write_goto(char *label)
{
  char str[MAX_ARG_LENGTH * 2 + 8];
  sprintf(str, "@%s\n0;JMP\n", label);
  fputs(str, out);
};

void write_if(char *label)
{
  pop_stack_to_d();
  char str[MAX_ARG_LENGTH * 2 + 8];
  sprintf(str, "@%s\nD;JNE\n", label);
  fputs(str, out);
};

int func_count = 0;
void write_call(char *func_name, int num_args)
{
  char return_label[16];
  // push return address to stack
  sprintf(return_label, "@RETURN%d\nD=A\n", func_count);
  fputs(return_label, out);
  fputs("@SP\nA=M\nM=D\n", out);
  increment_sp();
  fputs("@LCL\nD=M\n@SP\nA=M\nM=D\n", out);
  increment_sp();
  fputs("@ARG\nD=M\n@SP\nA=M\nM=D\n", out);
  increment_sp();
  fputs("@THIS\nD=M\n@SP\nA=M\nM=D\n", out);
  increment_sp();
  fputs("@THAT\nD=M\n@SP\nA=M\nM=D\n", out);
  increment_sp();
  // reposition ARG (sp - (n + 5))
  char str[16];
  sprintf(str, "@%d\nD=A\n", num_args + 5); // load offset into D
  fputs(str, out);
  fputs("@SP\nD=M-D\n@ARG\nM=D\n", out);
  // reposition LCL (sp)
  fputs("@SP\nD=M\n@LCL\nM=D\n", out);
  // jump to function code
  char func_str[MAX_ARG_LENGTH + 8];
  sprintf(func_str, "@%s\n0;JMP\n", func_name);
  fputs(func_str, out);
  // declare return point
  sprintf(return_label, "(RETURN%d)\n", func_count);
  fputs(return_label, out);

  func_count++;
};

void write_return(void)
{
  // store LCL temporarily in R14
  fputs("@LCL\nD=M\n@14\nM=D\n", out);
  // store return address (LCL - 5) temporarily in R15
  fputs("@5\nA=D-A\nD=M\n@15\nM=D\n", out);
  // reposition return value for caller
  pop_stack_to_seg("ARG", 0);
  // reposition pointers
  fputs("@ARG\nD=M+1\n@SP\nM=D\n", out);        // SP (ARG + 1)
  fputs("@14\nAM=M-1\nD=M\n@THAT\nM=D\n", out); // THAT (LCL - 1)
  fputs("@14\nAM=M-1\nD=M\n@THIS\nM=D\n", out); // THIS (LCL - 2)
  fputs("@14\nAM=M-1\nD=M\n@ARG\nM=D\n", out);  // ARG (LCL - 3)
  fputs("@14\nAM=M-1\nD=M\n@LCL\nM=D\n", out);  // LCL (LCL - 4)
  // jump back to caller return point
  fputs("@15\nA=M\n0;JMP\n", out);
};

void write_function(char *func_name, int num_locals)
{
  char str[MAX_ARG_LENGTH + 3];
  sprintf(str, "(%s)\n", func_name);
  fputs(str, out);
  // initialise all local vars to 0
  for (int i = 0; i < num_locals; i++)
  {
    write_push_pop(C_PUSH, "constant", 0);
  }
};
