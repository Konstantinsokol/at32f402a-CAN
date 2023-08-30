/*
 * Message.h
 *
 *  Created on: 7 окт. 2022 г.
 *      Author: on4ip
 */

#ifndef BT_UART_MESSAGE_H_
#define BT_UART_MESSAGE_H_

#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#include "CommInterface.h"
#define RD 0x00
#define WR 0x01
#define OK 0x1F
/*********Data structure defination **********************/
/* message struct defination */
typedef struct
{
	unsigned int byte1 :8;
	unsigned int byte2 :8;
	unsigned int byte3 :8;
	unsigned int byte4 :8;
	unsigned int byte5 :8;
	unsigned int byte6 :8;
	unsigned int byte7 :8;
	unsigned int byte8 :8;
	unsigned int byte9 :8;
	unsigned int byte10 :8;
	unsigned int byte11 :8;
	unsigned int byte12 :8;
	unsigned int byte13 :8;
	unsigned int byte14 :8;
	unsigned int byte15 :8;
	unsigned int byte16 :8;
	unsigned int byte17 :8;
	unsigned int byte18 :8;
	unsigned int byte19 :8;
	unsigned int byte20 :8;
	unsigned int byte21 :8;
	unsigned int byte22 :8;
	unsigned int byte23 :8;
	unsigned int byte24 :8;
	unsigned int byte25 :8;
	unsigned int byte26 :8;
	unsigned int byte27 :8;
	unsigned int byte28 :8;
	unsigned int byte29 :8;
	unsigned int byte30 :8;
	unsigned int byte31 :8;
	unsigned int byte32 :8;
} message_bytes_st;

typedef struct
{
	uint8_t screen_num :8;
	uint8_t cmd :8;
	uint16_t data_buffer[14];
	uint16_t crc;
} Message_st;

union message_un
{
	message_bytes_st message_bytes;
	Message_st message_layer;
	uint16_t int_array[16];
	uint32_t long_array[8];
};
#define RAM_FUNC 0
#if RAM_FUNC
#pragma CODE_SECTION(calcCRC,"ramfuncs");
uint16_t calcCRC(uint8_t *data_ptr,uint8_t size);
#pragma CODE_SECTION(byte_proc,"ramfuncs");
static void byte_proc(uint8_t msg_byte);
#pragma CODE_SECTION(message_time_loop,"ramfuncs");
uint8_t message_time_loop(void);
#pragma CODE_SECTION(packet_constructor,"ramfuncs");
void packet_constructor(uint8_t *data, Message_st *mesg);
uint8_t send_packet(uint8_t *data_ptr,uint8_t data_size);
#else
void sendData(Message_st	*mesg);
void processRxByte(uint8_t msg_byte);
uint8_t messageLoop(void);
#endif
//helper functions
union message_un* getMessagePrt(void);
#endif /* BT_UART_MESSAGE_H_ */
