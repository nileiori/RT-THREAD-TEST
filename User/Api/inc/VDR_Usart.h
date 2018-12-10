#ifndef	VDR_USART_H
#define	VDR_USART_H

#include "stm32f10x.h"

#define VDR_USART_TX_PIN 					        GPIO_Pin_9
#define VDR_USART_RX_PIN 					        GPIO_Pin_10
#define VDR_USART_TX_PinSource 					  GPIO_PinSource9
#define VDR_USART_RX_PinSource 					  GPIO_PinSource10
#define VDR_USART_PORT                    GPIOA
#define VDR_USART_AF                      GPIO_AF_USART1
#define VDR_USART_RCC                     RCC_APB2Periph_USART1
#define VDR_USART_IRQn                    USART1_IRQn
#define VDR_USART                         USART1
#define VDR_USART_BAUDRATE				        115200


#define VDR_USART_RX_BUFFER_SIZE	1200 
#define VDR_USART_TX_BUFFER_SIZE	1200
#define VDR_TX_Q_BUFFER_SIZE      1200
#define VDR_TX_TEMP_BUFFER_SIZE   500

typedef struct
{
	u8	TxBuffer[VDR_USART_TX_BUFFER_SIZE];
	u16	TxLen;
	u16	TxCount;
	
	u8	RxBuffer[VDR_USART_RX_BUFFER_SIZE];
	u16	RxCount;
	u32	RxIdleCount;
	
}VDR_USART_STRUCT;


void VDR_Usart_thread_init(void);
void VDRUsart_Init(void);
void VDRUsart_Isr(void);
u16  VDRUsart_SendData(u8 *pBuffer, u16 BufferLen);

#endif
