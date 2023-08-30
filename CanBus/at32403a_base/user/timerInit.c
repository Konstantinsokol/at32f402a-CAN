/*
 * timerInit.c
 *
 *  Created on: 2023 Apr 4
 *      Author: on4ip
 */

#include "timerInit.h"

static void (*plocalCallBack)(void) = NULL;
void setTimerCallBack(void *pCallBack)
{
	plocalCallBack = pCallBack;
}
void timerInit(uint16_t freq, bool isrEna)
{
	crm_clocks_freq_type crm_clocks_freq_struct =
	{ 0 };
	/* enable tmr1 clock */
	crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, TRUE);
	/* get system clock */
	crm_clocks_freq_get(&crm_clocks_freq_struct);
	/* tmr1 configuration */
	/* time base configuration */
	/* systemclock/24000/10000 = 1hz */
	tmr_32_bit_function_enable(TMR2,TRUE);
	tmr_base_init(TMR2,(crm_clocks_freq_struct.ahb_freq / freq) - 1,0);
	tmr_cnt_dir_set(TMR2, TMR_COUNT_UP);

	if (isrEna)
	{
		/* overflow interrupt enable */
		tmr_interrupt_enable(TMR2, TMR_OVF_INT, TRUE);
	}
	/* enable tmr2 */
	tmr_counter_enable(TMR2, TRUE);
}

void TMR2_GLOBAL_IRQHandler(void)
{
	if (tmr_flag_get(TMR2, TMR_OVF_FLAG) != RESET)
	{
		/* add user code... */
		if(plocalCallBack!=NULL) plocalCallBack();
		tmr_flag_clear(TMR2, TMR_OVF_FLAG);
	}
}
