/*******************************************************************************

                不带字库时，一行（长度*16）点阵缓冲－实现
此模块适用于不带字库的LCD显示屏
*******************************************************************************/

#include "TRowDotBuf.h"
#include "TGUIBase.h"
#include <string.h>

//点阵缓冲区,加半个缓冲防止全角半个字符内存溢出
static unsigned char _RowBuf[(TLCD_WIDTH + 1) * 2 * 8];
static unsigned char _ModuleBuf[32];//字模缓冲区


//-----------------------错误计数器处理程序--------------------------
#define _ErrCheck(pBuf, Count)  do{}while(0)
/*static unsigned char _ErrCount = 0;//错误计数器
//检查数据全是0xff时计数字库出错
static void _ErrCheck(const unsigned char *pBuf,//从外部读入的字模
                      unsigned char Count)//字模个数
{
  while(Count){
    if(*pBuf != 0xff){
      _ErrCount = 0;
      return;
    }
    *pBuf++;
    Count--;
  }
  //出错了！
  _ErrCount++;
  if(_ErrCount == 0xff) while(1);//字库出错了，等待看门狗复位
}*/

/****************************************************************************
                   取模方式1(横向取模,字节正序)支持
****************************************************************************/
#if T_ROW_DOT_BUF_MODULE == 0 //横向取模,字节正序

//-----------------------填充字模标志函数--------------------------
static void _FillModuleFlag(unsigned char NorFlag,  //普通更新标志时的其它标志
                             unsigned char ModuleLen)//取模长度,全角为2,半角为1
{
  unsigned char LenCount;
  unsigned char Len;

  if(NorFlag & DISP_CFG_ANTI){//反显时
    LenCount = ModuleLen * 16;
    for(Len = 0; Len < LenCount; Len++)
      _ModuleBuf[Len] = ~_ModuleBuf[Len];
  }
  if(NorFlag & DISP_CFG_UP_LINE){//上横线标志支持
    _ModuleBuf[0] = 0xff;
    if(ModuleLen == 2) _ModuleBuf[1] = 0xff;
  }
  if(NorFlag & DISP_CFG_DOWN_LINE){//下横线标志支持
    if(ModuleLen == 2){
      _ModuleBuf[30] = 0xff;
      _ModuleBuf[31] = 0xff;
    }
    else{
      _ModuleBuf[15] = 0xff;
    }
  }
}


//-----------------------将字模填入缓冲函数--------------------------
static void _FillBuf(unsigned char Pos,//指定位置
                      unsigned char w,  //更新长度
                      unsigned char ModuleLen)//取模长度,全角为2,半角为1
{
  unsigned char i;
  unsigned short CurPos;

  //此模式的LCD送数方式为:按行顺序送出,故需按该方式重新组织:
  for(i = 0; i < 16; i++){
    CurPos = w * i + Pos;
    _RowBuf[CurPos] = _ModuleBuf[i * ModuleLen];
    if(ModuleLen == 2)
      _RowBuf[CurPos + 1] = _ModuleBuf[i * ModuleLen + 1];
  }
}

/****************************************************************************
                   取模方式4(纵向取模,字节倒序)支持
****************************************************************************/
#elif T_ROW_DOT_BUF_MODULE == 3 //横向取模,字节正序

//-----------------------填充字模标志函数--------------------------
static void _FillModuleFlag(unsigned char NorFlag,  //普通更新标志时的其它标志
                             unsigned char ModuleLen)//取模长度,全角为2,半角为1
{
  unsigned char LenCount;
  unsigned char Len;

  LenCount = ModuleLen * 16;
  if(NorFlag & DISP_CFG_ANTI){//反显时
    for(Len = 0; Len < LenCount; Len++)
      _ModuleBuf[Len] = ~_ModuleBuf[Len];
  }
  if(NorFlag & DISP_CFG_UP_LINE){//上横线标志支持
    for(Len = 0; Len < LenCount; Len += 2)//偶行D0填充
      _ModuleBuf[Len] |= 0x01;
  }
  if(NorFlag & DISP_CFG_DOWN_LINE){//下横线标志支持
    for(Len = 1; Len < LenCount; Len += 2)//奇行D8填充
      _ModuleBuf[Len] |= 0x80;
  }
}

//-----------------------将字模填入缓冲函数--------------------------
static void _FillBuf(unsigned char Pos,//指定位置
                      unsigned char w,  //更新长度
                      unsigned char ModuleLen)//取模长度,全角为2,半角为1
{
  //此模式的LCD送数方式为:字依次送出,每字上左纵->上右纵->下左纵->下右纵
  memcpy(_RowBuf + (Pos << 4), _ModuleBuf, ModuleLen << 4);
}
/****************************************************************************
                   其它取模方式暂不支持
****************************************************************************/
#else
    //少函数实现,编译报错!!!!
#endif

/****************************************************************************
                   用户接口函数
****************************************************************************/

//-----------------------得到一块字符点阵缓冲--------------------------
//点阵具体格式：一个全角字16*16为一组成阵列
const unsigned char *TRowDotBuf_pGetBuf(unsigned char w,      //更新长度
                                        const char *pBuf,     //需更新字符
                                        //每个字符对应位置标志,见TWinMng_cbUpdateRow定义
                                        const unsigned char *pFlagBuf)
{
  unsigned char CurFlag;
  unsigned char Code;


  for(unsigned char Pos = 0; Pos < w; Pos++){
    CurFlag = *pFlagBuf++;
    Code = *pBuf++;
    //得到字模
    switch(CurFlag & DISP_CMD_MASK){
    case DISP_CMD_NOR://根据配置定义
      //先取出字模并将字模填入缓冲中
      if(Code >= 0x80){
        if((Pos + 1) >= w) break;//半个全角字符去除
        TRowDotBuf_cbGetWordModule(_ModuleBuf,(unsigned short)Code << 8 | *pBuf);
        _ErrCheck(_ModuleBuf, 32);
        pBuf++; pFlagBuf++; //下个字符
        _FillModuleFlag(CurFlag,2);//填标志
        _FillBuf(Pos,w,2);//填充行缓冲
        Pos++;//
      }
      else{
        TRowDotBuf_cbGetHalfWordModule(_ModuleBuf,Code);
        _ErrCheck(_ModuleBuf, 16);
        _FillModuleFlag(CurFlag,1);//先填标志
        _FillBuf(Pos,w,1);//填充行缓冲
      }
      break;
    default://其它或清屏标志,清除该位置所有内容
      memset(_ModuleBuf,0,16);
      _FillModuleFlag(CurFlag,1);//先填标志
      _FillBuf(Pos,w,1);//填充行缓冲
      break;
    }//end switch
  }//end for
  return _RowBuf;
}

//---------------------------得到显示缓冲区--------------------------
unsigned char *TRowDotBuf_pGetDispBuf(void)
{
  return _RowBuf;
}


