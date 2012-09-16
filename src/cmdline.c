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

static cmdline_option_t* cmdline_option_vector_at( cmdline_option_vector_t* options
                                                 , int index )
{
    return options->buffer[index];
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
    if (NULL == options) {
        return;
    }

    if (NULL == options->buffer) {
        return;
    }

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
    if (NULL == parser->options.buffer) {
        cmdline_option_parser_destroy(parser);
        return NULL;
    }

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

static int cmdline_is_reperesentation_set(cmdline_option_representation_t* repr)
{
    return repr->short_key != '\0' || repr->long_key != NULL;
}

static void cmdline_get_representation( cmdline_option_t* option
                                      , cmdline_option_representation_t* repr )
{
    if (NULL == option->long_key) {
        repr->short_key =   option->short_key;
        repr->long_key  =   NULL;
    }
    else {
        repr->short_key =   '\0';
        repr->long_key  =   option->long_key;
    }
}

static cmdline_option_representation_t cmdline_option_parser_preparse(cmdline_option_parser_t* parser)
{
    cmdline_option_vector_iterator_t    begin   =   cmdline_option_vector_begin(&parser->options);
    cmdline_option_vector_iterator_t    end     =   cmdline_option_vector_end(&parser->options);

    cmdline_option_representation_t     error_option;
    error_option.long_key   =   NULL;
    error_option.short_key  =   '\0';

    while ((!cmdline_is_reperesentation_set(&error_option)) && (begin != end)) {
        cmdline_option_t*           option  =   *begin;

        if (cmdline_do_not_use_pararm == option->is_use_param) {
            /*reset flags*/
            *((int*)option->value)  =   cmdline_flag_not_set;
        }
        else if (NULL != option->default_value) {
            /*set default values*/
            cmdline_cast_arg_result_e   result  =   option->caster( option->default_value
                                                                  , option->value );
            if (cmdline_cast_arg_failure == result) {
                cmdline_get_representation(option, &error_option);
                continue;
            }
        }

        begin   +=  1;
    }
    return error_option;
}

typedef struct cmdline_option_parser_parsing_state_s {
    char*               is_option_with_this_index_already_marked;
    char*               is_option_with_this_index_required_and_not_marked;
    int                 options_count;
    cmdline_option_t*   marked_option;
} cmdline_option_parser_parsing_state_t;

void cmdline_option_parser_parsing_state_destroy(cmdline_option_parser_parsing_state_t* state)
{
    if (NULL == state) {
        return;
    }

    if (NULL != state->is_option_with_this_index_already_marked) {
        free(state->is_option_with_this_index_already_marked);
    }

    if (NULL != state->is_option_with_this_index_required_and_not_marked) {
        free(state->is_option_with_this_index_required_and_not_marked);
    }

    free(state);
}

cmdline_option_parser_parsing_state_t* cmdline_option_parser_parsing_state_init(cmdline_option_parser_t* parser)
{
    int                                     options_count   =   cmdline_option_parser_options_count(parser);
    cmdline_option_parser_parsing_state_t*  state           =   malloc(sizeof(cmdline_option_parser_parsing_state_t));

    if (NULL == state) {
        return NULL;
    }

    state->options_count    =   options_count;
    state->marked_option    =   NULL;

    state->is_option_with_this_index_already_marked             =   malloc(sizeof(char) * state->options_count);
    state->is_option_with_this_index_required_and_not_marked    =   malloc(sizeof(char) * state->options_count);

    if (  (NULL == state->is_option_with_this_index_already_marked)
       || (NULL == state->is_option_with_this_index_required_and_not_marked) ) {
        cmdline_option_parser_parsing_state_destroy(state);
        return NULL;
    }

    memset(state->is_option_with_this_index_already_marked, 0, sizeof(char) * state->options_count);
    memset(state->is_option_with_this_index_required_and_not_marked, 0, sizeof(char) * state->options_count);

    cmdline_option_vector_iterator_t    begin   =   cmdline_option_vector_begin(&parser->options);
    cmdline_option_vector_iterator_t    iter    =   begin;
    cmdline_option_vector_iterator_t    end     =   cmdline_option_vector_end(&parser->options);

    while (iter != end) {
        cmdline_option_t*   option  =   *iter;
        if (option->required) {
            state->is_option_with_this_index_required_and_not_marked[iter - begin]    =   1;
        }
        iter    +=  1;
    }

    return state;
}

int cmdline_option_parser_parsing_state_is_all_required_marked(cmdline_option_parser_parsing_state_t* state)
{
    int index_of_unmarked   =   state->options_count;
    int i                   =   0;
    while ((state->options_count == index_of_unmarked) && (i != state->options_count)) {
        if (1 == state->is_option_with_this_index_required_and_not_marked[i]) {
            index_of_unmarked   =   i;
        }
    }

    return index_of_unmarked;
}

void cmdline_option_parser_parse_internal( cmdline_option_parser_t* parser
                                         , int argc
                                         , char** argv
                                         , cmdline_option_parser_parsing_state_t* state
                                         , cmdline_option_parser_report_t* report )
{
    if (cmdline_is_reperesentation_set(&report->option_wth_error)) {
        return;
    }

    int index_of_reuired_and_unmarked   =   cmdline_option_parser_parsing_state_is_all_required_marked(state);

    if (state->options_count != index_of_reuired_and_unmarked) {
        cmdline_get_representation( cmdline_option_vector_at(&parser->options, index_of_reuired_and_unmarked)
                                  , &report->option_wth_error);
        report->argument_index  =   -1;
        report->status          =   cmdline_option_parser_staus_no_required_option;
    }
}

cmdline_option_parser_report_t cmdline_option_parser_parse( cmdline_option_parser_t* parser
                                                          , int argc
                                                          , char** argv )
{
    cmdline_option_parser_report_t  report;
    report.status                       =   cmdline_option_parser_status_ok;
    report.argument_index               =   -1;
    report.option_wth_error.long_key    =   NULL;
    report.option_wth_error.short_key   =   '\0';

    report.option_wth_error =   cmdline_option_parser_preparse(parser);
    if (cmdline_is_reperesentation_set(&report.option_wth_error)) {
        report.status   =   cmdline_option_parser_status_wrong_default;
        return report;
    }

    cmdline_option_parser_parsing_state_t*  state   =   cmdline_option_parser_parsing_state_init(parser);

    cmdline_option_parser_parse_internal(parser, argc, argv, state, &report);

    cmdline_option_parser_parsing_state_destroy(state);
    return report;
}
