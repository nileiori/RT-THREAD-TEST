#define	_FCT_APP_GLOBALS
#include "include.h"
#include <rthw.h>
#include <rtthread.h>
#include "usart.h"

#define	FCT_APP_THREAD_PRIORITY	12
static rt_uint8_t fct_app_stack[ 1024 ];
static struct rt_thread fct_app_thread;



enum 
{
		CHECK_INPUT_EMPTY=0,					//
		CHECK_INPUT_DOORI_N,					//门状态信号
		CHECK_INPUT_DOOR_TG2, 				//扩展门状态信号		
		CHECK_INPUT_ACC,							//点火信号	
		CHECK_INPUT_BREAK,						//刹车信号
		CHECK_INPUT_HIGH_BEAM,				//远光灯
		CHECK_INPUT_LOW_LIGHT,				//小灯
		CHECK_INPUT_LEFT_LIGHT,				//左转灯
		CHECK_INPUT_RIGHT_LIGHT,			//右转灯
		CHECK_INPUT_AIR_CON,					//空调
		CHECK_INPUT_SOS,							//紧急报警				
		CHECK_INPUT_KEY,							//一键通话
		CHECK_INPUT_HORN,							//喇叭		
		CHECK_INPUT_Oil_Pressure, 
		CHECK_INPUT_NEAR_LIGHT,
		CHECK_INPUT_DOORI_P,
		CHECK_INPUT_MAX
}E_CHECK_INPUT;

enum 
{
    CHECK_OUTPUT_EMPTY=0,//
    CHECK_OUTPUT_LCD,//
    CHECK_OUTPUT_CAM,// 
    CHECK_OUTPUT_LED,//    
    CHECK_OUTPUT_MAX
}E_CHECK_OUTPUT;

/*************************************************************************
*	函 数 名: FCT_AppInit
*	功能说明: FCT相关参数初始化
*	形    参：无
*	返 回 值: 无
***************************************************************************/
void FCT_AppInit(void)
{
    static unsigned char sucFirstRunflag=0;    

    //初始化串口
    CommParamInit();
    //CommTest2Init();
    //COMM_Initialize(COM3,9600);
    CommTest4Init();//gps
    CommTest5Init(); //gsm
    //CommTest6Init();     
    //初始化定时器3
    Init_Tim3Pluse();
    Init_ExtPluse();
    // 初始化录音
    //Audio_Record_Init();
    //////////////////
    ReadRtc();
    g_fct.time = RTC_GetCounter();
    g_fct.iis = false;
    g_fct.gprs = false;
    g_fct.debug_f = 0;
    if(0==sucFirstRunflag)
		{			
			#ifdef	RT_USING_CONSOLE		 
			rt_kprintf("EGS701GB-III-FCT初始化...\r\n");
			#endif	 
			//Public_PlayTTSVoiceStr("EGS702FCT初始化");
    }
		sucFirstRunflag = 1;
		rt_thread_delay( RT_TICK_PER_SECOND/2 ); 

		Public_ParameterInitialize();
}

