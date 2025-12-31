//
// Created by nate on 12/22/25.
//
#include "penguin-main.h"

 void (*pen_lookup(char ** args))(char **, history *, size_t) {
    //algo, first see if first token is less than the first builtin or greater than the last builtin, if not then binary search
    if (strcmp(*args, "cd") >= 0 && strcmp(*args, "xpt") <= 0) {
        int low = 0;
        int high = 5;
        while (low <= high) {
            int mid = low + ((high - low) / 2);
            pen_builtin builtin = pen_builtins[mid];
            int comp_val = strcmp(*args, builtin.command);
            if (comp_val == 0) {
                if (*(args + 1) != NULL && (strcmp(*(args + 1), "--help") == 0 || strcmp(*(args + 1), "-h") == 0)) {
                    fprintf(stdout, "%s", pen_builtin_usages[mid].usage);
                }
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
            printf("Could not find directory or no such directory exists. :(\n");
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

int run(int argc, char ** argv) {

     int option_char = 0;
     while (((option_char) = getopt_long(argc, argv, "h::", pen_options, NULL)) != -1) {
         switch (option_char) {
             case 'h':
                 fprintf(stdout, "%s", USAGE);
                 exit(0);
                 break;
             default:
                 fprintf(stdout, "%s", USAGE);
                 exit(1);
         }
     }

    greet();

    history * hist = init_history();

    char cwd[MAX_PATH_LEN] = {0};

    char * cmmd;

    while (1) {

        //get the current working directory
        getcwd(cwd, MAX_PATH_LEN);

        char prompt[MAX_PATH_LEN + 16];

        snprintf(prompt, MAX_PATH_LEN + 16, "%s (•ᴗ•)ゝ ", cwd);

        //get user input
        if ((cmmd = readline(prompt)) != NULL) {

            //tokenize the command and fetch the args as well
            char ** tokens = malloc(sizeof(char *) * TOK_LIM);
            memset(tokens, 0, sizeof(char *) * TOK_LIM);

            size_t arg_count = tokenize(tokens, cmmd, strlen(cmmd));

            int hist_comp = strcmp(*(tokens), "history");
            if (hist_comp != 0) {
                add_to_history(hist, cmmd, *(tokens), tokens, strlen(cmmd), arg_count);
            }

            if (cmmd && *cmmd && hist_comp != 0) {
                add_history(cmmd);
            }

            free(cmmd);

            void (*pen_func)(char ** args, history *, size_t arg_c) = pen_lookup(tokens);

            if (pen_func == NULL) {
                //execute the entered command
                waddle(*(tokens), tokens);
            }else {
                if (*(tokens + 1) != NULL && strcmp(*(tokens + 1), "-h") != 0 && strcmp(*(tokens + 1), "--help") != 0) {
                    pen_func(tokens, hist, arg_count);
                } else if (*(tokens + 1) == NULL) {
                    pen_func(tokens, hist, arg_count);
                }
            }

            free_tokens(tokens, arg_count);
        }
    }
}