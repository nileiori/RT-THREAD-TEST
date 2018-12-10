/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*
*           (2) This file and its dependencies requires IAR v6.20 or later to be compiled.
*
*********************************************************************************************************
*/
#ifndef __QUEUE_H
#define __QUEUE_H


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "stm32f10x.h"


/*
*********************************************************************************************************
*                                             define
*********************************************************************************************************
*/
#define MIN_QUEUE_SIZE  ( 10 )
#define MAX_QUEUE_SIZE  ( 2000 )

//#define CIRQUEUE_SELF_TEST

/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/
typedef u8 ElementType;

//#define	QUEUE_HAVE_NODE		//有节点功能
//循环队列

typedef struct
{
	u16					lens;
}Q_NODE;
typedef struct CirQueueDef
{
  int 				m_iHead;            //队列头       
  int 				m_iTail;            //队列尾
  int 				m_iQueueLenth;      //队列长度
  int 				m_iQueueCapacity;   //队列容量
  #ifdef	QUEUE_HAVE_NODE
  int					m_iNodeHead;				//节点头
  int					m_iNodeTail;				//节点尾
  int					m_iNodeNum;					//节点数
  int					m_iMaxNodeNum;			//最大节点数
  Q_NODE			*m_pNode;						//节点指针
  #endif
  ElementType *m_pQueue;          //队列指针
}xCIRQUEUE;
typedef xCIRQUEUE	CirQueue_t;
typedef void * CirQueueHandle_t;

typedef enum q_state
{
	Q_STATE_SUCCESS =					0u,
	Q_STATE_FAILED,									
	Q_STATE_ENQUEUE_OVERFLOW,				//入队溢出,入队长度超出队列容量
	Q_STATE_DEQUEUE_OVERFLOW,				//出队溢出,出队长度超出队列容量
}Q_STATE;

/*********************************************************************************************************
*                                          IsCirQueueEmpty
*
* Description : 队列判空，如果长度=0则为空，这时没有元素可以出队
*
* Argument(s) : 循环队列指针
*
* Return(s)   : 队列状态
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
Q_STATE IsCirQueueEmpty(CirQueueHandle_t xCirQueue);
/*********************************************************************************************************
*                                          IsCirQueueFull
*
* Description : 队列判满，如果长度=容量，则此时不能有元素入队
*
* Argument(s) : 循环队列指针
*
* Return(s)   : 队列状态
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
Q_STATE IsCirQueueFull(CirQueueHandle_t xCirQueue);
/*********************************************************************************************************
*                                          GetCirQueueLength
*
* Description : 获取队列长度
*
* Argument(s) : 循环队列指针
*
* Return(s)   : 队列长度
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
int	GetCirQueueLength(CirQueueHandle_t xCirQueue);
/*********************************************************************************************************
*                                          EnCirQueue
*
* Description : 元素入队
*
* Argument(s) : element:待入队的元素   
*								pQ:循环队列指针
*
* Return(s)   : 队列状态
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
Q_STATE EnCirQueue(ElementType element,CirQueueHandle_t xCirQueue);
/*********************************************************************************************************
*                                          BufferEnCirQueue
*
* Description : 多元素入队
*
* Argument(s) : pBuffer:待入队的数据指针 
*								BufferLen:入队数据长度
*								pQ:循环队列指针
*
* Return(s)   : 队列状态
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
int BufferEnCirQueue(ElementType *pBuffer, int BufferLen, CirQueueHandle_t xCirQueue);

/*********************************************************************************************************
*                                          DeCirQueue
*
* Description : 元素出队
*
* Argument(s) : element:出队的数据指针 
*								pQ:循环队列指针
*
* Return(s)   : 队列状态
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
Q_STATE DeCirQueue(ElementType *element,CirQueueHandle_t xCirQueue);
/*********************************************************************************************************
*                                          BufferDeCirQueue
*
* Description : 多元素出队
*
* Argument(s) : pBuffer:待出队的数据指针 
*								BufferLen:出队数据长度
*								pQ:循环队列指针
*
* Return(s)   : 队列状态
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
int BufferDeCirQueue(ElementType *pBuffer, int BufferLen, CirQueueHandle_t xCirQueue);
/*********************************************************************************************************
*                                          AllBufferDeCirQueue
*
* Description : 所有元素出队
*
* Argument(s) : pBuffer:待出队的数据指针 
*								pQ:循环队列指针
*
* Return(s)   : 队列状态
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
int AllBufferDeCirQueue(ElementType *pBuffer,CirQueueHandle_t xCirQueue);
/*********************************************************************************************************
*                                          vCirQueueGenericClear
*
* Description :队列清空
*
* Argument(s) : pQ:循环队列指针
*
* Return(s)   : none
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
void vCirQueueGenericClear(CirQueueHandle_t xCirQueue);
/*********************************************************************************************************
*                                          xCirQueueGenericCreate
*
* Description :队列初始化，确认队列容量，申请内存。头尾和长度为0
*
* Argument(s) : queueCapacity:队列容量
*
* Return(s)   : 申请的队列指针
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
CirQueueHandle_t xCirQueueGenericCreate(const u32 uxQueueLength);
/*********************************************************************************************************
*                                          xCirQueueGenericStaticCreate
*
* Description :队列初始化，确认队列容量，静态内存。头尾和长度为0
*
* Argument(s) : uxQueueLength:队列容量 pxQueueBuffer :队列缓存指针
*
* Return(s)   : 申请的队列指针
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
CirQueueHandle_t	xCirQueueGenericStaticCreate(const u32 uxQueueLength,ElementType pxQueueBuffer[]);

#ifdef	QUEUE_HAVE_NODE
CirQueueHandle_t xCirQueueGenericDynamicCreate(int queueCapacity, int nodeNum);
#define vDynamicCirQueueCreate( xCirQueue,queueCapacity, nodeNum)											\
	{																																			\
		( xCirQueue ) = xCirQueueGenericDynamicCreate(queueCapacity, nodeNum);						\
	}
#endif
/*********************************************************************************************************
*                                          DestroyCirQueue
*
* Description :队列构销
*
* Argument(s) : pQ:循环队列指针
*
* Return(s)   : none
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
void DestroyCirQueue(CirQueueHandle_t xCirQueue);

#ifdef CIRQUEUE_SELF_TEST
void CirQueue_Self_Test(void);
#endif

#endif

