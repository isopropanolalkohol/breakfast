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