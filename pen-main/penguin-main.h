//
// Created by nate on 12/22/25.
//

#ifndef PENGUINC_PENGUIN_MAIN_H
#define PENGUINC_PENGUIN_MAIN_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

//common constants used by penguin go here
#define MAX_CMMD_LEN 65536
#define MAX_PATH_LEN 262144
#define TOK_LIM 1024

//parsing method for commands, results of parse saved in tokens, n represents the length of the input
void parse(char ** tokens, char * input, size_t n);

//method that handles execution of the commands
int waddle(char * base_command, char ** args);

//methods to handle build in shell commands
void pen_exit();
void pen_pwd();
void pen_cd(char ** args);

//main shell loop commands
void greet();
int run();

#endif //PENGUINC_PENGUIN_MAIN_H