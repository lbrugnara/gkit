#include "parse.h"
#include "parser.h"
#include "ast.h"

#define token_equals(tokenptr, value_str) fl_slice_equals_sequence(&(tokenptr)->value, (FlByte*)(value_str), strlen((value_str)))
#define token_to_string(tokenptr) fl_cstring_dup_n((const char*)(tokenptr)->value.sequence, (tokenptr)->value.length)

static struct GkmlNode* parse_integer_literal(struct GkmlParser *parser, struct GkmlContext *ctx);
static struct GkmlNode* parse_hex_literal(struct GkmlParser *parser, struct GkmlContext *ctx);
static struct GkmlNode* parse_string_literal(struct GkmlParser *parser, struct GkmlContext *ctx);
static struct GkmlNode* parse_value(struct GkmlParser *parser, struct GkmlContext *ctx);
static struct GkmlNode* parse_style_declaration(struct GkmlParser *parser, struct GkmlContext *ctx);
static struct GkmlNode* parse_text_declaration(struct GkmlParser *parser, struct GkmlContext *ctx);
static struct GkmlNode* parse_rect_declaration(struct GkmlParser *parser, struct GkmlContext *ctx);
static struct GkmlNode* parse_element_declaration(struct GkmlParser *parser, struct GkmlContext *ctx);
static struct GkmlRectElementNode* parse_root_declaration(struct GkmlParser *parser, struct GkmlContext *ctx);

static struct GkmlNode* parse_integer_literal(struct GkmlParser *parser, struct GkmlContext *ctx)
{
    struct GkmlToken temp_token;
    if (!gkml_parser_expects(parser, GKML_TOKEN_INTEGER, &temp_token))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting an integer literal");
        return NULL;
    }

    // Create the literal node with the basic information
    struct GkmlLiteralNode *literal_node = gkml_node_literal_new(GKML_LITERAL_INTEGER, GKML_UNIT_NONE);
    literal_node->base.location = temp_token.location;

    // This is the integer parsing logic (unsigned integers in base 10 by now)
    char *endptr;
    unsigned long long temp_int = strtoull((const char*)temp_token.value.sequence, &endptr, 10);
    
    // The token length must be equals to the parsed number
    if ((void*)(temp_token.value.sequence + temp_token.value.length) != (void*)endptr)
        goto range_error;

    // Check for specific strtoull errors
    if ((temp_int == ULLONG_MAX && errno == ERANGE) || (temp_int == 0ULL && errno == EINVAL))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_LARGE_INTEGER, "Integral value is too large");
        goto range_error;
    }

    literal_node->value.integer = temp_int;

    if (gkml_parser_peek(parser).type == GKML_TOKEN_PIXEL)
    {
        gkml_parser_consume(parser);
        literal_node->unit = GKML_UNIT_PIXEL;
    }
    else if (gkml_parser_peek(parser).type == GKML_TOKEN_PERCENTAGE)
    {
        gkml_parser_consume(parser);
        literal_node->unit = GKML_UNIT_PERCENTAGE;
    }

    // Success
    return (struct GkmlNode*)literal_node;

    // Cleanup code for error conditions
    range_error:    gkml_node_free((struct GkmlNode*)literal_node);

    return NULL;
}

static struct GkmlNode* parse_hex_literal(struct GkmlParser *parser, struct GkmlContext *ctx)
{
    struct GkmlToken temp_token;
    if (!gkml_parser_expects(parser, GKML_TOKEN_HEX, &temp_token))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting a hexadecimal value");
        return NULL;
    }

    // Create the literal node with the basic information
    struct GkmlLiteralNode *literal_node = gkml_node_literal_new(GKML_LITERAL_INTEGER, GKML_UNIT_NONE);
    literal_node->base.location = temp_token.location;

    char *endptr;
    unsigned long long temp_int = strtoull((const char*)temp_token.value.sequence, &endptr, 16);
    
    // The token length must be equals to the parsed number
    if ((void*)(temp_token.value.sequence + temp_token.value.length) != (void*)endptr)
        goto range_error;

    // Check for specific strtoull errors
    if ((temp_int == ULLONG_MAX && errno == ERANGE) || (temp_int == 0ULL && errno == EINVAL))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_LARGE_INTEGER, "Integral value is too large");
        goto range_error;
    }

    literal_node->value.integer = temp_int;

    // Success
    return (struct GkmlNode*)literal_node;

    // Cleanup code for error conditions
    range_error:    gkml_node_free((struct GkmlNode*)literal_node);

    return NULL;
}

