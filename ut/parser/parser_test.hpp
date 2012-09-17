#include <gtest/gtest.h>
#include <src/cmdline_utils.h>

TEST(ParserTest, you_can_create_parser_wth_no_options)
{
    cmdline_option_parser_t*    not_created =   NULL;
    cmdline_option_parser_t*    create_try  =   cmdline_option_parser_create();
    EXPECT_NE(not_created, create_try);
    EXPECT_EQ(0, cmdline_option_parser_options_count(create_try));
    cmdline_option_parser_destroy(create_try);
}

static cmdline_cast_arg_result_e dumb_caster( const char* /*cast_from*/
                                            , void*       /*cast_to*/ )
{
    return cmdline_cast_arg_success;
}

TEST(ParserTest, you_cannot_add_option_to_emtpy_parser_or_emtpy_option_to_parser)
{
    cmdline_option_t*           not_created =   NULL;
    cmdline_option_parser_t*    create_try  =   cmdline_option_parser_create();

    EXPECT_EQ( cmdline_option_add_wrong_option
             , cmdline_option_parser_add_option(create_try, NULL) );
    cmdline_option_parser_destroy(create_try);

    int                 tester      =   1;
    cmdline_option_t*   option      =   cmdline_option_create( 'a'
                                                             , "aa"
                                                             , "aa desc"
                                                             , static_cast<void*>(&tester)
                                                             , dumb_caster
                                                             , "1"
                                                             , cmdline_option_not_required );
    EXPECT_EQ( cmdline_option_add_parser_null
             , cmdline_option_parser_add_option(NULL, option) );
    cmdline_option_destroy(option);
}

TEST(ParserTest, you_can_add_option_to_parser)
{
    int                 tester      =   1;
    cmdline_option_t*   not_created =   NULL;
    cmdline_option_t*   option      =   cmdline_option_create( 'a'
                                                             , "aa"
                                                             , "aa desc"
                                                             , static_cast<void*>(&tester)
                                                             , dumb_caster
                                                             , "1"
                                                             , cmdline_option_not_required );
    ASSERT_NE(not_created, option);

    cmdline_option_parser_t*    parser              =   cmdline_option_parser_create();
    cmdline_option_parser_t*    parser_not_created  =   NULL;
    ASSERT_NE(parser_not_created, parser);
    ASSERT_EQ(0, cmdline_option_parser_options_count(parser));

    EXPECT_EQ( cmdline_option_add_success
             , cmdline_option_parser_add_option(parser, option) );
    EXPECT_EQ(1, cmdline_option_parser_options_count(parser));
    cmdline_option_parser_destroy(parser);
}

TEST(ParserTest, you_cannot_add_same_option_to_parser_twice)
{
    int                 tester      =   1;
    cmdline_option_t*   not_created =   NULL;
    cmdline_option_t*   option      =   cmdline_option_create( 'a'
                                                             , "aa"
                                                             , "aa desc"
                                                             , static_cast<void*>(&tester)
                                                             , dumb_caster
                                                             , "1"
                                                             , cmdline_option_not_required );
    ASSERT_NE(not_created, option);

    cmdline_option_parser_t*    parser              =   cmdline_option_parser_create();
    cmdline_option_parser_t*    parser_not_created  =   NULL;
    ASSERT_NE(parser_not_created, parser);

    EXPECT_EQ( cmdline_option_add_success
             , cmdline_option_parser_add_option(parser, option) );
    EXPECT_EQ( cmdline_option_add_same_option_twice
             , cmdline_option_parser_add_option(parser, option) );
    cmdline_option_parser_destroy(parser);
}

TEST(ParserTest, you_cannot_add_options_wth_equal_short_keys_to_parser)
{
    int                 tester      =   1;
    int                 tester2     =   1;
    cmdline_option_t*   not_created =   NULL;
    cmdline_option_t*   option      =   cmdline_option_create( 'a'
                                                             , "aa"
                                                             , "aa desc"
                                                             , static_cast<void*>(&tester)
                                                             , dumb_caster
                                                             , "1"
                                                             , cmdline_option_not_required );
    ASSERT_NE(not_created, option);
    cmdline_option_t*   option_wth_same_short   =   cmdline_option_create( 'a'
                                                                         , "not aa"
                                                                         , "aa desc"
                                                                         , static_cast<void*>(&tester2)
                                                                         , dumb_caster
                                                                         , "1"
                                                                         , cmdline_option_not_required );
    ASSERT_NE(not_created, option_wth_same_short);

    cmdline_option_parser_t*    parser              =   cmdline_option_parser_create();
    cmdline_option_parser_t*    parser_not_created  =   NULL;
    ASSERT_NE(parser_not_created, parser);

    EXPECT_EQ( cmdline_option_add_success
             , cmdline_option_parser_add_option(parser, option) );
    EXPECT_EQ( cmdline_option_add_short_key_already_exists
             , cmdline_option_parser_add_option(parser, option_wth_same_short) );
    cmdline_option_parser_destroy(parser);
    cmdline_option_destroy(option_wth_same_short);
}

