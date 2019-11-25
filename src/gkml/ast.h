#ifndef GKML_AST_H
#define GKML_AST_H

#include <stdint.h>
#include "token.h"

#include "nodes/attribute.h"
#include "nodes/element.h"
#include "nodes/identifier.h"
#include "nodes/literal.h"
#include "nodes/style.h"

/*
 * Struct: struct GkmlAst
 *  Represents the abstract syntax tree of the parsed program
 * 
 * Members:
 *  <struct GkmlNode> **decls: -fllib- array of pointers that contains the declarations of the program
 * 
 */
struct GkmlAst {
    struct GkmlRectElementNode *root;
};

/*
 * Function: gkml_ast_free
 *  Releases the memory of a whole <struct GkmlAst> object
 *
 * Parameters:
 *  ast - AST object to free
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkml_ast_free(struct GkmlAst *ast);

#endif /* GKML_AST_H */
