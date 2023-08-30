/*
 * Tasks.h
 *
 *  Created on: 2023 Apr 13
 *      Author: on4ip
 */

#ifndef APP_FREERTOSTASKS_TASKS_H_
#define APP_FREERTOSTASKS_TASKS_H_

/*** FREERTOS ****/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#define ENABLE_FREERTOS					1
void initFreeRtos(void);

#endif /* APP_FREERTOSTASKS_TASKS_H_ */
