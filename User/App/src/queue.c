
/*
*********************************************************************************************************
*
*                                               算法
*                                             循环队列
*
*
* Filename      : queue.c
* Version       : V1.00
* Programmer(s) : dxl,由标准算法略微改编而成
*********************************************************************************************************
*/	
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "queue.h"
#include "string.h"
#include<stdlib.h>

/*
*********************************************************************************************************
*                                             define
*********************************************************************************************************
*/	

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      LOCAL GLOBAL VARIABLRS
*********************************************************************************************************
*/

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
Q_STATE IsCirQueueEmpty(CirQueueHandle_t xCirQueue)
{
	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;
	
  if (pxCirQueue->m_iQueueLenth == 0)
  {
      return Q_STATE_SUCCESS;
  }
  return Q_STATE_FAILED;
}

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
Q_STATE IsCirQueueFull(CirQueueHandle_t xCirQueue)
{
	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;
	
  if (pxCirQueue->m_iQueueLenth == pxCirQueue->m_iQueueCapacity)
  {
      return Q_STATE_SUCCESS;
  }
	return Q_STATE_FAILED;
}
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
int	GetCirQueueLength(CirQueueHandle_t xCirQueue)
{
	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;
	
	return	pxCirQueue->m_iQueueLenth;
}
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
Q_STATE EnCirQueue(ElementType element,CirQueueHandle_t xCirQueue)
{
	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;
	
  if (Q_STATE_SUCCESS == IsCirQueueFull(xCirQueue))
  {
      return Q_STATE_FAILED;
  }
  pxCirQueue->m_pQueue[pxCirQueue->m_iTail++] = element;
  //因为队列是环形，所以tail需要通过取模来实现转回到0位置
  pxCirQueue->m_iTail = pxCirQueue->m_iTail % pxCirQueue->m_iQueueCapacity;
  pxCirQueue->m_iQueueLenth++;
	
  return Q_STATE_SUCCESS;
}
/*********************************************************************************************************
*                                          BufferEnCirQueue
*
* Description : 多元素入队
*
* Argument(s) : pBuffer:待入队的数据指针 
*								BufferLen:入队数据长度
*								pQ:循环队列指针
*
* Return(s)   : 入队数据长度
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
int BufferEnCirQueue(ElementType *pBuffer, int BufferLen, CirQueueHandle_t xCirQueue)
{
	int	increaseLens;
	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;
	int residueLen ;//剩余容量
	int	preZeroLen ;//队尾置0前的长度
	
	residueLen = pxCirQueue->m_iQueueCapacity - pxCirQueue->m_iQueueLenth;//剩余容量
	preZeroLen = pxCirQueue->m_iQueueCapacity - pxCirQueue->m_iTail;//队尾置0前的长度
	
	if((NULL == pxCirQueue)||(0 == BufferLen))return 0;
	if(BufferLen > residueLen)
	{
		increaseLens = residueLen;
	}
	else
	{
		increaseLens = BufferLen;
	}
	if(preZeroLen >= increaseLens)
	{
		memcpy((void*)&pxCirQueue->m_pQueue[pxCirQueue->m_iTail],pBuffer,increaseLens);
	}
	else
	{
		memcpy((void*)&pxCirQueue->m_pQueue[pxCirQueue->m_iTail],pBuffer,preZeroLen);
		memcpy((void*)&pxCirQueue->m_pQueue[0],pBuffer+preZeroLen,increaseLens-preZeroLen);//补上余下的
	}
	pxCirQueue->m_iTail += increaseLens;
  //因为队列是环形，所以tail需要通过取模来实现转回到0位置
  pxCirQueue->m_iTail = pxCirQueue->m_iTail % pxCirQueue->m_iQueueCapacity;
  pxCirQueue->m_iQueueLenth += increaseLens;
  
  return increaseLens;
}
/*********************************************************************************************************
*                                          DeCirQueue
*
* Description : 单个元素出队
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
Q_STATE DeCirQueue(ElementType *element,CirQueueHandle_t xCirQueue)
{
	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;
  if (Q_STATE_SUCCESS == IsCirQueueEmpty(xCirQueue))
  {
      return Q_STATE_FAILED;
  }
  *element = pxCirQueue->m_pQueue[pxCirQueue->m_iHead++];
  //因为队列是环形，所以head需要通过取模来实现转回到0位置
  pxCirQueue->m_iHead = pxCirQueue->m_iHead % pxCirQueue->m_iQueueCapacity;
  pxCirQueue->m_iQueueLenth--;  
	
  return Q_STATE_SUCCESS;

}
/*********************************************************************************************************
*                                          BufferDeCirQueue
*
* Description : 多元素出队
*
* Argument(s) : pBuffer:待出队的数据指针 
*								BufferLen:出队数据长度
*								pQ:循环队列指针
*
* Return(s)   : 出队数据长度
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
int BufferDeCirQueue(ElementType *pBuffer, int BufferLen, CirQueueHandle_t xCirQueue)
{
	int	returnLength;
	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;
  int	preZeroLen = pxCirQueue->m_iQueueCapacity - pxCirQueue->m_iHead;//队头置0前的长度
  
  if (Q_STATE_SUCCESS == IsCirQueueEmpty(xCirQueue))return 0;
  if((NULL == pxCirQueue)||(0 == BufferLen))return 0;
	
	if(BufferLen > pxCirQueue->m_iQueueLenth)
	{
		returnLength = pxCirQueue->m_iQueueLenth;
	}
	else
	{
		returnLength = BufferLen;
	}
 	if(preZeroLen >= returnLength)
	{
		memcpy(pBuffer,&pxCirQueue->m_pQueue[pxCirQueue->m_iHead],returnLength);
	}
	else
	{
		memcpy(pBuffer,&pxCirQueue->m_pQueue[pxCirQueue->m_iHead],preZeroLen);
		memcpy(pBuffer+preZeroLen,&pxCirQueue->m_pQueue[0],returnLength-preZeroLen);//补上余下的
	}
  
	pxCirQueue->m_iHead += returnLength;
  //因为队列是环形，所以head需要通过取模来实现转回到0位置
  pxCirQueue->m_iHead = pxCirQueue->m_iHead % pxCirQueue->m_iQueueCapacity;
  pxCirQueue->m_iQueueLenth -= returnLength; 
	
	return returnLength;
}
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
int AllBufferDeCirQueue(ElementType *pBuffer,CirQueueHandle_t xCirQueue)
{

	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;
  int	preZeroLen = pxCirQueue->m_iQueueCapacity - pxCirQueue->m_iHead;//队头置0前的长度

	if((NULL == pxCirQueue)||(NULL == pBuffer))return 0;
	if (Q_STATE_SUCCESS == IsCirQueueEmpty(xCirQueue))return 0;
	
	if(preZeroLen > pxCirQueue->m_iQueueLenth)
	{
		memcpy(pBuffer,&pxCirQueue->m_pQueue[pxCirQueue->m_iHead],pxCirQueue->m_iQueueLenth);
	}
	else
	{
		memcpy(pBuffer,&pxCirQueue->m_pQueue[pxCirQueue->m_iHead],preZeroLen);
		memcpy(pBuffer+preZeroLen,&pxCirQueue->m_pQueue[0],pxCirQueue->m_iQueueLenth-preZeroLen);//补上余下的
	}
  
	pxCirQueue->m_iHead += pxCirQueue->m_iQueueLenth;
  //因为队列是环形，所以head需要通过取模来实现转回到0位置
  pxCirQueue->m_iHead = pxCirQueue->m_iHead % pxCirQueue->m_iQueueCapacity;
  pxCirQueue->m_iQueueLenth = 0; 
	
	return pxCirQueue->m_iQueueLenth;
}
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
CirQueueHandle_t	xCirQueueGenericCreate(const u32 uxQueueLength)
{
	CirQueue_t * pxCirQueue;

	if(uxQueueLength < MIN_QUEUE_SIZE)
			return NULL;

	pxCirQueue = (CirQueue_t *)malloc(sizeof(CirQueue_t));
	if( pxCirQueue == NULL )
		return NULL;
	pxCirQueue->m_iQueueCapacity = uxQueueLength;
	pxCirQueue->m_iHead = 0;
	pxCirQueue->m_iTail = 0;
	pxCirQueue->m_iQueueLenth = 0;
	pxCirQueue->m_pQueue = (ElementType*)malloc(uxQueueLength*sizeof(ElementType));
	if( pxCirQueue->m_pQueue == NULL )
		return NULL;
	memset(pxCirQueue->m_pQueue,0,uxQueueLength);
	
	return pxCirQueue;
}
/*********************************************************************************************************
*                                          xCirQueueGenericStaticCreate
*
* Description :队列初始化，确认队列容量，申请内存。头尾和长度为0
*
* Argument(s) : uxQueueLength:队列容量 pxQueueBuffer :队列缓存指针
*
* Return(s)   : 申请的队列指针
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
CirQueueHandle_t	xCirQueueGenericStaticCreate(const u32 uxQueueLength,ElementType pxQueueBuffer[])
{
	CirQueue_t * pxCirQueue;

	if(uxQueueLength < MIN_QUEUE_SIZE)
			return NULL;

	pxCirQueue = (CirQueue_t *)malloc(sizeof(CirQueue_t));
	if( pxCirQueue == NULL )
		return NULL;
	pxCirQueue->m_iQueueCapacity = uxQueueLength;
	pxCirQueue->m_iHead = 0;
	pxCirQueue->m_iTail = 0;
	pxCirQueue->m_iQueueLenth = 0;
	pxCirQueue->m_pQueue = pxQueueBuffer;
	memset(pxCirQueue->m_pQueue,0,uxQueueLength);
	
	return pxCirQueue;
}

#ifdef	QUEUE_HAVE_NODE
CirQueueHandle_t xCirQueueGenericDynamicCreate(int queueCapacity,int nodeNum)
{
	CirQueue_t * pxCirQueue;

	if(queueCapacity < MIN_QUEUE_SIZE)
      return NULL;

  pxCirQueue = (CirQueue_t *)malloc(sizeof(CirQueue_t));
  if( pxCirQueue == NULL )
  	return NULL;
  pxCirQueue->m_iQueueCapacity 	= queueCapacity;
  pxCirQueue->m_iHead 					= 0;
  pxCirQueue->m_iTail 					= 0;
  pxCirQueue->m_iQueueLenth 		= 0;
	pxCirQueue->m_iNodeHead 			= 0;
	pxCirQueue->m_iNodeTail 			= 0;
	pxCirQueue->m_iNodeNum 				= 0;
	pxCirQueue->m_pQueue = (ElementType*)malloc(queueCapacity*sizeof(ElementType));
	if( pxCirQueue->m_pQueue == NULL )
		return NULL;
	pxCirQueue->m_pNode = (Q_NODE*)malloc(nodeNum*sizeof(Q_NODE));
	if( pxCirQueue->m_pNode == NULL )
		return NULL;
	pxCirQueue->m_iMaxNodeNum = nodeNum;
	pxCirQueue->m_pNode->lens = 0;
	
	return pxCirQueue;
}
Q_STATE eCirQueueGenericStaticCreate(int queueCapacity,
																									int nodeNum,
																									CirQueue_t *pCirQueue,
																									ElementType *pQBuffer,
																									Q_NODE *pNodeBuffer)
{
	
	if(queueCapacity < MIN_QUEUE_SIZE)return Q_STATE_FAILED;

  memset((void*)pCirQueue,0,sizeof(CirQueue_t));

  pCirQueue->m_iQueueCapacity 	= queueCapacity;
	pCirQueue->m_pQueue 					= pQBuffer;
	pCirQueue->m_pNode 					= pNodeBuffer;
	pCirQueue->m_iMaxNodeNum 		= nodeNum;
	pCirQueue->m_pNode->lens 		= 0;

	return Q_STATE_SUCCESS;
}

/*********************************************************************************************************
*                                          BufferNodeEnCirQueue
*
* Description : 节点入队
*
* Argument(s) : pBuffer:待入队的数据指针 
*								BufferLen:入队数据长度
*								pQ:循环队列指针
*
* Return(s)   : 入队数据长度
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
int BufferNodeEnCirQueue(ElementType *pBuffer, int BufferLen, CirQueueHandle_t xCirQueue)
{
	int	increaseLens;
	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;
	int residueLen = pxCirQueue->m_iQueueCapacity - pxCirQueue->m_iQueueLenth;//剩余容量
	int	preZeroLen = pxCirQueue->m_iQueueCapacity - pxCirQueue->m_iTail;//队尾置0前的长度

	Q_NODE *pNode;
	
	if((NULL == pxCirQueue)||(0 == BufferLen))return 0;
	if((pxCirQueue->m_iNodeNum+1) >= pxCirQueue->m_iMaxNodeNum)
  {
      return 0;//节点缓存已满
  }

	pNode = pxCirQueue->m_pNode + pxCirQueue->m_iNodeTail;
	
	if(BufferLen > residueLen)
	{
		increaseLens = residueLen;
	}
	else
	{
		increaseLens = BufferLen;
	}
	
	if(preZeroLen >= increaseLens)
	{
		memcpy((void*)&pxCirQueue->m_pQueue[pxCirQueue->m_iTail],pBuffer,increaseLens);
	}
	else
	{
		memcpy((void*)&pxCirQueue->m_pQueue[pxCirQueue->m_iTail],pBuffer,preZeroLen);
		memcpy((void*)&pxCirQueue->m_pQueue[0],pBuffer+preZeroLen,increaseLens-preZeroLen);//补上余下的
	}
	pxCirQueue->m_iTail += increaseLens;
  //因为队列是环形，所以tail需要通过取模来实现转回到0位置
  pxCirQueue->m_iTail = pxCirQueue->m_iTail % pxCirQueue->m_iQueueCapacity;
  pxCirQueue->m_iQueueLenth += increaseLens;

	pNode->lens = increaseLens;
	pxCirQueue->m_iNodeTail += 1;//节点尾加1
  pxCirQueue->m_iNodeTail %= pxCirQueue->m_iMaxNodeNum;
  pxCirQueue->m_iNodeNum += 1;//节点个数加1
  
  return increaseLens;
}
/*********************************************************************************************************
*                                          BufferNodeDeCirQueue
*
* Description : 节点出队
*
* Argument(s) : pBuffer:待出队的数据指针 
*								BufferLen:出队数据长度
*								pQ:循环队列指针
*
* Return(s)   : 出队数据长度
*
* Caller(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************/
int BufferNodeDeCirQueue(ElementType *pBuffer,CirQueueHandle_t xCirQueue)
{
	int deBufferLen;
	CirQueue_t * const pxCirQueue = (CirQueue_t *)xCirQueue;
  int	preZeroLen = pxCirQueue->m_iQueueCapacity - pxCirQueue->m_iHead;//队头置0前的长度

	Q_NODE *pNode;
	
  if (Q_STATE_SUCCESS == IsCirQueueEmpty(xCirQueue))return 0;
  if(NULL == pxCirQueue)return 0;

	if(0==pxCirQueue->m_iNodeNum)return 0;//说明没有数据
	
	//出队数据信息
	pNode = pxCirQueue->m_pNode + pxCirQueue->m_iNodeHead;//节点地址
  deBufferLen   = pNode->lens;
	
 	if(preZeroLen >= deBufferLen)
	{
		memcpy(pBuffer,&pxCirQueue->m_pQueue[pxCirQueue->m_iHead],deBufferLen);
	}
	else
	{
		memcpy(pBuffer,&pxCirQueue->m_pQueue[pxCirQueue->m_iHead],preZeroLen);
		memcpy(pBuffer+preZeroLen,&pxCirQueue->m_pQueue[0],deBufferLen-preZeroLen);//补上余下的
	}
  
	pxCirQueue->m_iHead += deBufferLen;
  //因为队列是环形，所以head需要通过取模来实现转回到0位置
  pxCirQueue->m_iHead = pxCirQueue->m_iHead % pxCirQueue->m_iQueueCapacity;
  pxCirQueue->m_iQueueLenth -= deBufferLen; 

	pxCirQueue->m_iNodeHead += 1;         //节点头加1
  pxCirQueue->m_iNodeHead %= pxCirQueue->m_iMaxNodeNum;
  pxCirQueue->m_iNodeNum -= 1;         //节点个数减1
  
  pNode->lens = 0;  //销毁长度
  
	return deBufferLen;
}
#endif
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
void vCirQueueGenericClear(CirQueueHandle_t xCirQueue)
{
	CirQueue_t * pxCirQueue = (CirQueue_t *)xCirQueue;
	if( pxCirQueue != NULL )
  {
    pxCirQueue->m_iHead = 0;
		pxCirQueue->m_iTail = 0;
		pxCirQueue->m_iQueueLenth = 0;
		
		#ifdef	QUEUE_HAVE_NODE
    
		#endif
  }
}

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
void DestroyCirQueue(CirQueueHandle_t xCirQueue)
{
	CirQueue_t * pxCirQueue = (CirQueue_t *)xCirQueue;
	if( pxCirQueue != NULL )
  {
    free( pxCirQueue->m_pQueue);pxCirQueue->m_pQueue = NULL;
		#ifdef	QUEUE_HAVE_NODE
    free( pxCirQueue->m_pNode);pxCirQueue->m_pNode = NULL;
		#endif
		free( pxCirQueue );pxCirQueue = NULL;
  }
}
#ifdef CIRQUEUE_SELF_TEST
static CirQueueHandle_t	pQ_Self_Test;
ElementType	InBuffer1[10] = {1,2,6,7,8,9,8,11,12,70};
ElementType	InBuffer2[10] = {4,5,6,7,8,4,10,11,12,0};
ElementType	InBuffer3[10] = {3,9,6,7,8,9,10,11,12,1};
ElementType	OutBuffer1[10];
ElementType QBuffer[100];
Q_NODE 			NodeBuffer[10];
CirQueue_t	Q_Static;

