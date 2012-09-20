#pragma once
#include "cmdline.h"

#ifdef __cplusplus
extern "C" {
#endif

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

cmdline_option_t* cmdline_help_option_create( char  short_key
                                            , int*  value );

int cmdline_opt( cmdline_option_parser_t* parser
                , char              short_key
                , const char*       long_key
                , const char*       desc
                , long int*         value
                , const char*       default_value
                , cmdline_cast_arg  caster
                , int               required );

int cmdline_flag( cmdline_option_parser_t* parser
                , char        short_key
                , const char* long_key
                , const char* desc
                , int*        value );

cmdline_cast_arg_result_e cast_int_arg( const char* cast_from
                                      , void*       cast_to );

cmdline_cast_arg_result_e cast_string_arg( const char* cast_from
                                         , void*       cast_to );
#define NOT_REQ cmdline_option_not_required
#define REQ cmdline_option_required
#ifdef __cplusplus
}/*extern "C"*/
#endif
