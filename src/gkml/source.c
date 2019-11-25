#include <fllib.h>
#include "source.h"

static struct GkmlSourceInfo* new_from_file(const char *filename);
static struct GkmlSourceInfo* new_from_string(const char *content);

/*
 * Function: new_from_file
 *  Reads the content of a file and creates a new <struct GkmlSourceInfo>
 *  object that contains it as the program's source code
 *
 * Parameters:
 *  filename - A valid filename with source code
 *
 * Returns:
 *  struct GkmlSourceInfo* - Represents the program's source code
 *
 */
static struct GkmlSourceInfo* new_from_file(const char *filename)
{
    if (!fl_io_file_exists(filename))
        return NULL;

    const char *content = fl_io_file_read_all_text(filename);

    if (content == NULL)
        return NULL;

    struct GkmlSourceInfo *srcinfo = new_from_string(content);
    fl_cstring_free(content);
    srcinfo->location.filename = fl_cstring_dup(filename);

    return srcinfo;
}

/*
 * Function: new_from_string
 *  Creates a <struct GkmlSourceInfo> object using the *content* parameter
 *  as the program's source code
 *
 * Parameters:
 *  content - Program's source code
 *
 * Returns:
 *  struct GkmlSourceInfo* - Represents the program's source code
 *
 */
static struct GkmlSourceInfo* new_from_string(const char *content)
{
    if (content == NULL)
        return NULL;

    struct GkmlSourceInfo *srcinfo = fl_malloc(sizeof(struct GkmlSourceInfo));

    srcinfo->source.content = fl_cstring_dup(content);
    srcinfo->source.length = strlen(content);

    srcinfo->location.filename = NULL;
    srcinfo->location.line = 1;
    srcinfo->location.col = 1;

    return srcinfo;
}

/*
 * Function: gkml_source_new
 *  Creates a <struct GkmlSourceInfo> object reading it from a file or by
 *  directly using the *input* parameter. The treatment of the *input*
 *  parameter depends on the origin or *type* of the program's code
 *
 */
struct GkmlSourceInfo* gkml_source_new(enum GkmlSourceType type, const char *input)
{
    if (type == GKML_SOURCE_FILE)
        return new_from_file(input);
    
    if (type == GKML_SOURCE_STRING)
        return new_from_string(input);

    return NULL;
}

/*
 * Function: gkml_source_free
 *  This function releases all the memory that is allocated
 *  in the <gkml_source_new> function, including the program's
 *  source code and the filename, if present.
 *
 */
void gkml_source_free(struct GkmlSourceInfo *srcinfo)
{
    if (!srcinfo)
        return;

    if (srcinfo->source.content)
        fl_cstring_free(srcinfo->source.content);

    if (srcinfo->location.filename)
        fl_cstring_free(srcinfo->location.filename);

    fl_free(srcinfo);
}
