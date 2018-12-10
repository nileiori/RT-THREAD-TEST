#ifndef __RTC_H
#define __RTC_H

//*********文件包含************
#include "stm32f10x.h"
#include "stm32f10x_conf.h"



//**********自定义数据类型********
//时间结构体类型
typedef struct
	{
		//s16	year;	//年:1970~
		s8  year;//年:0~99
		s8	month;	//月:1~12
		s8	day;	//日:1~31
		s8	hour;	//时:0~23
		s8	min;	//分:0~59
		s8	sec;	//秒:0~59
	}TIME_T;

//**********函数声明**********

//*********宏定义**************
//#define START_YEAR		2015
//#define END_YEAR		  2070

#define START_YEAR		13//dxl,2016.5.9,部标检测场景的时间是13年
#define END_YEAR		  70

//时间宏定义
#define MIN_SECOND		60
//#define HOUR_SECOND			(60*60)
//#define DAY_SECOND		(24*60*60)
//#define YEAR_SECOND			(365*24*60*60)
#define HOUR_SECOND			0xE10
#define DAY_SECOND		0x15180
#define YEAR_SECOND 			0x1E13380
#define LEAP_YEAR			1
//**********函数声明**********
void SetRtc(TIME_T *tt);
void ReadRtc(void);
u8 LeapYear(s16 year);
u16 YearDay(s16 yy,s8 mm,s8 dd);
u32 ConverseGmtime(TIME_T *tt);
void Gmtime(TIME_T *tt, u32 counter);
void RTC_Configuration(void);
void InitRtc(void);
void Num2NumStr(u32 Num, u8 *NumStr);
void RTC_GetCurTime(TIME_T *tt);
ErrorStatus CheckTimeStruct(TIME_T *tt);
void RTC_PowerOnHandle(void);
#ifdef REALTIME_SINGLE_TEST

void SendTimeToUsart(TIME_T *tt, u8 *pBuffer);

#endif

#endif
