//
// Created by nate on 12/22/25.
//
#include "penguin-main.h"
#include "../antarctic/antarctic_env.h"

//returns a set of parsed tokens and the amount of tokens found, right now this is just to parse command args
//TODO handle case of quotes
size_t parse(char ** tokens, char * input, size_t n) {

    *(tokens) = malloc(sizeof(char *) * MAX_ARG_LEN);
    size_t token_counter = 0;
    int curr_tok_ptr = 0;
    for (int i = 0; i < n; i = i + 1) {

        if (*(input + i) == '\n') {
            break;
        }

        if (*(input + i) == ' ') {
            *(*(tokens + token_counter) + curr_tok_ptr) = '\0';
            token_counter = token_counter + 1;
            *(tokens + token_counter) = malloc(sizeof(char *) * MAX_ARG_LEN);
            curr_tok_ptr = 0;
        }else{
            *(*(tokens + token_counter) + curr_tok_ptr) = *(input + i);
            curr_tok_ptr = curr_tok_ptr + 1;
        }
    }

    //the plus one is to take into account the first token
    return token_counter + 1;
}

int waddle(char * base_command, char ** args) {

    pid_t pid;
    int child_status;

    pid = fork();

    //child executes here
    if (pid < 0) {
        printf("Error executing child! errno: %d\n", errno);
        exit(-1);
    }

    if (pid == 0) {
        //execvp call here
        execvp(base_command, args);
        exit(0);
    }

    wait(&child_status);

}

void pen_exit() {
    printf("Goodbye (•ᴗ•)ゝ\n");
    exit(0);
}

void pen_pwd() {
    char cwd[MAX_PATH_LEN] = {0};
    getcwd(cwd, MAX_PATH_LEN);
    printf("%s\n", cwd);
}

void pen_cd(char ** args) {
    int cd_res = -1;
    if (*(args + 1) == NULL || strcmp(*(args + 1), "*") == 0) {
        char * home_dir = getenv("HOME");
        cd_res = chdir(home_dir);
        if (cd_res == -1) {
            exit(-1);
        }
    }else {
        cd_res = chdir(*(args + 1));
        if (cd_res == -1) {
            printf("Could not find or no such directory exists. :(\n");
        }
    }

}

void greet() {
    printf("===========================\n");
    printf("    P  E  N  G  U  I  N    \n");
    printf("===========================\n");
}

int run() {

    greet();

    history * hist = init_history();

    char cwd[MAX_PATH_LEN] = {0};
    char cmmd[MAX_CMMD_LEN] = {0};

    while (1) {

        //get the current working directory
        getcwd(cwd, MAX_PATH_LEN);

        printf("%s (•ᴗ•)ゝ ", cwd);

        //get user input
        fgets(cmmd, MAX_CMMD_LEN, stdin);

        //tokenize the command and fetch the args as well
        char ** tokens = malloc(sizeof(char *) * TOK_LIM);
        size_t arg_count = parse(tokens, cmmd, strlen(cmmd));

        if (strcmp(*(tokens), "exit") == 0) {
            pen_exit();
        }else if (strcmp(*(tokens), "pwd") == 0) {
            pen_pwd();
        }else if (strcmp(*(tokens), "cd") == 0) {
            pen_cd(tokens);
        }else if (strcmp(*(tokens), "xpt") == 0 || strcasecmp(*(tokens), "export") == 0) {
            export(*(tokens + 1));
        }else if (strcmp(*(tokens), "history") == 0) {
            print_history(hist);
        }
        else {
            //execute the entered command
            waddle(*(tokens), tokens);
        }

        if (strcmp(*(tokens), "history") != 0) {
            add_to_history(hist, cmmd, *(tokens), tokens, strlen(cmmd), arg_count);
        }

    }
}