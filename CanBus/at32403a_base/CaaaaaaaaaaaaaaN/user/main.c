/**
 **************************************************************************
 * @file     main.c
 * @brief    main program
 **************************************************************************
 *                       Copyright notice & Disclaimer
 *
 * The software Board Support Package (BSP) that is made available to
 * download from Artery official website is the copyrighted work of Artery.
 * Artery authorizes customers to use, copy, and distribute the BSP
 * software and its related documentation for the purpose of design and
 * development in conjunction with Artery microcontrollers. Use of the
 * software is governed by this copyright notice and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
 * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
 * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
 * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
 * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
 *
 **************************************************************************
 */

#include "at32f403a_407_board.h"
#include "at32f403a_407_clock.h"
#include "at32f403a_407_int.h"
/*Module includes*/
#include "nvicInit.h"
#include "pwmInit.h"
#include "timerInit.h"
#include "adcInit.h"
#include "canInit.h"
#include "CommInterface.h"
#include "CompileTime.h"

/** @addtogroup 403A_LED_toggle LED_toggle
 * @{
 */

#define DELAY                            100
#define FAST                             1
#define SLOW                             4

extern int sdeviceSettings;

const  uint16_t dataBudd[10240] __attribute__((section(".deviceSettings")))={1,1,2};

/**
 * @brief  timer interrupt handler
 * @param  none
 * @retval none
 */
static void timerTickCall(void)
{
	static uint32_t isrCnt = 0;
	if(!(isrCnt++%1000)) at32_led_toggle(LED3);
}


float actualVref = 0;
float actualTempr = 0;
/**
 * @brief  adc interrupt handler
 * @param  none
 * @retval none
 */
static void adcCall(void)
{

	at32_led_off(LED4);
	/* calc something */
	actualVref = (ADC_VINT_REF * 4096)
			/ adc1_ordinary_valuetab[UINT_REF_CH];
	actualTempr = (ADC_TEMP_BASE
			- adc1_ordinary_valuetab[TEMPR_INT_CH] * ADC_VREF / 4096)
			/ ADC_TEMP_SLOPE + 25;
	at32_led_on(LED4);
}

/**
 * @brief  canRx interrupt handler
 * @param  none
 * @retval none
 */
volatile uint32_t rxId;
static void canRxCall(can_rx_message_type *rx_message_struct)
{
	rxId = rx_message_struct->standard_id;

}

#define PWM_FREQUENCY_HZ				16000
#define TIMER_FREQUENCY_HZ				1000
/**
 * @brief  main function.
 * @param  none
 * @retval none
 */

int main(void)
{

	compileTimeInit();
	system_clock_config();
	/* get actual clocks*/
	crm_clocks_freq_type clocks_freq;
	crm_clocks_freq_get(&clocks_freq);
	at32_board_init();// Инициализация(в данном случае лампочек и их состояний)
	                  //здесь будем инициализировать порты под задачи
	/*Disable global IRQ*/
	__disable_irq();

	init3phPWM(PWM_FREQUENCY_HZ, TRUE);

	timerInit(TIMER_FREQUENCY_HZ, TRUE);
	setTimerCallBack(timerTickCall);

	adcInit(TRUE);
	setAdcCallBack(adcCall);

	initCommInterface();

	canInit(CAN_BAUD_1000k, TRUE);//param: скорость передачи данных   bool: включать или не включать обработку прерывания
	//В нашем случае обработка входящих сообщений
	//setCanRxCallBack(canRxCall); //Инициализация обратной функции
	pwmCounterEnable();

	adc_ordinary_software_trigger_enable(ADC1, TRUE);
	nvicInit();
	__enable_irq();

	  uint8_t data = 1; // массив данных
	  sendToCan(0x126, 1, data);

while (1)
{

	//uint8_t data[1] = {'1'}; // массив данных
		//sendToCan(0x123, 1, data); // отправка сообщения с ID = 0x123 и данными из массива data
		//at32_led_toggle(LED2);
		//delay_sec(10);
}

}
