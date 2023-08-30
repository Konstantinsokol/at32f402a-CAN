/*
 * nvicInit.c
 *
 *  Created on: 2023 Apr 4
 *      Author: on4ip
 */

#include "nvicInit.h"

void nvicInit(void)
{
	/* tmr1 interrupt nvic init */
	nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
	/* adc1 interrupt nvic init */
	nvic_irq_enable(ADC1_2_IRQn, 0, 0);

	nvic_irq_enable(TMR1_OVF_TMR10_IRQn, 1, 0);

	nvic_irq_enable(TMR1_CH_IRQn, 2, 0);
	/* tmr1 overflow interrupt nvic init */
	nvic_irq_enable(TMR2_GLOBAL_IRQn, 3, 0);
	/*usart nvic interrupt */
#if USART_NUM==1
	  nvic_irq_enable(USART1_IRQn, 10, 0);
#endif
#if USART_NUM==2
	nvic_irq_enable(USART2_IRQn, 10, 0);
#endif
	/* can interrupt config */
	nvic_irq_enable(CAN1_SE_IRQn, 4, 0);
	nvic_irq_enable(USBFS_L_CAN1_RX0_IRQn, 5, 0);
}

