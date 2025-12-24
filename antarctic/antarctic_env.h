//
// Created by nate on 12/24/25.
//

//this file will be used to manage the underlying environment of the penguin shell

#ifndef PENGUINC_ANTARCTIC_ENV_H
#define PENGUINC_ANTARCTIC_ENV_H

#include <string.h>
#include <stdlib.h>

#define VAR_NAME_LIM 256
#define VAR_VAL_LIM 1024
#define HISTORY_LIM 256

typedef struct {
    char * command;
    char ** args;
} history_entry ;

typedef struct {
    int next_empty;
    history_entry ** entries;
} history;

//environment manipulation library
int export(char * new_var);

void print_antarctic_vars();

//history management library
history * init_history();

int add_to_history(history * hist, char * command, char ** args);

#endif //PENGUINC_ANTARCTIC_ENV_H