/*
 * File      : gpio.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2015, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author            Notes
 * 2015-03-24     Bright            the first version
 * 2016-05-23     Margguo@gmail.com Add  48 pins IC define
 * 2018-07-23     jiezhi320         Add GPIO Out_OD mode config
 */

#include <rthw.h>
#include <rtdevice.h>
#include <board.h>
#include "gpio.h"

#ifdef RT_USING_PIN

#define STM32F10X_PIN_NUMBERS 144 //[48, 64, 100, 144 ]

#define __STM32_PIN(index, rcc, gpio, gpio_index) { 0, RCC_##rcc##Periph_GPIO##gpio, GPIO##gpio, GPIO_Pin_##gpio_index, GPIO_PortSourceGPIO##gpio, GPIO_PinSource##gpio_index}
#define __STM32_PIN_DEFAULT {-1, 0, 0, 0, 0, 0}
#define __STM32_PIN_IRQ_DEFAULT	RT_NULL

/* STM32 GPIO driver */
struct pin_index
{
    int index;
    uint32_t rcc;
    GPIO_TypeDef *gpio;
    uint32_t pin;
    uint8_t port_source;
    uint8_t pin_source;
};

static const struct pin_index pins[] =
{
#if (STM32F10X_PIN_NUMBERS == 48)
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(2, APB2, C, 13),
    __STM32_PIN(3, APB2, C, 14),
    __STM32_PIN(4, APB2, C, 15),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(10, APB2, A, 0),
    __STM32_PIN(11, APB2, A, 1),
    __STM32_PIN(12, APB2, A, 2),
    __STM32_PIN(13, APB2, A, 3),
    __STM32_PIN(14, APB2, A, 4),
    __STM32_PIN(15, APB2, A, 5),
    __STM32_PIN(16, APB2, A, 6),
    __STM32_PIN(17, APB2, A, 7),
    __STM32_PIN(18, APB2, B, 0),
    __STM32_PIN(19, APB2, B, 1),
    __STM32_PIN(20, APB2, B, 2),
    __STM32_PIN(21, APB2, B, 10),
    __STM32_PIN(22, APB2, B, 11),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(25, APB2, B, 12),
    __STM32_PIN(26, APB2, B, 13),
    __STM32_PIN(27, APB2, B, 14),
    __STM32_PIN(28, APB2, B, 15),
    __STM32_PIN(29, APB2, A, 8),
    __STM32_PIN(30, APB2, A, 9),
    __STM32_PIN(31, APB2, A, 10),
    __STM32_PIN(32, APB2, A, 11),
    __STM32_PIN(33, APB2, A, 12),
    __STM32_PIN(34, APB2, A, 13),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(37, APB2, A, 14),
    __STM32_PIN(38, APB2, A, 15),
    __STM32_PIN(39, APB2, B, 3),
    __STM32_PIN(40, APB2, B, 4),
    __STM32_PIN(41, APB2, B, 5),
    __STM32_PIN(42, APB2, B, 6),
    __STM32_PIN(43, APB2, B, 7),
    __STM32_PIN_DEFAULT,
    __STM32_PIN(45, APB2, B, 8),
    __STM32_PIN(46, APB2, B, 9),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,

#endif
#if (STM32F10X_PIN_NUMBERS == 64)
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(2, APB2, C, 13),
    __STM32_PIN(3, APB2, C, 14),
    __STM32_PIN(4, APB2, C, 15),
    __STM32_PIN(5, APB2, D, 0),
    __STM32_PIN(6, APB2, D, 1),
    __STM32_PIN_DEFAULT,
    __STM32_PIN(8, APB2, C, 0),
    __STM32_PIN(9, APB2, C, 1),
    __STM32_PIN(10, APB2, C, 2),
    __STM32_PIN(11, APB2, C, 3),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(14, APB2, A, 0),
    __STM32_PIN(15, APB2, A, 1),
    __STM32_PIN(16, APB2, A, 2),
    __STM32_PIN(17, APB2, A, 3),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(20, APB2, A, 4),
    __STM32_PIN(21, APB2, A, 5),
    __STM32_PIN(22, APB2, A, 6),
    __STM32_PIN(23, APB2, A, 7),
    __STM32_PIN(24, APB2, C, 4),
    __STM32_PIN(25, APB2, C, 5),
    __STM32_PIN(26, APB2, B, 0),
    __STM32_PIN(27, APB2, B, 1),
    __STM32_PIN(28, APB2, B, 2),
    __STM32_PIN(29, APB2, B, 10),
    __STM32_PIN(30, APB2, B, 11),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(33, APB2, B, 12),
    __STM32_PIN(34, APB2, B, 13),
    __STM32_PIN(35, APB2, B, 14),
    __STM32_PIN(36, APB2, B, 15),
    __STM32_PIN(37, APB2, C, 6),
    __STM32_PIN(38, APB2, C, 7),
    __STM32_PIN(39, APB2, C, 8),
    __STM32_PIN(40, APB2, C, 9),
    __STM32_PIN(41, APB2, A, 8),
    __STM32_PIN(42, APB2, A, 9),
    __STM32_PIN(43, APB2, A, 10),
    __STM32_PIN(44, APB2, A, 11),
    __STM32_PIN(45, APB2, A, 12),
    __STM32_PIN(46, APB2, A, 13),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(49, APB2, A, 14),
    __STM32_PIN(50, APB2, A, 15),
    __STM32_PIN(51, APB2, C, 10),
    __STM32_PIN(52, APB2, C, 11),
    __STM32_PIN(53, APB2, C, 12),
    __STM32_PIN(54, APB2, D, 2),
    __STM32_PIN(55, APB2, B, 3),
    __STM32_PIN(56, APB2, B, 4),
    __STM32_PIN(57, APB2, B, 5),
    __STM32_PIN(58, APB2, B, 6),
    __STM32_PIN(59, APB2, B, 7),
    __STM32_PIN_DEFAULT,
    __STM32_PIN(61, APB2, B, 8),
    __STM32_PIN(62, APB2, B, 9),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
#endif
#if (STM32F10X_PIN_NUMBERS == 100)
    __STM32_PIN_DEFAULT,
    __STM32_PIN(1, APB2, E, 2),
    __STM32_PIN(2, APB2, E, 3),
    __STM32_PIN(3, APB2, E, 4),
    __STM32_PIN(4, APB2, E, 5),
    __STM32_PIN(5, APB2, E, 6),
    __STM32_PIN_DEFAULT,
    __STM32_PIN(7, APB2, C, 13),
    __STM32_PIN(8, APB2, C, 14),
    __STM32_PIN(9, APB2, C, 15),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(15, APB2, C, 0),
    __STM32_PIN(16, APB2, C, 1),
    __STM32_PIN(17, APB2, C, 2),
    __STM32_PIN(18, APB2, C, 3),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(23, APB2, A, 0),
    __STM32_PIN(24, APB2, A, 1),
    __STM32_PIN(25, APB2, A, 2),
    __STM32_PIN(26, APB2, A, 3),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(29, APB2, A, 4),
    __STM32_PIN(30, APB2, A, 5),
    __STM32_PIN(31, APB2, A, 6),
    __STM32_PIN(32, APB2, A, 7),
    __STM32_PIN(33, APB2, C, 4),
    __STM32_PIN(34, APB2, C, 5),
    __STM32_PIN(35, APB2, B, 0),
    __STM32_PIN(36, APB2, B, 1),
    __STM32_PIN(37, APB2, B, 2),
    __STM32_PIN(38, APB2, E, 7),
    __STM32_PIN(39, APB2, E, 8),
    __STM32_PIN(40, APB2, E, 9),
    __STM32_PIN(41, APB2, E, 10),
    __STM32_PIN(42, APB2, E, 11),
    __STM32_PIN(43, APB2, E, 12),
    __STM32_PIN(44, APB2, E, 13),
    __STM32_PIN(45, APB2, E, 14),
    __STM32_PIN(46, APB2, E, 15),
    __STM32_PIN(47, APB2, B, 10),
    __STM32_PIN(48, APB2, B, 11),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(51, APB2, B, 12),
    __STM32_PIN(52, APB2, B, 13),
    __STM32_PIN(53, APB2, B, 14),
    __STM32_PIN(54, APB2, B, 15),
    __STM32_PIN(55, APB2, D, 8),
    __STM32_PIN(56, APB2, D, 9),
    __STM32_PIN(57, APB2, D, 10),
    __STM32_PIN(58, APB2, D, 11),
    __STM32_PIN(59, APB2, D, 12),
    __STM32_PIN(60, APB2, D, 13),
    __STM32_PIN(61, APB2, D, 14),
    __STM32_PIN(62, APB2, D, 15),
    __STM32_PIN(63, APB2, C, 6),
    __STM32_PIN(64, APB2, C, 7),
    __STM32_PIN(65, APB2, C, 8),
    __STM32_PIN(66, APB2, C, 9),
    __STM32_PIN(67, APB2, A, 8),
    __STM32_PIN(68, APB2, A, 9),
    __STM32_PIN(69, APB2, A, 10),
    __STM32_PIN(70, APB2, A, 11),
    __STM32_PIN(71, APB2, A, 12),
    __STM32_PIN(72, APB2, A, 13),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(76, APB2, A, 14),
    __STM32_PIN(77, APB2, A, 15),
    __STM32_PIN(78, APB2, C, 10),
    __STM32_PIN(79, APB2, C, 11),
    __STM32_PIN(80, APB2, C, 12),
    __STM32_PIN(81, APB2, D, 0),
    __STM32_PIN(82, APB2, D, 1),
    __STM32_PIN(83, APB2, D, 2),
    __STM32_PIN(84, APB2, D, 3),
    __STM32_PIN(85, APB2, D, 4),
    __STM32_PIN(86, APB2, D, 5),
    __STM32_PIN(87, APB2, D, 6),
    __STM32_PIN(88, APB2, D, 7),
    __STM32_PIN(89, APB2, B, 3),
    __STM32_PIN(90, APB2, B, 4),
    __STM32_PIN(91, APB2, B, 5),
    __STM32_PIN(92, APB2, B, 6),
    __STM32_PIN(93, APB2, B, 7),
    __STM32_PIN_DEFAULT,
    __STM32_PIN(95, APB2, B, 8),
    __STM32_PIN(96, APB2, B, 9),
    __STM32_PIN(97, APB2, E, 0),
    __STM32_PIN(98, APB2, E, 1),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
#endif
#if (STM32F10X_PIN_NUMBERS == 144)
    __STM32_PIN_DEFAULT,
    __STM32_PIN(1, APB2, E, 2),
    __STM32_PIN(2, APB2, E, 3),
    __STM32_PIN(3, APB2, E, 4),
    __STM32_PIN(4, APB2, E, 5),
    __STM32_PIN(5, APB2, E, 6),
    __STM32_PIN_DEFAULT,
    __STM32_PIN(7, APB2, C, 13),
    __STM32_PIN(8, APB2, C, 14),
    __STM32_PIN(9, APB2, C, 15),

    __STM32_PIN(10, APB2, F, 0),
    __STM32_PIN(11, APB2, F, 1),
    __STM32_PIN(12, APB2, F, 2),
    __STM32_PIN(13, APB2, F, 3),
    __STM32_PIN(14, APB2, F, 4),
    __STM32_PIN(15, APB2, F, 5),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(18, APB2, F, 6),
    __STM32_PIN(19, APB2, F, 7),
    __STM32_PIN(20, APB2, F, 8),
    __STM32_PIN(21, APB2, F, 9),
    __STM32_PIN(22, APB2, F, 10),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(26, APB2, C, 0),
    __STM32_PIN(27, APB2, C, 1),
    __STM32_PIN(28, APB2, C, 2),
    __STM32_PIN(29, APB2, C, 3),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(34, APB2, A, 0),
    __STM32_PIN(35, APB2, A, 1),
    __STM32_PIN(36, APB2, A, 2),
    __STM32_PIN(37, APB2, A, 3),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(40, APB2, A, 4),
    __STM32_PIN(41, APB2, A, 5),
    __STM32_PIN(42, APB2, A, 6),
    __STM32_PIN(43, APB2, A, 7),
    __STM32_PIN(44, APB2, C, 4),
    __STM32_PIN(45, APB2, C, 5),
    __STM32_PIN(46, APB2, B, 0),
    __STM32_PIN(47, APB2, B, 1),
    __STM32_PIN(48, APB2, B, 2),
    __STM32_PIN(49, APB2, F, 11),
    __STM32_PIN(50, APB2, F, 12),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(53, APB2, F, 13),
    __STM32_PIN(54, APB2, F, 14),
    __STM32_PIN(55, APB2, F, 15),
    __STM32_PIN(56, APB2, G, 0),
    __STM32_PIN(57, APB2, G, 1),
    __STM32_PIN(58, APB2, E, 7),
    __STM32_PIN(59, APB2, E, 8),
    __STM32_PIN(60, APB2, E, 9),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(63, APB2, E, 10),
    __STM32_PIN(64, APB2, E, 11),
    __STM32_PIN(65, APB2, E, 12),
    __STM32_PIN(66, APB2, E, 13),
    __STM32_PIN(67, APB2, E, 14),
    __STM32_PIN(68, APB2, E, 15),
    __STM32_PIN(69, APB2, B, 10),
    __STM32_PIN(70, APB2, B, 11),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(73, APB2, B, 12),
    __STM32_PIN(74, APB2, B, 13),
    __STM32_PIN(75, APB2, B, 14),
    __STM32_PIN(76, APB2, B, 15),
    __STM32_PIN(77, APB2, D, 8),
    __STM32_PIN(78, APB2, D, 9),
    __STM32_PIN(79, APB2, D, 10),
    __STM32_PIN(80, APB2, D, 11),
    __STM32_PIN(81, APB2, D, 12),
    __STM32_PIN(82, APB2, D, 13),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(85, APB2, D, 14),
    __STM32_PIN(86, APB2, D, 15),
    __STM32_PIN(87, APB2, G, 2),
    __STM32_PIN(88, APB2, G, 3),
    __STM32_PIN(89, APB2, G, 4),
    __STM32_PIN(90, APB2, G, 5),
    __STM32_PIN(91, APB2, G, 6),
    __STM32_PIN(92, APB2, G, 7),
    __STM32_PIN(93, APB2, G, 8),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(96, APB2, C, 6),
    __STM32_PIN(97, APB2, C, 7),
    __STM32_PIN(98, APB2, C, 8),
    __STM32_PIN(99, APB2, C, 9),
    __STM32_PIN(100, APB2, A, 8),
    __STM32_PIN(101, APB2, A, 9),
    __STM32_PIN(102, APB2, A, 10),
    __STM32_PIN(103, APB2, A, 11),
    __STM32_PIN(104, APB2, A, 12),
    __STM32_PIN(105, APB2, A, 13),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(109, APB2, A, 14),
    __STM32_PIN(110, APB2, A, 15),
    __STM32_PIN(111, APB2, C, 10),
    __STM32_PIN(112, APB2, C, 11),
    __STM32_PIN(113, APB2, C, 12),
    __STM32_PIN(114, APB2, D, 0),
    __STM32_PIN(115, APB2, D, 1),
    __STM32_PIN(116, APB2, D, 2),
    __STM32_PIN(117, APB2, D, 3),
    __STM32_PIN(118, APB2, D, 4),
    __STM32_PIN(119, APB2, D, 5),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(122, APB2, D, 6),
    __STM32_PIN(123, APB2, D, 7),
    __STM32_PIN(124, APB2, G, 9),
    __STM32_PIN(125, APB2, G, 10),
    __STM32_PIN(126, APB2, G, 11),
    __STM32_PIN(127, APB2, G, 12),
    __STM32_PIN(128, APB2, G, 13),
    __STM32_PIN(129, APB2, G, 14),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
    __STM32_PIN(132, APB2, G, 15),
    __STM32_PIN(133, APB2, B, 3),
    __STM32_PIN(134, APB2, B, 4),
    __STM32_PIN(135, APB2, B, 5),
    __STM32_PIN(136, APB2, B, 6),
    __STM32_PIN(137, APB2, B, 7),
    __STM32_PIN_DEFAULT,
    __STM32_PIN(139, APB2, B, 8),
    __STM32_PIN(140, APB2, B, 9),
    __STM32_PIN(141, APB2, E, 0),
    __STM32_PIN(142, APB2, E, 1),
    __STM32_PIN_DEFAULT,
    __STM32_PIN_DEFAULT,
#endif
};

