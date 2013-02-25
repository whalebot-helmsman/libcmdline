#include <src/cmdline_utils.h>
#include <src/cmdline_struct.h>
#include <stddef.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    int         flag;
    long int    i;
    const char* str;

    cmdline_option_parser_iface_t*  parser  =   cmdline_option_parser_iface_construct();
    parser->set_program_description(parser, "program to demonstrate abilities of libcmdline library");
    parser->set_free_params_description(parser, "why you need free params");
    parser->set_free_params_requirement(parser, REQ);

    cmdline_help(parser->base, 'h');
    cmdline_flag(parser->base, 'f', "flag"  , "flag symbolizes something binary", &flag);
    cmdline_int (parser->base, 'I', NULL    , "something you can count"         , &i   , "0" , NOT_REQ);
    cmdline_str (parser->base, 's', "string", "something looks like human word" , &str , NULL, REQ);

    cmdline_parse(parser->base, argc, argv);

    cmdline_option_parser_free_params_iterator_t    iter    =   parser->free_params_begin(parser);
    cmdline_option_parser_free_params_iterator_t    end     =   parser->free_params_end(parser);
    int                                             number  =   0;

    while (iter != end) {
        fprintf(stdout, "%d: %s\n", number, *iter);
        iter    +=  1;
        number  +=  1;
    }


    parser->destruct(parser);
    return 0;
}

