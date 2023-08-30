/*
 * canInit.c
 *
 *  Created on: 2023 Apr 12
 *      Author: on4ip
 */
#include "canInit.h"
#include "string.h"
#include "at32f403a_407_board.h"
#include <stdio.h>
#include <stdlib.h>


static void (*plocalCallBack)(can_rx_message_type*) = NULL;

void setCanRxCallBack(void *pCallBack)
{
	plocalCallBack = pCallBack;
}
static void can_gpio_config(void)
{
	gpio_init_type gpio_init_struct;

	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
	gpio_pin_remap_config(CAN1_GMUX_0010, TRUE);

	gpio_default_para_init(&gpio_init_struct);

	/* can tx pin */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
	gpio_init_struct.gpio_pins = GPIO_PINS_9;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOB, &gpio_init_struct);

	/* can rx pin */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_8;
	gpio_init_struct.gpio_pull = GPIO_PULL_UP;
	gpio_init(GPIOB, &gpio_init_struct);
}
/**
 *  @brief  can configiguration.
 */
static void can_configuration(uint16_t baud)
{
	can_base_type can_base_struct;
	can_baudrate_type can_baudrate_struct;
	can_filter_init_type can_filter_init_struct;

	crm_periph_clock_enable(CRM_CAN1_PERIPH_CLOCK, TRUE);
	/* can base init */
	can_default_para_init(&can_base_struct);
	can_base_struct.mode_selection = CAN_MODE_COMMUNICATE;
	can_base_struct.ttc_enable = FALSE;
	can_base_struct.aebo_enable = TRUE;
	can_base_struct.aed_enable = TRUE;
	can_base_struct.prsf_enable = FALSE;
	can_base_struct.mdrsel_selection = CAN_DISCARDING_FIRST_RECEIVED;
	can_base_struct.mmssr_selection = CAN_SENDING_BY_ID;
	can_base_init(CAN1, &can_base_struct);

	/**
	 * @brief  set the baudrate of the can peripheral
	 * @param  can_x: select the can peripheral.
	 *         this parameter can be one of the following values:
	 *         CAN1,CAN2.
	 * @param  baudrate_div: baudrate division.
	 * @param  rsaw_size: resynchronization adjust width.
	 * @param  bts1_size: bit time segment 1.
	 * @param  bts2_size: bit time segment 2.
	 * @note   baudrate calculate method is:
	 *         baudrate = fpclk/(baudrate_div *(1 + bts1_size + bts2_size))
	 */
	switch (baud)
	{
	case CAN_BAUD_125k:
		can_baudrate_default_para_init(&can_baudrate_struct);
		can_baudrate_struct.baudrate_div = 112;
		can_baudrate_struct.rsaw_size = CAN_RSAW_1TQ;
		can_baudrate_struct.bts1_size = CAN_BTS1_6TQ;
		can_baudrate_struct.bts2_size = CAN_BTS2_1TQ;
		can_baudrate_set(CAN1, &can_baudrate_struct);
		break;
	case CAN_BAUD_250k:
		can_baudrate_default_para_init(&can_baudrate_struct);
		can_baudrate_struct.baudrate_div = 56;
		can_baudrate_struct.rsaw_size = CAN_RSAW_1TQ;
		can_baudrate_struct.bts1_size = CAN_BTS1_6TQ;
		can_baudrate_struct.bts2_size = CAN_BTS2_1TQ;
		can_baudrate_set(CAN1, &can_baudrate_struct);
		break;
	case CAN_BAUD_500k:
		can_baudrate_default_para_init(&can_baudrate_struct);
		can_baudrate_struct.baudrate_div = 28;
		can_baudrate_struct.rsaw_size = CAN_RSAW_1TQ;
		can_baudrate_struct.bts1_size = CAN_BTS1_6TQ;
		can_baudrate_struct.bts2_size = CAN_BTS2_1TQ;
		can_baudrate_set(CAN1, &can_baudrate_struct);
		break;
	case CAN_BAUD_1000k:
		can_baudrate_default_para_init(&can_baudrate_struct);
		can_baudrate_struct.baudrate_div = 14;
		can_baudrate_struct.rsaw_size = CAN_RSAW_1TQ;
		can_baudrate_struct.bts1_size = CAN_BTS1_6TQ;
		can_baudrate_struct.bts2_size = CAN_BTS2_1TQ;
		can_baudrate_set(CAN1, &can_baudrate_struct);
		break;
	default:

		break;

	} //end of baudrate selection

	/* can filter init */
	can_filter_init_struct.filter_activate_enable = TRUE;
	can_filter_init_struct.filter_mode = CAN_FILTER_MODE_ID_MASK;
	can_filter_init_struct.filter_fifo = CAN_FILTER_FIFO0;
	can_filter_init_struct.filter_number = 0;
	can_filter_init_struct.filter_bit = CAN_FILTER_32BIT;
	can_filter_init_struct.filter_id_high = 0x0000;
	can_filter_init_struct.filter_id_low = 0x0000;
	can_filter_init_struct.filter_mask_high = 0x0000;
	can_filter_init_struct.filter_mask_low = 0x0000;
	can_filter_init(CAN1, &can_filter_init_struct);

}
void canInit(uint16_t baud, bool isrEna)
{
	can_gpio_config();
	can_configuration(baud);
	if (isrEna)
	{
		//включение прерываний при необходимости
		can_interrupt_enable(CAN1, CAN_RF0MIEN_INT, TRUE);
		/* error interrupt enable */
		can_interrupt_enable(CAN1, CAN_ETRIEN_INT, TRUE);
		can_interrupt_enable(CAN1, CAN_EOIEN_INT, TRUE);
	}
}

