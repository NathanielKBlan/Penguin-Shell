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
#include <getopt.h>
#include "../antarctic/antarctic_env.h"
#include "pen_lan.h"

#define USAGE   \
    "The penguin shell (•ᴗ•)ゝ\n" \
    "Run the shell by calling the penguin executable, be sure to set penguin in your path to call from any dir. \n"\
    "basic commands:\n"\
    "  cd [path]                             Change directory to path (use * for home directory).\n" \
    "  chirp [environment variable]          Outputs out the value of the specified environment variable.\n" \
    "  exit                                  Closes the shell.\n"\
    "  history                               Outputs command history throughout the shell's runtime up to a max of 128 commands (latest commands).\n" \
    "  pwd                                   Outputs the current working directory.\n" \
    "  xpt [variable name]=[value]           Sets a new environment variable with the specificied value.\n" \

static struct option pen_options[] = {
    {"help", no_argument, NULL, 'h'}
};

typedef struct {
    char * command;
    void (*pen_func)(char ** args, history *, size_t arg_count);
} pen_builtin;

//method that handles execution of the commands
int waddle(char * base_command, char ** args);

//methods to handle build in shell commands
void pen_exit(char ** args, history * hist, size_t arg_count);
void pen_pwd(char ** args, history * hist, size_t arg_count);
void pen_cd(char ** args, history * hist, size_t arg_count);

//main shell loop commands
void greet();
int clean_up(history * hist);
void free_tokens(char ** tokens, size_t arg_count);
int run(int argc, char ** argv);

static pen_builtin pen_builtins[] = {
    {"cd", pen_cd },
    { "chirp", pen_chirp},
    {"exit", pen_exit},
    { "history", pen_print_history},
    { "pwd", pen_pwd },
    { "xpt", pen_export}
};

void (*pen_lookup(char ** args))(char **, history *, size_t arg_count);

#endif //PENGUIN_PENGUIN_MAIN_H