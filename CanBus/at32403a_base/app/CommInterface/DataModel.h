/*
 * UART.h
 *
 *  Created on: 16 июля 2018 г.
 *      Author: Andrey
 */

#ifndef SOURCE_UART_H_
#define SOURCE_UART_H_
#include "Message.h"
enum ScreenList
{
	LIVE_SCREEN=0,
	MOTOR_SCREEN,
	DRIVE_SCREEN,
	IDENT_SCREEN,
	BATTERY_SCREEN,
	REG_SCREEN,
	INPUTS_SCREEN,
	CAN_SCREEN,
	SENSORLESS_SCREEN,
	ANALOG_SCREEN,
	FUNCTIONS_SCREEN,
	BUTTONS_SCREEN,
	LOGSETINNGS_SCREEN=125,
	INFO_SCREEN,
	SAVE_SCREEN,
	LOGREAD_SCREEN
};
typedef struct
{
  uint8_t motorPolePairs;		//number of motor pole pairs
  float motorRs;			//motor phase resistance
  float motorLs;			//motor phase inductance
  float motorKv;			//motor flux coefficient
  uint8_t motorDirection;		//motor direction bit
  uint16_t motorMaxSpeed;		//motor maxSpeed in rad\sec
  /*position sensor*/
  uint16_t posSensorType;		//motor position sensor type
  uint16_t posSensorCounts;		//motor position sensor counts per revolution
  float posSensorShift;			//motor position sensor shift angle in rad
  /*motor temperature*/
  uint16_t temprSensorType;		//motor temperature sensor type
  uint16_t temprLimitStart;		//power fade limit start in oC
  uint16_t temprLimitEnd;		//power fade limit end in oC
}motorSettings_st;

typedef struct
{
  float controllerUdc;			//controller DC bus voltage
  float controller
}liveData_st;
void Process_screen(void);
uint8_t isReset(void);
#endif /* SOURCE_UART_H_ */
