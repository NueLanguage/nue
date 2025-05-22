#ifndef LEXER_H
#define LEXER_H

#include "token.h"

// lexer structure
typedef struct {
    const char* start; // start of the current token
    const char* current; // current character in the source code
    int line; // current line in the source code
} Lexer;

void initLexer(Lexer* lexer, const char* source);
Token scanToken(Lexer* lexer);

#endif