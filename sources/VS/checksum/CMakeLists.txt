project (checksum)

message("------------------------------------------------------------------------ ${PROJECT_NAME}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

file(GLOB SRC  *.cpp)

source_group(src   FILES ${SRC})

add_executable(
    ${PROJECT_NAME}
    ${SRC}
)
