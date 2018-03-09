# - Try to find cmdline
# Once done this will define
#  CMDLINE_FOUND - System has leveldb
#  CMDLINE_INCLUDE_DIRS - The leveldb include directories
#  CMDLINE_LIBRARIES - The libraries needed to use leveldb
#  CMDLINE_DEFINITIONS - Compiler switches required for using leveldb

find_package(PkgConfig)
pkg_check_modules(PC_CMDLINE QUIET cmdline)
set(CMDLINE_DEFINITIONS ${PC_CMDLINE_CFLAGS_OTHER})

find_path(CMDLINE_INCLUDE_DIR NAMES cmdline.h cmdline_struct.h cmdline_utils.h
    HINTS ${PC_CMDLINE_INCLUDEDIR} ${PC_CMDLINE_INCLUDE_DIRS} )

find_library(CMDLINE_LIBRARY NAMES cmdline
             HINTS ${PC_CMDLINE_LIBDIR} ${PC_CMDLINE_LIBRARY_DIRS} )

set(CMDLINE_LIBRARIES ${CMDLINE_LIBRARY})
set(CMDLINE_INCLUDE_DIRS ${CMDLINE_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(cmdline  DEFAULT_MSG
                                  CMDLINE_LIBRARY CMDLINE_INCLUDE_DIR)

mark_as_advanced(CMDLINE_INCLUDE_DIR CMDLINE_LIBRARY )
