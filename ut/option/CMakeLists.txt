set(TITLE cmdline_option_test)

set(SOURCES cmdline_option.cpp)

add_executable(${TITLE} ${SOURCES})
target_link_libraries(${TITLE} ${GTEST_BOTH_LIBRARIES} cmdline pthread)
add_test(${TITLE} ${EXECUTABLE_OUTPUT_PATH}/${TITLE})
