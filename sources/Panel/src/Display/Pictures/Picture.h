// 2025/01/29 13:58:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


extern const unsigned char bmp_zip_Signal1[];


namespace Picture
{
    // ���������� ��������. buffer - ����� ��� � �������� ��� ���������
    void DrawPicure(int x, int y, const uint8 *, int num_bytes, void *buffer);
}
