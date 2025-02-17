// 2025/02/17 15:25:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "LAN/LAN.h"


void LAN::Init()
{
    /* Initialize the LwIP stack */
    lwip_init();

    /* Configure the Network interface */
    Netif_Config();

    /* Http webserver Init */
    http_server_init();
}


void LAN::Update()
{
    /* Read a received packet from the Ethernet buffers and send it
       to the lwIP for handling */
    ethernetif_input(&gnetif);

    /* Handle timeouts */
    sys_check_timeouts();

#if LWIP_NETIF_LINK_CALLBACK
    Ethernet_Link_Periodic_Handle(&gnetif);
#endif

#if LWIP_DHCP
    DHCP_Periodic_Handle(&gnetif);
#endif
}
