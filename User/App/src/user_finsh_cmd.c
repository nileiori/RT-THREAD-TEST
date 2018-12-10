/*
 * user_finsh_cmd.c
 *
 *  Created on: 2013年12月7日
 *      Author: Armink
 */


#include <rthw.h>
#include <rtthread.h>
#include <stm32f10x.h>
#include <finsh.h>
#include <sfud.h>
#include <shell.h>
#include <spi_flash.h>

#if defined(RT_USING_FINSH) && defined(FINSH_USING_MSH)
//自定义的一些msh命令
static void reboot(uint8_t argc, char **argv) 
{
    NVIC_SystemReset();
}
FINSH_FUNCTION_EXPORT(reboot, Reboot System);
MSH_CMD_EXPORT(reboot, Reboot System);
long sysclock(void)
{
		RCC_ClocksTypeDef RCC_ClocksStatus;
  
  	RCC_GetClocksFreq(&RCC_ClocksStatus);

    rt_kprintf("SYSCLK_Frequency :%d Hz\n",RCC_ClocksStatus.SYSCLK_Frequency);
		rt_kprintf("HCLK_Frequency 	 :%d Hz\n",RCC_ClocksStatus.HCLK_Frequency);
		rt_kprintf("PCLK1_Frequency  :%d Hz\n",RCC_ClocksStatus.PCLK1_Frequency);
		rt_kprintf("PCLK2_Frequency  :%d Hz\n",RCC_ClocksStatus.PCLK2_Frequency);
		rt_kprintf("ADCCLK_Frequency :%d Hz\n",RCC_ClocksStatus.ADCCLK_Frequency);

		return 0;
}
FINSH_FUNCTION_EXPORT(sysclock, show sysclock freq);
MSH_CMD_EXPORT(sysclock, show sysclock freq);
#endif