struct pin_irq_map
{
    rt_uint16_t            pinbit;
    rt_uint32_t            irqbit;
    enum IRQn              irqno;
};
static const  struct pin_irq_map pin_irq_map[] =
{
   {GPIO_Pin_0,  EXTI_Line0,  EXTI0_IRQn    },
   {GPIO_Pin_1,  EXTI_Line1,  EXTI1_IRQn    },
   {GPIO_Pin_2,  EXTI_Line2,  EXTI2_IRQn    },
   {GPIO_Pin_3,  EXTI_Line3,  EXTI3_IRQn    },
   {GPIO_Pin_4,  EXTI_Line4,  EXTI4_IRQn    },
   {GPIO_Pin_5,  EXTI_Line5,  EXTI9_5_IRQn  },
   {GPIO_Pin_6,  EXTI_Line6,  EXTI9_5_IRQn  },
   {GPIO_Pin_7,  EXTI_Line7,  EXTI9_5_IRQn  },
   {GPIO_Pin_8,  EXTI_Line8,  EXTI9_5_IRQn  },
   {GPIO_Pin_9,  EXTI_Line9,  EXTI9_5_IRQn  },
   {GPIO_Pin_10, EXTI_Line10, EXTI15_10_IRQn},
   {GPIO_Pin_11, EXTI_Line11, EXTI15_10_IRQn},
   {GPIO_Pin_12, EXTI_Line12, EXTI15_10_IRQn},
   {GPIO_Pin_13, EXTI_Line13, EXTI15_10_IRQn},
   {GPIO_Pin_14, EXTI_Line14, EXTI15_10_IRQn},
   {GPIO_Pin_15, EXTI_Line15, EXTI15_10_IRQn},
};
struct rt_pin_irq_hdr pin_irq_hdr_tab[] =
{
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},
    {-1, 0, RT_NULL, RT_NULL},                  
};

