set(TITLE cmdline)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99" )
set( SOURCES cmdline.c
             cmdline_utils.c
             cmdline_struct.c )

FILE( GLOB LIBCMDLINE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )
INSTALL( FILES ${LIBCMDLINE_HEADERS} DESTINATION include)

add_library(${TITLE} STATIC ${SOURCES})
INSTALL( TARGETS ${TITLE} DESTINATION lib )