/*************************************************************************
*	函 数 名: Task_DebugCommstrncmp
*	功能说明: 比较
*	形    参：无
*	返 回 值: 无
***************************************************************************/
unsigned char Task_DebugCommstrncmp(char *src)
{
    return strncmp(src,(char const *)FCT_recv.buf,strlen(src));
}
/*************************************************************************
*	函 数 名: App_EyeDebugAck
*	功能说明: 应答调试信息
*	形    参：无
*	返 回 值: 无
***************************************************************************/
void App_EyeDebugAck(void)
{
    char buf[100];
    unsigned char temp[30];
    unsigned char count,len; 
    memset(buf,0,66);
    if(E2_DEVICE_ID_LEN==EepromPram_ReadPram(E2_DEVICE_ID,temp))
    {
        strcpy(buf,"OK,");
        Public_ConvertBcdToAsc((unsigned char *)&buf[3],temp,E2_DEVICE_ID_LEN);
    }
    else
    {
        strcpy(buf,"OK,01721140600001");
    }
    len=strlen(buf);
    E2prom_ReadByte(ADDR_COUNT,&count,1);
    if(0xff==count)count = 1;
    sprintf(&buf[len],",EGS701GB-III-FCT,"FCT_SOFT_VER",%d\r",count);
    
		#ifdef	RT_USING_CONSOLE			
			rt_kprintf("%s",buf);
			//stm32_serial1_write(0,(unsigned char *)buf,strlen(buf));
		#endif
    /////////////
    g_fct.debug_f = 1;
}
/*************************************************************************
*	函 数 名: App_CheckCount
*	功能说明: 读取执行测试次数，
*	形    参：无
*	返 回 值: 无
***************************************************************************/
void App_CheckCount(void)
{
    char buf[20];
    u8 count;    
    E2prom_ReadByte(ADDR_COUNT,&count,1); 
    if(count<0xff)count++;
    else count=1; 
    E2prom_WriteByte(ADDR_COUNT,count);
    sprintf(buf,"OK,%d\r",count);
		#ifdef	RT_USING_CONSOLE			
			rt_kprintf("%s",buf);
		#endif
    ////////////////////    
}
/*************************************************************************
*	函 数 名: Api_CheckPower
*	功能说明: 
*	形    参：无
*	返 回 值: 无
//注意，提示测试字符“” 所有指令的标点符号只认半角，不认全角
*************************************************************************/
void Api_CheckPower(void)
{
    char buf[20];
    float vol;
    vol=Adc_MainPower_Readl(); 
    sprintf(buf,"OK,%.0f\r",vol/1000.0+0.5);
		#ifdef	RT_USING_CONSOLE			
			rt_kprintf("%s",buf);
		#endif
}
/*************************************************************************
*	函 数 名: Api_CheckBattery
*	功能说明: 
*	形    参：无
*	返 回 值: 无
//注意，提示测试字符“” 所有指令的标点符号只认半角，不认全角
*************************************************************************/
void Api_CheckBattery(void)
{
    char buf[10];
    float vol;
    //充电
    if(FCT_recv.buf[12]=='1')
    {
        //GpioOutOn(BAT_CHARGE);
        vol=Adc_BatteryPower_Readl();
        sprintf(buf,"OK,%.0f\r",vol/1000);
				#ifdef	RT_USING_CONSOLE			
				rt_kprintf(buf);
				#endif
    }
    else
    //放电
    if(FCT_recv.buf[12]=='0')
    {    
        //GpioOutOff(BAT_CHARGE);
        vol=Adc_BatteryPower_Readl();
        sprintf(buf,"OK,%.0f\r",vol/1000);
				#ifdef	RT_USING_CONSOLE			
				rt_kprintf(buf);
				#endif
    }
    else
    {
    		#ifdef	RT_USING_CONSOLE			
				rt_kprintf("ERR=5\r");
				#endif
    }
}
/*************************************************************************
*	函 数 名: App_CheckMyself
*	功能说明: 设备自检
*	形    参：无
*	返 回 值: 无
***************************************************************************/
void App_CheckMyself(void)
{
    unsigned char tmp[8];
    unsigned char flag=0;
    unsigned long addr,val,val1;

    //检测25LC320A
    E2prom_ReadByte(EE25LC320A_MAX_ADDR,&tmp[0],1);
    E2prom_WriteByte(EE25LC320A_MAX_ADDR,0xaa);
    E2prom_ReadByte(EE25LC320A_MAX_ADDR,&tmp[1],1);
    //写回数据
    E2prom_WriteByte(EE25LC320A_MAX_ADDR,tmp[0]);
    if(tmp[1]==0xaa)
    {
        flag&=~0x01;//0K
    }
    else
    {
        flag|=0x01;//ERR
        #ifdef	RT_USING_CONSOLE			
				rt_kprintf("ERR=1,\"93C86\"\r");
				#endif
        return;		
    }
    /////////////////////
    //读取BMA250E  
    flag&=~0x02;//0K
    ///////////////////
    //检测FM25C160
    tmp[2]=0x5a;
    tmp[3] = FRAM_ReadStatusRegister();
    //清除写忙标志
    FRAM_WriteDisable();
    FRAM_BufferWrite(FM25C160_MAX_ADDR,&tmp[2],1);
    FRAM_BufferRead(&tmp[0], 1, FM25C160_MAX_ADDR);
    if(tmp[0]==0x5a)
    {

        flag&=~0x04;//0K
    }
    else
    {
        flag|=0x04;//ERR
        #ifdef	RT_USING_CONSOLE			
				rt_kprintf("ERR=1,\"FM25C160\"\r");
				#endif
        return;
    }

    //检测FLASH AT25DF321   
    addr=FLASH_DATA_MAX_ADDR;
		sFLASH_EraseSector(addr);
    ///////////////////////////////////
    Public_ConvertLongToBuffer(0x12345678,tmp);
		sFLASH_WriteBuffer(tmp, addr, 4);
    ////////////////
    memset(tmp,0,sizeof(tmp));
    sFLASH_ReadBuffer(tmp,addr,4);
    val1=Public_ConvertBufferToLong(tmp);
    if(0x12345678==val1)
    {
        flag&=~0x08;//0K
    }
    else
    {
        flag|=0x08;//ERR
        #ifdef	RT_USING_CONSOLE			
				rt_kprintf("ERR=1,\"Flash\"\r");
        #endif
				return;
    }
    //检测时间秒
    //读取RTC时间秒
    val= RTC_GetCounter();
    if(val != g_fct.time)
    {
        flag&=~0x10;//0K
    }
    else
    {

        flag|=0x10;//ERR
        #ifdef	RT_USING_CONSOLE			
				rt_kprintf("ERR=1,\"RTC\"\r");
				#endif
        return;
    }
    /*//IIS音频录音自检
    if(g_fct.iis)
    {
        flag&=~0x20;//0K
    }
    else
    {
        flag|=0x20;//ERR
        FCT_AppSendString("ERR=1,\"IIS\"\r");
        return;
    }*/
    if(!flag)
    {
    	#ifdef	RT_USING_CONSOLE			
			rt_kprintf("OK\r");
			#endif
		}
}

