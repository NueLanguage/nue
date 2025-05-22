//
// Created by Richy Z on 22/05/2025.
//

#ifndef TOKENISER_H
#define TOKENISER_H

#include "token.h"

// lexer structure
typedef struct {
    const char* start; // start of the current token
    const char* current; // current character in the source code
    int line; // current line in the source code
} Tokeniser;

void initTokeniser(Tokeniser* lexer, const char* source);
Token scanToken(Tokeniser* lexer);

#endif //TOKENISER_H
