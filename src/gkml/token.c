#include "token.h"

static const char *TokenTypeString[] = {
    [GKML_TOKEN_UNKNOWN]       = "UNKNOWN",
    
    [GKML_TOKEN_INTEGER]       = "INTEGER",
    [GKML_TOKEN_HEX]           = "HEX NUMBER",
    [GKML_TOKEN_STRING]        = "STRING",

    [GKML_TOKEN_ID]            = "IDENTIFIER",

    [GKML_TOKEN_AMPERSAND]     = "AMPERSAND \"&\"",
    [GKML_TOKEN_ASSIGN]        = "ASSIGN \"=\"",

    [GKML_TOKEN_COMMA]         = "COMMA \",\"",
    [GKML_TOKEN_SEMICOLON]     = "SEMICOLON \";\"",
    [GKML_TOKEN_COLON]         = "COLON \":\"",
    [GKML_TOKEN_LBRACE]        = "LEFT BRACE \"{\"",
    [GKML_TOKEN_RBRACE]        = "RIGHT BRACE \"}\"",
    [GKML_TOKEN_LBRACKET]      = "LEFT BRACKET \"[\"",
    [GKML_TOKEN_RBRACKET]      = "RIGHT BRACKET \"]\"",
    [GKML_TOKEN_LPAREN]        = "LEFT \"PARENTHESIS\"",
    [GKML_TOKEN_RPAREN]        = "RIGHT PARENTHESIS \")\"",

    [GKML_TOKEN_PIXEL]         = "PIXEL \"px\"",
    [GKML_TOKEN_PERCENTAGE]    = "PERCENTAGE \"%%\"",
    
    [GKML_TOKEN_EOF]           = "EOF",
};

const char* gkml_token_print(enum GkmlTokenType type)
{
    if (type < 0 || type > GKML_TOKEN_EOF)
        return "<invalid token>";

    return TokenTypeString[type];
}
