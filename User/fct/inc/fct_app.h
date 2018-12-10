#ifndef	_FCT_APP_H_
#define	_FCT_APP_H_

#ifdef	_FCT_APP_GLOBALS
#define	_FCT_APP_EXT	
#else
#define	_FCT_APP_EXT	extern
#endif

#include <rtdef.h>

#define ADDR_COUNT              0x08

#define FM25C160_MAX_ADDR       (2048-1)//2K
#define EE25LC320A_MAX_ADDR     (2048-1)//2K

#define FLASH_DATA_MAX_ADDR     (1024*1024*4-6)//4M

#define AT25DF321_ID            0X1F4700//0X1F470000

#define	FCT_MSG_LEN							10

typedef struct	_DEBUG_FCT_
{
    uint32_t time;
    uint8_t debug_f;
    uint8_t	gprs;
    uint8_t iis;
}_APP_DEBUG_FCT;

_FCT_APP_EXT	_APP_DEBUG_FCT	g_fct;
_FCT_APP_EXT  rt_mq_t fct_app_msg;

/*************************************************************************
*	函 数 名: FCT_AppInit
*	功能说明: FCT相关参数初始化
*	形    参：无
*	返 回 值: 无
***************************************************************************/
_FCT_APP_EXT void FCT_AppInit(void);
/*************************************************************************
*	函 数 名: App_DisposeGprsRecvData
*	功能说明: GSM返回的数据回传FCT,过程中0x0d,0x0a用0x20代替
*	形    参：无
*	返 回 值: 无
***************************************************************************/
_FCT_APP_EXT void App_DisposeGprsRecvData(unsigned char *pBuffer,unsigned short datalen);
/*************************************************************************
*	函 数 名: Task_DebugCommIncode
*	功能说明: 
*	形    参：无
*	返 回 值: 无
***************************************************************************/
_FCT_APP_EXT void Task_DebugCommIncode(void);
/*************************************************************OK
** 函数名称: fct_app_thread_init
** 功能描述: 
** 入口参数: 
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
_FCT_APP_EXT void fct_app_thread_init(void);

#endif





