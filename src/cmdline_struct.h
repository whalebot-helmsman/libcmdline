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

    void (*destruct) (void* self);
} cmdline_option_parser_iface_t;


cmdline_option_parser_iface_t*  cmdline_option_parser_iface_construct();

#ifdef __cplusplus
}/*extern "C"*/
#endif
