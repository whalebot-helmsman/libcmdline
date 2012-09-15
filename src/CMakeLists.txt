set(TITLE cmdline)

set( SOURCES cmdline_option.c
             cmdline_parser.c )

add_library(${TITLE} STATIC ${SOURCES})
