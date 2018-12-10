/********************************************************************
//版权说明	:
//文件名称	:FCT_driver.h	
//功能		:调试输出头文件
//版本号	:
//开发人	:yjb
//开发时间	:2012.6
//修改者	:
//修改时间	:
//修改简要说明	:
//备注		:
***********************************************************************/
#ifndef __FCT_DRIVER__
#define __FCT_DRIVER__

#ifndef	_EXT_FCT_DRIVER_
	#define	_EXT_FCT_DR_		extern
#else
	#define	_EXT_FCT_DR_
#endif

#include <rtthread.h>
//#include "GPIOControl.h"
#include "pin.h"
#include "gpio.h"

extern u8 	BeepOnOffFlag;
extern u8	GprsLedOnOffFlag;
extern u8	GpsLedOnOffFlag;

extern u32	BeepOnTime;
extern u32	BeepOffTime;
extern u32	GprsLedOnTime;
extern u32	GprsLedOffTime;
extern u32	GpsLedOnTime;
extern u32	GpsLedOffTime;

//************蜂鸣器控制************
#define  BEEP_ON()  		gpio_out_on(PIN_NO_BUZZER);BeepOnOffFlag = 1;BeepOnTime=rt_tick_get()
#define  BEEP_OFF()     gpio_out_off(PIN_NO_BUZZER);BeepOnOffFlag = 0;BeepOffTime=rt_tick_get()
//*************扬声器控制*********
#define SPEAKER_ON()            gpio_out_on(PIN_NO_V_COL);
#define SPEAKER_OFF()           gpio_out_off(PIN_NO_V_COL);
//*************麦克偏置电压控制*********
#define MIC_BASE_ON()       GPIO_WriteBit(GPIOD,GPIO_Pin_5,Bit_SET);MicBaseOnOffFlag = 1
#define MIC_BASE_OFF()      GPIO_WriteBit(GPIOD,GPIO_Pin_5,Bit_RESET);MicBaseOnOffFlag = 0
//*************输出2控制*********
#define OUTPUT2_SET()       GPIO_WriteBit(GPIOF,GPIO_Pin_2,Bit_SET)
#define OUTPUT2_RESET()     GPIO_WriteBit(GPIOF,GPIO_Pin_2,Bit_RESET)
//*************SD卡电源电压控制*********
#define  SD_POWER_ON()      gpio_out_off(PIN_NO_SD_POWER)
#define  SD_POWER_OFF()     gpio_out_on(PIN_NO_SD_POWER)
//*************油路控制*********
#define  OIL_CUT_ON()       gpio_out_off(PIN_NO_COIL)  //通油
#define  OIL_CUT_OFF()      gpio_out_on(PIN_NO_COIL)   //断油,高电平输出
//**************12V/24V供电控制***************
#define   PWR_TYPE_24V()    gpio_out_on(PIN_NO_PWR_TYPE)
#define   PWR_TYPE_12V()    gpio_out_off(PIN_NO_PWR_TYPE)

//-------------------------------------------------------
#define FCT_RECV_LEN 128
//接收解析数据
typedef struct _DEBUG_REC_
{
	uint8_t	state;
	uint8_t	length;
	uint8_t	buf[FCT_RECV_LEN+1];
}PRTL_REC_DEBUG;

//接收解析数据
typedef struct _GNSS_
{
	uint32_t	time;
	uint8_t		gnss;
	uint8_t		gnss_ok;
}_API_GNSS_;


_EXT_FCT_DR_	PRTL_REC_DEBUG	FCT_recv;

_EXT_FCT_DR_		_API_GNSS_	g_Gnss;

//GSM-232空间划分
#define REC_GNSS_232		100		
#define SEND_GNSS_232		100		

//临时接收缓冲区
typedef struct _GNSS_REC_
{
	uint8_t	state;
	uint8_t	length;
	uint8_t	buf[REC_GNSS_232];
}PRTL_REC_GNSS;

_EXT_FCT_DR_	PRTL_REC_GNSS 	gnss_rec;	



//IO初始化
void Fct_GpioInit(void);
void GnssRecIsr(void);
//采用50MS为基准的时间比较函数
//超过时间返回TRUE ，没有超过延迟时间返回FALSE 
uint8_t	DebugCompareTime(uint32_t souce_time,uint32_t const_time);
void FCT_AppDisposeDebugRecvData(unsigned char *pBuffer,unsigned short len);


typedef struct	__PLUSE_TIM3__
{
	uint32_t time;
	uint32_t count;
	uint8_t flag;
	uint8_t end;
	
}_API_PLUSE_T_;

_EXT_FCT_DR_	_API_PLUSE_T_		g_Pluse;
//初始化定时器3
void Init_Tim3Pluse(void);
//初始化脉冲传感器中断
void Init_ExtPluse(void);

#endif
