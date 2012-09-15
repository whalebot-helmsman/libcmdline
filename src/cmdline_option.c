#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cmdline.h"

typedef enum cmdline_is_use_param_s {
    cmdline_do_not_use_pararm = 0,
    cmdline_use_param
} cmdline_is_use_param_e;


struct cmdline_option_s {
   char                 short_key;
   const char*          long_key;
   const char*          desc;
   void*                value;
   cmdline_cast_arg     caster;
   const char*          default_value;
   int                  is_use_param;
   int                  required;
};

static cmdline_option_t* cmdline_option_create_internal( char              short_key
                                                       , const char*       long_key
                                                       , const char*       desc
                                                       , void*             value
                                                       , cmdline_cast_arg  caster
                                                       , const char*       default_value
                                                       , int               is_use_param
                                                       , int               required )
{
    if (('\0' == short_key) && (NULL == long_key)) {
        return NULL;
    }

    if ((NULL != long_key) && (0 == strlen(long_key))) {
        return NULL;
    }

    if (NULL == value) {
        return NULL;
    }

    cmdline_option_t*   ret =   malloc(sizeof(cmdline_option_t));

    if (NULL == ret) {
        return NULL;
    }

    ret->short_key      =   short_key;
    ret->long_key       =   long_key;
    ret->desc           =   desc;
    ret->value          =   value;
    ret->caster         =   caster;
    ret->default_value  =   default_value;
    ret->is_use_param   =   is_use_param;
    ret->required       =   required;

    return ret;
}

cmdline_option_t* cmdline_option_create( char              short_key
                                       , const char*       long_key
                                       , const char*       desc
                                       , void*             value
                                       , cmdline_cast_arg  caster
                                       , const char*       default_value
                                       , int               required )
{
    if (NULL == caster) {
        return NULL;
    }

    return cmdline_option_create_internal( short_key
                                         , long_key
                                         , desc
                                         , value
                                         , caster
                                         , default_value
                                         , cmdline_use_param
                                         , required );
}


cmdline_option_t* cmdline_flag_create( char        short_key
                                     , const char* long_key
                                     , const char* desc
                                     , int*        flag
                                     , int         required )
{
    return cmdline_option_create_internal( short_key
                                         , long_key
                                         , desc
                                         , (void*)flag
                                         , NULL
                                         , NULL
                                         , cmdline_do_not_use_pararm
                                         , required );
}

cmdline_cast_arg_result_e cast_int_arg( const char* cast_from
                                      , void*       cast_to )
{
    long int*   value       =   (long int*)cast_to;
    char*       stop_symbol =   NULL;
    long int    memorize    =   strtol(cast_from, &stop_symbol, 10);

    if ('\0' != stop_symbol) {
        return cmdline_cast_arg_failure;
    }

    *value  =   memorize;
    return cmdline_cast_arg_success;
}

cmdline_option_t* cmdline_int_option_create( char        short_key
                                           , const char* long_key
                                           , const char* desc
                                           , long int*   value
                                           , const char* default_value
                                           , int         required )
{
    return cmdline_option_create( short_key
                                , long_key
                                , desc
                                , (void*)value
                                , cast_int_arg
                                , default_value
                                , required );
}

cmdline_cast_arg_result_e cast_string_arg( const char* cast_from
                                         , void*       cast_to )
{
    *(const char**)cast_to  =   cast_from;
    return cmdline_cast_arg_success;
}

cmdline_option_t* cmdline_string_option_create( char            short_key
                                              , const char*     long_key
                                              , const char*     desc
                                              , const char**    value
                                              , const char*     default_value
                                              , int             required )
{
    return cmdline_option_create( short_key
                                , long_key
                                , desc
                                , (void*)value
                                , cast_string_arg
                                , default_value
                                , required );
}

void cmdline_option_destroy(cmdline_option_t* option)
{
    free((void*)option);
}
