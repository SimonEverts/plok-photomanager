find_path(LIBMOSQUITTO_INCLUDE_DIR mosquitto.h
    HINTS ${PC_LIBMOSQUITTO_INCLUDE_DIR} ${PC_LIBMOSQUITTO_INCLUDE_DIRS} ENV INCLUDE
    )

find_path(LIBMOSQUITTO_LIBRARY_DIR libmosquitto.so libmosquitto.so.1 libmosquitto.lib
    HINTS ${PC_LIBMOSQUITTO_LIBRARY_DIR} ${PC_LIBMOSQUITTO_LIBRARY_DIRS} "/usr/lib/" "/usr/lib/x86_64-linux-gnu" ENV LIBPATH
    )

find_library(MOSQUITTO_LIBRARY NAMES mosquitto HINTS ${LIBMOSQUITTO_LIBRARY_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Mosquitto DEFAULT_MSG
        LIBMOSQUITTO_LIBRARY_DIR LIBMOSQUITTO_INCLUDE_DIR)

mark_as_advanced(LIBMOSQUITTO_INCLUDE_DIR LIBMOSQUITTO_LIBRARY_DIR )

SET(MOSQUITTO_LIBRARIES ${MOSQUITTO_LIBRARY})