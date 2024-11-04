#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/lexer.h"
#include "../src/token.h"

#define TOKEN_TYPE_COUNT (TOKEN_ERROR + 1) // total

const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_DOT: return "DOT";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_PLUS_EQ: return "PLUS_EQ";
        case TOKEN_MINUS_EQ: return "MINUS_EQ";
        case TOKEN_STAR: return "STAR";
        case TOKEN_SLASH: return "SLASH";
        case TOKEN_STAR_EQ: return "STAR_EQ";
        case TOKEN_SLASH_EQ: return "SLASH_EQ";
        case TOKEN_POWER: return "POWER";
        case TOKEN_POWER_EQ: return "POWER_EQ";
        case TOKEN_EQ: return "EQ";
        case TOKEN_EQ_EQ: return "EQ_EQ";
        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_XOR: return "XOR";
        case TOKEN_BANG: return "BANG";
        case TOKEN_BANG_EQ: return "BANG_EQ";
        case TOKEN_LT: return "LT";
        case TOKEN_GT: return "GT";
        case TOKEN_LT_EQ: return "LT_EQ";
        case TOKEN_GT_EQ: return "GT_EQ";
        case TOKEN_LENGTH: return "LENGTH";
        case TOKEN_ARROW: return "ARROW";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_PRIVATE: return "PRIVATE";
        case TOKEN_VAR: return "VAR";
        case TOKEN_ALIAS: return "ALIAS";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_FOR: return "FOR";
        case TOKEN_IN: return "IN";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_BREAK: return "BREAK";
        case TOKEN_RETURN: return "RETURN";
        case TOKEN_THIS: return "THIS";
        case TOKEN_DELETE: return "DELETE";
        case TOKEN_TRUE: return "TRUE";
        case TOKEN_FALSE: return "FALSE";
        case TOKEN_NULL: return "NULL";
        case TOKEN_FUNCTION: return "FUNCTION";
        case TOKEN_TABLE: return "TABLE";
        case TOKEN_BOOLEAN: return "BOOLEAN";
        case TOKEN_NUMBER_TYPE: return "NUMBER_TYPE";
        case TOKEN_STRING_TYPE: return "STRING_TYPE";
        case TOKEN_ANY: return "ANY";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
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