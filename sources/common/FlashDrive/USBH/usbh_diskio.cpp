/**
  ******************************************************************************
  * @file    usbh_diskio_dma.c
  * @author  MCD Application Team
  * @brief   USB Host Disk I/O driver (with internal DMA).
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

  /* Includes ------------------------------------------------------------------*/
#include "defines.h"
#include "FlashDrive/FlashDrive.h"
#include "ff_gen_drv.h"
#include "usbh_diskio.h"
#include <cstring>

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
    #pragma clang diagnostic ignored "-Wcast-qual"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define USB_DEFAULT_BLOCK_SIZE 512

/* Private variables ---------------------------------------------------------*/
static DWORD scratch[_MAX_SS / 4];
//extern USBH_HandleTypeDef  hUSBHost;

/* Private function prototypes -----------------------------------------------*/
DSTATUS USBH_initialize(BYTE);
DSTATUS USBH_status(BYTE);
DRESULT USBH_read(BYTE, BYTE *, DWORD, UINT);

#if _USE_WRITE == 1
DRESULT USBH_write(BYTE, const BYTE *, DWORD, UINT);
#endif /* _USE_WRITE == 1 */

#if _USE_IOCTL == 1
DRESULT USBH_ioctl(BYTE, BYTE, void *);
#endif /* _USE_IOCTL == 1 */

const Diskio_drvTypeDef  USBH_Driver =
{
  USBH_initialize,
  USBH_status,
  USBH_read,
#if  _USE_WRITE == 1
  USBH_write,
#endif /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
  USBH_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  lun : lun id
  * @retval DSTATUS: Operation status
  */
DSTATUS USBH_initialize(BYTE lun)
{
    (void)lun;

    /* CAUTION : USB Host library has to be initialized in the application */

    return RES_OK;
}

/**
  * @brief  Gets Disk Status
  * @param  lun : lun id
  * @retval DSTATUS: Operation status
  */
DSTATUS USBH_status(BYTE lun)
{
    DRESULT res = RES_ERROR;

    if (USBH_MSC_UnitIsReady(&FDrive::hUSB_Host, (uint8_t)lun))
    {
        res = RES_OK;
    }
    else
    {
        res = RES_ERROR;
    }

    return res;
}
/**
  * @brief  Reads Sector(s)
  * @param  lun : lun id
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USBH_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_ERROR;
    MSC_LUNTypeDef info;
    USBH_StatusTypeDef  status = USBH_OK;

    if (((DWORD)buff & 3) && (((HCD_HandleTypeDef *)(FDrive::hUSB_Host).pData)->Init.dma_enable))
    {
        while ((count--) && (status == USBH_OK))
        {
            status = USBH_MSC_Read(&FDrive::hUSB_Host, (uint8_t)lun, sector + count, (uint8_t *)scratch, 1);

            if (status == USBH_OK)
            {
                std::memcpy(&buff[count * _MAX_SS], scratch, _MAX_SS);
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        status = USBH_MSC_Read(&FDrive::hUSB_Host, (uint8_t)lun, sector, (uint8_t *)buff, count);
    }

    if (status == USBH_OK)
    {
        res = RES_OK;
    }
    else
    {
        USBH_MSC_GetLUNInfo(&FDrive::hUSB_Host, (uint8_t)lun, &info);

        switch (info.sense.asc)
        {
        case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
        case SCSI_ASC_MEDIUM_NOT_PRESENT:
        case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
            USBH_ErrLog("USB Disk is not ready!");
            res = RES_NOTRDY;
            break;

        default:
            res = RES_ERROR;
            break;
        }
    }

    return res;
}

/**
  * @brief  Writes Sector(s)
  * @param  lun : lun id
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT USBH_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_ERROR;
    MSC_LUNTypeDef info;
    USBH_StatusTypeDef  status = USBH_OK;

    if (((DWORD)buff & 3) && (((HCD_HandleTypeDef *)(FDrive::hUSB_Host).pData)->Init.dma_enable))
    {

        while (count--)
        {
            memcpy(scratch, &buff[count * _MAX_SS], _MAX_SS);

            status = USBH_MSC_Write(&FDrive::hUSB_Host, lun, sector + count, (BYTE *)scratch, 1);
            if (status == USBH_FAIL)
            {
                break;
            }
        }
    }
    else
    {
        status = USBH_MSC_Write(&FDrive::hUSB_Host, lun, sector, (BYTE *)buff, count);
    }

    if (status == USBH_OK)
    {
        res = RES_OK;
    }
    else
    {
        USBH_MSC_GetLUNInfo(&FDrive::hUSB_Host, lun, &info);

        switch (info.sense.asc)
        {
        case SCSI_ASC_WRITE_PROTECTED:
            USBH_ErrLog("USB Disk is Write protected!");
            res = RES_WRPRT;
            break;

        case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
        case SCSI_ASC_MEDIUM_NOT_PRESENT:
        case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
            USBH_ErrLog("USB Disk is not ready!");
            res = RES_NOTRDY;
            break;

        default:
            res = RES_ERROR;
            break;
        }
    }

    return res;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  lun : lun id
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT USBH_ioctl(BYTE lun, BYTE cmd, void *buff)
{
    DRESULT res = RES_ERROR;
    MSC_LUNTypeDef info;

    switch (cmd)
    {
        /* Make sure that no pending write process */
    case CTRL_SYNC:
        res = RES_OK;
        break;

        /* Get number of sectors on the disk (DWORD) */
    case GET_SECTOR_COUNT:
        if (USBH_MSC_GetLUNInfo(&FDrive::hUSB_Host, lun, &info) == USBH_OK)
        {
            *(DWORD *)buff = info.capacity.block_nbr;
            res = RES_OK;
        }
        else
        {
            res = RES_ERROR;
        }
        break;

        /* Get R/W sector size (WORD) */
    case GET_SECTOR_SIZE:
        if (USBH_MSC_GetLUNInfo(&FDrive::hUSB_Host, lun, &info) == USBH_OK)
        {
            *(DWORD *)buff = info.capacity.block_size;
            res = RES_OK;
        }
        else
        {
            res = RES_ERROR;
        }
        break;

        /* Get erase block size in unit of sector (DWORD) */
    case GET_BLOCK_SIZE:

        if (USBH_MSC_GetLUNInfo(&FDrive::hUSB_Host, lun, &info) == USBH_OK)
        {
            *(DWORD *)buff = info.capacity.block_size / USB_DEFAULT_BLOCK_SIZE;
            res = RES_OK;
        }
        else
        {
            res = RES_ERROR;
        }
        break;

    default:
        res = RES_PARERR;
    }

    return res;
}
#endif /* _USE_IOCTL == 1 */
