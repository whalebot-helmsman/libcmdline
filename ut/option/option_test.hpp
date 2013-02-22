#pragma once
#include <src/cmdline_utils.h>

#include <gtest/gtest.h>

TEST(OptionsTest, you_cannot_create_option_or_flag_wth_empty_names)
{
    int                 tester;
    cmdline_option_t*   not_created =   NULL;
    EXPECT_EQ(not_created, cmdline_option_create( '\0'
                                                , NULL
                                                , NULL
                                                , static_cast<void*>(&tester)
                                                , dumb_caster
                                                , NULL
                                                , cmdline_option_not_required ));
    EXPECT_EQ(not_created, cmdline_flag_create( '\0'
                                              , NULL
                                              , NULL
                                              , &tester ));
}

TEST(OptionsTest, you_cannot_create_option_or_flag_wth_null_output)
{
    cmdline_option_t*   not_created =   NULL;
    EXPECT_EQ(not_created, cmdline_option_create( 'a'
                                                , NULL
                                                , NULL
                                                , NULL
                                                , dumb_caster
                                                , NULL
                                                , cmdline_option_not_required ));
    EXPECT_EQ(not_created, cmdline_flag_create( 'a'
                                              , NULL
                                              , NULL
                                              , NULL ));
}

TEST(OptionsTest, you_cannot_create_option_with_forbidden_requirement)
{
    int                 tester;
    cmdline_option_t*   not_created =   NULL;
    EXPECT_EQ(not_created, cmdline_option_create( 'a'
                                                , "aa"
                                                , "aa desc"
                                                , static_cast<void*>(&tester)
                                                , dumb_caster
                                                , "1"
                                                , cmdline_option_forbiddien ));
}

TEST(OptionsTest, you_cannot_create_option_wth_null_caster)
{
    int                 tester;
    cmdline_option_t*   not_created =   NULL;
    EXPECT_EQ(not_created, cmdline_option_create( 'a'
                                                , NULL
                                                , NULL
                                                , static_cast<void*>(&tester)
                                                , NULL
                                                , NULL
                                                , cmdline_option_not_required ));
}

TEST(OptionsTest, you_can_create_option_or_flag_wth_valid_params)
{
    int                 tester;
    cmdline_option_t*   not_created =   NULL;
    cmdline_option_t*   create_try  =   cmdline_option_create( 'a'
                                                             , "aa"
                                                             , "aa desc"
                                                             , static_cast<void*>(&tester)
                                                             , dumb_caster
                                                             , "1"
                                                             , cmdline_option_not_required );

    EXPECT_NE(not_created, create_try);
    cmdline_option_destroy(create_try);

    create_try  =   cmdline_flag_create( 'a'
                                       , "aa"
                                       , "aa desc"
                                       , &tester );
    EXPECT_NE(not_created, create_try);
    cmdline_option_destroy(create_try);
}

TEST(OptionsTest, you_cannot_create_option_with_spaced_long_key)
{
    cmdline_option_t*   not_created =   NULL;
    int                 flag;
    EXPECT_EQ(not_created, cmdline_option_create( 'a'
                                                , "asadas sasdas"
                                                , NULL
                                                , &flag
                                                , dumb_caster
                                                , NULL
                                                , cmdline_option_not_required ));
}

TEST(OptionsTest, you_cannot_create_option_long_key_starting_from_dash)
{
    cmdline_option_t*   not_created =   NULL;
    int                 flag;
    EXPECT_EQ(not_created, cmdline_option_create( 'a'
                                                , "-asadas"
                                                , NULL
                                                , &flag
                                                , dumb_caster
                                                , NULL
                                                , cmdline_option_not_required ));
}
TEST(OptionsTest, you_cannot_create_option_long_key_with_equal_sign)
{
    cmdline_option_t*   not_created =   NULL;
    int                 flag;
    EXPECT_EQ(not_created, cmdline_option_create( 'a'
                                                , "asa=das"
                                                , NULL
                                                , &flag
                                                , dumb_caster
                                                , NULL
                                                , cmdline_option_not_required ));
}