TEST(ParserTest, you_cannot_add_options_wth_equal_long_keys_to_parser)
{
    int                 tester      =   1;
    int                 tester2     =   1;
    cmdline_option_t*   not_created =   NULL;
    cmdline_option_t*   option      =   cmdline_option_create( 'b'
                                                             , "aa"
                                                             , "aa desc"
                                                             , static_cast<void*>(&tester)
                                                             , dumb_caster
                                                             , "1"
                                                             , cmdline_option_not_required );
    ASSERT_NE(not_created, option);
    cmdline_option_t*   option_wth_same_long    =   cmdline_option_create( 'a'
                                                                         , "aa"
                                                                         , "aa desc"
                                                                         , static_cast<void*>(&tester2)
                                                                         , dumb_caster
                                                                         , "1"
                                                                         , cmdline_option_not_required );
    ASSERT_NE(not_created, option_wth_same_long);

    cmdline_option_parser_t*    parser              =   cmdline_option_parser_create();
    cmdline_option_parser_t*    parser_not_created  =   NULL;
    ASSERT_NE(parser_not_created, parser);

    EXPECT_EQ( cmdline_option_add_success
             , cmdline_option_parser_add_option(parser, option) );
    EXPECT_EQ( cmdline_option_add_long_key_already_exists
             , cmdline_option_parser_add_option(parser, option_wth_same_long) );
    cmdline_option_parser_destroy(parser);
    cmdline_option_destroy(option_wth_same_long);
}

TEST(ParserTest, you_cannot_add_options_change_same_value_to_parser)
{
    int                 tester      =   1;
    cmdline_option_t*   not_created =   NULL;
    cmdline_option_t*   option      =   cmdline_option_create( 'b'
                                                             , "aa"
                                                             , "aa desc"
                                                             , static_cast<void*>(&tester)
                                                             , dumb_caster
                                                             , "1"
                                                             , cmdline_option_not_required );
    ASSERT_NE(not_created, option);
    cmdline_option_t*   option_wth_same_output  =   cmdline_option_create( 'a'
                                                                         , "not aa"
                                                                         , "aa desc"
                                                                         , static_cast<void*>(&tester)
                                                                         , dumb_caster
                                                                         , "1"
                                                                         , cmdline_option_not_required );
    ASSERT_NE(not_created, option_wth_same_output);

    cmdline_option_parser_t*    parser              =   cmdline_option_parser_create();
    cmdline_option_parser_t*    parser_not_created  =   NULL;
    ASSERT_NE(parser_not_created, parser);

    EXPECT_EQ( cmdline_option_add_success
             , cmdline_option_parser_add_option(parser, option) );
    EXPECT_EQ( cmdline_option_add_same_value_change
             , cmdline_option_parser_add_option(parser, option_wth_same_output) );
    cmdline_option_parser_destroy(parser);
    cmdline_option_destroy(option_wth_same_output);
}

TEST(ParserTest, you_can_add_non_conflict_options_to_parser)
{
    int                 tester      =   1;
    int                 tester2     =   1;
    cmdline_option_t*   not_created =   NULL;
    cmdline_option_t*   option      =   cmdline_option_create( 'b'
                                                             , "aa"
                                                             , "aa desc"
                                                             , static_cast<void*>(&tester)
                                                             , dumb_caster
                                                             , "1"
                                                             , cmdline_option_not_required );
    ASSERT_NE(not_created, option);
    cmdline_option_t*   another_option  =   cmdline_option_create( 'a'
                                                                 , "not aa"
                                                                 , "aa desc"
                                                                 , static_cast<void*>(&tester2)
                                                                 , dumb_caster
                                                                 , "1"
                                                                 , cmdline_option_not_required );
    ASSERT_NE(not_created, another_option);

    cmdline_option_parser_t*    parser              =   cmdline_option_parser_create();
    cmdline_option_parser_t*    parser_not_created  =   NULL;
    ASSERT_NE(parser_not_created, parser);

    EXPECT_EQ( cmdline_option_add_success
             , cmdline_option_parser_add_option(parser, option) );
    EXPECT_EQ( cmdline_option_add_success
             , cmdline_option_parser_add_option(parser, another_option) );
    EXPECT_EQ(2, cmdline_option_parser_options_count(parser));
    cmdline_option_parser_destroy(parser);
}

