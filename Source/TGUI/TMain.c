/***************************************************************************

                 TGUI针对主界面的相关操作函数实现

****************************************************************************/

#include "TMain.h"
#include <string.h>

//---------------------------------清除一行---------------------------------
void Lcd_ClrRowBuffer(unsigned char y)
{
  char *pBuf = Lcd_pGetRowBuffer(y);
  memset(pBuf,' ',TLCD_WIDTH);
}

//---------------------------------设置缓冲区显示---------------------------------
void Lcd_SetBuffer(unsigned char y,
              unsigned char x,
              const char *pString,
              unsigned char StringSize)
{
  char *pBuf = Lcd_pGetRowBuffer(y);
  //memset(pBuf,' ',TLCD_WIDTH);
  memcpy(pBuf + x,pString,StringSize);
}
