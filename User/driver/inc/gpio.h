/*
 * File      : gpio.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2015, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-01-05     Bernard      the first version
 */
#ifndef GPIO_H__
#define GPIO_H__

#include "Rtdef.h"
#include "stm32f10x.h"
   
 
//****************宏定义********************
   
//**************输出脚*******************
#define DLIGHTI_PIN                     GPIO_Pin_13
#define DLIGHTI_GPIO_PORT               GPIOE
#define DLIGHTI_GPIO_CLK                RCC_APB2Periph_GPIOE

#define LLIGHTI_PIN                     GPIO_Pin_10
#define LLIGHTI_GPIO_PORT               GPIOE
#define LLIGHTI_GPIO_CLK                RCC_APB2Periph_GPIOE
   
#define BREAK_PIN                       GPIO_Pin_8
#define BREAK_GPIO_PORT                 GPIOE
#define BREAK_GPIO_CLK                  RCC_APB2Periph_GPIOE   
   
#define ACC_PIN                         GPIO_Pin_7
#define ACC_GPIO_PORT                   GPIOE
#define ACC_GPIO_CLK                    RCC_APB2Periph_GPIOE

#define DOORI_N_PIN                         GPIO_Pin_5
#define DOORI_N_GPIO_PORT                   GPIOC
#define DOORI_N_GPIO_CLK                    RCC_APB2Periph_GPIOC

#define DOORI_P_PIN                         GPIO_Pin_1
#define DOORI_P_GPIO_PORT                   GPIOG
#define DOORI_P_GPIO_CLK                    RCC_APB2Periph_GPIOG

#define DOORI2_PIN                         GPIO_Pin_15//dxl,2013.11.5,原来是PE15,错了是PF15
#define DOORI2_GPIO_PORT                   GPIOF
#define DOORI2_GPIO_CLK                    RCC_APB2Periph_GPIOF

#define LAMP_PIN                            GPIO_Pin_4
#define LAMP_GPIO_PORT                      GPIOC
#define LAMP_GPIO_CLK                       RCC_APB2Periph_GPIOC  

#define LOCK_OUT_PIN                         GPIO_Pin_0
#define LOCK_OUT_GPIO_PORT                   GPIOB
#define LOCK_OUT_GPIO_CLK                    RCC_APB2Periph_GPIOB     

#define UNLOCK_OUT_PIN                         GPIO_Pin_1
#define UNLOCK_OUT_GPIO_PORT                   GPIOB
#define UNLOCK_OUT_GPIO_CLK                    RCC_APB2Periph_GPIOB    
   
#define XLIGHTI_PIN                         GPIO_Pin_14
#define XLIGHTI_GPIO_PORT                   GPIOE
#define XLIGHTI_GPIO_CLK                    RCC_APB2Periph_GPIOE    

#define RLIGHTI_PIN                         GPIO_Pin_12
#define RLIGHTI_GPIO_PORT                   GPIOE
#define RLIGHTI_GPIO_CLK                    RCC_APB2Periph_GPIOE    


#define BKHI_PIN                         GPIO_Pin_9
#define BKHI_GPIO_PORT                   GPIOE
#define BKHI_GPIO_CLK                    RCC_APB2Periph_GPIOE     

#define SWITCHI_PIN                GPIO_Pin_14
#define SWITCHI_GPIO_PORT          GPIOF
#define SWITCHI_GPIO_CLK           RCC_APB2Periph_GPIOF  
   
#define HORNI_PIN                GPIO_Pin_0
#define HORNI_GPIO_PORT          GPIOG
#define HORNI_GPIO_CLK           RCC_APB2Periph_GPIOG    

#define VIRBRATIONI_PIN                GPIO_Pin_13
#define VIRBRATIONI_GPIO_PORT          GPIOF
#define VIRBRATIONI_GPIO_CLK           RCC_APB2Periph_GPIOF      

#define COIL_PIN                GPIO_Pin_2
#define COIL_GPIO_PORT          GPIOB
#define COIL_GPIO_CLK           RCC_APB2Periph_GPIOB   

