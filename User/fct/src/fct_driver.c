/********************************************************************
//版权说明	:Shenzhen E-EYE Co. Ltd., Copyright 2013- All Rights Reserved.
//文件名称	:app_fct.c		
//功能		:主函数
//版本号	:
//开发人	:ytl
//开发时间	:2013.09
//修改者	:
//修改时间	:
//修改简要说明	:
//备注		:
***********************************************************************/
#define	_EXT_FCT_DRIVER_

#include "include.h"
#include <rthw.h>
#include <rtthread.h>


//*********************蜂鸣器控制*****************
u32	BeepOnTime = 0;
u32	BeepOffTime = 0;
u8	BeepOnOffFlag = 0;//蜂鸣器开关标志

//*********************通信模块，定位模块指示灯控制************
u32	GprsLedOnTime = 0;
u32	GprsLedOffTime = 0;
u32	GpsLedOnTime = 0;
u32	GpsLedOffTime = 0;
u8	GprsLedOnOffFlag = 0;//蜂鸣器开关标志
u8	GpsLedOnOffFlag = 0;//蜂鸣器开关标志
u8	GprsLedState = 0;//GPRS LED灯状态,0：不在线熄灭,1：在线闪烁,2：故障常亮
u8	GpsLedState = 0;//GPS LED灯状态,0：不导航熄灭,1：导航闪烁,2：故障常亮


/*
************************************************************************
*	函 数 名: void Fct_GpioInit(void)
*	功能说明: 初始化GPIO输入/输出函数
*	形    参：无
*	返 回 值: 无
**************************************************************************
*/
void Fct_GpioInit(void)
{
	gpio_in_init(PIN_NO_DLIGHTI);
	gpio_in_init(PIN_NO_LLIGHTI);
	gpio_in_init(PIN_NO_BREAK);
	gpio_in_init(PIN_NO_ACC);
	gpio_in_init(PIN_NO_DOORI_N);
	gpio_in_init(PIN_NO_DOORI_P);
	gpio_in_init(PIN_NO_DOORI2);
	gpio_out_init(PIN_NO_LAMP);
	gpio_out_init(PIN_NO_LOCK_OUT);
	gpio_out_init(PIN_NO_UNLOCK_OUT);
	gpio_in_init(PIN_NO_XLIGHTI);
	gpio_in_init(PIN_NO_RLIGHTI);
	gpio_in_init(PIN_NO_BKHI);
	gpio_in_init(PIN_NO_SWITCHI);	
	gpio_in_init(PIN_NO_HORNI); 
	gpio_in_init(PIN_NO_VIRBRATIONI); 
	gpio_out_init(PIN_NO_COIL); 
	gpio_out_init(PIN_NO_HORN_OUT); 
	gpio_out_init(PIN_NO_GSM_PWC);
	gpio_out_init(PIN_NO_GSM_ON);
	
	gpio_in_init(PIN_NO_DETCT1I); 
	gpio_in_init(PIN_NO_DETCT2I); 

	gpio_out_init(PIN_NO_V_COL);

	gpio_out_init(PIN_NO_BUZZER);
	//GpioInInit(MIC_EN);//dxl,2014.9.30原来是配置为输出，后来由于MIC_IN同时也用于区分降成本版本，因此需配置为输入，
											//在检测完降成本版本后函数会自动配置为输出。
	gpio_out_init(PIN_NO_MIC_EN);//dxl,2016.3.30
	gpio_out_init(PIN_NO_SD_POWER);//dxl,2013.5.16

	gpio_out_init(PIN_NO_I2S_AD_DA);//dxl,2013.5.23
	gpio_out_init(PIN_NO_I2S_PWDAN);//dxl,2013.5.23
	gpio_out_init(PIN_NO_I2S_PWADN);//dxl,2013.5.23

	gpio_out_init(PIN_NO_PWR_TYPE);//dxl,2013.5.26

	gpio_in_init(PIN_NO_SD_DETECT);//dxl,2013.8.16

	//Io_InitStatusHighValid(); 			 //初始化状态高有效电平
	//Io_InitExtCarStatusHighValid();  //初始化扩展车辆状态高有效电平
	//Io_InitAlarmHighValid();				 //初始化报警高有效电平
	//Io_InitDefine2StatusHighValid();//初始化自定义状态字2高有效电平

}

