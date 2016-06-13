//*********************************************************************
//
//			T6963C底层驱动命令头文件
//
//此头文件封装了T6963C的操作接口，供用户LCD程序调用
//当控制方式改变时,需修改此文件的实现
//*********************************************************************/

#ifndef __T6963C__COMMAND_H
#define __T6963C__COMMAND_H

/************************************************************************
			LCD控制IO接口层实现
 .修改此接口与实际定义的相对应
 .需修改硬件初始化IO函数													 
************************************************************************/
#include "Parallel.h"
#include "ParallelSel.h"
#include "ParallelCtrl.h"

//选择显示屏
#define		T6963C_CsEn()	do{ParallelSel_Dev(PARALLEL_SEL_LCD);}while(0)
//关显示屏
#define		T6963C_CsDis()	do{ParallelSel_Idie();}while(0)

//选择数据
#define		T6963C_SelData()   do{ParallelCtrl_ClrA0();}while(0)
//选择指令
#define		T6963C_SelCmd()  do{ParallelCtrl_SetA0();}while(0)

//读信号
#define		T6963C_SetRd()	do{ParallelCtrl_SetRd();}while(0)
#define		T6963C_ClrRd()	do{ParallelCtrl_ClrRd();}while(0)

//写信号
#define		T6963C_SetWr()	do{ParallelCtrl_SetWr();}while(0)
#define		T6963C_ClrWr()	do{ParallelCtrl_ClrWr();}while(0)

//置为输出
#define		T6963C_SetOut()	do{Parallel_SetOutput();}while(0)
//置为输入
#define		T6963C_SetIn()	do{Parallel_SetInput();}while(0)
//写数据
#define		T6963C_SetData(Data) do{Parallel_WrData(Data);}while(0)

//读数据
#define		T6963C_GetData()	(Parallel_RdData())

/************************************************************************
			读写延时时间实现												 
************************************************************************/

#include "math_2.h"
//读数据延时时间	
#define		T6963C_RdDelay() do{Delay40Ns(20);}while(0)
//写数据延时时间
#define		T6963C_WrDelay() do{Delay40Ns(20);}while(0)		


/************************************************************************
			驱动层实现												 
************************************************************************/
#include "T6963C_Drv.h"

//--------------------------底层IO口初始化-------------------------------
//由用户初始化其IO口到默认状态
//void T6963C_HwInit(void);
//并口已单独完成初始化
#define T6963C_HwInit() do{}while(0)

//--------------------------顺列写数据----------------------------------
//调用T6963_StartAutoWr()可调用此函数连续写数据
void T6963C_SqureWrData(const unsigned char *pData,
                        unsigned short Len);


#endif
