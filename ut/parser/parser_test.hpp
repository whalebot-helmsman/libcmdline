#include <gtest/gtest.h>
#include <src/cmdline.h>

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