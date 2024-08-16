// the nue lexer!
#include "lexer.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

// initialise the lexer with the source code
void initLexer(Lexer* lexer, const char* source) {
    lexer->start = source;
    lexer->current = source;
    lexer->line = 1;
}

// reminder that in c, there is a difference between ' (single quotes) and " (double quotes)
// 'a' - single character literal
// "a" - string literal containing an 'a' abd a null terminator. this is a 2 character array

// check if the lexer has reached the end of the source code
static bool isAtEnd(Lexer* lexer) {
    return *lexer->current == '\0';
}

// advance the lexer pointer to the next character and return the current character
static char advance(Lexer* lexer) {
    lexer->current++;
    return lexer->current[-1];
}

// peek at the current character without consuming it
static char peek(Lexer* lexer) {
    return *lexer->current;
}

// peek at the next character without consuming it
static char peekNext(Lexer* lexer) {
    if (isAtEnd(lexer)) return '\0';
    return lexer->current[1];
}

// skip whitespace and comments
// TODO: multiline comments still not detected properly. advances only twice instead of three times on first / encounter, fix later
static void skipWhitespace(Lexer* lexer) {
    // for non-c programmers, this is an infinite loop.
    // (in our case its not entirely infinite because we return at some point anyways)
    // gotta love the C bible
    for (;;) {
        char c = peek(lexer);
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance(lexer);
                break;
            case '\n':
                lexer->line++;
                advance(lexer);
                break;
            case '/': // possible start of a comment or division operator
                if (peekNext(lexer) == '/') {
                    advance(lexer); // consume first '/'
                    // TODO: changed from if (peek(lexer) == '#) {}
                    if (peekNext(lexer) == '#') { // multi-line comment
                        advance(lexer); // consume second '/'
                        // TODO: added an extra consumption here
                        advance(lexer); // consume '#'
                        while (!isAtEnd(lexer) && !(peek(lexer) == '#' && peekNext(lexer) == '/' && lexer->current[2] == '/')) {
                            if (peek(lexer) == '\n') lexer->line++;
                            advance(lexer);
                        }
                        if (!isAtEnd(lexer)) {
                            // consumes the three characters which are the multiline comment terminator, "#//"
                            advance(lexer); // consume '#'
                            advance(lexer); // consume '/'
                            advance(lexer); // consume '/'
                        }
                    } else { // single line comment because no '#' detected after "//"
                        while (peek(lexer) != '\n' && !isAtEnd(lexer)) advance(lexer);
                    }
                } else {
                    return; // not a comment, exit skipWhitespace to handle as a token (likely a division operator in this case)
                }
                break;
            default:
                return;
        }
    }
}

// match current character with expected character
// TODO: concerns about this function
static bool match(Lexer* lexer, char expected) {
    if (isAtEnd(lexer)) return false;
    if (*lexer->current != expected) return false;
    lexer->current++;
    return true;
}

// create a token
static Token makeToken(Lexer* lexer, TokenType type, void *literal) {
    Token token;
    token.type = type;
    token.lexeme = strndup(lexer->start, lexer->current - lexer->start);
    token.literal = literal;
    token.line = lexer->line;
    return token;
}

// create an error token
static Token errorToken(Lexer* lexer, const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.lexeme = strdup(message);
    token.literal = NULL;
    token.line = lexer->line;
    return token;
}

// https://trello.com/c/SxTSWtIl
// TODO: handle numbers differently later on. check trello board
// TODO: handle invalid numbers like 3.14.28746.3232xerf
static Token number(Lexer* lexer) {
    while (isdigit(peek(lexer))) advance(lexer);
    if (peek(lexer) == '.' && isdigit(peekNext(lexer))) {
        advance(lexer); // consume '.'
        while (isdigit(peek(lexer))) advance(lexer);
    }
    double value = strtod(lexer->start, NULL);
    double *literal = malloc(sizeof(double));
    *literal = value;
    return makeToken(lexer, TOKEN_NUMBER, literal);
}

