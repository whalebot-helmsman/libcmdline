#include <src/cmdline_utils.h>
#include <stddef.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    int         flag;
    long int    i;
    const char* str;

    cmdline_option_parser_t*    parser  =   cmdline_option_parser_create();

    cmdline_help(parser, 'h' );
    cmdline_flag(parser, 'f'   , "flag"  , "flag symbolizes something binary", &flag);
    cmdline_int (parser, 'I'   , NULL    , "something you can count"         , &i     , "0" , NOT_REQ);
    cmdline_str (parser, 's'   , "string", "something looks like human word" , &str   , NULL, REQ);

    cmdline_parse(parser, argc, argv);
    cmdline_option_parser_destroy(parser);
    return 0;
}

