// 2022/03/30 08:25:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/String_.h"


//template      String<(int)DEFAULT_SIZE_STRING>::String(char *, ...);
template void String<(int)DEFAULT_SIZE_STRING>::Append(pchar);
template void String<(int)DEFAULT_SIZE_STRING>::SetFormat(pchar format, ...);
template void String<128>::AppendFormat(pchar format, ...);
template void String<1024>::SetFormat(pchar format, ...);
template void String<(int)DEFAULT_SIZE_STRING>::Append(char);
template bool String<(int)DEFAULT_SIZE_STRING>::ToInt(int *);
template void String<(int)DEFAULT_SIZE_STRING>::AppendBytes(const void *, int);
template String<(int)DEFAULT_SIZE_STRING> String<(int)DEFAULT_SIZE_STRING>::GetWord(int);


template<int capacity>
String<capacity> String<capacity>::GetWord(const int num_word)
{
    String<capacity> result;

    int pos = 0;

    int current_word = 0;

    while (pos < Size())
    {
        // Выбираем пробелы
        while (pos < Size() && buffer[pos] == ' ')
        {
            pos++;
        }

        // Теперь выбираем символы
        while (pos < Size() && buffer[pos] != ' ')
        {
            result.Append(buffer[pos]);
            pos++;
        }

        if (current_word == num_word)
        {
            return result;
        }

        result.Clear();

        current_word++;
    }

    return result;
}


template<int capacity>
void String<capacity>::SetFormat(pchar format, ...)
{
    std::va_list args;
    va_start(args, format);
    int num_symbols = std::vsprintf(buffer, format, args);
    va_end(args);

    if(num_symbols < 0 || num_symbols > capacity - 1)
    {
        LOG_ERROR("Very small string buffer %d, need %d:", capacity, num_symbols);
    }
}


template<int capacity>
void String<capacity>::AppendFormat(pchar format, ...)
{
    char append[capacity];
    std::va_list args;
    va_start(args, format);
    int num_symbols = std::vsprintf(append, format, args);
    va_end(args);

    if (num_symbols < 0 || num_symbols > capacity - Size() - 1)
    {
        LOG_ERROR("Very small string buffer %d, need %d:", capacity, num_symbols);
    }
    else
    {
        std::strcat(buffer, append);
    }
}


template<int capacity>
void String<capacity>::Append(pchar str)
{
    int need_size = Size() + (int)std::strlen(str) + 1;

    if (need_size > capacity)
    {
        LOG_ERROR("Very small string buffer %d, need %d:", capacity, need_size);

        int pointer = Size();

        while (pointer < capacity)
        {
            Append(str[pointer++]);
        }

        buffer[capacity - 1] = '\0';

        LOG_WRITE(buffer);
    }
    else
    {
        std::strcat(buffer, str);
    }
}


template<int capacity>
void String<capacity>::AppendBytes(const void *bytes, int num_bytes)
{
    int need_size = Size() + num_bytes + 1;

    if (need_size > capacity)
    {
        LOG_ERROR("Very small string buffer %d, need %d:", capacity, need_size);
        LOG_WRITE(c_str());
    }
    else
    {
        std::memcpy(buffer + Size(), bytes, (uint)num_bytes);
        buffer[need_size - 1] = '\0';
    }
}


template<int capacity>
void String<capacity>::Append(char symbol)
{
    if (!Filled())
    {
        int pos = (int)std::strlen(buffer);
        buffer[pos] = symbol;
        buffer[pos + 1] = '\0';
    }
    else
    {
        LOG_ERROR("buffer is full");
    }
}


template<int capacity>
bool String<capacity>::ToInt(int *out)
{
    char *str = buffer;

    int sign = str[0] == '-' ? -1 : 1;

    if (str[0] < '0' || str[0] > '9')
    {
        ++str;
    }

    int length = (int)(std::strlen(str));

    if (length == 0)
    {
        return false;
    }

    *out = 0;

    int pow = 1;

    for (int i = length - 1; i >= 0; i--)
    {
        int val = str[i] & (~(0x30));
        if (val < 0 || val > 9)
        {
            return false;
        }
        *out += val * pow;
        pow *= 10;
    }

    if (sign == -1)
    {
        *out *= -1;
    }

    return true;
}