#define HORN_OUT_PIN                GPIO_Pin_11
#define HORN_OUT_GPIO_PORT          GPIOF
#define HORN_OUT_GPIO_CLK           RCC_APB2Periph_GPIOF   

#define DETCT1I_PIN                GPIO_Pin_11
#define DETCT1I_GPIO_PORT          GPIOB
#define DETCT1I_GPIO_CLK           RCC_APB2Periph_GPIOB  
   
#define DETCT2I_PIN                GPIO_Pin_15
#define DETCT2I_GPIO_PORT          GPIOE
#define DETCT2I_GPIO_CLK           RCC_APB2Periph_GPIOE     


#define GPS_OC_PIN                GPIO_Pin_2
#define GPS_OC_GPIO_PORT          GPIOF
#define GPS_OC_GPIO_CLK           RCC_APB2Periph_GPIOF     

#define GPS_SC_PIN                GPIO_Pin_13
#define GPS_SC_GPIO_PORT          GPIOC
#define GPS_SC_GPIO_CLK           RCC_APB2Periph_GPIOC    

#define GPS_CTR_PIN                GPIO_Pin_6
#define GPS_CTR_GPIO_PORT          GPIOE
#define GPS_CTR_GPIO_CLK           RCC_APB2Periph_GPIOE      

#define MEM_WP_PIN                GPIO_Pin_15
#define MEM_WP_GPIO_PORT          GPIOA
#define MEM_WP_GPIO_CLK           RCC_APB2Periph_GPIOA    
   
#define FLASH_CS_PIN                GPIO_Pin_4
#define FLASH_CS_GPIO_PORT          GPIOA
#define FLASH_CS_GPIO_CLK           RCC_APB2Periph_GPIOA      

#define FR_CS_PIN                GPIO_Pin_8
#define FR_CS_GPIO_PORT          GPIOG
#define FR_CS_GPIO_CLK           RCC_APB2Periph_GPIOG   

#define EPROM_CS_PIN                GPIO_Pin_4
#define EPROM_CS_GPIO_PORT          GPIOG
#define EPROM_CS_GPIO_CLK           RCC_APB2Periph_GPIOG  

#define GSM_ON_PIN                GPIO_Pin_4
#define GSM_ON_GPIO_PORT          GPIOD
#define GSM_ON_GPIO_CLK           RCC_APB2Periph_GPIOD  
   
#define GSM_PWC_PIN                GPIO_Pin_10
#define GSM_PWC_GPIO_PORT          GPIOC
#define GSM_PWC_GPIO_CLK           RCC_APB2Periph_GPIOC    
   
#define GSM_DTR_PIN                GPIO_Pin_5
#define GSM_DTR_GPIO_PORT          GPIOG
#define GSM_DTR_GPIO_CLK           RCC_APB2Periph_GPIOG    
   
#define GSM_RI_PIN                GPIO_Pin_13
#define GSM_RI_GPIO_PORT          GPIOG
#define GSM_RI_GPIO_CLK           RCC_APB2Periph_GPIOG     
   
#define CAM_POWER_PIN                GPIO_Pin_12
#define CAM_POWER_GPIO_PORT          GPIOF
#define CAM_POWER_GPIO_CLK           RCC_APB2Periph_GPIOF       
   
#define CAM_RD_PIN                GPIO_Pin_10
#define CAM_RD_GPIO_PORT          GPIOD
#define CAM_RD_GPIO_CLK           RCC_APB2Periph_GPIOD       
   
#define TTS_RDY_PIN                GPIO_Pin_6
#define TTS_RDY_GPIO_PORT          GPIOG
#define TTS_RDY_GPIO_CLK           RCC_APB2Periph_GPIOG   

#define TTS_P_PIN                GPIO_Pin_7
#define TTS_P_GPIO_PORT          GPIOG
#define TTS_P_GPIO_CLK           RCC_APB2Periph_GPIOG     
  