/*************************************************************************
*	函 数 名: App_CheckGsmGprsTimeOut
*	功能说明: 
*	形    参：无
*	返 回 值: 无
***************************************************************************/
void App_CheckGsmGprsTimeOut(void)
{
    if(false==g_fct.gprs)return;
    g_fct.gprs=false;
		#ifdef	RT_USING_CONSOLE			
		rt_kprintf("ERR=1,\"GPRS=ERR\"\r");
		#endif
}
/*************************************************************************
*	函 数 名: App_CheckGsmGprs
*	功能说明: 检测GSM模块的通讯功能，采取透传模式，直接传输AT到GSM，GSM返回数据回传
*	形    参：GPRS/GSM检测指令 逗号分隔 OD 0A 
*	返 回 值: 无
//注意，所有指令的标点符号只认半角，不认全角
***************************************************************************/
void App_CheckGsmGprs(void)
{
    unsigned long timeout;
    u8 buf[40],i=0;//tmp_buf[20];
    //获取指令与时间基准
    while(FCT_recv.buf[10+i])
    {
        if(FCT_recv.buf[10+i]==',')
            break;
        if(i>30)
            break;
        buf[i]=FCT_recv.buf[10+i+1];
        i++;
    }
    if(FCT_recv.buf[10]!='"'||FCT_recv.buf[10+i-1]!='"')
    {
        //AT错误
        #ifdef	RT_USING_CONSOLE			
				rt_kprintf("ERR=1,\"\"\"or ,\"\r");
				#endif
        return;
    }
    //计算延迟时间
    timeout=atoi((char const *)&FCT_recv.buf[10+i+1]); 
    if(0==timeout)//时间默认
    {
        timeout=10;
    }	
    buf[i-2]='\r';	
    //发送AT指令到GSM-GPRS模块
    g_fct.gprs=true;
    CommTest5_SendData(buf,i-1);
    Public_SetTaskTimer(App_CheckGsmGprsTimeOut,PUBLICSECS(timeout));
}
/*************************************************************************
*	函 数 名: App_DisposeGprsRecvData
*	功能说明: GSM返回的数据回传FCT,过程中0x0d,0x0a用0x20代替
*	形    参：无
*	返 回 值: 无
***************************************************************************/
void App_DisposeGprsRecvData(unsigned char *pBuffer,unsigned short datalen)
{
    char buffer[100];
    unsigned short i;
    unsigned char temp;
    if(false==g_fct.gprs)return;
    g_fct.gprs = false;
    memset(buffer,0,100);
    memcpy(buffer,"OK,",3);
    if(datalen>95)datalen=95;
    for(i=0;i<datalen;i++)
    {
        temp=pBuffer[i];
        if((0x0d==temp)||(0x0a==temp))temp=0x20;
        ///////////////
        buffer[3+i]=temp;
    }
    buffer[3+i++]=0x0d;
    
		#ifdef	RT_USING_CONSOLE			
			rt_kprintf("%s",buffer);
		#endif
}
/*************************************************************************
*	函 数 名: App_CheckDigitalInput
*	功能说明: 检测数字输入信号
*	形    参："EYE+SIN"
*	返 回 值: 无
***************************************************************************/
ErrorStatus App_CheckDigitalInput(void)
{
    char data[4];
    unsigned char temp,channel=0;//,i;
    if(FCT_recv.buf[9]==',')
    {
        temp =1;
    }
    else 
    if(FCT_recv.buf[10]==',')
    {								
        temp =2;
    }
    //指令接收错误
    else
    {
    		#ifdef	RT_USING_CONSOLE			
				rt_kprintf("ERR=3\r");
        #endif
				return ERROR;
    }
    memcpy(data,&FCT_recv.buf[8],temp);
    data[temp]=0;
    channel=atoi(data);
    ////////////////
    ////////////////////

    switch(channel)
    {
				case CHECK_INPUT_DOORI_N:////门负触发状态信号,低有效(终端默认高电平)
            temp=gpio_pin_read(PIN_NO_DOORI_N);            
            break;
				case CHECK_INPUT_DOOR_TG2://DOOR_TG2
            temp=gpio_pin_read(PIN_NO_DOORI2);                
            break;
         case CHECK_INPUT_ACC://ACC检测
            temp=gpio_pin_read(PIN_NO_ACC);	
            break; 
				case CHECK_INPUT_BREAK://制动（刹车）
            temp=gpio_pin_read(PIN_NO_BREAK);	
            break;                       
        case CHECK_INPUT_HIGH_BEAM://检测远观灯
            temp=gpio_pin_read(PIN_NO_DLIGHTI);
            break;   
				case CHECK_INPUT_LOW_LIGHT:
            temp=gpio_pin_read(PIN_NO_XLIGHTI);
						break;
				case CHECK_INPUT_LEFT_LIGHT://左转向灯
            temp=gpio_pin_read(PIN_NO_LLIGHTI);           
            break;
        case CHECK_INPUT_RIGHT_LIGHT://右转向灯
            temp=gpio_pin_read(PIN_NO_RLIGHTI);
            break;
				case CHECK_INPUT_AIR_CON://空调
            temp=gpio_pin_read(PIN_NO_BKHI);           
            break;
        case CHECK_INPUT_SOS://紧急报警,低有效(终端默认高电平)
            temp=gpio_pin_read(PIN_NO_SWITCHI);	
            break;
				case CHECK_INPUT_KEY:////一键通话,低有效(终端默认高电平)
            temp=gpio_pin_read(PIN_NO_VIRBRATIONI);
            break;
        case CHECK_INPUT_HORN://喇叭
            temp=gpio_pin_read(PIN_NO_HORNI);
            break;
        case CHECK_INPUT_Oil_Pressure://机油压力标志位	
            temp=gpio_pin_read(PIN_NO_COIL);    
            break; 
        case CHECK_INPUT_NEAR_LIGHT://近光灯
            temp=gpio_pin_read(PIN_NO_DETCT1I);             
            break;      
        case CHECK_INPUT_DOORI_P:////门正触发状态信号
            temp=gpio_pin_read(PIN_NO_DOORI_P);
            break;        
         default:
         		#ifdef	RT_USING_CONSOLE			
						rt_kprintf("ERR=3,\"channel\"\r");
						#endif
            return ERROR;	
    }
    //返回检测状态
    if(temp)
		{		
				#ifdef	RT_USING_CONSOLE			
				rt_kprintf("OK,+\r");
				#endif
    }
		else
		{
				#ifdef	RT_USING_CONSOLE			
				rt_kprintf("OK,-\r");
				#endif
    }
		return SUCCESS;
}
/*************************************************************************
*	函 数 名: App_CheckDigitalOutput
*	功能说明: 检测数字输出信号
*	形    参：无
*	返 回 值: 无
***************************************************************************/
void App_CheckDigitalOutput(void)
{
    char data[20];
    u8 tmp,channel;
    if(FCT_recv.buf[10]==',')
    {
        if(FCT_recv.buf[11]=='+')
            tmp=1;
        else
            tmp=0;
    }
    else
    {
    		#ifdef	RT_USING_CONSOLE			
				rt_kprintf("ERR=3\r");
        #endif
				return;
    }
    //通道
    if(FCT_recv.buf[9]>'2'||FCT_recv.buf[9]<'1')
    {
    		#ifdef	RT_USING_CONSOLE			
				rt_kprintf("ERR=3,\"channel\"\r");
        #endif
				return;								
    }
    channel=FCT_recv.buf[9];	
    if(0==tmp)
    {
        if(channel=='1')
        {
            gpio_out_on(PIN_NO_HORN_OUT);   
        }
        else//油路控制
        {
            gpio_out_on(PIN_NO_COIL);
        }
        strcpy(data,"OK,-\r");
    }
    else
    {
        if(channel=='1')
        {
            gpio_out_off(PIN_NO_HORN_OUT);   
        }
        else//油路控制
        {
            gpio_out_off(PIN_NO_COIL);
        }	
        strcpy(data,"OK,+\r");
    }
    ////////////////
    rt_thread_delay( 20 );
		#ifdef	RT_USING_CONSOLE			
		rt_kprintf(data);
		#endif
}
/*************************************************************************
*	函 数 名: App_CheckAnalogInput
*	功能说明: 获取模拟输入信号电压值
*	形    参：无
*	返 回 值: 无
***************************************************************************/
void App_CheckAnalogInput(void)
{
    uchar buf[20];
    float vol;

    memset(buf,0,20);
    if(FCT_recv.buf[9]!=',')
    {//越界错误
    		#ifdef	RT_USING_CONSOLE			
				rt_kprintf("ERR=3\r");
        #endif
				return;
    }		
    //---------------------------------------------	
    Adc_CollectApp_TimeTask();
    //-------------------------------------------				
    //通道
    if(FCT_recv.buf[8]>'2'||FCT_recv.buf[8]<'1')				
    {
    		#ifdef	RT_USING_CONSOLE			
				rt_kprintf("ERR=3\r");
        #endif
				return;								
    }	
    else
    {
        if(FCT_recv.buf[8]=='1')//adc1
        {
            vol=Adc_ExternInputVoltage_1_Read();
        }
        else//ADC2
        {
            vol=Adc_ExternInputVoltage_2_Read();
        }
        //sprintf((char*)buf,"OK,%.0f\r",vol/1000);
				snprintf((char*)buf,20,"OK,%.0f\r",vol/1000);
        	
        #ifdef	RT_USING_CONSOLE			
				rt_kprintf("%s",(char *)buf);
				#endif
    }
}
/*************************************************************************
*	函 数 名: App_CheckAnalogOutput
*	功能说明: 获取模拟输入信号电压值
*	形    参：无
*	返 回 值: 无
***************************************************************************/
void App_CheckAnalogOutput(void)
{
    unsigned char channel,onflag;
    uchar buf[20];
    //获取时间
    memset(buf,0,20);
    if(FCT_recv.buf[10]!=',')
    {//越界错误
    		#ifdef	RT_USING_CONSOLE			
				rt_kprintf("ERR=3\r");
        #endif
				return;
    }		
    if(FCT_recv.buf[9]>'4'||FCT_recv.buf[9]<'1')				
    {
    		#ifdef	RT_USING_CONSOLE			
				rt_kprintf("ERR=3\r");
        #endif
				return;								
    }	
    else
    {
        channel=FCT_recv.buf[9]-0x30;
        if(FCT_recv.buf[12]==',')
        {
            onflag = FCT_recv.buf[11]-0x30;
        }
        else
        {
            onflag = 1;
        }
        //////////////
        switch(channel)
        {
            case CHECK_OUTPUT_LCD:
                if(onflag)
                gpio_out_on(PIN_NO_LCD_ON);
                else
                gpio_out_off(PIN_NO_LCD_ON);
                break;
            case CHECK_OUTPUT_CAM:
                if(onflag)
                gpio_out_on(PIN_NO_CAM_POWER);
                else
                gpio_out_off(PIN_NO_CAM_POWER);
                break;
            /*case CHECK_OUTPUT_LED:
                if(onflag)
                gpio_out_on(COMM4_PWR);
                else
                gpio_out_off(COMM4_PWR);
                break;*/
        }
    }
    ////////////////////
    rt_thread_delay(30);
    /////////////
    strcpy((char*)buf,"OK\r");
		#ifdef	RT_USING_CONSOLE			
		rt_kprintf((char *)buf);
		#endif
}
/*************************************************************************
*	函 数 名: App_CheckAntenna
*	功能说明: 天线检测指令
*	形    参：无
*	返 回 值: 无
//注意，提示测试字符“” 所有指令的标点符号只认半角，不认全角
***************************************************************************/
void App_CheckAntenna(void)
{
    uchar tmp=0;
    unsigned char channel;
    rt_thread_delay(20);
    channel = FCT_recv.buf[13]-0x30;    
    if(0==channel)//开路检测
    {
        tmp=gpio_pin_read(PIN_NO_GPS_OC);
        if(tmp==1)
        {
        		#ifdef	RT_USING_CONSOLE			
						rt_kprintf("OK,0\r");
            #endif
        }
				else
				{
						#ifdef	RT_USING_CONSOLE			
						rt_kprintf("OK,2\r");
        		#endif
				}
    }
    else 
    if(1==channel)//检测短路
    {
        tmp=gpio_pin_read(PIN_NO_GPS_SC);
        if(tmp==0)
        {
        		#ifdef	RT_USING_CONSOLE			
						rt_kprintf("OK,1\r");
            #endif
        }
				else
				{
						#ifdef	RT_USING_CONSOLE			
						rt_kprintf("OK,2\r");
        		#endif
				}
    }    
    else     	
    {
        tmp=gpio_pin_read(PIN_NO_GPS_OC);;	
        if(tmp==1)
        {
        		#ifdef	RT_USING_CONSOLE			
						rt_kprintf("OK,0\r");
            #endif
						return;
        }
        /////////////
        tmp=gpio_pin_read(PIN_NO_GPS_SC);
        if(tmp==0)
        {
        		#ifdef	RT_USING_CONSOLE			
						rt_kprintf("OK,1\r");
            #endif
						return;
        }
				#ifdef	RT_USING_CONSOLE			
				rt_kprintf("OK,2\r");
				#endif
    }
}
/*
************************************************************************
*	函 数 名: App_CheckComunication
*	功能说明: 
*	形    参：无
*	返 回 值: 无
//注意，提示测试字符“” 所有指令的标点符号只认半角，不认全角
**************************************************************************/
void App_CheckComunication(void)
{
    unsigned char i=0;
    unsigned char channel;
    while(FCT_recv.buf[8+i])
    {
        if(FCT_recv.buf[8+i]==',')
            break;
        i++;
        if(i>20)break;
    };
    if(FCT_recv.buf[8]!='"'||FCT_recv.buf[8+i-1]!='"')
    {
    		#ifdef	RT_USING_CONSOLE			
				rt_kprintf("ERR=3,\"\"\r");
        #endif
				return;				
    }	
    if(FCT_recv.buf[10+i]==',')
    {
        channel=FCT_recv.buf[9+i]-0x30;
        if(channel>6||channel<1)
        {
        		#ifdef	RT_USING_CONSOLE			
						rt_kprintf("ERR=3,\"Channel\"\r");
            #endif
						return;									
        }
    }
    else
    {
    		#ifdef	RT_USING_CONSOLE			
				rt_kprintf("ERR=3,\"Channel\"\r");
        #endif
				return;					
    }
		#ifdef	RT_USING_CONSOLE			
		rt_kprintf("OK\r");
		#endif
}
/*
************************************************************************
*	函 数 名: Api_CheckPwm
*	功能说明: 
*	形    参：EYE+PWM=10,125,3,3 表示：频率为1HZ，占空比为12.5%，持续时间3秒，超时时间3秒。
*	返 回 值: 无
************************************************************************
*/
void Api_CheckPwm(void)
{
    char buf[30],i=0,len,offset;//,tmp;
    while(FCT_recv.buf[8+i])
    {
        if(FCT_recv.buf[8+i]==',')
            break;
        i++;
    };
    ////////////
    i++;
    ///////////
    offset=i;
    //--占空比计算
    while(FCT_recv.buf[8+i])
    {
        if(FCT_recv.buf[8+i]==',')
            break;
        i++;
        if(i>(4+offset))//&&i<(2+4))//20)
        {
        		#ifdef	RT_USING_CONSOLE			
						rt_kprintf("ERR=3,\"width\"\r");
            #endif
						return;
            //break;	
        }
    };
    //
    TIM_Cmd(TIM3, ENABLE);   //使能定时器计时
    g_Pluse.flag=true;
    g_Pluse.time=0;
    g_Pluse.count=0;
    g_Pluse.end=0;
    while(!g_Pluse.end)
    {
        ;
    }
		TIM_Cmd(TIM3, DISABLE);   //使能定时器计时
    g_Pluse.count=g_Pluse.count*10;
    sprintf(buf,"OK,%d,",g_Pluse.count);
    len=strlen(buf);
    memcpy(&buf[len],&FCT_recv.buf[8+offset],i-offset);
    buf[len+i-offset]='\r';
    buf[len+i-offset+1]=0;
    	
		#ifdef	RT_USING_CONSOLE			
		rt_kprintf("%s",(char *)buf);
		#endif 
}

