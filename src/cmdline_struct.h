#pragma once
#include "cmdline.h"

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

    void (*destruct) (void* self);
} cmdline_option_parser_iface_t;


cmdline_option_parser_iface_t*  cmdline_option_parser_iface_construct();

#ifdef __cplusplus
}/*extern "C"*/
#endif