// scan a string
// TODO: ANSI escape sequences
// TODO: implement single quotes, and other ways of writing strings. maybe multi line strings?
static Token string(Lexer* lexer) {
    while (peek(lexer) != '"' && !isAtEnd(lexer)) {
        if (peek(lexer) == '\n') lexer->line++;
        advance(lexer);
    }

    if (isAtEnd(lexer)) return errorToken(lexer, "Unterminated string");

    advance(lexer); // consume closing quote

    char *value = strndup(lexer->start + 1, (lexer->current - lexer->start) - 2);
    return makeToken(lexer, TOKEN_STRING, value);
}

// scan identifier or keyword
// TODO: get variables names and put them into the lexeme or literal property
static Token identifier(Lexer* lexer) {
    // you might question whether the identifier function allows identifiers to start with a number...
    // however thats not the case because in the scanToken we initially check with isalpha() which doesnt allow numbers
    while (isalnum(peek(lexer)) || peek(lexer) == '_') advance(lexer);

    // check if the identifier is a keyword
    size_t length = lexer->current - lexer->start;
    if (length == 6 && strncmp(lexer->start, "private", 6) == 0) return makeToken(lexer, TOKEN_PRIVATE, NULL);
    if (length == 3 && strncmp(lexer->start, "var", 3) == 0) return makeToken(lexer, TOKEN_VAR, NULL);
    if (length == 5 && strncmp(lexer->start, "alias", 5) == 0) return makeToken(lexer, TOKEN_ALIAS, NULL);
    if (length == 2 && strncmp(lexer->start, "if", 2) == 0) return makeToken(lexer, TOKEN_IF, NULL);
    if (length == 4 && strncmp(lexer->start, "else", 4) == 0) return makeToken(lexer, TOKEN_ELSE, NULL);
    // TODO: detect "else if" early on by combining them into one token
    // simplifies for later stages
    // if (length == 7 && strncmp(lexer->start, "else if", 7) == 0) return makeToken(lexer, TOKEN_ELSEIF, NULL);
    if (length == 3 && strncmp(lexer->start, "for", 3) == 0) return makeToken(lexer, TOKEN_FOR, NULL);
    if (length == 2 && strncmp(lexer->start, "in", 2) == 0) return makeToken(lexer, TOKEN_IN, NULL);
    if (length == 5 && strncmp(lexer->start, "while", 5) == 0) return makeToken(lexer, TOKEN_WHILE, NULL);
    if (length == 6 && strncmp(lexer->start, "return", 6) == 0) return makeToken(lexer, TOKEN_RETURN, NULL);
    if (length == 4 && strncmp(lexer->start, "this", 4) == 0) return makeToken(lexer, TOKEN_THIS, NULL);
    if (length == 6 && strncmp(lexer->start, "delete", 6) == 0) return makeToken(lexer, TOKEN_DELETE, NULL);
    if (length == 4 && strncmp(lexer->start, "true", 4) == 0) return makeToken(lexer, TOKEN_TRUE, NULL);
    if (length == 5 && strncmp(lexer->start, "false", 5) == 0) return makeToken(lexer, TOKEN_FALSE, NULL);
    if (length == 4 && strncmp(lexer->start, "null", 4) == 0) return makeToken(lexer, TOKEN_NULL, NULL);
    if (length == 8 && strncmp(lexer->start, "function", 8) == 0) return makeToken(lexer, TOKEN_FUNCTION, NULL);
    if (length == 5 && strncmp(lexer->start, "table", 5) == 0) return makeToken(lexer, TOKEN_TABLE, NULL);
    if (length == 7 && strncmp(lexer->start, "boolean", 7) == 0) return makeToken(lexer, TOKEN_BOOLEAN, NULL);
    if (length == 6 && strncmp(lexer->start, "number", 6) == 0) return makeToken(lexer, TOKEN_NUMBER_TYPE, NULL);
    if (length == 6 && strncmp(lexer->start, "string", 6) == 0) return makeToken(lexer, TOKEN_STRING_TYPE, NULL);
    if (length == 3 && strncmp(lexer->start, "any", 3) == 0) return makeToken(lexer, TOKEN_ANY, NULL);

    return makeToken(lexer, TOKEN_IDENTIFIER, NULL);
}