/*
 *   @brief  can1 interrupt function rx0
 */


//ФУНКЦИЯ ПРЕОБРАЗОВАНИЯ HEX в CHAR
char* decodeMessage(can_rx_message_type rx_message_struct) {

		char* command = (char*)calloc(rx_message_struct.dlc, 1);
		for(int i = 0; i < rx_message_struct.dlc; i++)
			command[i] = (char)rx_message_struct.data[i];

         return command;
}


size_t i = 0;
uint16_t rxCnt = 0;
void USBFS_L_CAN1_RX0_IRQHandler(void) //ЭТО ОБРАБОТЧИК ПРЕРЫВАНИЙ
{
	if (can_flag_get(CAN1, CAN_RF0OF_FLAG) != RESET){
		at32_led_toggle(LED2);
	}

	can_rx_message_type rx_message_struct;//создаем структуру соообщения
	if (can_flag_get(CAN1, CAN_RF0MN_FLAG) != RESET)//проверяет пришло ли сообщение в FIFO 0
	{
		//do
		//{
			can_message_receive(CAN1, CAN_RX_FIFO0, &rx_message_struct);// читает


			//if (plocalCallBack != NULL)//если функция обратного вызова установлена
			//{ ЭТО РЕАЛИЗАЦИЯ ОБРАТНОЙ ФУНКЦИИ, НА ДАННЫЙ МОМЕНТ Я ЕЁ УБРАЛ
				//plocalCallBack(&rx_message_struct);//передаем в нее адрес значение сообщения, которое будет обработано функцией, которая лежит в plocalcallback

		    if (rx_message_struct.standard_id == 0x111){//если идентификатор сообщения 111, включаем лампочку

		    	//uint8_t command = decodeMessage(rx_message_struct);//пришедшее
		    	uint8_t command = rx_message_struct.data[0];
		        uint8_t com = 1;//с чем сравниваем
		        if(command == com)
		            {

		        	  float x = 3.44;
		              at32_led_toggle(LED5);//Здесь пишете свою задачу
		              uint8_t msg = 1;
		              delay_sec(5);
		              sendToCanFloat(0x126, 4, x); // отправка сообщения с ID = 0x126 и данными из массива data

		            }

		            else {

		            	uint8_t err = 0;
		  			    sendToCan(0x124, 1, err);

		             }
			}

		//} while (can_receive_message_pending_get(CAN1, CAN_RX_FIFO0));//условие того, что если не все данные считаны, то продолжить цикл
	}
}
	//ожидание данных
//}

/**
 *  @brief  can1 interrupt function se
 */
void CAN1_SE_IRQHandler(void)
{
	__IO uint32_t err_index = 0;
	if (can_flag_get(CAN1, CAN_ETR_FLAG) != RESET)
	{
		err_index = CAN1->ests & 0x70;
		can_flag_clear(CAN1, CAN_ETR_FLAG);
		/* error type is stuff error */
		if (err_index == 0x00000010)
		{
			/* when stuff error occur: in order to ensure communication normally,
			 user must restart can or send a frame of highest priority message here */
		}
	}
}

/**
 *  @brief  can1 transmit message public function
 *  @param  data to can ID DLC and pointer to data
 *  @retval 1 if message transfered to mail box or  0 if NOt
 */

void sendToCanFloat(uint32_t ID,uint8_t DLC, float curr)
{

	uint8_t pData[4];
	memcpy(pData, (uint8_t*)(&curr), sizeof(float));
	sendToCan(ID,DLC,pData);

}

volatile uint8_t error=0;
uint8_t sendToCan(uint32_t ID,uint8_t DLC,uint8_t* pData)
{
	can_tx_message_type tx_message_struct;
	tx_message_struct.dlc = DLC;
	if(ID==0)
	{
		error=1;
	}
	if(ID>0x7FF) /*ext id*/
	{
		tx_message_struct.extended_id = ID;
		tx_message_struct.id_type = CAN_ID_EXTENDED;
	}else	/*std ID*/
	{
		tx_message_struct.standard_id = (uint16_t)ID;
		tx_message_struct.id_type = CAN_ID_STANDARD;
	}

	tx_message_struct.frame_type = CAN_TFT_DATA;
	if(DLC < 2){
	tx_message_struct.data[0] = pData[0];

	}
	else {

	memcpy(&tx_message_struct.data,pData,tx_message_struct.dlc);

	}
	if(can_message_transmit(CAN1,&tx_message_struct)!= CAN_TX_STATUS_NO_EMPTY) return 1;
	return 0;
}
