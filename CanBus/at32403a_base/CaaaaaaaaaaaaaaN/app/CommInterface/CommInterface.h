/*
 * Uart_Driver.h
 *
 *  Created on: 16 но€б. 2018 г.
 *      Author: Andrey
 */

#ifndef ECU_BT_UART_UART_DRIVER_H_
#define ECU_BT_UART_UART_DRIVER_H_
#include "uartInit.h"
#include "Message.h"
#include "stdint.h"
void initCommInterface(void);
void sendPacket(uint8_t *data_ptr,uint8_t data_size);
#endif /* ECU_BT_UART_UART_DRIVER_H_ */
