/*
 * Tasks.c
 *
 *  Created on: 2023 Apr 13
 *      Author: on4ip
 */
#include "../app/FreeRtosTasks/Tasks.h"

#include "at32f403a_407_board.h"
#include "nvicInit.h"
#include "CommInterface.h"
#include "RadarCan.h"
TaskHandle_t led2_handler;
/* led2 task */
void led2_task_function(void *pvParameters);

/* FreeRtos HOOK functions*/
uint16_t heapError = 0;
void vApplicationMallocFailedHook( void )
{
	heapError =1;
}

void vApplicationDaemonTaskStartupHook( void )
{
	/*Enable global IRQ*/
	nvicInit();
	__enable_irq();
}
/* IDLE loop low prio function */
void vApplicationIdleHook( void )
{
	messageLoop();
}


void initFreeRtos(void)
{
	/* enter critical */
#if ENABLE_FREERTOS
	taskENTER_CRITICAL();
	xTaskCreate((TaskFunction_t) led2_task_function, (const char*) "LED2_task",
			(uint16_t) 128, (void*) NULL, (UBaseType_t) 2,
			(TaskHandle_t*) &led2_handler);
	/* exit critical */
	taskEXIT_CRITICAL();

	/* start scheduler */
	vTaskStartScheduler();
#endif
}



/* led2 task function */
void led2_task_function(void *pvParameters)
{
  while(1)
  {
    at32_led_toggle(LED2);
    commProcess();
    vTaskDelay(1000);
  }
}
