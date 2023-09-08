cmake_minimum_required(VERSION 3.14)

project (CH3-96-2)

set (TARGET_NAME CH3-96-2)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

add_subdirectory(Panel)
add_subdirectory(Loader)
