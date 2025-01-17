# 2023/09/08 21:46:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (Panel)

set(CMAKE_UNITY_BUILD OFF)

message("------------------------------------------------------------------------ ${PROJECT_NAME}")

include_directories(
    src
    src/Hardware/HAL
    src/Hardware/USBD
    tests
    ../common
    ../common/_VS_KEIL/CMSIS
    ../common/_VS/std_includes
    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Include
    ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc
)

add_definitions(-DSTM32F429xx)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(
    /wd4211    #  nonstandard extension used: redefined extern to static
    /wd4514    # 'function' : unreferenced inline function has been removed
    /wd4623    # 'xxx': default constructor was implicitly defined as deleted
    /wd4626    # 'derived class' : assignment operator was implicitly defined as deleted because a base
               # class assignment operator is inaccessible or deleted
    /wd4820    # 'N' bytes padding added after data member
    /wd4996    # This function or variable may be unsafe
    /wd5027    # move assignment operator was implicitly defined as deleted
    /wd5045    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    /wd5204
)

file(GLOB LIBS_HAL            ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/*.h
                              ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/*.c)
file(GLOB SRC                       src/*.*)
file(GLOB CONNECTOR           ../common/Connector/Messages_.*
                              ../common/Connector/Transceiver_.h
                              ../common/Connector/Value_.h
                                    src/Connector/*.*)
file(GLOB DEVICE                    src/Device/*.*)
file(GLOB DISPLAY             ../common/Display/*.*)
file(GLOB DISPLAY_FONT        ../common/Display/Font/*.*)
file(GLOB HARDWARE                  src/Hardware/*.*)
file(GLOB HARDWARE_CMSIS            src/Hardware/CMSIS/*.*)
file(GLOB HARDWARE_HAL              src/Hardware/HAL/*.*
                              ../common/Hardware/HAL/HAL_SPI1_p.cpp
                              ../common/Hardware/HAL/HAL_PINS.*)
file(GLOB HARDWARE_USBD             src/Hardware/USBD/*.*
                              ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
                              ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
                              ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c)
file(GLOB HARDWARE_KEYBOARD   ../common/Hardware/Keyboard/*.*)
file(GLOB MENU                ../common/Menu/*.*)
file(GLOB MENU_PAGES          ../common/Menu/Pages/*.*)
file(GLOB SETTINGS            ../common/Settings/*.*)
file(GLOB UTILS               ../common/Utils/*.*)


source_group(libs/HAL          FILES ${LIBS_HAL})
source_group(src               FILES ${SRC})
source_group(Connector         FILES ${CONNECTOR})
source_group(Device            FILES ${DEVICE})
source_group(Display           FILES ${DISPLAY})
source_group(Display/Font      FILES ${DISPLAY_FONT})
source_group(Hardware          FILES ${HARDWARE})
source_group(Hardware/CMSIS    FILES ${HARDWARE_CMSIS})
source_group(Hardware/HAL      FILES ${HARDWARE_HAL})
source_group(Hardware/USBD     FILES ${HARDWARE_USBD})
source_group(Hardware/Keyboard FILES ${HARDWARE_KEYBOARD})
source_group(Menu              FILES ${MENU})
source_group(Menu/Pages/       FILES ${MENU_PAGES})
source_group(Settings          FILES ${SETTINGS})
source_group(Utils             FILES ${UTILS})

set_source_files_properties(${HARDWARE_CMSIS} ${LIBS_HAL}
    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c
    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
    PROPERTIES HEADER_FILE_ONLY TRUE
)

add_library(${PROJECT_NAME}
    ${LIBS_HAL}
    ${SRC}
    ${CONNECTOR}
    ${DEVICE}
    ${DISPLAY}
    ${DISPLAY_FONT}
    ${HARDWARE}
    ${HARDWARE_CMSIS}
    ${HARDWARE_HAL}
    ${HARDWARE_USBD}
    ${HARDWARE_KEYBOARD}
    ${MENU}
    ${MENU_PAGES}
    ${SETTINGS}
    ${UTILS}
)
