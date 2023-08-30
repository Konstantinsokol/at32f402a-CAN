/*
 * pwmInit.h
 *
 *  Created on: 2023 Mar 30
 *      Author: on4ip
 */

#ifndef USER_PWMINIT_H_
#define USER_PWMINIT_H_
#include "at32f403a_407_conf.h"
#include "at32f403a_407_int.h"
#include "stdint.h"
#include "stdbool.h"
/*!
 * @brief function initialize timer to 3ph mode with dt
 * @details function calculate base timer CNT value due refFreq, enable timer ISR
 * 			and set proper value for dead time
 * @param freq - Reference PWM frequence
 * @param isrEna- Generate isr or not
 */
void init3phPWM(uint16_t freq,bool isrEna);
void pwmDisable(void);
void pwmEnable(void);
void pwmRefVal(uint16_t ch1,uint16_t ch2,uint16_t ch3);
void pwmCounterEnable(void);
extern uint32_t pwmIsrCnt;

#endif /* USER_PWMINIT_H_ */
