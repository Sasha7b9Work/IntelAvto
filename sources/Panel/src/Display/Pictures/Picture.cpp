// 2025/01/29 13:58:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Pictures/Picture.h"


void Picture::DrawPicure(int x, int y, const uint8 *archive, int length_archive, void *buffer)
{
    uint bytes_decompress = 0;

    int result = tinf_gzip_uncompress(buffer, &bytes_decompress, archive, length_archive);
}
