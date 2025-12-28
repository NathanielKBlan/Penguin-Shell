//
// Created by nate on 12/28/25.
//

#ifndef PENGUIN_PEN_LAN_H
#define PENGUIN_PEN_LAN_H

#include <stddef.h>
#include <stdlib.h>
#include <antarctic_env.h>

typedef enum {
    VARIABLE_TOKEN,
    QUOTE_TOKEN,
    TOKEN
} parser_state;

//tokenizing method for commands, results of parse saved in tokens, n represents the length of the input
size_t tokenize(char ** tokens, char * input, size_t n);

#endif //PENGUIN_PEN_LAN_H