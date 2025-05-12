// 2025/02/03 14:33:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FlashDrive.h"
#include "Utils/Log_.h"
#include "Display/Display_.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "FileManager.h"
#include "Utils/Dictionary.h"
#include "usbh_diskio.h"
#include "Display/Text_.h"
#include <ff_gen_drv.h>
#include <cstring>



namespace FDrive
{
    static bool need_mount = false;

    USBH_HandleTypeDef hUSB_Host;

    static HCD_HandleTypeDef hHCD;
    void *handleHCD = &hHCD;

    static FATFS USBDISKFatFs;
    static char USBDISKPath[4];
    static bool gFlashDriveIsConnected = false;

    // Устанавливает текущее время для файла nameFile
    static void SetTimeForFile(char *nameFile);

    static void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id);

    static void SetTimeForFile(char *name);

    static void InitHardware();
}


void FDrive::USBH_UserProcess(USBH_HandleTypeDef *, uint8 id)
{
    switch (id)
    {
    case HOST_USER_SELECT_CONFIGURATION:
        break;

    case HOST_USER_CLASS_ACTIVE:
        need_mount = true;
        /*
        if (f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 1) != FR_OK)
        {
            display.ShowWarning(WrongFileSystem);
        }
        else
        {
            gFlashDriveIsConnected = true;
            FM_Init();
            Menu::ChangeStateFlashDrive();
        }
        */
        break;

    case HOST_USER_CLASS_SELECTED:
        break;

    case HOST_USER_CONNECTION:
        f_mount(nullptr, (TCHAR const *)"", 0);
        break;

    case HOST_USER_DISCONNECTION:
        gFlashDriveIsConnected = false;
//        Menu::ChangeStateFlashDrive();
        break;

    default:
        break;
    }
}



void FDrive::Mount()
{
    FileManager::Init();
//    Menu::ChangeStateFlashDrive();
    if (f_mount(&USBDISKFatFs, (TCHAR const *)USBDISKPath, 0) != FR_OK)
    {
        LOG_ERROR("Не могу примонтировать диск");
    }
}


bool FDrive::IsConnected()
{
    return gFlashDriveIsConnected;
}



void FDrive::Init()
{
#ifndef WIN32

    InitHardware();

    if (FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == FR_OK)
    {
        USBH_Init(&hUSB_Host, USBH_UserProcess, 0);
        USBH_RegisterClass(&hUSB_Host, USBH_MSC_CLASS);
        USBH_Start(&hUSB_Host);
    }
    else
    {
        LOG_ERROR("Can not %s", __FUNCTION__);
    }

#endif
}



void FDrive::Update()
{
    if (need_mount)      // Если обнаружено физическое подключение внешнего диска
    {
        uint timeStart = TIME_MS;
        need_mount = false;

//        Display::FuncOnWaitStart(DICT(DDetectFlashDrive), false);

        if (f_mount(&USBDISKFatFs, (TCHAR const *)USBDISKPath, 1) != FR_OK)
        {
//            Display::ShowWarning(WrongFileSystem);
        }
        else
        {
            gFlashDriveIsConnected = true;
            FileManager::Init();
//            Menu::ChangeStateFlashDrive();
        }
        while (TIME_MS - timeStart < 3000)
        {
        }

//        Display::FuncOnWaitStop();
    }
    else
    {
        USBH_Process(&hUSB_Host);
    }
}



bool FDrive::AppendStringToFile(const char *)
{
    return false;
}



void FDrive::GetNumDirsAndFiles(const char *fullPath, int *num_dirs, int *num_files)
{
    FILINFO fno;
    DIR dir;

    *num_dirs = 0;
    *num_files = 0;


    char nameDir[_MAX_LFN + 1];
    std::memcpy(nameDir, (void *)fullPath, std::strlen(fullPath));
    nameDir[std::strlen(fullPath)] = '\0';

    if (f_opendir(&dir, nameDir) == FR_OK)
    {
        int numReadingElements = 0;
        (void)numReadingElements;
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(&dir, &fno) != FR_OK)
            {
                break;
            }
            if (fno.fname[0] == 0)
            {
                if (alreadyNull)
                {
                    break;
                }
                alreadyNull = true;
                continue;
            }
            numReadingElements++;
            if (fno.fname[0] != '.')
            {
                if (fno.fattrib & AM_DIR)
                {
                    (*num_dirs)++;
                }
                else
                {
                    (*num_files)++;
                }
            }
        }
        f_closedir(&dir);
    }
}



bool FDrive::GetNameDir(const char *fullPath, int numDir, char *nameDirOut, StructForReadDir *s)
{
    std::memcpy(s->nameDir, (void *)fullPath, std::strlen(fullPath));
    s->nameDir[std::strlen(fullPath)] = '\0';

    DIR *pDir = &s->dir;
    if (f_opendir(pDir, s->nameDir) == FR_OK)
    {
        int num_dirs = 0;
        FILINFO *pFNO = &s->fno;
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(pDir, pFNO) != FR_OK)
            {
                *nameDirOut = '\0';
                f_closedir(pDir);
                return false;
            }
            if (pFNO->fname[0] == 0)
            {
                if (alreadyNull)
                {
                    *nameDirOut = '\0';
                    f_closedir(pDir);
                    return false;
                }
                alreadyNull = true;
            }
            if (numDir == num_dirs && (pFNO->fattrib & AM_DIR))
            {
                std::strcpy(nameDirOut, pFNO->fname);
                return true;
            }
            if ((pFNO->fattrib & AM_DIR) && (pFNO->fname[0] != '.'))
            {
                num_dirs++;
            }
        }
    }
    return false;
}



