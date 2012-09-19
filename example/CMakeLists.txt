set(TITLE example_c)

set(SOURCES example.c)

add_executable(${TITLE} ${SOURCES})
target_link_libraries( ${TITLE} cmdline )
