
/*****************************************************************************
 * Copyright ©2020. Radiocrafts AS (Radiocrafts).  All Rights Reserved.
 * Permission to use, copy, modify, and distribute this software and
 * its documentation, without fee and without a signed licensing
 * agreement, is hereby granted, provided that the above copyright
 * notice, this paragraph and the following two paragraphs appear in
 * all copies, modifications, and distributions.
 *
 * IN NO EVENT SHALL RADIOCRFTS BE LIABLE TO ANY PARTY FOR DIRECT,
 * INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
 * LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
 * DOCUMENTATION, EVEN IF RADIOCRAFTS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * RADIOCRAFTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
 * DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
 * RADIOCRAFTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
 * UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
****************************************************************************/
/**
 @file
 @brief This is a demonstration of the UART Transparent Mode Example over RIIM - Border Router Code
 @version 1.00 : First revision
 @date 2020.05.15
*/

#include "RIIM_UAPI.h"
#define MaxPayloadLength 128
#define UARTtimeOut 3000
#define RadioChannel 32
#define UDPport 12345
#define OutputPower 14
#define UARTBaudRate 115200
#define NumberOfModbusDevices 10
#define DUMMY_ADDRESS {.byte={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}}

typedef struct modbusDevice
{
    IPAddr ipaddr;
    uint8_t modbusaddr;
} modbusDevice;

const IPAddr 			dummyAddr = DUMMY_ADDRESS;
const IPAddr 			mcAddr = UAPI_UDP_ALL_LOCAL_NODES;

modbusDevice deviceList[NumberOfModbusDevices];
uint8_t					payload[MaxPayloadLength];

void rxHandler(uint8_t *buffer, uint8_t len);

void initDeviceList()
{
    uint8_t i;

    for (i=0;i<NumberOfModbusDevices;i++)
    {
        deviceList[i].ipaddr = dummyAddr;
        deviceList[i].modbusaddr = 255;
    }
}

 void rxHandler(uint8_t *buffer, uint8_t len)
{
    uint8_t i,j;
    static IPAddr Addr;

    if(len>0)
    {
       for(i=0;i<len;i++)
       {
        payload[i]=buffer[i];
       }
    UART.startReceive_Adv(rxHandler,MaxPayloadLength,UARTtimeOut,UART_Timeout_Infinite,UART_No_Termination_Character,false);   
    
    Addr = mcAddr;

    for (j=0;j<NumberOfModbusDevices;j++)
     {
         if (payload[0]==deviceList[j].modbusaddr)
         {
             Addr = deviceList[j].ipaddr;
             //Util.printf("A");
         }
     }

    UDP.send6(Addr, UDPport, payload, len);
    }
    else
    {
     UART.startReceive_Adv(rxHandler,MaxPayloadLength,UARTtimeOut,UART_Timeout_Infinite,UART_No_Termination_Character,false);   
    }
}

void modbusTxHandler(uint8_t len)
{  
    UART.startReceive_Adv(rxHandler,MaxPayloadLength,UARTtimeOut,UART_Timeout_Infinite,UART_No_Termination_Character,false);  
}

/**
 * @brief When we receive a UDP packet. If this is a new modbus device we add it to the modbus/IP-address mapping table.
 * Then we send the incoming packet to UART
 */

void udpCallback(IPAddr src_ipAddr, uint8_t *payload, uint8_t payloadSize)
{
    uint8_t i;
    uint8_t reg;
    reg = 0;

    for (i=0;i<NumberOfModbusDevices;i++)
    {
        if (payload[0]==deviceList[i].modbusaddr)
         {
             reg = 1;
         }
         
         if ((reg==0)&&(deviceList[i].modbusaddr == 255))
         {
             deviceList[i].modbusaddr = payload[0];
             deviceList[i].ipaddr = src_ipAddr;
             reg = 1;
         }
    }
    UART.startTransmit(payload,payloadSize,modbusTxHandler);
}

/**
 * @brief This is the entry point of the user application. It is
 *        called only once during startup. It is responsible for
 *        setting up callbacks, timers etc.
 */
RIIM_SETUP()
{
    UART.init(UARTBaudRate, UART_PARITY_NONE, UART_DATA_8_BITS, UART_STOP_1_BIT);
    const uint8_t nwKey[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    Network.setNWKey((uint8_t*)nwKey);
    Network.setPanId(4444);
    Network.startBorderRouter(NULL, NULL, NULL, NULL);
    Network.setChannel(RadioChannel);
    Network.setTxPower(OutputPower);
    Network.setTschMaxBroadcastRate(2); 
    initDeviceList();
    UDP.setup(udpCallback,UDPport);
    UART.startReceive_Adv(rxHandler,MaxPayloadLength,UARTtimeOut,UART_Timeout_Infinite,UART_No_Termination_Character,false);   
    return UAPI_OK;
}