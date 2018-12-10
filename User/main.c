#define	_SYSTEM_CFG_GLOBALS
#include <rthw.h>
#include <rtthread.h>
#ifdef RT_USING_DFS
/* 包含DFS 的头文件 */
#include <dfs_fs.h>
#include <dfs_elm.h>
#include <dfs_posix.h>

#endif
//#include <shell.h>
#include "board.h"
#include "include.h"


#ifdef __CC_ARM
extern int Image$$RW_IRAM1$$ZI$$Limit;
#elif __ICCARM__
#pragma section="HEAP"
#else
extern int __bss_end;
#endif

//static rt_sem_t my_sem = RT_NULL;
static rt_thread_t sys_init_tid = RT_NULL;


int rt_application_init(void);
//空闲钩子函数发送任务邮件
void rt_thread_idle_hook(void)
{
	if(task_mailBox)
	{
		TimerTaskScheduler();
	}
}
//设置调度器钩子
void rt_system_scheduler_hook(struct rt_thread *from, struct rt_thread *to)
{
}
void ei_show_version(void)
{
    rt_kprintf("\n \\ | /\n");
    rt_kprintf("- EI -     Egs701gb-iii fct System\n");
    rt_kprintf(" / | \\     %d.%d.%d build %s\n",
               EI_VERSION, EI_SUBVERSION, EI_REVISION, __DATE__);
    rt_kprintf(" 2006 - 2018 Copyright by Shenzhen EI company\n");
}

void rtthread_startup(void)
{
		#ifdef RT_USING_HEAP
		#if STM32_EXT_SRAM
			rt_system_heap_init((void*)STM32_EXT_SRAM_BEGIN, (void*)STM32_EXT_SRAM_END);
		#else
		#ifdef __CC_ARM
			rt_system_heap_init((void*)&Image$$RW_IRAM1$$ZI$$Limit, (void*)STM32_SRAM_END);
		#elif __ICCARM__
			rt_system_heap_init(__segment_end("HEAP"), (void*)STM32_SRAM_END);
		#else
			/* init memory system */
			rt_system_heap_init((void*)&__bss_end, (void*)STM32_SRAM_END);
		#endif
		#endif  /* STM32_EXT_SRAM */
		#endif /* RT_USING_HEAP */
		
		/* init board */
    rt_hw_board_init();
	
    /* show version */
    ei_show_version();
		
    /* init scheduler system */
    rt_system_scheduler_init();
		rt_scheduler_sethook(rt_system_scheduler_hook);
	
    /* initialize timer */
    rt_system_timer_init();

	#ifdef RT_USING_FINSH    
	/* init finsh */    
	//finsh_system_init();
	#endif
	
    /* init timer thread */
    rt_system_timer_thread_init();
	
	/* init application */
    rt_application_init();
	
    /* init idle thread */
	rt_thread_idle_init();
	rt_thread_idle_sethook(rt_thread_idle_hook);   
		
    /* start scheduler */
    rt_system_scheduler_start();

    /* never reach here */
    return ;
}

#include <sfud.h>
#include <spi_flash.h>
#include <spi_flash_sfud.h>
#include <spi_master.h>
#include <spi_msd.h>
#include <ffconf.h>
//File System test
unsigned char	rt_dir_operate_test(const char* dir_path)
{

		unsigned char i;
		DIR *dirp;
		int fd,size;
		struct dirent *d;
		
		char *path = (char*)rt_malloc(20);
		if(RT_NULL == path)
			return 1;
		memset(path,0,sizeof(path));
		
		
		/* 创建目录*/
		fd = mkdir(dir_path, 0x777);
		if(fd < 0)
		{
			/* 创建目录失败*/
			rt_kprintf("mkdir error!\n");
		}
		else
		{
			/* 创建目录成功*/
			rt_kprintf("mkdir ok!\n");
		}
		/* 打开目录*/
		dirp = opendir(dir_path);
		if(dirp == RT_NULL)
		{
			rt_kprintf("open directory error!\n");
		}
		else
		{
			char buffer[20];
		/* 在这儿进行读取目录相关操作*/		
			strncpy(path,dir_path,strlen(dir_path));
			i = strlen(dir_path);
			sprintf(&path[i], "/%s", "text1.txt");
			fd = open(path, O_RDWR | O_CREAT);
			if (fd >= 0)
			{
				write(fd, "hello\n", sizeof("hello\n"));
				close(fd);
			}
			memset(path,0,sizeof(path));
			i = strlen(dir_path);
			strncpy(path,dir_path,i);	
			sprintf(&path[i], "/%s", "text2.txt");
			fd = open(path, O_RDWR | O_CREAT);
			if (fd >= 0)
			{
				write(fd, "world\n", sizeof("world\n"));
				close(fd);
			}
			
			/* 读取目录*/
			while ((d = readdir(dirp)) != RT_NULL)
			{
				rt_kprintf("found %s\n", d->d_name);

				memset(path,0,sizeof(path));
				i = strlen(dir_path);
				strncpy(path,dir_path,i);
				sprintf(&path[i], "/%s", d->d_name);					
				fd = open(path, O_RDONLY);
				if (fd >= 0)
				{
					size = read(fd, buffer, sizeof(buffer));
					close(fd);
				}
				
				if(size)
					rt_kprintf("%s", buffer);		

				/* 删除目录文件 */
				fd = unlink(path);
				if(0 == fd)
					rt_kprintf("delete [file: %s] success...\n",path);
			}	
			/* 关闭目录 */	
			closedir(dirp);	
			/* 删除目录 */	
			fd = rmdir(dir_path);
			if(0 == fd)
					rt_kprintf("delete [dir: %s] success...\r\n",dir_path);
		}
		/* 释放动态内存 */
		rt_free(path);	
		
		return 0;
}
void rt_file_operate_test(const char* filename)
{
		int fd,size = 0;
		char s[] = "RT-Thread File System test!\n", buffer[80];

		/* 打开text.txt 作写入，如果该文件不存在则建立该文件*/
		fd = open(filename, O_RDWR | O_CREAT);
		if (fd >= 0)
		{
			write(fd, s, sizeof(s));
			close(fd);
		}

		/* 打开text.txt 准备作读取动作*/
		fd = open(filename, O_RDONLY);
		if (fd >= 0)
		{
			size = read(fd, buffer, sizeof(buffer));
			close(fd);
		}		
		if(size)
			rt_kprintf("%s", buffer);

		/* 删除文件 */
		unlink(filename);
}
void rt_file_system_test(void)
{
		
		rt_file_operate_test("text.txt");

		rt_dir_operate_test("/web");

}

