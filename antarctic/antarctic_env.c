//
// Created by nate on 12/24/25.
//

#include "antarctic_env.h"

void pen_export(char ** args, history * hist, size_t arg_count){

    //the scheme will be something like VAR_NAME=String
    //so to verify that it's a valid variable entry we can just do a simple
    //state machine that checks for that =

    (void)hist;
    (void)arg_count;

    char * new_var = *(args + 1);

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
        return;
    }
    //check if there are characters remaining after =
    if (total_chars_parsed >= length_to_parse) {
        return;
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
    setenv(name, value, 1);
}

void pen_chirp(char ** args, history * hist, size_t arg_count) {
    (void)hist;
    (void)arg_count;
    char * var = *(args + 1);
    char * value = getenv(var);
    if (value != NULL) {
        printf("(•ᴗ•)ゝ->%s\n", value);
    }else {
        printf("(•ᴖ•)ゝ Penguin couldn't find %s\n", var);
    }
}

//history management library
history * init_history() {
    history * hist = malloc(sizeof(history));
    hist->next_empty = 0;
    hist->cells_filled = 0;
    hist->entries = malloc(sizeof(history_entry *) * HISTORY_LIM);
    for (int i = 0; i < HISTORY_LIM; i++) {
        *(hist->entries + i) = NULL;
    }
    return hist;
}

history_entry * init_entry(size_t command_len, size_t arg_count) {
    history_entry * new_entry = malloc(sizeof(history_entry));

    new_entry->full_cmmd = NULL;
    new_entry->command = NULL;
    new_entry->args = NULL;

    //allocate memory for the full command
    new_entry->full_cmmd = malloc(sizeof(char) * command_len + 1);
    memset(new_entry->full_cmmd, 0, sizeof(char) * command_len + 1);

    //allocate memory for the command portion
    new_entry->command = malloc(sizeof(char) * MAX_CMMD_LEN + 1);
    memset(new_entry->command, 0, sizeof(char) * MAX_CMMD_LEN + 1);

    //allocate memory for the arguments
    new_entry->args = malloc(sizeof(char *) * arg_count);
    for (int i = 0; i < arg_count; i++) {
        *(new_entry->args + i) = malloc(sizeof(char) * MAX_ARG_LEN);
        memset(*(new_entry->args + i), 0, sizeof(char) * MAX_ARG_LEN);
    }

    return new_entry;
}

void fill_entry(history_entry ** entry, char * full_cmmd, char * command, char ** args, size_t command_len, size_t arg_count) {
    memcpy((*entry)->full_cmmd, full_cmmd, command_len);
    memcpy((*entry)->command, command, strlen(command));
    for (int i = 0; i < arg_count; i++) {
        memcpy(*((*entry)->args + i), *(args + i), sizeof(char) * MAX_ARG_LEN);
    }
}

void clear_entry(history_entry * entry) {
    free(entry->full_cmmd);
    free(entry->command);
    for (int i = 0; i < entry->arg_count; i++) {
        free(*(entry->args + i));
    }
    free(entry->args);
}


//TODO handle duplicate or empty case
int add_to_history(history * hist, char * full_cmmd, char * command, char ** args, size_t command_len, size_t arg_count) {

    //set the next history entry to fill (can be one that's already in use, in that case it gets overwritten)
    history_entry ** next_entry = hist->entries + hist->next_empty;

    //if the entry is null then we allocate a new entry into memory
    if ((*next_entry) == NULL) {
        (*next_entry) = init_entry(command_len, arg_count);
    } else {
        //clear out the full command, command, and arguments
        clear_entry((*next_entry));
        (*next_entry) = init_entry(command_len, arg_count);
    }

    //deep copy the full command, command, and arguments
    fill_entry(next_entry, full_cmmd, command, args, command_len, arg_count);

    //set the argument count
    (*next_entry)->arg_count = arg_count;

    //set the full command length
    (*next_entry)->full_cmmd_len = command_len;

    //increase the cells filled count, if we're at the limit then don't increase
    if (hist->cells_filled < HISTORY_LIM) {
        hist->cells_filled = hist->cells_filled + 1;
    }

    //specify the next entry to be filled
    hist->next_empty = (hist->next_empty + 1) % HISTORY_LIM;

    return 0;
}

void pen_print_history(char ** args, history * hist, size_t arg_count) {
    (void)args;
    (void)arg_count;
    for (int i = 0; i < hist->cells_filled; i++) {
        printf("%s", (*(hist->entries + i))->full_cmmd);
    }
}