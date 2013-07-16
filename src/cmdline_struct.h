#pragma once
#include "cmdline.h"
#include "cmdline_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cmdline_option_parser_iface_s {
    cmdline_option_parser_t*    base;

    void (*set_free_params_description) (void* self, const char* description);
    void (*set_free_params_requirement) (void* self, int required);
    cmdline_option_parser_free_params_iterator_t (*free_params_begin) (void* self);
    cmdline_option_parser_free_params_iterator_t (*free_params_end)   (void* self);

    void (*set_program_description) (void* self, const char* description);

    void (*add_help_flag) (void* self, char short_key);
    void (*print_help) (void* self);

    cmdline_option_parser_report_t (*parse) (void* self, int argc, char** argv);
    void (*full_parse) (void* self, int argc, char** argv);
    void (*report) (cmdline_option_parser_report_t report, int argc, char** argv);

    cmdline_is_option_add_e (*add_section) (void* self, const char* section_title);
    void (*add_sect) (void* self, const char* section_title);

    cmdline_is_option_add_e (*add_option) (void* self, cmdline_option_t* option);
    void (*add_opt) ( void*             self
                    , const char*       easy_format
                    , const char*       desc
                    , void*             value
                    , const char*       default_value
                    , cmdline_cast_arg  caster
                    , int               required );

    void (*add_flag) ( void*       self
                     , const char* easy_format
                     , const char* desc
                     , int*        value );

    void (*add_int) ( void*       self
                    , const char* easy_format
                    , const char* desc
                    , long int*   value
                    , const char* default_value
                    , int         required );

    void (*add_str) ( void*        self
                    , const char*  easy_format
                    , const char*  desc
                    , const char** value
                    , const char*  default_value
                    , int          required );

    void (*add_double) ( void*       self
                       , const char* easy_format
                       , const char* desc
                       , double*     value
                       , const char* default_value
                       , int         required );

    void (*add_raw_enum) ( void*                  self
                         , const char* easy_format
                         , const char*            desc
                         , cmdline_enum_mapper_t* mapper
                         , const char*            default_value
                         , int                    required );

    cmdline_is_option_required_e    REQUIRED;
    cmdline_is_option_required_e    NOT_REQUIRED;
    cmdline_is_option_required_e    FORBIDDEN;

    void (*destruct) (void* self);

} cmdline_option_parser_iface_t;


cmdline_option_parser_iface_t*  cmdline_option_parser_iface_construct();

#ifdef __cplusplus
}/*extern "C"*/
#endif
