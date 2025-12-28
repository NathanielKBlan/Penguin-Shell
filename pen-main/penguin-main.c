//
// Created by nate on 12/22/25.
//
#include "penguin-main.h"

//returns a set of parsed tokens and the amount of tokens found, right now this is just to parse command args
size_t parse(char ** tokens, char * input, size_t n) {

    parser_state state = TOKEN;

    *(tokens) = malloc(sizeof(char *) * MAX_ARG_LEN);
    memset(*(tokens), 0, sizeof(char *) * MAX_ARG_LEN);

    size_t token_counter = 0;
    int curr_tok_ptr = 0;
    for (int i = 0; i < n; i = i + 1) {

        if (*(input + i) == '\n') {
            break;
        }

        if (*(input + i) == ' ' && state == TOKEN) {
            *(*(tokens + token_counter) + curr_tok_ptr) = '\0';
            token_counter = token_counter + 1;
            *(tokens + token_counter) = malloc(sizeof(char *) * MAX_ARG_LEN);
            memset(*(tokens + token_counter), 0, sizeof(char *) * MAX_ARG_LEN);
            curr_tok_ptr = 0;
        }else if (*(input + i) == '"' && state == TOKEN) {
            state = QUOTE_TOKEN;
        }else if(*(input + i) == '"' && state == QUOTE_TOKEN) {
            state = TOKEN;
        }else{
            *(*(tokens + token_counter) + curr_tok_ptr) = *(input + i);
            curr_tok_ptr = curr_tok_ptr + 1;
        }
    }

    //the plus one is to take into account the first token
    return token_counter + 1;
}

 void (*pen_lookup(char ** args))(char **, history *, size_t arg_count) {
    //algo, first see if first token is less than the first builtin or greater than the last builtin, if not then binary search
    if (strcmp(*args, "cd") >= 0 && strcmp(*args, "xpt") <= 0) {
        int low = 0;
        int high = 4;
        while (low <= high) {
            int mid = low + ((high - low) / 2);
            pen_builtin builtin = pen_builtins[mid];
            int comp_val = strcmp(*args, builtin.command);
            if (comp_val == 0) {
                return builtin.pen_func;
            }

            if (comp_val > 0) {
                low = mid + 1;
            }else {
                high = mid - 1;
            }
        }
    }

     return NULL;
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

void pen_exit(char ** args, history * hist, size_t arg_count) {
    (void)args;
    (void)arg_count;
    clean_up(hist);
    free_tokens(args, arg_count);
    printf("Goodbye (•ᴗ•)ゝ\n");
    exit(0);
}

void pen_pwd(char ** args, history * hist, size_t arg_count) {
    (void)args;
    (void)hist;
    (void)arg_count;
    char cwd[MAX_PATH_LEN] = {0};
    getcwd(cwd, MAX_PATH_LEN);
    printf("%s\n", cwd);
}

void pen_cd(char ** args, history * hist, size_t arg_count) {
    (void)hist;
    (void)arg_count;
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

int clean_up(history * hist) {
    for (int i = 0; i < hist->cells_filled; i++) {
        history_entry * entry = *(hist->entries + i);
        clear_entry(entry);
        free(entry);
    }
    free(hist->entries);
    free(hist);
}

void free_tokens(char ** tokens, size_t arg_count) {
    //free the resources taken by the tokens
    for (int i = 0; i < arg_count; i++) {
        free(*(tokens + i));
    }

    free(tokens);
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
        memset(tokens, 0, sizeof(char *) * TOK_LIM);

        size_t arg_count = parse(tokens, cmmd, strlen(cmmd));

        void (*pen_func)(char ** args, history *, size_t arg_c) = pen_lookup(tokens);

        if (pen_func == NULL) {
            //execute the entered command
            waddle(*(tokens), tokens);
        }else {
            pen_func(tokens, hist, arg_count);
        }

        if (strcmp(*(tokens), "history") != 0) {
            add_to_history(hist, cmmd, *(tokens), tokens, strlen(cmmd), arg_count);
        }

        free_tokens(tokens, arg_count);
    }
}