# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (Loader)

message("------------------------------------------------------------------------ ${PROJECT_NAME}")

include_directories(../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
                    ../common/_VS_KEIL/CMSIS
                    ../common/_VS_KEIL/std_includes
                             src
                             src/Hardware/FatFS
                             src/Hardware/HAL
                             src/Hardware/USBH
                    ../Panel/src
                    )

add_definitions(-DSTM32F401xE -DLOADER)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4159)    # #pragma pack(pop,...): has popped previously pushed identifier xxx
add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
add_compile_options(/wd4623)    # 'xxx': default constructor was implicitly defined as deleted
add_compile_options(/wd4820)    # 'xxx': '4' bytes padding added after data member 'yyy'
add_compile_options(/wd5039)    # 'xxx': pointer or reference to potentially throwing function passed to extern C function under -EHc.
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd5219)    # implicit conversion from 'xxx' to 'float', possible loss of data
add_compile_options(/wd5220)    # a non-static data member with a volatile qualified type no longer implies
add_compile_options(/wd26812)   # The enum type %type% is unscoped. Prefer 'enum class' over 'enum' (Enum.3)


file(GLOB SRC            ../Panel/src/defines.h
                                  src/*.*)
file(GLOB HAL            ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_cortex.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rcc.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_gpio.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_hcd.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_ll_usb.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_flash*

                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash*
                         )
file(GLOB DISPLAY                 src/Display/*.*)
file(GLOB HARDWARE                src/Hardware/*.*)
file(GLOB HARDWARE_CMSIS          src/Hardware/CMSIS/*.*)
file(GLOB HARDWARE_FATFS          src/Hardware/FatFS/*.*
                         ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/ff_gen_drv.*
                         ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/ff.*
                         ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/diskio.*
                         ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/option/unicode.*
                         ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/option/syscall.*)
file(GLOB HARDWARE_HAL   ../Panel/src/Hardware/HAL/HAL.*
                         ../Panel/src/Hardware/HAL/HAL_EEPROM.cpp
                         ../Panel/src/Hardware/HAL/HAL_TIM.cpp
                                  src/Hardware/HAL/*.*)
file(GLOB HARDWARE_USBH           src/Hardware/USBH/*.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_core.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_pipes.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_ctlreq.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_ioreq.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc_bot.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc_scsi.*
                         ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.*)

source_group(src            FILES ${SRC})
source_group(HAL            FILES ${HAL})
source_group(Display        FILES ${DISPLAY})
source_group(Hardware       FILES ${HARDWARE})
source_group(Hardware/CMSIS FILES ${HARDWARE_CMSIS})
source_group(Hardware/FatFS FILES ${HARDWARE_FATFS})
source_group(Hardware/HAL   FILES ${HARDWARE_HAL})
source_group(Hardware/USBH  FILES ${HARDWARE_USBH})

set_source_files_properties(${HAL} ${HARDWARE_CMSIS} ${HARDWARE_FATFS} ${HARDWARE_USBH}
                            PROPERTIES HEADER_FILE_ONLY TRUE)

add_library(${PROJECT_NAME}
                    ${SRC}
                    ${HAL}
                    ${DISPLAY}
                    ${HARDWARE}
                    ${HARDWARE_CMSIS}
                    ${HARDWARE_FATFS}
                    ${HARDWARE_HAL}
                    ${HARDWARE_USBH}
                    )
