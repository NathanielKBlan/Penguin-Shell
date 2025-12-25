//
// Created by nate on 12/24/25.
//

#include "antarctic_env.h"

int export(char * new_var){

    //the scheme will be something like VAR_NAME=String
    //so to verify that it's a valid variable entry we can just do a simple
    //state machine that checks for that =

    size_t length_to_parse = strlen(new_var);

    int total_chars_parsed = 0;
    char * curr_char = new_var;
    char name[VAR_NAME_LIM] = {0};
    char value[VAR_VAL_LIM] = {0};

    //parse out the var name
    while (total_chars_parsed < length_to_parse && *(curr_char) != '=') {
        *(name + total_chars_parsed) = *(curr_char);
        total_chars_parsed = total_chars_parsed + 1;
        curr_char = curr_char + 1;
    }

    //check if the = is present
    if (*(curr_char) == '=') {
        curr_char = curr_char + 1;
        total_chars_parsed = total_chars_parsed + 1;
    }else {
        return -1;
    }

    //check if there are characters remaining after =
    if (total_chars_parsed >= length_to_parse) {
        return -1;
    }

    //parse out the var value
    int var_char_counter = 0;
    while (total_chars_parsed < strlen(new_var) && var_char_counter < VAR_VAL_LIM) {
        *(value + var_char_counter) = *(curr_char);
        var_char_counter = var_char_counter + 1;
        curr_char = curr_char + 1;
        total_chars_parsed = total_chars_parsed + 1;
    }

    //set the var value
    int set_res = setenv(name, value, 1);
    return set_res;
}

//history management library
history * init_history() {
    history * hist = malloc(sizeof(history));
    hist->next_empty = 0;
    hist->cells_filled = 0;
    hist->entries = malloc(sizeof(char *) * HISTORY_LIM);
    return hist;
}

//TODO handle duplicate or empty case
int add_to_history(history * hist, char * full_cmmd, char * command, char ** args, size_t command_len, size_t arg_count) {

    history_entry * next_entry = *(hist->entries + hist->next_empty);

    if (next_entry == NULL) {
        next_entry = malloc(sizeof(history_entry));
    } else {
        free(next_entry->full_cmmd);
        free(next_entry->command);
        free(next_entry->args);
    }

    next_entry->full_cmmd = malloc(sizeof(char) * command_len);
    strcpy(next_entry->full_cmmd, full_cmmd);

    next_entry->command = command;
    next_entry->args = args;
    next_entry->arg_count = arg_count;

    *((hist->entries) + hist->next_empty) = next_entry;

    if (hist->cells_filled < HISTORY_LIM) {
        hist->cells_filled = hist->cells_filled + 1;
    }

    hist->next_empty = (hist->next_empty + 1) % HISTORY_LIM;

    return 0;
}

void print_history(history * hist) {
    for (int i = 0; i < hist->cells_filled; i++) {
        printf("%s", (*(hist->entries + i))->full_cmmd);
    }
}