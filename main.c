#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "linenoise.h"

#include "nue/tokeniser.h"
#include "nue/token.h"

#define TOKEN_TYPE_COUNT (TOKEN_ERROR + 1) // total

const char* tokenTypeString(const TokenType type) {
    switch (type) {
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_LBRACKET: return "LBRACKET";
        case TOKEN_RBRACKET: return "RBRACKET";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_DOT: return "DOT";
        case TOKEN_DOT_DOT: return "RANGE";
        case TOKEN_ELLIPSIS: return "ELLIPSIS";
        case TOKEN_SEMICOLON: return "SEMICOLON";



        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_PLUS_PLUS: return "INCREASE";
        case TOKEN_MINUS_MINUS: return "DECREASE";
        case TOKEN_PLUS_EQ: return "PLUS_EQ";
        case TOKEN_MINUS_EQ: return "MINUS_EQ";



        case TOKEN_STAR: return "STAR";
        case TOKEN_SLASH: return "SLASH";
        case TOKEN_STAR_EQ: return "STAR_EQ";
        case TOKEN_SLASH_EQ: return "SLASH_EQ";



        case TOKEN_POWER: return "POWER";
        case TOKEN_POWER_EQ: return "POWER_EQ";



        case TOKEN_PERCENT: return "MODULUS";
        case TOKEN_BACKSLASH: return "FLOORDIV";



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



        case TOKEN_QUESTION_MARK: return "NULLABLE";



        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";



        case TOKEN_TRUE: return "TRUE";
        case TOKEN_FALSE: return "FALSE";



        case TOKEN_VOID: return "VOID";
        case TOKEN_NULL: return "NULL";



        case TOKEN_PRIVATE: return "PRIVATE";
        case TOKEN_VAR: return "VAR";
        case TOKEN_ALIAS: return "ALIAS";

        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";

        case TOKEN_FOR: return "FOR";
        case TOKEN_IN: return "IN";
        case TOKEN_STEP: return "STEP";

        case TOKEN_WHILE: return "WHILE";
        case TOKEN_CONTINUE: return "CONTINUE";
        case TOKEN_BREAK: return "BREAK";

        case TOKEN_RETURN: return "RETURN";

        case TOKEN_THIS: return "THIS";

        case TOKEN_MATCH: return "MATCH";
        case TOKEN_CASE: return "CASE";




        case TOKEN_FUNCTION: return "FUNCTION";



        case TOKEN_TABLE: return "TABLE";
        case TOKEN_BOOLEAN: return "BOOLEAN";
        case TOKEN_NUMBER_TYPE: return "NUMBER_TYPE";
        case TOKEN_STRING_TYPE: return "STRING_TYPE";
        case TOKEN_ANY: return "ANY";


        case TOKEN_AS: return "AS";



        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";



        default: return "UNKNOWN";
    }
}

int usedTokenTypes[TOKEN_TYPE_COUNT] = {0};

// this is purely for when this file is compiled as a library, serves no use in the actual repl binary itself
// temporarily removed shared library export; will be re-added sometime later
// char* exportTokenisedStr(const char* source) {
//     Tokeniser lexer;
//     initTokeniser(&lexer, source);
//
//     Token token;
//     size_t bufferSize = 1024;
//     size_t bufferUsed = 0;
//     char* buffer = (char*)malloc(bufferSize);
//
//     if (!buffer) {
//         fprintf(stderr, "Memory allocation failed\n");
//         exit(1);
//     }
//
//     buffer[0] = '\0';
//
//     do {
//         token = scanToken(&lexer);
//
//         // ensure the buffer can accommodate the next tokens formatted string
//         const size_t requiredSize = bufferUsed + 128; // we are kind of assuming here that this is okay
//         if (requiredSize > bufferSize) {
//             bufferSize *= 2;
//             buffer = (char*)realloc(buffer, bufferSize);
//             if (!buffer) {
//                 fprintf(stderr, "Memory allocation failed\n");
//                 exit(1);
//             }
//         }
//
//         // append token info to buffer
//         bufferUsed += snprintf(buffer + bufferUsed, bufferSize - bufferUsed, "[Line %d] %s '%s'", token.line, tokenTypeString(token.type), token.lexeme);
//
//         if (token.literal != NULL) {
//             if (token.type == TOKEN_NUMBER) {
//                 bufferUsed += snprintf(buffer + bufferUsed, bufferSize - bufferUsed, " (%f)", *(double*)token.literal);
//             } else if (token.type == TOKEN_STRING) {
//                 bufferUsed += snprintf(buffer + bufferUsed, bufferSize - bufferUsed, " ('%s')", (char*)token.literal);
//             }
//         }
//         bufferUsed += snprintf(buffer + bufferUsed, bufferSize - bufferUsed, "\n");
//
//         // cleanup
//         free(token.lexeme);
//         if (token.literal != NULL) {
//             if (token.type == TOKEN_NUMBER) {
//                 free(token.literal);
//             } else if (token.type == TOKEN_STRING) {
//                 free(token.literal);
//             }
//         }
//     } while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR);
//
//     return buffer; // the caller should free this
// }

