#include <src/cmdline_utils.h>
#include <stddef.h>

int main(int argc, char** argv)
{
    cmdline_option_parser_t* parser =   cmdline_option_parser_create();
    int flag;
    cmdline_option_parser_add_option(parser, cmdline_flag_create( 'f'
                                                                , "flag"
                                                                , "flag symbolizes something binary"
                                                                , &flag ));
    long int    i;
    cmdline_option_parser_add_option(parser, cmdline_int_option_create( 'I'
                                                                      , NULL
                                                                      , "something you can count"
                                                                      , &i
                                                                      , "0"
                                                                      , cmdline_option_not_required ));
    const char* str;
    cmdline_option_parser_add_option(parser, cmdline_string_option_create( 's'
                                                                         , "string"
                                                                         , "something looks like human word"
                                                                         , &str
                                                                         , NULL
                                                                         , cmdline_option_required ));
    cmdline_option_parser_print_help(parser);
    return 0;
}

