#include <stddef.h>
#include <stdlib.h>
#include "cmdline.h"

typedef struct cmdline_option_vector_s {
    cmdline_option_t**  buffer;
    unsigned int        size;
} cmdline_option_vector_t;

static void cmdline_option_vector_init(cmdline_option_vector_t* options)
{
    options->buffer     =   malloc(sizeof(cmdline_option_t*));
    *options->buffer    =   NULL;
    options->size       =   0;
}

typedef enum cmdline_option_vector_add_result_s {
    cmdline_option_vector_add_result_success = 0,
    cmdline_option_vector_add_result_failure,
} cmdline_option_vector_add_result_t;

static cmdline_option_vector_add_result_t cmdline_option_vector_push( cmdline_option_vector_t* options
                                                                    , cmdline_option_t* option )
{
    int place   =   options->size;

    options->size   +=  1;

    cmdline_option_t**  new_buffer  =   (cmdline_option_t**)realloc( options->buffer
                                                                   , sizeof(cmdline_option_t*) * options->size );

    if (NULL == new_buffer) {
        return cmdline_option_vector_add_result_failure;
    }

    options->buffer =   new_buffer;

    options->buffer[place]  =   option;
    return cmdline_option_vector_add_result_success;
}

typedef cmdline_option_t**  cmdline_option_vector_iterator_t;

static cmdline_option_vector_iterator_t cmdline_option_vector_begin(cmdline_option_vector_t* options)
{
    return options->buffer;
}

static cmdline_option_vector_iterator_t cmdline_option_vector_end(cmdline_option_vector_t* options)
{
    return options->buffer + options->size;
}

static void cmdline_option_vector_destroy(cmdline_option_vector_t* options)
{
    cmdline_option_vector_iterator_t    begin   =   cmdline_option_vector_begin(options);
    cmdline_option_vector_iterator_t    end     =   cmdline_option_vector_end(options);
    while (begin != end) {
        free(*begin);
        begin   +=  1;
    }
    free(options->buffer);
}

struct cmdline_option_parser_s {
    cmdline_option_vector_t options;
};

cmdline_option_parser_t* cmdline_option_parser_create()
{
    cmdline_option_parser_t*    parser  =   malloc(sizeof(cmdline_option_parser_t));

    if (NULL == parser) {
        return NULL;
    }
    cmdline_option_vector_init(&parser->options);

    return parser;
}

void cmdline_option_parser_destroy(cmdline_option_parser_t* parser)
{
    cmdline_option_vector_destroy(&parser->options);
    free(parser);
}

int cmdline_option_parser_options_count(cmdline_option_parser_t* parser)
{
    return parser->options.size;
}

cmdline_is_option_add_e cmdline_option_parser_add_option( cmdline_option_parser_t* parser
                                                        , cmdline_option_t*        option )
{
    if (NULL == parser) {
        return cmdline_option_add_parser_null;
    }

    if (NULL == option) {
        return cmdline_option_add_wrong_option;
    }

    if (cmdline_option_vector_add_result_failure == cmdline_option_vector_push(&parser->options, option)) {
        return cmdline_option_add_no_memory;
    }

    return cmdline_option_add_success;
}
