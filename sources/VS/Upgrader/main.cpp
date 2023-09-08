// 2022/1/17 19:45:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma warning(push, 0)
#pragma warning(disable : 4668)
#include <iostream>
#include <fstream>
#pragma warning(pop)
#include "defines.h"
#include "ComPort.h"


/*
*   Формат файла таков:
*   Сначала идут 4 байта размера
*   Потом для каждого полного или неполного (последнего) килобайта записывается 4 байта хэша
*/


using namespace std;

static uint CalculateCRC32(char *buffer, int size);
static void SendFirmware(pchar file);
static void OpenPort();
static bool EraseSector();
static bool SendZone(int num, uint hash, void *buffer, int size);



int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Using :" << endl;
        cout << "        checsum input_file" << endl;
        return -1;
    }

    OpenPort();

    if (ComPort::IsOpened())
    {
        ComPort::Close();

        SendFirmware(argv[1]);
    }

    return 0;
}


static void OpenPort()
{
    for (int i = 0; i < 10; i++)
    {
        ComPort::Open();
        if (ComPort::IsOpened())
        {
            break;
        }
    }
}


static unsigned int CalculateCRC32(char *buffer, int size)
{
    unsigned int hash = 0;

    while (size--)
    {
        hash = (*buffer++) + (hash << 6u) + (hash << 16u) - hash;
    }

    return hash;
}


static void SendFirmware(pchar file)
{
    ifstream ifile;
    ifile.open(string(file) + ".bin", ios::in | ios::binary);

    if (ifile.is_open())
    {
        ifile.seekg(0, ifile.end);
        int length = (int)ifile.tellg();
        ifile.seekg(0, ifile.beg);

        int number = 0;;

        while (!EraseSector())
        {
        }

        while (length)
        {
            char buffer[1024];

            int read_bytes = (length < 1024) ? length : 1024;

            length -= read_bytes;

            ifile.read(buffer, read_bytes);

            uint hash = CalculateCRC32(buffer, read_bytes);

            while (!SendZone(number, hash, buffer, read_bytes))
            {
            }

            number++;
        }

        ifile.close();
    }
    else
    {
        cout << "ERROR !!! Input file " << file << ".bin" << " not found" << endl;
    }
}


static bool EraseSector()
{
    pchar sended_string = ":upgrade:erase\x0d";

    ComPort::Send(sended_string);

    char buffer[100];

    int numBytes = ComPort::Receive(buffer, (int)strlen(sended_string), 1000);

    buffer[numBytes] = '\0';

    return (numBytes == (int)strlen(sended_string)) && strcmp(buffer, sended_string) == 0;
}


static bool SendZone(int num, uint hash, void *data, int size)
{
    char buffer[100];

    sprintf(buffer, ":upgrade:write %d %d %d\x0d", num, hash, size);

    ComPort::Send(buffer);

    ComPort::Send(data, size);

    uint recv_buffer[2];                // Сюда будем принимать ответ с номером зоны и контрольной суммой

    if (ComPort::Receive(recv_buffer, 8, 1000) == 8)
    {
        return (recv_buffer[0] == (uint)num) && (recv_buffer[1] == hash);
    }

    return false;
}
