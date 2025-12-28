//
// Created by nate on 12/28/25.
//

#include "pen_lan.h"

//returns a set of parsed tokens and the amount of tokens found, right now this is just to parse command args
size_t tokenize(char ** tokens, char * input, size_t n) {

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
        }else if (*(input + i) == '{' && i > 0 && *(input + i - 1) == '!') {
            state = VARIABLE_TOKEN;
        }else if (*(input + i) == '!' && i < n - 1 && *(input + i + 1) == '{') {
            state = VARIABLE_TOKEN;
        }else if (*(input + i) == '}') {
            state = TOKEN;
        }else if (*(input + i) == ' ' && state == VARIABLE_TOKEN) {
            return -1;
        }
        else{
            *(*(tokens + token_counter) + curr_tok_ptr) = *(input + i);
            curr_tok_ptr = curr_tok_ptr + 1;
        }
    }

    //the plus one is to take into account the first token
    return token_counter + 1;
}