#define ITEM_NUM(items) sizeof(items)/sizeof(items[0])
const struct pin_index *get_pin(uint8_t pin)
{
    const struct pin_index *index;

    if (pin < ITEM_NUM(pins))
    {
        index = &pins[pin];
        if (index->index == -1)
            index = RT_NULL;
    }
    else
    {
        index = RT_NULL;
    }

    return index;
};

void stm32_pin_write(rt_device_t dev, rt_base_t pin, rt_base_t value)
{
    const struct pin_index *index;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return;
    }

    if (value == PIN_LOW)
    {
        GPIO_ResetBits(index->gpio, index->pin);
    }
    else
    {
        GPIO_SetBits(index->gpio, index->pin);
    }
}

int stm32_pin_read(rt_device_t dev, rt_base_t pin)
{
    int value;
    const struct pin_index *index;

    value = PIN_LOW;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return value;
    }

    if (GPIO_ReadInputDataBit(index->gpio, index->pin) == Bit_RESET)
    {
        value = PIN_LOW;
    }
    else
    {
        value = PIN_HIGH;
    }

    return value;
}

void stm32_pin_mode(rt_device_t dev, rt_base_t pin, rt_base_t mode)
{
    const struct pin_index *index;
    GPIO_InitTypeDef  GPIO_InitStructure;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return;
    }

    /* GPIO Periph clock enable */
    RCC_APB2PeriphClockCmd(index->rcc, ENABLE);

    /* Configure GPIO_InitStructure */
    GPIO_InitStructure.GPIO_Pin     = index->pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    if (mode == PIN_MODE_OUTPUT)
    {
        /* output setting */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    }
    else if (mode == PIN_MODE_OUTPUT_OD)
    {
        /* output setting: od. */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
    }		
		else if (mode == PIN_MODE_INPUT_ANL)
    {
        /* input setting: analog input . */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
    }
    else if (mode == PIN_MODE_INPUT)
    {
        /* input setting: not pull. */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        /* input setting: pull up. */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    }
    else
    {
        /* input setting:default. */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
    }
    GPIO_Init(index->gpio, &GPIO_InitStructure);
}

