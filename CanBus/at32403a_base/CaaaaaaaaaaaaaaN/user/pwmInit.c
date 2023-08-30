/*
 * pwmInit.c
 *
 *  Created on: 2023 Mar 30
 *      Author: on4ip
 */
#include "pwmInit.h"
#include "at32f403a_407_board.h"
inline void pwmDisable(void) {
	tmr_output_enable(TMR1, FALSE);
}
inline void pwmEnable(void) {
	tmr_output_enable(TMR1, TRUE);
}
inline void pwmCounterEnable(void) {
	/* enable tmr1 */
	tmr_counter_enable(TMR1, TRUE);
}

inline void pwmRefVal(uint16_t ch1, uint16_t ch2, uint16_t ch3) {
	tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, ch1);
	tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_2, ch2);
	tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_3, ch3);
}
void init3phPWM(uint16_t freq, bool isrEna) {

	gpio_init_type gpio_init_struct = { 0 };
	/* enable tmr1/gpioa/gpiob clock */
	crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

	/* timer1 output pin configuration */
	gpio_init_struct.gpio_pins = GPIO_PINS_8 | GPIO_PINS_9 | GPIO_PINS_10;
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init(GPIOA, &gpio_init_struct);

	gpio_init_struct.gpio_pins = GPIO_PINS_13 | GPIO_PINS_14 | GPIO_PINS_15;
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init(GPIOB, &gpio_init_struct);

	/* timer ch4 out put */
	gpio_init_struct.gpio_pins = GPIO_PINS_11;
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init(GPIOA, &gpio_init_struct);
	/* brake intpur */
	gpio_init_struct.gpio_pins = GPIO_PINS_12;
	gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init(GPIOB, &gpio_init_struct);

	crm_clocks_freq_type crm_clocks_freq_struct = { 0 };
	//read actual clock of MCU
	crm_clocks_freq_get(&crm_clocks_freq_struct);
	//enable TIM base clock
	crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);

	//set max count value to overflow use /2 because UP_DOWN count mode
	//set divide for sysClk to ZERO (real div 1)

	uint32_t arrValue = (crm_clocks_freq_struct.sclk_freq / freq) >> 1;
	tmr_base_init(TMR1, arrValue - 1, 0);
	tmr_cnt_dir_set(TMR1, TMR_COUNT_TWO_WAY_1);
	tmr_clock_source_div_set(TMR1, TMR_CLOCK_DIV1);

	uint32_t actualArrValue = tmr_period_value_get(TMR1);
	/* compute ccr1 value to generate a duty cycle at 50% for channel 1 and 1n */
	uint16_t channel1_pulse = (uint16_t) (((uint32_t) actualArrValue >> 1));

	/* compute ccr2 value to generate a duty cycle at 50%  for channel 2 and 2n */
	uint16_t channel2_pulse = (uint16_t) (((uint32_t) actualArrValue >> 1));

	/* compute ccr3 value to generate a duty cycle at 50%  for channel 3 and 3n */
	uint16_t channel3_pulse = (uint16_t) (((uint32_t) actualArrValue >> 1));

	tmr_output_config_type tmr_output_struct = { 0 };
	/* channel 1, 2, 3 and 4 Configuration in output mode */
	tmr_output_default_para_init(&tmr_output_struct);
	tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_B;
	tmr_output_struct.oc_output_state = TRUE;
	tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
	tmr_output_struct.oc_idle_state = FALSE;
	tmr_output_struct.occ_output_state = TRUE;
	tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
	tmr_output_struct.occ_idle_state = FALSE;

	/* channel 1 */
	tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
	tmr_output_channel_buffer_enable(TMR1, TMR_SELECT_CHANNEL_1, TRUE);
	/* channel 2 */
	tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_2, &tmr_output_struct);
	tmr_output_channel_buffer_enable(TMR1, TMR_SELECT_CHANNEL_2, TRUE);
	/* channel 3 */
	tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_3, &tmr_output_struct);
	tmr_output_channel_buffer_enable(TMR1, TMR_SELECT_CHANNEL_3, TRUE);

	/* channel 4 */
	tmr_output_default_para_init(&tmr_output_struct);
	tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_B;
	tmr_output_struct.oc_output_state = TRUE;
	tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
	tmr_output_struct.oc_idle_state = FALSE;
	tmr_output_struct.occ_output_state = FALSE;
	tmr_output_struct.occ_polarity = TMR_OUTPUT_CONTROL_OFF;
	tmr_output_struct.occ_idle_state = FALSE;

	tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_4, &tmr_output_struct);
	tmr_output_channel_buffer_enable(TMR1, TMR_SELECT_CHANNEL_4, TRUE);

	actualArrValue = tmr_period_value_get(TMR1);
	tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_4, actualArrValue-1);
	// set channel ref value
	pwmRefVal(channel1_pulse, channel2_pulse, channel3_pulse);

	tmr_primary_mode_select(TMR1, TMR_PRIMARY_SEL_C4ORAW);

	tmr_brkdt_config_type tmr_brkdt_config_struct = { 0 };
	/* automatic output enable, stop, dead time and lock configuration */
	tmr_brkdt_default_para_init(&tmr_brkdt_config_struct);
	tmr_brkdt_config_struct.brk_enable = TRUE;
	tmr_brkdt_config_struct.auto_output_enable = FALSE;  //No auto output ON
	tmr_brkdt_config_struct.deadtime = 0x60 | 0x7;
	tmr_brkdt_config_struct.fcsodis_state = TRUE;
	tmr_brkdt_config_struct.fcsoen_state = TRUE;
	tmr_brkdt_config_struct.brk_polarity = TMR_BRK_INPUT_ACTIVE_LOW;
	tmr_brkdt_config_struct.wp_level = TMR_WP_OFF;
	tmr_brkdt_config(TMR1, &tmr_brkdt_config_struct);

	//pwm disable
	pwmDisable();

	if (isrEna) {

		/* enable tmr1 debug mode  */
		debug_periph_mode_set(DEBUG_TMR1_PAUSE, TRUE);
		/* overflow interrupt enable */
		tmr_interrupt_enable(TMR1, TMR_OVF_INT, FALSE);
		/* ch4 interrupt enable */
		tmr_interrupt_enable(TMR1, TMR_C4_INT, TRUE);
		/* brk interrupt enable */
		tmr_interrupt_enable(TMR1, TMR_BRK_INT, TRUE);
	}
}
uint32_t pwmIsrCnt = 0;
/**
 * @brief  this function handles tmr1 overflow handler.
 * @param  none
 * @retval none
 */
void TMR1_OVF_TMR10_IRQHandler(void) {
	if (tmr_flag_get(TMR1, TMR_OVF_FLAG) != RESET) {
		/* add user code... */
		tmr_flag_clear(TMR1, TMR_OVF_FLAG);
	}
}
/**
 * @brief  this function handles tmr1 ch handler.
 * @param  none
 * @retval none
 */
void TMR1_CH_IRQHandler(void) {
	if (tmr_flag_get(TMR1, TMR_C4_FLAG) != RESET) {

		tmr_flag_clear(TMR1, TMR_C4_FLAG);

		pwmIsrCnt++;
	}
}
