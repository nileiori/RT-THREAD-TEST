/*******************************************************************************
模块名       : 软件定时器管理
文件名       : 
相关文件     : 
文件实现功能 : 
作者         : 张文波
版本         : Ver 1.0。
-------------------------------------------------------------------------------
备注         : 
-------------------------------------------------------------------------------
修改记录 : 
日 期        版本     修改人              修改内容
YYYY/MM/DD   X.Y      <作者或修改者名>    <修改内容>
===============================================================================
其他说明:   无。
*******************************************************************************/
#ifndef __APITIMER_C__
#define __APITIMER_C__

#include <include.h>
#include <rthw.h>
#include <rtthread.h>

static STR_TIMER gTimeInf10ms[MAX_TIMENUM_10MS];
static STR_TIMER gTimeInf1s[MAX_TIMENUM_1S];


/******************************************************************************
函数名      : PowerOnStatusCheck(void)
函数实现功能: 检查是否上电
输入        : 无
返回        : 1:上电0:掉电
作者		: 张文波
描述        : 1s定时器
*******************************************************************************/
u8 PowerOnStatusCheck(void)
{
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12);
}

void InitTimer1(void);
void InitTimer1(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* ---------------------------------------------------------------
	TIM2 Configuration: Output Compare Timing Mode:
	TIM2 counter clock = 72/12 = 6 MHz
	CC1 update rate = TIM2 counter clock / CCR1_Val = 1000 Hz
	--------------------------------------------------------------- */
	//T2CCR1_Val = T2CCR_VAL_10MS;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 65535;
	//将TIM2的时钟设为100K
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_PrescalerConfig(TIM1,PRESCALER_VAL,TIM_PSCReloadMode_Immediate);
	
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = T2CCR_VAL_10MS;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
	
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE); 
	TIM_Cmd(TIM1, ENABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure); 
}
/******************************************************************************
函数名      : InitTimer5(void)
函数实现功能: 初始化TIM5
输入        : 无
返回        : 无
作者		: 张文波
描述        : 1s定时器
*******************************************************************************/
void InitTimer5(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 10000;
	if(1/*PowerOnStatusCheck()*/)
	{
    //TIM_TimeBaseStructure.TIM_Prescaler = 100*POWERON_ON_SYSSPEED-1;
		TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock/10000ul-1;
	}
	else
	{
		TIM_TimeBaseStructure.TIM_Prescaler = 100*WAKEUP_ON_SPEED-1;
	}
	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM5, ENABLE); 
  
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure); 
}

/******************************************************************************
函数名      : InitTimer7(void)
函数实现功能: 初始化TIM7
输入        : 无
返回        : 无
作者		: 张文波
描述        : 10ms定时器
*******************************************************************************/
void InitTimer7(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	/* ---------------------------------------------------------------
	TIM2 Configuration: Output Compare Timing Mode:
	TIM2 counter clock = 100 KHz
	CC1 update rate = TIM2 counter clock / CCR1_Val = 100 Hz
	--------------------------------------------------------------- */
	TIM_TimeBaseStructure.TIM_Period = 1000;
	//将TIM2的时钟设为100K
	if(1/*PowerOnStatusCheck()*/)
	{
    //TIM_TimeBaseStructure.TIM_Prescaler = 10*POWERON_ON_SYSSPEED-1;
		TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock/100000ul-1;
	}
  else
  {
    TIM_TimeBaseStructure.TIM_Prescaler = 10*WAKEUP_ON_SPEED-1;
  }
	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0000;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
  TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM7, ENABLE); 
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure); 
}
/******************************************************************************
函数名      : TimerInit(void)
函数实现功能: 初始化定时器
输入        : 无
返回        : 无
作者		: 张文波
描述        : 
*******************************************************************************/
void TimerInit(void)
{
#if(MCU_STM32F1XX==MCU_TYPE) 
	//InitTimer1();
	InitTimer5();
	InitTimer7();
#endif
#if(TIMEER_10MS_ENABLE != 0)
  memset(gTimeInf10ms,0,sizeof(gTimeInf10ms));
#endif	
#if(TIMEER_1S_ENABLE != 0)
  memset(gTimeInf1s,0,sizeof(gTimeInf1s));
#endif
}

