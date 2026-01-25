//
// Created by nate on 12/28/25.
//

#include "pen_lan.h"

//returns a set of parsed tokens and the amount of tokens found, right now this is just to parse command args
size_t tokenize(char ** tokens, char * input, pen_alias_table * table, size_t n) {

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

            //perform alias expansion here
            char * alias_value = alias_lookup(table, *(tokens + token_counter));

            if (alias_value != NULL) {

                memset(*(tokens + token_counter), 0, curr_tok_ptr);

                char ** alias_tokens = malloc(sizeof(char *) * ALIAS_ARG_LIM);
                size_t alias_token_count = tokenize(alias_tokens, alias_value, table, strlen(alias_value));

                for (int a = 0; a < alias_token_count; a++) {
                    if (*(tokens + token_counter) == NULL) {
                        *(tokens + token_counter) = malloc(sizeof(char) * strlen(*(alias_tokens + a)) + 1);
                    }
                    memcpy(*(tokens + token_counter), *(alias_tokens + a), strlen(*(alias_tokens + a)) + 1);
                    token_counter = token_counter + 1;
                }

            }else {
                token_counter = token_counter + 1;
            }

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

    //perform alias expansion here for the last token
    size_t pre_exapnsion_token_count = token_counter;
    char * alias_value = alias_lookup(table, *(tokens + token_counter));

    if (alias_value != NULL) {

        memset(*(tokens + token_counter), 0, curr_tok_ptr);

        char ** alias_tokens = malloc(sizeof(char *) * ALIAS_ARG_LIM);
        size_t alias_token_count = tokenize(alias_tokens, alias_value, table, strlen(alias_value));

        for (int a = 0; a < alias_token_count; a++) {
            if (*(tokens + token_counter) == NULL) {
                *(tokens + token_counter) = malloc(sizeof(char) * strlen(*(alias_tokens + a)) + 1);
            }
            memcpy(*(tokens + token_counter), *(alias_tokens + a), strlen(*(alias_tokens + a)) + 1);
            token_counter = token_counter + 1;
        }

        if (pre_exapnsion_token_count > 0) {
            token_counter = token_counter - 1;
            token_counter = token_counter + alias_token_count;
            return token_counter;
        }

        return alias_token_count;
    }

    //the plus one is to take into account the first token
    return token_counter + 1;
}