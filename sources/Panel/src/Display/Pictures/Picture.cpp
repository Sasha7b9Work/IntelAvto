// 2025/01/29 13:58:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Pictures/Picture.h"
#include "Display/Pictures/Signal1.inc"
#include "Display/Pictures/Signal2a.inc"
#include "Display/Pictures/Signal2b.inc"
#include "Display/Pictures/Signal3a.inc"
#include "Display/Pictures/Signal3b.inc"
#include "Display/Pictures/Signal4.inc"
#include "Display/Pictures/Signal5a.inc"
#include "Display/Pictures/Signal5b.inc"
#include "Display/Primitives_.h"
#include "Hardware/Timer.h"
#include "Display/Display_.h"
#include <cstring>


using namespace Primitives;


namespace Picture
{
    static const unsigned char *archives[TypeSignal::Count] =
    {
        bmp_zip_Signal1,
        bmp_zip_Signal2a,
        bmp_zip_Signal2b,
        bmp_zip_Signal3a,
        bmp_zip_Signal3b,
        bmp_zip_Signal4,
        bmp_zip_Signal5a,
        bmp_zip_Signal5b
    };

    // \warn сюда нельз€ распаковать картинку больше 64 кЅ
    static uint8 buffer[1024 * 64] __attribute__ ((section("CCM_DATA")));
    static TypeSignal::E prev_type = TypeSignal::Count;

    static bool Uncompress(TypeSignal::E);

    static unsigned long CalculateSize(TypeSignal::E);
}


bool Picture::Uncompress(TypeSignal::E type)
{
//    if (type == prev_type)
//    {
//        return true;
//    }
//
//    prev_type = TypeSignal::Count;
//
//    mz_zip_archive zip_archive;
//    std::memset(&zip_archive, 0, sizeof(zip_archive));
//
//    if (mz_zip_reader_init_mem(&zip_archive, archives[type], CalculateSize(type), 0))
//    {
//        mz_zip_archive_file_stat file_stat;
//
//        if (mz_zip_reader_file_stat(&zip_archive, 0, &file_stat))
//        {
//            if (mz_zip_reader_extract_file_to_mem(&zip_archive, file_stat.m_filename, buffer, (size_t)file_stat.m_uncomp_size, 0))
//            {
//                prev_type = type;
//            }
//        }
//    }
//
//    mz_zip_reader_end(&zip_archive);
//
//    return (prev_type != TypeSignal::Count);

    return false;
}


void Picture::DrawPicure(int x, int y, TypeSignal::E type)
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

    if (Uncompress(type))
    {
        StructureBMP *head = (StructureBMP *)buffer; //-V641

        uint8 *pixel = ((uint8 *)buffer) + head->bmp_offset;

        for (int j = y + head->height; j > y; j--)
        {
            for (int i = x; i < x + head->width; i++)
            {
                if (*pixel++ > 1)
                {
                    Point().Draw(i, j, Color::WHITE);
                }
            }
        }
    }
}


//unsigned long Picture::CalculateSize(TypeSignal::E type)
//{
//    static unsigned long sizes[TypeSignal::Count] =
//    {
//        sizeof(bmp_zip_Signal1),
//        sizeof(bmp_zip_Signal2a),
//        sizeof(bmp_zip_Signal2b),
//        sizeof(bmp_zip_Signal3a),
//        sizeof(bmp_zip_Signal3b),
//        sizeof(bmp_zip_Signal4),
//        sizeof(bmp_zip_Signal5a),
//        sizeof(bmp_zip_Signal5b)
//    };
//
//    return sizes[type];
//}
