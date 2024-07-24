// nue headers

#ifndef NUE_H
#define NUE_H

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_KEYWORD,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_COMMENT,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

// token structure
typedef struct {
    TokenType type;
    char* value;
    int line;
    int column;
} Token;

// functionzz
Token* lexer(const char* source);

#endif // ends NUE_H