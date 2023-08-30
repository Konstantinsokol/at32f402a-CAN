/*
 * timerInit.h
 *
 *  Created on: 2023 Apr 4
 *      Author: on4ip
 */

#ifndef USER_TIMERINIT_H_
#define USER_TIMERINIT_H_

#include "at32f403a_407_conf.h"
#include "at32f403a_407_int.h"
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
void timerInit(uint16_t freq,bool isrEna);
void setTimerCallBack(void *);



#endif /* USER_TIMERINIT_H_ */
