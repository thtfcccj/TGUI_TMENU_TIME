/*******************************************************************************

 			            LCD硬件及TGUI显示接口之t6963驱动实现接口
 
*******************************************************************************/
#ifndef __LCD_DRV_H
#define __LCD_DRV_H

/****************************************************************************
							              相关配置															 
****************************************************************************/
#define     LCD_DEV_WIDTH    240 //显示屏宽度
#define     LCD_DEV_HIGH     128 //显示屏高度    

//支持测试功能时定义
#define SUPPORT_LCD_TEST

/****************************************************************************
							               相关函数																 
****************************************************************************/

//-------------------------模块初始化---------------------------------------
void Lcd_Drv_Init(void);	


/****************************************************************************
							               显示屏测试相关																 
****************************************************************************/
#ifdef SUPPORT_LCD_TEST

//定义显示屏测试相关时间,0.5s为单位
#define LCD_DEV_TEST_COUNT     25  //测试总时间
#define LCD_DEV_TEST_FULL      20  //满屏测试结束时刻
#define LCD_DEV_TEST_NULL      15  //空屏测试结束时刻
#define LCD_DEV_TEST_REFRESH   (LCD_DEV_TEST_NULL - 7)//刷屏测试结束时刻
//#define LCD_DEV_TEST_END_WAIT  0   //结束等待时间

extern unsigned char Lcd_Drv_TestTimer;  //测试定时器
//-------------------------测试任务函数---------------------------------------
//任务函数,0.5s一次
void Lcd_Drv_TestTask(void);	

//-------------------------进入测试状态---------------------------------------
//void Lcd_Drv_EnterTest(void);	
#define Lcd_Drv_EnterTest() \
  do{Lcd_Drv_TestTimer = LCD_DEV_TEST_COUNT;}while(0)

//-------------------------退出测试状态---------------------------------------
void Lcd_Drv_QuitTest(void);	
#define Lcd_Drv_QuitTest() do{Lcd_Drv_TestTimer = 0;}while(0)

#endif

/****************************************************************************
							              回调函数																 
****************************************************************************/

//----------------------延时时间控制--------------------------
//void Lcd_cbDelayUs(unsigned char Us);
//void Lcd_cbDelayMs(unsigned char Ms);
//直接实现:
#include    "math_2.h"
#define Lcd_cbDelayUs(Us)  Delay40Ns((unsigned int)(Us) * 10)
#define Lcd_cbDelayMs(Ms)  Delay40Ns((unsigned int)(Ms) * 10000)

//--------------------测试完成通报------------------------
//void Lcd_Drv_cbTestFinalNotify(void);
//直接实现为:
#include    "Gui_Task.h"
#define Lcd_Drv_cbTestFinalNotify()  do{GUI_Task_ReturnMain();}while(0)

#endif

