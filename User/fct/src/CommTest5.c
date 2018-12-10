/*******************************************************************************
 * File Name:			CommTest5.c 
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
#define TEST_COM5               COM5//参数使用的串口
//////////////////////////
#define COMTEST5_MAX_REC_LEN    128 //接收数据的长度
#define COMTEST5_MAX_SEND_LEN   COM5_MAX_LEN_TX //发送数据的长度
///////////////////////
static unsigned char s_ucCommTest5recvBuff[COMTEST5_MAX_REC_LEN+1];     //解析缓冲
//static unsigned char s_ucCommTest5resendBuff[COMTEST5_MAX_SEND_LEN+1];  //发送缓冲
/////////////////////////
static ST_COMM s_stCommTest5;
//////////////////////////
/****************************************************/
/*                                                  */
/* Local Definitions and macros                     */
/*                                                  */
/****************************************************/
/*************************************************************
** 函数名称: CommTest5_SendData
** 功能描述: 发送数据
** 入口参数: pBuffer源数据,BufferLen数据长度
** 出口参数: 无
** 返回参数: 0:成功；1:失败,0xff:超出缓存长度
** 全局变量: 无
** 调用模块: 无
*************************************************************/
rt_size_t CommTest5_SendData(unsigned char *pBuffer,unsigned short datalen)
{
    return stm32_serial5_write(0,pBuffer,datalen);
}
/*************************************************************OK
** 函数名称: CommTest5_ProtocolParse
** 功能描述: 协议解释
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
void CommTest5_ProtocolParse(unsigned char *pBuffer,unsigned short datalen)
{
	App_DisposeGprsRecvData(pBuffer,datalen);
}
/*************************************************************OK
** 函数名称: CommTest5_TimeTask
** 功能描述: 串口接收数据处理时间任务接口
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
**  备 注  :50毫秒调度1次
*************************************************************/
void CommTest5_CheckReadData(void)
{
    unsigned short datalen;
    datalen=stm32_serial5_read(0,&s_stCommTest5.recvBuff[s_stCommTest5.recvlen],COMTEST5_MAX_REC_LEN-s_stCommTest5.recvlen);
    ///////////////////////////////
    s_stCommTest5.recvlen += datalen;
    ///////////////////////////////////
    if(0 == datalen)////处理的数据长度为0//////////    
    {
        if(s_stCommTest5.recvlen)
        {
            s_stCommTest5.recvfreeCnt++;
            ////////////////////////////////
            if(s_stCommTest5.recvfreeCnt >1)//空闲两次调度
            {
                CommTest5_ProtocolParse(s_stCommTest5.recvBuff,s_stCommTest5.recvlen);
                s_stCommTest5.recvlen       = 0;
                s_stCommTest5.recvfreeCnt   = 0;//
            }
        }
    }
    else//////////////////
    if(s_stCommTest5.recvlen+10>COMTEST5_MAX_REC_LEN)//缓存已满的时候
    {
        CommTest5_ProtocolParse(s_stCommTest5.recvBuff,s_stCommTest5.recvlen);
        s_stCommTest5.recvlen       = 0;
        s_stCommTest5.recvfreeCnt   = 0;
    }
    else//////////////////
    {
        s_stCommTest5.recvfreeCnt = 0;
    }
}
/*************************************************************OK
** 函数名称: CommTest5_TimeTask
** 功能描述: 串口接收数据处理时间任务接口
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
**  备 注  :50毫秒调度1次
*************************************************************/
FunctionalState CommTest5_TimeTask(void)
{
    CommTest5_CheckReadData();
    ////////////////////
    //LZM_PublicTimerHandler(s_stCMTEST5Timer,CMTEST5_TIMERS_MAX);    
    //////////////////////
    return ENABLE;
}
/*************************************************************OK
** 函数名称: CommTest5Init
** 功能描述: //串口初始化
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
void CommTest5Init(void)
{
    //LZM_PublicKillTimerAll(s_stCMTEST5Timer,CMTEST5_TIMERS_MAX); 
    //////////////////
    s_stCommTest5.recvlen       = 0;
    s_stCommTest5.resendlen     = 0;
    s_stCommTest5.resendCnt     = 0;
    s_stCommTest5.recvfreeCnt   = 0;
    s_stCommTest5.recvBuff      = s_ucCommTest5recvBuff;
    //s_stCommTest5.resendBuff    = s_ucCommTest5resendBuff;
    ////////////////////
    //COMM_Initialize(TEST_COM5,115200);
		gpio_out_on(PIN_NO_GSM_PWC);
		gpio_out_on(PIN_NO_GSM_ON);
    ////////////////////    
    SetTimerTask(TIME_COM5_TEST, LZM_TIME_BASE); 
    ////////////////////////    
}
/*******************************************************************************
 *                             end of module
 *******************************************************************************/

