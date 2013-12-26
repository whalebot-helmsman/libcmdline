#include <src/cmdline_utils.h>
#include <src/cmdline_struct.h>
#include <stddef.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    int         flag;
    long int    i;
    const char* str;
    int         enumerated;

    typedef enum something_enumerated_s {
        zero = 0,
        one,
        two,
        three
    } something_enumerated_e;

    cmdline_option_parser_iface_t*  parser  =   cmdline_option_parser_iface_construct();
    parser->set_program_description(parser, "program to demonstrate abilities of libcmdline library");
    parser->set_program_example(parser, parser->format(parser, "%s -e two -s smth asas", argv[0]));
    parser->set_free_params_description(parser, "why you need free params");
    parser->set_free_params_requirement(parser, parser->REQUIRED);


    parser->add_sect(parser, "options start");
    parser->add_help_flag(parser, 'h');
    parser->add_flag(parser, "f,flag"  , "flag symbolizes something binary", &flag);
    parser->add_int(parser,  "I"       , "something you can count"         , &i   , parser->format(parser, "%i", 2 * 5), parser->NOT_REQUIRED);
    parser->add_str(parser,  "string,s", "something looks like human word" , &str , NULL,                                parser->REQUIRED);


    parser->add_sect(parser, "last option");
    cmdline_enum_mapper_cell_t  cells[] =   { {"zero",  zero }
                                            , {"one",   one  }
                                            , {"two",   two  }
                                            , {"three", three} };
    CMDLINE_ENUM_MAPPER(mapper, cells, enumerated);
    parser->add_raw_enum( parser
                        , "enum,e"
                        , parser->format(parser, "something enumerated (%s, %s, %s, %s)", CMDLINE_ENUM_ENUMERATOR4(cells))
                        , &mapper
                        , CMDLINE_ENUM_DEFAULT(cells, 1)
                        , parser->NOT_REQUIRED);

    parser->full_parse(parser, argc, argv);

    fprintf(stdout, "flag              =   %d\n", flag);
    fprintf(stdout, "i                 =   %ld\n", i);
    fprintf(stdout, "str               =   %s\n", str);
    fprintf(stdout, "enumerated        =   %d\n", enumerated);
    cmdline_option_parser_free_params_iterator_t    iter    =   parser->free_params_begin(parser);
    cmdline_option_parser_free_params_iterator_t    end     =   parser->free_params_end(parser);
    int                                             number  =   0;

    while (iter != end) {
        fprintf(stdout, "free_params[%d]    =   %s\n", number, *iter);
        iter    +=  1;
        number  +=  1;
    }


    parser->destruct(parser);
    return 0;
}

