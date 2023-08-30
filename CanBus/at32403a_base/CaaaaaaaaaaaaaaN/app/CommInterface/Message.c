/*
 * Message.c
 *
 *  Created on: 7 окт. 2022 г.
 *      Author: on4ip
 */

#include "Message.h"


uint8_t recived_data[32]; ///< Buffer for data packet
uint8_t full_message = 0; ///< flag indicate that valid message received
union message_un message;	///< message union definition
/*!
 * @brief function process received bytes and read data packet
 * @param msg_byte Unsigned int received from UART
 */
uint8_t byte_raw_cnt = 0;
void processRxByte(uint8_t msg_byte)
{
	static uint8_t byte_cnt = 0;//Received bytes counter
	static uint8_t sync = 0;
	if(!full_message){
				/* if msg_byte is start byte  go to 1st data array index */
				if(msg_byte == '#'/*in HEX 0x23 */)
				{
					if(!sync)
					{
						memset(recived_data,0,sizeof(recived_data));
						byte_cnt = 0;
						sync = 1; /*Received sync*/
						byte_raw_cnt = 0;
					}
					else  {
						if(byte_cnt <sizeof(recived_data)){
						recived_data[byte_cnt++] = msg_byte;
						}else sync = 0;
					}
				}else {
					if(sync)/*if sync */
					{
						if((msg_byte == '*')/* In HEX 0x2a */&& (byte_raw_cnt >32))
						{
							full_message = 1; /* flag indicate that full massage received and wait processing */
							byte_cnt = 0;
							sync = 0;
						}else
							{	if(byte_cnt <sizeof(recived_data)){
								recived_data[byte_cnt++] = msg_byte;
								}else sync = 0;
							}
					}else byte_cnt = 0;
				}
				byte_raw_cnt++;
			}
}

/*!
* @brief Function convert raw buffer to struct data
* @param data_ptr pointer to uint9_t recived_data
*/
inline void message_from_raw(uint8_t *raw_data, message_bytes_st *message_ptr)
{

	message_ptr->byte1 = raw_data[0];
	message_ptr->byte2 = raw_data[1];
	message_ptr->byte3 = raw_data[2];
	message_ptr->byte4 = raw_data[3];
	message_ptr->byte5 = raw_data[4];
	message_ptr->byte6 = raw_data[5];
	message_ptr->byte7 = raw_data[6];
	message_ptr->byte8 = raw_data[7];
	/*--------------------------------------*/
	message_ptr->byte9 = raw_data[8];
	message_ptr->byte10 = raw_data[9];
	message_ptr->byte11 = raw_data[10];
	message_ptr->byte12 = raw_data[11];
	message_ptr->byte13 = raw_data[12];
	message_ptr->byte14 = raw_data[13];
	message_ptr->byte15 = raw_data[14];
	message_ptr->byte16 = raw_data[15];
	/*----------------------------------------*/
	message_ptr->byte17 = raw_data[16];
	message_ptr->byte18 = raw_data[17];
	message_ptr->byte19 = raw_data[18];
	message_ptr->byte20 = raw_data[19];
	message_ptr->byte21 = raw_data[20];
	message_ptr->byte22 = raw_data[21];
	message_ptr->byte23 = raw_data[22];
	message_ptr->byte24 = raw_data[23];
	/*----------------------------------------*/
	message_ptr->byte25 = raw_data[24];
	message_ptr->byte26 = raw_data[25];
	message_ptr->byte27 = raw_data[26];
	message_ptr->byte28 = raw_data[27];
	message_ptr->byte29 = raw_data[28];
	message_ptr->byte30 = raw_data[29];
	message_ptr->byte31 = raw_data[30];
	message_ptr->byte32 = raw_data[31];
}
/*!
* @brief Function convert struct data to raw buffer
* @param data_ptr pointer to message_byte_st
*/
inline void raw_from_message(message_bytes_st *data_ptr,uint8_t *raw_data)
{
	raw_data[0] = data_ptr->byte1;
	raw_data[1] = data_ptr->byte2;
	raw_data[2] = data_ptr->byte3;
	raw_data[3] = data_ptr->byte4;
	raw_data[4] = data_ptr->byte5;
	raw_data[5] = data_ptr->byte6;
	raw_data[6] = data_ptr->byte7;
	raw_data[7] = data_ptr->byte8;
	/*---------------------------------------*/
	raw_data[8] = data_ptr->byte9;
	raw_data[9] = data_ptr->byte10;
	raw_data[10] = data_ptr->byte11;
	raw_data[11] = data_ptr->byte12;
	raw_data[12] = data_ptr->byte13;
	raw_data[13] = data_ptr->byte14;
	raw_data[14] = data_ptr->byte15;
	raw_data[15] = data_ptr->byte16;
	/*---------------------------------------*/
	raw_data[16] = data_ptr->byte17;
	raw_data[17] = data_ptr->byte18;
	raw_data[18] = data_ptr->byte19;
	raw_data[19] = data_ptr->byte20;
	raw_data[20] = data_ptr->byte21;
	raw_data[21] = data_ptr->byte22;
	raw_data[22] = data_ptr->byte23;
	raw_data[23] = data_ptr->byte24;
	/*---------------------------------------*/
	raw_data[24] = data_ptr->byte25;
	raw_data[25] = data_ptr->byte26;
	raw_data[26] = data_ptr->byte27;
	raw_data[27] = data_ptr->byte28;
	raw_data[28] = data_ptr->byte29;
	raw_data[29] = data_ptr->byte30;
	raw_data[30] = data_ptr->byte31;
	raw_data[31] = data_ptr->byte32;
}
/*!
 *  @brief Function calculate CRC 16 for packet
 */
