//
// Created by nate on 12/22/25.
//

#ifndef PENGUIN_PENGUIN_MAIN_H
#define PENGUIN_PENGUIN_MAIN_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include "../antarctic/antarctic_env.h"

//parsing method for commands, results of parse saved in tokens, n represents the length of the input
size_t parse(char ** tokens, char * input, size_t n);

//method that handles execution of the commands
int waddle(char * base_command, char ** args);

//methods to handle build in shell commands
void pen_exit(history * hist);
void pen_pwd();
void pen_cd(char ** args);

//main shell loop commands
void greet();
int clean_up(history * hist);
void free_tokens(char ** tokens, size_t arg_count);
int run();

#endif //PENGUIN_PENGUIN_MAIN_H