#if TIMEER_10MS_ENABLE  
u8 CloseTime10ms(u8 timeNumber);
u8 RestartTime10ms(u8 timeNumber);
void TimerManage10ms(void);
/******************************************************************************
函数名		: Timer10msStart(u8 timeNumber, TIMER_WORDTYPE timeLen, u8 mode, PEFUNCTION callBack) 
函数实现功能: 开起基数为10MS的定时函数
输入		: timeNumber：定时器号  
TimeLen：定时时间长度  
mode:	模式选择  TIMER_MODE_ONCE:单次 TIMER_MODE_CONTINUE:循环
CallBack：回调函数
返回		: 0:成功1:失败
作者		: 张文波
描述		: 
*******************************************************************************/
u8 Timer10msStart(u8 timeNumber, TIMER_WORDTYPE timeLen, u8 mode, PEFUNCTION callBack)    
{
  if((timeNumber < MAX_TIMENUM_10MS)&&(timeLen > 0))
  {
    gTimeInf10ms[timeNumber].number = timeNumber;
    gTimeInf10ms[timeNumber].timeLen = timeLen;
    gTimeInf10ms[timeNumber].timeCount = 0;
    gTimeInf10ms[timeNumber].timerMode = mode;
    gTimeInf10ms[timeNumber].callback = callBack;
    gTimeInf10ms[timeNumber].timeswitch = TIMR_ON;
    return(0);
  }  
  return(1);
}
/*************************************************************
函数名: RestartTime10ms(u8 timeNumber)
函数实现功能:重置基数为10MS的定时函数
输入：定时器号
返回: 0：成功;1：失败
作者		: 张文波
描述		: 
**************************************************************/
u8 RestartTime10ms(u8 timeNumber)
{
  if(timeNumber < MAX_TIMENUM_10MS)
  {   
    gTimeInf10ms[timeNumber].timeswitch = TIMR_ON;
    gTimeInf10ms[timeNumber].timeCount = 0;
    return(0);
  }
  return(1);
}
/*************************************************************
函数名: CloseTime10ms(u8 timeNumber)
函数实现功能:关闭基数为10MS的定时函数
输入：定时器号
返回: 0：成功;1：失败
作者		: 张文波
描述		: 
**************************************************************/
u8 CloseTime10ms(u8 timeNumber)
{
  if(timeNumber < MAX_TIMENUM_10MS)
  {   
    gTimeInf10ms[timeNumber].timeswitch = TIMR_OFF;
    return(0);
  }
  return(1);
}
/*************************************************************
函数名: TimerManage10ms(void)
函数实现功能:管理基数为10MS的定时函数
输入：
返回: 
作者		: 张文波
描述		: 
**************************************************************/
void TimerManage10ms(void)
{
	u8 i;
	ErrorStatus err = ERROR;
	for(i=0;i<MAX_TIMENUM_10MS;i++)
	{
		if(gTimeInf10ms[i].timeswitch == TIMR_ON)
		{
			gTimeInf10ms[i].timeCount++;
			if(gTimeInf10ms[i].timeCount >= gTimeInf10ms[i].timeLen)
			{
				gTimeInf10ms[i].timeCount=0;
				if(gTimeInf10ms[i].callback != NULL)
				{
					err = gTimeInf10ms[i].callback();
				}
				if(gTimeInf10ms[i].timerMode == TIMER_MODE_ONCE)
				{
					gTimeInf10ms[i].timeswitch = TIMR_OFF;
				}
				else
				if((gTimeInf10ms[i].timerMode == TIMER_MODE_STOP_WHEN_SUCCESS)&&(SUCCESS == err))
				{
					gTimeInf10ms[i].timeswitch = TIMR_OFF;
				}
			} 			
		}
	}
}

#endif

#if TIMEER_1S_ENABLE 

u8 CloseTime1s(u8 timeNumber);
void TimerManage1s(void);
/******************************************************************************
函数名		: Timer1sStart(u8 timeNumber, TIMER_WORDTYPE timeLen, u8 mode, PEFUNCTION callBack) 
函数实现功能: 开起基数为1S的定时函数
输入		: timeNumber：定时器号  
TimeLen：定时时间长度  
mode:	模式选择  TIMER_MODE_ONCE:单次 TIMER_MODE_CONTINUE:循环
CallBack：回调函数
返回		: 0:成功1:失败
作者		: 张文波
描述		: 
*******************************************************************************/
u8 Timer1sStart(u8 timeNumber, TIMER_WORDTYPE timeLen, u8 mode,/* void* msg,*/PEFUNCTION callBack)  
{
  if((timeNumber < MAX_TIMENUM_1S)&&(timeLen > 0))
  {
    gTimeInf1s[timeNumber].number = timeNumber;
    gTimeInf1s[timeNumber].timeLen = timeLen;
    gTimeInf1s[timeNumber].timeCount = 0;
    gTimeInf1s[timeNumber].timerMode = mode;
		//gTimeInf1s[timeNumber].msg = msg;
    gTimeInf1s[timeNumber].callback = callBack;
    gTimeInf1s[timeNumber].timeswitch = TIMR_ON;
    return(0);
  }
  return(1);
}

/*************************************************************
函数名: CloseTime1s(u8 timeNumber)
函数实现功能:关闭基数为1S的定时函数
输入：定时器号
返回: 0：成功;1：失败
作者		: 张文波
描述		: 
**************************************************************/
u8 CloseTime1s(u8 timeNumber)
{
  if(timeNumber < MAX_TIMENUM_1S)
  {   
    gTimeInf1s[timeNumber].timeswitch = TIMR_OFF;
    return(0);
  }
  return(1);
}
/*************************************************************
函数名: TimerManage1s(void)
函数实现功能:管理基数为1S的定时函数
输入：
返回: 
作者		: 张文波
描述		: 
**************************************************************/
void TimerManage1s(void)
{
	u8 i;
	ErrorStatus err = ERROR;
	for(i=0;i<MAX_TIMENUM_1S;i++)
	{
		if(gTimeInf1s[i].timeswitch == TIMR_ON)
		{
			gTimeInf1s[i].timeCount++;
			if(gTimeInf1s[i].timeCount >= gTimeInf1s[i].timeLen)
			{
				gTimeInf1s[i].timeCount = 0;
				if(gTimeInf1s[i].callback != NULL)
				{
					// TODO: Need check to protect more
					err = gTimeInf1s[i].callback();
				}
				if(gTimeInf1s[i].timerMode == TIMER_MODE_ONCE)
				{
					gTimeInf1s[i].timeswitch = TIMR_OFF;
				}
				else
				if((gTimeInf1s[i].timerMode == TIMER_MODE_STOP_WHEN_SUCCESS)&&(SUCCESS == err))
				{
					gTimeInf1s[i].timeswitch = TIMR_OFF;
				}
			} 					
		}
	}
}

#endif
#endif

