/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "include.h"
#include "comm.h"
//#include "Vdr_Usart.h"
#include <rthw.h>
#include <rtthread.h>
#include "board.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_CC_IRQHandler
* Description    : This function handles TIM1 capture compare interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_CC_IRQHandler(void)
{
	rt_interrupt_enter();
	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
	{   
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
		//TimerManage10ms();
	}
	rt_interrupt_leave();
}
/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{
	rt_interrupt_enter();
  if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
  {
      TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
      g_Pluse.time++;
      if(g_Pluse.time>4999)
      {
          g_Pluse.end=true;
      }
  }
	rt_interrupt_leave();
}
#if 0
//#include "usb_hcd_int.h"
#include "usb_bsp.h"
/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{
	rt_interrupt_enter();
  USB_OTG_BSP_TimerIRQ();
	rt_interrupt_leave();
}
#endif
/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : This function handles TIM5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM5_IRQHandler(void)
{
	rt_interrupt_enter();
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{   
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	TimerManage1s();
	}
	rt_interrupt_leave();
}
/*******************************************************************************
* Function Name  : TIM7_IRQHandler
* Description    : This function handles TIM7 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM7_IRQHandler(void)
{
  rt_interrupt_enter();
  if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
  {   
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    TimerManage10ms();
  }
  rt_interrupt_leave();
}
#if 0
/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	COMM_IRQHandler(COM2);

	/* leave interrupt */
	rt_interrupt_leave();
}
/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	if(GetUsartStatus(COM3, USART_IT_RXNE) != RESET)
	{
		// Gps_RxIsr();
		GnssRecIsr();
	}  
	if(GetUsartStatus(COM3, USART_IT_TXE) != RESET)
	{
		//Gps_TxIsr();
	}
	
	/* leave interrupt */
	rt_interrupt_leave();
}
#endif
/*******************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : This function handles UART4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART4_IRQHandler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	//COMM_IRQHandler(COM4);
	if(GetUsartStatus(COM4, USART_IT_RXNE) != RESET)
	{
		// Gps_RxIsr();
		GnssRecIsr();
	}  
	if(GetUsartStatus(COM4, USART_IT_TXE) != RESET)
	{
		//Gps_TxIsr();
	}
	/* leave interrupt */
	rt_interrupt_leave();
}

#if 0
/*******************************************************************************
* Function Name  : UART5_IRQHandler
* Description    : This function handles UART5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART5_IRQHandler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	COMM_IRQHandler(COM5);

	/* leave interrupt */
	rt_interrupt_leave();
}
#endif
/*******************************************************************************
* Function Name  : USART6_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART6_IRQHandler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	COMM_IRQHandler(COM6);

	/* leave interrupt */
	rt_interrupt_leave();
}


/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

//void SysTick_Handler(void)
//{
//    // definition in boarc.c
//}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

#ifdef  RT_USING_LWIP
/*******************************************************************************
* Function Name  : EXTI4_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI4_IRQHandler(void)
{

    /* enter interrupt */
    rt_interrupt_enter();

    /* Clear the DM9000A EXTI line pending bit */
    EXTI_ClearITPendingBit(EXTI_Line4);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* RT_USING_LWIP */

/**
  * @}
  */


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