rt_inline rt_int32_t bit2bitno(rt_uint32_t bit)
{
    int i;
    for(i = 0; i < 32; i++)
    {
        if((0x01 << i) == bit)
        {
            return i;
        }
    }
    return -1;
}
rt_inline const struct pin_irq_map *get_pin_irq_map(uint32_t pinbit)
{
    rt_int32_t mapindex = bit2bitno(pinbit);
    if(mapindex < 0 || mapindex >= ITEM_NUM(pin_irq_map))
    {
        return RT_NULL;
    }
    return &pin_irq_map[mapindex];
};
rt_err_t stm32_pin_attach_irq(struct rt_device *device, rt_int32_t pin,
                  rt_uint32_t mode, void (*hdr)(void *args), void *args)
{
    const struct pin_index *index;
    rt_base_t level;
    rt_int32_t irqindex = -1;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return -RT_ENOSYS;
    }
    irqindex = bit2bitno(index->pin);
    if(irqindex < 0 || irqindex >= ITEM_NUM(pin_irq_map))
    {
        return -RT_ENOSYS;
    }

    level = rt_hw_interrupt_disable();
    if(pin_irq_hdr_tab[irqindex].pin == pin   &&
       pin_irq_hdr_tab[irqindex].hdr == hdr   &&
       pin_irq_hdr_tab[irqindex].mode == mode &&
       pin_irq_hdr_tab[irqindex].args == args
    )
    {
        rt_hw_interrupt_enable(level);
        return RT_EOK;
    }
    if(pin_irq_hdr_tab[irqindex].pin != -1)
    {
        rt_hw_interrupt_enable(level);
        return -RT_EBUSY;
    }
    pin_irq_hdr_tab[irqindex].pin = pin;
    pin_irq_hdr_tab[irqindex].hdr = hdr;
    pin_irq_hdr_tab[irqindex].mode = mode;
    pin_irq_hdr_tab[irqindex].args = args;
    rt_hw_interrupt_enable(level);
 
    return RT_EOK;
}
rt_err_t stm32_pin_detach_irq(struct rt_device *device, rt_int32_t pin)
{
    const struct pin_index *index;
    rt_base_t level;
    rt_int32_t irqindex = -1;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return -RT_ENOSYS;
    }
    irqindex = bit2bitno(index->pin);
    if(irqindex < 0 || irqindex >= ITEM_NUM(pin_irq_map))
    {
        return -RT_ENOSYS;
    }

    level = rt_hw_interrupt_disable();
    if(pin_irq_hdr_tab[irqindex].pin == -1)
    {
        rt_hw_interrupt_enable(level);
        return RT_EOK;
    }
    pin_irq_hdr_tab[irqindex].pin = -1;
    pin_irq_hdr_tab[irqindex].hdr = RT_NULL;
    pin_irq_hdr_tab[irqindex].mode = 0;
    pin_irq_hdr_tab[irqindex].args = RT_NULL;
    rt_hw_interrupt_enable(level);
 
    return RT_EOK;
}
rt_err_t stm32_pin_irq_enable(struct rt_device *device, rt_base_t pin,
                                                  rt_uint32_t enabled)
{
    const struct pin_index *index;
    const struct pin_irq_map *irqmap;
    rt_base_t level;
    rt_int32_t irqindex = -1;
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    index = get_pin(pin);
    if (index == RT_NULL)
    {
        return -RT_ENOSYS;
    }
    if(enabled == PIN_IRQ_ENABLE)
    {
        irqindex = bit2bitno(index->pin);
        if(irqindex < 0 || irqindex >= ITEM_NUM(pin_irq_map))
        {
            return -RT_ENOSYS;
        }
        level = rt_hw_interrupt_disable();
        if(pin_irq_hdr_tab[irqindex].pin == -1)
        {
            rt_hw_interrupt_enable(level);
            return -RT_ENOSYS;
        }
        irqmap = &pin_irq_map[irqindex];
        /* GPIO Periph clock enable */
        RCC_APB2PeriphClockCmd(index->rcc, ENABLE);
        /* Configure GPIO_InitStructure */
        GPIO_InitStructure.GPIO_Pin     = index->pin;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(index->gpio, &GPIO_InitStructure);

        NVIC_InitStructure.NVIC_IRQChannel= irqmap->irqno;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;  
        NVIC_InitStructure.NVIC_IRQChannelSubPriority= 2; 
        NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        GPIO_EXTILineConfig(index->port_source, index->pin_source);
        EXTI_InitStructure.EXTI_Line = irqmap->irqbit;  
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        switch(pin_irq_hdr_tab[irqindex].mode)
        {
        case PIN_IRQ_MODE_RISING:
          EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
          break;
        case PIN_IRQ_MODE_FALLING:
          EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
          break;
        case PIN_IRQ_MODE_RISING_FALLING:
          EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
          break;
        }
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
        EXTI_Init(&EXTI_InitStructure);
        rt_hw_interrupt_enable(level);
    }
    else if(enabled == PIN_IRQ_DISABLE)
    {
        irqmap = get_pin_irq_map(index->pin);
        if(irqmap == RT_NULL)
        {
            return -RT_ENOSYS;
        }
        EXTI_InitStructure.EXTI_Line = irqmap->irqbit;  
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
        EXTI_InitStructure.EXTI_LineCmd = DISABLE;  
        EXTI_Init(&EXTI_InitStructure);
    }
    else
    {
        return -RT_ENOSYS;
    }

    return RT_EOK;
}
const static struct rt_pin_ops _stm32_pin_ops =
{
    stm32_pin_mode,
    stm32_pin_write,
    stm32_pin_read,
    stm32_pin_attach_irq,
    stm32_pin_detach_irq,
    stm32_pin_irq_enable,
};

