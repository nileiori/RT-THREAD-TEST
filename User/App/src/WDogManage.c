#define	_WDOGMANAGE_GLOBALS
#include "include.h"
#include <rthw.h>
#include <rtthread.h>



#define	WDOG_THREAD_PRIORITY	10
static rt_uint8_t watchDog_stack[ 512 ];
static struct rt_thread watchDog_thread;

/*************************************************************OK
** 函数名称: watchDog_thread_entry
** 功能描述: 看门狗线程入口
** 入口参数: 
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
static void watchDog_thread_entry(void* parameter)
{     
	rt_uint32_t rcvEvt;
	wdog_event = rt_event_create ("wdog_event", RT_IPC_FLAG_FIFO);
	
	#if E_EYE_ENABLE == CONFIG_IWDG  ////开启看门狗
		IDWG_Configuration(); 
	#endif
	while (1)    
	{        
		/* waitting watchDog  all event arrive... */
		if (rt_event_recv(wdog_event, WDOG_EVENT_FLAG,
				RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
				RT_WAITING_FOREVER, &rcvEvt) == RT_EOK)
		{
			
			#ifdef	RT_USING_CONSOLE	 
					//rt_kprintf("重要线程都运行过了才来喂狗...\r\n");
			#endif		  
			#if E_EYE_ENABLE == CONFIG_IWDG
				IWDG_ReloadCounter();
			#endif
			rt_thread_yield();       
			//rt_thread_delay( RT_TICK_PER_SECOND*5 );   
		}
	}
}
/*************************************************************OK
** 函数名称: watchDog_thread_init
** 功能描述: 看门狗线程初始化
** 入口参数: 
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
void watchDog_thread_init(void)
{
	rt_err_t result;
	
	result = rt_thread_init(&watchDog_thread,                            
							"watchDog_task",                            
							watchDog_thread_entry,                            
							RT_NULL,                            
							(rt_uint8_t*)&watchDog_stack[0],                            
							sizeof(watchDog_stack),                            
							WDOG_THREAD_PRIORITY,                            
							5);    
	if (result == RT_EOK)    
	{        
		rt_thread_startup(&watchDog_thread);    
	} 
}


