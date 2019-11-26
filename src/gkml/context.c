#include <fllib.h>
#include "ast.h"
#include "context.h"
#include "source.h"

/*
 * Function: error_free
 *  Frees the memory of an error object allocated with the <gkml_context_error> function
 *
 * Parameters:
 *  errorptr - Pointer to a <struct GkmlError> object
 *
 * Returns:
 *  void - This function does not return a value
 *
 */
static void error_free(void *errorptr)
{
    if (!errorptr)
        return;

    struct GkmlError *error = (struct GkmlError*)errorptr;

    if (error->message)
        fl_cstring_free(error->message);
}


/*
 * Function: gkml_context_new
 *  Allocates memory for a <struct GkmlSymbolTable> object and a <struct GkmlSourceInfo> object
 */
struct GkmlContext gkml_context_new(enum GkmlSourceType type, const char *input)
{
    struct GkmlContext ctx = { 
        .srcinfo = NULL,
        .errors = NULL
    };

    struct GkmlSourceInfo *srcinfo = gkml_source_new(type, input);

    if (srcinfo == NULL)
    {
        gkml_context_error(&ctx, (struct GkmlSourceLocation){ 0 }, GKML_ERROR_INTERNAL, "Could not read from the source");
        return ctx;
    }

    ctx.srcinfo = srcinfo;

    return ctx;
}

/*
 * Function: gkml_context_free
 *  Releases all the memory allocated by the <gkml_context_new> function
 *  but also if present, this function releases the memory of the <struct GkmlAst>
 *  object.
 */
void gkml_context_free(struct GkmlContext *ctx)
{
    if (!ctx)
        return;

    if (ctx->srcinfo)
        gkml_source_free(ctx->srcinfo);

    if (ctx->ast)
        gkml_ast_free(ctx->ast);

    if (ctx->errors)
        fl_array_free_each(ctx->errors, error_free);
}

/*
 * Function: gkml_context_error
 *  Initializes the *errors* property if needed and allocates memory for the formatted string. Both objects are
 *  deallocated by the <error_free> function.
 */
void gkml_context_error(struct GkmlContext *ctx, struct GkmlSourceLocation location, enum GkmlErrorType type, const char *message, ...)
{
    if (message == NULL)
        return;

    if (ctx->errors == NULL)
        ctx->errors = fl_array_new(sizeof(struct GkmlError), 0);

    va_list args;
    va_start(args, message);
    size_t length = vsnprintf(NULL, 0, message, args);
    va_end(args);

    if (length == 0)
    {
        return;
    }

    char *formatted_msg = fl_cstring_new(length);
    
    va_start(args, message);
    vsnprintf(formatted_msg, length+1, message, args);
    va_end(args);

    struct GkmlError error = {
        .type = type,
        .location = location,
        .message = formatted_msg
    };

    ctx->errors = fl_array_append(ctx->errors, &error);
}
