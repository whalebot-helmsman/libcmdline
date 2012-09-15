cmake_minimum_required(VERSION 2.6)

project(LIBCMDLINE)

set(LIBCMDLINE_SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(LIBCMDLINE_BUILD_ROOT ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${LIBCMDLINE_BUILD_ROOT}/lib" )
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${LIBCMDLINE_BUILD_ROOT}/lib" )
set(EXECUTABLE_OUTPUT_PATH         "${LIBCMDLINE_BUILD_ROOT}/bin" )

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -pedantic -Wextra")

include_directories(${LIBCMDLINE_SRC_ROOT})
add_subdirectory(src)

find_package(GTest)
if (GTEST_FOUND)
    enable_testing()
    add_subdirectory(ut)
else(GTEST_FOUND)
    message(STATUS "No gtest found - test not build")
endif(GTEST_FOUND)