/*
************************************************************************
*	函 数 名: void DebugRec_232(void)
*	功能说明: 接收DEBUG调试数据，放到缓冲器
*	形    参：无
*	返 回 值: 无
**************************************************************************
*/
void Init_Tim3Pluse(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    NVIC_InitTypeDef NVIC_InitStructure;


    /* TIM1 Peripheral Configuration */ 
    TIM_DeInit(TIM3);
    TIM_DeInit(TIM4);

    /* TIM1 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	

    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler = 2400-1	;//0x0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 3;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//TIM_CKD_DIV4;//0x0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;

    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);	

    /* TIM IT enable */ 
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 

    TIM_Cmd(TIM3, DISABLE);   //使能定时器计时
    TIM_Cmd(TIM4, DISABLE); 

    /* Enable the TIM1 Trigger and commutation interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn ;//TIM1_TRG_COM_TIM11_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
}
void Init_ExtPluse(void)
{
#if 0
	EXTI_InitTypeDef   EXTI_InitStructure;
  	GPIO_InitTypeDef   GPIO_InitStructure;
  	NVIC_InitTypeDef   NVIC_InitStructure;

  	/* Enable GPIOE clock */
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  	/* Enable SYSCFG clock */
  	//RCC_APB2PeriphClockCmd(RCC_APB1Periph_SYSCFG, ENABLE);
  
  	/* Configure PE2 pin as input floating */
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);

  	/* Connect EXTI Line2 to PE2 pin */
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);

  	/* Configure EXTI Line2 */
  	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);

  	/* Enable and set EXTI2 Interrupt to the lowest priority */
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
	gpio_irq_install(PIN_NO_EXT_PLUSE,PIN_MODE_INPUT,PIN_IRQ_MODE_FALLING,RT_NULL);
} 

/*
************************************************************************
*	函 数 名: void DebugRec_232(void)
*	功能说明: 接收DEBUG调试数据，放到缓冲器
*	形    参：无
*	返 回 值: 无
**************************************************************************
*/
void FCT_AppDisposeDebugRecvData(unsigned char *pBuffer,unsigned short len)
{
    unsigned short i;
    unsigned char *buffer;
    unsigned char tmp;
    unsigned char flag=0;
    buffer=pBuffer;
    for(i=0; i<len-3; i++)
    {
        tmp = buffer[i];
        if('E'==tmp)
        {
            if(strncmp("EYE+",(char const *)&buffer[i],4)==0)
            {
                flag = 1;
                FCT_recv.length = 0;
								memset(FCT_recv.buf,0,sizeof(FCT_recv.buf));
                break;
            }
        }
    }
    ///////////////////////
    if(flag)
    {
        for( ;i<len; i++)
        {
            tmp = buffer[i];
            if(tmp!=0x0d&&FCT_recv.length<FCT_RECV_LEN)
            {
                FCT_recv.buf[FCT_recv.length++]=tmp;
            }
            else
            {
                FCT_recv.buf[FCT_recv.length+1]= '\0'; 
								rt_mq_send(fct_app_msg, "fct_debug", FCT_MSG_LEN);
                //SetEvTask(EV_FCT_CMD);
                break;
            }
        }
    }    
}
/*
************************************************************************
*	函 数 名: uint8_t	DebugCompareTime(uint32_t souce_time,uint32_t const_time)
*	功能说明: //时间延迟方法，本函数采取的是 基准心跳时间 50MS为基准
*	形    参：uint32_t souce_time	开始时间
						uint32_t const_time，延后时间
*	返 回 值: 超过时间返回TRUE ，没有超过延迟时间返回FALSE 
**************************************************************************
*/
void GnssRecIsr(void)
{
    u8 temp;
    if(USART_GetITStatus(UART4,USART_IT_RXNE)==SET)
    {   
        USART_ClearITPendingBit(UART4,USART_IT_RXNE); 
        temp = USART_ReceiveData(UART4);
        if(temp=='$')//起始符
        {
            gnss_rec.length=0;
            gnss_rec.buf[gnss_rec.length++]=temp;	
        }
        else if(0x0a == temp)//结尾符
        {
            gnss_rec.buf[gnss_rec.length++]=temp;	
            if((0 == strncmp("$GPRMC",(char const *)gnss_rec.buf,6))//GPS格式
            ||(0 == strncmp("$GNRMC",(char const *)gnss_rec.buf,6))//GPS+BD格式
            ||(0 == strncmp("$BDRMC",(char const *)gnss_rec.buf,6)))//BD格式
            {
                g_Gnss.gnss_ok=true;

            }
            else if((0 == strncmp("$GPGGA",(char const *)gnss_rec.buf,6))//GPS格式
            ||(0 == strncmp("$GNGGA",(char const *)gnss_rec.buf,6))//GPS+BD格式
            ||(0 == strncmp("$BDGGA",(char const *)gnss_rec.buf,6)))//BD格式
            {
                //memcpy(g_Gnss.gga,gnss_rec.buf,gnss_rec.length);
            }
        }
        else//接收中间数据
        {
            if(gnss_rec.length<REC_GNSS_232)
            gnss_rec.buf[gnss_rec.length++]=temp;	
        }
    }
}

