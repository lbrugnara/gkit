#include <fllib.h>
#include "lexer.h"

/*
 * Macro: is_number
 *  Checks if *chr* is an ASCII number
 *
 * Parameters:
 *  chr - Char
 *
 */
#define is_number(chr) ((chr) >= 0x30 && (chr) <= 0x39)

/*
 * Macro: is_alpha
 *  Checks if *chr* is a letter
 *
 * Parameters:
 *  chr - Char.
 *
 */
#define is_alpha(chr) (((chr) >= 0x41 && (chr) <= 0x5A) || ((chr) >= 0x61 && (chr) <= 0x7A))

/*
 * Macro: is_reserved_keyword
 *  Checks if the <struct FlSlice> object *seq* is equals to the array of <FlByte>s represented
 *  by the string *str*
 *
 * Parameters:
 *  seq - Pointer to a <struct FlSlice> object.
 *  str - String.
 *
 */
#define is_reserved_keyword(seq, str) fl_slice_equals_sequence(seq, (FlByte*)(str), strlen(str))

/*
 * Macro: is_string
 *  Checks if the <struct FlSlice> object *seq* is equals to the string *str*
 *
 * Parameters:
 *  seq - Pointer to a <struct FlSlice> object.
 *  str - String.
 *
 */
#define is_string(seq, str) fl_slice_equals_sequence(seq, (FlByte*)(str), strlen(str))

/* Private API */
/*
 * Variable: sync_char
 *  Group of synchronization characters of length 1
 *
 */
static char sync_char[] = { '\0', '\n', ' ', '\t', '\r', '#', ';', '=' };

/*
 * Variable: sync_chars
 *  Group of synchronization characters of length greater than 1
 *
 */
static char *sync_chars[] = { "//", "/*" };

/*
 * Function: has_input
 *  Returns *true* if there is still input to process
 *
 * Parameters:
 *  lexer - Lexer object
 *
 * Returns:
 *  bool - *true* if there is input pending to process
 *
 */
static inline bool has_input(struct GkmlLexer *lexer)
{
    return lexer->index < lexer->srcinfo->source.length;
}

/*
 * Function: peek
 *  Returns the current character pointed by the lexer, without
 *  actually consuming it
 *
 * Parameters:
 *  lexer - Lexer object
 *
 * Returns:
 *  char - Current character pointed by the lexer's internal pointer
 *
 */
static inline char peek(struct GkmlLexer *lexer)
{
    return lexer->srcinfo->source.content[lexer->index];
}

/*
 * Function: peek_at
 *  Similar to <peek> but returning an element that is *index* elements
 *  ahead in the source. If the *index* "falls" outside of the valid range
 *  this function returns the NULL character ('\0')
 *
 * Parameters:
 *  lexer - Lexer object
 *  index - Offset
 *
 * Returns:
 *  char - Char placed *index* elements ahead of the lexer's internal pointer
 *          or NULL if *index* falls outside of the valid range.
 *
 */
static inline char peek_at(struct GkmlLexer *lexer, size_t index)
{
    if (lexer->index + index >= lexer->srcinfo->source.length)
        return '\0';

    return lexer->srcinfo->source.content[lexer->index + index];
}

/*
 * Function: peek_many
 *  Returns a slice that takes *n* characters starting from the current internal
 *  pointer position.
 *  If *n* takes elements that fall outside of the valid range, the whole sequence
 *  is NULL to indicate that is not possible to peek that much number of characters
 *
 * Parameters:
 *  lexer - Lexer object
 *  n - Elements to take from the current internal pointer.
 *
 * Returns:
 *  struct FlSlice - Slice of characters
 *
 */
static inline struct FlSlice peek_many(struct GkmlLexer *lexer, size_t n)
{
    if (lexer->index + n >= lexer->srcinfo->source.length)
        return (struct FlSlice){ .sequence = NULL };

    return fl_slice_new((const FlByte*)lexer->srcinfo->source.content, 1, lexer->index, n);
}

/*
 * Function: peek_many_at
 *  Returns a slice that takes *n* characters starting from the current internal
 *  pointer position.
 *  If *n* takes elements that fall outside of the valid range, the whole sequence
 *  is NULL to indicate that is not possible to peek that much number of characters
 *
 * Parameters:
 *  lexer - Lexer object
 *  offset - Number of elements to skip
 *  n - Elements to take from the current internal pointer.
 *
 * Returns:
 *  struct FlSlice - Slice of characters
 *
 */
static inline struct FlSlice peek_many_at(struct GkmlLexer *lexer, size_t offset, size_t n)
{
    if (lexer->index + offset + n >= lexer->srcinfo->source.length)
        return (struct FlSlice){ .sequence = NULL };

    return fl_slice_new((const FlByte*)lexer->srcinfo->source.content, 1, lexer->index + offset, n);
}

/*
 * Function: consume
 *  Returns the current character and increments the internal pointer
 *
 * Parameters:
 *  lexer - Lexer object
 *
 * Returns:
 *  char - Character that has been consumed
 *
 */