void CirQueue_Self_Test(void)
{
	ElementType	InBuffer[10] = {4,5,6,7,8,9,10,11,12,13};
	ElementType	OutBuffer[20];
	ElementType	el;
	
	CirQueueHandle_t CircleQueue = xCirQueueGenericCreate(20);

	if(NULL == CircleQueue)return;
	
	EnCirQueue(1,CircleQueue);
	EnCirQueue(2,CircleQueue);
	EnCirQueue(3,CircleQueue);

	BufferEnCirQueue(InBuffer,10,CircleQueue);
	//BufferDeCirQueue(OutBuffer,10,CircleQueue);
	EnCirQueue(6,CircleQueue);
	EnCirQueue(7,CircleQueue);
	EnCirQueue(8,CircleQueue);
	DeCirQueue(&el,CircleQueue);
	DeCirQueue(&el,CircleQueue);
	DeCirQueue(&el,CircleQueue);
	BufferDeCirQueue(OutBuffer, 10, CircleQueue);
	BufferEnCirQueue(InBuffer,10,CircleQueue);
  AllBufferDeCirQueue(OutBuffer,CircleQueue);
#ifdef	QUEUE_HAVE_NODE
	vDynamicCirQueueCreate(pQ_Self_Test,100,5);
	BufferNodeEnCirQueue(InBuffer1,8,pQ_Self_Test);
	BufferNodeEnCirQueue(InBuffer2,9,pQ_Self_Test);
	BufferNodeEnCirQueue(InBuffer3,10,pQ_Self_Test);
	BufferNodeDeCirQueue(OutBuffer1,pQ_Self_Test);
	BufferNodeDeCirQueue(OutBuffer1,pQ_Self_Test);
	BufferNodeDeCirQueue(OutBuffer1,pQ_Self_Test);
	BufferNodeDeCirQueue(OutBuffer1,pQ_Self_Test);
	
	if(Q_STATE_FAILED == eCirQueueGenericStaticCreate(100,
																									5,
																									&Q_Static,
																									QBuffer,
																									NodeBuffer))
		return;
	BufferNodeEnCirQueue(InBuffer1,8,&Q_Static);
	BufferNodeEnCirQueue(InBuffer2,9,&Q_Static);
	BufferNodeEnCirQueue(InBuffer3,10,&Q_Static);
	BufferNodeDeCirQueue(OutBuffer1,&Q_Static);
	BufferNodeDeCirQueue(OutBuffer1,&Q_Static);
	BufferNodeDeCirQueue(OutBuffer1,&Q_Static);
#endif
}
#endif


