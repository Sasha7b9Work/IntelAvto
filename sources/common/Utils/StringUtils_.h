// 2023/09/08 21:07:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
//#include "Utils/String.h"


struct Word
{
    char   *address;
    int8    numSymbols;
    uint8   notUsed0;
    uint8   notUsed1;
    uint8   notUsed2;
};


namespace SU
{
    // Эта команда сразу преобразует к верхенму регистру слово.
    bool GetWord(pchar string, Word *word, const int numWord);

    bool WordEqualZeroString(Word *word, pchar string);

    // Сравнивает две строки. Число символов берётся из str1
    bool EqualsStrings(pchar str1, pchar str2);

    bool EqualsStrings(void *str1, void *str2, int size);

    bool EqualsStrings(uint8 *str1, pchar  const str2, int size);

    bool EqualsZeroStrings(char *str1, char *str2);

    // Возвращает позицию первого символа, который не является цифрой либо точкой
    int FirstNotNumeral(char *buffer);

    // Преобразует строку в float. Обрабатываются символы до первого нецифрового символа
    float StringToFloat(pchar string);

    char *ToUpper(char *str, int size);

    char *ToUpper(char *str);

    char ToUpper(char symbol);

    char ToLower(char symbol);

    bool IsDigit(char symbol);

    bool StringToDouble(double *value, pchar buffer);

    bool String2Int(pchar buffer, int *value, char **end);
 
    pchar Int2String(int n, char out[32]);

    // Оставляет в строке buffer размером size ровно digits цифр. Первые нули тоже учитываются
    void LeaveFewDigits(char *buffer, int size, int digits);

    // Оставляет в строке buffer размером size ровно digits значащих цифр. Без учёта первых незначащих нулей
    void LeaveFewSignedDigits(char *buffer, int size, int digits);

    // Заменить в строке все символы old на new
    void ReplaceSymbol(char *string, char oldSymbol, char newSymbol);

    // Возвращает true, если в строке существует символ symbol
    bool ExistSymbol(char *string, char symbol);
    
    inline uint Hash(uint hash, char byte)
    {
        return (uint8)byte + (hash << 6) + (hash << 16) - hash;
    }

    inline uint CalculateHash(const void *buffer, int size)
    {
        uint crc = 0;

        const uint8 *data = (const uint8 *)buffer;

        for (int i = 0; i < size; i++)
        {
            crc = Hash(crc, (char)*data++);
        }

        return crc;
    }

    pchar TimeMStoText(uint timeMS, char buffer[32]);
};


#ifndef WIN32

int strcpy_s(char *dest, uint dest_size, pchar src);

#endif
