#ifndef GKML_LEXER_H
#define GKML_LEXER_H

#include <fllib.h>
#include "source.h"
#include "token.h"

/*
 * Struct: struct GkmlLexer
 *  Object that keeps track of the lexical analysis phase
 *
 * Members:
 *  struct GkmlSourceInfo *srcinfo - Object that contains the program's source code
 *  unsigned int index - Used as a pointer to perform the scan's operations
 */
struct GkmlLexer {
    struct GkmlSourceInfo *srcinfo;
    unsigned int index;
};

/*
 * Function: gkml_lexer_new
 *  Creates a new <struct GkmlLexer> feeded with the provided source object.
 *
 * Parameters:
 *  srcinfo - Object that keeps track of source information
 *
 * Returns:
 *  struct GkmlLexer - Object to keep track of the lexical analysis
 *
 */
struct GkmlLexer gkml_lexer_new(struct GkmlSourceInfo *srcinfo);

/*
 * Function: gkml_lexer_consume
 *  Consumes and returns the next available token.
 *
 * Parameters:
 *  lexer - Lexer object
 *
 * Returns:
 *  struct GkmlToken - Consumed token
 *
 */
struct GkmlToken gkml_lexer_consume(struct GkmlLexer *lexer);

/*
 * Function: gkml_lexer_peek
 *  Returns the next available token without actually consuming it.
 *
 * Parameters:
 *  lexer - Lexer object
 *
 * Returns:
 *  struct GkmlToken - Next available token to consume
 *
 */
struct GkmlToken gkml_lexer_peek(struct GkmlLexer *lexer);

/*
 * Function: gkml_lexer_tokenize
 *  Tokenizes the whole source content and returns an array
 *  containing all the tokens. The array must be freed with the
 *  fllib's <fl_array_free> function 
 *
 * Parameters:
 *  lexer - Object that makes the lexical analysis
 *
 * Returns:
 *  struct GkmlToken* - Array of tokens. Must be freed with the <fl_array_free> function
 *
 */
struct GkmlToken* gkml_lexer_tokenize(struct GkmlLexer *lexer);

#endif /* GKML_LEXER_H */