TEST(ParserTest, aaaa_is_a_bad_int_default_value)
{
    long int            tester      =   1;
    cmdline_option_t*   option      =   cmdline_int_option_create( 'b'
                                                                 , "aa"
                                                                 , "aa desc"
                                                                 , &tester
                                                                 , "aaaaa"
                                                                 , cmdline_option_not_required );

    cmdline_option_parser_t*        parser  =   cmdline_option_parser_create();
    cmdline_option_parser_add_option(parser, option);
    cmdline_option_parser_report_t  report  =   cmdline_option_parser_parse(parser, 0, NULL);
    EXPECT_EQ(cmdline_option_parser_status_wrong_default, report.status);
    EXPECT_STREQ("aa", report.option_wth_error.long_key);
    cmdline_option_parser_destroy(parser);
}

TEST(ParserTest, you_cannot_not_pass_required_option)
{
    cmdline_option_parser_t*        parser  =   cmdline_option_parser_create();
    long int                        tester      =   1;
    cmdline_option_parser_add_option(parser, cmdline_int_option_create( 'b'
                                                                      , "aa"
                                                                      , "aa desc"
                                                                      , &tester
                                                                      , NULL
                                                                      , cmdline_option_required ));
    cmdline_option_parser_report_t  report  =   cmdline_option_parser_parse(parser, 0, NULL);
    EXPECT_EQ(cmdline_option_parser_status_no_required_option, report.status);
    EXPECT_STREQ("aa", report.option_wth_error.long_key);
    cmdline_option_parser_destroy(parser);
}

TEST(ParserTest, you_can_pass_required_option)
{
    cmdline_option_parser_t*        parser  =   cmdline_option_parser_create();
    long int                        tester      =   1;
    cmdline_option_parser_add_option(parser, cmdline_int_option_create( 'b'
                                                                      , "aa"
                                                                      , "aa desc"
                                                                      , &tester
                                                                      , NULL
                                                                      , cmdline_option_required ));
    char*   argv[]  =   {"some_command", "-b", "2"};
    int     argc    =   sizeof(argv)/sizeof(argv[0]);

    cmdline_option_parser_report_t  report  =   cmdline_option_parser_parse(parser, argc, argv);
    EXPECT_EQ(cmdline_option_parser_status_ok, report.status);
    EXPECT_EQ(2, tester);
    cmdline_option_parser_destroy(parser);
}

TEST(ParserTest, you_can_pass_required_option_by_long_key)
{
    cmdline_option_parser_t*        parser  =   cmdline_option_parser_create();
    long int                        tester      =   1;
    cmdline_option_parser_add_option(parser, cmdline_int_option_create( 'b'
                                                                      , "aa"
                                                                      , "aa desc"
                                                                      , &tester
                                                                      , NULL
                                                                      , cmdline_option_required ));
    char*   argv[]  =   {"some_command", "--aa", "2"};
    int     argc    =   sizeof(argv)/sizeof(argv[0]);

    cmdline_option_parser_report_t  report  =   cmdline_option_parser_parse(parser, argc, argv);
    EXPECT_EQ(cmdline_option_parser_status_ok, report.status);
    EXPECT_EQ(2, tester);
    cmdline_option_parser_destroy(parser);
}

TEST(ParserTest, you_can_set_flag)
{
    cmdline_option_parser_t*        parser  =   cmdline_option_parser_create();
    int                             tester      =   1;
    cmdline_option_parser_add_option(parser, cmdline_flag_create( 'b'
                                                                , "aa"
                                                                , "aa desc"
                                                                , &tester ));
    char*   argv[]  =   {"some_command", "--aa"};
    int     argc    =   sizeof(argv)/sizeof(argv[0]);

    cmdline_option_parser_report_t  report  =   cmdline_option_parser_parse(parser, argc, argv);
    EXPECT_EQ(cmdline_option_parser_status_ok, report.status);
    EXPECT_EQ(cmdline_flag_set, tester);
    cmdline_option_parser_destroy(parser);
}

