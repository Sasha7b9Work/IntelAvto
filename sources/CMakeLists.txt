cmake_minimum_required(VERSION 3.14)

project (IntelAvto)

set (TARGET_NAME IntelAvto)

set(CMAKE_UNITY_BUILD ON)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

add_subdirectory(Device)
add_subdirectory(Panel)
add_subdirectory(PLoader)
