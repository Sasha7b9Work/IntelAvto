// 2025/02/20 11:44:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "LAN/ServerTCP.h"
#include <lwip/tcp.h>
#include <cstring>

namespace ServerTCP
{
    static tcp_pcb *pcbServer = nullptr;

    enum State
    {
        S_NOT_CONNECTED = 0,
        S_CONNECTED,
        S_RECEIVED,
        S_CLOSING,
    };

    struct Server
    {
        State    state;     // connection status
        tcp_pcb *pcb;       // pointer on the current tcp_pcb
        pbuf    *p_tx;      // pointer on pbuf to be transmitted
    };

    static void(*SocketFuncReciever)(pchar buffer, uint length) = nullptr;     // this function will be called when a message is recieved from any client

    static void Send(tcp_pcb *, Server *);
    static void CloseConnection(tcp_pcb *, Server *);

    static err_t CallbackOnConnect(void *, tcp_pcb *, err_t);
    static err_t CallbackRecv(void *, tcp_pcb *, pbuf *, err_t);
    static err_t CallbackPoll(void *, tcp_pcb *);
    static err_t CallbackSent(void *, tcp_pcb *, uint16);
}


void ServerTCP::Init()
{
    // Создаём клиента для подключения к серверу блока питания

    tcp_pcb *pcb = tcp_new();

    if (pcb != nullptr)
    {
        ip_addr_t ipaddr;

        IP4_ADDR(&ipaddr, 192, 168, 1, 4);

        tcp_connect(pcb, &ipaddr, 30000, CallbackOnConnect);
    }
}


err_t ServerTCP::CallbackOnConnect(void * /*arg*/, tcp_pcb *tpcb, err_t err)
{
    Server *es = nullptr;

    if (err == ERR_OK)
    {
        pcbServer = tpcb;

        /* allocate structure es to maintain tcp connection information */
        es = (Server *)mem_malloc(sizeof(Server));

        if (es != nullptr)
        {
            es->state = S_CONNECTED;
            es->pcb = tpcb;
            es->p_tx = nullptr;

            /* pass newly allocated es structure as argument to tpcb */
            tcp_arg(tpcb, es);

            /* initialize LwIP tcp_recv callback function */
            tcp_recv(tpcb, CallbackRecv);

            /* initialize LwIP tcp_sent callback function */
            tcp_sent(tpcb, CallbackSent);

            /* initialize LwIP tcp_poll callback function */
            tcp_poll(tpcb, CallbackPoll, 1);

            /* send data */
            Send(tpcb, es);

            SendString("I'm connected");

            return ERR_OK;
        }
        else
        {
            /* close connection */
            CloseConnection(tpcb, es);

            /* return memory allocation error */
            return ERR_MEM;
        }
    }
    else
    {
        /* close connection */
        CloseConnection(tpcb, es);
    }
    return err;
}


err_t ServerTCP::CallbackRecv(void *arg, tcp_pcb *tpcb, pbuf *p, err_t err)
{
    Server *es = (Server *)arg;

    /* if we receive an empty tcp frame from server => close connection */
    if (p == nullptr)
    {
        /* remote host closed connection */
        es->state = S_CLOSING;
        if (es->p_tx == nullptr)
        {
            /* we're done sending, close connection */
            CloseConnection(tpcb, es);
        }
        else
        {
            /* send remaining data*/
            Send(tpcb, es);
        }

        err = ERR_OK;
    }
    /* else : a non empty frame was received from echo server but for some reason err != ERR_OK */
    else if (err != ERR_OK)
    {
        /* free received pbuf*/
        if (p != nullptr)
        {
            pbuf_free(p);
        }
    }
    else if (es->state == S_CONNECTED)
    {
        es->state = S_RECEIVED;

        /* Acknowledge data reception */
        tcp_recved(tpcb, p->tot_len);

//        pbuf_free(p);
//        CloseConnection(tpcb, es);

        err = ERR_OK;
    }
    else if (es->state == S_RECEIVED)
    {
        // read some more data
        if (es->p_tx == nullptr)
        {
            //ss->p = _p;
            tcp_sent(tpcb, CallbackSent);
            //Send(_tpcb, ss);
            SocketFuncReciever((char *)p->payload, p->len);

            u8_t freed = 0;
            do
            {
                // try hard to free pbuf 
                freed = pbuf_free(p);
            } while (freed == 0);

        }
        else
        {
            pbuf *ptr;
            // chain pbufs to the end of what we recv'ed previously
            ptr = es->p_tx;
            pbuf_chain(ptr, p);
        }

        err = ERR_OK;
    }
    else if (es->state == S_CLOSING)
    {
        // odd case, remote side closing twice, trash data
        tcp_recved(tpcb, p->tot_len);
        es->p_tx = nullptr;
        pbuf_free(p);

        err = ERR_OK;

        CloseConnection(tpcb, es);
    }
    /* data received when connection already closed */
    else
    {
        /* Acknowledge data reception */
        tcp_recved(tpcb, p->tot_len);
        es->p_tx = nullptr;
        /* free pbuf and do nothing */
        pbuf_free(p);

        err = ERR_OK;
    }
    return err;
}


