cmake_minimum_required(VERSION 3.14)

project (IntelAvto)

set (TARGET_NAME IntelAvto)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

add_subdirectory(Panel)
add_subdirectory(PLoader)
