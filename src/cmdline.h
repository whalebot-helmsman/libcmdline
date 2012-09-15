#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cmdline_option_s cmdline_option_t;

typedef enum cmdline_cast_arg_result_s {
    cmdline_cast_arg_success = 0,
    cmdline_cast_arg_failure
} cmdline_cast_arg_result_e;

typedef cmdline_cast_arg_result_e (*cmdline_cast_arg) ( const char* cast_from
                                                      , void*       cast_to );

typedef enum cmdline_is_option_required_s {
    cmdline_option_not_required = 0,
    cmdline_option_required
} cmdline_is_option_required_t;

cmdline_option_t* cmdline_option_create( char              short_key
                                       , const char*       long_key
                                       , const char*       desc
                                       , void*             value
                                       , cmdline_cast_arg  caster
                                       , const char*       default_value
                                       , int               required );

typedef enum cmdline_is_flag_set_s {
    cmdline_flag_not_set = 0,
    cmdline_flag_set
} cmdline_is_flag_set_e;

cmdline_option_t* cmdline_flag_create( char        short_key
                                     , const char* long_key
                                     , const char* desc
                                     , int*        flag
                                     , int         required );

cmdline_option_t* cmdline_int_option_create( char        short_key
                                           , const char* long_key
                                           , const char* desc
                                           , long int*   value
                                           , const char* default_value
                                           , int         required );

cmdline_option_t* cmdline_string_option_create( char            short_key
                                              , const char*     long_key
                                              , const char*     desc
                                              , const char**    value
                                              , const char*     default_value
                                              , int             required );

void cmdline_option_destroy(cmdline_option_t* option);

typedef struct cmdline_option_parser_s cmdline_option_parser_t;

cmdline_option_parser_t* cmdline_option_parser_create();
void cmdline_option_parser_destroy(cmdline_option_parser_t* parser);

typedef enum cmdline_is_option_add_s {
    cmdline_option_add_success = 0,
    cmdline_option_add_no_memory,
    cmdline_option_add_parser_null,
    cmdline_option_add_wrong_option,
    cmdline_option_add_short_key_already_exists,
    cmdline_option_add_long_key_already_exists,
    cmdline_option_add_same_option_twice,
    cmdline_option_add_same_value_change
} cmdline_is_option_add_e;

cmdline_is_option_add_e cmdline_option_parser_add_option( cmdline_option_parser_t* parser
                                                        , cmdline_option_t*        option );
void cmdline_option_parser_add_report(cmdline_option_t* option, cmdline_is_option_add_e status);
int cmdline_option_parser_options_count(cmdline_option_parser_t* parser);
void cmdline_option_parser_print_help(cmdline_option_parser_t* parser);

typedef enum cmdline_option_parser_status_s {
    cmdline_option_parser_status_ok = 0,
    cmdline_option_parser_status_unknown_option,
    cmdline_option_parser_status_no_arg,
    cmdline_option_parser_staus_no_required_option,
    cmdline_option_parser_status_wrong_default
} cmdline_option_parser_status_e;

typedef struct cmdline_option_parser_report_s {
    cmdline_option_parser_status_e  status;
    int                             error_place;
    const char*                     option_with_error;
} cmdline_option_parser_report_t;

cmdline_option_parser_report_t cmdline_option_parser_parse(int argc, char** argv);
void cmdline_option_parser_report_print(cmdline_option_parser_report_t report);

#ifdef __cplusplus
}//extern "C"
#endif
