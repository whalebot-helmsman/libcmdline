set(TITLE cmdline)

set( SOURCES cmdline.c
             cmdline_utils.c
             cmdline_struct.c )

add_library(${TITLE} STATIC ${SOURCES})
