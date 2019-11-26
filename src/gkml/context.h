#ifndef GKML_CONTEXT_H
#define GKML_CONTEXT_H

#include "ast.h"
#include "source.h"

/*
 * Macro: gkml_context_error_count
 *  Returns the size of the *errors* property of a <struct GkmlContext> object
 *
 * Parameters:
 *  ctxptr - A pointer to a <struct GkmlContext> object
 */
#define gkml_context_error_count(ctxptr) ((size_t)((ctxptr)->errors ? fl_array_length((ctxptr)->errors) : 0))


/*
 * Macro: gkml_context_has_errors
 *  Evaluates to *true* if the *errors* property in the <struct GkmlContext> object is
 *  greater than 0
 *
 * Parameters:
 *  ctxptr - A pointer to a <struct GkmlContext> object
 */
#define gkml_context_has_errors(ctxptr) (gkml_context_error_count((ctxptr)) > 0)

/*
 * Enum: enum GkmlErrorType 
 *  Represents every type of error that might occur in the
 *  compilation process
 *
 */
enum GkmlErrorType {
    // Gkml specific errors
    GKML_ERROR_INTERNAL,

    // Mostly missing/unexpected tokens
    GKML_ERROR_SYNTAX,

    // Number cannot be represented by any integer type
    GKML_ERROR_LARGE_INTEGER,

    // Symbol couldn't be found
    GKML_ERROR_UNKNOWN_VALUE,

    // Symbol already exists
    GKML_ERROR_DUPLICATED_SYMBOL,

    // An operation involves types that are incompatible
    GKML_ERROR_TYPE_MISSMATCH,
};

/*
 * Struct: struct GkmlError
 *  All the errors that occur in the compilation are tracked by
 *  objects of this type
 * 
 * Members:
 *  message: The error explanation
 *  location: Where in the source the error occurred tracked by a <struct GkmlSourceLocation> object
 *  type: The type of the error being one of the <enum GkmlErrorType> values
 */
struct GkmlError {
    const char *message;
    struct GkmlSourceLocation location;
    enum GkmlErrorType type;
};

/*
 * Struct: struct GkmlContext
 *  Keeps track of the information generated in the compilation
 *  process.
 * 
 * Members:
 *  errors - Array of <struct GkmlError> objects that occur in the compilation process
 *  ast - After the parse process this property contains the <struct GkmlAst> object
 *  srcinfo - <struct GkmlSourceInfo> object
 *  program - The object that contains the program being compiled
 */
struct GkmlContext {
    struct GkmlError *errors;
    struct GkmlAst *ast;
    struct GkmlSourceInfo *srcinfo;
};

/*
 * Function: gkml_context_new
 *  Creates a new <struct GkmlContext> object that can be used to start the compilation process.
 *
 * Parameters:
 *  type - The origin or source from where to get the program's source code (enum GkmlSourceType)
 *  input - Based on the *type* parameter this could be a filename or the program's source code
 *
 * Returns:
 *  struct GkmlContext - An object to ready to be used in the compilation process
 *
 * Notes:
 *  The object returned by this function internally allocates memory for its properties, so
 *  the user must free that memory with the <gkml_context_free> function
 */
struct GkmlContext gkml_context_new(enum GkmlSourceType type, const char *input);

/*
 * Function: gkml_context_free
 *  Releases the memory of an object allocated with the <gkml_context_new> function
 *
 * Parameters:
 *  ctx - <struct GkmlContext> object to free
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkml_context_free(struct GkmlContext* ctx);

/*
 * Function: gkml_context_error
 *  Adds a new <struct GkmlError> object to the <struct GkmlContext>'s *errors* property
 *
 * Parameters:
 *  ctx - <struct GkmlContext> object to free
 *  location - <struct GkmlSourceLocation> object that contains information of where in the source code the error happened
 *  type - The <enum GkmlErrorType> that represents the type of the error
 *  message - A string that accepts format specifiers containing information about the error
 *  ... - Depending on the format string, the function may expect a sequence of additional arguments, each containing 
 *          a value to be used to replace a format specifier in the format string.
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkml_context_error(struct GkmlContext *ctx, struct GkmlSourceLocation location, enum GkmlErrorType type, const char *message, ...);


#endif /* GKML_CONTEXT_H */