static inline char consume(struct GkmlLexer *lexer)
{
    lexer->srcinfo->location.col++;
    return lexer->srcinfo->source.content[lexer->index++];
}

/*
 * Function: is_sync_character
 *  Returns *true* if a char or a group of chars placed at *offset* elements of
 *  the current pointer source is *synchronization character* when an error
 *  occurs
 *
 * Parameters:
 *  lexer - Lexer object
 *  offset - Number of elements to skip from the current pointer source
 *
 * Returns:
 *  bool - *true* if the character (or group of chars) is a synchronization character
 *
 */
static inline bool is_sync_character(struct GkmlLexer *lexer, size_t offset)
{
    char c = peek_at(lexer, offset);

    for (size_t i=0; i < sizeof(sync_char) / sizeof(sync_char[0]); i++)
        if (sync_char[i] == c)
            return true;

    // Comments
    struct FlSlice cc = peek_many_at(lexer, offset, 2);

    if (!cc.sequence)
        return false;

    for (size_t i=0; i < sizeof(sync_chars) / sizeof(sync_chars[0]); i++)
        if (is_string(&cc, sync_chars[i]))
            return true;

    return false;
}

/*
 * Function: remove_ws_and_comments
 *  This function strips out all the following white spaces and comments until
 *  fetching something that doesn't fall in that category
 *
 * Parameters:
 *  lexer - Lexer object
 *
 * Returns:
 *  void - This function does not return a value
 *
 */
static inline void remove_ws_and_comments(struct GkmlLexer *lexer)
{
    while(has_input(lexer))
    {
        char c = peek(lexer);

        // New lines
        if (c == '\n')
        {
            consume(lexer);
            lexer->srcinfo->location.line++;
            lexer->srcinfo->location.col = 1;
            continue;
        }
        
        // Consume whitespace
        if (c == ' ' || c == '\t' || c == '\r')
        {
            consume(lexer);
            continue;
        }

        // Consume comments
        struct FlSlice cc = peek_many(lexer, 2);
        
        if (cc.sequence && is_string(&cc, "//"))
        {
            // Consume comment line
            while (has_input(lexer) && peek(lexer) != '\n')
                consume(lexer);

            // Try again
            continue;
        }
        else if (cc.sequence && is_string(&cc, "/*"))
        {
            // Consume comment line
            while (has_input(lexer))
            {
                char c = consume(lexer);
                if (c == '*' && peek(lexer) == '/')
                {
                    consume(lexer);
                    break;
                }
                if (c == '\n')
                {
                    lexer->srcinfo->location.line++;
                    lexer->srcinfo->location.col = 1;
                }
            }

            // Try again
            continue;
        }

        // No ws nor comments, just break
        break;
    }
}

/*
 * Function: create_token
 *  Creates a new token object that starts at the lexer's internal pointer position
 *  and the length is determined by the *chars* variable. This function advances the pointer
 *  (consumes) *chars* time.
 *
 * Parameters:
 *  lexer - Lexer object
 *  type - Token's type.
 *  chars - Number of characters to consume as part of the token.
 *  line - Token's line number.
 *  col - Token's column number.
 *
 * Returns:
 *  struct GkmlToken - Token object
 *
 */
static inline struct GkmlToken create_token(struct GkmlLexer *lexer, enum GkmlTokenType type, size_t chars)
{
    // Current pointer position
    FlByte *starts = (FlByte*)lexer->srcinfo->source.content + lexer->index;    

    struct GkmlToken token = { 
        .type = type,
        .value = fl_slice_new(starts, sizeof(char), 0, chars),
        .location = lexer->srcinfo->location
    };    

    // Advance the pointer as much as needed
    for (size_t i=0; i < chars; i++)
        consume(lexer);

    return token;
}

/* Public API */

struct GkmlLexer gkml_lexer_new(struct GkmlSourceInfo *srcinfo)
{
    return (struct GkmlLexer) {
        .index = 0,
        .srcinfo = srcinfo
    };
}

struct GkmlToken* gkml_lexer_tokenize(struct GkmlLexer *lexer)
{
    FlVector *tempvec = fl_vector_new_args((struct FlVectorArgs) {
        .writer = fl_container_writer,
        .element_size = sizeof(struct GkmlToken),
        .capacity = 1000
    });

    while (has_input(lexer))
    {
        struct GkmlToken token = gkml_lexer_consume(lexer);

        if (token.type == GKML_TOKEN_EOF)
            break;

        fl_vector_add(tempvec, &token);

        remove_ws_and_comments(lexer);
    }

    struct GkmlToken* tokens = fl_vector_to_array(tempvec);

    fl_vector_free(tempvec);

    return tokens;
}

struct GkmlToken gkml_lexer_consume(struct GkmlLexer *lexer)
{
    remove_ws_and_comments(lexer);

    if (!has_input(lexer))
        return (struct GkmlToken){ .type = GKML_TOKEN_EOF };

