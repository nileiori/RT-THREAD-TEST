/*************************************************************************
*
* Copyright (c) 2008,深圳市伊爱高新技术开发有限公司
* All rights reserved.
*
* 文件名称 : E2prom_25LC320A.h
* 功能     : E2prom_25LC320A的读写操作.
*
* 当前版本 : 1.0
* 开发者   : Senon Dou
* 修改时间 : 2013.07.26
*
* 历史版本 : 
* 开发者   : 
* 完成时间 : 
* 
* 备注 :
*************************************************************************/
#ifndef _E2prom_25LC320A_H_
#define _E2prom_25LC320A_H_

#include "stm32f10x.h"
#include "gpio.h"
#include "Rthw.h"

#define READ       0x03  /* Read data from memory array beginning at selected address */
#define WRITE      0x02  /* Write data to memory array beginning at selected address  */
#define WREN       0x06  /* Reset the write enable latch (disable write operations) */
#define EWDI       0x04  /* Set the write enable latch (enable write operations) */
#define RDSR       0x05  /* Read STATUS register */
#define WRSR       0x01  /* Write STATUS register */
#define E2PROM_WIP_BIT	0x01	//bit1
#define E2PROM_WEL_BIT	0x02	//bit1
#define E2PROM_WPEN_BIT	0x80	//bit7
#define E2PROM_BP1BP0_BIT	0x0C	//bit3,bit2

/* E2PROM 25LC320A Microwire 接口引脚  */  
#define E2PROM_25LC320A_CS_PIN          GPIO_Pin_4
#define E2PROM_25LC320A_CS_GPIO_PORT    GPIOG
#define E2PROM_25LC320A_CS_GPIO_CLK     RCC_APB2Periph_GPIOG
#define E2PROM_25LC320A_CS_LOW()       	gpio_out_off(PIN_NO_EPROM_CS);rt_hw_us_delay(30)
#define E2PROM_25LC320A_CS_HIGH()      	gpio_out_on(PIN_NO_EPROM_CS);	rt_hw_us_delay(30)
#define E2PROM_25LC320A_WP_HIGH()      	gpio_out_on(PIN_NO_MEM_WP);		rt_hw_us_delay(30)
#define E2PROM_25LC320A_WP_LOW()      	gpio_out_off(PIN_NO_MEM_WP);	rt_hw_us_delay(30)

void E2prom_InitIO(void);
void E2prom_ReadByte(u16 Address, u8 * destin, u8 bytenum);
u8 E2prom_WriteByte(u16 Address, u8 data);
u8 E2prom_ReadStatusRegister(void);
ErrorStatus CheckEepromChip(void);
#endif