// the absolute core yk. the heart of the lexer. without it, blood stops pumping and the language becomes dead.
Token scanToken(Lexer* lexer) {
    skipWhitespace(lexer);
    lexer->start = lexer->current;

    if (isAtEnd(lexer)) return makeToken(lexer, TOKEN_EOF, NULL);

    char c = advance(lexer);

    // handle single character tokens directly (and sometimes double character tokens)
    switch (c) {
        case '(': return makeToken(lexer, TOKEN_LPAREN, NULL);
        case ')': return makeToken(lexer, TOKEN_RPAREN, NULL);

        case '{': return makeToken(lexer, TOKEN_LBRACE, NULL);
        case '}': return makeToken(lexer, TOKEN_RBRACE, NULL);

        case '[': return makeToken(lexer, TOKEN_LBRACKET, NULL);
        case ']': return makeToken(lexer, TOKEN_RBRACKET, NULL);

        case ';': return makeToken(lexer, TOKEN_SEMICOLON, NULL);
        case ',': return makeToken(lexer, TOKEN_COMMA, NULL);
        case '.': return makeToken(lexer, TOKEN_DOT, NULL);

        case '+': return makeToken(lexer, match(lexer, '=') ? TOKEN_PLUS_EQ : TOKEN_PLUS, NULL);
        case '-':
            if (match(lexer, '>')) return makeToken(lexer, TOKEN_ARROW, NULL);
            if (match(lexer, '=')) return makeToken(lexer, TOKEN_MINUS_EQ, NULL);
            return makeToken(lexer, TOKEN_MINUS, NULL); // neither an arrow nor syntactic sugar, therefore it is a regular minus

        case '*': return makeToken(lexer, match(lexer, '=') ? TOKEN_STAR_EQ : TOKEN_STAR, NULL);
        // this assumes that comments have been effectively handled in skipWhitespace(). therefore, if we have ended up here in the scanToken() function, then that means it is definitely a division operator.
        case '/': return makeToken(lexer, match(lexer, '=') ? TOKEN_SLASH_EQ : TOKEN_SLASH, NULL);
        case '^': return makeToken(lexer, match(lexer, '=') ? TOKEN_POWER_EQ : TOKEN_POWER, NULL);

        case '=': return makeToken(lexer, match(lexer, '=') ? TOKEN_EQ_EQ : TOKEN_EQ, NULL);
        case '!': return makeToken(lexer, match(lexer, '=') ? TOKEN_BANG_EQ : TOKEN_BANG, NULL);

        case '<': return makeToken(lexer, match(lexer, '=') ? TOKEN_LT_EQ : TOKEN_LT, NULL);
        case '>': return makeToken(lexer, match(lexer, '=') ? TOKEN_GT_EQ : TOKEN_GT, NULL);

        case '&': return makeToken(lexer, TOKEN_AND, NULL);
        case '|': return makeToken(lexer, match(lexer, '|') ? TOKEN_OR : TOKEN_XOR, NULL);

        case '#': return makeToken(lexer, TOKEN_LENGTH, NULL);
    }

    // handle number literals
    if (isdigit(c)) return number(lexer);

    // handle string literals
    // TODO: implement single quotes, and other ways of writing strings. maybe multi line strings?
    if (c == '"') return string(lexer);

    // handle identifiers and keywords
    if (isalpha(c) || c == '_') return identifier(lexer);

    return errorToken(lexer, "Unexpected character");
}