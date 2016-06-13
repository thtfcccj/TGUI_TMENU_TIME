/* ----------------------------------------------------------------------------
 *                用户自定义字模－相关实现
 * --------------------------------------------------------------------------*/

#include "TWord.h"
#include "string.h"


//-------------得到将GB2312标准区位码转换为自定义索引值函数------------------
//此函数需配合用户自定义字模相关操作函数才能得到字模
//返回0xffff表示未找到
unsigned short TWord_Word2UserID(unsigned short Word,//前半字与后半字必须>=128
                                 const char *pLUT,         //该字对应的自定义字符
                                 unsigned short Count)//自定义字符大小
{
  unsigned short Left  = 0;    //左边界
  unsigned short Center = Count >> 1;//相对中间位置
  unsigned short CurPos;       //当前位置
  unsigned short Condition;       //当前条件值
  
  do{
    Center >>= 1;      //复用作查找位置
    CurPos = Center + Left;
    Condition = *((unsigned short*)pLUT + CurPos);//得到条件结果
    //检查是否在范围
    if(Word > Condition)Left = CurPos;  //在后面
    else if(Word == Condition) return CurPos;//找到了
    //else 在前面，继续
  }while(Center);
  
  //没找到时,也许在其左位置呢!
  if((Condition > Word) && (Word == *((unsigned short*)pLUT + CurPos - 1)))
    return CurPos;
  //还没找到时,也许在右位置呢!
  if((Condition < Word) && (Word == *((unsigned short*)pLUT + CurPos + 1)))
    return CurPos;
  //找不到，没撤了！！！！！
  return 0xffff;
}

//--------------------得到16X16文本一行缓冲字模函数--------------------------
//可直接通过此函数获得一行字的GB2312图形点阵
void TWord_GetLineBuf_GB2312(char *pString,       //行字符
                             unsigned char w,     //行宽
                             unsigned char *pLine)//需填充的行缓冲区,>=w * 32
{
  char Code;
  unsigned short Word;
  unsigned short WordCount = TWord_GetGB2312LUTSize();//自定义字符大小
  
  for(unsigned char i = 0; i < w; i++){
    Code = *pString++;
    if(Code < 0x80){//ASCII码
      //不支持控制字符时，显示空格
      #ifndef TWORD_ASC_EN_CTRL 
      if(Code < 32) Code = 32;
      #endif
      memcpy(pLine,TWord_pGetAsciiModule(Code),16);
      pLine += 16;
    }
    else{//全角字符
      Word = (unsigned short)Code << 8 + *pString++;
      Word = TWord_Word2UserID(Word,TWord_GB2312LUT,WordCount);
      if(Word < 0xffff){ //找到了
        memcpy(pLine,TWord_pGetWordModule(Word),32);
        pLine += 16;
      }
      else{//未找到时，解为两个空格
        memcpy(pLine,TWord_pGetAsciiModule(32),16);
        pLine += 16;
        memcpy(pLine,TWord_pGetAsciiModule(32),16);
        pLine += 16;
      }
    }
  }
}

