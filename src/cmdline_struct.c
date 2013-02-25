#include "cmdline_struct.h"
#include "cmdline_utils.h"

#include <stdlib.h>

void  cmdline_option_parser_iface_destroy(void* self)
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    cmdline_option_parser_destroy(self_typed->base);
    free(self_typed);
}

void cmdline_option_parser_iface_add_program_description(void* self, const char* description)
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    cmdline_option_parser_add_description(self_typed->base, description);
}

void cmdline_option_parser_iface_add_free_params_description(void* self, const char* description)
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    cmdline_option_parser_add_free_params_description(self_typed->base, description);
}

void cmdline_option_parser_iface_set_free_params_requiremnt(void* self, int required)
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    cmdline_option_parser_set_free_params_requiremnt(self_typed->base, required);
}

cmdline_option_parser_free_params_iterator_t cmdline_option_parser_iface_free_params_begin(void* self)
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    return cmdline_option_parser_free_params_begin(self_typed->base);
}

cmdline_option_parser_free_params_iterator_t cmdline_option_parser_iface_free_params_end(void* self)
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    return cmdline_option_parser_free_params_end(self_typed->base);
}

void cmdline_option_parser_iface_add_help_flag(void* self, char short_key)
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    cmdline_help(self_typed->base, short_key);
}

cmdline_option_parser_report_t cmdline_option_parser_iface_parse(void* self, int argc, char** argv)
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    return cmdline_option_parser_parse(self_typed->base, argc, argv);
}

void cmdline_option_parser_iface_print_help(void* self)
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    cmdline_option_parser_print_help(self_typed->base);
}

void cmdline_option_parser_iface_parse_full(void* self, int argc, char** argv)
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    cmdline_option_parser_report_t  result      =   self_typed->parse(self_typed, argc, argv);
    if (cmdline_option_parser_status_ok != result.status) {
        self_typed->report(result, argc, argv);
        self_typed->print_help(self_typed);
        self_typed->destruct(self_typed);
        exit(1);
    }
}

cmdline_option_parser_iface_t*  cmdline_option_parser_iface_construct()
{
    cmdline_option_parser_iface_t*  iface  =   malloc(sizeof(cmdline_option_parser_iface_t));
    iface->base =   cmdline_option_parser_create();

    if (NULL == iface->base) {
        free(iface);
        return NULL;
    }

    iface->destruct                     =   cmdline_option_parser_iface_destroy;
    iface->set_program_description      =   cmdline_option_parser_iface_add_program_description;
    iface->set_free_params_description  =   cmdline_option_parser_iface_add_free_params_description;
    iface->set_free_params_requirement  =   cmdline_option_parser_iface_set_free_params_requiremnt;
    iface->free_params_end              =   cmdline_option_parser_iface_free_params_end;
    iface->free_params_begin            =   cmdline_option_parser_iface_free_params_begin;
    iface->add_help_flag                =   cmdline_option_parser_iface_add_help_flag;
    iface->parse                        =   cmdline_option_parser_iface_parse;
    iface->report                       =   cmdline_option_parser_report_print;
    iface->full_parse                   =   cmdline_option_parser_iface_parse_full;
    iface->print_help                   =   cmdline_option_parser_iface_print_help;

    return iface;
}
