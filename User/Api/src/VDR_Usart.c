//#include "stm32f10x.h"
//#include "stm32f10x_usart.h"
//#include "stm32f10x_gpio.h"
//#include "stm32f10x_rcc.h"
//#include "misc.h"
//#include "VDR_Log.h"
//#include "modem_debug.h"
//#include "Carload.h"
#include <string.h>
#include "include.h"
#include <rthw.h>
#include <rtthread.h>

static rt_sem_t VDR_Usart_sem = RT_NULL;

VDR_USART_STRUCT    VdrUsart;

u8  VdrTxBuffer[VDR_TX_Q_BUFFER_SIZE];//VDR发送队列缓冲

u8  VdrTxTempBuffer[VDR_TX_TEMP_BUFFER_SIZE];

static void VDRUsart_RxIsr(void);
static void VDRUsart_TxIsr(void);

static void VDR_Usart_thread_entry(void* parameter)
{   
	VDRUsart_Init();
	VDR_Usart_sem = rt_sem_create("VDR_Usart_sem",0,RT_IPC_FLAG_FIFO);
	while (1)    
	{        
		//等待串口接受数据
		if(RT_EOK == rt_sem_take(VDR_Usart_sem,RT_WAITING_FOREVER))
		{
			VDRUsart_SendData(VdrUsart.RxBuffer, VdrUsart.RxCount);
			VdrUsart.RxCount = 0;
		}
	
	}
}
static void VDR_Usart_release_sem(void)
{
	rt_sem_release(VDR_Usart_sem);
}

void VDR_Usart_thread_init(void)
{
	rt_err_t result;
	
	result = rt_thread_init(&VDR_Usart_thread,                            
							"VDR_Usart_task",                            
							VDR_Usart_thread_entry,                            
							RT_NULL,                            
							(rt_uint8_t*)&VDR_Usart_stack[0],                            
							sizeof(VDR_Usart_stack),                            
							VDR_Usart_THREAD_PRIORITY,                            
							5);    
	if (result == RT_EOK)    
	{        
		rt_thread_startup(&VDR_Usart_thread);    
	} 
}

void VDRUsart_Init(void)
{
  // dxl,2015.8,以下是针对STM32F105的初始化代码
      GPIO_InitTypeDef    GPIO_InitStructure;
    USART_InitTypeDef   USART_InitStructure;
      NVIC_InitTypeDef    NVIC_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = VDR_USART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = VDR_USART_TX_PIN;          
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(VDR_USART_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = VDR_USART_RX_PIN;                   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(VDR_USART_PORT, &GPIO_InitStructure);
    
    USART_InitStructure.USART_BaudRate = VDR_USART_BAUDRATE;                
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;    
    //USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;         
    USART_InitStructure.USART_Parity = USART_Parity_Odd ;          
    //USART_InitStructure.USART_Parity = USART_Parity_No; 
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(VDR_USART, &USART_InitStructure);         
  
    USART_ITConfig(VDR_USART, USART_IT_RXNE, ENABLE);    
    USART_Cmd(VDR_USART, ENABLE);  

}   

void VDRUsart_Isr(void)
{
    if(USART_GetITStatus(VDR_USART, USART_IT_RXNE) != RESET)
    {
        VDRUsart_RxIsr();
    } 
  
    if(USART_GetITStatus(VDR_USART, USART_IT_TXE) != RESET)
    {
        VDRUsart_TxIsr();
    }
		
		if(USART_GetITStatus(VDR_USART, USART_IT_TC) != RESET)
    {
        USART_ITConfig(VDR_USART, USART_IT_TC, DISABLE);
    }
}

u16  VDRUsart_SendData(u8 *pBuffer, u16 BufferLen)
{

    if(0 == VdrUsart.TxCount)
    {
        if(BufferLen > VDR_USART_TX_BUFFER_SIZE)    
        {
            return 0;
        }
        else
        {
            memcpy(VdrUsart.TxBuffer,pBuffer,BufferLen);
            VdrUsart.TxLen = BufferLen;
            USART_ITConfig(VDR_USART, USART_IT_TXE, ENABLE);
            return BufferLen;
        }
    }
    else
    {
        return 0;
    }
}

static void VDRUsart_TxIsr(void)
{
    u8  ch;

    if(VdrUsart.TxCount < VdrUsart.TxLen)
    {
        
        if(VdrUsart.TxCount < VDR_USART_TX_BUFFER_SIZE)
        {
            ch = VdrUsart.TxBuffer[VdrUsart.TxCount];
            USART_SendData(VDR_USART, ch);
            VdrUsart.TxCount++;
        }
        else
        {
            VdrUsart.TxCount = 0;
            VdrUsart.TxLen = 0;
            USART_ITConfig(VDR_USART, USART_IT_TXE, DISABLE);
        }
    }
    else
    {
        VdrUsart.TxCount = 0;
        VdrUsart.TxLen = 0;
        USART_ITConfig(VDR_USART, USART_IT_TXE, DISABLE);
    } 
}   

static void VDRUsart_RxIsr(void)
{
    u8 ch;
    
    VdrUsart.RxIdleCount = 0;
    
    ch = USART_ReceiveData(VDR_USART);
    VdrUsart.RxBuffer[VdrUsart.RxCount] = ch;
    VdrUsart.RxCount++;
    if(VdrUsart.RxCount >= VDR_USART_RX_BUFFER_SIZE)
    {
        VdrUsart.RxCount = 0;
    }
		Timer10msStart(TIMER_10MS_ID_COMM1,3,TIMER_MODE_ONCE,VDR_Usart_release_sem);//30ms则认为接收完成
}
