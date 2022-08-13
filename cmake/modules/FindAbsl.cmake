find_path(LIBABSL_INCLUDE_DIR absl/types/span.h
        HINTS
                ${PC_LIBABSL_INCLUDE_DIR}
                ${PC_LIBABSL_INCLUDE_DIRS}
                ENV INCLUDE
                ENV PC_LIBABSL_INCLUDE_DIR
                ENV LIBABSL_INCLUDE_DIRS
                ${CMAKE_SOURCE_DIR}/external/abseil-cpp/
        )

#find_path(LIBABSL_LIBRARY_DIR libabsl.so
#        HINTS ${PC_LIBABSL_LIBRARY_DIR} ${PC_LIBABSL_LIBRARY_DIRS} ENV LIBPATH ENV PC_LIBABSL_LIBRARY_DIR ENV PC_LIBABSL_LIBRARY_DIRS
#        PATH_SUFFIXES ipp )

#find_library(ABSL_LIBRARY NAMES absl libabsl HINTS ${LIBABSL_LIBRARY_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE

#find_package_handle_standard_args(Absl  DEFAULT_MSG
#        ABSL_LIBRARY LIBABSL_INCLUDE_DIR)
find_package_handle_standard_args(Absl  DEFAULT_MSG LIBABSL_INCLUDE_DIR)

mark_as_advanced(LIBABSL_INCLUDE_DIR LIBABSL_LIBRARY_DIR )