static struct GkmlNode* parse_string_literal(struct GkmlParser *parser, struct GkmlContext *ctx)
{
    struct GkmlToken temp_token;
    if (!gkml_parser_expects(parser, GKML_TOKEN_STRING, &temp_token))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting string literal");
        return NULL;
    }

    // Create the literal node with the basic information
    struct GkmlLiteralNode *literal_node = gkml_node_literal_new(GKML_LITERAL_STRING, GKML_UNIT_NONE);
    literal_node->base.location = temp_token.location;
    literal_node->value.string = token_to_string(&temp_token);

    return (struct GkmlNode*)literal_node;
}

static struct GkmlNode* parse_value(struct GkmlParser *parser, struct GkmlContext *ctx)
{
    if (gkml_parser_peek(parser).type == GKML_TOKEN_HEX)
        return parse_hex_literal(parser, ctx);

    if (gkml_parser_peek(parser).type == GKML_TOKEN_INTEGER)
        return parse_integer_literal(parser, ctx);

    if (gkml_parser_peek(parser).type == GKML_TOKEN_STRING)
        return parse_string_literal(parser, ctx);

    gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Unexpected token %s. Expecting a value (integer, string, hex value, etc).", 
            gkml_token_print(gkml_parser_peek(parser).type));

    return NULL;
}

static bool parse_attributes(struct GkmlParser *parser, struct GkmlContext *ctx, struct GkmlAttributeNodeMap *map)
{
    struct GkmlToken token;
    if (!gkml_parser_expects(parser, GKML_TOKEN_LBRACE, &token))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting %s received %s", 
            gkml_token_print(GKML_TOKEN_LBRACE),
            gkml_token_print(gkml_parser_peek(parser).type));
        return false;
    }

    while (gkml_parser_has_input(parser))
    {
        struct GkmlToken tmp = gkml_parser_peek(parser);

        if (tmp.type == GKML_TOKEN_RBRACE)
            break;

        struct GkmlToken attr_name;
        if (!gkml_parser_expects(parser, GKML_TOKEN_ID, &attr_name))
        {
            gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting %s received %s", 
                gkml_token_print(GKML_TOKEN_ID),
                gkml_token_print(gkml_parser_peek(parser).type));

            return false;
        }

        if (!gkml_parser_expects(parser, GKML_TOKEN_COLON, &tmp))
        {
            gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting %s received %s", 
                gkml_token_print(GKML_TOKEN_COLON),
                gkml_token_print(gkml_parser_peek(parser).type));

            return false;
        }
        
        struct GkmlNode *value = parse_value(parser, ctx);

        if (value == NULL)
            return false;

        struct GkmlAttributeNode *attr = gkml_node_attribute_new(token_to_string(&attr_name), value);
        attr->base.location = attr_name.location;
        gkml_node_attribute_map_add(map, attr);

        gkml_parser_consume_if(parser, GKML_TOKEN_COMMA);
    }

    if (!gkml_parser_expects(parser, GKML_TOKEN_RBRACE, &token))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting %s received %s", 
            gkml_token_print(GKML_TOKEN_RBRACE),
            gkml_token_print(gkml_parser_peek(parser).type));
        return false;
    }

    return true;
}

static struct GkmlNode* parse_style_declaration(struct GkmlParser *parser, struct GkmlContext *ctx)
{
    struct GkmlToken style_token;
    if (!gkml_parser_expects(parser, GKML_TOKEN_ID, &style_token) || !token_equals(&style_token, "Style"))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting 'Style' identifier");
        return NULL;
    }

    struct GkmlStyleNode *style = gkml_node_style_new();
    style->base.location = style_token.location;

    parse_attributes(parser, ctx, &style->attributes);

    return (struct GkmlNode*)style;
}

