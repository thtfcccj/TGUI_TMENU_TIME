/*******************************************************************************

                     TGUI颜色实现
此模块配合Lcd_Drv/TFT回调，可实现文本，边框等着色功能
*******************************************************************************/

#ifndef __TGUI_COLOR_H
#define __TGUI_COLOR_H

/*******************************************************************************
                            相关配置
*******************************************************************************/

#include "TGUI.h"
#include "Color.h" //ComGui仓库里定义

//-----------------------------颜色配置------------------------------

//默认列表内容前景色
#ifndef TGUI_COLOR_DEFAULT_PEN_DATA
  #define   TGUI_COLOR_DEFAULT_PEN_DATA      RGBFFFFFF  //白色
#endif
//默认列表标题前景色
#ifndef TGUI_COLOR_DEFAULT_PEN_TITLE
  #define   TGUI_COLOR_DEFAULT_PEN_TITLE     RGBFFFFFF  //白色
#endif
//默认列表内容背景色
#ifndef TGUI_COLOR_DEFAULT_BRUSH_DATA
  #define   TGUI_COLOR_DEFAULT_BRUSH_DATA    RGB000000   //黑色
#endif
//默认列表标题背景色
#ifndef TGUI_COLOR_DEFAULT_BRUSH_TITLE
  #define   TGUI_COLOR_DEFAULT_BRUSH_TITLE   RGB333333   //深灰色
#endif
//焦点区背景色
#ifndef TGUI_COLOR_DEFAULT_BRUSH_FOCUS
  #define   TGUI_COLOR_DEFAULT_BRUSH_FOCUS   RGB3333FF   //偏蓝色
#endif
/*******************************************************************************
                            相关结构
*******************************************************************************/

//主界面时，空间分配结构
struct _TGuiColor{
  //字体颜色控制缓冲区:
  Color_t MainPenColor[TLCD_HIGH][TLCD_WIDTH];      //主显示前景色控制
  Color_t MainBrushColor[TLCD_HIGH][TLCD_WIDTH];    //主显示背景色控制
  RowMask_t LcdLineEnMask;   //TGUI中，显示屏行更新允许掩码  
};

extern struct _TGuiColor TGuiColor;

/*******************************************************************************
                              相关函数
*******************************************************************************/

//--------------------------------初始化函数----------------------------------
//开机时调用
void TGuiColor_Init(unsigned char TitleY);//默认标题栏行位置,255表示无

//--------------------------------置为默认函数----------------------------------
//恢复默认配色
void TGuiColor_SetDefault(unsigned char TitleY);

//---------------------------允许与禁止TGUI的所有显示--------------------------
#define TGuiColor_EnTGuiDispAll() do{TGuiColor.LcdLineEnMask = (RowMask_t)-1;}while(0)
#define TGuiColor_DisTGuiDispAll() do{TGuiColor.LcdLineEnMask = 0;}while(0)

//---------------------------允许与禁止TGUI的部分显示--------------------------
#define TGuiColor_EnTGuiDisp(mask)  do{TGuiColor.LcdLineEnMask |= (mask);}while(0)
#define TGuiColor_DisTGuiDisp(mask) do{TGuiColor.LcdLineEnMask &= ~(mask);}while(0)

//-------------------------------设置行坐标整体颜色函数--------------------------
void TGuiColor_SetPenColor(unsigned char y,
                        unsigned char x,
                        unsigned char xLen,
                        Color_t Color);

//-------------------------------设置行坐标对应颜色函数--------------------------
void TGuiColor_SetPenColorP(unsigned char y,
                        unsigned char x,
                        unsigned char xLen,
                        const Color_t *pColor);


#endif //__TGUI_COLOR_H