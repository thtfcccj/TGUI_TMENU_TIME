/*******************************************************************************

 			            LCD硬件及TGUI显示接口-在TftDrv中的实现
此模块不负责对显示设备的初始化,仅支持单个显示屏
此模块在测试模式时不再刷屏
*******************************************************************************/
#ifndef __LCD_DRV_H
#define __LCD_DRV_H

#ifdef SUPPORT_EX_VFB
  #include "TftDbi.h"
#else
  #include "TftDrv.h"
#endif

#include "TWinMng_Row.h"//RowMask_t

/****************************************************************************
							             相关配置															 
****************************************************************************/

//系统是否有保留的X区域，即列, 可用于使显示一部分受TGUI管辖，一部分自绘图
//#define SUPPORT_LCD_DEV_RESERVE_X //全局里定义   

/****************************************************************************
							             回调函数																 
****************************************************************************/

//----------------------------是否在测试状态--------------------------------
//测试状态更新显示屏由外部实现
//void LCD_Drv_cbIsTest(void);
#include "TftTest.h"
#define LCD_Drv_cbIsTest() (TftTest_IsDoing())

//-------------------------该行是否允许更新----------------------------------
//可用于测试模式，或某行在特定情况下当作其它用途使用
RowMask_t LCD_Drv_cbLineIsEn(unsigned char y);


//-------------------------得到x轴像素偏移----------------------------------
//可用于指定TGUI区域x轴起始,默认实现返回为0
unsigned char  LCD_Drv_cbGetXpixelOffset(void);

//-------------------------得到y轴像素偏移----------------------------------
//可用于像素与16不对齐时情况,或指定TGUI区域y轴起始,默认实现返回为0
unsigned char  LCD_Drv_cbGetYpixelOffset(void);

//--------------------------修正该行起始--------------------------------------
#ifdef SUPPORT_LCD_DEV_RESERVE_X //保留有X区域时需定义
//返回往后偏移字符个数
unsigned char LCD_Drv_cbReviseStartX(unsigned char y, //所在行
                                      unsigned char StartX);//起始位置
#endif

//--------------------------修正该行结束--------------------------------------
#ifdef SUPPORT_LCD_DEV_RESERVE_X //保留有X区域时需定义
//返回往前偏移字符个数
unsigned char LCD_Drv_cbReviseEndX(unsigned char y, //所在行
                                      unsigned char EndX);//结束位置
#endif
  
//-------------------------得到指定位置文字颜色信息流--------------------------
//每个Color_t表示对应位置起始字的颜色，可用于对显示字进行着色
const Color_t *LCD_Drv_pcbGetPenColor(unsigned char x,      //需要更新的行起始位置
                                      unsigned char y);      //需要更新的行

//--------------------------得到指定位置背景颜色信息流--------------------------
//每个Color_t表示对应位置起始字的背景颜色，可用于对显示字进行着色
const Color_t *LCD_Drv_pcbGetBrushColor(unsigned char x,      //需要更新的行起始位置
                                        unsigned char y);      //需要更新的行

//-------------------------------得到焦点时的背景色-----------------------------
//焦点位置的背景色此以此为标准
Color_t LCD_Drv_cbGetForcusBrushColor(void);



#endif

