#ifndef GKML_PARSE_H
#define GKML_PARSE_H

#include "context.h"
#include "lexer.h"
#include "ast.h"

/*
 * Function: gkml_parse_source
 *  Parses the whole source code and populates the <struct GkmlContext>
 *  with the resulting <struct GkmlAst> object.
 *
 * Parameters:
 *  ctx - Context object
 *
 * Returns:
*  void - *true* tif he parsing ends without errors. Otheriwise, this function 
 *          returns *false* and the *errors* property in the <struct GkmlContext> 
 *          object should contain the error(s) description
 *
 */
bool gkml_parse_source(struct GkmlContext *ctx);

#endif /* GKML_PARSE_H */
