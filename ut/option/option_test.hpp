#include <gtest/gtest.h>
#include <src/cmdline.h>


static cmdline_cast_arg_result_e dumb_caster( const char* /*cast_from*/
                                            , void*       /*cast_to*/ )
{
    return cmdline_cast_arg_success;
}

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

TEST(optionstest, you_cannot_create_option_with_spaced_long_key)
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

TEST(optionstest, you_cannot_create_option_long_key_starting_from_dash)
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
