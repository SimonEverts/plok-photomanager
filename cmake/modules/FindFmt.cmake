find_path(LIBFMT_INCLUDE_DIR fmt/format.h
        HINTS
                ${PC_LIBFMT_INCLUDE_DIR}
                ${PC_LIBFMT_INCLUDE_DIRS}
                ENV INCLUDE
                ENV PC_LIBFMT_INCLUDE_DIR
                ENV PC_LIBFMT_INCLUDE_DIRS
                ${CMAKE_SOURCE_DIR}/external/fmt/include/
        )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Fmt DEFAULT_MSG LIBFMT_INCLUDE_DIR)

mark_as_advanced(LIBFMT_INCLUDE_DIR )

