/*
 * File      : usart.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

#ifndef __USART_H__
#define __USART_H__

#include <rthw.h>
#include <rtthread.h>

#define UART_ENABLE_IRQ(n)            NVIC_EnableIRQ((n))
#define UART_DISABLE_IRQ(n)           NVIC_DisableIRQ((n))

//extern struct rt_serial_device serial1;
void rt_hw_usart_init(void);
//读串口1数据
rt_size_t stm32_serial1_read(rt_off_t pos,void* buffer,rt_size_t size);
//写串口1数据
rt_size_t stm32_serial1_write(rt_off_t pos,void* buffer,rt_size_t size);
//读串口5数据
rt_size_t stm32_serial5_read(rt_off_t pos,void* buffer,rt_size_t size);
//写串口5数据
rt_size_t stm32_serial5_write(rt_off_t pos,void* buffer,rt_size_t size);

#endif
