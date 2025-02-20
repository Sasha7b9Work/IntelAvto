// 2025/02/20 11:44:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "LAN/ServerTCP.h"
#include <lwip/tcp.h>


namespace ServerTCP
{
    static tcp_pcb *server = nullptr;     // Эти подключаемся к серверу

    // Эта функция вызывается, когда происходит подключение к серверу
    static err_t CallbackOnConnect(void *, tcp_pcb *, err_t);
}


void ServerTCP::Init()
{
    // Создаём клиента для подключения к серверу блока питания

    tcp_pcb *pcb = tcp_new();

    if (pcb != nullptr)
    {
        ip_addr_t ipaddr;

        IP4_ADDR(&ipaddr, 192, 168, 1, 200);

        err_t err = tcp_connect(pcb, &ipaddr, 30000, CallbackOnConnect);

        if (err == ERR_OK)
        {
        }
    }
}


err_t ServerTCP::CallbackOnConnect(void * /*arg*/, tcp_pcb *tpcb, err_t err)
{
    server = tpcb;

    return err;
}