uint8_t spi_write_buff[10] = {1,2,3,4,5,6,7,8,9,10};
uint8_t spi_read_buff[10];

uint8_t CheckSdOnOff(void)
{
    u8  status;
    
    status  = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13);

		return status;
}

void rt_init_thread_entry(void* parameter) {					
			
#ifdef RT_USING_COMPONENTS_INIT    
	/* initialization RT-Thread Components */    
			rt_components_init();
#endif
			PeripheralInit();
			//PowerOnUpdata();
			/* SFUD initialize */ 
			rt_spi_flash_device_t flash_handle;
		#ifdef FLASH_AT26DF161
			flash_handle = rt_sfud_flash_probe("AT26DF161", "spi10");	
		#if 1
			if(flash_handle) {
				rt_kprintf("FLASH_AT26DF161 initialized!\n");
				spi_write_status("spi10", 0, 0);//unlock all
				spi_erase("spi10", 0, 4096);		
				spi_write("spi10",0,10,spi_write_buff);
				spi_read("spi10",0,10,spi_read_buff);
				memset(spi_read_buff,0x00,sizeof(spi_read_buff));
			}
		#else
			sfud_flash_t sfud_dev = rt_sfud_flash_find("spi10");
  		if(sfud_dev) {
				sfud_write_status(sfud_dev, 0, 0);//unlock all
				sfud_erase(sfud_dev, 0, 4096);		
				//sfud_write(sfud_dev,0,10,spi_write_buff);
				//sfud_read(sfud_dev,0,10,spi_read_buff);
			}
		#endif
		#endif
		#ifdef E2PROM_25LC320A
			flash_handle = rt_sfud_flash_probe("25LC320A", "spi11");	
			if(flash_handle) {
				rt_kprintf("E2PROM_25LC320A initialized!\n");
				CheckEepromChip();
			}
		#endif
			if(0 == CheckSdOnOff()) {
				msd_init("sd0", "spi30");
			}
			/* Filesystem Initialization */
#if defined(RT_USING_DFS) && defined(RT_USING_DFS_ELMFAT)
			/* mount sd card fat partition 1 as root directory */
			if(0 == dfs_mount("sd0", "/", "elm", 0, 0)) {
				rt_kprintf("File System initialized!\n");
				rt_file_system_test();
			}else {
				if(0 == dfs_mkfs("elm", "sd0")) {
					if (dfs_mount("sd0", "/", "elm", 0, 0) == 0) {
						rt_kprintf("File System initialized!\n");
						rt_file_system_test();
					}else {
						rt_kprintf("File System initialzation failed!\n");
					}
				}else {
					rt_kprintf("File System initialzation failed!\n");
				}
			}
#endif  /* RT_USING_DFS */


	//rt_thread_delay( 10 );  
	//#ifdef	RT_USING_CONSOLE      
	//rt_kprintf("系统初始化线程自动退出...\r\n");
	//#endif 
	//sys_init_tid = RT_NULL;
	//rt_thread_delete(rt_thread_self());
}

int rt_application_init(void)
{         

  	#if (RT_THREAD_PRIORITY_MAX == 32)    
	sys_init_tid = rt_thread_create("init",                                   
									rt_init_thread_entry, 
									RT_NULL,                                   
									2048, 
									8, 
									20);
	#else    
	sys_init_tid = rt_thread_create("init",                                   
									rt_init_thread_entry, 
									RT_NULL,                                   
									2048, 
									80, 
									20);
	#endif    
	if (sys_init_tid != RT_NULL)        
		rt_thread_startup(sys_init_tid);
	
	//VDR_Usart_thread_init();
	watchDog_thread_init();
	//comm_thread_init();
	task_schedule_thread_init();
	fct_app_thread_init();
	return 0;
}
int main(void)
{
/* disable interrupt first */
    rt_hw_interrupt_disable();

    /* startup RT-Thread RTOS */
    rtthread_startup();

    return 0;
}

