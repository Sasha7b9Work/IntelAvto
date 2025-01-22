# 2023/09/08 21:15:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Saver)

set(CMAKE_UNITY_BUILD OFF)

set(CMAKE_UNITY_BUILD_BATCH_SIZE 4)

message("------------------------------------------------------------------------ ${PROJECT_NAME}")

include_directories(
    src/
    ../../common
    ../../common/_VS_KEIL/CMSIS/
    ../../common/_VS_KEIL/HAL/
    ../../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc/
    ../../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/
    ../../Panel/src/
    ../../Panel/src/Hardware/HAL/
    ../../Panel/src/Hardware/
    ../ThirdParty/wxWidgets/include
    ../../generated/ThirdParty/lib/vc_dll/mswud
)

link_directories(../../generated/ThirdParty/lib/vc_dll)

add_definitions(-DSTM32F429xx -DWXUSINGDLL -DGUI)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

# Older cmake versions don't support string(APPEND ...) provide a workaround
macro(wx_string_append var str)
    set(${var} ${${var}}${str})
endmacro()

if(BUILD_USE_STATIC_RUNTIME)
    set(MSVC_LIB_USE "/MTd")
    set(MSVC_LIB_REPLACE "/MDd")
    
    # Set MSVC runtime flags for all configurations
    foreach(cfg "" ${CMAKE_CONFIGURATION_TYPES})
        set(c_flag_var CMAKE_C_FLAGS)
        set(cxx_flag_var CMAKE_CXX_FLAGS)
        if(cfg)
            string(TOUPPER ${cfg} cfg_upper)
            wx_string_append(c_flag_var "_${cfg_upper}")
            wx_string_append(cxx_flag_var "_${cfg_upper}")
        endif()
        if(${c_flag_var} MATCHES ${MSVC_LIB_REPLACE})
            string(REPLACE ${MSVC_LIB_REPLACE} ${MSVC_LIB_USE} ${c_flag_var} "${${c_flag_var}}")
            set(${c_flag_var} ${${c_flag_var}} CACHE STRING
            "Flags used by the C compiler during ${cfg_upper} builds." FORCE)
        endif()
        if(${cxx_flag_var} MATCHES ${MSVC_LIB_REPLACE})
            string(REPLACE ${MSVC_LIB_REPLACE} ${MSVC_LIB_USE} ${cxx_flag_var} "${${cxx_flag_var}}")
            set(${cxx_flag_var} ${${cxx_flag_var}} CACHE STRING
            "Flags used by the CXX compiler during ${cfg_upper} builds." FORCE)
        endif()
    endforeach()
endif()

add_compile_options(
    /wd4211    # nonstandard extension used: redefined extern to static
    /wd4355    # 'this': used in base member initializer list
    /wd4514    # 'xxx': unreferenced inline function has been removed
    /wd4571    # Informational: catch(...) semantics changed since Visual C++ 7.1
    /wd4623    # 'xxx': default constructor was implicitly defined as deleted
    /wd4625    # 'xxx': copy constructor was implicitly defined as deleted
    /wd4626    # 'xxx': assignment operator was implicitly defined as deleted
    /wd4710    # function not inlined
    /wd4711    # function 'function' selected for inline expansion
    /wd4774    # 'xxx' : format string expected in argument Y is not a string literal
    /wd4820    # 'xxx': 'y' bytes padding added after data member 'zzz'
    /wd4996    # 'xxx': This function or variable may be unsafe. Consider using yyy instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS
    /wd5026    # 'xxx': move constructor was implicitly defined as deleted
    /wd5027    # 'xxx': move assignment operator was implicitly defined as deleted
    /wd5039    # 'xxx': pointer or reference to potentially throwing function passed to extern C function under -EHc.
    /wd5045    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    /wd5204    # 'xxx': class has virtual functions, but its trivial destructor is not virtual; instances of objects derived from this class may not be destructed correctly
    /wd5219
    /wd5267
    /wd26821
    /wd26812
)

file(GLOB SRC                 ../../Panel/src/defines.h
                                          src/Application_win.*
                                          src/main_win.cpp
                                          src/functions_win.cpp)
file(GLOB DISPLAY             ../../common/Display/Colors_.*
                              ../../common/Display/Text_.*
                                          src/Display/*.*)
file(GLOB DISPLAY_FONT        ../../common/Display/Font/Font.cpp)
file(GLOB KEYBOARD            ../../Panel/src/Keyboard/Keyboard.h
                                          src/Keyboard/Keyboard_win.cpp)
file(GLOB GUI                             src/GUI/*)
file(GLOB SETTINGS            ../../Panel/src/Settings/*.*)
file(GLOB UTILS               ../../common/Utils/Buffer.*
                              ../../common/Utils/Stack.*
                              ../../common/Utils/String.*
                              ../../common/Utils/StringUtils.*)


set_source_files_properties(
    ../../Panel/src/Utils/String.cpp
    ../../Panel/src/Utils/StringUtils.cpp
    PROPERTIES SKIP_UNITY_BUILD_INCLUSION ON
)

source_group(src               FILES ${SRC})
source_group(Display           FILES ${DISPLAY})
source_group(Display\\Font     FILES ${DISPLAY_FONT})
source_group(Keyboard          FILES ${KEYBOARD})
source_group(GUI               FILES ${GUI})
source_group(Settings          FILES ${SETTINGS})
source_group(Utils             FILES ${UTILS})


add_executable(
    ${PROJECT_NAME}
    ${SRC}
    ${DISPLAY}
    ${DISPLAY_FONT}
    ${GUI}
    ${KEYBOARD}
    ${SETTINGS}
    ${UTILS}
)

target_link_libraries(Saver wxbase32ud)
target_link_libraries(Saver wxmsw32ud_core)

target_link_libraries(Saver WinMM.lib)
target_link_libraries(Saver version.lib)
target_link_libraries(Saver Imm32.Lib)
target_link_libraries(Saver Setupapi.lib)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxbase32ud_vc_custom.dll $(OutputPath) /Y)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxmsw32ud_core_vc_custom.dll $(OutputPath) /Y)