#define LCD_ON_PIN                GPIO_Pin_11
#define LCD_ON_GPIO_PORT          GPIOC
#define LCD_ON_GPIO_CLK           RCC_APB2Periph_GPIOC      

#define LCD_CS_PIN                GPIO_Pin_15
#define LCD_CS_GPIO_PORT          GPIOG
#define LCD_CS_GPIO_CLK           RCC_APB2Periph_GPIOG    

#define LCD_RST_PIN                GPIO_Pin_8
#define LCD_RST_GPIO_PORT          GPIOA
#define LCD_RST_GPIO_CLK           RCC_APB2Periph_GPIOA    
   
#define LCD_AO_PIN                GPIO_Pin_3
#define LCD_AO_GPIO_PORT          GPIOD
#define LCD_AO_GPIO_CLK           RCC_APB2Periph_GPIOD    
   
#define LCD_SPI_SCK_PIN                GPIO_Pin_0
#define LCD_SPI_SCK_GPIO_PORT          GPIOE
#define LCD_SPI_SCK_GPIO_CLK           RCC_APB2Periph_GPIOE       

#define LCD_SPI_MISO_PIN                GPIO_Pin_8
#define LCD_SPI_MISO_GPIO_PORT          GPIOC
#define LCD_SPI_MISO_GPIO_CLK           RCC_APB2Periph_GPIOC 
   
#define LCD_SPI_MOSI_PIN                GPIO_Pin_1
#define LCD_SPI_MOSI_GPIO_PORT          GPIOE
#define LCD_SPI_MOSI_GPIO_CLK           RCC_APB2Periph_GPIOE    

#define CAN_CTR_MOSI_PIN                GPIO_Pin_15
#define CAN_CTR_MOSI_GPIO_PORT          GPIOD
#define CAN_CTR_MOSI_GPIO_CLK           RCC_APB2Periph_GPIOD    

#define SHOCK_CS_MOSI_PIN                GPIO_Pin_7
#define SHOCK_CS_MOSI_GPIO_PORT          GPIOD
#define SHOCK_CS_MOSI_GPIO_CLK           RCC_APB2Periph_GPIOD   
 
#define V_COL_MOSI_PIN                GPIO_Pin_7
#define V_COL_MOSI_GPIO_PORT          GPIOC
#define V_COL_MOSI_GPIO_CLK           RCC_APB2Periph_GPIOC    

#define IC_P_MOSI_PIN                GPIO_Pin_9
#define IC_P_MOSI_GPIO_PORT          GPIOC
#define IC_P_MOSI_GPIO_CLK           RCC_APB2Periph_GPIOC     
   
#define BUZZER_PIN                      GPIO_Pin_12
#define BUZZER_GPIO_PORT                GPIOG
#define BUZZER_GPIO_CLK                 RCC_APB2Periph_GPIOG
   
#define	MIC_EN_PIN			GPIO_Pin_5//dxl,2013.5.15,PD5与IC卡检测口共用
#define MIC_EN_GPIO_PORT                GPIOD
#define MIC_EN_GPIO_CLK                 RCC_APB2Periph_GPIOD 
	 
#define	I2S_AD_DA_PIN			GPIO_Pin_2//dxl,2013.5.16,PG2
#define I2S_AD_DA_GPIO_PORT             GPIOG
#define I2S_AD_DA_GPIO_CLK              RCC_APB2Periph_GPIOG 
	 
#define	I2S_PWDAN_PIN			GPIO_Pin_12//dxl,2013.5.16,PD12
#define I2S_PWDAN_GPIO_PORT             GPIOD
#define I2S_PWDAN_GPIO_CLK              RCC_APB2Periph_GPIOD 
	 
#define	I2S_PWADN_PIN			GPIO_Pin_11//dxl,2013.5.16,PD11
#define I2S_PWADN_GPIO_PORT             GPIOD
#define I2S_PWADN_GPIO_CLK              RCC_APB2Periph_GPIOD 
	 
#define	SD_POWER_PIN			GPIO_Pin_3//dxl,2013.5.16,PG3
#define SD_POWER_GPIO_PORT             GPIOG
#define SD_POWER_GPIO_CLK              RCC_APB2Periph_GPIOG 
   
