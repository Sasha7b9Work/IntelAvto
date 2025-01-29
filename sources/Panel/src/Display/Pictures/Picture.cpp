// 2025/01/29 13:58:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Pictures/Picture.h"
#include "Display/Pictures/Signal1.inc"
#include <miniz/miniz.h>


void Picture::DrawPicure(int x, int y, const uint8 *archive, void *buffer)
{
    uint bytes_decompress = 0;

    unsigned long length_archive = 0;

    if (archive == bmp_zip_Signal1)
    {
        length_archive = sizeof(bmp_zip_Signal1);
    }

    int status = uncompress((uint8 *)buffer, (unsigned long *)&bytes_decompress, archive, (unsigned long)length_archive);

    if (status == Z_OK)
    {
        // Разахивировалось
    }
}