static struct GkmlNode* parse_text_declaration(struct GkmlParser *parser, struct GkmlContext *ctx)
{
    struct GkmlToken text_token;
    if (!gkml_parser_expects(parser, GKML_TOKEN_ID, &text_token) || !token_equals(&text_token, "Text"))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting identifier");
        return NULL;
    }

    if (!gkml_parser_expects(parser, GKML_TOKEN_LBRACE, NULL))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting %s received %s", 
            gkml_token_print(GKML_TOKEN_LBRACE),
            gkml_token_print(gkml_parser_peek(parser).type));
        return NULL;
    }

    struct GkmlTextElementNode *text = (struct GkmlTextElementNode*)gkml_node_element_new(GKML_ELEMENT_TEXT);
    text->base_element.base.location = text_token.location;

    while (gkml_parser_has_input(parser))
    {
        struct GkmlToken tmp = gkml_parser_peek(parser);

        if (tmp.type == GKML_TOKEN_RBRACE)
            break;
        
        if (tmp.type != GKML_TOKEN_ID)
            goto error;
        
        if (token_equals(&tmp, "Style"))
        {
            text->base_element.style = (struct GkmlStyleNode*)parse_style_declaration(parser, ctx);
        }
        else if (token_equals(&tmp, "Attributes"))
        {
            gkml_parser_consume(parser);
            if (!parse_attributes(parser, ctx, &text->base_element.attributes))
                goto error;
        }
        else if (token_equals(&tmp, "Content"))
        {
            gkml_parser_consume(parser);
            if (!gkml_parser_expects(parser, GKML_TOKEN_LBRACE, NULL))
            {
                gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting %s received %s", 
                    gkml_token_print(GKML_TOKEN_LBRACE),
                    gkml_token_print(gkml_parser_peek(parser).type));
                return NULL;
            }

            while (gkml_parser_has_input(parser))
            {
                struct GkmlToken tmp = gkml_parser_peek(parser);

                if (tmp.type == GKML_TOKEN_RBRACE)
                    break;

                if (tmp.type != GKML_TOKEN_STRING)
                    goto error;

                struct GkmlNode *string = parse_string_literal(parser, ctx);

                if (string == NULL)
                    goto error;

                text->content = fl_array_append(text->content, &string);
            }

            if (!gkml_parser_expects(parser, GKML_TOKEN_RBRACE, NULL))
            {
                gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting %s received %s", 
                    gkml_token_print(GKML_TOKEN_RBRACE),
                    gkml_token_print(gkml_parser_peek(parser).type));
                goto error;
            }
        }
        else
        {
            goto error;
        }

        gkml_parser_consume_if(parser, GKML_TOKEN_COMMA);
    }

    if (!gkml_parser_expects(parser, GKML_TOKEN_RBRACE, NULL))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting %s received %s", 
            gkml_token_print(GKML_TOKEN_RBRACE),
            gkml_token_print(gkml_parser_peek(parser).type));
        goto error;
    }

    return (struct GkmlNode*)text;

    error: gkml_node_element_free((struct GkmlElementNode*)text);
    return NULL;
}

static struct GkmlNode* parse_rect_declaration(struct GkmlParser *parser, struct GkmlContext *ctx)
{
    struct GkmlToken rect_token;
    if (!gkml_parser_expects(parser, GKML_TOKEN_ID, &rect_token) || !token_equals(&rect_token, "Rect"))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting identifier");
        return NULL;
    }

    if (!gkml_parser_expects(parser, GKML_TOKEN_LBRACE, NULL))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting %s received %s", 
            gkml_token_print(GKML_TOKEN_LBRACE),
            gkml_token_print(gkml_parser_peek(parser).type));
        return NULL;
    }

    struct GkmlRectElementNode *rect = (struct GkmlRectElementNode*)gkml_node_element_new(GKML_ELEMENT_RECT);
    rect->base_element.base.location = rect_token.location;

    while (gkml_parser_has_input(parser))
    {
        struct GkmlToken tmp = gkml_parser_peek(parser);

        if (tmp.type == GKML_TOKEN_RBRACE)
            break;
        
        if (tmp.type != GKML_TOKEN_ID)
            goto error;
        
        if (token_equals(&tmp, "Style"))
        {
            rect->base_element.style = (struct GkmlStyleNode*)parse_style_declaration(parser, ctx);
        }
        else if (token_equals(&tmp, "Attributes"))
        {
            gkml_parser_consume(parser);
            if (!parse_attributes(parser, ctx, &rect->base_element.attributes))
                goto error;
        }
        else
        {
            struct GkmlNode *node = parse_element_declaration(parser, ctx);

            if (node == NULL)
                goto error;

            rect->children = fl_array_append(rect->children, &node);
        }

        gkml_parser_consume_if(parser, GKML_TOKEN_COMMA);
    }

    if (!gkml_parser_expects(parser, GKML_TOKEN_RBRACE, NULL))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting %s received %s", 
            gkml_token_print(GKML_TOKEN_RBRACE),
            gkml_token_print(gkml_parser_peek(parser).type));
        goto error;
    }

    return (struct GkmlNode*)rect;

    error: gkml_node_element_free((struct GkmlElementNode*)rect);
    return NULL;
}