#define PWR_TYPE_PIN                    GPIO_Pin_3//dxl,2013.5.26,PF3  
#define PWR_TYPE_GPIO_PORT             GPIOF
#define PWR_TYPE_GPIO_CLK              RCC_APB2Periph_GPIOF 
	 
#define SD_DETECT_PIN                    GPIO_Pin_13//dxl,2013.8.16,PD13 
#define SD_DETECT_GPIO_PORT             GPIOD
#define SD_DETECT_GPIO_CLK              RCC_APB2Periph_GPIOD 
	

struct stm32_hw_pin_userdata
{
    int pin;
    uint32_t mode;
};
typedef void (*stm32f10x_pin_hdr)(void *args);
#define STM32F10X_IRQ_FUNCTION_ALIAS(pinFunc, pin)	pinFunc

#define PIN_USERDATA_END {-1,0}
//输出脚,输出脚编号
enum 
{
  //CON5接口,32PIN接口
  PIN_NO_EXT_PLUSE 					= 1,			//外部脉冲检测 E2
  //GNSS模块 
  PIN_NO_GPS_CTR 						= 5,             //GPS电源控制脚,输出E6
 	PIN_NO_GPS_SC 						= 7,              //天线短路,输入,别名[ANT_SHORT]C13 
 	PIN_NO_GPS_OC 						= 12,              //天线断开,输入,别名[ANT_SHUT]F2 
	//POWER TYPE  F3
  PIN_NO_PWR_TYPE 					= 13,
	
 	PIN_NO_FLASH_CS 					= 40,           //Flash片选线,输出A4
  PIN_NO_LAMP 							= 44,                //转向灯,输出,别名[TURN_LIGHT]C4
  PIN_NO_DOORI_N 						= 45,             //车门1,输入,低有效(负触发)C5
  PIN_NO_LOCK_OUT 					= 46,            //中控锁-关,输出,B0
  PIN_NO_UNLOCK_OUT 				= 47,          //中控锁-开,输出,B1
  PIN_NO_COIL 							= 48,                //断油路,输出,别名[OIL_CUT]B2
  PIN_NO_HORN_OUT 					= 49,            //报警喇叭,输出,F11
  //摄像头控制脚
  PIN_NO_CAM_POWER 					= 50,          //摄像头电源控制脚,输出F12
  PIN_NO_VIRBRATIONI 				= 53,         //一键通话,输入,F13
  PIN_NO_SWITCHI 						= 54,            //紧急报警,输入,  F14
  PIN_NO_DOORI2 						= 55,              //车门2,输入,可配置,默认低有效(负触发)F15
  PIN_NO_HORNI 							= 56,               //汽车喇叭,输入,默认高有效(正触发)G0
 	PIN_NO_DOORI_P 						= 57,             //车门1,输入,高有效(正触发)G1 
 	PIN_NO_ACC 								= 58,                 //ACC,输入,高有效(正触发)E7
 	PIN_NO_BREAK 							= 59,               //刹车,输入,高有效(正触发)E8
 	PIN_NO_BKHI								= 60,                //空调,输入,高有效(正触发),别名[AIR_CONDITION]  E9	
  
 	PIN_NO_LLIGHTI 						= 63,             //左转灯,输入,高有效(正触发),别名[RIGHT_LIGHT]E10 	
  PIN_NO_RLIGHTI 						= 65,             //右转灯,输入,高有效(正触发)E12
  PIN_NO_DLIGHTI 						= 66,         //大灯(远光灯),输入,高有效(正触发),别名[BIG_LIGHT]E13 
 	PIN_NO_XLIGHTI 						= 67,             //小灯,输入,高有效,别名[LITTLE_LIGHT]E14 	  
  //CON6,16PIN接口  
  PIN_NO_DETCT2I 						= 68,             //机油压力,输入,,别名[GAS_PRESS]E15
 	PIN_NO_DETCT1I 						= 70,             //近光灯,输入,,别名[NEAR_LIGHT]B11 
 	
