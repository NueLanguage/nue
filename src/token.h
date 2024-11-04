#ifndef TOKEN_H
#define TOKEN_H

// definitions:
// LHS = Left Hand Side
// RHS = Right Hand Side

// defines the types of tokens that the lexer can return
typedef enum {
    // single character tokens
    TOKEN_LPAREN, TOKEN_RPAREN, // ()
    TOKEN_LBRACE, TOKEN_RBRACE, // {}
    TOKEN_LBRACKET, TOKEN_RBRACKET, // []
    TOKEN_COMMA, // ,
    TOKEN_DOT, // .
    TOKEN_SEMICOLON, // ; (optional, separates multiple statements on one line)

    // one or two character tokens
    TOKEN_PLUS, TOKEN_MINUS, // + -
    TOKEN_PLUS_EQ, // += (syntactic sugar for adding RHS value to LHS variable)
    TOKEN_MINUS_EQ, // -= (syntactic sugar for subtracting RHS value from LHS variable)

    TOKEN_STAR, TOKEN_SLASH, // * /
    TOKEN_STAR_EQ, // *= (syntactic sugar for multiplying LHS variable with RHS value)
    TOKEN_SLASH_EQ, // /= (syntactic sugar for dividing LHS variable by RHS value)

    TOKEN_POWER, // ^ (exponentiation)
    TOKEN_POWER_EQ, // ^= (syntactic sugar for raising LHS variable to the power of the RHS value)

    TOKEN_EQ, // = (assignment)
    TOKEN_EQ_EQ, // == (equal to)

    TOKEN_AND, // & (logical AND)
    TOKEN_OR, // || (logical OR)
    TOKEN_XOR, // | (logical XOR)

    TOKEN_BANG, // ! (logical NOT)
    TOKEN_BANG_EQ, // != (not equal to)

    TOKEN_LT, // < (less than)
    TOKEN_GT, // > (greater than)
    TOKEN_LT_EQ, // <= (less than or equal to)
    TOKEN_GT_EQ, // >= (greater than or equal to)

    TOKEN_LENGTH, // # (length operator)

    TOKEN_ARROW, // -> (used for function return types)

    // literals
    TOKEN_IDENTIFIER, // variable names, etc
    TOKEN_NUMBER, // numbers like 82374875 or 3.14159
    TOKEN_STRING, // strings like "hello, world"

    // keywords
    TOKEN_PRIVATE, TOKEN_VAR, TOKEN_ALIAS,
    TOKEN_IF, TOKEN_ELSE,
    TOKEN_FOR, TOKEN_IN,
    TOKEN_WHILE,
    TOKEN_BREAK,
    TOKEN_RETURN,
    TOKEN_THIS,

    TOKEN_DELETE, // unallocates something from memory

    TOKEN_TRUE, // boolean true
    TOKEN_FALSE, // boolean false

    TOKEN_NULL, // "null" keyword, type, and value
    TOKEN_FUNCTION, // "function" keyword, type, and constructor for new functions

    // type keywords
    TOKEN_TABLE,
    TOKEN_BOOLEAN,
    TOKEN_NUMBER_TYPE,
    TOKEN_STRING_TYPE,
    TOKEN_ANY, // denotes something which accepts any type

    // miscellaneous
    TOKEN_EOF, // end of file
    TOKEN_ERROR // error or unknown token
} TokenType;

// token structure. stores information about each token
typedef struct {
    TokenType type; // the type of the token
    char *lexeme; // literal text of the token
    void *literal; // literal value of the token
    int line; // the line number in the source file
} Token;

#endif