int stm32_hw_pin_init(void)
{
    int result;

    result = rt_device_pin_register("pin", &_stm32_pin_ops, RT_NULL);
    return result;
}
INIT_BOARD_EXPORT(stm32_hw_pin_init);

rt_inline void pin_irq_hdr(int irqno)
{
    EXTI_ClearITPendingBit(pin_irq_map[irqno].irqbit);
    if(pin_irq_hdr_tab[irqno].hdr)
    {
       pin_irq_hdr_tab[irqno].hdr(pin_irq_hdr_tab[irqno].args);
    }
}
#include "fct_driver.h"
void stm32f10x_pin_irq_hdr_num_01(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin1中断产生...\r\n");
#endif
	g_Pluse.count++;
}
void stm32f10x_pin_irq_hdr_num_02(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin2中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_03(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin3中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_04(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin4中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_05(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin5中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_06(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin6中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_07(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin7中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_08(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin8中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_09(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin9中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_10(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin10中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_11(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin11中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_12(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin12中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_13(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin13中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_14(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin14中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_15(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin15中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_16(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin16中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_17(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin17中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_18(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin18中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_19(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin19中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_20(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin20中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_21(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin21中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_22(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin22中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_23(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin23中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_24(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin24中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_25(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin25中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_26(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin26中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_27(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin27中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_28(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin28中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_29(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin29中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_30(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin30中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_31(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin31中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_32(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin32中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_33(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin33中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_34(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin34中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_35(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin35中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_36(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin36中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_37(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin37中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_38(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin38中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_39(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin39中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_40(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin40中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_41(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin41中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_42(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin42中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_43(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin43中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_44(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin44中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_45(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin45中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_46(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin46中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_47(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin47中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_48(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin48中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_49(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin49中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_50(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin50中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_51(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin51中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_52(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin52中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_53(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin53中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_54(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin54中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_55(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin55中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_56(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin56中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_57(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_58(void *args)
{
	u8 arg = *((u8*)args);	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin58中断产生...%d\r\n",arg);
#endif 
}
void stm32f10x_pin_irq_hdr_num_59(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_60(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_61(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_62(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_63(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_64(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_65(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_66(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_67(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_68(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_69(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_70(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_71(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_72(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_73(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_74(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_75(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_76(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_77(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_78(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_79(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_80(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_81(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_82(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_83(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_84(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_85(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_86(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_87(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_88(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_89(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_90(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_91(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_92(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_93(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_95(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_96(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_97(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_98(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_99(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_100(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
static void stm32f10x_pin_irq_hdr_num_101(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_102(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_103(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_104(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_105(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_109(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_110(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_111(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_112(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_113(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_114(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_115(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_116(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_117(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_118(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_119(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_122(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_123(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_124(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_125(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_126(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_127(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_128(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_129(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_132(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_133(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_134(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_135(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_136(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_137(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_139(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_140(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_141(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}
void stm32f10x_pin_irq_hdr_num_142(void *args)
{
	args = args;	
#ifdef	RT_USING_CONSOLE			
	rt_kprintf("pin中断产生...\r\n");
#endif 
}

static const stm32f10x_pin_hdr	stm32f10x_pin_hdr_tab[] =
{
#if (STM32F10X_PIN_NUMBERS == 48)
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_02,PC13),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_03,PC14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_04,PC15),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_10,PA0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_11,PA1),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_12,PA2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_13,PA3),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_14,PA4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_15,PA5),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_16,PA6),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_17,PA7),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_18,PB0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_19,PB1),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_20,PB2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_21,PB10),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_22,PB11),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_25,PB12),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_26,PB13),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_27,PB14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_28,PB15),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_29,PA8),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_30,PA9),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_31,PA10),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_32,PA11),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_33,PA12),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_34,PA13),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_37,PA14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_38,PA15),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_39,PB3),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_40,PB4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_41,PB5),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_42,PB6),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_43,PB7),
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_45,PB8),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_46,PB9),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,

#endif
#if (STM32F10X_PIN_NUMBERS == 64)
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_02,PC13),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_03,PC14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_04,PC15),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_05,PD0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_06,PD1),
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_08,PC0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_09,PC1),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_10,PC2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_11,PC3),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_14,PA0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_15,PA1),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_16,PA2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_17,PA3),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_20,PA4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_21,PA5),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_22,PA6),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_23,PA7),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_24,PC4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_25,PC5),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_26,PB0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_27,PB1),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_28,PB2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_29,PB10),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_30,PB11),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_33,PB12),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_34,PB13),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_35,PB14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_36,PB15),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_37,PC6),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_38,PC7),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_39,PC8),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_40,PC9),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_41,PA8),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_42,PA9),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_43,PA10),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_44,PA11),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_45,PA12),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_46,PB13),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_49,PA14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_50,PA15),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_51,PC10),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_52,PC11),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_53,PC12),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_54,PD2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_55,PB3),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_56,PB4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_57,PB5),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_58,PB6),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_59,PB7),
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_61,PB8),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_62,PB9),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
#endif
#if (STM32F10X_PIN_NUMBERS == 100)
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_01,PE2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_02,PE3),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_03,PE4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_04,PE5),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_05,PE6),
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_07,PC13),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_08,PC14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_09,PC15),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_15,PC0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_16,PC1),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_17,PC2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_18,PC3),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_23,PA0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_24,PA1),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_25,PA2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_26,PA3),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_29,PA4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_30,PA5),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_31,PA6),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_32,PA7),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_33,PC4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_34,PC5),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_35,PB0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_36,PB1),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_37,PB2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_38,PE7),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_39,PE8),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_40,PE9),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_41,PE10),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_42,PE11),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_43,PE12),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_44,PE13),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_45,PE14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_46,PE15),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_47,PB10),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_48,PB11),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_51,PB12),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_52,PB13),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_53,PB14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_54,PB15),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_55,PD8),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_56,PD9),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_57,PD10),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_58,PD11),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_59,PD12),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_60,PD13),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_61,PD14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_62,PD15),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_63,PC6),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_64,PC7),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_65,PC8),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_66,PC9),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_67,PA8),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_68,PA9),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_69,PA10),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_70,PA11),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_71,PA12),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_72,PA13),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_76,PA14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_77,PA15),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_78,PC10),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_79,PC11),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_80,PC12),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_81,PD0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_82,PD1),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_83,PD2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_84,PD3),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_85,PD4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_86,PD5),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_87,PD6),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_88,PD7),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_89,PB3),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_90,PB4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_91,PB5),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_92,PB6),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_93,PB7),
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_95,PB8),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_96,PB9),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_97,PE0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_98,PE1),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
#endif
#if (STM32F10X_PIN_NUMBERS == 144)
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_01,PE2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_02,PE3),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_03,PE4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_04,PE5),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_05,PE6),
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_07,PC13),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_08,PC14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_09,PC15),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_10,PF0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_11,PF1),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_12,PF2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_13,PF3),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_14,PF4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_15,PF5),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_18,PF6),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_19,PF7),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_20,PF8),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_21,PF9),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_22,PF10),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_26,PC0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_27,PC1),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_28,PC2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_29,PC3),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_34,PA0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_35,PA1),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_36,PA2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_37,PA3),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_40,PA4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_41,PA5),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_42,PA6),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_43,PA7),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_44,PC4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_45,PC5),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_46,PB0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_47,PB1),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_48,PB2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_49,PF11),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_50,PF12),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_53,PF13),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_54,PF14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_55,PF15),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_56,PG0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_57,PG1),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_58,PE7),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_59,PE8),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_60,PE9),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_63,PE10),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_64,PE11),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_65,PE12),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_66,PE13),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_67,PE14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_68,PE15),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_69,PB10),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_70,PB11),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_73,PB12),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_74,PB13),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_75,PB14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_76,PB15),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_77,PD8),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_78,PD9),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_79,PD10),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_80,PD11),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_81,PD12),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_82,PD13),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_85,PD14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_86,PD15),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_87,PG2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_88,PG3),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_89,PG4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_90,PG5),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_91,PG6),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_92,PG7),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_93,PG8),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_96,PC6),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_97,PC7),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_98,PC8),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_99,PC9),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_100,PA8),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_101,PA9),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_102,PA10),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_103,PA11),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_104,PA12),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_105,PA13),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_109,PA14),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_110,PA15),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_111,PC10),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_112,PC11),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_113,PC12),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_114,PD0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_115,PD1),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_116,PD2),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_117,PD3),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_118,PD4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_119,PD5),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_122,PD6),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_123,PD7),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_124,PG9),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_125,PG10),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_126,PG11),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_127,PG12),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_128,PG13),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_129,PG14),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_132,PG15),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_133,PB3),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_134,PB4),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_135,PB5),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_136,PB6),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_137,PB7),
    __STM32_PIN_IRQ_DEFAULT,
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_139,PB8),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_140,PB9),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_141,PE0),
    STM32F10X_IRQ_FUNCTION_ALIAS(stm32f10x_pin_irq_hdr_num_142,PE1),
    __STM32_PIN_IRQ_DEFAULT,
    __STM32_PIN_IRQ_DEFAULT,
