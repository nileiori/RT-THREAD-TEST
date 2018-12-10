/********************************************************************
//版权说明  :Shenzhen E-EYE Co. Ltd., Copyright 2009- All Rights Reserved.
//文件名称  :SystemInit.c       
//功能      :系统初始化 
//版本号    :
//开发人    :dxl
//开发时间  :2012.4
//修改者    :
//修改时间  :
//修改简要说明  :
//备注      :
***********************************************************************/
//***************包含文件*****************
#include "include.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "gpio.h"

//***************本地变量******************
//***************外部变量*****************

//**************函数定义*******************

/*********************************************************************
//函数名称  :MemoryChipSelfCheck
//功能      :上电存储芯片自检
//输入      :
//输出      :
//使用资源  :
//全局变量  :   
//调用函数  :
//中断资源  :  
//返回      :
//备注      :自检的存储芯片有：FRAM、FLASH、EEPROM
*********************************************************************/
void MemoryChipSelfCheck(void)
{

}
/*********************************************************************
//函数名称	:PowerOnUpdata
//功能		:上电更新变量
//输入		:
//输出		:
//使用资源	:
//全局变量	:   
//调用函数	:
//中断资源	:  
//返回		:
//备注		:更新的变量有：IO、GPS、位置汇报、摄像头
*********************************************************************/
void PowerOnUpdata(void)
{
 
    /////////////////////
    //BEEP_ON();
    //更新IO变量
    //Io_UpdataPram();
    //更新GPS变量
    //更新心跳包时间间隔
    //HeartBeat_UpdatePram();
    //更新脉冲变量
    
    //脉冲系数校准初始化
    //Adjust_Init();       
    //更新手机号码
    //RadioProtocol_UpdateTerminalPhoneNum();
    //更新位置汇报变量
    //Report_UpdataPram();
    //更新摄像头变量
    //Camera_StartCheck();
    //Camera_UpdataPram();
    //Camera_PowerOnSelfCheck();//检测485总线上有多少个摄像头,去掉上电检测，缩短上线时间
    //Photo_UpdataPram();//无限张数的定时定距拍照参数更新
    //////////////////
   // Report_ParameterInitialize();
    //录音任务初始化   
    //////////////////
    //GPS_POWER_ON();
    
    //检测是12V供电还是24V供电
    //Io_PowerOnDetectMainPower();     
    ////////////////////////
    FCT_AppInit();
    ////////////    
    ////////////////////////////
    Public_ParameterInitialize();
    //更新超速变量
    //BEEP_OFF();
   
}

ErrorStatus hd_timer_callback_0(void)
{	
	#ifdef	RT_USING_CONSOLE	 
		rt_kprintf("hd_timer_callback_0...\r\n");
	#endif	
	return SUCCESS;
}
ErrorStatus hd_timer_callback_1(void)
{	
	#ifdef	RT_USING_CONSOLE	 
		rt_kprintf("hd_timer_callback_1...\r\n");
	#endif	
	return SUCCESS;
}
void pin_irq_hdr_acc(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin_acc中断产生...\r\n");
#endif 
}
/*********************************************************************
//函数名称  :PeripheralInit
//功能      :外设初始化
//输入      :
//输出      :
//使用资源  :
//全局变量  :   
//调用函数  :
//中断资源  :  
//返回      :
//备注      :初始化的外设有：GPS、GPRS、FRAM、FLASH、EEPROM、摄像头
*********************************************************************/
void PeripheralInit(void)
{
	
	//GPIO初始化
  Fct_GpioInit(); 
	
	InitRtc();
	    
	Ad_Init();

	TimerInit();

	//SD_POWER_OFF();//先关闭SD卡电源供电
	//Io_Init();
	    
	//硬件复位显示屏,使用的是USB电源控制引脚
	USB_SW_Init();
	USB_SW_0();
	rt_thread_delay( RT_TICK_PER_SECOND/2 );  
	
	USB_SW_1();

	rt_thread_delay( RT_TICK_PER_SECOND/2 );        

	BEEP_ON();

	//FLASH模块初始化 
  //sFLASH_DeInit();
  //sFLASH_Init();
  //sFLASH_AllUnprotect();

	//EEPROM模块初始化
	//E2prom_InitIO(); 

  FRAM_Init();
	
	//SD_POWER_ON();//给SD卡供电
	rt_thread_delay( RT_TICK_PER_SECOND/2 ); 
	// SD SPI初始
        
  //Media_CheckSdOnOff();//dxl,2014.6.18,检测SD卡在不在需要放在InitSDFile之前,因为在InitSDFile里面还会检测SD卡是否损坏，若损坏了等同于不在
  //SPIx_Init(); 
  //f_mount(1,&fs);
  //InitSDFile();//程序在此处计数次进入会启动不了，偶数次可以
    
	BEEP_OFF();

	//USB_AppInitialize();
	
	//装载gpio管脚中断回调函数
	gpio_irq_install_callback(PIN_NO_ACC,PIN_MODE_INPUT_PULLDOWN,pin_irq_hdr_acc,PIN_IRQ_MODE_RISING,RT_NULL);
	gpio_irq_install(PIN_NO_SWITCHI,PIN_MODE_INPUT,PIN_IRQ_MODE_FALLING,RT_NULL);

	
	#if 0//TIMEER_1S_ENABLE 
	Timer1sStart(TIMER_1S_ID_0,10,TIMER_MODE_ONCE,hd_timer_callback_0);
	StartTime1s(TIMER_1S_ID_1,10,hd_timer_callback_1);
	#endif
}

/*******************************************************************************
* Function Name  : IDWG_Configuration
* Description    : Configures IDWG clock source and prescaler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void IDWG_Configuration(void)
{
#if E_EYE_ENABLE == CONFIG_IWDG
/* Enable the LSI oscillator ************************************************/
  RCC_LSICmd(ENABLE);
  
  /* Wait till LSI is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  /* IWDG counter clock: 40KHz(LSI) / 32 = 1.25 KHz */
  //IWDG_SetPrescaler(IWDG_Prescaler_32);
    IWDG_SetPrescaler(IWDG_Prescaler_128);/*40KHz(LSI) / 128 = 320Hz */
    
  /* Set counter reload value to 2500 */
  IWDG_SetReload(0x9c4);//2秒
    //IWDG_SetReload(0x4E2);//1秒
  //IWDG_SetReload(0x271);//0.5秒
  //IWDG_SetReload(0xFA);//0.2秒


  /* Reload IWDG counter */
  IWDG_ReloadCounter();

  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable();  
#endif
}

/*******************************************************************************
* Function Name  : PWR_PVDInit
* Description    : 电源PVD初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PWR_PVDInit(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    
    PWR_PVDLevelConfig(PWR_PVDLevel_2V9);
    PWR_PVDCmd(ENABLE);
    EXTI_StructInit(&EXTI_InitStructure);
    EXTI_InitStructure.EXTI_Line = EXTI_Line16;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
}

/*******************************************************************************
* Function Name  : USB_SW_Init
* Description    : USB供电引脚初始化，该引脚用于硬件复位LCD屏
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void   USB_SW_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
  
/******************************IO***********************************/    
    RCC_APB2PeriphClockCmd(  RCC_USB_SW , ENABLE);         /* 打开GPIO时钟 */

    GPIO_InitStructure.GPIO_Pin = PIN_USB_SW  ;
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        /* 快速模式 */ 
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             /* 输出/输入 */
    
    //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           /* 开漏 */
    
  //  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             /* 上拉 */
    
    GPIO_Init( GPIO_USB_SW , &GPIO_InitStructure);
    
    USB_SW_1();
}
