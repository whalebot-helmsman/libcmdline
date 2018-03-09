libcmdline
==========
pure-c library to handle command line arguments. MIT-licenced

Usage
=====
See [example.c](https://bitbucket.org/whalebot_helmsman/libcmdline/src/tip/example/example.c?at=default)


```c
    cmdline_option_parser_iface_t*  parser  =   cmdline_option_parser_iface_construct();
    parser->set_program_description(parser, "program to demonstrate abilities of libcmdline library");
    parser->set_free_params_description(parser, "why you need free params");
    parser->set_program_example(parser, parser->format(parser, "%s -e two -s smth asas", argv[0]));
    parser->set_free_params_requirement(parser, parser->REQUIRED);


    parser->add_sect(parser, "options start");
    parser->add_help_flag(parser, 'h');
    parser->add_flag(parser, "f,flag"  , "flag symbolizes something binary", &flag);
    parser->add_int(parser,  "I"       , "something you can count"         , &i   , parser->format(parser, "%i", 2 * 5), parser->NOT_REQUIRED);
    parser->add_str(parser,  "string,s", "something looks like human word" , &str , NULL,                                parser->REQUIRED);

    parser->add_memory(parser,  "memory,m",  "some computer size - Gb, Mb, Kb, b or no postfix supported" ,       &memory ,  "10", parser->NOT_REQUIRED);
    parser->add_seconds(parser, "seconds,S", "some time duration - sec, min, hour, day or no postfix supported" , &seconds , "10", parser->NOT_REQUIRED);


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
```

If there is some errors in command line parameters you will see help message

```bash
    option required: "string,s"
    Description: program to demonstrate abilities of libcmdline library
    Example    : bin/example_c -e two -s smth asas

    option start:
        -h, --help, [flag]          show this message
        -f, --flag, [flag]          flag symbolizes something binary
        -I                          something you can count [= 10]
        -s, --string, [required]    something looks like human word
        -m, --memory                some computer size - Gb, Mb, Kb, b or no postfix supported [= 10]
        -S, --seconds               some time duration - sec, min, hour, day or no postfix supported [= 10]
    last option:
        -e, --enum                  something enumerated (zero, one, two, three) [= one]
    free params: why you need free params
```

Install
============
Building in source root run next commands

```bash
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ../
    make
    #you need to be root to install library in your system
    su -c 'make install'
```


Requirements
============
This library is requirenments free, you only need c/c++ compiler, cmake and make

[![Build Status](https://travis-ci.org/whalebot-helmsman/libcmdline.svg?branch=master)](https://travis-ci.org/whalebot-helmsman/libcmdline) Testing
=======

For testing you need [gtest](http://code.google.com/p/googletest/).

After build you need to run

    :::bash
    make test