    while (has_input(lexer))
    {
        char c = peek(lexer);

        if (c == ';')
        {
            return create_token(lexer, GKML_TOKEN_SEMICOLON, 1);
        }
        else if (c == ',')
        {
            return create_token(lexer, GKML_TOKEN_COMMA, 1);
        }
        else if (c == ':')
        {
            return create_token(lexer, GKML_TOKEN_COLON, 1);
        }
        else if (c == '=')
        {
            return create_token(lexer, GKML_TOKEN_ASSIGN, 1);
        }
        else if (c == '{')
        {
            return create_token(lexer, GKML_TOKEN_LBRACE, 1);
        }
        else if (c == '}')
        {
            return create_token(lexer, GKML_TOKEN_RBRACE, 1);
        }
        else if (c == '[')
        {
            return create_token(lexer, GKML_TOKEN_LBRACKET, 1);
        }
        else if (c == ']')
        {
            return create_token(lexer, GKML_TOKEN_RBRACKET, 1);
        }
        else if (c == '(')
        {
            return create_token(lexer, GKML_TOKEN_LPAREN, 1);
        }
        else if (c == ')')
        {
            return create_token(lexer, GKML_TOKEN_RPAREN, 1);
        }
        else if (c == '&')
        {
            return create_token(lexer, GKML_TOKEN_AMPERSAND, 1);
        }
        else if (c == '%')
        {
            return create_token(lexer, GKML_TOKEN_PERCENTAGE, 1);
        }
        else if (c == 'p' && peek_at(lexer, 1) == 'x')
        {
            return create_token(lexer, GKML_TOKEN_PIXEL, 2);
        }
        else if (c == '#')
        {
            c = consume(lexer);

            // Take as much numbers as possible
            size_t digits = 0;
            do {
                char dc[] = { peek_at(lexer, digits), '\0' };

                if (is_number(dc[0]) || fl_cstring_find("abcdefABCDEF", dc))
                    digits++;
                else break;

            } while (true);

            if (digits > 0)
                return create_token(lexer, GKML_TOKEN_HEX, digits);
        }
        else if (c == '0' && (peek_at(lexer, 1) == 'x' || peek_at(lexer, 1) == 'X'))
        {
            consume(lexer);
            consume(lexer);

            // Take as much numbers as possible
            size_t digits = 0;
            do {
                char dc[] = { peek_at(lexer, digits), '\0' };

                if (dc[0] && (is_number(dc[0]) || fl_cstring_find("abcdefABCDEF", dc)))
                    digits++;
                else break;

            } while (true);

            if (digits > 0)
                return create_token(lexer, GKML_TOKEN_HEX, digits);

            // Continue to process the token as "unknown"
        }
        else if (is_number(c))
        {
            // Take as much numbers as possible
            // FIXME: Hex and binary support (prefix "0x" and suffix "b")
            size_t digits = 1;   
            while (is_number(peek_at(lexer, digits)))
                digits++;

            return create_token(lexer, GKML_TOKEN_INTEGER, digits);
        }
        else if (c == '"' || c == '\'')
        {
            c = consume(lexer);

            const FlByte *string = (const FlByte*)lexer->srcinfo->source.content + lexer->index;;

            size_t chars = 0;
            while (peek(lexer) != c)
            {
                if (peek(lexer) == '\\')
                {
                    consume(lexer); // '\\'
                    chars++;

                    if (peek(lexer) == '\n')
                    {
                        consume(lexer); // '\n'
                        chars++;
                    }
                    else if (peek(lexer) == '\r' && peek_at(lexer, 1) == '\n')
                    {
                        consume(lexer); // '\r'
                        consume(lexer); // '\n'
                        chars += 2;
                    }
                }

                consume(lexer);
                chars++;
            }

            struct GkmlToken token = { 
                .type = GKML_TOKEN_STRING,
                .value = fl_slice_new(string, 1, 0, chars),
                .location = lexer->srcinfo->location
            };

            // Consume the last quote
            consume(lexer);

            return token;
        }
        else if (is_alpha(c))
        {
            size_t chars = 1;
            while (is_number(peek_at(lexer, chars)) || is_alpha(peek_at(lexer, chars)) || peek_at(lexer, chars) == '_' || peek_at(lexer, chars) == '-')
                chars++;

            struct GkmlToken token = create_token(lexer, GKML_TOKEN_ID, chars);

            return token;
        }
        else
        {
            break;
        }
    }

    // If we reach this point, the token is unrecognized by the lexer
    // so we look for a synchronizing character and we return a UNKNOWN
    // token with the whole content
    size_t sync_chars = 0;
    while (!is_sync_character(lexer, sync_chars))
        sync_chars++;

    return create_token(lexer, GKML_TOKEN_UNKNOWN, sync_chars);
}

struct GkmlToken gkml_lexer_peek(struct GkmlLexer *lexer)
{
    // Save the lexer state
    unsigned int index = lexer->index;
    unsigned int line = lexer->srcinfo->location.line;
    unsigned int col = lexer->srcinfo->location.col;

    // Get the next token
    // FIXME: We should buffer consumed tokens, by now
    // for simplicity it is ok to use this
    struct GkmlToken token = gkml_lexer_consume(lexer);
    
    // Restore the lexer state
    lexer->index = index;
    lexer->srcinfo->location.line = line;
    lexer->srcinfo->location.col = col;

    return token;
}
