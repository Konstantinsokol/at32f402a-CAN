/*
 * CompileTime.c
 *
 *  Created on: 17.06.2013
 *      Author: Salnikov
 */
#include "CompileTime.h"
/*==============================================
Интерфейс аналогичен предложенному Глебовым Н.А.
Параметр select содержит подиндекс из запроса.
В зависимости от значения select возвращаются
следующие параметры:
00 - год;
01 - месяц;
02 - день;
03 - час;
04 - минута;
05 - секунда;
06 - обобщенное время в формате (ггммддччмм).
==============================================*/
CompileTime_st CompileTime;
void compileTimeInit(void)
{
    const char monthEnum[12][3] =
    {
        "Jan", "Feb", "Mar", "Apr",
        "May", "Jun", "Jul", "Aug",
        "Sep", "Oct", "Nov", "Dec"
    };
    
    char wkStr[9];
    
    uint16_t i;
    for (i = 0; i < 12; i++)
    {
        if (strncmp(__DATE__, monthEnum[i], 3) == 0)
        {
            CompileTime.c_month = i + 1;
            break;
        }
    }
    
    wkStr[4] = 0;
    
    strncpy(wkStr, &__DATE__[7], 4);
    CompileTime.c_year = (uint16_t) atoi(wkStr);
    
    wkStr[2] = 0;
    
    strncpy(wkStr, &__DATE__[4], 2);
    CompileTime.c_day = atoi(wkStr);
    
    strncpy(wkStr, &__TIME__[0], 2);
    CompileTime.c_hour = atoi(wkStr);
    
    strncpy(wkStr, &__TIME__[3], 2);
    CompileTime.c_min = atoi(wkStr);
    
    strncpy(wkStr, &__TIME__[6], 2);
    CompileTime.c_sec = atoi(wkStr);
    struct pack_st{
    	uint16_t min:6;
    	uint16_t hour:5;
    	uint16_t day:5;
    	uint16_t month:5;
    	uint16_t year:5;
    	uint16_t res:6;
    };
    union{
    	 struct pack_st time_pack;
    	uint32_t time_word;
    }time_un;
    time_un.time_pack.min =CompileTime.c_min;
    time_un.time_pack.hour = CompileTime.c_hour;
    time_un.time_pack.day = CompileTime.c_day;
    time_un.time_pack.month = CompileTime.c_month;
    time_un.time_pack.year = CompileTime.c_year - 2000;
    CompileTime.kvu_time_stamp =(uint32_t)time_un.time_word;
}

uint32_t getCompileTime(uint16_t select)
{
    uint32_t ret_time;
    
    switch (select)
    {
    case 0:
        ret_time = CompileTime.c_year;
        break;
    case 1:
        ret_time = CompileTime.c_month;
        break;
    case 2:
        ret_time = CompileTime.c_day;
        break;
    case 3:
        ret_time = CompileTime.c_hour;
        break;
    case 4:
        ret_time = CompileTime.c_min;
        break;
    case 5:
        ret_time = CompileTime.c_sec;
        break;
    case 6:
        ret_time = CompileTime.kvu_time_stamp;
        break;
    default:
        ret_time = 0;
        break;
    }
    
    return ret_time;
}

