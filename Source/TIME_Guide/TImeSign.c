/*****************************************************************************
                         TGUI上的输入法模块-符号输入法部分
*****************************************************************************/

#include "TImeSign.h"
#include "TImeBase.h"
#include <string.h>


//默认的全角或半角符号表(半角后加一空格)
static const char _SignTbl[] = {
"\
   . , ? ! : * | ~ ^ ` < > - ( ) @ / _ ; +\
 & % # = [ ] { } $ &。，、；：？！“”…\
《》【】〖〗○●◇◆□■★☆≤≥￥℃‰\
→←↑↓"
};

//定义一页能显示下的最大字符个数(半角为单位)
#define _SIGNAL_PAGE_MAX   (2 * 5 * 5)
//有下一页时, 一页总数
#define _MUTI_PAGE_MAX   (2 * 5 * 4)

/*****************************************************************************
                        相关函数
*****************************************************************************/

//---------------------------初始化函数---------------------------------
//切换到符号输入法时调用
void TImeSign_Init(struct _TImeSign *pSign,
                   const char *pSignTbl) //挂接的符号输入表,NULL使用默认
{
  memset(pSign, 0, sizeof(struct _TImeSign));//先初始化到0
  if(pSignTbl == NULL) //NULL使用默认
    pSignTbl = _SignTbl; 
  pSign->pSignTbl = pSignTbl;
  pSign->Len = strlen(pSignTbl);
}

//-------------------------填充第四行用户提示-----------------------------------
static const char _chSelGroup[] = {"符号: 选择分组"};
static const char _chSelSign[] = {"符号: 选择符号"};
void TImeSign_FullTitle(const struct _TImeSign *pSign,//输入法结构
                           char *pStr)
{
   if(pSign->Flag & TIME_SIGN_SEL_SIGN)
     strcpy(pStr, _chSelSign);
   else
     strcpy(pStr, _chSelGroup);
}

//----------------------------得到当前字符---------------------------------------
//返回NULL超过长度了
const char *_pGetCurStr(struct _TImeSign *pSign,//输入法结构
                        unsigned char GuideKey)//对应键值位
{
  unsigned char StrPos = pSign->GrpStart;
  if(pSign->Len > _SIGNAL_PAGE_MAX)//一页不能显示下时,有下一页
      StrPos += (pSign->FirstKey * 4 + TIme_GuideKeyToNoEnterKey[GuideKey]) * 2;
  else StrPos += (pSign->FirstKey * 5 + GuideKey) * 2;//一页能显示下了
  
  if((StrPos + 2) > pSign->Len) return NULL;//超过长度了
  //copy到缓冲区返回
  const char *pStr = pSign->pSignTbl + StrPos;
  return pStr;
}

//-------------------------得到对应键值字符函数---------------------------------
//如：上键对应 返回NULL表示不需要字符
static const char _chNextGroup[] = {"下一组"};
const char *TImeSign_pGetArrowNotice(struct _TImeSign *pSign,//输入法结构
                                     unsigned char GuideKey)//对应键值位置
{
  //单独处理退出键
  if(GuideKey == TIME_MNG_KEY_EXIT) return chReturn;
    
  //========================单个符号时=========================
  if(pSign->Flag & TIME_SIGN_SEL_SIGN){
    const char *pStr = _pGetCurStr(pSign, GuideKey);
    if(pStr == NULL) return NULL;//超过长度了
    //copy到缓冲区返回
    pSign->SignBuf[0] = *pStr++;
    pSign->SignBuf[1] = *pStr;
    pSign->SignBuf[2] = ' ';//后部填充空格以美观
    pSign->SignBuf[3] = '\0';
    return pSign->SignBuf;
  }
  
  //============================选择分组时======================
  unsigned char GroupCount; //一页内显示字符组数,
  if(pSign->Len > _SIGNAL_PAGE_MAX){//一页不能显示下时,有下一页
    //选确认键时，确认键表示下一组
    if(GuideKey == TIME_MNG_KEY_ENTER) 
      return _chNextGroup;
    GuideKey = TIme_GuideKeyToNoEnterKey[GuideKey]; //转换为4组
    GroupCount = 4;
  }
  else GroupCount = 5; //一页显示下时，确认键为正常选择
  
  unsigned char StrPos = pSign->GrpStart + GuideKey * (2 * GroupCount);
  if((StrPos + 2) > pSign->Len) return NULL;//超过长度了(1个也算一组)
  unsigned char Len = pSign->Len - StrPos;
  if(Len > 10) Len = 10;
  memcpy(pSign->SignBuf, pSign->pSignTbl + StrPos, Len);
  pSign->SignBuf[Len] = '\0';
  return pSign->SignBuf;
}

//--------------------------按键响应函数-----------------------------------
//若修改完成，则返回对应字符，否则0: 不修改返回,  0xffff: 内部操作过程中 
unsigned short TImeSign_Key(struct _TImeSign *pSign,
                              unsigned char GuideKey)//导航键值0-5
{
  //单独处理退出键
  if(GuideKey == TIME_MNG_KEY_EXIT){
    //选择符号时按键
    if(pSign->Flag & TIME_SIGN_SEL_SIGN){
      pSign->Flag &= ~TIME_SIGN_SEL_SIGN; //退至分组选择
      return 0xffff;
    }
    return 0; //选择分组时， 不修改返回
  }
  
  //============================选择符号时============================
  if(pSign->Flag & TIME_SIGN_SEL_SIGN){
    const char *pStr = _pGetCurStr(pSign, GuideKey);
    if(pStr == NULL) return 0xffff;//此位无字符,按键无效
    unsigned short Str = *pStr;
    if(Str == ' ') Str = 0; //空格时只有后半部分，为半角
    else Str <<= 8; //全角是高位在前
    return Str | *(pStr + 1); //低位在后
  }
  
  //============================选择分组时============================
  if(pSign->Len > _SIGNAL_PAGE_MAX){//一页不能显示下时,有下一页
    //选确认键时，确认键切换到下一组(末尾回环)
    if(GuideKey == TIME_MNG_KEY_ENTER){
      pSign->GrpStart += _MUTI_PAGE_MAX;
      if(pSign->GrpStart > pSign->Len)//回环到第一页了
        pSign->GrpStart = 0;
      return 0xffff;//内部处理了
    }
    GuideKey = TIme_GuideKeyToNoEnterKey[GuideKey]; //转换为4组
  }
  //else GuideKey不变
  
  //记住组内位置并切换到符号选择模式
  pSign->FirstKey = GuideKey;
  pSign->Flag |= TIME_SIGN_SEL_SIGN; //退至分组选择
  return 0xffff;//内部处理了
}


