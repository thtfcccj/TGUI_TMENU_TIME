/*****************************************************************************
                         TGUI上的输入法模块-符号输入法部分
*****************************************************************************/

#include "TImeCapital.h"
#include "TImeBase.h"
#include <string.h>

/*****************************************************************************
                                相关函数实现
*****************************************************************************/

//---------------------------初始化函数---------------------------------
//切换到符号输入法时调用
void TImeCapital_Init(struct _TImeCapital *pCapital,//输入法结构
                      unsigned char CapitalFlag)   //大写输入模式标志
{
  memset(pCapital, 0, sizeof(struct _TImeCapital));
  if(CapitalFlag) pCapital->Flag |= TIME_CAPITAL_CAPITAL; //大写状态
}

//-------------------------填充第四行用户提示-----------------------------------
static const char _chSelGroup[] = {"选择分组"};
static const char _chSelChar[] =  {"选择字母"};
static const char _chNotice[] =  {"快速↓两次)"};

void TImeCapital_FullTitle(const struct _TImeCapital *pCapital,//输入法结构
                           char *pStr)
{
  //填充:"A: "或"a: "
  if(pCapital->Flag & TIME_CAPITAL_CAPITAL) *pStr = 'A';
  else *pStr = 'a';
  pStr++;
  *pStr++ = ':';
  *pStr++ = ' ';
  //选择字母
  if(pCapital->Flag & TIME_CAPITAL_SEL_CHAR){
    strcpy(pStr, _chSelChar);
    if(pCapital->FirstKey == TIME_MNG_KEY_DOWN){//最后一屏时：增加提示z:快速↓两次
      pStr += strlen(_chSelChar);
      *pStr++ = ' ';
      *pStr++ = '(';
      if(pCapital->Flag & TIME_CAPITAL_CAPITAL) *pStr = 'Z';
      else *pStr = 'z';
      pStr++;
      strcpy(pStr, _chNotice);  
    }
  }
  else//选择分组
    strcpy(pStr, _chSelGroup);
}

//-------------------------分组时各GuideKey对应字符---------------------------
static const char _chCapitalUp[] =    {"ABCDE"};
static const char _chCapitalLeft[] =  {"FGHIJ"};
static const char _chCapitalSave[] =  {"KLMNO"};
static const char _chCapitalRight[] = {"PQRST"};
static const char _chCapitalDown[] =  {"UVWXYZ"};
static const char _chLowercaseUp[] =    {"abcde"};
static const char _chLowercaseLeft[] =  {"fghij"};
static const char _chLowercaseSave[] =  {"klmno"};
static const char _chLowercaseRight[] = {"pqrst"};
static const char _chLowercaseDown[] =  {"uvwxyz"};
static const char *const _CapitalArrow[] = {//大写
  _chCapitalUp, _chCapitalLeft,
  _chCapitalSave, _chCapitalRight,
  _chCapitalDown
};
static const char *const _LowercaseArrow[] = {//小写
  _chLowercaseUp, _chLowercaseLeft,
  _chLowercaseSave, _chLowercaseRight,
  _chLowercaseDown
};

//-------------------------得到对应键值字符函数-----------------------------------
//如：上键对应 返回NULL表示不需要字符
const char *TImeCapital_pGetArrowNotice(struct _TImeCapital *pCapital,//输入法结构
                                         unsigned char GuideKey)//对应键值位置
{
  //单独处理退出键
  if(GuideKey == TIME_MNG_KEY_EXIT) return chReturn;
  
  //选择字母时,返回缓冲区
  if(pCapital->Flag & TIME_CAPITAL_SEL_CHAR){
    char Char = 'A' + (pCapital->FirstKey * 5 + GuideKey);
    char *pStr = pCapital->CharBuf;
    if(!(pCapital->Flag & TIME_CAPITAL_CAPITAL))//小写时
      *pStr = Char + 32;
    else *pStr = Char;
    pStr++;
    //Y时需增加Z
    if(Char == 'Y'){
      if(!(pCapital->Flag & TIME_CAPITAL_CAPITAL))//小写时
        *pStr = 'z';
      else *pStr = 'Z';
      pStr++;
    }
    *pStr = '\0';//字符串结束字符
    return pCapital->CharBuf;
  }
  else{//选择分组时
    if(!(pCapital->Flag & TIME_CAPITAL_CAPITAL))//小写时
      return _LowercaseArrow[GuideKey];
    return _CapitalArrow[GuideKey];
  }
}

//--------------------------按键响应函数-----------------------------------
//若修改完成，则返回对应字符，否则0: 不修改返回,  0xffff: 内部操作过程中 
unsigned short TImeCapital_Key(struct _TImeCapital *pCapital,
                                 unsigned char GuideKey)//导航键值0-5
{
  //单独处理退出键
  if(GuideKey == TIME_MNG_KEY_EXIT){
    //选择字母时按键
    if(pCapital->Flag & TIME_CAPITAL_SEL_CHAR){
      pCapital->Flag &= ~TIME_CAPITAL_SEL_CHAR; //退至分组选择
      return 0xffff;
    }
    return 0; //选择分组时， 不修改返回
  }
  
  //选择字母时按键
  if(pCapital->Flag & TIME_CAPITAL_SEL_CHAR){
    if(pCapital->yzTimer){//yz选择模式时
      if(GuideKey == TIME_MNG_KEY_DOWN){//第二次为z
        if(pCapital->Flag & TIME_CAPITAL_CAPITAL)
          return (unsigned short)'Z';
        return (unsigned short)'z';
      }
      else{//其它键默认为y
        if(pCapital->Flag & TIME_CAPITAL_CAPITAL)
          return (unsigned short)'Y';
        return (unsigned short)'y';        
      }
    }
    //第一次按键时
    char Char = 'A' + (pCapital->FirstKey * 5 + GuideKey);
    if(Char == 'Y'){//需要第二次选择
      pCapital->yzTimer = TIME_CAPITAL_YZ_TIMER_OV;
      return 0xffff;
    }
    if(!(pCapital->Flag & TIME_CAPITAL_CAPITAL))//小写时
      Char += 32;
    return (unsigned short)Char; //返回结果
  }
  else{//分组选择时记住键值并切换至字符选择模式
    pCapital->FirstKey = GuideKey;
    pCapital->Flag |= TIME_CAPITAL_SEL_CHAR;
    return 0xffff;
  }
}

//-----------------------------任务函数----------------------------------
//128ms调用一次, 若修改完成，返回对应输入完成的字符，否则0不处理
unsigned short TImeCapital_Task(struct _TImeCapital *pCapital)
{
  //yz选择模式时自动确认Y或y
  if(!pCapital->yzTimer) return 0;
  pCapital->yzTimer--;
  if(!pCapital->yzTimer){//时间到自动确认为Y
    if(pCapital->Flag & TIME_CAPITAL_CAPITAL)
      return (unsigned short)'Y';
    return (unsigned short)'y';
  }
  return 0;
}


