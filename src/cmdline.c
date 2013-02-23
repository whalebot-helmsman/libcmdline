#include "cmdline.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

int cmdline_option_validate_long_key(const char* long_key)
{
    if (NULL == long_key) {
        return 1;
    }

    int length  =   strlen(long_key);
    if (0 == length) {
        return 0;
    }

    if ('-' == long_key[0]) {
        return 0;
    }

    if (NULL != strchr(long_key, '=')) {
        return 0;
    }

    int idx     =   0;
    int is_ok   =   1;
    while ((1 == is_ok) && (idx != length)) {
        if (' ' == long_key[idx]) {
            is_ok   =   0;
        }
        else {
            idx +=  1;
        }
    }

    return is_ok;
}
static cmdline_option_t* cmdline_option_create_internal( char              short_key
                                                       , const char*       long_key
                                                       , const char*       desc
                                                       , void*             value
                                                       , cmdline_cast_arg  caster
                                                       , const char*       default_value
                                                       , int               is_use_param
                                                       , int               required )
{
    if (cmdline_option_forbiddien == required) {
        return NULL;
    }

    if (('\0' == short_key) && (NULL == long_key)) {
        return NULL;
    }

    if (0 == cmdline_option_validate_long_key(long_key)) {
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
    cmdline_option_vector_add_result_failure
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

typedef int (*condition_checker) (cmdline_option_t* option, void* context);

static cmdline_option_vector_iterator_t cmdline_option_vector_find( cmdline_option_vector_t* options
                                                                  , condition_checker checker
                                                                  , void* context )
{
    cmdline_option_vector_iterator_t    begin       =   cmdline_option_vector_begin(options);
    cmdline_option_vector_iterator_t    end         =   cmdline_option_vector_end(options);
    int                                 is_find     =   0;

    while ((0 == is_find) && (begin != end)) {
        cmdline_option_t*   option  =   *begin;
        if (1 == checker(option, context)) {
             is_find    =   1;
        }
        else {
            begin   +=  1;
        }
    }

    return begin;
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

typedef struct cmdline_option_parser_free_params_s {
    const char**    buffer;
    int             size;
    int             is_required;
    const char*     description;
} cmdline_option_parser_free_params_t;

cmdline_option_parser_free_params_t* cmdline_option_parser_free_params_init()
{
    cmdline_option_parser_free_params_t* free_params    =   malloc(sizeof(cmdline_option_parser_free_params_t));

    if (NULL == free_params) {
        return NULL;
    }

    free_params->buffer         =   malloc(sizeof(cmdline_option_t*));
    *free_params->buffer        =   NULL;
    free_params->size           =   0;
    free_params->is_required    =   cmdline_option_not_required;
    free_params->description    =   NULL;

    return free_params;
}

void cmdline_option_parser_free_params_destroy(cmdline_option_parser_free_params_t* free_params)
{
    if (NULL == free_params) {
        return;
    }

    if (NULL != free_params->buffer) {
        free(free_params->buffer);
    }

    free(free_params);
}

cmdline_option_vector_add_result_t cmdline_option_parser_free_params_push( cmdline_option_parser_free_params_t* free_params
                                                                         , const char* param )
{
    int place   =   free_params->size;

    free_params->size   +=  1;

    const char**  new_buffer  =   realloc( free_params->buffer
                                         , sizeof(cmdline_option_t*) * free_params->size );

    if (NULL == new_buffer) {
        return cmdline_option_vector_add_result_failure;
    }

    free_params->buffer =   new_buffer;

    free_params->buffer[place]  =   param;
    return cmdline_option_vector_add_result_success;
}

cmdline_option_parser_free_params_iterator_t cmdline_free_params_begin(cmdline_option_parser_free_params_t* free_params)
{
    return free_params->buffer;
}

cmdline_option_parser_free_params_iterator_t cmdline_free_params_end(cmdline_option_parser_free_params_t* free_params)
{
    return free_params->buffer + free_params->size;
}

struct cmdline_option_parser_s {
    cmdline_option_vector_t                 options;
    cmdline_option_parser_free_params_t*    free_params;
    int                                     is_help_asked;
    const char*                             description;
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

    parser->free_params =   cmdline_option_parser_free_params_init();

    if (NULL == parser->free_params) {
        cmdline_option_parser_destroy(parser);
        return NULL;
    }

    parser->is_help_asked           =   cmdline_flag_not_set;
    parser->description             =   NULL;

    return parser;
}

void cmdline_option_parser_destroy(cmdline_option_parser_t* parser)
{
    cmdline_option_vector_destroy(&parser->options);
    cmdline_option_parser_free_params_destroy(parser->free_params);
    free(parser);
}

int* cmdline_option_parser_help_flag(cmdline_option_parser_t* parser)
{
    return &parser->is_help_asked;
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

        if (('\0' != option->short_key) && ((*begin)->short_key == option->short_key)) {
            result  =   cmdline_option_add_short_key_already_exists;
            continue;
        }

        if (  (NULL != option->long_key)
           && (NULL != (*begin)->long_key)
           && (0 == strcmp((*begin)->long_key, option->long_key)) ) {
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

void cmdline_option_parser_add_description( cmdline_option_parser_t* parser
                                          , const char*              description )
{
    parser->description =   description;
}

void cmdline_option_parser_add_free_params_description( cmdline_option_parser_t* parser
                                                      , const char*              description )
{
    parser->free_params->description    =   description;
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
    int                 arg_index;
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
    /* as argv[0] is command, not param*/
    state->arg_index        =   1;

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
        i   +=  1;
    }

    return index_of_unmarked;
}

typedef enum cmdline_arg_type_s {
    cmdline_arg_type_value,
    cmdline_arg_type_long_key,
    cmdline_arg_type_error,
    cmdline_arg_type_long_key_wth_value,
    cmdline_arg_type_short_key_complex
} cmdline_arg_type_e;

cmdline_arg_type_e cmdline_option_parser_classify_arg(const char* param, int length)
{
    char    key_sign    =   '-';
    char    equal_sign  =   '=';

    if (0 == length) {
        return cmdline_arg_type_error;
    }

    if ((1 == length) || (param[0] != key_sign)) {
        return cmdline_arg_type_value;
    }

    if ((2 <= length) && (param[0] == key_sign) && (param[1] != key_sign)) {
        return cmdline_arg_type_short_key_complex;
    }

    if (NULL != strchr(param, equal_sign)) {
        return cmdline_arg_type_long_key_wth_value;
    }

    return cmdline_arg_type_long_key;
}

static const char* remove_dash_escaping(const char* value)
{
    if (value[0] != '/') {
        return value;
    }

    const char* value_iter   =   value + 1;
    while ((*value_iter) && (*value_iter == '/')) {
        value_iter  +=  1;
    }

    if (*value_iter == '-') {
        return value + 1;
    }

    return value;
}

void cmdline_option_parser_set_value( const char* value
                                    , cmdline_option_parser_t* parser
                                    , cmdline_option_parser_parsing_state_t* state
                                    , cmdline_option_parser_report_t* report )
{

    const char* unescaped_value =   remove_dash_escaping(value);

    if (NULL == state->marked_option) {

        if (cmdline_option_forbiddien == parser->free_params->is_required) {
            report->option_wth_error.long_key   =   NULL;
            report->option_wth_error.short_key  =   '\0';
            report->argument_index              =   state->arg_index;
            report->status                      =   cmdline_option_parser_status_unexpected_free_params;
            return;
        }

        cmdline_option_vector_add_result_t  res =   cmdline_option_parser_free_params_push(parser->free_params, unescaped_value);
        if (cmdline_option_vector_add_result_failure == res) {
            report->option_wth_error.long_key   =   NULL;
            report->option_wth_error.short_key  =   '\0';
            report->argument_index              =   state->arg_index;
            report->status                      =   cmdline_option_parser_status_memory_error;
        }
        return;
    }

    cmdline_cast_arg_result_e   res =   state->marked_option->caster( unescaped_value
                                                                    , state->marked_option->value );
    if (cmdline_cast_arg_failure == res) {
        cmdline_get_representation(state->marked_option, &report->option_wth_error);
        report->argument_index  =   state->arg_index;
        report->status          =   cmdline_option_parser_status_wrong_value;
        return;
    }

    state->marked_option    =   NULL;
}

void cmdline_option_parser_set_option( cmdline_option_vector_iterator_t option_pos
                                     , cmdline_option_parser_t* parser
                                     , cmdline_option_parser_parsing_state_t* state
                                     , cmdline_option_parser_report_t* report )
{
    if (NULL != state->marked_option) {
        cmdline_get_representation(state->marked_option, &report->option_wth_error);
        report->argument_index  =   state->arg_index - 1;
        report->status          =   cmdline_option_parser_status_no_arg;
        return;
    }

    int                 option_index    =   option_pos - cmdline_option_vector_begin(&parser->options);
    cmdline_option_t*   option          =   *option_pos;

    if (1 == state->is_option_with_this_index_already_marked[option_index]) {
        cmdline_get_representation(option, &report->option_wth_error);
        report->argument_index  =   state->arg_index;
        report->status          =   cmdline_option_parser_status_meet_option_twice;
        return;
    }

    state->is_option_with_this_index_already_marked[option_index]           =   1;
    state->is_option_with_this_index_required_and_not_marked[option_index]  =   0;

    if (option->is_use_param) {
        state->marked_option    =   option;
    }
    else {
        *(int*)option->value    =   cmdline_flag_set;
    }
}

static int cmdline_short_key_locator(cmdline_option_t* option, void* context)
{
    char    short_key   =   *(char*)context;

    if (option->short_key == short_key) {
        return 1;
    }
    else {
        return 0;
    }
}

void cmdline_option_parser_set_short_key( char short_key
                                        , cmdline_option_parser_t* parser
                                        , cmdline_option_parser_parsing_state_t* state
                                        , cmdline_option_parser_report_t* report )
{
    cmdline_option_vector_iterator_t    option_pos  =   cmdline_option_vector_find( &parser->options
                                                                                  , cmdline_short_key_locator
                                                                                  , (void*)&short_key );
    if (cmdline_option_vector_end(&parser->options) == option_pos) {
        report->option_wth_error.short_key  =   short_key;
        report->option_wth_error.long_key   =   NULL;
        report->argument_index              =   state->arg_index;
        report->status                      =   cmdline_option_parser_status_unknown_option;
        return;
    }

    cmdline_option_parser_set_option(option_pos, parser, state, report);
}

static int cmdline_long_key_locator(cmdline_option_t* option, void* context)
{
    const char* long_key   =   (const char*)context;

    if ((NULL != option->long_key) && (0 == strcmp(option->long_key,long_key))) {
        return 1;
    }
    else {
        return 0;
    }
}

void cmdline_option_parser_set_long_key( const char* long_key
                                       , cmdline_option_parser_t* parser
                                       , cmdline_option_parser_parsing_state_t* state
                                       , cmdline_option_parser_report_t* report )
{
    cmdline_option_vector_iterator_t    option_pos  =   cmdline_option_vector_find( &parser->options
                                                                                  , cmdline_long_key_locator
                                                                                  , (void*)long_key );
    if (cmdline_option_vector_end(&parser->options) == option_pos) {
        report->option_wth_error.short_key  =   '\0';
        report->option_wth_error.long_key   =   long_key;
        report->argument_index              =   state->arg_index;
        report->status                      =   cmdline_option_parser_status_unknown_option;
        return;
    }

    cmdline_option_parser_set_option(option_pos, parser, state, report);
}

typedef struct cmdline_buffer_s {
    const char*     buffer;
    unsigned int    size;
} cmdline_buffer_t;

static int cmdline_long_key_wth_value_locator(cmdline_option_t* option, void* context)
{
    cmdline_buffer_t*   long_key    =   (cmdline_buffer_t*)context;

    if (  (NULL != option->long_key)
       && (strlen(option->long_key) == long_key->size)
       && (0 == strncmp(option->long_key, long_key->buffer, long_key->size)) ) {
        return 1;
    }
    else {
        return 0;
    }
}

void cmdline_option_parser_set_long_key_wth_value( const char* long_key_wth_value
                                                 , cmdline_option_parser_t* parser
                                                 , cmdline_option_parser_parsing_state_t* state
                                                 , cmdline_option_parser_report_t* report )
{
    const char* equal_sign_pos  =   strchr(long_key_wth_value, '=');

    if (NULL == equal_sign_pos) {
        report->option_wth_error.short_key  =   '\0';
        report->option_wth_error.long_key   =   long_key_wth_value;
        report->argument_index              =   state->arg_index;
        report->status                      =   cmdline_option_parser_status_wrong_option_format;
        return;
    }

    cmdline_buffer_t                    long_key    =   { long_key_wth_value
                                                        , equal_sign_pos - long_key_wth_value};

    cmdline_option_vector_iterator_t    option_pos  =   cmdline_option_vector_find( &parser->options
                                                                                  , cmdline_long_key_wth_value_locator
                                                                                  , (void*)&long_key );
    if (cmdline_option_vector_end(&parser->options) == option_pos) {
        report->option_wth_error.short_key  =   '\0';
        report->option_wth_error.long_key   =   long_key_wth_value;
        report->argument_index              =   state->arg_index;
        report->status                      =   cmdline_option_parser_status_unknown_option;
        return;
    }

    cmdline_option_parser_set_option(option_pos, parser, state, report);

    if (cmdline_is_reperesentation_set(&report->option_wth_error)) {
        return;
    }

    if (NULL == state->marked_option) {
        report->option_wth_error.short_key  =   '\0';
        report->option_wth_error.long_key   =   long_key_wth_value;
        report->argument_index              =   state->arg_index;
        report->status                      =   cmdline_option_parser_status_wrong_option_format;
        return;
    }

    cmdline_option_parser_set_value(equal_sign_pos + 1, parser, state, report);
}

void cmdline_option_parser_set_short_key_complex( const char* short_key_complex
                                                , cmdline_option_parser_t* parser
                                                , cmdline_option_parser_parsing_state_t* state
                                                , cmdline_option_parser_report_t* report )
{
    if (NULL != state->marked_option) {
        report->option_wth_error.short_key  =   state->marked_option->short_key;
        report->option_wth_error.long_key   =   state->marked_option->long_key;
        report->argument_index              =   state->arg_index;
        report->status                      =   cmdline_option_parser_status_no_arg;
        return;
    }

    const char* complex_iterator    =   short_key_complex;

    while (  (!cmdline_is_reperesentation_set(&report->option_wth_error))
          && ('\0' != *complex_iterator)
          && (NULL == state->marked_option) ) {
        cmdline_option_parser_set_short_key(*complex_iterator, parser, state, report);
        complex_iterator    +=  1;
    }

    if (cmdline_is_reperesentation_set(&report->option_wth_error)) {
        return;
    }

    if ('\0' == *complex_iterator) {
        return;
    }

    if ('=' == *complex_iterator) {
        complex_iterator    +=  1;
    }

    cmdline_option_parser_set_value(complex_iterator, parser, state, report);
}

void cmdline_option_parser_parse_internal( cmdline_option_parser_t* parser
                                         , int argc
                                         , char** argv
                                         , cmdline_option_parser_parsing_state_t* state
                                         , cmdline_option_parser_report_t* report )
{
    while ((!cmdline_is_reperesentation_set(&report->option_wth_error)) && (state->arg_index < argc)) {
        const char*         param   =   argv[state->arg_index];
        int                 length  =   strlen(param);
        cmdline_arg_type_e  type    =   cmdline_option_parser_classify_arg(param, length);

        if (cmdline_arg_type_error == type) {
            report->argument_index              =   state->arg_index;
            report->status                      =   cmdline_option_parser_status_wrong_option_format;
            report->option_wth_error.long_key   =   param;
            continue;
        }

        switch (type) {
            case cmdline_arg_type_value :
                cmdline_option_parser_set_value(param, parser, state, report);
                break;
            case cmdline_arg_type_long_key :
                cmdline_option_parser_set_long_key(param + 2, parser, state, report);
                break;
            case cmdline_arg_type_long_key_wth_value :
                cmdline_option_parser_set_long_key_wth_value(param + 2, parser, state, report);
                break;
            case cmdline_arg_type_short_key_complex :
                cmdline_option_parser_set_short_key_complex(param + 1, parser, state, report);
                break;

            default:
                exit(1);
        }

        state->arg_index    +=  1;
    }

    if (cmdline_is_reperesentation_set(&report->option_wth_error)) {
        return;
    }

    if (NULL != state->marked_option) {
        cmdline_get_representation(state->marked_option, &report->option_wth_error);
        report->argument_index  =   argc - 1;
        report->status          =   cmdline_option_parser_status_no_arg;
        return;
    }

    int index_of_required_and_unmarked  =   cmdline_option_parser_parsing_state_is_all_required_marked(state);

    if (state->options_count != index_of_required_and_unmarked) {
        cmdline_get_representation( cmdline_option_vector_at(&parser->options, index_of_required_and_unmarked)
                                  , &report->option_wth_error);
        report->argument_index  =   -1;
        report->status          =   cmdline_option_parser_status_no_required_option;
        return;
    }

    if (  (cmdline_option_required == parser->free_params->is_required)
       && (cmdline_option_parser_free_params_begin(parser) == cmdline_option_parser_free_params_end(parser)) ) {
        report->option_wth_error.long_key   =   NULL;
        report->option_wth_error.short_key  =   '\0';
        report->argument_index              =   -1;
        report->status                      =   cmdline_option_parser_status_no_required_free_params;
        return;
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

    if (NULL == state) {
        report.status   =   cmdline_option_parser_status_memory_error;
        return report;
    }

    cmdline_option_parser_parse_internal(parser, argc, argv, state, &report);

    cmdline_option_parser_parsing_state_destroy(state);

    if (cmdline_flag_set == parser->is_help_asked) {
        report.status                       =   cmdline_option_parser_status_show_help;
        report.argument_index               =   -1;
        report.option_wth_error.long_key    =   NULL;
        report.option_wth_error.short_key   =   '\0';
    }
    return report;
}

cmdline_option_parser_free_params_iterator_t cmdline_option_parser_free_params_begin(cmdline_option_parser_t* parser)
{
    return cmdline_free_params_begin(parser->free_params);
}

void cmdline_option_parser_set_free_params_requiremnt( cmdline_option_parser_t*     parser
                                                     , cmdline_is_option_required_t required )
{
    parser->free_params->is_required    =   required;
}
cmdline_option_parser_free_params_iterator_t cmdline_option_parser_free_params_end(cmdline_option_parser_t* parser)
{
    return cmdline_free_params_end(parser->free_params);
}

const char*     flag_mark       =   ", [flag]";
const char*     required_mark   =   ", [required]";

unsigned int cmdline_option_key_size(cmdline_option_t* option)
{
    unsigned int    size            =   0;

    /*-K*/
    if ('\0' != option->short_key) {
        size    +=  2;
    }

    /*', ' - beetwen them*/
    if (('\0' != option->short_key) && (NULL != option->long_key)) {
        size    +=  2;
    }

    /*--long-key*/
    if (NULL != option->long_key) {
        size    +=  2;
        size    +=  strlen(option->long_key);
    }

    if (cmdline_do_not_use_pararm == option->is_use_param) {
        size    +=  strlen(flag_mark);
    }

    if (cmdline_option_required == option->required) {
        size    +=  strlen(required_mark);
    }

    return size;
}


unsigned int cmdline_option_description_size(cmdline_option_t* option)
{
    unsigned int    size            =   0;

    if (NULL != option->desc) {
        size    +=  strlen(option->desc);
    }

    /* [= default_value]*/
    if (NULL != option->default_value) {
        size    +=  5;
        size    +=  strlen(option->default_value);
    }

    return size;
}

void cmdline_option_key_print( cmdline_option_t* option
                             , unsigned int keys_max_size )
{
    fprintf(stderr, "\t");
    if ('\0' != option->short_key) {
        fprintf(stderr, "-%c", option->short_key);
    }

    if (('\0' != option->short_key) && (NULL != option->long_key)) {
        fprintf(stderr, ", ");
    }

    if (NULL != option->long_key) {
        fprintf(stderr, "--%s", option->long_key);
    }

    if (cmdline_do_not_use_pararm == option->is_use_param) {
        fprintf(stderr, "%s", flag_mark);
    }

    if (cmdline_option_required == option->required) {
        fprintf(stderr, "%s", required_mark);
    }

    unsigned int    size    =   cmdline_option_key_size(option);
    unsigned int    i;
    for (i = 0; i != keys_max_size - size; ++i) {
        fprintf(stderr, " ");
    }

    fprintf(stderr, "    ");
}

void cmdline_option_description_print( cmdline_option_t* option
                                     , unsigned int desc_max_size )
{
    if (NULL != option->desc) {
        fprintf(stderr, "%s", option->desc);
    }

    if (NULL != option->default_value) {
        fprintf(stderr, " [= %s]", option->default_value);
    }

    unsigned int    size    =   cmdline_option_description_size(option);
    unsigned int    i;
    for (i = 0; i != desc_max_size - size; ++i) {
        fprintf(stderr, " ");
    }

    fprintf(stderr, "\n");
}

void cmdline_option_parser_print_help(cmdline_option_parser_t* parser)
{
    unsigned int    keys_max_size           =   0;
    unsigned int    description_max_size    =   0;

    if (NULL != parser->description) {
        fprintf(stderr, "Description: %s\n", parser->description);
    }

    cmdline_option_vector_iterator_t    begin   =   cmdline_option_vector_begin(&parser->options);
    cmdline_option_vector_iterator_t    end     =   cmdline_option_vector_end(&parser->options);

    cmdline_option_vector_iterator_t    iter    =   begin;
    while (iter != end) {
        cmdline_option_t*   option      =   *iter;
        unsigned int        key_size    =   cmdline_option_key_size(option);
        unsigned int        descr_size  =   cmdline_option_description_size(option);

        if (key_size > keys_max_size) {
            keys_max_size   =   key_size;
        }

        if (descr_size > description_max_size) {
            description_max_size    =   descr_size;
        }

        iter    +=  1;
    }

    iter    =   begin;
    while (iter != end) {
        cmdline_option_t*   option      =   *iter;
        cmdline_option_key_print(option, keys_max_size);
        cmdline_option_description_print(option, description_max_size);
        iter    +=  1;
    }

    if (NULL != parser->free_params->description) {
        fprintf(stderr, "free params: %s\n", parser->free_params->description);
    }
}

const char* cmdline_option_is_option_add_translate(cmdline_is_option_add_e status)
{
    switch(status) {
        case cmdline_option_add_success:
            return "success";
        case cmdline_option_add_no_memory:
            return "memory error";
        case cmdline_option_add_parser_null:
            return "malformed parser";
        case cmdline_option_add_wrong_option:
            return "malformed option";
        case cmdline_option_add_short_key_already_exists:
            return "option with such short key already exists";
        case cmdline_option_add_long_key_already_exists:
            return "option with such long key already exists";
        case cmdline_option_add_same_option_twice:
            return "use the same option twice";
        case cmdline_option_add_same_value_change:
            return "use the same value for two options";
        default:
            return "status unknown";

    }
}

void cmdline_option_to_human(cmdline_option_t* option)
{
    if (NULL != option->long_key) {
        fprintf(stderr, " \"%s\"\n", option->long_key);
    }
    else {
        fprintf(stderr, " \"%c\"\n", option->short_key);
    }
}

void cmdline_option_parser_add_report(cmdline_option_t* option, cmdline_is_option_add_e status)
{
    const char* translation =   cmdline_option_is_option_add_translate(status);
    switch(status) {

        case cmdline_option_add_success:
            fprintf(stderr, "%s ", translation);
            cmdline_option_to_human(option);
            break;

        case cmdline_option_add_no_memory:
            fprintf(stderr, "%s\n", translation);
            break;

        case cmdline_option_add_parser_null:
            fprintf(stderr, "%s\n", translation);
            break;

        case cmdline_option_add_wrong_option:
            fprintf(stderr, "%s\n", translation);
            break;

        case cmdline_option_add_short_key_already_exists:
            fprintf(stderr, "%s \"%c\"\n", translation, option->short_key);
            break;

        case cmdline_option_add_long_key_already_exists:
            fprintf(stderr, "%s \"%s\"\n", translation, option->long_key);
            break;

        case cmdline_option_add_same_option_twice:
            fprintf(stderr, "%s ", translation);
            cmdline_option_to_human(option);
            break;

        case cmdline_option_add_same_value_change:
            fprintf(stderr, "%s ", translation);
            cmdline_option_to_human(option);
            break;
    }
}

const char* cmdline_option_parser_status_to_human(cmdline_option_parser_status_e status)
{
    switch(status) {
        case cmdline_option_parser_status_ok:
            return "success";
        case cmdline_option_parser_status_unknown_option:
            return "option unknown";
        case cmdline_option_parser_status_no_arg:
            return "no argument for oprion";
        case cmdline_option_parser_status_no_required_option:
            return "option required";
        case cmdline_option_parser_status_wrong_default:
            return "non-convertible default for option";
        case cmdline_option_parser_status_meet_option_twice:
            return "option twice";
        case cmdline_option_parser_status_wrong_value:
            return "non-convertible value for option";
        case cmdline_option_parser_status_wrong_option_format:
            return "wrong format for option";
        case cmdline_option_parser_status_memory_error:
            return "memory error";
        case cmdline_option_parser_status_show_help:
            return "Usage:";
        case cmdline_option_parser_status_unexpected_free_params:
            return "free params forbidden, but used";
        case cmdline_option_parser_status_no_required_free_params:
            return "free params required";
        default:
            return "unknown status";
    }
}

void cmdline_option_repr_to_human(cmdline_option_representation_t repr)
{
    if (NULL != repr.long_key) {
        fprintf(stderr, ": \"%s\"\n", repr.long_key);
    }
    else if ('\0' != repr.short_key) {
        fprintf(stderr, ": \"%c\"\n", repr.short_key);
    }
    else {
        fprintf(stderr, "\n");
    }
}

void cmdline_option_parser_report_print( cmdline_option_parser_report_t report
                                       , int argc
                                       , char** argv)
{
    const char* translation =   cmdline_option_parser_status_to_human(report.status);
    fprintf(stderr, "%s", translation);
    cmdline_option_repr_to_human(report.option_wth_error);
    if (-1 != report.argument_index) {
        int i;
        for (i = 0; i != argc; ++i) {
            const char* template    =   " %s";
            if (i == report.argument_index) {
                template    =   " __%s__";
            }
            fprintf(stderr, template, argv[i]);
        }
        fprintf(stderr, "\n");
    }
}
