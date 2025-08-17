#pragma once
#include <stdlib.h>
#include <string.h>


#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wpadded"
    #pragma clang diagnostic ignored "-Wextra-semi-stmt"
    #pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
    #pragma clang diagnostic ignored "-Wbad-function-cast"
    #pragma clang diagnostic ignored "-Wswitch-enum"
    #pragma clang diagnostic ignored "-Wcovered-switch-default"
    #pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif


#define USBH_MAX_NUM_ENDPOINTS                2
#define USBH_MAX_NUM_INTERFACES               2
#define USBH_MAX_NUM_CONFIGURATION            1
#define USBH_MAX_NUM_SUPPORTED_CLASS          1
#define USBH_KEEP_CFG_DESCRIPTOR              0
#define USBH_MAX_SIZE_CONFIGURATION           0x200
#define USBH_MAX_DATA_BUFFER                  0x200
#define USBH_DEBUG_LEVEL                      0
#define USBH_USE_OS                           0

#include "stm32f4xx.h"
    
#if (USBH_USE_OS == 1)
    #include "cmsis_os.h"
    #define   USBH_PROCESS_PRIO    osPriorityNormal
#endif    

#define USBH_malloc               malloc
#define USBH_free                 free
#define USBH_memset               memset
#define USBH_memcpy               memcpy


#if (USBH_DEBUG_LEVEL > 0)
#define USBH_UsrLog(...)  Log_Write(__VA_ARGS__)
#define USBH_ErrLog(...)  Log_Write(__VA_ARGS__)
#define USBH_DbgLog(...)  Log_Write(__VA_ARGS__)
#else
#define USBH_UsrLog(...) {}
#define USBH_ErrLog(...) {}
#define USBH_DbgLog(...) {}
#endif


#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic pop
#endif
