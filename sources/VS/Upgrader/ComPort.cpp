// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "ComPort.h"
#include "rs232.h"
#include <cstring>
#include <ctime>


static int openedPort = -1;


bool ComPort::Open()
{
    char mode[] = { '8', 'N', '1', '\0' };

    uint8 buffer[4096];

    uint8 message[] = "*idn?\x0d";

    for (int i = 0; i < 30; i++)
    {
        if (RS232_OpenComport(i, 115200, mode, 0) == 0)
        {
            RS232_SendBuf(i, message, 6);

            int n = RS232_PollComport(i, buffer, 4095);

            if (n > 0)
            {
                openedPort = i;
                return true;
            }

            RS232_CloseComport(i);
        }
    }

    return false;
}

void ComPort::Close()
{
    if (openedPort != -1)
    {
        RS232_CloseComport(openedPort);
    }
}


bool ComPort::IsOpened()
{
    return openedPort != -1;
}


void ComPort::Send(pchar buffer)
{
    if (IsOpened())
    {
        char *p = const_cast<char *>(buffer);

        RS232_SendBuf(openedPort, reinterpret_cast<uint8 *>(p), static_cast<int>(std::strlen(buffer)));
    }
}


void ComPort::Send(void *buffer, int size)
{
    if (IsOpened())
    {
        RS232_SendBuf(openedPort, (uint8 *)buffer, size);
    }
}

int ComPort::Receive(void *buffer, int size, int timeWait)
{
    if (IsOpened())
    {
        int received = 0;

        clock_t timeStart = clock();

        while (received < size && clock() < timeStart + timeWait)
        {
            int newBytes = RS232_PollComport(openedPort, reinterpret_cast<unsigned char *>((uint8 *)buffer + received), size - received);
            received += newBytes;
        }

        return received;
    }

    return 0;
}