void printToken(const Token token) {
    const char* COLOUR_LINE     = "\x1b[38;5;244m";  // grey
    const char* COLOUR_TYPE     = "\x1b[38;5;81m";   // cyan
    const char* COLOUR_STRING   = "\x1b[38;5;197m";  // pink
    const char* COLOUR_NUMBER   = "\x1b[38;5;141m";  // lavender
    const char* COLOUR_LEXEME   = "\x1b[38;5;114m";  // green-ish
    const char* COLOUR_ERROR    = "\x1b[38;5;196m";  // bright red
    const char* COLOUR_RESET    = "\x1b[0m";

    printf("%s%4d%s  ", COLOUR_LINE, token.line, COLOUR_RESET);
    printf("%s%-16s%s  ", COLOUR_TYPE, tokenTypeString(token.type), COLOUR_RESET);

    if (token.type == TOKEN_STRING) {
        printf("%s'%s'%s", COLOUR_STRING, token.lexeme, COLOUR_RESET);
    } else if (token.type == TOKEN_NUMBER) {
        printf("%s'%s'%s", COLOUR_NUMBER, token.lexeme, COLOUR_RESET);
    } else if (token.type == TOKEN_ERROR) {
        printf("%s'%s'%s", COLOUR_ERROR, token.lexeme, COLOUR_RESET);
    } else {
        printf("%s'%s'%s", COLOUR_LEXEME, token.lexeme, COLOUR_RESET);
    }

    if (token.literal != NULL) {
        if (token.type == TOKEN_NUMBER) {
            printf("  %s(%f)%s", COLOUR_NUMBER, *(double*)token.literal, COLOUR_RESET);
        } else if (token.type == TOKEN_STRING) {
            printf("  %s('%s')%s", COLOUR_STRING, (char*)token.literal, COLOUR_RESET);
        }
    }

    printf("\n");
}

void printUnused() {
    printf("Unused tokens: ");
    bool first = true;
    for (int i = 0; i < TOKEN_TYPE_COUNT; i++) {
        if (!usedTokenTypes[i]) {
            if (!first) printf(", ");
            printf("%s", tokenTypeString(i));
            first = false;
        }
    }
    if (first) printf("None");
    printf("\n");
}

void printUsage() {
    int uniqueTokensUsed = 0;
    for (int i = 0; i < TOKEN_TYPE_COUNT; i++) {
        if (usedTokenTypes[i]) uniqueTokensUsed++;
    }

    const int percentage = (uniqueTokensUsed * 100) / TOKEN_TYPE_COUNT;

    printf("You have used %d out of %d token types. (%d%%)\n", uniqueTokensUsed, TOKEN_TYPE_COUNT, percentage);
    printUnused();
    printf("\n");
}

void tokeniseInput(const char* source) {
    Tokeniser lexer;
    initTokeniser(&lexer, source);

    Token token;
    do {
        token = scanToken(&lexer);
        printToken(token);

        // mark token type as used
        if (token.type >= 0 && token.type < TOKEN_TYPE_COUNT) {
            usedTokenTypes[token.type] = 1;
        }

        // free token resources
        free(token.lexeme);
        if (token.literal != NULL) free(token.literal);
    } while (token.type != TOKEN_EOF && token.type != TOKEN_ERROR);
}

void repl(const bool silent) {
    printf("Nue Lexer REPL - Type code below or 'exit' to quit.\n");

    while (1) {
        char* line = linenoise("> ");
        if (!line) {
            break; // EOF or error
        }

        if (strcmp(line, "exit") == 0) {
            printf("Bye!\n");
            free(line);
            break;
        }

        linenoiseHistoryAdd(line);

        tokeniseInput(line);

        if (!silent) {
            printf("\n");
            printUsage();
            printf("\n");
        }

        free(line);
    }
}

void evalFile(const char* filename, const bool silent) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Could not open source file '%s'\n", filename);
        exit(1);
    }

    fseek(file, 0L, SEEK_END);
    const size_t fileSize = ftell(file);
    rewind(file);

    char* source = malloc(fileSize + 1);
    if (!source) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        exit(1);
    }
    fread(source, sizeof(char), fileSize, file);

    // this isn't necessary; however, we add an extra terminator just incase someone catastrophically saved without a terminator
    source[fileSize] = '\0'; // null terminates the string
    fclose(file);

    tokeniseInput(source);
    free(source);

    if (!silent) {
        // print stats
        printf("--------------------------------------------------\n");
        printUsage();
    }
}

int main(const int argc, char* argv[]) {
    bool silent = false;
    const char* filename = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--silent") == 0) {
            silent = true;
        } else {
            filename = argv[i];
        }
    }

    if (filename == NULL) {
        printf("No source file provided. Do you want to start REPL mode? (y/n): ");
        char response;
        if (scanf(" %c", &response) && (response == 'y' || response == 'Y')) {
            getchar(); // clear newline from input buffer
            repl(silent);
        } else {
            printf("Exiting.\n");
            return 0;
        }
    } else {
        evalFile(filename, silent);
    }

    return 0;
}