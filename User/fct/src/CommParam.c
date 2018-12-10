/*******************************************************************************
 * File Name:			CommParam.c 
 * Function Describe:	
 * Relate Module:		测试相关协议。
 * Writer:				Joneming
 * Date:				2011-09-08
 * ReWriter:			
 * Date:				
 *******************************************************************************/
#include "include.h"
#include "usart.h"
#include <rthw.h>
#include <rtthread.h>

///////////////////////
#define PARAM_COM               COM1//参数使用的串口
//////////////////////////
#define COMPARAM_MAX_REC_LEN    128 //接收数据的长度
#define COMPARAM_MAX_SEND_LEN   COM1_MAX_LEN_TX //发送数据的长度
///////////////////////
static unsigned char s_ucCommrecvBuff[COMPARAM_MAX_REC_LEN+1];     //解析缓冲
static unsigned char s_ucCommresendBuff[COMPARAM_MAX_SEND_LEN+1];  //发送缓冲
/////////////////////////
static ST_COMM s_stCommParam;
//////////////////////////
enum 
{
    CPARAM_TIMER_RESEND, //    
    CPARAM_TIMERS_MAX
}E_COM_PARAMTIME;
//static LZM_TIMER s_stCParamTimer[CPARAM_TIMERS_MAX];
/****************************************************/
/*                                                  */
/* Local Definitions and macros                     */
/*                                                  */
/****************************************************/
/*************************************************************
** 函数名称: CommParam_StartSendData
** 功能描述: 发送数据
** 入口参数: pBuffer源数据,BufferLen数据长度
** 出口参数: 无
** 返回参数: 0:成功；1:失败,0xff:超出缓存长度
** 全局变量: 无
** 调用模块: 无
*************************************************************/
rt_size_t CommParam_StartSendData(unsigned char *pBuffer,unsigned short datalen)
{
	return stm32_serial1_write(0,pBuffer,datalen);
}
/************************************************************
** 函数名称: CommParam_ResendData
** 功能描述: 重发时间到，执行重发处理
             
** 入口参数:
** 出口参数:
************************************************************/ 
ErrorStatus CommParam_ResendData(void)
{
    if(0 == CommParam_StartSendData(s_stCommParam.resendBuff,s_stCommParam.resendlen))//发送成功
    {
        s_stCommParam.resendCnt = 0;
        return SUCCESS;
    }
    /////////////////////////
    if(s_stCommParam.resendCnt)//开启重发
    {
        s_stCommParam.resendCnt--;
        Timer10msStart(TIMER_10MS_ID_COMM_RESEND,15,TIMER_MODE_ONCE,CommParam_ResendData);
				return ERROR;
    }  
		return SUCCESS;
}
/*************************************************************
** 函数名称: CommParam_SendData
** 功能描述: 发送数据
** 入口参数: pBuffer源数据,BufferLen数据长度
** 出口参数: 无
** 返回参数: 0:成功；1:等待重发,2:失败,0xff:超出缓存长度
** 全局变量: 无
** 调用模块: 无
*************************************************************/
unsigned char CommParam_SendData(unsigned char *pBuffer,unsigned short datalen)
{
    if(datalen>COMPARAM_MAX_SEND_LEN)return 0xff;//超过缓存,直接返回
    //////////////////
    if(0 == CommParam_StartSendData(pBuffer,datalen))return 0;//发送成功
    //////失败//////////////////
    if(0==s_stCommParam.resendCnt)
    {
        s_stCommParam.resendlen = datalen;
        memcpy(&s_stCommParam.resendBuff,pBuffer,s_stCommParam.resendlen);
        s_stCommParam.resendCnt = 4;//重发送次数
        //LZM_PublicSetOnceTimer(&s_stCParamTimer[CPARAM_TIMER_RESEND], PUBLICSECS(0.15), CommParam_ResendData); 
				Timer10msStart(TIMER_10MS_ID_COMM_RESEND,15,TIMER_MODE_ONCE,CommParam_ResendData);
        return 1;//等待重发
    }
    else
    if((s_stCommParam.resendlen+datalen)<COMPARAM_MAX_SEND_LEN)
    {
        memcpy(&s_stCommParam.resendBuff[s_stCommParam.resendlen],pBuffer,datalen);
        s_stCommParam.resendlen += datalen;
        return 1;//等待重发
    }
    else//
    {
        return 2;//失败
    }
}

/*************************************************************OK
** 函数名称: CommParam_ProtocolParse
** 功能描述: 协议解释
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
void CommParam_ProtocolParse(unsigned char *pBuffer,unsigned short datalen)
{
    FCT_AppDisposeDebugRecvData(pBuffer,datalen);
}
/*************************************************************OK
** 函数名称: CommParam_CheckReadData
** 功能描述: 串口接收数据处理时间任务接口
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
**  备 注  :50毫秒调度1次
*************************************************************/
void CommParam_CheckReadData(void)
{
		unsigned short datalen;

		datalen=stm32_serial1_read(0,&s_stCommParam.recvBuff[s_stCommParam.recvlen],COMPARAM_MAX_REC_LEN-s_stCommParam.recvlen);
		///////////////////////////////
		s_stCommParam.recvlen += datalen;
		///////////////////////////////////
		if(0 == datalen)////处理的数据长度为0////////// 	 
		{
				if(s_stCommParam.recvlen)
				{
						s_stCommParam.recvfreeCnt++;
						////////////////////////////////
						if(s_stCommParam.recvfreeCnt >1)//空闲两次调度
						{
								CommParam_ProtocolParse(s_stCommParam.recvBuff,s_stCommParam.recvlen);
								s_stCommParam.recvlen 			= 0;
								s_stCommParam.recvfreeCnt 	= 0;//
						}
				}
		}
		else//////////////////
		if(s_stCommParam.recvlen+10>COMPARAM_MAX_REC_LEN)//缓存已满的时候
		{
				CommParam_ProtocolParse(s_stCommParam.recvBuff,s_stCommParam.recvlen);
				s_stCommParam.recvlen 			= 0;
				s_stCommParam.recvfreeCnt 	= 0;
		}
		else//////////////////
		{
				s_stCommParam.recvfreeCnt = 0;
		}
}

/*************************************************************OK
** 函数名称: CommParam_TimeTask
** 功能描述: 串口接收数据处理时间任务接口
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
**  备 注  :50毫秒调度1次
*************************************************************/
FunctionalState CommParam_TimeTask(void)
{
    CommParam_CheckReadData();
    ////////////////////
    //LZM_PublicTimerHandler(s_stCParamTimer,CPARAM_TIMERS_MAX);    
    //////////////////////
    return ENABLE;
}
/*************************************************************OK
** 函数名称: CommParamInit
** 功能描述: //串口初始化
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
void CommParamInit(void)
{
    s_stCommParam.recvlen       = 0;
    s_stCommParam.resendlen     = 0;
    s_stCommParam.resendCnt     = 0;
    s_stCommParam.recvfreeCnt   = 0;
    s_stCommParam.recvBuff      = s_ucCommrecvBuff;
    s_stCommParam.resendBuff    = s_ucCommresendBuff;
    ////////////////////
    //COMM_Initialize(PARAM_COM,115200);
		////////////////////    
    SetTimerTask(TIME_COM_PARAM, LZM_TIME_BASE); 
}
/*******************************************************************************
 *                             end of module
 *******************************************************************************/