inline  uint16_t calcCRC(uint8_t *data_ptr,uint8_t size)
{
	uint16_t crc = 0;
	uint16_t i = 0;
	/* Calculate CRC like summ of all bytes with data, 30 bytes */
	for(i = 0; i < size; i++)
	{ crc = crc + data_ptr[i]; }
	//uint16_t CRC1 = crc/0x10; //разбиение на старшую и младшую тетрады
	//uint16_t CRC2 = crc-CRC1*0x10;
	//printf("calculated CRC id %i",crc);
	return (crc);

}
inline void packet_constructor(uint8_t *data, Message_st	*mesg)
{
	//union message_un message;
	message.message_layer = *mesg;
	/* make answer to PC */
	data[0] = 0x23;				//start data
	uint8_t packet_raw[32];
	raw_from_message(&message.message_bytes,packet_raw);
	/* calculate CRC for packet */
	uint16_t chck_summ = 0;
	chck_summ = calcCRC(packet_raw, sizeof(packet_raw) - 2);
	/* apply CRC to packet 30 byte - LSB 31 byte- MSB */
	packet_raw[30] = (uint8_t)(chck_summ & 0xFF);
	packet_raw[31] = (uint8_t)((chck_summ >> 8) & 0xFF);
	for (uint8_t i = 0; i < 32; i++)
	{
		data[1 + i] = packet_raw[i];
	}
	data[33] = 0x2A;
}
/*!
 * @brief Loop for UART messages processing
 * @details Loop will use full_message flag and recived_data buffer. Buffer contain message from host
 */
uint16_t OK_save = 0; /*Переменная для ручной записи при отладке через jtag*/
uint8_t messageLoop(void)
{
	/* If full_message flag set do message process */
	if(full_message)
	{
		/* find checksumm of data packet */
		uint16_t chck_summ = 0;
		chck_summ = recived_data[31]*256+recived_data[30];
		/*	calculate packet check summ and compare with internal */
		if(chck_summ==calcCRC(recived_data,sizeof(recived_data)-2))
		{
			/* valid  RAW packet  copy data to message union*/
			message_from_raw(recived_data,&message.message_bytes);
			/* clear full_message flag */
			full_message = 0;
			return 1; //Message ready to read

		}else{
			/* discard packet */
		}
		/* clear full_message flag */
		full_message = 0;
	}
	static unsigned int first_loop = 0;
	if(!first_loop)
	{
		/* Clear data stuct */
		memset(message.int_array,0,sizeof(message.int_array));
		first_loop = 1;
	}
	return 0;
}

uint8_t tx_buf[34]; /// TX buffer
void sendData(Message_st	*mesg)
{
	packet_constructor(tx_buf,mesg);

	sendPacket(tx_buf,sizeof(tx_buf));
}
union message_un* getMessagePrt(void)
{
	return (&message);
}


