/*******************************************************************************
 * File Name:			CommTest2.c 
 * Function Describe:	
 * Relate Module:		测试相关协议。
 * Writer:				Joneming
 * Date:				2011-09-08
 * ReWriter:			
 * Date:				
 *******************************************************************************/
#include "include.h"
///////////////////////
#define TEST_COM2               COM2//参数使用的串口
//////////////////////////
#define COMTEST2_MAX_REC_LEN    128 //接收数据的长度
#define COMTEST2_MAX_SEND_LEN   COM2_MAX_LEN_TX //发送数据的长度
///////////////////////
static unsigned char s_ucCommTest2recvBuff[COMTEST2_MAX_REC_LEN+1];     //解析缓冲
//static unsigned char s_ucCommTest2resendBuff[COMTEST2_MAX_SEND_LEN+1];  //发送缓冲
/////////////////////////
static ST_COMM s_stCommTest2;
//////////////////////////
/****************************************************/
/*                                                  */
/* Local Definitions and macros                     */
/*                                                  */
/****************************************************/
/*************************************************************
** 函数名称: CommTest2_SendData
** 功能描述: 发送数据
** 入口参数: pBuffer源数据,BufferLen数据长度
** 出口参数: 无
** 返回参数: 0:成功；1:失败,0xff:超出缓存长度
** 全局变量: 无
** 调用模块: 无
*************************************************************/
unsigned char CommTest2_SendData(unsigned char *pBuffer,unsigned short datalen)
{
    return COMM_SendData(TEST_COM2,pBuffer,datalen);
}
/*************************************************************OK
** 函数名称: CommTest2_ProtocolParse
** 功能描述: 协议解释
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
void CommTest2_ProtocolParse(unsigned char *pBuffer,unsigned short datalen)
{
    App_DisposeGprsRecvData(pBuffer,datalen);
}
/*************************************************************OK
** 函数名称: CommTest2_TimeTask
** 功能描述: 串口接收数据处理时间任务接口
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
**  备 注  :20毫秒调度1次
*************************************************************/
void CommTest2_CheckReadData(void)
{
    unsigned short datalen;
    datalen=COMM_ReadData(TEST_COM2,&s_stCommTest2.recvBuff[s_stCommTest2.recvlen],COMTEST2_MAX_REC_LEN-s_stCommTest2.recvlen);
    ///////////////////////////////
    s_stCommTest2.recvlen += datalen;
    ///////////////////////////////////
    if(0 == datalen)////处理的数据长度为0//////////    
    {
        if(s_stCommTest2.recvlen)
        {
            s_stCommTest2.recvfreeCnt++;
            ////////////////////////////////
            if(s_stCommTest2.recvfreeCnt >1)//空闲两次调度
            {
                CommTest2_ProtocolParse(s_stCommTest2.recvBuff,s_stCommTest2.recvlen);
                s_stCommTest2.recvlen       = 0;
                s_stCommTest2.recvfreeCnt   = 0;//
            }
        }
    }
    else//////////////////
    if(s_stCommTest2.recvlen+10>COMTEST2_MAX_REC_LEN)//缓存已满的时候
    {
        CommTest2_ProtocolParse(s_stCommTest2.recvBuff,s_stCommTest2.recvlen);
        s_stCommTest2.recvlen       = 0;
        s_stCommTest2.recvfreeCnt   = 0;
    }
    else//////////////////
    {
        s_stCommTest2.recvfreeCnt = 0;
    }
}
/*************************************************************OK
** 函数名称: CommTest2_TimeTask
** 功能描述: 串口接收数据处理时间任务接口
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
**  备 注  :20毫秒调度1次
*************************************************************/
FunctionalState CommTest2_TimeTask(void)
{
    CommTest2_CheckReadData();
    ////////////////////
    //LZM_PublicTimerHandler(s_stCMTEST2Timer,CMTEST2_TIMERS_MAX);    
    //////////////////////
    return ENABLE;
}
/*************************************************************OK
** 函数名称: CommTest2Init
** 功能描述: //串口初始化
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
void CommTest2Init(void)
{
    //LZM_PublicKillTimerAll(s_stCMTEST2Timer,CMTEST2_TIMERS_MAX); 
    //////////////////
    s_stCommTest2.recvlen       = 0;
    s_stCommTest2.resendlen     = 0;
    s_stCommTest2.resendCnt     = 0;
    s_stCommTest2.recvfreeCnt   = 0;
    s_stCommTest2.recvBuff      = s_ucCommTest2recvBuff;
    //s_stCommTest2.resendBuff    = s_ucCommTest2resendBuff;
    ////////////////////
    COMM_Initialize(TEST_COM2,115200);
    ////////////////////    
//    SetTimerTask(TIME_COM2_TEST, LZM_TIME_BASE); 
    ////////////////////////    
}
/*******************************************************************************
 *                             end of module
 *******************************************************************************/

