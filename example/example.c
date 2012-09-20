#include <src/cmdline_utils.h>
#include <stddef.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    int         flag;
    long int    i;
    const char* str;

    cmdline_option_parser_t*    parser  =   cmdline_option_parser_create();

    cmdline_help(parser , 'h' );
    cmdline_flag(parser , 'f' , "flag"   , "flag symbolizes something binary" , &flag);
    cmdline_opt (parser , 'I' , NULL     , "something you can count"          , &i      , "0"  , cast_int_arg    , NOT_REQ);
    cmdline_opt (parser , 's' , "string" , "something looks like human word"  , &str    , NULL , cast_string_arg , REQ);

    if (0 != cmdline_parse(parser, argc, argv)) {
        return 1;
    }

    cmdline_option_parser_destroy(parser);
    return 0;
}

