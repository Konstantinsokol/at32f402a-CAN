/*
 * adcInit.h
 *
 *  Created on: 2023 Mar 31
 *      Author: on4ip
 */

#ifndef USER_ADCINIT_H_
#define USER_ADCINIT_H_
#include "at32f403a_407_conf.h"
#include "at32f403a_407_int.h"
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

void adcInit(bool isrEna);
void setAdcCallBack(void *pCallBack);

enum ADC_ORDINARY_CHANNELS
{
	BRAKE_CH=0,
	THROTTLE_CH,
	MOTOR_TEMPR_CH,
	HEATSINK_CH,
	BACK_SINK_CH,
	UINT_ERR_CH,
	UINT_REF_CH,
	TEMPR_INT_CH,

	MAX_ORDINARY_CH
};
extern __IO uint32_t adc1_ordinary_valuetab[MAX_ORDINARY_CH];


enum ADC_PREEMPT_CH
{
	IA_CH=0,
	IB_CH,
	IC_CH,
	UDC_CH,
	MAX_PREEMPT_CH
};
extern __IO uint32_t adc2_preempt_value[MAX_PREEMPT_CH];

#define ADC_VINT_REF						(1.2f)
#define ADC_VREF                         	(3.3f)
#define ADC_TEMP_BASE                    	(1.26f)
#define ADC_TEMP_SLOPE                   	(-0.00423f)


#endif /* USER_ADCINIT_H_ */
