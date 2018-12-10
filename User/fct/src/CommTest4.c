/*******************************************************************************
 * File Name:			CommTest4.c 
 * Function Describe:	
 * Relate Module:		测试相关协议。
 * Writer:				Joneming
 * Date:				2011-09-08
 * ReWriter:			
 * Date:				
 *******************************************************************************/
#include "include.h"
#include "board.h"
///////////////////////
#define TEST_COM4               COM4//参数使用的串口
//////////////////////////
#define COMTEST4_MAX_REC_LEN    128 //接收数据的长度
#define COMTEST4_MAX_SEND_LEN   COM4_MAX_LEN_TX //发送数据的长度
///////////////////////
static unsigned char s_ucCommTest4recvBuff[COMTEST4_MAX_REC_LEN+1];     //解析缓冲
//static unsigned char s_ucCommTest4resendBuff[COMTEST4_MAX_SEND_LEN+1];  //发送缓冲
/////////////////////////
static ST_COMM s_stCommTest4;
//////////////////////////
/****************************************************/
/*                                                  */
/* Local Definitions and macros                     */
/*                                                  */
/****************************************************/
/*************************************************************
** 函数名称: CommTest4_SendData
** 功能描述: 发送数据
** 入口参数: pBuffer源数据,BufferLen数据长度
** 出口参数: 无
** 返回参数: 0:成功；1:失败,0xff:超出缓存长度
** 全局变量: 无
** 调用模块: 无
*************************************************************/
unsigned char CommTest4_SendData(unsigned char *pBuffer,unsigned short datalen)
{
    return COMM_SendData(TEST_COM4,pBuffer,datalen);
}
/*************************************************************OK
** 函数名称: CommTest4_ProtocolParse
** 功能描述: 协议解释
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
void CommTest4_ProtocolParse(unsigned char *pBuffer,unsigned short datalen)
{
    unsigned short i;
    for(i=0; i<datalen; i++)
    {
        if(0x0d==pBuffer[i])
        {
            CommTest4_SendData(pBuffer,i+1);
            break;
        }
    }
}
/*************************************************************OK
** 函数名称: CommTest4_TimeTask
** 功能描述: 串口接收数据处理时间任务接口
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
**  备 注  :50毫秒调度1次
*************************************************************/
void CommTest4_CheckReadData(void)
{
    unsigned short datalen;
    datalen=COMM_ReadData(TEST_COM4,&s_stCommTest4.recvBuff[s_stCommTest4.recvlen],COMTEST4_MAX_REC_LEN-s_stCommTest4.recvlen);
    ///////////////////////////////
    s_stCommTest4.recvlen += datalen;
    ///////////////////////////////////
    if(0 == datalen)////处理的数据长度为0//////////    
    {
        if(s_stCommTest4.recvlen)
        {
            s_stCommTest4.recvfreeCnt++;
            ////////////////////////////////
            if(s_stCommTest4.recvfreeCnt >1)//空闲两次调度
            {
                CommTest4_ProtocolParse(s_stCommTest4.recvBuff,s_stCommTest4.recvlen);
                s_stCommTest4.recvlen       = 0;
                s_stCommTest4.recvfreeCnt   = 0;//
            }
        }
    }
    else//////////////////
    if(s_stCommTest4.recvlen+10>COMTEST4_MAX_REC_LEN)//缓存已满的时候
    {
        CommTest4_ProtocolParse(s_stCommTest4.recvBuff,s_stCommTest4.recvlen);
        s_stCommTest4.recvlen       = 0;
        s_stCommTest4.recvfreeCnt   = 0;
    }
    else//////////////////
    {
        s_stCommTest4.recvfreeCnt = 0;
    }
}
/*************************************************************OK
** 函数名称: CommTest4_TimeTask
** 功能描述: 串口接收数据处理时间任务接口
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
**  备 注  :50毫秒调度1次
*************************************************************/
FunctionalState CommTest4_TimeTask(void)
{
    CommTest4_CheckReadData();
    return ENABLE;
}
/*************************************************************OK
** 函数名称: CommTest4RecvEnable
** 功能描述: 
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
void CommTest4RecvEnable(void)
{
    gpio_out_on(PIN_NO_GPS_CTR);                                //初始关闭GPS电源    
    rt_hw_us_delay(0x1000);
}
/*************************************************************OK
** 函数名称: CommTest4SendEnable
** 功能描述: 
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
void CommTest4SendEnable(void)
{
	gpio_out_off(PIN_NO_GPS_CTR);                                //初始开启GPS电源
    rt_hw_us_delay(0x1000);
}
/*************************************************************OK
** 函数名称: CommTest4Init
** 功能描述: //串口初始化
** 入口参数:  
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
void CommTest4Init(void)
{
    s_stCommTest4.recvlen       = 0;
    s_stCommTest4.resendlen     = 0;
    s_stCommTest4.resendCnt     = 0;
    s_stCommTest4.recvfreeCnt   = 0;
    s_stCommTest4.recvBuff      = s_ucCommTest4recvBuff;
    //s_stCommTest4.resendBuff    = s_ucCommTest4resendBuff;
    ////////////////////
    COMM_Initialize(TEST_COM4,9600);
    ////////////////////
    gpio_out_init(PIN_NO_GPS_CTR);		
    COMM_CtrlEnableInitialize(TEST_COM4,CommTest4RecvEnable,CommTest4SendEnable);
}
/*******************************************************************************
 *                             end of module
 *******************************************************************************/

