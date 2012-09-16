set(TITLE cmdline)

set( SOURCES cmdline.c
             cmdline_utils.c )

add_library(${TITLE} STATIC ${SOURCES})
