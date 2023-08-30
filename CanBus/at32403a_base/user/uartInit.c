/*
 * uartInit.c
 *
 *  Created on: 2023 Apr 1
 *      Author: on4ip
 */

#include "uartInit.h"



volatile uint8_t dataBuffer = 0;

volatile uint32_t *setAddr=NULL;

static void (*pReciveCallBack)(uint8_t) = NULL;

void setUartRxCallBack(void *pCallBack) {
	pReciveCallBack = pCallBack;
}

static uint8_t uartInitFlg = 0;

static void gpioInit(void) {
      gpio_init_type gpio_init_struct;
#if USART_NUM==1
	/* enable usart clock*/
	crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

	gpio_default_para_init(&gpio_init_struct);

	/* configure the usart1 tx pin */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
	gpio_init_struct.gpio_pins = GPIO_PINS_9;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOA, &gpio_init_struct);

	/* configure the usart2 rx pin */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_10;
	gpio_init_struct.gpio_pull = GPIO_PULL_UP;
	gpio_init(GPIOA, &gpio_init_struct);
#define ACTIVE_USART USART1
#elif USART_NUM==2
	/* enable usart clock*/
	crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

	gpio_default_para_init(&gpio_init_struct);

	/* configure the usart1 tx pin */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
	gpio_init_struct.gpio_pins = GPIO_PINS_2;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOA, &gpio_init_struct);

	/* configure the usart2 rx pin */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_3;
	gpio_init_struct.gpio_pull = GPIO_PULL_UP;
	gpio_init(GPIOA, &gpio_init_struct);
#define ACTIVE_USART USART2
#else
#error "USART port number not defined"
#endif
}
static uint32_t baudRates[MAX_UART_BAUDS] = { 115200 };

void uartInit(enum UART_BAUDS baud, uint8_t isrEna) {
	gpioInit();
	/* configure usart2 param */
	usart_init(ACTIVE_USART, baudRates[baud], USART_DATA_8BITS,
			USART_STOP_1_BIT);
	usart_parity_selection_config(ACTIVE_USART, USART_PARITY_NONE);

	usart_transmitter_enable(ACTIVE_USART, TRUE);
	usart_receiver_enable(ACTIVE_USART, TRUE);

	if (isrEna) {
		/* enable usart1 interrupt */
		usart_interrupt_enable(ACTIVE_USART, USART_RDBF_INT, TRUE);
		usart_interrupt_enable(ACTIVE_USART, USART_TDBE_INT, FALSE);
	}
	usart_enable(ACTIVE_USART, TRUE);
	uartInitFlg = 1;
}

void sendToUart(uint8_t *data, uint16_t len) {
	uint16_t dataCnt = 0;
	if (uartInitFlg) {
		for (; dataCnt < len; dataCnt++) {

			while (usart_flag_get(ACTIVE_USART, USART_TDBE_FLAG) == RESET)
				;

			usart_data_transmit(ACTIVE_USART, data[dataCnt]);

		}
	}
}

#if USART_NUM==1
/**
  * @brief  this function handles usart2 handler.
  * @param  none
  * @retval none
  */

void USART1_IRQHandler(void)
{
  if(ACTIVE_USART->ctrl1_bit.rdbfien != RESET)
  {
    if(usart_flag_get(ACTIVE_USART, USART_RDBF_FLAG) != RESET)
    {
      /* read one byte from the receive data register */
      usart_data_receive(ACTIVE_USART);
    }
  }

  if(ACTIVE_USART->ctrl1_bit.tdbeien != RESET)
  {
    if(usart_flag_get(ACTIVE_USART, USART_TDBE_FLAG) != RESET)
    {
      /* write one byte to the transmit data register */
      //usart_data_transmit(USART2, usart2_tx_buffer[usart2_tx_counter++]);
    }
  }
}
#endif
#if USART_NUM==2
/**
 * @brief  this function handles usart2 handler.
 * @param  none
 * @retval none
 */
void USART2_IRQHandler(void) {
	if (USART2->ctrl1_bit.rdbfien != RESET) {
		if (usart_flag_get(USART2, USART_RDBF_FLAG) != RESET) {
			/* read one byte from the receive data register */
			uint8_t byte = (uint8_t) usart_data_receive(ACTIVE_USART);
			if (pReciveCallBack != NULL) {
				pReciveCallBack(byte);
			}
		}
	}

	if (USART2->ctrl1_bit.tdbeien != RESET) {
		if (usart_flag_get(USART2, USART_TDBE_FLAG) != RESET) {

		}
	}
}
#endif
