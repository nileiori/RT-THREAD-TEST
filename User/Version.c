/*******************************************************************************
 * File Name:           Version.c
 * Function Describe:   
 * Relate Module:		Version.h
 * Writer:              joneming
 * Date:                2014-4-11
 * Rewriter:            joneming
 ******************************************************************************/
#include "include.h"
//////////////////////////
#define SOFTVERSION                 01//版本号:两位:表示当前版本号
/////////////////////////////////////////
#define FIRMWARE_TIME               "201404140940"//编译时间
///////////////////////////////////////////////
#define HARDWARE_VERSION             "Ver:1.41"//硬件版本号
///////////////////////////////
#if (MOD_TYPE == GPRS_MODULE_M10)//网络类型
#define NETWORK_TYPE                11//两位:表示网络类型:奇数表示G网通用版本
#else
#define NETWORK_TYPE                12//两位:表示网络类型:偶数表示G网通用版本
#endif
/////////////////////////////////
//////////以下宏不需要修改///////
//////////////////////////////////////////////
#define SOFTVERSIONS                STR(SOFTVERSION)
#define NETWORK_TYPES               STR(NETWORK_TYPE)
#define FIRMWARE_VERSION            "2"NETWORK_TYPES""SOFTVERSIONS//版本号共5位数字，//固件版本号,2表示702,01表示G网通用版本,02表示C网通用版(华为模块),04表示C通用版(中兴模块)
///////////////////////////////
#define FIRMWARE_INFORMATION        "Ver:"FIRMWARE_VERSION"_"FIRMWARE_TIME//固件信息(包括版本号及日期)
///////////////////////////////////
/********************************************
 * Function: Version_UpdateSoftVersion
 * Describe: 更新当前软件版本号
**********************************************/
void Version_UpdateSoftVersion(void)
{
    EepromPram_WritePram(E2_FIRMWARE_VERSION_ID,(unsigned char *)FIRMWARE_VERSION, E2_FIRMWARE_VERSION_ID_LEN);
}
/********************************************
 * Function: Version_GetFirmwareInformation
 * Describe: 获取当前固件信息(版本号+编译日期)
 * 返回    : 长度
**********************************************/
unsigned char Version_GetFirmwareInformation(unsigned char *buffer)
{
    strcpy((char *)buffer,FIRMWARE_INFORMATION);
    return strlen(FIRMWARE_INFORMATION);
}
/********************************************
 * Function: Version_GetFirmwareInformation
 * Describe: 获取当前硬件信息(版本号)
 * 返回    : 长度
**********************************************/
unsigned char Version_GetHardwareInformation(unsigned char *buffer)
{
    strcpy((char *)buffer,HARDWARE_VERSION);
    return strlen(HARDWARE_VERSION);
}
/******************************************************************************
								EndModule
******************************************************************************/