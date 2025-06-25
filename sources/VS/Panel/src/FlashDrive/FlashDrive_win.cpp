// 2025/05/12 14:20:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FlashDrive/FlashDrive.h"


bool FDrive::IsConnected()
{
    return false;
}


void FDrive::Update()
{
}


bool FDrive::OpenNewFileForWrite(const char * /*fullPathToFile*/, StructForWrite * /*structForWrite*/)
{
    LOG_ERROR("Not implemented");

    return false;
}


bool FDrive::WriteToFile(uint8 * /*data*/, int /*sizeData*/, StructForWrite * /*structForWrite*/)
{
    LOG_ERROR("Not implemented");

    return false;
}


bool FDrive::CloseFile(StructForWrite * /*structForWrite*/)
{
    LOG_ERROR_NOT_IMPLEMENTED();

    return false;
}
