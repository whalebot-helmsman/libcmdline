#pragma once
#include "cmdline.h"

#ifdef __cplusplus
extern "C" {
#endif

void cmdline_opt( cmdline_option_parser_t* parser
                , char                     short_key
                , const char*              long_key
                , const char*              desc
                , void*                    value
                , const char*              default_value
                , cmdline_cast_arg         caster
                , int                      required );

void cmdline_flag( cmdline_option_parser_t* parser
                 , char                     short_key
                 , const char*              long_key
                 , const char*              desc
                 , int*                     value );

void cmdline_help(cmdline_option_parser_t* parser, char short_key);

void cmdline_int( cmdline_option_parser_t* parser
                , char                     short_key
                , const char*              long_key
                , const char*              desc
                , long int*                value
                , const char*              default_value
                , int                      required );

void cmdline_str( cmdline_option_parser_t* parser
                , char                     short_key
                , const char*              long_key
                , const char*              desc
                , const char**             value
                , const char*              default_value
                , int                      required );

void cmdline_double( cmdline_option_parser_t* parser
                   , char                     short_key
                   , const char*              long_key
                   , const char*              desc
                   , double*                  value
                   , const char*              default_value
                   , int                      required );


cmdline_cast_arg_result_e cast_int_arg( const char* cast_from
                                      , void*       cast_to );
cmdline_cast_arg_result_e cast_double_arg( const char* cast_from
                                         , void*       cast_to );
cmdline_cast_arg_result_e cast_string_arg( const char* cast_from
                                         , void*       cast_to );
#define NOT_REQ cmdline_option_not_required
#define REQ cmdline_option_required

void cmdline_parse(cmdline_option_parser_t* parser, int argc, char** argv);

cmdline_cast_arg_result_e dumb_caster(const char* cast_from, void* cast_to);
#ifdef __cplusplus
}/*extern "C"*/
#endif
