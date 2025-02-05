#pragma once


typedef enum {
    RES_OK = 0,		/* 0: Successful */
    RES_ERROR,		/* 1: R/W Error */
    RES_WRPRT,		/* 2: Write Protected */
    RES_NOTRDY,		/* 3: Not Ready */
    RES_PARERR		/* 4: Invalid Parameter */
} DRESULT;


#define BYTE    char
#define DSTATUS char
#define DRESULT char
#define DWORD   unsigned int
#define UINT    unsigned int


struct Diskio_drvTypeDef
{
    DSTATUS(*disk_initialize) (BYTE);                      /*!< Initialize Disk Drive  */
    DSTATUS(*disk_status)     (BYTE);                      /*!< Get Disk Status        */
    DRESULT(*disk_read)       (BYTE, BYTE*, DWORD, UINT);  /*!< Read Sector(s)         */
};

#include "diskio.h"
#include "ff.h"

uint8_t FATFS_LinkDriver(const Diskio_drvTypeDef *drv, char *path);
uint8_t FATFS_UnLinkDriver(char *path);