//初始化脉冲传感器中断
#if 0                                                                             
/*
************************************************************************
*	函 数 名: uint8_t	DebugCompareTime(uint32_t souce_time,uint32_t const_time)
*	功能说明: //时间延迟方法，本函数采取的是 基准心跳时间 50MS为基准
*	形    参：uint32_t souce_time	开始时间
						uint32_t const_time，延后时间
*	返 回 值: 超过时间返回TRUE ，没有超过延迟时间返回FALSE 
**************************************************************************
*/
uint8_t	DebugCompareTime(uint32_t souce_time,uint32_t const_time)
{
    uint32_t tmp_time;
    tmp_time=rt_tick_get();
    if(tmp_time>souce_time)
    {
        if(tmp_time-souce_time>const_time||tmp_time-souce_time==const_time)
            return true;
        else
            return	false;

    }
    else
    {
        if(souce_time-tmp_time>const_time||souce_time-tmp_time==const_time)
            return true;
        else
            return false;

    }

}
/*
************************************************************************
*	函 数 名: void Init_Can_1_2(void)
*	功能说明: 初始化CAN的发送寄存器
*	形    参：
						
*	返 回 值: 
**************************************************************************
*/
void Init_Can_1_2(void)
{
	g_tCanTxMsg_1.StdId = 0x321;
	g_tCanTxMsg_1.ExtId = 0x01;
	g_tCanTxMsg_1.RTR = CAN_RTR_DATA;
	g_tCanTxMsg_1.IDE = CAN_ID_STD;
	g_tCanTxMsg_1.DLC = 1;
	
	g_tCanTxMsg_2.StdId = 0x321;
	g_tCanTxMsg_2.ExtId = 0x01;
	g_tCanTxMsg_2.RTR = CAN_RTR_DATA;
	g_tCanTxMsg_2.IDE = CAN_ID_STD;
	g_tCanTxMsg_2.DLC = 1;		
	
}
//CAN 发送
 void SendCanMsg_1(void)
{
    if(g_recCan_1)
    {
        /* 向CAN网络发送一包数据, 数据区第1个字节指LED灯序号 */
        // g_tCanTxMsg.Data[0] = 0x11;//g_ucLedNo;
        CAN_Transmit(CAN1, &g_tCanTxMsg_1);	
        g_recCan_1=0;
    }
}
//can 发送
 void SendCanMsg_2(void)
{
    if(g_recCan_2)
    {
        /* 向CAN网络发送一包数据, 数据区第1个字节指LED灯序号 */
        // g_tCanTxMsg.Data[0] = 0x11;//g_ucLedNo;
        CAN_Transmit(CAN2, &g_tCanTxMsg_2);	
        g_recCan_2=0;
    }
}

//CAN1---can2 发送数据
void Api_CanSendDate_1_2(void)
{
    SendCanMsg_1();	
}
#endif
