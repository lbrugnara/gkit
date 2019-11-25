#include "ast.h"

/*
 * Function: gkml_ast_free
 *  Frees the memory of all the declarations and the AST
 *  itself
 */
void gkml_ast_free(struct GkmlAst *ast)
{
    if (!ast)
        return;

    if (ast->root)
        gkml_node_element_free((struct GkmlElementNode*)ast->root);

    fl_free(ast);
}
