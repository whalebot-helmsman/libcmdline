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

    parser->add_help_flag(parser, 'h');

    parser->add_flag(parser, 'f', "flag"  , "flag symbolizes something binary", &flag);
    parser->add_int(parser,  'I', NULL    , "something you can count"         , &i   , "0" , NOT_REQ);
    parser->add_str(parser,  's', "string", "something looks like human word" , &str , NULL, REQ);

    parser->full_parse(parser, argc, argv);

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

