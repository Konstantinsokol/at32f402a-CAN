/*
 * uartInit.h
 *
 *  Created on: 2023 Apr 1
 *      Author: on4ip
 */

#ifndef USER_UARTINIT_H_
#define USER_UARTINIT_H_

#include "at32f403a_407_conf.h"
#include "at32f403a_407_int.h"
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

/* Set board USART NUMBER */

enum UART_BAUDS
{
	BAUD_115200,
	MAX_UART_BAUDS
};

void uartInit(enum UART_BAUDS baud,uint8_t isrEna);

void setUartRxCallBack(void *);

void sendToUart(uint8_t *data,uint16_t len);


#endif /* USER_UARTINIT_H_ */
