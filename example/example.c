#include <src/cmdline_utils.h>
#include <stddef.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    int         flag;
    long int    i;
    const char* str;
    int         help;

    cmdline_option_parser_t* parser =   cmdline_option_parser_create();

    if(1 != cmdline_flag(parser, 'f', "flag", "flag symbolizes something binary", &flag)) {
        return 1;
    }
    if(1 != cmdline_opt(parser, 'I', NULL, "something you can count", &i, "0", cast_int_arg, NOT_REQ)) {
        return 1;
    }
    if(1 != cmdline_opt(parser, 's', "string", "something looks like human word", &str, NULL, cast_string_arg, REQ)) {
        return 1;
    }
    if(1 != cmdline_flag(parser, 'h', "help", "show this message", &help)) {
        return 1;
    }

    cmdline_option_parser_report_t result   =   cmdline_option_parser_parse( parser
                                                                           , argc
                                                                           , argv );
    if (cmdline_flag_set == help) {
        cmdline_option_parser_print_help(parser);
        cmdline_option_parser_destroy(parser);
        return 1;
    }
    if (cmdline_option_parser_status_ok != result.status) {
        cmdline_option_parser_report_print(result, argc, argv);
        cmdline_option_parser_print_help(parser);
        cmdline_option_parser_destroy(parser);
        return 1;
    }

    cmdline_option_parser_destroy(parser);
    return 0;
}

