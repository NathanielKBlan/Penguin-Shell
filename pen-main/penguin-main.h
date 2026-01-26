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
#include <readline/readline.h>
#include <readline/history.h>
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

#define BUILT_INS_HIGH 6
#define BUILT_INS_LOW 0

static struct option pen_options[] = {
    {"help", no_argument, NULL, 'h'}
};

typedef struct {
    char * command;
    void (*pen_func)(char ** args, history *, pen_alias_table *, size_t);
} pen_builtin;

typedef struct {
    char * command;
    char * usage;
} pen_builtin_usage;

//method that handles execution of the commands
int waddle(char * base_command, char ** args);

//methods to handle build in shell commands
void pen_exit(char ** args, history * hist, pen_alias_table * alias_table, size_t arg_count);
void pen_pwd(char ** args, history * hist, pen_alias_table * alias_table, size_t arg_count);
void pen_cd(char ** args, history * hist, pen_alias_table * alias_table, size_t arg_count);

//main shell loop commands
void greet();
int clean_up(history * hist, pen_alias_table * alias_table);
void free_tokens(char ** tokens, size_t arg_count);
int run(int argc, char ** argv);

static pen_builtin pen_builtins[] = {
    {"alias", pen_export},
    {"cd", pen_cd },
    { "chirp", pen_chirp},
    {"exit", pen_exit},
    { "history", pen_print_history},
    { "pwd", pen_pwd },
    {"unalias", pen_unalias},
    { "xpt", pen_export}
};

static pen_builtin_usage pen_builtin_usages[] = {
    {"alias", "alias [alias name]=[alias value], Creates an alias.\n"},
    {"cd", "cd [path], Change directory to path (use * for home directory).\n"  },
    { "chirp", "chirp [environment variable], Outputs out the value of the specified environment variable.\n"},
    {"exit", "exit, Closes the shell.\n"},
    { "history", "history, Outputs command history throughout the shell's runtime up to a max of 128 commands (latest commands).\n"},
    { "pwd", "pwd, Outputs the current working directory.\n" },
    {"unalias", "unalias [alias name], deletes the specified alias.\n"},
    { "xpt", "xpt [variable name]=[value], Sets a new environment variable with the specified value.\n"}
};

void (*pen_lookup(char ** args))(char **, history *, pen_alias_table *, size_t);
#endif //PENGUIN_PENGUIN_MAIN_H