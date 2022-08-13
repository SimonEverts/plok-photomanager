cmake_minimum_required(VERSION 3.10)

set(target_name plok-photomanager-gui)
project(${target_name})

set(CMAKE_CXX_STANDARD 17)

# SET (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
# SET (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(sources
    main.cpp
)

add_executable(${target_name} ${sources})

target_include_directories(${target_name} PRIVATE
    .
    ..
    ../../external/
    ../../external/cxxopts/include
    $<TARGET_PROPERTY:katla-core,INTERFACE_INCLUDE_DIRECTORIES>)

file(GENERATE OUTPUT inc.txt CONTENT $<TARGET_PROPERTY:katla-core,INTERFACE_INCLUDE_DIRECTORIES>)

target_link_libraries(${target_name} katla-core katla-wm katla-gtk)