#endif
};
/****************函数定义********************************************
//函数名称	:gpio_out_init
//功能		:初始化GPIO输出引脚
//输入		:pin:gpio管脚编号
//输出		:
//使用资源	:
//全局变量	:   
//调用函数	:
//中断资源	:  
//返回		:
//备注		:
*********************************************************************/
void gpio_out_init(rt_base_t pin)
{
  rt_pin_mode(pin, PIN_MODE_OUTPUT);
}
/****************函数定义********************************************
//函数名称	:gpio_out_on
//功能		:设置GPIO输出引脚为高(即数字1)
//输入		:pin:gpio管脚编号
//输出		:
//使用资源	:
//全局变量	:   
//调用函数	:
//中断资源	:  
//返回		:
//备注		:
*********************************************************************/
void gpio_out_on(rt_base_t pin)
{
  rt_pin_write(pin, PIN_HIGH);
}
/****************函数定义********************************************
//函数名称	:gpio_out_off
//功能		:设置GPIO输出引脚为低(即数字0)
//输入		:pin:gpio管脚编号
//输出		:
//使用资源	:
//全局变量	:   
//调用函数	:
//中断资源	:  
//返回		:
//备注		:
*********************************************************************/
void gpio_out_off(rt_base_t pin)
{
  rt_pin_write(pin, PIN_LOW);
}
/****************函数定义********************************************
//函数名称	:gpio_in_init
//功能		:初始化GPIO输入引脚
//输入		:pin:gpio管脚编号
//输出		:
//使用资源	:
//全局变量	:   
//调用函数	:
//中断资源	:  
//返回		:
//备注		:
*********************************************************************/
void gpio_in_init(rt_base_t pin)
{
  rt_pin_mode(pin, PIN_MODE_INPUT);
}
/****************函数定义********************************************
//函数名称	:gpio_pin_read
//功能		:获取输入引脚状态
//输入		:pin:gpio管脚编号
//输出		:
//使用资源	:
//全局变量	:   
//调用函数	:
//中断资源	:  
//返回		:当前IO口状态值,0或1
//备注		:
*********************************************************************/
int gpio_pin_read(rt_base_t pin)
{
  return rt_pin_read(pin);
}
/****************函数定义********************************************
//函数名称	:gpio_irq_install
//功能		:gpio中断装载函数
//输入		:pin:gpio管脚编号
					 pinMode:gpio管脚模式
					 irqMode:gpio中断模式
					 hdr:中断回调函数
					 args:中断回调函数参数 
//输出		:
//使用资源	:
//全局变量	:   
//调用函数	:
//中断资源	:  
//返回		:
//备注		:
*********************************************************************/
rt_err_t gpio_irq_install(rt_base_t pin, rt_base_t pinMode, rt_uint32_t irqMode,void  *args)
{
	rt_pin_mode(pin, pinMode);
	// 绑定中断
	rt_pin_attach_irq(pin, irqMode, stm32f10x_pin_hdr_tab[pin], args);
	// 使能中断
	return rt_pin_irq_enable(pin, PIN_IRQ_ENABLE);
}
rt_err_t gpio_irq_install_callback(rt_base_t pin, rt_base_t pinMode, void (*hdr)(void *args),rt_uint32_t irqMode,void  *args)
{
	rt_pin_mode(pin, pinMode);
	// 绑定中断
	rt_pin_attach_irq(pin, irqMode, hdr, args);
	// 使能中断
	return rt_pin_irq_enable(pin, PIN_IRQ_ENABLE);
}

