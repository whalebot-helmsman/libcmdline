set(TITLE cmdline_parser_test)

set(SOURCES cmdline_parser.cpp)

add_executable(${TITLE} ${SOURCES})
target_link_libraries(${TITLE} ${GTEST_BOTH_LIBRARIES} cmdline pthread)
add_test(${TITLE} ${EXECUTABLE_OUTPUT_PATH}/${TITLE})

