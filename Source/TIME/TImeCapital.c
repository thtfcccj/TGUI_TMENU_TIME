/*****************************************************************************
                         TGUI上的输入法模块-符号输入法部分
*****************************************************************************/

#include "TImeCapital.h"
#include <string.h>

//===================大写字母转义表========================
//含数字键0-9
static const char _CapitalLUT[10][5] = {
  {' ','+','-','_','0'},
  {'.',',','?','!','1'}, {'A','B','C','@','2'},
  {'D','E','F','#','3'}, {'G','H','I','@','4'},
  {'J','K','L','%','5'}, {'M','N','O','^','6'},
  {'P','Q','R','S','7'}, {'T','U','V','*','8'},
  {'W','X','Y','Z','9'}
};

/*****************************************************************************
                        相关函数
*****************************************************************************/

//---------------------------初始化函数---------------------------------
//切换到符号输入法时调用
void TImeCapital_Init(struct _TImeCapital *pCapital,//输入法结构
                      unsigned char CapitalFlag)   //大写输入模式标志
{
  memset(pCapital, 0, sizeof(struct _TImeCapital));
  
  pCapital->CapitalFlag = CapitalFlag;
}

//----------------------得到当前用户选择字符函数---------------------------
//返回当前用户选择的字符
char TImeCapital_GetChar(struct _TImeCapital *pCapital)
{
  char Char = _CapitalLUT[pCapital->Num][pCapital->NumCount];
  if(!pCapital->CapitalFlag){//小写字母模式时需转换大写为小写
    if((Char >= 'A') && (Char <= 'Z'))
      Char += 32; //对应差32个值
  }
  return Char;
}

//-----------------------数字键响应函数---------------------------------
//当用户按数字键0-9时响应用户清求
//返回是否已切换到了其它按键上面
signed char TImeCapital_KeyNum(struct _TImeCapital *pCapital,
                               char KeyNum)//键值，应确保为'0'-'9'
{
  unsigned char Num = KeyNum -= '0';
  
  //有过按键且两次输入相同时，切到下个字符
  if((pCapital->SameKeyIndex) && (pCapital->Num == Num)){
    if(pCapital->SameKeyIndex < 255) pCapital->SameKeyIndex++;
    pCapital->NumCount++;
    if(pCapital->NumCount >= 5)//超限了
      pCapital->NumCount = 0;
    return 0;  //还在当前按键上面
  }
  else{//上次无按键或切换到其它按键时
    pCapital->SameKeyIndex = 1;
    pCapital->Num = Num;
    pCapital->NumCount = 0;
  }
  return -1;  //切换按键了
}


