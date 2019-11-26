#include <stdio.h>
#include "gkml.h"
#include "build.h"
#include "parse.h"
#include "context.h"

static inline void print_context_errors(struct GkmlContext *ctx)
{
    if (!gkml_context_has_errors(ctx))
        return;

    for (size_t i=0; i < fl_array_length(ctx->errors); i++)
    {
        fprintf(stderr, "%s:%d:%d: %s\n", 
            ctx->srcinfo->location.filename, 
            ctx->errors[i].location.line, 
            ctx->errors[i].location.col, 
            ctx->errors[i].message
        );
    }
}

GKitElement gkml_load(const char *filename)
{
    struct GkmlContext ctx = gkml_context_new(GKML_SOURCE_FILE, filename);

    if (gkml_context_has_errors(&ctx))
        goto error;
    
    if (!gkml_parse_source(&ctx))
        goto error;

    GKitElement root = gkml_build_tree(&ctx);

    // If root is NULL print and return NULL
    if (root == NULL)
        goto error;

    // If there are errors, but we got an element to render, print the errors
    // and try to render what we got
    if (gkml_context_has_errors(&ctx))
        print_context_errors(&ctx);

    gkml_context_free(&ctx);

    return root;

    error:
    print_context_errors(&ctx);
    gkml_context_free(&ctx);
    return NULL;
}
