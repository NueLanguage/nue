#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "token.h"

#define TOKEN_TYPE_COUNT (TOKEN_ERROR + 1) // total

const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case TOKEN_LPAREN: return "TOKEN_LPAREN";
        case TOKEN_RPAREN: return "TOKEN_RPAREN";
        case TOKEN_LBRACE: return "TOKEN_LBRACE";
        case TOKEN_RBRACE: return "TOKEN_RBRACE";
        case TOKEN_LBRACKET: return "TOKEN_LBRACKET";
        case TOKEN_RBRACKET: return "TOKEN_RBRACKET";
        case TOKEN_COMMA: return "TOKEN_COMMA";
        case TOKEN_DOT: return "TOKEN_DOT";
        case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
        case TOKEN_PLUS: return "TOKEN_PLUS";
        case TOKEN_MINUS: return "TOKEN_MINUS";
        case TOKEN_PLUS_EQ: return "TOKEN_PLUS_EQ";
        case TOKEN_MINUS_EQ: return "TOKEN_MINUS_EQ";
        case TOKEN_STAR: return "TOKEN_STAR";
        case TOKEN_SLASH: return "TOKEN_SLASH";
        case TOKEN_STAR_EQ: return "TOKEN_STAR_EQ";
        case TOKEN_SLASH_EQ: return "TOKEN_SLASH_EQ";
        case TOKEN_POWER: return "TOKEN_POWER";
        case TOKEN_POWER_EQ: return "TOKEN_POWER_EQ";
        case TOKEN_EQ: return "TOKEN_EQ";
        case TOKEN_EQ_EQ: return "TOKEN_EQ_EQ";
        case TOKEN_AND: return "TOKEN_AND";
        case TOKEN_OR: return "TOKEN_OR";
        case TOKEN_XOR: return "TOKEN_XOR";
        case TOKEN_BANG: return "TOKEN_BANG";
        case TOKEN_BANG_EQ: return "TOKEN_BANG_EQ";
        case TOKEN_LT: return "TOKEN_LT";
        case TOKEN_GT: return "TOKEN_GT";
        case TOKEN_LT_EQ: return "TOKEN_LT_EQ";
        case TOKEN_GT_EQ: return "TOKEN_GT_EQ";
        case TOKEN_LENGTH: return "TOKEN_LENGTH";
        case TOKEN_ARROW: return "TOKEN_ARROW";
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_NUMBER: return "TOKEN_NUMBER";
        case TOKEN_STRING: return "TOKEN_STRING";
        case TOKEN_PRIVATE: return "TOKEN_PRIVATE";
        case TOKEN_VAR: return "TOKEN_VAR";
        case TOKEN_ALIAS: return "TOKEN_ALIAS";
        case TOKEN_IF: return "TOKEN_IF";
        case TOKEN_ELSE: return "TOKEN_ELSE";
        case TOKEN_FOR: return "TOKEN_FOR";
        case TOKEN_IN: return "TOKEN_IN";
        case TOKEN_WHILE: return "TOKEN_WHILE";
        case TOKEN_RETURN: return "TOKEN_RETURN";
        case TOKEN_THIS: return "TOKEN_THIS";
        case TOKEN_DELETE: return "TOKEN_DELETE";
        case TOKEN_TRUE: return "TOKEN_TRUE";
        case TOKEN_FALSE: return "TOKEN_FALSE";
        case TOKEN_NULL: return "TOKEN_NULL";
        case TOKEN_FUNCTION: return "TOKEN_FUNCTION";
        case TOKEN_TABLE: return "TOKEN_TABLE";
        case TOKEN_BOOLEAN: return "TOKEN_BOOLEAN";
        case TOKEN_NUMBER_TYPE: return "TOKEN_NUMBER_TYPE";
        case TOKEN_STRING_TYPE: return "TOKEN_STRING_TYPE";
        case TOKEN_ANY: return "TOKEN_ANY";
        case TOKEN_EOF: return "TOKEN_EOF";
        case TOKEN_ERROR: return "TOKEN_ERROR";
        default: return "UNKNOWN_TOKEN";
    }
}

void printToken(Token token) {
    printf("[Line %d] %s '%s'", token.line, tokenTypeToString(token.type), token.lexeme);

    if (token.literal != NULL) {
        if (token.type == TOKEN_NUMBER) {
            printf(" (%f)", *(double*)token.literal);
        } else if (token.type == TOKEN_STRING) {
            printf(" ('%s')", (char*)token.literal);
        }
    } else {
        printf(" (null)");
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file.nue>\n", argv[0]);
        return 1;
    }

    // read source file
    const char* filename = argv[1];
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Could not open source file '%s'\n", filename);
        return 1;
    }

    // get file size
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    // read file into a buffer
    char* source = (char*)malloc(fileSize + 1);
    if (!source) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return 1;
    }
    fread(source, sizeof(char), fileSize, file);
    // this isnt necessary, however we add an extra terminator just incase someone catastrophically saved without a terminator
    source[fileSize] = '\0'; // null terminate the string
    fclose(file);

    // initialise the lexer
    Lexer lexer;
    initLexer(&lexer, source);

    // tokenise the source code
    Token token;

    // keep track of used token types
    // init all to false
    int usedTokenTypes[TOKEN_TYPE_COUNT];
    memset(usedTokenTypes, 0, sizeof(usedTokenTypes));

    int totalTokens = 0;

    do {
        token = scanToken(&lexer);
        printToken(token);

        // mark token type as used
        if (token.type >= 0 && token.type < TOKEN_TYPE_COUNT) {
            usedTokenTypes[token.type] = 1;
        }

        totalTokens++;

        // free the token's lexeme and literal if needed
        free(token.lexeme);
        if (token.literal != NULL) {
            if (token.type == TOKEN_NUMBER) {
                free((double*)token.literal);
            } else if (token.type == TOKEN_STRING) {
                free((char*)token.literal);
            }
        }
    } while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR);

    int uniqueTokensUsed = 0;
    for (int i = 1; i < TOKEN_TYPE_COUNT; i++) {
        if (usedTokenTypes[i]) {
            uniqueTokensUsed++;
        }
    }

    printf("Used %d out of %d token types.\n", uniqueTokensUsed, TOKEN_TYPE_COUNT);

    // Clean up
    free(source);
    return 0;
}