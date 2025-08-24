//
// Created by joshua on 25.06.25.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>

typedef struct BF_CELL
{
    __uint8_t val;
    struct BF_CELL* next;
    struct BF_CELL* prev;
} Cell;
typedef struct BF_RUN_ENVIR
{
    FILE *file_pointer;
    struct BF_CELL* head_cell;
    struct BF_CELL* exec_pointer;
} Environment;

struct BF_RUN_ENVIR* BF_start_env(const char *filename);
// to free all the shit
void BF_end_env(struct BF_RUN_ENVIR *env);

void BF_run_program(struct BF_RUN_ENVIR *env);

void BF_parse_char(struct BF_RUN_ENVIR *env, char c);

void BF_increment_pointer(struct BF_RUN_ENVIR *env); // >
void BF_decrement_pointer(struct BF_RUN_ENVIR *env); // <
void BF_increment_cell(struct BF_RUN_ENVIR *env); // +
void BF_decrement_cell(struct BF_RUN_ENVIR *env); // -
void BF_stdout_curr_cell(struct BF_RUN_ENVIR *env); // .
void BF_stdin_curr_cell(struct BF_RUN_ENVIR *env); // ,
// loop handlign will be in the funtion used for running




#endif //INTERPRETER_H
