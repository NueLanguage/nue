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
    TOKEN_DOT_DOT, // .. (range operator), note that the "step" keyword is optional. if not provided, either 1 or -1 is used as the default step value
    TOKEN_ELLPISIS, // ... (variadic arguments)
    TOKEN_SEMICOLON, // ; (optional, separates multiple statements on one line)

    // one or two character tokens
    TOKEN_PLUS, TOKEN_MINUS, // + -
    TOKEN_PLUS_PLUS, // ++ (increment by 1)
    TOKEN_MINUS_MINUS, // -- (decrement by 1)
    TOKEN_PLUS_EQ, // += (syntactic sugar for adding RHS value to LHS variable)
    TOKEN_MINUS_EQ, // -= (syntactic sugar for subtracting RHS value from LHS variable)

    TOKEN_STAR, TOKEN_SLASH, // * /
    TOKEN_STAR_EQ, // *= (syntactic sugar for multiplying LHS variable with RHS value)
    TOKEN_SLASH_EQ, // /= (syntactic sugar for dividing LHS variable by RHS value)

    TOKEN_POWER, // ^ (exponentiation)
    TOKEN_POWER_EQ, // ^= (syntactic sugar for raising LHS variable to the power of the RHS value)

    // TODO: PERCENT_EQ and BACKSLASH_EQ?? maybe??
    TOKEN_PERCENT, // % (modulus operator)
    TOKEN_BACKSLASH, // \ (integer/floor division operator)

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

    TOKEN_QUESTION_MARK, // ? (used for nullability)

    TOKEN_IDENTIFIER, // variable names, etc
    // literals
    TOKEN_NUMBER, // numbers like 82374875 or 3.14159
    TOKEN_STRING, // strings like "hello, world"

    TOKEN_TRUE, // boolean true
    TOKEN_FALSE, // boolean false

    // void is the equivalent of not storing anything
    // in terms of memory consumption, null is just after "void" if we sort from lowest to highest mem usage
    // lets say it was measured in arbitrary units: null = 0, void = 1
    TOKEN_VOID, // void - dealloc from memory, complete absence of value
    TOKEN_NULL, // "null" keyword, type, and value

    // keywords
    TOKEN_PRIVATE, TOKEN_ALIAS,
    
    TOKEN_IF, TOKEN_ELSE,

    TOKEN_FOR, TOKEN_IN,
    TOKEN_STEP, // step keyword for range stepping, accompanied by TOKEN_DOT_DOT

    TOKEN_WHILE,
    TOKEN_CONTINUE,
    TOKEN_BREAK,

    TOKEN_RETURN,

    TOKEN_THIS,

    TOKEN_MATCH, // match keyword for pattern matching
    TOKEN_CASE, // case keyword for pattern matching

    TOKEN_FUNCTION, // "function" keyword, type, and constructor for new functions

    // type keywords
    TOKEN_TABLE,
    TOKEN_BOOLEAN,
    TOKEN_NUMBER_TYPE,
    TOKEN_STRING_TYPE,
    TOKEN_ANY, // denotes something which accepts any type

    TOKEN_AS, // type casting

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