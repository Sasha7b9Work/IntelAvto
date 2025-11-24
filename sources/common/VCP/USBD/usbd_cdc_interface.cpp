// 2025/03/19 11:06:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "VCP/VCP.h"
#include "Hardware/Timer.h"
#include "SCPI/SCPI.h"
#include <usbd_cdc.h>


namespace VCP
{
    extern bool isConnected;
    extern bool cableIsConnected;
}


USBD_CDC_LineCodingTypeDef LineCoding = {
  115200,                       // baud rate
  0x00,                         // stop bits-1
  0x00,                         // parity - none
  0x08                          // nb. of bits 8
};

#define APP_RX_DATA_SIZE  32
static uint8_t UserRxBuffer[APP_RX_DATA_SIZE];  // Received Data over USB are stored in this buffer


// USB handler declaration
extern USBD_HandleTypeDef USBD_Device;


static int8_t CDC_Itf_Init(void);
static int8_t CDC_Itf_DeInit(void);
static int8_t CDC_Itf_Control(uint8_t cmd, uint8_t *pbuf, uint16_t length);
static int8_t CDC_Itf_Receive(uint8_t *pbuf, uint32_t *Len);
static int8_t CDC_Itf_TransmitCplt(uint8_t *pbuf, uint32_t *Len, uint8_t epnum);


USBD_CDC_ItfTypeDef USBD_CDC_fops = {
  CDC_Itf_Init,
  CDC_Itf_DeInit,
  CDC_Itf_Control,
  CDC_Itf_Receive,
  CDC_Itf_TransmitCplt
};


static void SetAttributeConnected()
{
    VCP::cableIsConnected = true;
    VCP::isConnected = false;
}


static int8_t CDC_Itf_Init(void)
{
    USBD_CDC_SetRxBuffer(&VCP::handleUSBD, UserRxBuffer);
    Timer::SetDefferedOnceTask(TimerTask::USBD, 100, SetAttributeConnected);   // \todo Задержка введена для того, чтобы не было ложных срабатываний в
                                                                               // usbd_conf.c:HAL_PCD_SetupStageCallback при определении подключения хоста
    return USBD_OK;
}

// DeInitializes the CDC media low layer
static int8_t CDC_Itf_DeInit(void)
{
    VCP::cableIsConnected = false;
    VCP::isConnected = false;

    return USBD_OK;
}


static int8_t CDC_Itf_Control(uint8_t cmd, uint8_t *pbuf, uint16_t)
{
    switch (cmd)
    {
    case CDC_SEND_ENCAPSULATED_COMMAND:
        /* Add your code here */
        break;

    case CDC_GET_ENCAPSULATED_RESPONSE:
        /* Add your code here */
        break;

    case CDC_SET_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_GET_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_CLEAR_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_SET_LINE_CODING:
        LineCoding.bitrate = (uint32_t)(pbuf[0] | (pbuf[1] << 8) |
            (pbuf[2] << 16) | (pbuf[3] << 24));
        LineCoding.format = pbuf[4];
        LineCoding.paritytype = pbuf[5];
        LineCoding.datatype = pbuf[6];

        break;

    case CDC_GET_LINE_CODING:
        pbuf[0] = (uint8_t)(LineCoding.bitrate);
        pbuf[1] = (uint8_t)(LineCoding.bitrate >> 8);
        pbuf[2] = (uint8_t)(LineCoding.bitrate >> 16);
        pbuf[3] = (uint8_t)(LineCoding.bitrate >> 24);
        pbuf[4] = LineCoding.format;
        pbuf[5] = LineCoding.paritytype;
        pbuf[6] = LineCoding.datatype;
        break;

    case CDC_SET_CONTROL_LINE_STATE:
        /* Add your code here */
        break;

    case CDC_SEND_BREAK:
        /* Add your code here */
        break;

    default:
        break;
    }

    return USBD_OK;
}


static int8_t CDC_Itf_Receive(uint8 *buffer, uint *length)
{
    VCP::isConnected = true;

    SCPI::AppendNewData(DirectionSCPI::USB, buffer, *length);

    USBD_CDC_ReceivePacket(&VCP::handleUSBD);

    return USBD_OK;
}

// Data transmitted callback
// This function is IN transfer complete callback used to inform user that
// the submitted Data is successfully sent over USB.
static int8_t CDC_Itf_TransmitCplt(uint8_t *Buf, uint32_t *Len, uint8_t epnum)
{
    (void)Buf;
    (void)Len;
    (void)epnum;

    return (0);
}
