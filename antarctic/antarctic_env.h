//
// Created by nate on 12/24/25.
//

//this file will be used to manage the underlying environment of the penguin shell

#ifndef PENGUIN_ANTARCTIC_ENV_H
#define PENGUIN_ANTARCTIC_ENV_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define VAR_NAME_LIM 256
#define VAR_VAL_LIM 1024
#define HISTORY_LIM 128

typedef struct {
    char * full_cmmd;
    char * command;
    char ** args;
    int arg_count;
} history_entry ;

typedef struct {
    int next_empty;
    int cells_filled;
    history_entry ** entries;
} history;

//environment manipulation library
int export(char * new_var);

void print_antarctic_vars();

//history management library
history * init_history();

int add_to_history(history * hist, char * full_cmmd, char * command, char ** args, size_t command_len, size_t arg_count);

void print_history(history * hist);

#endif //PENGUIN_ANTARCTIC_ENV_H