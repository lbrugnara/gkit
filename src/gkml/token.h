#ifndef GKML_TOKEN_H
#define GKML_TOKEN_H

#include <fllib.h>
#include "source.h"

/*
 * Enum: enum GkmlTokenType 
 *  The set of Zenit's token types
 */
enum GkmlTokenType {
    GKML_TOKEN_UNKNOWN,    

    // Types
    GKML_TOKEN_INTEGER,
    GKML_TOKEN_HEX,
    GKML_TOKEN_STRING,

    // Keywords
    GKML_TOKEN_ID,

    // Operators
    GKML_TOKEN_AMPERSAND,
    GKML_TOKEN_ASSIGN,

    // Punctuation
    GKML_TOKEN_COMMA,
    GKML_TOKEN_SEMICOLON,
    GKML_TOKEN_COLON,
    GKML_TOKEN_LBRACE,
    GKML_TOKEN_RBRACE,
    GKML_TOKEN_LBRACKET,
    GKML_TOKEN_RBRACKET,
    GKML_TOKEN_LPAREN,
    GKML_TOKEN_RPAREN,

    // Units
    GKML_TOKEN_PIXEL,
    GKML_TOKEN_PERCENTAGE,

    // Special tokens
    GKML_TOKEN_EOF,
};

/*
 * Struct: struct GkmlToken 
 *  Represents a token entity
 */
struct GkmlToken {
    enum GkmlTokenType type;
    struct FlSlice value;
    struct GkmlSourceLocation location;
};

/*
 * Function: gkml_token_print
 *  Returns a pointer to string that prints the friendly version
 *  of the enum <enum GkmlTokenType> value
 *
 * Parameters:
 *  type - enumeration value
 *
 * Returns:
 *  const char* - String pointer
 *
 */
const char* gkml_token_print(enum GkmlTokenType type);

#endif /* GKML_TOKEN_H */
