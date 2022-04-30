/*******************************************************************************

                     TGUI颜色实现
此模块配合Lcd_Drv/TFT回调，可实现文本，边框等着色功能
*******************************************************************************/
#include "TGuiColor.h"
#include <string.h>

struct _TGuiColor TGuiColor;

/****************************************************************************
							             主要函数实现																 
****************************************************************************/

//--------------------------------初始化函数----------------------------------
//开机时调用
void TGuiColor_Init(unsigned char TitleY)//默认标题栏行位置,255表示无
{
  //初始化结构
  memset(&TGuiColor, 0, sizeof(struct _TGuiColor));
  TGuiColor_SetDefault(TitleY);//恢复默认配色
}

//--------------------------------置为默认函数----------------------------------
//恢复默认配色
void TGuiColor_SetDefault(unsigned char TitleY)
{
  //颜色区置默认
  Color_Full(TGUI_COLOR_DEFAULT_PEN_DATA,
             TGuiColor.MainPenColor[0],
             sizeof(TGuiColor.MainPenColor) / sizeof(Color_t));
  Color_Full(TGUI_COLOR_DEFAULT_BRUSH_DATA, 
             TGuiColor.MainBrushColor[0],
             sizeof(TGuiColor.MainPenColor) / sizeof(Color_t));   
  //标题栏
  if(TitleY < TLCD_HIGH){
    Color_Full(TGUI_COLOR_DEFAULT_PEN_TITLE, 
               TGuiColor.MainPenColor[0],
               TGuiColor_cbGetCurMainW());
    Color_Full(TGUI_COLOR_DEFAULT_BRUSH_TITLE, 
               TGuiColor.MainBrushColor[0],
               TGuiColor_cbGetCurMainW()); 
  }
}

//-------------------------------设置行坐标整体前景色函数-----------------------
void TGuiColor_SetPenColor(unsigned char y,
                        unsigned char x,
                        unsigned char xLen,
                        Color_t Color)
{
  Color_Full(Color, &TGuiColor.MainPenColor[y][x], xLen);  
}


//-------------------------------设置行坐标整体背景色函数-----------------------
void TGuiColor_SetBrushColor(unsigned char y,
                        unsigned char x,
                        unsigned char xLen,
                        Color_t Color)
{
  Color_Full(Color, &TGuiColor.MainBrushColor[y][x], xLen);  
}

//---------------------------设置行坐标对应前景色函数-------------------------
void TGuiColor_SetPenColorP(unsigned char y,
                        unsigned char x,
                        unsigned char xLen,
                        const Color_t *pColor)
{
  Color_Copy(&TGuiColor.MainPenColor[y][x], pColor, xLen); 
}

//---------------------------设置行坐标对应背景色函数-------------------------
void TGuiColor_SetBrushColorP(unsigned char y,
                        unsigned char x,
                        unsigned char xLen,
                        const Color_t *pColor)
{
  Color_Copy(&TGuiColor.MainBrushColor[y][x], pColor, xLen); 
}

/****************************************************************************
							             Lcd_Drv回调函数实现																 
****************************************************************************/

//-------------------------该行是否允许更新----------------------------------
//可用于测试模式，或某行在特定情况下当作其它用途使用
RowMask_t LCD_Drv_cbLineIsEn(unsigned short y)
{
  return TGuiColor.LcdLineEnMask & RowShift2Mask(y);
}

//-------------------------得到指定位置文字颜色信息流--------------------------
//每个Color_t表示对应位置起始字的颜色，可用于对显示字进行着色
const Color_t *LCD_Drv_pcbGetPenColor(unsigned char x,      //需要更新的行起始位置
                                      unsigned char y)      //需要更新的行
{
  return &TGuiColor.MainPenColor[y][x];
}

//--------------------------得到指定位置背景颜色信息流--------------------------
//每个Color_t表示对应位置起始字的背景颜色，可用于对显示字进行着色
const Color_t *LCD_Drv_pcbGetBrushColor(unsigned char x,      //需要更新的行起始位置
                                        unsigned char y)      //需要更新的行
{
  return &TGuiColor.MainBrushColor[y][x];
}

//-------------------------------得到焦点时的背景色-----------------------------
//焦点位置的背景色此以此为标准
Color_t LCD_Drv_cbGetForcusBrushColor(void)
{
  return TGUI_COLOR_DEFAULT_BRUSH_FOCUS;
}





