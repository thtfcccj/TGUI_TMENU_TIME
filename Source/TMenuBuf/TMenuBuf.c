/*******************************************************************************

                           菜单编辑临时缓冲区实现

*******************************************************************************/

#include "TMenuBuf.h"
#include <string.h>

struct _TMenuBuf TMenuBuf;

//-----------------------------头部插入查找表函数--------------------------------
//将原有查找表插入已存在的TMenuBuf.Lut中，并返回TMenuBuf.Lut
const unsigned char *TMenuBuf_pLutInsertH(unsigned char NewCount,//.Lut已有大小
                                           const unsigned char *pLut)//原表
{
  if(NewCount == 0) return pLut;//没有插入
  memcpy(&TMenuBuf.Lut[1 + NewCount], pLut + 1, *pLut);
  TMenuBuf.Lut[0] = NewCount + *pLut;//总数更新
  return TMenuBuf.Lut;
}