TEST(ParserTest, you_cannot_set_same_flag_twice)
{
    cmdline_option_parser_t*        parser  =   cmdline_option_parser_create();
    int                             tester      =   1;
    cmdline_option_parser_add_option(parser, cmdline_flag_create( 'a'
                                                                , "aa"
                                                                , "aa desc"
                                                                , &tester ));
    char*   argv[]  =   {"some_command", "--aa", "-a"};
    int     argc    =   sizeof(argv)/sizeof(argv[0]);

    cmdline_option_parser_report_t  report  =   cmdline_option_parser_parse(parser, argc, argv);
    EXPECT_EQ(cmdline_option_parser_status_meet_option_twice, report.status);
    EXPECT_EQ(cmdline_flag_set, tester);
    cmdline_option_parser_destroy(parser);
}

TEST(ParserTest, you_cannot_pass_unknown_option)
{
    cmdline_option_parser_t*        parser  =   cmdline_option_parser_create();
    int                             tester      =   1;
    char*   argv[]  =   {"some_command", "--aa"};
    int     argc    =   sizeof(argv)/sizeof(argv[0]);

    cmdline_option_parser_report_t  report  =   cmdline_option_parser_parse(parser, argc, argv);
    EXPECT_EQ(cmdline_option_parser_status_unknown_option, report.status);
    EXPECT_EQ(cmdline_flag_set, tester);
    cmdline_option_parser_destroy(parser);
}

TEST(ParserTest, you_cannot_pass_option_wo_value)
{
    cmdline_option_parser_t*        parser  =   cmdline_option_parser_create();
    long int                        tester      =   1;
    cmdline_option_parser_add_option(parser, cmdline_int_option_create( 'b'
                                                                      , "aa"
                                                                      , "aa desc"
                                                                      , &tester
                                                                      , NULL
                                                                      , cmdline_option_required ));
    char*   argv[]  =   {"some_command", "--aa"};
    int     argc    =   sizeof(argv)/sizeof(argv[0]);

    cmdline_option_parser_report_t  report  =   cmdline_option_parser_parse(parser, argc, argv);
    EXPECT_EQ(cmdline_option_parser_status_no_arg, report.status);
    cmdline_option_parser_destroy(parser);
}

TEST(ParserTest, you_cannot_pass_wrong_value)
{
    cmdline_option_parser_t*        parser  =   cmdline_option_parser_create();
    long int                        tester      =   1;
    cmdline_option_parser_add_option(parser, cmdline_int_option_create( 'b'
                                                                      , "aa"
                                                                      , "aa desc"
                                                                      , &tester
                                                                      , NULL
                                                                      , cmdline_option_required ));
    char*   argv[]  =   {"some_command", "--aa", "q"};
    int     argc    =   sizeof(argv)/sizeof(argv[0]);

    cmdline_option_parser_report_t  report  =   cmdline_option_parser_parse(parser, argc, argv);
    EXPECT_EQ(cmdline_option_parser_status_wrong_value, report.status);
    cmdline_option_parser_destroy(parser);
}

TEST(ParserTest, you_cannot_use_long_options_as_short)
{
    cmdline_option_parser_t*        parser  =   cmdline_option_parser_create();
    long int                        tester      =   1;
    cmdline_option_parser_add_option(parser, cmdline_int_option_create( 'b'
                                                                      , "aa"
                                                                      , "aa desc"
                                                                      , &tester
                                                                      , NULL
                                                                      , cmdline_option_required ));
    char*   argv[]  =   {"some_command", "-ba"};
    int     argc    =   sizeof(argv)/sizeof(argv[0]);

    cmdline_option_parser_report_t  report  =   cmdline_option_parser_parse(parser, argc, argv);
    EXPECT_EQ(cmdline_option_parser_status_wrong_option_format, report.status);
    cmdline_option_parser_destroy(parser);
}

TEST(ParserTest, you_cannot_use_free_params)
{
    cmdline_option_parser_t*        parser  =   cmdline_option_parser_create();
    char*   argv[]  =   {"some_command", "ba", "ab", "bc"};
    int     argc    =   sizeof(argv)/sizeof(argv[0]);

    cmdline_option_parser_report_t  report  =   cmdline_option_parser_parse(parser, argc, argv);
    EXPECT_EQ(cmdline_option_parser_status_ok, report.status);
    cmdline_option_parser_free_params_iterator_t    begin   =   cmdline_option_parser_free_params_begin(parser);
    cmdline_option_parser_free_params_iterator_t    end     =   cmdline_option_parser_free_params_end(parser);
    EXPECT_EQ(3, end - begin);
    cmdline_option_parser_destroy(parser);
}
