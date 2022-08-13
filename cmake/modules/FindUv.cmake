find_path(LIBUV_INCLUDE_DIR uv.h
    HINTS ${PC_LIBUV_INCLUDE_DIR} ${PC_LIBUV_INCLUDE_DIRS} ENV INCLUDE
    )

find_path(LIBUV_LIBRARY_DIR libuv.so uv.lib
    HINTS ${PC_LIBUV_LIBRARY_DIR} ${PC_LIBUV_LIBRARY_DIRS} ENV LIBPATH
    )

find_library(UV_LIBRARY NAMES uv HINTS ${LIBUV_LIBRARY_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Uv DEFAULT_MSG
    UV_LIBRARY LIBUV_INCLUDE_DIR)

mark_as_advanced(LIBUV_INCLUDE_DIR LIBUV_LIBRARY_DIR )

SET(LIBUV_LIBRARIES ${UV_LIBRARY})