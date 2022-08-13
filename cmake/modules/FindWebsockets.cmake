if(USE_SYSTEM_LIBWEBSOCKETS)
    find_path(LIBWEBSOCKETS_INCLUDE_DIR libwebsockets.h
        HINTS
            ${PC_LIBWEBSOCKETS_INCLUDE_DIR}
            ${PC_LIBWEBSOCKETS_INCLUDE_DIRS}
            ENV INCLUDE
        )

    find_path(LIBWEBSOCKETS_LIBRARY_DIR libwebsockets.so libwebsockets.lib
        HINTS
            ${PC_LIBWEBSOCKETS_LIBRARY_DIR}
            ${PC_LIBWEBSOCKETS_LIBRARY_DIRS}
            ENV LIBPATH
        )

    find_library(WEBSOCKETS_LIBRARY NAMES websockets HINTS ${LIBWEBSOCKETS_LIBRARY_DIR})

    include(FindPackageHandleStandardArgs)
    # handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
    # if all listed variables are TRUE
    find_package_handle_standard_args(Websockets DEFAULT_MSG
        WEBSOCKETS_LIBRARY LIBWEBSOCKETS_INCLUDE_DIR)

    mark_as_advanced(LIBWEBSOCKETS_INCLUDE_DIR LIBWEBSOCKETS_LIBRARY_DIR )

    SET(LIBWEBSOCKETS_LIBRARIES ${WEBSOCKETS_LIBRARY})
else(USE_SYSTEM_LIBWEBSOCKETS)
    message("using included libwebsockets.")

    SET(LIBWEBSOCKETS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/libwebsockets/include/ ${CMAKE_BINARY_DIR}/external/libwebsockets/)

    SET(WEBSOCKETS_LIBRARY ${CMAKE_BINARY_DIR}/external/libwebsockets/lib/libwebsockets.so)
    SET(LIBWEBSOCKETS_LIBRARIES ${WEBSOCKETS_LIBRARY})

    find_package_handle_standard_args(Websockets DEFAULT_MSG
        WEBSOCKETS_LIBRARY LIBWEBSOCKETS_INCLUDE_DIR)
endif(USE_SYSTEM_LIBWEBSOCKETS)