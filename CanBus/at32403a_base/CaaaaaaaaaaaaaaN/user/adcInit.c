/*
 * adcInit.c
 *
 *  Created on: 2023 Mar 31
 *      Author: on4ip
 */
#include "adcInit.h"
#include "at32f403a_407_board.h"

static void (*plocalCallBack)(void) = NULL;
void setAdcCallBack(void *pCallBack)
{
	plocalCallBack = pCallBack;
}

typedef struct
{
	uint8_t chNum;
	uint8_t chSequence;
	uint8_t chAdcSample;
} chSettings_st;
chSettings_st chPreemptSettings[MAX_PREEMPT_CH];
chSettings_st chOrdinaySettings[MAX_ORDINARY_CH];
static void setChannelsSettings(void)
{
	chPreemptSettings[IA_CH].chNum = ADC_CHANNEL_10;
	chPreemptSettings[IA_CH].chSequence = IA_CH + 1;
	chPreemptSettings[IA_CH].chAdcSample = ADC_SAMPLETIME_13_5;

	chPreemptSettings[IB_CH].chNum = ADC_CHANNEL_11;
	chPreemptSettings[IB_CH].chSequence = IB_CH + 1;
	chPreemptSettings[IB_CH].chAdcSample = ADC_SAMPLETIME_13_5;

	chPreemptSettings[IC_CH].chNum = ADC_CHANNEL_0;
	chPreemptSettings[IC_CH].chSequence = IC_CH + 1;
	chPreemptSettings[IC_CH].chAdcSample = ADC_SAMPLETIME_13_5;

	chPreemptSettings[UDC_CH].chNum = ADC_CHANNEL_4;
	chPreemptSettings[UDC_CH].chSequence = UDC_CH + 1;
	chPreemptSettings[UDC_CH].chAdcSample = ADC_SAMPLETIME_13_5;

	/* set ordinary channels 	 */
	chOrdinaySettings[BRAKE_CH].chNum = ADC_CHANNEL_14;
	chOrdinaySettings[BRAKE_CH].chSequence = BRAKE_CH + 1;
	chOrdinaySettings[BRAKE_CH].chAdcSample = ADC_SAMPLETIME_13_5;

	chOrdinaySettings[THROTTLE_CH].chNum = ADC_CHANNEL_15;
	chOrdinaySettings[THROTTLE_CH].chSequence = THROTTLE_CH + 1;
	chOrdinaySettings[THROTTLE_CH].chAdcSample = ADC_SAMPLETIME_13_5;

	chOrdinaySettings[MOTOR_TEMPR_CH].chNum = ADC_CHANNEL_2;
	chOrdinaySettings[MOTOR_TEMPR_CH].chSequence = MOTOR_TEMPR_CH + 1;
	chOrdinaySettings[MOTOR_TEMPR_CH].chAdcSample = ADC_SAMPLETIME_13_5;

	chOrdinaySettings[HEATSINK_CH].chNum = ADC_CHANNEL_6;
	chOrdinaySettings[HEATSINK_CH].chSequence = HEATSINK_CH + 1;
	chOrdinaySettings[HEATSINK_CH].chAdcSample = ADC_SAMPLETIME_13_5;

	chOrdinaySettings[BACK_SINK_CH].chNum = ADC_CHANNEL_5;
	chOrdinaySettings[BACK_SINK_CH].chSequence = BACK_SINK_CH + 1;
	chOrdinaySettings[BACK_SINK_CH].chAdcSample = ADC_SAMPLETIME_13_5;

	chOrdinaySettings[UINT_ERR_CH].chNum = ADC_CHANNEL_9;
	chOrdinaySettings[UINT_ERR_CH].chSequence = UINT_ERR_CH + 1;
	chOrdinaySettings[UINT_ERR_CH].chAdcSample = ADC_SAMPLETIME_13_5;

	chOrdinaySettings[UINT_REF_CH].chNum = ADC_CHANNEL_17;
	chOrdinaySettings[UINT_REF_CH].chSequence = UINT_REF_CH + 1;
	chOrdinaySettings[UINT_REF_CH].chAdcSample = ADC_SAMPLETIME_13_5;

	chOrdinaySettings[TEMPR_INT_CH].chNum = ADC_CHANNEL_16;
	chOrdinaySettings[TEMPR_INT_CH].chSequence = TEMPR_INT_CH + 1;
	chOrdinaySettings[TEMPR_INT_CH].chAdcSample = ADC_SAMPLETIME_13_5;
}
/**
 * @brief  gpio configuration.
 * @param  none
 * @retval none
 */
static void gpio_config(void)
{
	gpio_init_type gpio_initstructure;
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

	gpio_default_para_init(&gpio_initstructure);
	gpio_initstructure.gpio_mode = GPIO_MODE_ANALOG;
	gpio_initstructure.gpio_pins = GPIO_PINS_0 | GPIO_PINS_2 | GPIO_PINS_4
			| GPIO_PINS_5 | GPIO_PINS_6;
	gpio_init(GPIOA, &gpio_initstructure);

	gpio_initstructure.gpio_mode = GPIO_MODE_ANALOG;
	gpio_initstructure.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_4
			| GPIO_PINS_5;
	gpio_init(GPIOC, &gpio_initstructure);
}

__IO uint32_t adc1_ordinary_valuetab[MAX_ORDINARY_CH];
/**
 * @brief  dma configuration.
 * @param  none
 * @retval none
 */
