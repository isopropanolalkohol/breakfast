//
// Created by joshua on 25.06.25.
//

#include "../include/interpreter.h"
#include <stdlib.h>

struct BF_RUN_ENVIR* BF_start_env(const char *filename)
{
    struct BF_RUN_ENVIR *env = malloc(sizeof(struct BF_RUN_ENVIR));
    env->exec_pointer = NULL;
    env->head_cell = NULL;
    env->file_pointer = fopen(filename, "r");
    if (env->file_pointer == NULL)
    {
        free(env);
        return NULL;
    }
    return env;
}

void BF_end_env(struct BF_RUN_ENVIR *env)
{
    if (env->file_pointer != NULL)
    {
        fclose(env->file_pointer);
    }
    struct BF_CELL* current = env->head_cell;
    struct BF_CELL* prev_cell = current->prev;
    while (current != NULL)
    {
        struct BF_CELL* temp = current->next;
        free(current);
        current = temp;
    }
    while (prev_cell != NULL)
    {
        struct BF_CELL* temp = prev_cell->prev;
        free(prev_cell);
        prev_cell = temp;
    }
    free(env);
}
void BF_clear_stack_frame(struct BF_STACKFRAME *frame)
{
    struct BF_STACKFRAME *current = frame;
    while (current != NULL)
    {
        struct BF_STACKFRAME *temp = current->next;
        free(current);
        current = temp;
    }
}

// here are our functions used for brainfuck syntax
void BF_increment_pointer(struct BF_RUN_ENVIR *env)
{
    if (env->exec_pointer->next != NULL)
    {
        env->exec_pointer = env->exec_pointer->next;
        return;
    }
    env->exec_pointer->next = malloc(sizeof(struct BF_RUN_ENVIR));
    env->exec_pointer->val = 0;
    env->exec_pointer->next->prev = env->exec_pointer;
    env->exec_pointer = env->exec_pointer->next;
}
void BF_decrement_pointer(struct BF_RUN_ENVIR *env)
{
    if (env->exec_pointer->prev != NULL)
    {
        env->exec_pointer = env->exec_pointer->prev;
        return;
    }
    env->exec_pointer->prev = malloc(sizeof(struct BF_RUN_ENVIR));
    env->exec_pointer->val = 0;
    env->exec_pointer->prev->next = env->exec_pointer;
    env->exec_pointer = env->exec_pointer->prev;
}
void BF_increment_cell(struct BF_RUN_ENVIR *env)
{
    env->exec_pointer->val++;
}
void BF_decrement_cell(struct BF_RUN_ENVIR *env)
{
    env->exec_pointer->val--;
}
void BF_stdout_curr_cell(struct BF_RUN_ENVIR *env)
{
    fprintf(stdout, "%c", (char)env->exec_pointer->val);
}
void BF_stdin_curr_cell(struct BF_RUN_ENVIR *env)
{
    char readin = getchar();
    env->exec_pointer->val = (__uint8_t)readin;
}
void BF_start_loop(struct BF_RUN_ENVIR *env, struct BF_STACKFRAME *frame)
{
    rewind(env->file_pointer);
    struct BF_STACKFRAME *frame2 = malloc(sizeof(struct BF_STACKFRAME));
    frame2->prev = frame;
    frame->next = frame2;
    fgetpos(env->file_pointer, &frame2->frame_position);
    fgetc(env->file_pointer);
    char current_char = fgetc(env->file_pointer);
    while (1)
    {
        switch (current_char)
        {
            case '>':
                BF_increment_pointer(env);
                break;
            case '<':
                BF_decrement_pointer(env);
                break;
            case '+':
                BF_increment_cell(env);
                break;
            case '-':
                BF_decrement_cell(env);
                break;
            case '.':
                BF_stdout_curr_cell(env);
                break;
            case ',':
                BF_stdin_curr_cell(env);
                break;
            case '[':
                if (env->exec_pointer->val == 0)
                {
                    // forward to end of the loop
                    int brace_count = 1;
                    while (brace_count != 0)
                    {
                        char new_char = fgetc(env->file_pointer);
                        if (new_char == '[') brace_count++;
                        else if (new_char == ']') brace_count--;
                    }
                }
                else BF_start_loop(env, frame2);
                break;
            case ']':
                frame->next = NULL;
                fsetpos(env->file_pointer, &frame2->frame_position);
                BF_clear_stack_frame(frame2);
                return;
            case EOF:
                BF_clear_stack_frame(frame);
                return;
            default:
                break;
        }
    }
}
// the loop handling will be a headache
void BF_run_program(struct BF_RUN_ENVIR *env)
{
    struct BF_STACKFRAME* frame = malloc(sizeof(struct BF_STACKFRAME));
    frame->next = NULL;
    frame->prev = NULL;
    fpos_t framepos;
    fgetpos(env->file_pointer, &framepos);
    frame->frame_position = framepos;

    while (1)
    {
        char current_char = fgetc(env->file_pointer);
        switch (current_char)
        {
            case '>':
                BF_increment_pointer(env);
                break;
            case '<':
                BF_decrement_pointer(env);
                break;
            case '+':
                BF_increment_cell(env);
                break;
            case '-':
                BF_decrement_cell(env);
                break;
            case '.':
                BF_stdout_curr_cell(env);
                break;
            case ',':
                BF_stdin_curr_cell(env);
                break;
            case '[':
                BF_start_loop(env, frame);
                break;
            case ']':
                fprintf(stderr, "Encountered unwell formed parentheses");
                BF_clear_stack_frame(frame);
                return;
            case EOF:
                BF_clear_stack_frame(frame);
                return;
            default:
                break;
        }
    }
}