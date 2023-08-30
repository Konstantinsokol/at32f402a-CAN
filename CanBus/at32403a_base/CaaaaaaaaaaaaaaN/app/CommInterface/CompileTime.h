/*
 * CompileTime.h
 *
 *  Created on: 27 дек. 2018 г.
 *      Author: Andrey
 */

#ifndef ECU_ALG_CODE_COMPILETIME_H_
#define ECU_ALG_CODE_COMPILETIME_H_

#include "stdlib.h"
#include "string.h"
#include "stdint.h"

typedef struct
{
  unsigned int c_day;
  unsigned int c_hour;
  unsigned int c_min;
  unsigned int c_month;
  unsigned int c_sec;
  unsigned int c_year;
  uint32_t kvu_time_stamp;
}CompileTime_st;

/*Определение функции*/
void compileTimeInit(void);
uint32_t getCompileTime(uint16_t);
extern CompileTime_st CompileTime;
#endif /* ECU_ALG_CODE_COMPILETIME_H_ */