void ServerTCP::Send(tcp_pcb *tpcb, Server *es)
{
    err_t wr_err = ERR_OK;

    while ((wr_err == ERR_OK) &&
        (es->p_tx != nullptr) &&
        (es->p_tx->len <= tcp_sndbuf(tpcb)))
    {

        /* get pointer on pbuf from es structure */
        pbuf *ptr = es->p_tx;

        /* enqueue data for transmission */
        wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

        tcp_output(tpcb);

        if (wr_err == ERR_OK)
        {
            /* continue with next pbuf in chain (if any) */
            es->p_tx = ptr->next;

            if (es->p_tx != nullptr)
            {
                /* increment reference count for es->p */
                pbuf_ref(es->p_tx);
            }

            /* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
            pbuf_free(ptr);
        }
        else if (wr_err == ERR_MEM)
        {
            /* we are low on memory, try later, defer to poll */
            es->p_tx = ptr;
        }
        else
        {
            /* other problem ?? */
        }
    }
}


err_t ServerTCP::CallbackPoll(void *arg, tcp_pcb *tpcb)
{
    Server *es = (Server *)arg;

    err_t err = ERR_OK;

    if (es != nullptr)
    {
        if (es->p_tx != nullptr)
        {
            /* there is a remaining pbuf (chain) , try to send data */
            Send(tpcb, es);
        }
        else
        {
            /* no remaining pbuf (chain)  */
            if (es->state == S_CLOSING)
            {
                /* close tcp connection */
                CloseConnection(tpcb, es);
            }
        }
    }
    else
    {
        /* nothing to be done */
        tcp_abort(tpcb);

        err = ERR_ABRT;
    }

    return err;
}


err_t ServerTCP::CallbackSent(void *arg, tcp_pcb *tpcb, u16_t /*len*/)
{
    Server *es = (Server *)arg;

    if (es->p_tx != nullptr)
    {
        /* still got pbufs to send */
        Send(tpcb, es);
    }
    else
    {
        if (es->state == S_CLOSING)
        {
            CloseConnection(tpcb, es);
        }
    }

    return ERR_OK;
}


void ServerTCP::CloseConnection(tcp_pcb *tpcb, Server *es)
{
    /* remove callbacks */
    tcp_recv(tpcb, nullptr);
    tcp_sent(tpcb, nullptr);
    tcp_poll(tpcb, nullptr, 0);

    if (es != nullptr)
    {
        mem_free(es);
    }

    /* close tcp connection */
    tcp_close(tpcb);
}


void ServerTCP::SendString(pchar buffer)
{
    if (pcbServer)
    {
        uint16 length = (uint16)(std::strlen(buffer) + 1);

        pbuf *tcpBuffer = pbuf_alloc(PBUF_RAW, length, PBUF_POOL);
        tcpBuffer->flags = 1;
        pbuf_take(tcpBuffer, buffer, length);
        Server *ss = (Server *)mem_malloc(sizeof(Server));
        ss->p_tx = tcpBuffer;
        Send(pcbServer, ss);
        mem_free(ss);
    }
}
