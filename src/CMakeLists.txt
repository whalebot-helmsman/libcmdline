set(TITLE cmdline)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99" )
set( SOURCES cmdline.c
             cmdline_utils.c
             cmdline_struct.c )

add_library(${TITLE} STATIC ${SOURCES})
