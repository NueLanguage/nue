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
// "a" - string literal containing an 'a' and a null terminator. this is a 2 character array.

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
                    if (peekNext(lexer) == '#') { // multi-line comment
                        advance(lexer); // consume second '/'
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
// TODO: concerns about this function: should it advance after simply matching?
// just match() by itself implies were only checking rather than affecting the state of the lexer
static bool match(Lexer* lexer, char expected) {
    if (isAtEnd(lexer)) return false;
    if (peek(lexer) != expected) return false;
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

static Token number(Lexer* lexer) {
    // this function became so un-maintainable i've decided to actually comment it for better uhhh.. comprehension?
    bool hasDot = false;
    bool isHex = false;
    bool isBinary = false;
    bool isOctal = false;

    // start of the number
    const char* numberStart = lexer->current - 1;

    // handle prefixes like 0x, 0b, 0o
    if (*numberStart == '0') {
        char nextChar = peek(lexer);
        if (nextChar == 'x' || nextChar == 'X') {
            isHex = true;
            advance(lexer); // consume 'x'
        } else if (nextChar == 'b' || nextChar == 'B') {
            isBinary = true;
            advance(lexer); // consume 'b'
        } else if (nextChar == 'o' || nextChar == 'O') {
            isOctal = true;
            advance(lexer); // consume 'o'
        }
        // there is no need to advance if its a leading zero in a decimal number
    }

    // main parsing loop
    while (true) {
        char c = peek(lexer);

        if (c == '_') {
            advance(lexer); // skip underscores
            continue;
        }

        if (isHex) {
            if (isxdigit(c)) {
                advance(lexer);
            } else if (isalnum(c)) {
                return errorToken(lexer, "Invalid hexadecimal digit");
            } else {
                break;
            }
        } else if (isBinary) {
            if (c == '0' || c == '1') {
                advance(lexer);
            } else if (isdigit(c)) {
                return errorToken(lexer, "Invalid binary digit");
            } else {
                break;
            }
        } else if (isOctal) {
            if (c >= '0' && c <= '7') {
                advance(lexer);
            } else if (isdigit(c)) {
                return errorToken(lexer, "Invalid octal digit");
            } else {
                break;
            }
        } else {
            if (isdigit(c)) {
                advance(lexer);
            } else if (c == '.') {
                if (hasDot) {
                    return errorToken(lexer, "Invalid number format: too many decimal points");
                }
                hasDot = true;
                advance(lexer);
            } else if (c == 'e' || c == 'E') {
                advance(lexer); // consume 'e'

                // add optional sign
                if (peek(lexer) == '+' || peek(lexer) == '-') {
                    advance(lexer);
                }

                if (!isdigit(peek(lexer))) {
                    return errorToken(lexer, "Invalid number format: incomplete exponent");
                }

                while (isdigit(peek(lexer))) {
                    advance(lexer);
                }
                break; // exponent handled
            } else {
                break;
            }
        }
    }

    const char* lexemeEnd = lexer->current;

    // build cleanLexeme without the underscores
    // yes i know not everyone will use underscores, theres probably a better way to handle stuff like "1_000_000" because not everyone will use it
    // TODO: maybe implement a bool variable at the top for detecting whether '_' has been used or 0x,0b,0o detected
    // if so, construct cleanLexeme since this thing is literally only required for numbers that have underscores for code readability or hex/bin/oct numbers
    // not required for regular numbers and therefore a tiny waste of resources, could directly jump to getting literal value
    size_t lexemeLength = lexemeEnd - numberStart;
    char* cleanLexeme = malloc(lexemeLength + 1); // +1 for null terminator
    size_t cleanIndex = 0;
    for (const char* p = numberStart; p < lexemeEnd; p++) {
        if (*p != '_') {
            cleanLexeme[cleanIndex++] = *p;
        }
    }
    cleanLexeme[cleanIndex] = '\0';

    // finally convert the number, oh my...
    double value = 0;
    char* endptr;

    if (isHex) {
        if (cleanIndex <= 2) { // no digits after "0x"
            free(cleanLexeme);
            return errorToken(lexer, "Invalid hexadecimal number");
        }

        value = (double)strtoll(cleanLexeme, &endptr, 16);
        if (*endptr != '\0') {
            free(cleanLexeme);
            return errorToken(lexer, "Invalid hexadecimal number");
        }
    } else if (isBinary) {
        if (cleanIndex <= 2) { // no digits after "0b"
            free(cleanLexeme);
            return errorToken(lexer, "Invalid binary number");
        }

        // manual conversion for binary
        // Q: why not just use strtoll?
        // A: because strtoll doesn't provide us with the granularity we need - we want to manually check whether it is EXPLICITLY ONLY 1s and 0s.
        // strtoll will literally just continue silently without any errors until it encounters an invalid character
        // this means that binary numbers like "0b102" will be successfully converted into the number 2, since it continued converting until it reached an invalid number (not 1 or 0) and interpreted it as "0b10"
        // this is an issue because we actually want to give users an error when they wrote an invalid binary number, not just silently continue converting the number and then have people complain about weird quirks in the language
        value = 0;
        size_t startIndex = 2; // skip "0b"
        for (size_t i = startIndex; i < cleanIndex; ++i) {
            char c = cleanLexeme[i];
            if (c == '0' || c == '1') {
                value = value * 2 + (c - '0');
            } else {
                free(cleanLexeme);
                return errorToken(lexer, "Invalid binary number");
            }
        }
    } else if (isOctal) {
        if (cleanIndex <= 2) { // no digits after "0o"
            free(cleanLexeme);
            return errorToken(lexer, "Invalid octal number");
        }

        value = (double)strtoll(cleanLexeme + 2, &endptr, 8);
        if (*endptr != '\0') {
            free(cleanLexeme);
            return errorToken(lexer, "Invalid octal number");
        }
    } else {
        value = strtod(cleanLexeme, &endptr);
        if (*endptr != '\0') {
            free(cleanLexeme);
            return errorToken(lexer, "Invalid number format");
        }
    }

    free(cleanLexeme);

    double* literal = malloc(sizeof(double));
    *literal = value;
    return makeToken(lexer, TOKEN_NUMBER, literal);
}

// scan a string
// TODO: follow lua's convention of ignoring whitespace and newline on first line and allowing newlines on other lines
// tabs / indentation shall be ignored too in multiline strings
// CLEAN THIS CODE UP
static Token string(Lexer* lexer, char quote) {
    bool isMultiline = false;
    int quoteCount = 1; // it is 1 bc we have already consumed a quote

    // check for triple quotes
    if (peek(lexer) == quote && peekNext(lexer) == quote) {
        advance(lexer); // consume second quote
        advance(lexer); // consume third quote (pointer is now pointing at the third quote)
        isMultiline = true;
        quoteCount = 3;
    }

    // use dynamic buffer to build actual processed string
    size_t bufferSize = 128;
    size_t bufferIndex = 0;
    char* buffer = malloc(bufferSize);

    while (!isAtEnd(lexer)) {
        char c = advance(lexer);

        if (c == '\n') {
            lexer->line++;
            if (!isMultiline) {
                free(buffer);
                return errorToken(lexer, "Unterminated string");
            }
        }

        if (c == quote) {
            int matchedQuotes = 1;

            // check if we have matching closing quotes
            while (matchedQuotes < quoteCount && peek(lexer) == quote) {
                advance(lexer);
                matchedQuotes++;
            }
            
            if (matchedQuotes == quoteCount) {
                // STRING END (TERMINATION)
                buffer[bufferIndex] = '\0'; // null terminate the string
                return makeToken(lexer, TOKEN_STRING, buffer);
            } else {
                // less than required quotes, treat as part of the string
                for (int i = 0; i < matchedQuotes; i++) {
                    if (bufferIndex >= bufferSize - 1) {
                        bufferSize *= 2;
                        buffer = realloc(buffer, bufferSize);
                    }
                    buffer[bufferIndex++] = quote;
                }
            }
        } else if (c == '\\') {
            // handle escape sequences
            if (isAtEnd(lexer)) {
                free(buffer);
                return errorToken(lexer, "Unterminated escape sequence");
            }

            char escapeChar = advance(lexer);
            if (escapeChar == 'x') {
                // hexadecimal escape sequence
                int hexValue = 0;
                int digits = 0;

                while (isxdigit(peek(lexer))) {
                    char hexDigit = advance(lexer);
                    hexValue = hexValue * 16 + (isdigit(hexDigit) ? hexDigit - '0' : tolower(hexDigit) - 'a' + 10);
                    digits++;
                }
                if (digits == 0) {
                    free(buffer);
                    return errorToken(lexer, "Invalid hexadecimal escape sequence");
                }
                // append processed character to buffer
                if (bufferIndex >= bufferSize - 1) {
                    bufferSize *= 2;
                    buffer = realloc(buffer, bufferSize);
                }
                buffer[bufferIndex++] = (char)hexValue;
            } else if (escapeChar == 'u' || escapeChar == 'U') {
                // unicode escape sequence
                int unicodeLength = (escapeChar == 'u') ? 4 : 8;
                int unicodeValue = 0;

                for (int i = 0; i < unicodeLength; i++) {
                    if (!isxdigit(peek(lexer))) {
                        free(buffer);
                        return errorToken(lexer, "Invalid Unicode escape sequence");
                    }
                    char hexDigit = advance(lexer);
                    unicodeValue = unicodeValue * 16 + (isdigit(hexDigit) ? hexDigit - '0' : tolower(hexDigit) - 'a' + 10);
                }

                // final step: encode unicode value into UTF-8
                char utf8[4];
                int utf8Length = 0;

                if (unicodeValue <= 0x7F) {
                    utf8[0] = (char)unicodeValue;
                    utf8Length = 1;
                } else if (unicodeValue <= 0x7FF) {
                    utf8[0] = (char)(0xC0 | ((unicodeValue >> 6) & 0x1F));
                    utf8[1] = (char)(0x80 | (unicodeValue & 0x3F));
                    utf8Length = 2;
                } else if (unicodeValue <= 0xFFFF) {
                    utf8[0] = (char)(0xE0 | ((unicodeValue >> 12) & 0x0F));
                    utf8[1] = (char)(0x80 | ((unicodeValue >> 6) & 0x3F));
                    utf8[2] = (char)(0x80 | (unicodeValue & 0x3F));
                    utf8Length = 3;
                } else if (unicodeValue <= 0x10FFFF) {
                    utf8[0] = (char)(0xF0 | ((unicodeValue >> 18) & 0x07));
                    utf8[1] = (char)(0x80 | ((unicodeValue >> 12) & 0x3F));
                    utf8[2] = (char)(0x80 | ((unicodeValue >> 6) & 0x3F));
                    utf8[3] = (char)(0x80 | (unicodeValue & 0x3F));
                    utf8Length = 4;
                } else {
                    free(buffer);
                    return errorToken(lexer, "Unicode code point out of range");
                }

                // append processed UTF-8 bytes to buffer
                for (int i = 0; i < utf8Length; i++) {
                    if (bufferIndex >= bufferSize - 1) {
                        bufferSize *= 2;
                        buffer = realloc(buffer, bufferSize);
                    }
                    buffer[bufferIndex++] = utf8[i];
                }
            } else if (escapeChar >= '0' && escapeChar <= '7') {
                // octal escape sequence
                int octalValue = escapeChar - '0';
                int digits = 1;

                while (digits < 3 && peek(lexer) >= '0' && peek(lexer) <= '7') {
                    octalValue = octalValue * 8 + (advance(lexer) - '0');
                    digits++;
                }

                // append character to buffer
                if (bufferIndex >= bufferSize - 1) {
                    bufferSize *= 2;
                    buffer = realloc(buffer, bufferSize);
                }
                buffer[bufferIndex++] = (char)octalValue;
            } else {
                // handle standard escape sequences
                char decodedChar;
                switch (escapeChar) {
                    case 'n': decodedChar = '\n'; break;
                    case 'r': decodedChar = '\r'; break;
                    case 't': decodedChar = '\t'; break;
                    case '\\': decodedChar = '\\'; break;
                    case '\'': decodedChar = '\''; break;
                    case '\"': decodedChar = '\"'; break;
                    case '0': decodedChar = '\0'; break;
                    case 'b': decodedChar = '\b'; break;
                    case 'f': decodedChar = '\f'; break;
                    case 'v': decodedChar = '\v'; break;
                    case 'a': decodedChar = '\a'; break; // should we even process this lmao?
                    default:
                        free(buffer);
                        return errorToken(lexer, "Invalid escape sequence");
                }

                if (bufferIndex >= bufferSize - 1) {
                    bufferSize *= 2;
                    buffer = realloc(buffer, bufferSize);
                }
                buffer[bufferIndex++] = decodedChar;
            }
        } else {
            // regular character

            if (bufferIndex >= bufferSize - 1) {
                bufferSize *= 2;
                buffer = realloc(buffer, bufferSize);
            }
            buffer[bufferIndex++] = c;
        }
    }

    // obviously if all of the if statements didnt catch all of the characters in a string or it just wasnt terminated, then return error
    free(buffer);
    return errorToken(lexer, "Unterminated string");
}

// scan identifier or keyword
static Token identifier(Lexer* lexer) {
    // you might question whether the identifier function allows identifiers to start with a number...
    // however thats not the case because in the scanToken we initially check with isalpha() which doesnt allow numbers
    while (isalnum(peek(lexer)) || peek(lexer) == '_') advance(lexer);

    // check if the identifier is a keyword
    size_t length = lexer->current - lexer->start;
    if (length == 4 && strncmp(lexer->start, "true", 4) == 0) return makeToken(lexer, TOKEN_TRUE, NULL);
    if (length == 5 && strncmp(lexer->start, "false", 5) == 0) return makeToken(lexer, TOKEN_FALSE, NULL);

    if (length == 4 && strncmp(lexer->start, "void", 4) == 0) return makeToken(lexer, TOKEN_VOID, NULL);
    if (length == 4 && strncmp(lexer->start, "null", 4) == 0) return makeToken(lexer, TOKEN_NULL, NULL);

    if (length == 7 && strncmp(lexer->start, "private", 7) == 0) return makeToken(lexer, TOKEN_PRIVATE, NULL);
    if (length == 5 && strncmp(lexer->start, "alias", 5) == 0) return makeToken(lexer, TOKEN_ALIAS, NULL);

    if (length == 2 && strncmp(lexer->start, "if", 2) == 0) return makeToken(lexer, TOKEN_IF, NULL);
    if (length == 4 && strncmp(lexer->start, "else", 4) == 0) return makeToken(lexer, TOKEN_ELSE, NULL);

    if (length == 3 && strncmp(lexer->start, "for", 3) == 0) return makeToken(lexer, TOKEN_FOR, NULL);
    if (length == 2 && strncmp(lexer->start, "in", 2) == 0) return makeToken(lexer, TOKEN_IN, NULL);
    if (length == 4 && strncmp(lexer->start, "step", 4) == 0) return makeToken(lexer, TOKEN_STEP, NULL);

    if (length == 5 && strncmp(lexer->start, "while", 5) == 0) return makeToken(lexer, TOKEN_WHILE, NULL);
    if (length == 8 && strncmp(lexer->start, "continue", 8) == 0) return makeToken(lexer, TOKEN_CONTINUE, NULL);
    if (length == 5 && strncmp(lexer->start, "break", 5) == 0) return makeToken(lexer, TOKEN_BREAK, NULL);

    if (length == 6 && strncmp(lexer->start, "return", 6) == 0) return makeToken(lexer, TOKEN_RETURN, NULL);

    if (length == 4 && strncmp(lexer->start, "this", 4) == 0) return makeToken(lexer, TOKEN_THIS, NULL);
    
    if (length == 5 && strncmp(lexer->start, "match", 5) == 0) return makeToken(lexer, TOKEN_MATCH, NULL);
    if (length == 4 && strncmp(lexer->start, "case", 4) == 0) return makeToken(lexer, TOKEN_CASE, NULL);

    if (length == 8 && strncmp(lexer->start, "function", 8) == 0) return makeToken(lexer, TOKEN_FUNCTION, NULL);

    if (length == 5 && strncmp(lexer->start, "table", 5) == 0) return makeToken(lexer, TOKEN_TABLE, NULL);
    if (length == 7 && strncmp(lexer->start, "boolean", 7) == 0) return makeToken(lexer, TOKEN_BOOLEAN, NULL);
    if (length == 6 && strncmp(lexer->start, "number", 6) == 0) return makeToken(lexer, TOKEN_NUMBER_TYPE, NULL);
    if (length == 6 && strncmp(lexer->start, "string", 6) == 0) return makeToken(lexer, TOKEN_STRING_TYPE, NULL);
    if (length == 3 && strncmp(lexer->start, "any", 3) == 0) return makeToken(lexer, TOKEN_ANY, NULL);

    if (length == 2 && strncmp(lexer->start, "as", 3) == 0) return makeToken(lexer, TOKEN_AS, NULL);

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
        case '.':
            if (match(lexer, '.')) return makeToken(lexer, match(lexer, '.') ? TOKEN_ELLPISIS : TOKEN_DOT_DOT, NULL);
            return makeToken(lexer, TOKEN_DOT, NULL);
        case '+':
            if (match(lexer, '+')) return makeToken(lexer, TOKEN_PLUS_PLUS, NULL);
            if (match(lexer, '=')) return makeToken(lexer, TOKEN_PLUS_EQ, NULL);
            return makeToken(lexer, TOKEN_PLUS, NULL); // not increment or syntactic sugar; therefore it is a regular plus
        
        case '-':
            if (match(lexer, '-')) return makeToken(lexer, TOKEN_MINUS_MINUS, NULL);
            if (match(lexer, '=')) return makeToken(lexer, TOKEN_MINUS_EQ, NULL);
            if (match(lexer, '>')) return makeToken(lexer, TOKEN_ARROW, NULL);
            return makeToken(lexer, TOKEN_MINUS, NULL); // not decrement, syntactic sugar or arrow; therefore it is a regular minus

        case '*': return makeToken(lexer, match(lexer, '=') ? TOKEN_STAR_EQ : TOKEN_STAR, NULL);
        // this assumes that comments have been effectively handled in skipWhitespace(). therefore, if we have ended up here in the scanToken() function, then that means it is definitely a division operator.
        case '/': return makeToken(lexer, match(lexer, '=') ? TOKEN_SLASH_EQ : TOKEN_SLASH, NULL);
        case '^': return makeToken(lexer, match(lexer, '=') ? TOKEN_POWER_EQ : TOKEN_POWER, NULL);

        case '%': return makeToken(lexer, TOKEN_PERCENT, NULL);
        case '\\': return makeToken(lexer, TOKEN_BACKSLASH, NULL);

        case '=': return makeToken(lexer, match(lexer, '=') ? TOKEN_EQ_EQ : TOKEN_EQ, NULL);
        case '!': return makeToken(lexer, match(lexer, '=') ? TOKEN_BANG_EQ : TOKEN_BANG, NULL);

        case '<': return makeToken(lexer, match(lexer, '=') ? TOKEN_LT_EQ : TOKEN_LT, NULL);
        case '>': return makeToken(lexer, match(lexer, '=') ? TOKEN_GT_EQ : TOKEN_GT, NULL);

        case '&': return makeToken(lexer, TOKEN_AND, NULL);
        case '|': return makeToken(lexer, match(lexer, '|') ? TOKEN_OR : TOKEN_XOR, NULL);

        case '#': return makeToken(lexer, TOKEN_LENGTH, NULL);

        case '?': return makeToken(lexer, TOKEN_QUESTION_MARK, NULL);
    }

    // handle number literals
    if (isdigit(c)) return number(lexer);

    // handle string literals
    if (c == '"' || c == '\'') return string(lexer, c);

    // handle identifiers and keywords
    if (isalpha(c) || c == '_') return identifier(lexer);

    return errorToken(lexer, "Unexpected character");
}