/*************************************************************************
*	函 数 名: Task_DebugCommIncode
*	功能说明: 
*	形    参：无
*	返 回 值: 无
***************************************************************************/
void Task_DebugCommIncode(void)
{
		if(Task_DebugCommstrncmp("EYE+RESET")==0)
		{
				#ifdef	RT_USING_CONSOLE			
				rt_kprintf("\nsystem would reset after one second\n");
				#endif
				rt_thread_delay( RT_TICK_PER_SECOND ); 				
				NVIC_SystemReset();
				return;
		}
		if(0==g_fct.debug_f)		
		{
				if(Task_DebugCommstrncmp("EYE+DEBUG")==0)
				{
						Public_SetOperateTimer(FCT_AppInit,PUBLICSECS(2*60));
						Public_SetTestTimer(App_EyeDebugAck,PUBLICSECS(1));
						//App_EyeDebugAck();
				}
		} 	
		else
		{
				Public_SetOperateTimer(FCT_AppInit,PUBLICSECS(2*60));
				////////////
				if(Task_DebugCommstrncmp("EYE+CHECKSELF")==0)
				{
						App_CheckMyself();							 
						return;
				} 				
				else	//3、读取执行测试次数，
				if(Task_DebugCommstrncmp("EYE+COUNT")==0)
				{
						App_CheckCount(); 							
						return;
				}
				else //4、检测数字输入信号
				if(Task_DebugCommstrncmp("EYE+SIN")==0)
				{
						//Public_SetTestTimer(App_CheckDigitalInput,PUBLICSECS(0.3));
						Timer10msStart(TIMER_10MS_ID_2,30,TIMER_MODE_ONCE,App_CheckDigitalInput);
						return;
				} 
				else //5、检测数字输出信号
				if(Task_DebugCommstrncmp("EYE+SOUT")==0)
				{
						App_CheckDigitalOutput(); 							
						return;
				}
				else //6、获取模拟输入信号电压值
				if(Task_DebugCommstrncmp("EYE+VIN=")==0)
				{
						Public_SetTestTimer(App_CheckAnalogInput,PUBLICSECS(0.5));
						return;
				} 	
				else //6S、获取模拟输入信号电压值
				if(Task_DebugCommstrncmp("EYE+VOUT=")==0)
				{
						App_CheckAnalogOutput();							 
						return;
				}
				else //7、天线检测指令
				if(Task_DebugCommstrncmp("EYE+GNSSANTE=")==0)
				{
						Public_SetTestTimer(App_CheckAntenna,PUBLICSECS(0.3));							
						return;
				} 																		
				else //8、GPS_GNSS检测指令
				if(Task_DebugCommstrncmp("EYE+GNSS=")==0)
				{
						if(g_Gnss.gnss_ok)
						{
							#ifdef	RT_USING_CONSOLE			
							rt_kprintf("OK\r");
							#endif
						}
						else
						{
							#ifdef	RT_USING_CONSOLE			
							rt_kprintf("ERR=1\r");
							#endif
						}
						return;
				} 			
				else	//9、GPRS/GSM检测指令 逗号分隔 OD 0A 
				if(Task_DebugCommstrncmp("EYE+MBCOM=")==0)
				{
						App_CheckGsmGprs(); 							
						return;
				}
				else	//10、COM通讯口  修改0XOD 0A 替换成','
				if(Task_DebugCommstrncmp("EYE+COM=")==0)
				{
						App_CheckComunication();								
						return;
				}
				else	 //11、输入电压检测
				if(Task_DebugCommstrncmp("EYE+POWER=")==0)
				{
						Public_SetTestTimer(Api_CheckPower,PUBLICSECS(0.3));
						return;
				}
				else	//12、电池充放电检测  GB-III只是摆设而已
				if(Task_DebugCommstrncmp("EYE+BATTERY=")==0)
				{
						Api_CheckBattery(); 							 
						return;
				}
				else	//13、PWM输入检测，不检测占空比
				if(Task_DebugCommstrncmp("EYE+PWM=")==0)
				{
						Api_CheckPwm(); 						 
						return;
				}
				else
				if(Task_DebugCommstrncmp("EYE+DEBUG")==0)
				{
						App_EyeDebugAck();
						return;
				}
				else	//其他错误
				{
						#ifdef	RT_USING_CONSOLE			
						rt_kprintf("ERR=3\r");	
						#endif
						return;
				} 	 
		}
		
}

