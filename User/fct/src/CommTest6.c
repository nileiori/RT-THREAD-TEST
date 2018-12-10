/*******************************************************************************
 * File Name:			CommTest6.c 
 * Function Describe:	
 * Relate Module:		测试相关协议。
 * Writer:				Joneming
 * Date:				2011-09-08
 * ReWriter:			
 * Date:				
 *******************************************************************************/
#include "include.h"
///////////////////////
#define TEST_COM6               COM6//参数使用的串口
//////////////////////////
#define COMTEST6_MAX_REC_LEN    128 //接收数据的长度
#define COMTEST6_MAX_SEND_LEN   COM6_MAX_LEN_TX //发送数据的长度
///////////////////////
static unsigned char s_ucCommTest6recvBuff[COMTEST6_MAX_REC_LEN+1];     //解析缓冲
//static unsigned char s_ucCommTest6resendBuff[COMTEST6_MAX_SEND_LEN+1];  //发送缓冲
/////////////////////////
static ST_COMM s_stCommTest6;
//////////////////////////
/****************************************************/
/*                                                  */
/* Local Definitions and macros                     */
/*                                                  */
/****************************************************/
/*************************************************************
** 函数名称: CommTest6_SendData
** 功能描述: 发送数据
** 入口参数: pBuffer源数据,BufferLen数据长度
** 出口参数: 无
** 返回参数: 0:成功；1:失败,0xff:超出缓存长度
** 全局变量: 无
** 调用模块: 无
*************************************************************/
unsigned char CommTest6_SendData(unsigned char *pBuffer,unsigned short datalen)
{
    return COMM_SendData(TEST_COM6,pBuffer,datalen);
}
/*************************************************************OK
** 函数名称: CommTest6_ProtocolParse
** 功能描述: 协议解释
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
void CommTest6_ProtocolParse(unsigned char *pBuffer,unsigned short datalen)
{
    unsigned short i;
    for(i=0; i<datalen; i++)
    {
        if(0x0d==pBuffer[i])
        {
            CommTest6_SendData(pBuffer,i+1);
            break;
        }
    }
}
/*************************************************************OK
** 函数名称: CommTest6_TimeTask
** 功能描述: 串口接收数据处理时间任务接口
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
**  备 注  :50毫秒调度1次
*************************************************************/
void CommTest6_CheckReadData(void)
{
    unsigned short datalen;
    datalen=COMM_ReadData(TEST_COM6,&s_stCommTest6.recvBuff[s_stCommTest6.recvlen],COMTEST6_MAX_REC_LEN-s_stCommTest6.recvlen);
    ///////////////////////////////
    s_stCommTest6.recvlen += datalen;
    ///////////////////////////////////
    if(0 == datalen)////处理的数据长度为0//////////    
    {
        if(s_stCommTest6.recvlen)
        {
            s_stCommTest6.recvfreeCnt++;
            ////////////////////////////////
            if(s_stCommTest6.recvfreeCnt >1)//空闲两次调度
            {
                CommTest6_ProtocolParse(s_stCommTest6.recvBuff,s_stCommTest6.recvlen);
                s_stCommTest6.recvlen       = 0;
                s_stCommTest6.recvfreeCnt   = 0;//
            }
        }
    }
    else//////////////////
    if(s_stCommTest6.recvlen+10>COMTEST6_MAX_REC_LEN)//缓存已满的时候
    {
        CommTest6_ProtocolParse(s_stCommTest6.recvBuff,s_stCommTest6.recvlen);
        s_stCommTest6.recvlen       = 0;
        s_stCommTest6.recvfreeCnt   = 0;
    }
    else//////////////////
    {
        s_stCommTest6.recvfreeCnt = 0;
    }
}
/*************************************************************OK
** 函数名称: CommTest6_TimeTask
** 功能描述: 串口接收数据处理时间任务接口
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
**  备 注  :50毫秒调度1次
*************************************************************/
FunctionalState CommTest6_TimeTask(void)
{
    CommTest6_CheckReadData();
    ////////////////////
    //LZM_PublicTimerHandler(s_stCMTEST6Timer,CMTEST6_TIMERS_MAX);    
    //////////////////////
    return ENABLE;
}
/*************************************************************OK
** 函数名称: CommTest6RecvEnable
** 功能描述: 
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
void CommTest6RecvEnable(void)
{
    GpioOutOff(CAM_RD);
    Delay_uS(0x1000);
}
/*************************************************************OK
** 函数名称: CommTest6SendEnable
** 功能描述: 
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
void CommTest6SendEnable(void)
{
    GpioOutOn(CAM_RD);
    Delay_uS(0x1000);
}
/*************************************************************OK
** 函数名称: CommTest6Init
** 功能描述: //串口初始化
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
void CommTest6Init(void)
{
    //LZM_PublicKillTimerAll(s_stCMTEST6Timer,CMTEST6_TIMERS_MAX); 
    //////////////////
    s_stCommTest6.recvlen       = 0;
    s_stCommTest6.resendlen     = 0;
    s_stCommTest6.resendCnt     = 0;
    s_stCommTest6.recvfreeCnt   = 0;
    s_stCommTest6.recvBuff      = s_ucCommTest6recvBuff;
    //s_stCommTest6.resendBuff    = s_ucCommTest6resendBuff;
    ////////////////////
    COMM_Initialize(TEST_COM6,9600);
    ////////////////////
    GpioOutInit(CAM_RD);
    GpioOutInit(CAM_PWR);
    GpioOutOn(CAM_PWR);
    COMM_CtrlEnableInitialize(TEST_COM6,CommTest6RecvEnable,CommTest6SendEnable);
    SetTimerTask(TIME_COM6_TEST, LZM_TIME_BASE); 
    ////////////////////////    
}
/*******************************************************************************
 *                             end of module
 *******************************************************************************/

