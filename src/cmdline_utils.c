#include "cmdline_utils.h"
#include <stddef.h>
#include <stdlib.h>

cmdline_cast_arg_result_e cast_int_arg( const char* cast_from
                                      , void*       cast_to )
{
    long int*   value       =   (long int*)cast_to;
    char*       stop_symbol =   NULL;
    long int    memorize    =   strtol(cast_from, &stop_symbol, 10);

    if ('\0' != *stop_symbol) {
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

cmdline_option_t* cmdline_help_option_create( char  short_key
                                            , int*  value )
{
    return cmdline_flag_create( short_key
                              , "help"
                              , "show this message"
                              , value );
}

int cmdline_add_internal( cmdline_option_parser_t* parser
                        , cmdline_option_t*        option )
{

    cmdline_is_option_add_e status = cmdline_option_parser_add_option( parser
                                                                     , option );
    if (cmdline_option_add_success == status) {
        return 1;
    }

    cmdline_option_parser_add_report(option, status);

    if (NULL != option) {
        cmdline_option_destroy(option);
    }

    if (NULL != parser) {
        cmdline_option_parser_destroy(parser);
    }

    return 0;
}

int cmdline_opt( cmdline_option_parser_t* parser
                , char              short_key
                , const char*       long_key
                , const char*       desc
                , long int*         value
                , const char*       default_value
                , cmdline_cast_arg  caster
                , int               required )
{
    cmdline_option_t*   option  =   cmdline_option_create( short_key
                                                         , long_key
                                                         , desc
                                                         , value
                                                         , caster
                                                         , default_value
                                                         , required );
    return cmdline_add_internal(parser, option);

}

int cmdline_flag( cmdline_option_parser_t* parser
                , char        short_key
                , const char* long_key
                , const char* desc
                , int*        value )
{

    cmdline_option_t*   option  =   cmdline_flag_create( short_key
                                                       , long_key
                                                       , desc
                                                       , value );
    return cmdline_add_internal(parser, option);
}
