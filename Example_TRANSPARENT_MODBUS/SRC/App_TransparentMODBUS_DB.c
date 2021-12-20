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
 @brief This is a demonstration of the UART Transparent Mode Example over RIIM - Development Board/Mesh Router Code
 @version 1.00 : First revision
 @date 2020.05.15
*/

#include "RIIM_UAPI.h"

#define MaxUDPPayloadLength 128
#define UARTtimeOut 3000
#define RadioChannel 32
#define UDPport 12345
#define OutputPower 14
#define UARTBaudRate 38400
#define MaxPayloadLength 128
#define RS485_TX_RX_control true

static uint8_t payload[MaxUDPPayloadLength];
//static const uint32_t   timerPeriod=UARTtimeOut;

void rxHandler(uint8_t *buffer, uint8_t len);

static uint8_t blinkTimerHandler;

/**
 * @brief A timer that blinks LED on mesh router. Started when module starts and stops blinking when node is online and part of network.
*/
static void startup()
{
    // Wait until we are part of network
    if(Network.getNetworkState() != ONLINE){
        GPIO.toggle(GPIO_6);
        return;
    } else {
        GPIO.setValue(GPIO_6, HIGH);
        Timer.stop(blinkTimerHandler);
        return;
    }
}

/**
 * @brief When you receive "MaxPayloadLength" bytes or when you stop writing for "UARTtimeOut" milli seconds, payload is transmitted to the Border Router and its printed there via UART.
 */
void rxHandler(uint8_t *buffer, uint8_t len)
{  
    uint8_t i;
    IPAddr Addr;
    if (len>0)//((len>0)&&(status == 0))
    {
        if(Network.getNetworkState() != ONLINE)
        {
            return;
        }
        if(Network.getBorderRouterAddress(&Addr)!=UAPI_OK)
        {
            return;
        }
        for(i=0;i<len;i++) 
        {
            payload[i]=buffer[i];
        }
        UDP.send6(Addr, UDPport, buffer, len);
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
 * @brief When we receive a UDP packet, print it out on UART
 */
void udpCallback(IPAddr src_ipAddr, uint8_t *payload, uint8_t payloadSize)
{
    UART.startTransmit(payload,payloadSize,modbusTxHandler);
    return;
}

/**
 * @brief This is the entry point of the user application. It is
 *        called only once during startup. It is responsible for
 *        setting up callbacks, timers etc.
 */

RIIM_SETUP()
{
    UART.enableRXTX(RS485_TX_RX_control);  
    UART.init(UARTBaudRate, UART_PARITY_NONE, UART_DATA_8_BITS, UART_STOP_1_BIT);
    const uint8_t nwKey[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    Network.setNWKey((uint8_t*)nwKey);
    Network.setPanId(4444);
    Network.startMeshRouter();
    Network.setTxPower(OutputPower);
    GPIO.setDirection(GPIO_6, OUTPUT);
    GPIO.setValue(GPIO_6, HIGH);
    blinkTimerHandler=Timer.create(PERIODIC, 500, startup);
    Timer.start(blinkTimerHandler);
    UDP.setup(udpCallback,UDPport);
    UART.startReceive_Adv(rxHandler,MaxPayloadLength,UARTtimeOut,UART_Timeout_Infinite,UART_No_Termination_Character,false);   
    return UAPI_OK;
}