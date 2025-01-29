// 2025/01/29 13:58:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Pictures/Picture.h"
#include "Display/Pictures/Signal1.inc"
#include "Display/Primitives_.h"
#include <miniz/miniz.h>
#include <cstring>


using namespace Primitives;


void Picture::DrawPicure(int x, int y, const uint8 *archive)
{
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

        if (mz_zip_reader_file_stat(&zip_archive, 0, &file_stat))
        {
            void *buffer = malloc((size_t)file_stat.m_uncomp_size);

            if(mz_zip_reader_extract_file_to_mem(&zip_archive, file_stat.m_filename, buffer, (size_t)file_stat.m_uncomp_size, 0))
            {
#pragma pack(1)
                struct StructureBMP
                {
                    uint8   magic[2];

                    uint    file_size;
                    uint16  creator1;
                    uint16  creator2;
                    uint    bmp_offset;

                    uint    header_size;
                    int     width;
                    int     height;
                    uint16  num_planes;
                    uint16  bits_per_pixel;
                    uint    compression;
                    uint    bmp_byte_size;
                    int     hres;
                    int     vres;
                    uint    num_colors;
                    uint    num_important_colors;
                };

                StructureBMP *head = (StructureBMP *)buffer;

                uint8 *pixel = ((uint8 *)buffer) + head->bmp_offset;

                for (int j = y + head->height; j > y; j--)
                {
                    for (int i = x; i < x + head->width; i++)
                    {
                        Point().Draw(i, j, *pixel++ < 1 ? Color::GRAY : Color::WHITE);
                    }
                }
            }

            free(buffer);
        }
    }

    return;
}
