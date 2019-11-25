#ifndef GKML_SOURCE_H
#define GKML_SOURCE_H

#include "source.h"

/*
 * Enum: enum GkmlSourceType 
 *  The source code of a program can be
 *  consumed from a file or a string, so this
 *  enum represents those options,
 *
 */
enum GkmlSourceType {
    GKML_SOURCE_FILE,
    GKML_SOURCE_STRING
};

/*
 * Struct: struct GkmlSource
 *  Objects of this type contains the source code of
 *  a program and its length
 */
struct GkmlSource {
    const char *content;
    size_t length;
};

/*
 * Struct: struct GkmlSourceLocation
 *  Objects of this type keeps track of the
 *  position in the source code during the compilation
 *  process.
 * 
 * Members:
 *  filename: If the source of the program's code is a file, the filename
 *  line: The line number
 *  col: The column number
 */
struct GkmlSourceLocation {
    const char *filename;
    unsigned int line;
    unsigned int col;
};

/*
 * Struct: struct GkmlSourceInfo
 *  Represents the source code of the program and keeps track
 *  of the location during the compilation process
 * 
 * Members:
 *  source: A <struct GkmlSource> object containing the program's code
 *  location: A <struct GkmlSourceLocation> object that keeps track of the
 *              line and column numbers
 */
struct GkmlSourceInfo {
    struct GkmlSource source;
    struct GkmlSourceLocation location;
};

/*
 * Function: gkml_source_new
 *  Creates a <struct GkmlSourceInfo> object that contains the program's source
 *  and is ready to be used by a <struct ZenitContext> object.
 *
 * Parameters:
 *  type - The origin of the source code represented by a <enum GkmlSourceType> value
 *  input - A string that represents a filename or the source code based on the value
 *          of the *type* parameter.
 *
 * Returns:
 *  struct GkmlSourceInfo* - Pointer to an object that is ready to be used by a <struct ZenitContext>
 *                      object to start the compilation process
 * 
 * Notes:
 *  The pointer returned by this function MUST be freed with the <gkml_source_free> function
 *
 */
struct GkmlSourceInfo* gkml_source_new(enum GkmlSourceType type, const char *input);


/*
 * Function: gkml_source_free
 *  Releases the memory used by a <struct GkmlSourceInfo> object allocated
 *  with the <gkml_source_new> function.
 *
 * Parameters:
 *  srcinfo - Pointer to a <struct GkmlSourceInfo> object
 *
 * Returns:
 *  void - This function does not return a value
 */
void gkml_source_free(struct GkmlSourceInfo *srcinfo);

#endif /* GKML_SOURCE_H */
