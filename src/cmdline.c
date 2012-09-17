#include "cmdline.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

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
} cmdline_option_parser_free_params_t;

cmdline_option_parser_free_params_t* cmdline_option_parser_free_params_init()
{
    cmdline_option_parser_free_params_t* free_params    =   malloc(sizeof(cmdline_option_parser_free_params_t));

    if (NULL == free_params) {
        return NULL;
    }

    free_params->buffer     =   malloc(sizeof(cmdline_option_t*));
    *free_params->buffer    =   NULL;
    free_params->size       =   0;

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

    return parser;
}

void cmdline_option_parser_destroy(cmdline_option_parser_t* parser)
{
    cmdline_option_vector_destroy(&parser->options);
    cmdline_option_parser_free_params_destroy(parser->free_params);
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
    cmdline_arg_type_short_key,
    cmdline_arg_type_long_key,
    cmdline_arg_type_error
} cmdline_arg_type_e;

cmdline_arg_type_e cmdline_option_parser_classify_arg(const char* param, int length)
{
    char    key_sign    =   '-';

    if (0 == length) {
        return cmdline_arg_type_error;
    }

    if ((1 == length) || (param[0] != key_sign)) {
        return cmdline_arg_type_value;
    }

    if ((2 == length) && (param[0] == key_sign)) {
        return cmdline_arg_type_short_key;
    }

    if ((2 < length) && (param[0] == key_sign) && (param[1] != key_sign)) {
        return cmdline_arg_type_error;
    }

    return cmdline_arg_type_long_key;
}

void cmdline_option_parser_set_value( const char* value
                                    , cmdline_option_parser_t* parser
                                    , cmdline_option_parser_parsing_state_t* state
                                    , cmdline_option_parser_report_t* report )
{
    if (NULL == state->marked_option) {
        cmdline_option_vector_add_result_t  res =   cmdline_option_parser_free_params_push(parser->free_params, value);
        if (cmdline_option_vector_add_result_failure == res) {
            report->option_wth_error.long_key   =   NULL;
            report->option_wth_error.short_key  =   '\0';
            report->argument_index              =   state->arg_index;
            report->status                      =   cmdline_option_parser_status_memory_error;
        }
        return;
    }

    cmdline_cast_arg_result_e   res =   state->marked_option->caster( value
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

    if (0 == strcmp(option->long_key,long_key)) {
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
            case cmdline_arg_type_short_key :
                cmdline_option_parser_set_short_key(param[1], parser, state, report);
                break;
            case cmdline_arg_type_long_key :
                cmdline_option_parser_set_long_key(param + 2, parser, state, report);
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

    int index_of_reuired_and_unmarked   =   cmdline_option_parser_parsing_state_is_all_required_marked(state);

    if (state->options_count != index_of_reuired_and_unmarked) {
        cmdline_get_representation( cmdline_option_vector_at(&parser->options, index_of_reuired_and_unmarked)
                                  , &report->option_wth_error);
        report->argument_index  =   -1;
        report->status          =   cmdline_option_parser_status_no_required_option;
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
    return report;
}

cmdline_option_parser_free_params_iterator_t cmdline_option_parser_free_params_begin(cmdline_option_parser_t* parser)
{
    return cmdline_free_params_begin(parser->free_params);
}

cmdline_option_parser_free_params_iterator_t cmdline_option_parser_free_params_end(cmdline_option_parser_t* parser)
{
    return cmdline_free_params_end(parser->free_params);
}
