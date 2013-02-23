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

cmdline_cast_arg_result_e cast_double_arg( const char* cast_from
                                         , void*       cast_to )
{
    double*     value       =   (double*)cast_to;
    char*       stop_symbol =   NULL;
    double      memorize    =   strtod(cast_from, &stop_symbol);

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

void cmdline_add_internal( cmdline_option_parser_t* parser
                         , cmdline_option_t*        option )
{

    cmdline_is_option_add_e status = cmdline_option_parser_add_option( parser
                                                                     , option );
    if (cmdline_option_add_success == status) {
        return;
    }

    cmdline_option_parser_add_report(option, status);

    if (NULL != option) {
        cmdline_option_destroy(option);
    }

    if (NULL != parser) {
        cmdline_option_parser_destroy(parser);
    }

    exit(1);
}

void cmdline_opt( cmdline_option_parser_t* parser
                , char              short_key
                , const char*       long_key
                , const char*       desc
                , void*             value
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
    cmdline_add_internal(parser, option);

}

void cmdline_flag( cmdline_option_parser_t* parser
                 , char        short_key
                 , const char* long_key
                 , const char* desc
                 , int*        value )
{

    cmdline_option_t*   option  =   cmdline_flag_create( short_key
                                                       , long_key
                                                       , desc
                                                       , value );
    cmdline_add_internal(parser, option);
}

void cmdline_help(cmdline_option_parser_t* parser, char short_key)
{
    cmdline_flag( parser
                , short_key
                , "help"
                , "show this message"
                , cmdline_option_parser_help_flag(parser) );
}

void cmdline_int( cmdline_option_parser_t* parser
                , char              short_key
                , const char*       long_key
                , const char*       desc
                , long int*         value
                , const char*       default_value
                , int               required )
{
    cmdline_opt( parser
               , short_key
               , long_key
               , desc
               , value
               , default_value
               , cast_int_arg
               , required );
}

void cmdline_double( cmdline_option_parser_t* parser
                   , char              short_key
                   , const char*       long_key
                   , const char*       desc
                   , double*           value
                   , const char*       default_value
                   , int               required )
{
    cmdline_opt( parser
               , short_key
               , long_key
               , desc
               , value
               , default_value
               , cast_double_arg
               , required );
}
void cmdline_str( cmdline_option_parser_t* parser
                , char              short_key
                , const char*       long_key
                , const char*       desc
                , const char**      value
                , const char*       default_value
                , int               required )
{
    cmdline_opt( parser
               , short_key
               , long_key
               , desc
               , value
               , default_value
               , cast_string_arg
               , required );
}

void cmdline_parse(cmdline_option_parser_t* parser, int argc, char** argv)
{
    cmdline_option_parser_report_t result   =   cmdline_option_parser_parse( parser
                                                                           , argc
                                                                           , argv );
    if (cmdline_option_parser_status_ok != result.status) {
        cmdline_option_parser_report_print(result, argc, argv);
        cmdline_option_parser_print_help(parser);
        cmdline_option_parser_destroy(parser);
        exit(1);
    }
}

cmdline_cast_arg_result_e dumb_caster(const char* cast_from, void* cast_to)
{
    return cmdline_cast_arg_success;
}