/*************************************************************OK
** 函数名称: fct_app_thread_entry
** 功能描述: 硬件定时器线程入口
** 入口参数: 
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
static void fct_app_thread_entry(void* parameter)
{     
	char buf[FCT_MSG_LEN];

	FCT_AppInit();
	//Public_ParameterInitialize();
	fct_app_msg = rt_mq_create("fct_app_msg",10,5,RT_IPC_FLAG_FIFO);

	while (1)    
	{        
		/* 从消息队列中接收消息 */
		if (rt_mq_recv(fct_app_msg, &buf[0], sizeof(buf), RT_WAITING_FOREVER)
		== RT_EOK)
		{			
			if(strstr("fct_debug",buf))
			{
				Task_DebugCommIncode();
			}		
			//#ifdef	RT_USING_CONSOLE	 
			//	rt_kprintf("%s...\r\n",buf);
			//#endif			
			rt_thread_yield();        
		}
	}
}
/*************************************************************OK
** 函数名称: fct_app_thread_init
** 功能描述: 
** 入口参数: 
** 出口参数: 
** 返回参数: 
** 全局变量: 无
** 调用模块:
*************************************************************/
void fct_app_thread_init(void)
{
	rt_err_t result;
	
	result = rt_thread_init(&fct_app_thread,                            
							"fct_app_task",                            
							fct_app_thread_entry,                            
							RT_NULL,                            
							(rt_uint8_t*)&fct_app_stack[0],                            
							sizeof(fct_app_stack),                            
							FCT_APP_THREAD_PRIORITY,                            
							5);    
	if (result == RT_EOK)    
	{        
		rt_thread_startup(&fct_app_thread);    
	} 
}


