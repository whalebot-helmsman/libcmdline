#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cmdline.h"

typedef enum cmdline_is_use_param_s {
    cmdline_do_not_use_pararm = 0,
    cmdline_use_param
} cmdline_is_use_param_e;


struct cmdline_option_s {
   char                 short_key;
   const char*          long_key;
   const char*          desc;
   void*                value;
   cmdline_cast_arg     caster;
   const char*          default_value;
   int                  is_use_param;
   int                  required;
};

static cmdline_option_t* cmdline_option_create_internal( char              short_key
                                                       , const char*       long_key
                                                       , const char*       desc
                                                       , void*             value
                                                       , cmdline_cast_arg  caster
                                                       , const char*       default_value
                                                       , int               is_use_param
                                                       , int               required )
{
    if (('\0' == short_key) && (NULL == long_key)) {
        return NULL;
    }

    if ((NULL != long_key) && (0 == strlen(long_key))) {
        return NULL;
    }

    if (NULL == value) {
        return NULL;
    }

    if ((NULL != default_value) && (cmdline_option_required == required)) {
        return NULL;
    }

    cmdline_option_t*   ret =   malloc(sizeof(cmdline_option_t));

    if (NULL == ret) {
        return NULL;
    }

    ret->short_key      =   short_key;
    ret->long_key       =   long_key;
    ret->desc           =   desc;
    ret->value          =   value;
    ret->caster         =   caster;
    ret->default_value  =   default_value;
    ret->is_use_param   =   is_use_param;
    ret->required       =   required;

    return ret;
}

cmdline_option_t* cmdline_option_create( char              short_key
                                       , const char*       long_key
                                       , const char*       desc
                                       , void*             value
                                       , cmdline_cast_arg  caster
                                       , const char*       default_value
                                       , int               required )
{
    if (NULL == caster) {
        return NULL;
    }

    return cmdline_option_create_internal( short_key
                                         , long_key
                                         , desc
                                         , value
                                         , caster
                                         , default_value
                                         , cmdline_use_param
                                         , required );
}


cmdline_option_t* cmdline_flag_create( char        short_key
                                     , const char* long_key
                                     , const char* desc
                                     , int*        flag )
{
    return cmdline_option_create_internal( short_key
                                         , long_key
                                         , desc
                                         , (void*)flag
                                         , NULL
                                         , NULL
                                         , cmdline_do_not_use_pararm
                                         , cmdline_option_not_required );
}

cmdline_cast_arg_result_e cast_int_arg( const char* cast_from
                                      , void*       cast_to )
{
    long int*   value       =   (long int*)cast_to;
    char*       stop_symbol =   NULL;
    long int    memorize    =   strtol(cast_from, &stop_symbol, 10);

    if ('\0' != stop_symbol) {
        return cmdline_cast_arg_failure;
    }

    *value  =   memorize;
    return cmdline_cast_arg_success;
}

cmdline_option_t* cmdline_int_option_create( char        short_key
                                           , const char* long_key
                                           , const char* desc
                                           , long int*   value
                                           , const char* default_value
                                           , int         required )
{
    return cmdline_option_create( short_key
                                , long_key
                                , desc
                                , (void*)value
                                , cast_int_arg
                                , default_value
                                , required );
}

cmdline_cast_arg_result_e cast_string_arg( const char* cast_from
                                         , void*       cast_to )
{
    *(const char**)cast_to  =   cast_from;
    return cmdline_cast_arg_success;
}

cmdline_option_t* cmdline_string_option_create( char            short_key
                                              , const char*     long_key
                                              , const char*     desc
                                              , const char**    value
                                              , const char*     default_value
                                              , int             required )
{
    return cmdline_option_create( short_key
                                , long_key
                                , desc
                                , (void*)value
                                , cast_string_arg
                                , default_value
                                , required );
}

void cmdline_option_destroy(cmdline_option_t* option)
{
    free((void*)option);
}

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

    cmdline_option_vector_iterator_t    begin   =   cmdline_option_vector_begin(&parser->options);
    cmdline_option_vector_iterator_t    end     =   cmdline_option_vector_end(&parser->options);
    cmdline_is_option_add_e             result  =   cmdline_option_add_success;

    while ((cmdline_option_add_success == result) && (begin != end)) {

        if (*begin == option) {
            result  =   cmdline_option_add_same_option_twice;
            continue;
        }

        if ((*begin)->short_key == option->short_key) {
            result  =   cmdline_option_add_short_key_already_exists;
            continue;
        }

        if (0 == strcmp((*begin)->long_key, option->long_key)) {
            result  =   cmdline_option_add_long_key_already_exists;
            continue;
        }

        if ((*begin)->value == option->value) {
            result  =   cmdline_option_add_same_value_change;
            continue;
        }

        begin   +=  1;
    }

    if (cmdline_option_add_success != result) {
        return result;
    }

    if (cmdline_option_vector_add_result_failure == cmdline_option_vector_push(&parser->options, option)) {
        return cmdline_option_add_no_memory;
    }

    return cmdline_option_add_success;
}