static void dma_config(void)
{
	dma_init_type dma_init_struct;
	crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
	nvic_irq_enable(DMA1_Channel1_IRQn, 0, 0);
	dma_reset(DMA1_CHANNEL1);
	dma_default_para_init(&dma_init_struct);
	dma_init_struct.buffer_size = MAX_ORDINARY_CH;
	dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
	dma_init_struct.memory_base_addr = (uint32_t) adc1_ordinary_valuetab;
	dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_WORD;
	dma_init_struct.memory_inc_enable = TRUE;
	dma_init_struct.peripheral_base_addr = (uint32_t) &(ADC1->odt);
	dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_WORD;
	dma_init_struct.peripheral_inc_enable = FALSE;
	dma_init_struct.priority = DMA_PRIORITY_HIGH;
	dma_init_struct.loop_mode_enable = TRUE;
	dma_init(DMA1_CHANNEL1, &dma_init_struct);

	dma_interrupt_enable(DMA1_CHANNEL1, DMA_FDT_INT, FALSE);
	dma_channel_enable(DMA1_CHANNEL1, TRUE);
}
void adcInit(bool isrEna)
{
	gpio_config();
	dma_config();
	setChannelsSettings();
	adc_base_config_type adc_base_struct;
	crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_ADC2_PERIPH_CLOCK, TRUE);
	crm_adc_clock_div_set(CRM_ADC_DIV_4);

	/* select combine mode */
	adc_combine_mode_select(ADC_INDEPENDENT_MODE);
	adc_base_default_para_init(&adc_base_struct);
	adc_base_struct.sequence_mode = TRUE;
	adc_base_struct.repeat_mode = TRUE;
	adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
	adc_base_struct.ordinary_channel_length = MAX_ORDINARY_CH;
	adc_base_config(ADC1, &adc_base_struct);
	uint8_t i = 0;
	for (i = 0; i < MAX_ORDINARY_CH; i++)
	{
		adc_ordinary_channel_set(ADC1, chOrdinaySettings[i].chNum,
				chOrdinaySettings[i].chSequence,
				chOrdinaySettings[i].chAdcSample);
	}

	/* set converter start trigger */
	adc_ordinary_conversion_trigger_set(ADC1, ADC12_ORDINARY_TRIG_SOFTWARE,
			TRUE);
	/* enable internal temperature sensor and internal VrefMonitor*/
	adc_tempersensor_vintrv_enable(TRUE);

	adc_dma_mode_enable(ADC1, TRUE);

	/* config adc 2 */
	adc_base_default_para_init(&adc_base_struct);
	adc_base_struct.sequence_mode = TRUE;
	adc_base_struct.repeat_mode = TRUE;
	adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
	adc_base_config(ADC2, &adc_base_struct);

	adc_preempt_channel_length_set(ADC2, MAX_PREEMPT_CH);
	adc_preempt_channel_set(ADC2, chPreemptSettings[IA_CH].chNum,
			chPreemptSettings[IA_CH].chSequence,
			chPreemptSettings[IA_CH].chAdcSample);
	adc_preempt_channel_set(ADC2, chPreemptSettings[IB_CH].chNum,
			chPreemptSettings[IB_CH].chSequence,
			chPreemptSettings[IB_CH].chAdcSample);
	adc_preempt_channel_set(ADC2, chPreemptSettings[IC_CH].chNum,
			chPreemptSettings[IC_CH].chSequence,
			chPreemptSettings[IC_CH].chAdcSample);
	adc_preempt_channel_set(ADC2, chPreemptSettings[UDC_CH].chNum,
			chPreemptSettings[UDC_CH].chSequence,
			chPreemptSettings[UDC_CH].chAdcSample);
	adc_preempt_conversion_trigger_set(ADC2, ADC12_PREEMPT_TRIG_TMR1TRGOUT,
			TRUE);
	adc_preempt_auto_mode_enable(ADC2, FALSE);
	adc_interrupt_enable(ADC2, ADC_PCCE_INT, FALSE);

	adc_enable(ADC1, TRUE);
	adc_enable(ADC2, TRUE);
	adc_calibration_init(ADC1);
	while (adc_calibration_init_status_get(ADC1))
		;
	adc_calibration_start(ADC1);
	while (adc_calibration_status_get(ADC1))
		;
	adc_calibration_init(ADC2);
	while (adc_calibration_init_status_get(ADC2))
		;
	adc_calibration_start(ADC2);
	while (adc_calibration_status_get(ADC2))
		;

	if (isrEna)
	{
		adc_interrupt_enable(ADC2, ADC_PCCE_INT, TRUE);
	}
}
__IO uint32_t adc2_preempt_value[MAX_PREEMPT_CH];

void ADC1_2_IRQHandler(void)
{
	if (adc_flag_get(ADC2, ADC_PCCE_FLAG) != RESET)
	{

		adc2_preempt_value[0] = adc_preempt_conversion_data_get(
		ADC2, ADC_PREEMPT_CHANNEL_1);
		adc2_preempt_value[1] = adc_preempt_conversion_data_get(
		ADC2, ADC_PREEMPT_CHANNEL_2);
		adc2_preempt_value[2] = adc_preempt_conversion_data_get(
		ADC2, ADC_PREEMPT_CHANNEL_3);
		adc2_preempt_value[3] = adc_preempt_conversion_data_get(
		ADC2, ADC_PREEMPT_CHANNEL_4);


		if (plocalCallBack != NULL)	plocalCallBack();

		adc_flag_clear(ADC2, ADC_PCCE_FLAG);
	}
}
