libcmdline
==========
pure-c library to handle command line arguments

Usage
=====
See [example.c](https://bitbucket.org/whalebot_helmsman/libcmdline/src/ba05097855e23daad62daf08e70bf2cdca593b48/example/example.c?at=default)
    :::clang
    cmdline_option_parser_iface_t*  parser  =   cmdline_option_parser_iface_construct();
    parser->set_program_description(parser, "program to demonstrate abilities of libcmdline library");
    parser->set_free_params_description(parser, "why you need free params");
    parser->set_free_params_requirement(parser, parser->REQUIRED);

    parser->add_help_flag(parser, 'h');

    parser->add_flag(parser, "f,flag"  , "flag symbolizes something binary", &flag);
    parser->add_int(parser,  "I"       , "something you can count"         , &i   , "0" , parser->NOT_REQUIRED);
    parser->add_str(parser,  "string,s", "something looks like human word" , &str , NULL, parser->REQUIRED);


    cmdline_enum_mapper_cell_t      cells[]     =   { {"zero",   zero}
                                                    , {"one",    one}
                                                    , {"two",    two}
                                                    , { "three", three} };
    cmdline_enum_mapper_t           mapper      =   { cells
                                                    , sizeof(cells)/sizeof(cells[0])
                                                    , &enumerated };
    parser->add_raw_enum(parser,  "enum,e", "something enumerated" , &mapper , "zero", parser->NOT_REQUIRED);

    parser->full_parse(parser, argc, argv);

If there is some errors in command line parameters you will see help message

    :::bash
    option required: "string,s"
    Description: program to demonstrate abilities of libcmdline library
        -h, --help, [flag]          show this message
        -f, --flag, [flag]          flag symbolizes something binary
        -I                          something you can count [= 0]
        -s, --string, [required]    something looks like human word
        -e, --enum                  something enumerated [= zero]
    free params: why you need free params

Building
============
Building in source root run next commands

    :::bash
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ../
    make

Requirements
============
This library is requirenments free, you only need c/c++ compiler, cmake and make

[![Build Status](https://drone.io/bitbucket.org/whalebot_helmsman/libcmdline/status.png)](https://drone.io/bitbucket.org/whalebot_helmsman/libcmdline/latest)Testing
=======

For testing you need [gtest](http://code.google.com/p/googletest/).

After build you need to run

    :::bash
    make test


