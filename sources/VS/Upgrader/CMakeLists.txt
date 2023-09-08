project (Upgrader)

message("------------------------------------------------------------------------ ${PROJECT_NAME}")

include_directories(../../Panel/src/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(
    /wd4505
    /wd4996     # 'xxx': This function or variable may be unsafe. Consider using yyy instead. To disable
                # deprecation, use _CRT_SECURE_NO_WARNINGS
    /wd5039     # 'xxx': pointer or reference to potentially throwing function passed to extern C function under -EHc.
    /wd5045     # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    )


file(GLOB SRC  *.cpp *.c *.h)

source_group(src FILES ${SRC})

add_executable(${PROJECT_NAME} ${SRC})
