// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/String.h"


struct Word
{
    char   *address;
    int8    numSymbols;
    uint8   notUsed0;
    uint8   notUsed1;
    uint8   notUsed2;
};


class SU
{
public:
    // ��� ������� ����� ����������� � �������� �������� �����.
    static bool GetWord(pchar string, Word *word, const int numWord);

    static bool WordEqualZeroString(Word *word, pchar string);

    // ���������� ��� ������. ����� �������� ������ �� str1
    static bool EqualsStrings(pchar str1, pchar str2);

    static bool EqualsStrings(void *str1, void *str2, int size);

    static bool EqualsStrings(uint8 *str1, pchar  const str2, int size);

    static bool EqualsZeroStrings(char *str1, char *str2);

    // ���������� ������� ������� �������, ������� �� �������� ������ ���� ������
    static int FirstNotNumeral(char *buffer);

    // ����������� ������ � float. �������������� ������� �� ������� ����������� �������
    static float StringToFloat(pchar string);

    static char *ToUpper(char *str, int size);

    static char *ToUpper(char *str);

    static char ToUpper(char symbol);

    static char ToLower(char symbol);

    static char *DoubleToString(double value);

    static bool IsDigit(char symbol);

    static bool StringToDouble(double *value, pchar buffer);

    static bool String2Int(pchar buffer, int *value, char **end);
 
    static String Int2String(int n);

    // ��������� � ������ buffer �������� size ����� digits ����. ������ ���� ���� �����������
    static void LeaveFewDigits(char *buffer, int size, int digits);

    // ��������� � ������ buffer �������� size ����� digits �������� ����. ��� ����� ������ ���������� �����
    static void LeaveFewSignedDigits(char *buffer, int size, int digits);

    // �������� � ������ ��� ������� old �� new
    static void ReplaceSymbol(char *string, char oldSymbol, char newSymbol);

    // ���������� true, ���� � ������ ���������� ������ symbol
    static bool ExistSymbol(char *string, char symbol);
};


#ifndef WIN32

int strcpy_s(char *dest, uint dest_size, pchar src);

#endif