  PIN_NO_CAM_RD 						= 79,             //摄像头485读写控制,输出   D10
  //I2S_PWADN D11
  PIN_NO_I2S_PWADN 					= 80,
  //I2S_PWDAN  D12
  PIN_NO_I2S_PWDAN 					= 81,
  //SD插入检测  D13
  PIN_NO_SD_DETECT 					= 82, 
  //CAN总线
  PIN_NO_CAN_CTR 						= 86,            //CAN电源控制,输出  D15
  //I2S_AD/DA  G2
  PIN_NO_I2S_AD_DA 					= 87,
  //SD供电控制 G3
  PIN_NO_SD_POWER 					= 88,
  
  PIN_NO_EPROM_CS						= 89,           //EEPROM片选线,输出G4
  PIN_NO_GSM_DTR 						= 90,            //通讯模块DTR脚,输出G5
  
  //TTS控制脚
  PIN_NO_TTS_RDY 						= 91,            //TTS语音播报就绪脚,输入G6
  PIN_NO_TTS_P 							= 92,              //TTS电源控制脚,输出G7
  PIN_NO_FR_CS 							= 93,              //铁电片选线,输出G8
  //喇叭控制
  PIN_NO_V_COL 							= 97,//C7  
  PIN_NO_LCD_SPI_MISO 			= 98,       //LCD SPI信号输入,输入C8
  //ic 卡电源控制
  PIN_NO_IC_P 							= 99,//C9
  PIN_NO_LCD_RST 						= 100,             //LCD复位A8 
 	//存储片选线和使能线
  PIN_NO_MEM_WP 						= 110,             //存储写保护使能线,输出A15 
  PIN_NO_GSM_PWC 						= 111,            //通讯模块电源控制脚,输出C10
  //LCD控制脚
  PIN_NO_LCD_ON 						= 112,             //LCD电源控制脚,输出C11
  PIN_NO_LCD_AO 						= 117,             //LCD寄存器选择,输出D
 	//通讯模块控制脚
  PIN_NO_GSM_ON 						= 118,             //通讯模块开机脚,输出D4 
  //麦克风使能控制D5
  PIN_NO_MIC_EN 						= 119,
  //BMA220震动传感器
  PIN_NO_SHOCK_CS 					= 123,           //震动传感器地址选择  D7
  //其他
  PIN_NO_BUZZER 						= 127,             //蜂鸣器,输出,G12
  PIN_NO_GSM_RI 						= 128,             //通讯模块响铃脚,输入G13
  PIN_NO_LCD_CS 						= 132,             //LCD片选线,输出G153
  PIN_NO_LCD_SPI_SCK 				= 141,        //LCD SPI时钟信号,输出E0 
  PIN_NO_LCD_SPI_MOSI 			= 142,       //LCD SPI信号输出,输出E1

  
};

extern struct stm32_hw_pin_userdata stm32_pins[];
//extern const stm32f10x_pin_hdr	stm32f10x_pin_hdr_tab[];

int stm32_hw_pin_init(void);


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
void gpio_out_init(rt_base_t pin);
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
void gpio_out_on(rt_base_t pin);
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
void gpio_out_off(rt_base_t pin);
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
void gpio_in_init(rt_base_t pin);
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
int gpio_pin_read(rt_base_t pin);

/****************函数定义********************************************
//函数名称	:gpio_irq_install
//功能		:gpio终端装载函数
//输入		:IoNum 输入值,参考GPIO_OUT_IN_NUM枚举
//输出		:
//使用资源	:
//全局变量	:   
//调用函数	:
//中断资源	:  
//返回		:
//备注		:
*********************************************************************/
rt_err_t gpio_irq_install(rt_base_t pin, rt_base_t pinMode, rt_uint32_t irqMode,void  *args);
rt_err_t gpio_irq_install_callback(rt_base_t pin, rt_base_t pinMode, void (*hdr)(void *args),rt_uint32_t irqMode,void  *args);

#endif
