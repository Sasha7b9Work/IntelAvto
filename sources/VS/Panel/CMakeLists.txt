# 2023/09/08 20:52:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (PanelGUI)

set(CMAKE_UNITY_BUILD OFF)

message("------------------------------------------------------------------------ ${PROJECT_NAME}")

include_directories(src
                    ../../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    ../../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
                    ../../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
                    ../../common/_VS_KEIL/CMSIS
                    ../../common/_VS_KEIL/HAL
                    ../../Panel/src
                    ../../Panel/tests
                    ../../Panel/src/Hardware/HAL
                    ../../Panel/src/Hardware
                    ../ThirdParty/wxWidgets/include
                    ../../generated/ThirdParty/lib/vc_dll/mswud)

link_directories(../../generated/ThirdParty/lib/vc_dll)

add_definitions(-DSTM32F429xx -DWXUSINGDLL -DGUI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(
    /wd4211     # nonstandard extension used: redefined extern to static
    /wd4355     # 'this': used in base member initializer list
    /wd4514     # 'xxx': unreferenced inline function has been removed
    /wd4571     # Informational: catch(...) semantics changed since Visual C++ 7.1
    /wd4623     # 'xxx': default constructor was implicitly defined as deleted
    /wd4625     # 'xxx': copy constructor was implicitly defined as deleted
    /wd4626     # 'xxx': assignment operator was implicitly defined as deleted
    /wd4710     # function not inlined
    /wd4774     # 'xxx' : format string expected in argument Y is not a string literal
    /wd4820     # 'xxx': 'y' bytes padding added after data member 'zzz'
    /wd4996     # 'xxx': This function or variable may be unsafe. Consider using yyy instead. To disable
                # deprecation, use _CRT_SECURE_NO_WARNINGS
    /wd5026     # 'xxx': move constructor was implicitly defined as deleted
    /wd5027     # 'xxx': move assignment operator was implicitly defined as deleted
    /wd5039     # 'xxx': pointer or reference to potentially throwing function passed to extern C
                # function under -EHc.
    /wd5045     # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    /wd5204     # 'xxx': class has virtual functions, but its trivial destructor is not virtual;
                # instances of objects derived from this class may not be destructed correctly
    /wd5219 
    /wd26821
    /wd26812
)

file(GLOB VS_HAL              ../../common/_VS_KEIL/HAL/stm32f4xx_hal_gpio.*
                              ../../common/_VS_KEIL/HAL/stm32f4xx_hal_rcc.*
                              ../../common/_VS_KEIL/HAL/stm32f4xx_hal_cortex.*
                              ../../common/_VS_KEIL/HAL/stm32f4xx_hal.*)
file(GLOB SRC                 ../../Panel/src/defines.h
                              ../../Panel/src/FreqMeter.*
                              ../../Panel/src/Log.*
                              ../../Panel/src/Settings.*
                                          src/Application_win.*
                                          src/main_win.cpp)
file(GLOB CALCULATE           ../../Panel/src/Calculate/*)
file(GLOB DISPLAY                         src/Display/Display_win.cpp
                                          src/Display/Colors_win.cpp
                              ../../Panel/src/Display/Colors.*
                              ../../Panel/src/Display/Console.*
                              ../../Panel/src/Display/Display.*
                              ../../Panel/src/Display/Indicator.*
                              ../../Panel/src/Display/RedrawingZone.*
                              ../../Panel/src/Display/Primitives.*
                                          src/Display/Primitives_win.cpp
                              ../../Panel/src/Display/Symbols.*
                              ../../Panel/src/Display/Text.*)
file(GLOB DISPLAY_FONT        ../../Panel/src/Display/Font/*.h
                              ../../Panel/src/Display/Font/*.cpp
                              ../../Panel/src/Display/Font/*.inc
                              ../../Panel/src/Display/Font/*.xml)
file(GLOB FREQMETER           ../../Panel/src/FreqMeter/*)
file(GLOB GUI                             src/GUI/*)
file(GLOB HARDWARE            ../../Panel/src/Hardware/*FPGA.*
                              ../../Panel/src/Hardware/FreqMeter.*
                              ../../Panel/src/Hardware/VCP.h
                                          src/Hardware/VCP_win.cpp)
file(GLOB HARDWARE_HAL        ../../Panel/src/Hardware/HAL/HAL.*
                              ../../Panel/src/Hardware/HAL/HAL_FSMC.cpp
                              ../../Panel/src/Hardware/HAL/HAL_TIM.cpp
                                          src/Hardware/HAL/HAL_EEPROM_win.cpp)
file(GLOB KEYBOARD            ../../Panel/src/Keyboard/Keyboard.h
                                          src/Keyboard/Keyboard_win.cpp)
file(GLOB MENU                ../../Panel/src/Menu/*.*)
file(GLOB MENU_PAGES          ../../Panel/src/Menu/Pages/*.*)
file(GLOB MENU_PAGES_CHANNELS ../../Panel/src/Menu/Pages/Channels/*.*)
file(GLOB MENU_PAGES_OTHER    ../../Panel/src/Menu/Pages/Other/*.*)
file(GLOB UTILS               ../../Panel/src/Utils/*.*)

source_group(_VS/HAL               FILES ${VS_HAL})
source_group(src                   FILES ${SRC})
source_group(Calculate             FILES ${CALCULATE})
source_group(Display               FILES ${DISPLAY})
source_group(Display/Font          FILES ${DISPLAY_FONT})
source_group(FreqMeter             FILES ${FREQMETER})
source_group(GUI                   FILES ${GUI})
source_group(Hardware              FILES ${HARDWARE})
source_group(Hardware/HAL          FILES ${HARDWARE_HAL})
source_group(Keyboard              FILES ${KEYBOARD})
source_group(Menu                  FILES ${MENU})
source_group(Menu/Pages            FILES ${MENU_PAGES})
source_group(Menu/Pages/Channels   FILES ${MENU_PAGES_CHANNELS})
source_group(Menu/Pages/Other      FILES ${MENU_PAGES_OTHER})
source_group(Utils                 FILES ${UTILS})

add_executable(PanelGUI
                        ${VS_HAL}
                        ${SRC} ${CALCULATE} ${DISPLAY} ${DISPLAY_FONT} ${HARDWARE} ${FREQMETER} ${GUI}
                        ${HARDWARE_HAL} ${KEYBOARD} ${MENU} ${MENU_PAGES} ${MENU_PAGES_CHANNELS}
                        ${MENU_PAGES_OTHER} ${UTILS})

target_link_libraries(PanelGUI wxbase32ud)
target_link_libraries(PanelGUI wxmsw32ud_core)

target_link_libraries(PanelGUI WinMM.lib)
target_link_libraries(PanelGUI version.lib)
target_link_libraries(PanelGUI Imm32.Lib)
target_link_libraries(PanelGUI Setupapi.lib)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxbase32ud_vc_custom.dll $(OutputPath) /Y)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxmsw32ud_core_vc_custom.dll $(OutputPath) /Y)