void EXTI0_IRQHandler(void)
{
     /* enter interrupt */
    rt_interrupt_enter();
    pin_irq_hdr(0);
    /* leave interrupt */
    rt_interrupt_leave();
}
void EXTI1_IRQHandler(void)
{
     /* enter interrupt */
    rt_interrupt_enter();
    pin_irq_hdr(1);
    /* leave interrupt */
    rt_interrupt_leave();
}
void EXTI2_IRQHandler(void)
{
     /* enter interrupt */
    rt_interrupt_enter();
    pin_irq_hdr(2);
    /* leave interrupt */
    rt_interrupt_leave();
}
void EXTI3_IRQHandler(void)
{
     /* enter interrupt */
    rt_interrupt_enter();
    pin_irq_hdr(3);
    /* leave interrupt */
    rt_interrupt_leave();
}
void EXTI4_IRQHandler(void)
{
     /* enter interrupt */
    rt_interrupt_enter();
    pin_irq_hdr(4);
    /* leave interrupt */
    rt_interrupt_leave();
}
void EXTI9_5_IRQHandler(void)
{
     /* enter interrupt */
    rt_interrupt_enter();
    if(EXTI_GetITStatus(EXTI_Line5) != RESET)
    {
        pin_irq_hdr(5);
    }
    if(EXTI_GetITStatus(EXTI_Line6) != RESET)
    {
        pin_irq_hdr(6);
    }
    if(EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
        pin_irq_hdr(7);
    }
    if(EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
        pin_irq_hdr(8);
    }
    if(EXTI_GetITStatus(EXTI_Line9) != RESET)
    {
        pin_irq_hdr(9);
    }
    /* leave interrupt */
    rt_interrupt_leave();
}
void EXTI15_10_IRQHandler(void)
{
     /* enter interrupt */
    rt_interrupt_enter();
    if(EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
        pin_irq_hdr(10);
    }
    if(EXTI_GetITStatus(EXTI_Line11) != RESET)
    {
        pin_irq_hdr(11);
    }
    if(EXTI_GetITStatus(EXTI_Line12) != RESET)
    {
        pin_irq_hdr(12);
    }
    if(EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        pin_irq_hdr(13);
    }
    if(EXTI_GetITStatus(EXTI_Line14) != RESET)
    {
        pin_irq_hdr(14);
    }
    if(EXTI_GetITStatus(EXTI_Line15) != RESET)
    {
        pin_irq_hdr(15);
    }
    /* leave interrupt */
    rt_interrupt_leave();
}

#endif
