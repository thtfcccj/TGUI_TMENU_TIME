/***************************************************************************

                 TGUI������������ز�������ʵ��

****************************************************************************/

#include "TMain.h"
#include <string.h>

//---------------------------------���һ��---------------------------------
void Lcd_ClrRowBuffer(unsigned char y)
{
  char *pBuf = Lcd_pGetRowBuffer(y);
  memset(pBuf,' ',TLCD_WIDTH);
}

//---------------------------------���û�������ʾ---------------------------------
void Lcd_SetBuffer(unsigned char y,
              unsigned char x,
              const char *pString,
              unsigned char StringSize)
{
  char *pBuf = Lcd_pGetRowBuffer(y);
  //memset(pBuf,' ',TLCD_WIDTH);
  memcpy(pBuf + x,pString,StringSize);
}
