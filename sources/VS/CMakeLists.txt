cmake_minimum_required(VERSION 3.15)

project (IntelAvto-GUI)

set (IntelAvto-GUI)

set(CMAKE_UNITY_BUILD ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")         # Установка флага линковки /MT

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

add_subdirectory(Panel)
#add_subdirectory(Saver)
add_subdirectory(checksum)
add_subdirectory(Upgrader)
