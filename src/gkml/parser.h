#ifndef GKML_PARSER_H
#define GKML_PARSER_H

#include <fllib.h>
#include "source.h"
#include "lexer.h"

/*
 * Struct: struct GkmlParser
 *  Object to keep track of the parsing process
 */
struct GkmlParser {
    struct GkmlLexer lexer;
};

/*
 * Function: gkml_parser_new
 *  Returns a parser object ready to parse the provided source.
 *
 * Parameters:
 *  srcinfo - Object that keeps track of the source information
 *
 * Returns:
 *  struct GkmlParser - Parser object
 *
 */
struct GkmlParser gkml_parser_new(struct GkmlSourceInfo *srcinfo);

/*
 * Function: gkml_parser_peek
 *  Peeks without consuming the next available token
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  struct GkmlToken - The next available token
 *
 */
struct GkmlToken gkml_parser_peek(struct GkmlParser *parser);

/*
 * Function: gkml_parser_consume
 *  Consumes the next available token
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  struct GkmlToken - The consumed token
 *
 */
struct GkmlToken gkml_parser_consume(struct GkmlParser *parser);

/*
 * Function: gkml_parser_expects
 *  If the type of the next available token matches with the provided <enum GkmlTokenType>
 *  the parser consumes the token, if *consumed_token* is not NULL copies the consumed token
 *  to it, and returns *true*. If the type does not match this function returns *false* without
 *  consuming anything.
 *
 * Parameters:
 *  parser - Parser object
 *  type - Expected token type
 *  consumed_token - If not NULL and the types match, the consumed token will be copied to it
 *
 * Returns:
 *  bool - *true* if the next token's type matches the provided type.
 *
 */
bool gkml_parser_expects(struct GkmlParser *parser, enum GkmlTokenType type, struct GkmlToken *consumed_token);

/*
 * Function: gkml_parser_consume_if
 *  If the type of the following token is equals to the provided *type*
 *  this function consumes the token and returns true. If the types differ, the parser
 *  remains in the same position and returns false
 *
 * Parameters:
 *  parser - Parser object
 *  type - Expected type of the following token
 *
 * Returns:
 *  bool - *true* if the following token matches the *type*, otherwise *false*.
 */
bool gkml_parser_consume_if(struct GkmlParser *parser, enum GkmlTokenType type);

/*
 * Function: gkml_parser_has_input
 *  Returns *true* if there is pending input to process by the parser
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  bool - *true* if there is pending input, otherwise *false*.
 *
 */
bool gkml_parser_has_input(struct GkmlParser *parser);

#endif /* GKML_PARSER_H */
