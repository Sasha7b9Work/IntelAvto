// 2025/01/29 13:58:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Pictures/Picture.h"
#include "Display/Pictures/Signal1.inc"
#include <miniz/miniz.h>
#include <cstring>

void Picture::DrawPicure(int x, int y, const uint8 *archive, void *buffer)
{
    uint bytes_decompress = 0;

    unsigned long length_archive = 0;

    if (archive == bmp_zip_Signal1)
    {
        length_archive = sizeof(bmp_zip_Signal1);
    }

    mz_zip_archive zip_archive;
    std::memset(&zip_archive, 0, sizeof(zip_archive));

    if (mz_zip_reader_init_mem(&zip_archive, archive, length_archive, 0))
    {
        mz_zip_archive_file_stat file_stat;
        
        if (!mz_zip_reader_file_stat(&zip_archive, 0, &file_stat))
        {
            return;
        }
    }

    int status = uncompress2((uint8 *)buffer, (unsigned long *)&bytes_decompress, archive, (unsigned long *)&length_archive);

    if (status == Z_OK)
    {
        // Разахивировалось
    }
}
