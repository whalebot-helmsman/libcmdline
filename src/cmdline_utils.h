#pragma once
#include "cmdline.h"
/*
 *
 * WARNING:This interface is deprecated, please use struct from cmdline_struct.h
 *
 */

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

typedef struct cmdline_enum_mapper_cell_s {
    const char* from;
    int         to;
} cmdline_enum_mapper_cell_t;

typedef struct cmdline_enum_mapper_s {
    cmdline_enum_mapper_cell_t* cells_start;
    int                         cells_size;
    int*                        cast_to;
} cmdline_enum_mapper_t;

#define CMDLINE_ENUM_MAPPER(m, t, v) \
    cmdline_enum_mapper_t m = {t, sizeof(t)/sizeof(t[0]), &v};

#define CMDLINE_ENUM_ENUMERATOR1(t) \
    t[0].from

#define CMDLINE_ENUM_ENUMERATOR2(t) \
    CMDLINE_ENUM_ENUMERATOR1(t), t[1].from

#define CMDLINE_ENUM_ENUMERATOR3(t) \
    CMDLINE_ENUM_ENUMERATOR2(t), t[2].from

#define CMDLINE_ENUM_ENUMERATOR4(t) \
    CMDLINE_ENUM_ENUMERATOR3(t), t[3].from

#define CMDLINE_ENUM_ENUMERATOR5(t) \
    CMDLINE_ENUM_ENUMERATOR4(t), t[4].from

#define CMDLINE_ENUM_ENUMERATOR6(t) \
    CMDLINE_ENUM_ENUMERATOR5(t), t[5].from

#define CMDLINE_ENUM_ENUMERATOR7(t) \
    CMDLINE_ENUM_ENUMERATOR6(t), t[6].from

#define CMDLINE_ENUM_ENUMERATOR8(t) \
    CMDLINE_ENUM_ENUMERATOR7(t), t[7].from

#define CMDLINE_ENUM_ENUMERATOR9(t) \
    CMDLINE_ENUM_ENUMERATOR8(t), t[8].from

#define CMDLINE_ENUM_ENUMERATOR10(t) \
    CMDLINE_ENUM_ENUMERATOR9(t), t[9].from

#define CMDLINE_ENUM_DEFAULT(m, idx) \
    m[idx].from

void cmdline_enum( cmdline_option_parser_t* parser
                 , char                     short_key
                 , const char*              long_key
                 , const char*              desc
                 , cmdline_enum_mapper_t*   mapper
                 , const char*              default_value
                 , int                      required );

cmdline_cast_arg_result_e cmdline_cast_int_arg( const char* cast_from
                                              , void*       cast_to );
cmdline_cast_arg_result_e cmdline_cast_double_arg( const char* cast_from
                                                 , void*       cast_to );
cmdline_cast_arg_result_e cmdline_cast_string_arg( const char* cast_from
                                                 , void*       cast_to );
cmdline_cast_arg_result_e cmdline_cast_enum_arg( const char* cast_from
                                               , void*       cast_to );
#define NOT_REQ cmdline_option_not_required
#define REQ cmdline_option_required

void cmdline_parse(cmdline_option_parser_t* parser, int argc, char** argv);

cmdline_cast_arg_result_e dumb_caster(const char* cast_from, void* cast_to);
#ifdef __cplusplus
}/*extern "C"*/
#endif
