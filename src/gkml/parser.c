#include <fllib.h>
#include "parser.h"
#include "lexer.h"

/* Public API */
struct GkmlParser gkml_parser_new(struct GkmlSourceInfo *srcinfo)
{
    return (struct GkmlParser){
        .lexer = gkml_lexer_new(srcinfo)
    };
}

bool gkml_parser_has_input(struct GkmlParser *parser)
{
    return gkml_lexer_peek(&parser->lexer).type != GKML_TOKEN_EOF;
}

struct GkmlToken gkml_parser_peek(struct GkmlParser *parser)
{
    return gkml_lexer_peek(&parser->lexer);
}

struct GkmlToken gkml_parser_consume(struct GkmlParser *parser)
{
    return gkml_lexer_consume(&parser->lexer);
}

bool gkml_parser_expects(struct GkmlParser *parser, enum GkmlTokenType type, struct GkmlToken *consumed_token)
{
    struct GkmlToken token = gkml_parser_peek(parser);

    if (token.type != type)
        return false;

    if (consumed_token)
        memcpy(consumed_token, &token, sizeof(struct GkmlToken));

    gkml_parser_consume(parser);
    
    return true;
}

bool gkml_parser_consume_if(struct GkmlParser *parser, enum GkmlTokenType type)
{
    if (gkml_parser_peek(parser).type == type)
    {
        gkml_parser_consume(parser);
        return true;
    }

    return false;
}
