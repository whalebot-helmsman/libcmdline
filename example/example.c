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
    int help;
    cmdline_option_parser_add_option(parser, cmdline_help_option_create('h', &help));
    cmdline_option_parser_report_t result   =   cmdline_option_parser_parse( parser
                                                                           , argc
                                                                           , argv );
    if ((cmdline_option_parser_status_ok != result.status) || (cmdline_flag_set == help)) {
        cmdline_option_parser_print_help(parser);
    }
    cmdline_option_parser_destroy(parser);
    return 0;
}

