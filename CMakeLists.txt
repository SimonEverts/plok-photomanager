cmake_minimum_required(VERSION 3.10)
project(plok-photomanager)

include(cmake/cmake-common.txt)

option(USE_SYSTEM_GSL "Use system GSL library" ON)
option(USE_SYSTEM_LIBWEBSOCKETS "use system libwebsockets" OFF) # using included libwebsockets as system version on ubuntu 1804 is too old
option(USE_SYSTEM_LIBUV "use system libuv" OFF)

option(BUILD_KATLA_UV "Build uv module" ON)
option(BUILD_KATLA_GTK4 "Build gtk-4 module" ON)

option(BUILD_GMOCK "Builds the googlemock subproject" OFF)
option(INSTALL_GTEST "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF.)" OFF)
add_subdirectory(external/googletest)
add_subdirectory(external/fmt)

IF(USE_SYSTEM_GSL)
    find_package(Microsoft.GSL REQUIRED)
    # add_library(Microsoft.GSL::GSL ALIAS GSL::gsl)
ELSE()
    add_subdirectory(external/gsl)
    include_directories(external/gsl/include)
ENDIF()

find_package(Threads REQUIRED)
add_subdirectory(external/katla/core)

add_subdirectory(external/katla/ui)
add_subdirectory(external/katla/window-management)

find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK4 REQUIRED gtk4)
add_subdirectory(external/katla/gtk4)

add_subdirectory(src/gui)
