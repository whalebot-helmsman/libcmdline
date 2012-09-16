#pragma once
#include "cmdline.h"

#ifdef __cplusplus
extern "C" {
#endif

cmdline_option_t* cmdline_int_option_create( char        short_key
                                           , const char* long_key
                                           , const char* desc
                                           , long int*   value
                                           , const char* default_value
                                           , int         required );

cmdline_option_t* cmdline_string_option_create( char            short_key
                                              , const char*     long_key
                                              , const char*     desc
                                              , const char**    value
                                              , const char*     default_value
                                              , int             required );

#ifdef __cplusplus
}//extern "C"
#endif
