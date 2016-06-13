/************************************************************************
                 ＴＧＵＩ针对主界面的相关操作函数
*************************************************************************/

#ifndef T_MAIN_H
#define T_MAIN_H

#include "TGUI.h"

#include "TMenuCfg.h"	//	

//得到行缓冲区:
#define  Lcd_pGetRowBuffer(y)  \
  (TWin_pGetString(TWM_pGetWin(TMENU_FIX_WIN_COUNT),y))

//得到某位置缓冲区:
#define  Lcd_pGetPosBuffer(y,x)  (Lcd_pGetRowBuffer(y) + (x))


//清除一行
void Lcd_ClrRowBuffer(unsigned char y);

//设置某行更新
#define  Lcd_SetUpdateRow(y)  do{ }while(0)

//设置缓冲区显示
void Lcd_SetBuffer(unsigned char y,
              unsigned char x,
              const char *pString,
              unsigned char StringSize);
//置某些行下划线:不支持
#define  Lcd_SetDownLine(y) do{}while(0)

//置某此行反显与否:不支持
#define Lcd_SetNega(y) do{}while(0)


#endif
