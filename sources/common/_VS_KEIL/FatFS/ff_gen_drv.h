#pragma once


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
