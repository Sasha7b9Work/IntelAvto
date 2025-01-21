# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Device)

set(CMAKE_UNITY_BUILD OFF)

include_directories(
    ../common/_VS_KEIL
    ../common/_VS_KEIL/CMSIS
    ../common/_VS_KEIL/std_includes
    ../common
    ../Hardware/HAL
    src
    ..
    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Include
    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
    ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
    ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src
    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
)

add_definitions(-DSTM32F407xx)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(
    /wd4201
    /wd4514
    /wd4623
    /wd4626
    /wd4820
    /wd4996
    /wd5027
    /wd5039
    /wd5045
    /wd5204
    /wd5220    # a non-static data member with a volatile qualified type no longer implies
    /wd5246
)

file(GLOB LIBS_HAL          ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/*.*
                            ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/*.*)

file(GLOB SRC                         src/*.h src/*.cpp)
file(GLOB CONNECTOR_DEVICE  ../common/Connector/Device/Value_.*
                            ../common/Connector/Device/Messages_.*
                            ../common/Connector/Device/Transceiver_.h
                                      src/Connector/Device/*.*)
file(GLOB GENERATOR                   src/Generator/*.*)
file(GLOB HARDWARE_CMSIS              src/Hardware/CMSIS/*.*)
file(GLOB HARDWARE_HAL                src/Hardware/HAL/*)
file(GLOB UTILS             ../common/Utils/Queue.*
                            ../common/Utils/StringUtils.*)

set_source_files_properties(${LIBS_FATFS} ${LIBS_HAL}
    src/Hardware/CMSIS/system_stm32f4xx.c
    PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(libs/HAL           FILES ${LIBS_HAL})
source_group(src                FILES ${SRC})
source_group(Connector/Device   FILES ${CONNECTOR_DEVICE})
source_group(Generator          FILES ${GENERATOR})
source_group(Hardware/CMSIS     FILES ${HARDWARE_CMSIS})
source_group(Hardware/HAL       FILES ${HARDWARE_HAL})
source_group(Utils              FILES ${UTILS})

add_library(${PROJECT_NAME}
    ${LIBS_HAL}
    ${SRC}
    ${CONNECTOR_DEVICE}
    ${GENERATOR}
    ${HARDWARE_CMSIS}
    ${HARDWARE_HAL}
    ${UTILS}
)
