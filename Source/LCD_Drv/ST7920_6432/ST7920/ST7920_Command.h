//*********************************************************************
//
//			ST7920底层驱动命令头文件
//
//此头文件封装了ST7920的操作接口，供用户LCD程序调用
//当控制方式改变时,需修改此文件的实现
//*********************************************************************/

#ifndef __ST7920__COMMAND_H
#define __ST7920__COMMAND_H

/*-----------------------------------------------------------------------------------------

					LCD控制IO接口层实现
 .修改此接口与实际定义的相对应
 .需修改硬件初始化IO函数
----------------------------------------------------------------------------------------*/
#include "Apublic.h"//LCD-CMD,set_bit,_nop
#include "Delay.h"



//开上半屏
#define		Lcd_OnUpperScreen()	do{set_bit(LCD_CTRLPORT,LCD_E);}while(0)	//读写使能信号E1，1有效
//开下半屏
//#define		Lcd_OnUnderScreen()	do{PSD8xx_reg.DATAOUT_C |= 0x08;}while(0)	//读写使能信号E2，1有效
#define		Lcd_OnUnderScreen()	Lcd_OnUpperScreen()
//关上半屏
#define		Lcd_OffUpperScreen()	do{clear_bit(LCD_CTRLPORT,LCD_E);}while(0)	//读写使能信号E1，1有效
//关下半屏
//#define		Lcd_OffUnderScreen()	do{PSD8xx_reg.DATAOUT_C &= ~0x08;}while(0)	//读写使能信号E2，1有效
#define		Lcd_OffUnderScreen()		Lcd_OffUpperScreen()
//选择数据
#define		Lcd_D_Select()   do{set_bit(LCD_CTRLPORT,LCD_RS);}while(0)	//数据指令选择信号RS；1为数据操作
//选择指令
#define		Lcd_I_Select()   do{clear_bit(LCD_CTRLPORT,LCD_RS);}while(0)	//数据指令选择信号RS；0为写指令或读状态
//读选择信号
#define		Lcd_RD_Select()		do{set_bit(LCD_CTRLPORT,LCD_R_W);}while(0)	//读写选择信号RW；1为读选通
//写选择信号
#define		Lcd_WR_Select()		do{clear_bit(LCD_CTRLPORT,LCD_R_W);}while(0)	//读写选择信号RW；0为写选通

//置为输出
#define		Lcd_Set_Out()		do{BUS_DDR = 0xFF;}while(0)	//P4口实现，不需特别置为输出
//写数据
#define		Lcd_Set_Data(Data)		do{BUS_PORT = Data;}while(0)
//置为输入
#define		Lcd_Set_In()		do{BUS_DDR = 0x00;BUS_PORT = 0xff;}while(0)	//P4口实现，0xff即为输入
//读数据
#define		Lcd_Get_Data()			(BUS_PIN)

/*-----------------------------------------------------------------------------------------

					LCD控制读写延时时间实现

----------------------------------------------------------------------------------------*/
#define		Lcd_RdDelay()		do{vDelayUs(20);}while(0)		//读数据延时时间	
#define		Lcd_WrDelay()		do{vDelayUs(20);}while(0)		//写数据延时时间


/*-----------------------------------------------------------------------------------------

										驱动层实现

----------------------------------------------------------------------------------------*/
#include "ST7920_Drv.h"

//-------------------底层IO口初始化------------------------
//由用户初始化其IO口到默认状态
void Lcd_HwInit(void);

//-------------------LCD写指令---------------------------		
//上半屏写指令
void Lcd_UpperWrCommand(unsigned char Command);

//下半屏写指令
//void Lcd_UnderWrCommand(unsigned char Command) ;
#define	Lcd_UnderWrCommand(Command) Lcd_UpperWrCommand(Command) 	

//-------------------LCD写数据准备---------------------------
//上半屏写数据前调用此函数
void Lcd_UpperWrDataRdy(void);

//下半屏写数据前调用此函数
//void Lcd_UnderWrDataRdy(void);
#define	Lcd_UnderWrDataRdy()	Lcd_UpperWrDataRdy()

//-------------------LCD写数据---------------------------
//写单个或连续数据,写第一个数据前必须调用写准备函数

//上半屏写数据
void Lcd_UpperWrData(unsigned char uData);
//下半屏写数据
//void Lcd_UnderWrData(uData);
#define	Lcd_UnderWrData(uData) Lcd_UpperWrData(uData)

#endif
