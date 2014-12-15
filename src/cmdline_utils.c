#include "cmdline_utils.h"
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

cmdline_cast_arg_result_e cmdline_cast_enum_arg( const char* cast_from
                                               , void*       cast_to )
{
    cmdline_enum_mapper_t*  mapper  =   (cmdline_enum_mapper_t*)cast_to;
    int                     is_find =   0;
    int                     iter    =   0;

    while ((0 == is_find) && (mapper->cells_size > iter)) {
        const cmdline_enum_mapper_cell_t*   current_cell    =   &mapper->cells_start[iter];
        if (0 == strcmp(cast_from, current_cell->from)) {
            is_find =   1;
            continue;
        }
        iter    +=  1;
    }

    if (0 == is_find) {
        return cmdline_cast_arg_failure;
    }

    const cmdline_enum_mapper_cell_t*   find_cell   =   &mapper->cells_start[iter];
    *mapper->cast_to    =   find_cell->to;

    return cmdline_cast_arg_success;
}

cmdline_cast_arg_result_e cmdline_cast_int_arg( const char* cast_from
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

cmdline_cast_arg_result_e cmdline_cast_double_arg( const char* cast_from
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
                                , cmdline_cast_int_arg
                                , default_value
                                , required );
}

cmdline_cast_arg_result_e cmdline_cast_string_arg( const char* cast_from
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
                                , cmdline_cast_string_arg
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
                , char                     short_key
                , const char*              long_key
                , const char*              desc
                , void*                    value
                , const char*              default_value
                , cmdline_cast_arg         caster
                , int                      required )
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
                 , char                     short_key
                 , const char*              long_key
                 , const char*              desc
                 , int*                     value )
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
                , char                     short_key
                , const char*              long_key
                , const char*              desc
                , long int*                value
                , const char*              default_value
                , int                      required )
{
    cmdline_opt( parser
               , short_key
               , long_key
               , desc
               , value
               , default_value
               , cmdline_cast_int_arg
               , required );
}

void cmdline_double( cmdline_option_parser_t* parser
                   , char                     short_key
                   , const char*              long_key
                   , const char*              desc
                   , double*                  value
                   , const char*              default_value
                   , int                      required )
{
    cmdline_opt( parser
               , short_key
               , long_key
               , desc
               , value
               , default_value
               , cmdline_cast_double_arg
               , required );
}
void cmdline_str( cmdline_option_parser_t* parser
                , char                     short_key
                , const char*              long_key
                , const char*              desc
                , const char**             value
                , const char*              default_value
                , int                      required )
{
    cmdline_opt( parser
               , short_key
               , long_key
               , desc
               , value
               , default_value
               , cmdline_cast_string_arg
               , required );
}

void cmdline_enum( cmdline_option_parser_t* parser
                 , char                     short_key
                 , const char*              long_key
                 , const char*              desc
                 , cmdline_enum_mapper_t*   mapper
                 , const char*              default_value
                 , int                      required )
{
    cmdline_opt( parser
               , short_key
               , long_key
               , desc
               , mapper
               , default_value
               , cmdline_cast_enum_arg
               , required );
}

void cmdline_memory( cmdline_option_parser_t* parser
                   , char                     short_key
                   , const char*              long_key
                   , const char*              desc
                   , long int*                value
                   , const char*              default_value
                   , int                      required )
{
    cmdline_opt( parser
               , short_key
               , long_key
               , desc
               , value
               , default_value
               , cmdline_cast_memory_arg
               , required );
}

void cmdline_seconds( cmdline_option_parser_t* parser
                    , char                     short_key
                    , const char*              long_key
                    , const char*              desc
                    , long int*                value
                    , const char*              default_value
                    , int                      required )
{
    cmdline_opt( parser
               , short_key
               , long_key
               , desc
               , value
               , default_value
               , cmdline_cast_seconds_arg
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
#if (__GNUC__ > 2) || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#       define PGM_GNUC_UNUSED         __attribute__((__unused__))
#else
#       define PGM_GNUC_UNUSED
#endif

cmdline_cast_arg_result_e dumb_caster( PGM_GNUC_UNUSED const char* cast_from
                                     , PGM_GNUC_UNUSED void* cast_to)
{
    return cmdline_cast_arg_success;
}

typedef struct table_mapper_s {
    const char* postfix;
    long int    multiplier;
} table_mapper_t;

cmdline_cast_arg_result_e cmdline_get_multiplier_from_table( const char*           postfix
                                                           , const table_mapper_t* mapper
                                                           , const long int        mapper_size
                                                           , const long int        default_idx
                                                           , long int*             result_multiplier )
{
    if (default_idx >= mapper_size) {
        return cmdline_cast_arg_failure;
    }

    if ('\0' == *postfix) {
        const table_mapper_t* default_mapper  =   mapper + default_idx;
        *result_multiplier  =   default_mapper->multiplier;
        return cmdline_cast_arg_success;
    }


    const table_mapper_t* found   =   NULL;
    long int                    idx     =   0;

    while ((NULL == found) && (idx < mapper_size)) {
        const table_mapper_t* candidate   =   mapper + idx;

        if (0 == strcmp(candidate->postfix, postfix)) {
            found   =   candidate;
        } else {
            idx     +=  1;
        }
    }

    if (NULL == found) {
        return cmdline_cast_arg_failure;
    }

    *result_multiplier  =   found->multiplier;
    return cmdline_cast_arg_success;
}

cmdline_cast_arg_result_e cmdline_cast_memory_arg( const char* cast_from
                                                 , void*       cast_to )
{
    long int*   value       =   (long int*)cast_to;
    char*       stop_symbol =   NULL;
    long int    memorize    =   strtol(cast_from, &stop_symbol, 10);

    static const table_mapper_t mapper[]    =   { { "b",  1                  }
                                                , { "Kb", 1024               }
                                                , { "Mb", 1024 * 1024        }
                                                , { "Gb", 1024 * 1024 * 1024 } };
    static const long int       mapper_size =   sizeof(mapper) / sizeof(mapper[0]);

    long int                    multiplier      =   0;
    cmdline_cast_arg_result_e   postfix_result  =   cmdline_get_multiplier_from_table( stop_symbol
                                                                                     , mapper
                                                                                     , mapper_size
                                                                                     , 0
                                                                                     , &multiplier );

    if (cmdline_cast_arg_success != postfix_result) {
        return cmdline_cast_arg_failure;
    }

    *value  =   memorize * multiplier;

    return cmdline_cast_arg_success;
}

cmdline_cast_arg_result_e cmdline_cast_seconds_arg( const char* cast_from
                                                  , void*       cast_to )
{
    long int*   value       =   (long int*)cast_to;
    char*       stop_symbol =   NULL;
    long int    memorize    =   strtol(cast_from, &stop_symbol, 10);

    static const table_mapper_t mapper[]    =   { { "sec",  1            }
                                                , { "min",  60           }
                                                , { "hour", 60 * 60      }
                                                , { "day",  60 * 60 * 24 } };
    static const long int       mapper_size =   sizeof(mapper) / sizeof(mapper[0]);

    long int                    multiplier      =   0;
    cmdline_cast_arg_result_e   postfix_result  =   cmdline_get_multiplier_from_table( stop_symbol
                                                                                     , mapper
                                                                                     , mapper_size
                                                                                     , 0
                                                                                     , &multiplier );

    if (cmdline_cast_arg_success != postfix_result) {
        return cmdline_cast_arg_failure;
    }

    *value  =   memorize * multiplier;

    return cmdline_cast_arg_success;
}
