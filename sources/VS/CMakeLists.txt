cmake_minimum_required(VERSION 3.14)

project (IntelAvto-GUI)

set (IntelAvto-GUI)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

add_subdirectory(Panel)
add_subdirectory(Saver)
add_subdirectory(checksum)
add_subdirectory(Upgrader)
