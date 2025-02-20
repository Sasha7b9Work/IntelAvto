// 2025/02/20 11:44:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "LAN/ServerTCP.h"
#include <lwip/tcp.h>


namespace ServerTCP
{
    static struct tcp_pcb *pcbServer = nullptr;     // Эти подключаемся к серверу

    // Эта функция вызывается, когда происходит подключение к серверу
    static err_t CallbackOnConnect(void *, struct tcp_pcb *, err_t);
}


void ServerTCP::Init()
{
    // Создаём клиента для подключения к серверу блока питания

    pcbServer = tcp_new();

    if (pcbServer != nullptr)
    {
        ip_addr_t ipaddr;

        IP4_ADDR(&ipaddr, 192, 168, 1, 200);

        err_t err = tcp_connect(pcbServer, &ipaddr, 30000, CallbackOnConnect);

        if (err == ERR_OK)
        {
        }
    }
}


err_t ServerTCP::CallbackOnConnect(void * /*arg*/, struct tcp_pcb * /*tpcb*/, err_t err)
{
    return err;
}
