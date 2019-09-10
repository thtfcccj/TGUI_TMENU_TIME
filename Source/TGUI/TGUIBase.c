/*******************************************************************************

                   TGUI基本构件-相关共享函数实现

*******************************************************************************/
#include "TGUICfg.h"
#include "TGUIBase.h"
#include <string.h>

//-----------------------由项总数得到占位函数-----------------------
unsigned char TGetItemsPlace(TItemSize_t Items)
{
  if(Items <= 9) return 1;
  if(Items <= 99) return 2;
  //短整型时
  #if TITEM_SIZE_T >= 2
    if(Items <= 999) return 3;
    if(Items <= 9999) return 4;
    else return 5;
 #else
   else return 3;
  #endif
}

//-----------------------得到两字符右对齐后的长度函数-----------------------
unsigned char TGetAlignLenR(unsigned char Len,     //原字符长度,<128
                          unsigned char Append) //附加字符长度,<128
{
  Len += Append;
  #ifdef  TGUI_WORD_ALIGN
    if(Len & 0x01) return Len + 1;
  #endif
  return Len;
}

//-----------------------得到两字符左对齐后的长度函数-----------------------
unsigned char TGetAlignLenL(unsigned char Len,     //原字符长度,<128,> 1
                          unsigned char Append) //附加字符长度,<128
{
  Len += Append;
  #ifdef  TGUI_WORD_ALIGN
    if(Len & 0x01) return Len - 1;
  #endif
  return Len;
}

//-----------------------转换为字符串-显示为最简函数----------------------
unsigned char TValue2StringMin(TSigned_t Value,
                            char *pString,//接收缓冲
                            unsigned char Min)//保证的最小位数
{
  char *pOrgString = pString;
  
  //长整型时
  #if(TSIGNED_SIZE == 4)
    if((Value >= 1000000000) || (Min >= 10)){
      *pString++ = Value / 1000000000 + '0';
      Value %= 1000000000;
	  Min = 10;
    }
    if((Value >= 100000000) || (Min >= 9)){
      *pString++ = Value / 100000000 + '0';
      Value %= 100000000;
	  Min = 10;
    }
    if((Value >= 10000000) || (Min >= 8)){
      *pString++ = Value / 10000000 + '0';
      Value %= 10000000;
	  Min = 10;
    }
    if((Value >= 1000000) || (Min >= 7)){
      *pString++ = Value / 1000000 + '0';
      Value %= 1000000;
	  Min = 10;
    } 
    if((Value >= 100000) || (Min >= 6)){
      *pString++ = Value / 100000 + '0';
      Value %= 100000;
	  Min = 10;
    }
  #endif

  //短整型时
  #if(TSIGNED_SIZE >= 2)
    if((Value >= 10000) || (Min >= 5)){
      *pString++ = Value / 10000 + '0';
      Value %= 10000;
	  Min = 10;
    }
    if((Value >= 1000) || (Min >= 4)){
      *pString++ = Value / 1000 + '0';
      Value %= 1000;
	  Min = 10;
    }
  #endif

  if((Value >= 100) || (Min >= 3)){
    *pString++ = Value / 100 + '0';
    Value %= 100;
	Min = 10;
  }
  if((Value >= 10) || (Min >= 2)){
    *pString++ = Value / 10 + '0';
    Value %= 10;
	Min = 10;
  }
  if((Min >= 1) || (Value > 0)){
    *pString++ = Value + '0';
  }
  return pString - pOrgString;
}

//-----------------------得到两字符对齐后的长度函数-----------------------
//当需要双字节对齐时,未对齐时在该位置填充' '
//返回填充后的缓冲
char *pTAlignString(TItemSize_t ItemLen,//字符长度
                   char *pString)//接收缓冲     
{
  #ifdef  TGUI_WORD_ALIGN
    if(ItemLen & 0x01) *pString++ = ' ';
  #endif
  return pString;
}

//------------------字符串扩展拷贝函数-----------------------
//1:响应结束与换行字符,
//2:返回结束的位置
char *Tstrcpy_ex(char *s1, const char *s2)
{
  char s = *s2++;
  while((s != '\0') && (s != '\n')){
    *s1++ = s;
    s = *s2++;
  }
  //*s1 = '\0';//自动填充结束字符
  return s1;
}

