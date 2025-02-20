// 2025/02/20 11:44:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "LAN/ServerTCP.h"
#include <lwip/tcp.h>
#include <cstring>

namespace ServerTCP
{
    static __IO uint32_t message_count = 0;

    static uint8 data[100];

    enum echoclient_states
    {
        ES_NOT_CONNECTED = 0,
        ES_CONNECTED,
        ES_RECEIVED,
        ES_CLOSING,
    };

    struct echoclient
    {
        enum echoclient_states state; /* connection status */
        struct tcp_pcb *pcb;          /* pointer on the current tcp_pcb */
        struct pbuf *p_tx;            /* pointer on pbuf to be transmitted */
    };

    /* Private function prototypes -----------------------------------------------*/
    static err_t tcp_echoclient_recv(void *, tcp_pcb *, pbuf *, err_t);
    static void tcp_echoclient_connection_close(tcp_pcb *, echoclient *);
    static err_t tcp_echoclient_poll(void *, tcp_pcb *);
    static err_t tcp_echoclient_sent(void *, tcp_pcb *, uint16);
    static void tcp_echoclient_send(tcp_pcb *, echoclient *);

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
    struct echoclient *es = nullptr;

    if (err == ERR_OK)
    {
        /* allocate structure es to maintain tcp connection information */
        es = (struct echoclient *)mem_malloc(sizeof(struct echoclient));

        if (es != nullptr)
        {
            es->state = ES_CONNECTED;
            es->pcb = tpcb;

            sprintf((char *)data, "sending tcp client message %d", (int)message_count);

            /* allocate pbuf */
            es->p_tx = pbuf_alloc(PBUF_TRANSPORT, (uint16)std::strlen((char *)data), PBUF_POOL);

            if (es->p_tx)
            {
                /* copy data to pbuf */
                pbuf_take(es->p_tx, (char *)data, (uint16)std::strlen((char *)data));

                /* pass newly allocated es structure as argument to tpcb */
                tcp_arg(tpcb, es);

                /* initialize LwIP tcp_recv callback function */
                tcp_recv(tpcb, tcp_echoclient_recv);

                /* initialize LwIP tcp_sent callback function */
                tcp_sent(tpcb, tcp_echoclient_sent);

                /* initialize LwIP tcp_poll callback function */
                tcp_poll(tpcb, tcp_echoclient_poll, 1);

                /* send data */
                tcp_echoclient_send(tpcb, es);

                return ERR_OK;
            }
        }
        else
        {
            /* close connection */
            tcp_echoclient_connection_close(tpcb, es);

            /* return memory allocation error */
            return ERR_MEM;
        }
    }
    else
    {
        /* close connection */
        tcp_echoclient_connection_close(tpcb, es);
    }
    return err;
}


err_t ServerTCP::tcp_echoclient_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    struct echoclient *es;
    err_t ret_err;

    LWIP_ASSERT("arg != nullptr", arg != nullptr);

    es = (struct echoclient *)arg;

    /* if we receive an empty tcp frame from server => close connection */
    if (p == nullptr)
    {
        /* remote host closed connection */
        es->state = ES_CLOSING;
        if (es->p_tx == nullptr)
        {
            /* we're done sending, close connection */
            tcp_echoclient_connection_close(tpcb, es);
        }
        else
        {
            /* send remaining data*/
            tcp_echoclient_send(tpcb, es);
        }
        ret_err = ERR_OK;
    }
    /* else : a non empty frame was received from echo server but for some reason err != ERR_OK */
    else if (err != ERR_OK)
    {
        /* free received pbuf*/
        if (p != nullptr)
        {
            pbuf_free(p);
        }
        ret_err = err;
    }
    else if (es->state == ES_CONNECTED)
    {
        /* increment message count */
        message_count++;

        /* Acknowledge data reception */
        tcp_recved(tpcb, p->tot_len);

        pbuf_free(p);
        tcp_echoclient_connection_close(tpcb, es);
        ret_err = ERR_OK;
    }

    /* data received when connection already closed */
    else
    {
        /* Acknowledge data reception */
        tcp_recved(tpcb, p->tot_len);

        /* free pbuf and do nothing */
        pbuf_free(p);
        ret_err = ERR_OK;
    }
    return ret_err;
}


void ServerTCP::tcp_echoclient_send(struct tcp_pcb *tpcb, struct echoclient *es)
{
    struct pbuf *ptr;
    err_t wr_err = ERR_OK;

    while ((wr_err == ERR_OK) &&
        (es->p_tx != nullptr) &&
        (es->p_tx->len <= tcp_sndbuf(tpcb)))
    {

        /* get pointer on pbuf from es structure */
        ptr = es->p_tx;

        /* enqueue data for transmission */
        wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

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


err_t ServerTCP::tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb)
{
    err_t ret_err;
    struct echoclient *es;

    es = (struct echoclient *)arg;
    if (es != nullptr)
    {
        if (es->p_tx != nullptr)
        {
            /* there is a remaining pbuf (chain) , try to send data */
            tcp_echoclient_send(tpcb, es);
        }
        else
        {
            /* no remaining pbuf (chain)  */
            if (es->state == ES_CLOSING)
            {
                /* close tcp connection */
                tcp_echoclient_connection_close(tpcb, es);
            }
        }
        ret_err = ERR_OK;
    }
    else
    {
        /* nothing to be done */
        tcp_abort(tpcb);
        ret_err = ERR_ABRT;
    }
    return ret_err;
}


err_t ServerTCP::tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    struct echoclient *es;

    LWIP_UNUSED_ARG(len);

    es = (struct echoclient *)arg;

    if (es->p_tx != nullptr)
    {
        /* still got pbufs to send */
        tcp_echoclient_send(tpcb, es);
    }

    return ERR_OK;
}


void ServerTCP::tcp_echoclient_connection_close(struct tcp_pcb *tpcb, struct echoclient *es)
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