static struct GkmlNode* parse_element_declaration(struct GkmlParser *parser, struct GkmlContext *ctx)
{
    struct GkmlToken element_token = gkml_parser_peek(parser);
    if (element_token.type != GKML_TOKEN_ID)
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting identifier");
        return NULL;
    }

    if (token_equals(&element_token, "Rect"))
        return parse_rect_declaration(parser, ctx);
    else if (token_equals(&element_token, "Text"))
        return parse_text_declaration(parser, ctx);

    return NULL;
}

static struct GkmlRectElementNode* parse_root_declaration(struct GkmlParser *parser, struct GkmlContext *ctx)
{
    struct GkmlToken root_token;
    if (!gkml_parser_expects(parser, GKML_TOKEN_ID, &root_token) || !token_equals(&root_token, "Root"))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting 'Root' identifier");
        return NULL;
    }

    if (!gkml_parser_expects(parser, GKML_TOKEN_LBRACE, NULL))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting %s received %s", 
            gkml_token_print(GKML_TOKEN_LBRACE),
            gkml_token_print(gkml_parser_peek(parser).type));
        return NULL;
    }

    struct GkmlRectElementNode *root = (struct GkmlRectElementNode*)gkml_node_element_new(GKML_ELEMENT_ROOT);
    root->base_element.base.location = root_token.location;

    while (gkml_parser_has_input(parser))
    {
        struct GkmlToken tmp = gkml_parser_peek(parser);

        if (tmp.type == GKML_TOKEN_RBRACE)
            break;
        
        if (tmp.type != GKML_TOKEN_ID)
            goto error;
        
        if (token_equals(&tmp, "Style"))
        {
            root->base_element.style = (struct GkmlStyleNode*)parse_style_declaration(parser, ctx);
        }
        else if (token_equals(&tmp, "Attributes"))
        {
            gkml_parser_consume(parser);
            if (!parse_attributes(parser, ctx, &root->base_element.attributes))
                goto error;
        }
        else
        {
            struct GkmlNode *node = parse_element_declaration(parser, ctx);

            if (node == NULL)
            {
                // If node  is NULL, something happened, so let's synch
                do {
                    struct GkmlToken token = gkml_parser_consume(parser);

                    if (token.type == GKML_TOKEN_SEMICOLON 
                        || token.type == GKML_TOKEN_COMMA
                        || token.type == GKML_TOKEN_RBRACE
                        || token.type == GKML_TOKEN_EOF)
                        break;

                } while (true);

                // Let's try again
                continue;
            }


            root->children = fl_array_append(root->children, &node);
        }

        gkml_parser_consume_if(parser, GKML_TOKEN_COMMA);
    }

    if (!gkml_parser_expects(parser, GKML_TOKEN_RBRACE, NULL))
    {
        gkml_context_error(ctx, ctx->srcinfo->location, GKML_ERROR_SYNTAX, "Expecting %s received %s", 
            gkml_token_print(GKML_TOKEN_RBRACE),
            gkml_token_print(gkml_parser_peek(parser).type));
        goto error;
    }

    return root;

    error: gkml_node_element_free((struct GkmlElementNode*)root);
    return NULL;
}

/*
 * Function: gkml_parse_source
 *  While the parser has input, we call the <parse_root_declaration> function
 *  that is the top-level function that knows how to "traverse" the code
 */
bool gkml_parse_source(struct GkmlContext *ctx)
{
    struct GkmlParser parser = gkml_parser_new(ctx->srcinfo);

    // Create the struct GkmlAst object
    ctx->ast = fl_malloc(sizeof(struct GkmlAst));
    ctx->ast->root = parse_root_declaration(&parser, ctx);

    return !ctx->errors;
}
