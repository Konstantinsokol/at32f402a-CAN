/*
 * canInit.h
 *
 *  Created on: 2023 Apr 12
 *      Author: on4ip
 */

#ifndef USER_CANINIT_H_
#define USER_CANINIT_H_
#include "at32f403a_407_conf.h"
#include "at32f403a_407_int.h"
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

enum CAN_BUAR_RATES
{
	CAN_BAUD_125k=0,
	CAN_BAUD_250k,
	CAN_BAUD_500k,
	CAN_BAUD_1000k

};
void canInit(uint16_t baud,bool isrEna);

void setCanRxCallBack(void *);

uint8_t sendToCan(uint32_t ID,uint8_t DLC,uint8_t* pData);

void sendToCanFloat(uint32_t ID,uint8_t DLC, float curr);

char* decodeMessage(can_rx_message_type rx_message_struct);


#endif /* USER_CANINIT_H_ */
