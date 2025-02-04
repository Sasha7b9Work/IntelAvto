// 2025/01/29 13:58:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


extern const unsigned char bmp_zip_Signal1[];


namespace Picture
{
    int GetLengthBuffer(const uint8 *);

    // Нарисовать картинку. buffer - место для её хранения без распковки
    void DrawPicure(int x, int y, const uint8 *);
}