bool FDrive::GetNextNameDir(char *nameDirOut, StructForReadDir *s)
{
    DIR *pDir = &s->dir;
    FILINFO *pFNO = &s->fno;
    bool alreadyNull = false;
    while (true)
    {
        if (f_readdir(pDir, pFNO) != FR_OK)
        {
            *nameDirOut = '\0';
            f_closedir(pDir);
            return false;
        }
        else if (pFNO->fname[0] == 0)
        {
            if (alreadyNull)
            {
                *nameDirOut = '\0';
                f_closedir(pDir);
                return false;
            }
            alreadyNull = true;
        }
        else
        {
            if (pFNO->fattrib & AM_DIR)
            {
                std::strcpy(nameDirOut, pFNO->fname);
                return true;
            }
        }
    }
}



void FDrive::CloseCurrentDir(StructForReadDir *s)
{
    f_closedir(&s->dir);
}



bool FDrive::GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *s)
{
    std::memcpy(s->nameDir, (void *)fullPath, std::strlen(fullPath));
    s->nameDir[std::strlen(fullPath)] = '\0';

    DIR *pDir = &s->dir;
    FILINFO *pFNO = &s->fno;
    if (f_opendir(pDir, s->nameDir) == FR_OK)
    {
        int num_files = 0;
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(pDir, pFNO) != FR_OK)
            {
                *nameFileOut = '\0';
                f_closedir(pDir);
                return false;
            }
            if (pFNO->fname[0] == 0)
            {
                if (alreadyNull)
                {
                    *nameFileOut = '\0';
                    f_closedir(pDir);
                    return false;
                }
                alreadyNull = true;
            }
            if (numFile == num_files && (pFNO->fattrib & AM_DIR) == 0)
            {
                std::strcpy(nameFileOut, pFNO->fname);
                return true;
            }
            if ((pFNO->fattrib & AM_DIR) == 0 && (pFNO->fname[0] != '.'))
            {
                num_files++;
            }
        }
    }
    return false;
}



bool FDrive::GetNextNameFile(char *nameFileOut, StructForReadDir *s)
{
    FILINFO *pFNO = &s->fno;
    bool alreadyNull = false;
    while (true)
    {
        if (f_readdir(&s->dir, &s->fno) != FR_OK)
        {
            *nameFileOut = '\0';
            f_closedir(&s->dir);
            return false;
        }
        if (s->fno.fname[0] == 0)
        {
            if (alreadyNull)
            {
                *nameFileOut = '\0';
                f_closedir(&s->dir);
                return false;
            }
            alreadyNull = true;
        }
        else
        {
            if ((pFNO->fattrib & AM_DIR) == 0 && pFNO->fname[0] != '.')
            {
                std::strcpy(nameFileOut, pFNO->fname);
                return true;
            }
        }
    }
}



bool FDrive::OpenNewFileForWrite(const char *fullPathToFile, StructForWrite *structForWrite)
{
    if (f_open(&structForWrite->fileObj, fullPathToFile, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    {
        return false;
    }
    std::strcpy(structForWrite->name, fullPathToFile);
    structForWrite->sizeData = 0;
    return true;
}



bool FDrive::WriteToFile(uint8 *data, int sizeData, StructForWrite *structForWrite)
{
    while (sizeData > 0)
    {
        int dataToCopy = sizeData;
        if (sizeData + structForWrite->sizeData > StructForWrite::SIZE_FLASH_TEMP_BUFFER)
        {
            dataToCopy = StructForWrite::SIZE_FLASH_TEMP_BUFFER - structForWrite->sizeData;
        }
        sizeData -= dataToCopy;
        std::memcpy(structForWrite->tempBuffer + structForWrite->sizeData, data, (uint)dataToCopy);
        data += dataToCopy;
        structForWrite->sizeData += dataToCopy;
        if (structForWrite->sizeData == StructForWrite::SIZE_FLASH_TEMP_BUFFER)
        {
            uint wr = 0;
            if (f_write(&structForWrite->fileObj, structForWrite->tempBuffer, (uint)structForWrite->sizeData, &wr) != FR_OK || structForWrite->sizeData != (int)wr)
            {
                return false;
            }
            structForWrite->sizeData = 0;
        }
    }

    return true;
}



bool FDrive::CloseFile(StructForWrite *structForWrite)
{
    if (structForWrite->sizeData != 0)
    {
        uint wr = 0;
        if (f_write(&structForWrite->fileObj, structForWrite->tempBuffer, (uint)structForWrite->sizeData, &wr) != FR_OK || structForWrite->sizeData != (int)wr)
        {
            f_close(&structForWrite->fileObj);
            return false;
        }
    }
    f_close(&structForWrite->fileObj);

    SetTimeForFile(structForWrite->name);

    return true;
}


void FDrive::SetTimeForFile(char * /*name*/)
{
//    FILINFO info;
//
//    PackedTime time = CPU::RTC_::GetPackedTime();
//
//    info.fdate = (WORD)(((time.year + 2000 - 1980) * 512) | time.month * 32 | time.day);
//    info.ftime = (WORD)(time.hours * 2048 | time.minutes * 32 | time.seconds / 2);
//
//    f_utime(name, &info);
}


void FDrive::InitHardware()
{
    GPIO_InitTypeDef is =
    {
        GPIO_PIN_14 |               // 53 - USB_OTG_HS_DM
        GPIO_PIN_15,                // 54 - USB_OTG_HS_DP
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FAST,
        GPIO_AF12_OTG_HS_FS
    };

    HAL_GPIO_Init(GPIOB, &is);

    HAL_NVIC_SetPriority(OTG_HS_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}
