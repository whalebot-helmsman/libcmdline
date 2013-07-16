#include "cmdline_struct.h"

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

cmdline_is_option_add_e cmdline_option_parser_iface_add_option( void*             self
                                                              , cmdline_option_t* option )
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    return cmdline_option_parser_add_option(self_typed->base, option);
}

cmdline_is_option_add_e cmdline_option_parser_iface_add_section( void*       self
                                                               , const char* section_title )
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    return cmdline_option_parser_add_section(self_typed->base, section_title);
}

void cmdline_option_parser_iface_add_section_full( void*       self
                                                 , const char* section_title )
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    cmdline_is_option_add_e         status      =   self_typed->add_section( self_typed
                                                                           , section_title );
    if (cmdline_option_add_success == status) {
        return;
    }

    cmdline_option_parser_section_add_report(section_title, status);


    self_typed->destruct(self_typed);
    exit(1);
}

void cmdline_option_parser_iface_add_option_full_internal( void*             self
                                                         , cmdline_option_t* option )
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    cmdline_is_option_add_e         status      =   self_typed->add_option( self_typed
                                                                          , option );
    if (cmdline_option_add_success == status) {
        return;
    }

    cmdline_option_parser_add_report(option, status);

    if (NULL != option) {
        cmdline_option_destroy(option);
    }

    self_typed->destruct(self_typed);
    exit(1);
}

void cmdline_option_parser_iface_add_opt( void*             self
                                        , const char*       easy_format
                                        , const char*       desc
                                        , void*             value
                                        , const char*       default_value
                                        , cmdline_cast_arg  caster
                                        , int               required )
{
    cmdline_option_t*   option      =   cmdline_option_create_easy_format( easy_format
                                                                         , desc
                                                                         , value
                                                                         , caster
                                                                         , default_value
                                                                         , required );
    cmdline_option_parser_iface_add_option_full_internal(self, option);
}

void cmdline_option_parser_iface_add_flag( void*       self
                                         , const char* easy_format
                                         , const char* desc
                                         , int*        value )
{
    cmdline_option_t*   option  =   cmdline_flag_create_easy_format( easy_format
                                                                   , desc
                                                                   , value );
    cmdline_option_parser_iface_add_option_full_internal(self, option);
}

void cmdline_option_parser_iface_add_int( void*       self
                                        , const char* easy_format
                                        , const char* desc
                                        , long int*   value
                                        , const char* default_value
                                        , int         required )
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    self_typed->add_opt( self
                       , easy_format
                       , desc
                       , value
                       , default_value
                       , cmdline_cast_int_arg
                       , required );
}

void cmdline_option_parser_iface_add_str( void*        self
                                        , const char*  easy_format
                                        , const char*  desc
                                        , const char** value
                                        , const char*  default_value
                                        , int          required )
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    self_typed->add_opt( self
                       , easy_format
                       , desc
                       , value
                       , default_value
                       , cmdline_cast_string_arg
                       , required );
}

void cmdline_option_parser_iface_add_double( void*       self
                                           , const char* easy_format
                                           , const char* desc
                                           , double*     value
                                           , const char* default_value
                                           , int         required )
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    self_typed->add_opt( self
                       , easy_format
                       , desc
                       , value
                       , default_value
                       , cmdline_cast_double_arg
                       , required );
}
void cmdline_option_parser_iface_add_raw_enum( void*                  self
                                             , const char*            easy_format
                                             , const char*            desc
                                             , cmdline_enum_mapper_t* mapper
                                             , const char*            default_value
                                             , int                    required )
{
    cmdline_option_parser_iface_t*  self_typed  =   (cmdline_option_parser_iface_t*)self;
    self_typed->add_opt( self
                       , easy_format
                       , desc
                       , mapper
                       , default_value
                       , cmdline_cast_enum_arg
                       , required );
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
    iface->add_section                  =   cmdline_option_parser_iface_add_section;
    iface->add_sect                     =   cmdline_option_parser_iface_add_section_full;
    iface->add_option                   =   cmdline_option_parser_iface_add_option;
    iface->add_opt                      =   cmdline_option_parser_iface_add_opt;
    iface->add_int                      =   cmdline_option_parser_iface_add_int;
    iface->add_flag                     =   cmdline_option_parser_iface_add_flag;
    iface->add_str                      =   cmdline_option_parser_iface_add_str;
    iface->add_double                   =   cmdline_option_parser_iface_add_double;
    iface->add_raw_enum                 =   cmdline_option_parser_iface_add_raw_enum;

    iface->REQUIRED     =   cmdline_option_required;
    iface->NOT_REQUIRED =   cmdline_option_not_required;
    iface->FORBIDDEN    =   cmdline_option_forbiddien;

    return iface;
}
