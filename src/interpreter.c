//
// Created by joshua on 25.06.25.
//

#include "../include/interpreter.h"
#include <stdlib.h>

struct BF_RUN_ENVIR* BF_start_env(const char *filename)
{
    struct BF_RUN_ENVIR *env = malloc(sizeof(struct BF_RUN_ENVIR));

    return env;
}