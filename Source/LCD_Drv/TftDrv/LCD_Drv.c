/*******************************************************************************

 			            LCD硬件及TGUI显示接口之t6963驱动实现
 
*******************************************************************************/
#include    "LCD_Drv.h"
#include    "PlotPB.h" //绘制字符串
#include    <string.h>

/****************************************************************************
							       TGUI显示屏回调函数实现	
****************************************************************************/
//注:此模块暂只支持	 DISP_CFG_ANTI 功能
#include "TGUIBase.h"

//-------------------------显示屏更新一行内容函数--------------------
void TWinMng_cbUpdateRow(unsigned char Lcd,    //当前操作那个显示屏
                         unsigned char x,      //需要更新的行起始位置
                         unsigned char y,      //需要更新的行
                         unsigned char w,      //更新长度
                         const char *pBuf,    //需更新字符
                         //每个字符对应位置标志,见定义
                         const unsigned char *pFlagBuf)
{
  if(!LCD_Drv_cbLineIsEn(y)) return; //行不允许更新时
  
  const Color_t *pPen = LCD_Drv_pcbGetPenColor(x, y);         //取出文字颜色
  const Color_t *pBrush = LCD_Drv_pcbGetBrushColor(x, y);     //取出背景颜色
  Color_t ForcusBrush = LCD_Drv_cbGetForcusBrushColor();       //取出焦点背景颜色
  unsigned char xEnd = x + w;
  unsigned short yPixel = (unsigned short )y << 4;
  for(; x < xEnd; x++){
    char c = *pBuf;
    //当前背景色
    Color_t Brush;
    if(*pFlagBuf & DISP_CFG_ANTI) Brush = ForcusBrush;//被选择了
    else Brush = *pBrush;
    //ASCII直接转换
    if(c < 0x80){
      PlotPB_Asc((unsigned short )x << 3, yPixel, c, *pPen, Brush);
    }
    //未检查GB2312正确性！！！！
    else{
      if((x + 1) >= xEnd) break;//半个汉字结束了，异常 
      pBuf++; //取下半个字
      PlotPB_GB2312((unsigned short )x << 3, yPixel,
                    ((unsigned char)c << 8) | *pBuf, *pPen, Brush); //高位在前
      //后半增加
      x++;
      pFlagBuf++;
      pPen++;
      pBrush++;      
    }
    //除x外，各变量这里统一增加
    pBuf++;
    pFlagBuf++;
    pPen++;
    pBrush++;
  }
}





