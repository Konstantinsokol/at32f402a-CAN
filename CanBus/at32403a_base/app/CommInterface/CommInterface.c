/*
 * Uart_Driver.c
 *
 *  Created on: 16 но€б. 2018 г.
 *      Author: Andrey
 */

//#include <DataModel.h>
#include "CommInterface.h"

static void (*sendMessage)(uint8_t *, uint16_t)=NULL;
/*!
 * @brief Initialization function for UART hardware
 * @param baud bautrate of uart
 */
void initCommInterface(void)
{
	uartInit(BAUD_115200, TRUE);
	/*Init transmit function*/
	sendMessage = sendToUart;
	/*Init receive function */
	setUartRxCallBack(processRxByte);
}
void sendPacket(uint8_t *data_ptr,uint8_t data_size)
{
	if(sendMessage!=NULL)
	{
		sendMessage(data_ptr,data_size);
	}

}
