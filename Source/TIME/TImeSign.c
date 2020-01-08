/*****************************************************************************
                         TGUI上的输入法模块-符号输入法部分
*****************************************************************************/

#include "TImeSign.h"
#include <string.h>


//默认的全角或半角符号表(半角后加一空格)
static const char _SignTbl[] = {
"\
   . , ? ! : * | ~ ^ ` < > - ( ) @ / _ ; +\
 & % # = [ ] { } $ &。，、；：？！“”…\
《》【】〖〗○●◇◆□■★☆≤≥￥℃‰\
→←↑↓"
};

/*****************************************************************************
                        相关函数
*****************************************************************************/

//---------------------------初始化函数---------------------------------
//切换到符号输入法时调用
void TImeSign_Init(struct _TImeSign *pSign,//输入法结构
                   //挂接的符号输入表,全字对齐，为NULL时用默认符号表
                   const char *pSignTbl,
                   unsigned char DispW,        //显示宽度,>16
                   unsigned char DispH)        //显示高度>=3
{
  memset(pSign, 0, sizeof(struct _TImeSign));//先初始化到0
  if(pSignTbl == NULL){//使用默认符号表
    pSign->pSignTbl = _SignTbl;
    pSign->Len = sizeof(_SignTbl);
  }
  else{
    pSign->pSignTbl = pSignTbl;
    pSign->Len = strlen(pSignTbl);
  }
  //过高，及宽度够时，前面自动增加行号
  if((DispH > 3) && (DispW > 16)){
    pSign->EnDispLine = 1;
    DispW -= 4; //占用显示长度了
  }
  DispW >>= 2; //一行全角字符;
  if(DispW > 9) DispW = 9; //一页最大允许显示9个以对应数字键
  pSign->w = DispW;
  if(DispH > 9) DispH = 9; //一页最大允许显示9个以对应数字键  
  pSign->h = DispH;      //与显示高度相同
}


//----------------------填充行显示字符串函数---------------------------
//返回填充数量
unsigned char TImeSign_GetDispChar(struct _TImeSign *pSign,
                                    char *pBuf,              //被填充的字符
                                    unsigned char VNum)     //列号0-8
{
  if(VNum > 8) VNum = 8;//异常
  const char *pPos, *pEndPos;
  unsigned short Pos;
  Pos = VNum * pSign->w * 2;//转换为半角
  Pos += pSign->PageStart;
  if(Pos >= pSign->Len) return 0; //超了
  
  pPos = pSign->pSignTbl + Pos; 
  pEndPos = pSign->pSignTbl + pSign->Len;
  
  char *pStartBuf = pBuf;
  if(pSign->EnDispLine){//增加行号
    *pBuf++ = '[';
    *pBuf++ = '1' + VNum;
    *pBuf++ = ']';
    *pBuf++ = ' ';
  } 
  //填充本行字符
  for(unsigned char Num = 0; Num < pSign->w; Num++){
    if(pPos < pEndPos){//字符未结束时
      *pBuf++ = *pPos++;    //第一个字符空格填充或前半角
      *pBuf++ = *pPos++;    //第一个字符或后半角
      *pBuf++ = Num + '1';  //指示行位置
      *pBuf++ = ' ';        //两字符间填充空格
    }
    else break;
  }//end for
  return pBuf - pStartBuf;
}

//----------------------判断一页是否能显示下函数---------------------------
signed char TImeSign_IsOnePage(const struct _TImeSign *pSign)
{
  unsigned short Pages = pSign->w * pSign->h * 2;//每页字符个数*2
  if(pSign->Len <= Pages) return 1;//无下一页面
  return 0;
}
         
//----------------------得到当前用户选择字符函数---------------------------
//返回当前用户选择的字符,前两位有效
//读取字符后强制返回空闲状态
//注：这里没有限制上层必须在eTImeSign_Final才能获得字符串
const char *TImeSign_pGetChar(struct _TImeSign *pSign)
{
  unsigned short Pos;
  
  Pos = pSign->PosV * pSign->w + pSign->PosH;
  Pos <<= 1; //转换为半角
  Pos += pSign->PageStart;//起始页
  pSign->eState = eTImeSign_Idie;
  if(Pos >= pSign->Len) //超限时选择最后一个
    return pSign->pSignTbl + pSign->Len - 1;
  
  return pSign->pSignTbl + Pos;
}

//-------------------------清除键响应函数---------------------------------
//这里仅用于在已选择某列时取消列选择
//用户按清除或退出键时调用，返回是否已响应了清除键
signed char TImeSign_KeyClr(struct _TImeSign *pSign)
{
  if(pSign->eState != eTImeSign_SelV) return -1;//没响应按键
  pSign->eState = eTImeSign_Idie;
  return 0;  //已响应
}

//-----------------------数字键响应函数---------------------------------
//当用户按数字键1-9时响应用户清求
//返回1:响应按键并退出; 0:响应按键未退出
//负值，响应按键值无效
signed char TImeSign_KeyNum(struct _TImeSign *pSign,
                            char KeyNum)//键值，应确保为'1'-'9'
{
  enum _eTImeSign eState = pSign->eState;
  
  KeyNum -= '1';
  switch(eState){
  case eTImeSign_Idie://空闲，需进行列选
    if(KeyNum >= pSign->h) return -2;//超过当前列范围选择了
    pSign->PosV = KeyNum;
    eState = eTImeSign_SelV;  //用户选择完成了
    break; 
  case eTImeSign_SelV://已进行了列选，行选选择具体某个字
    if(KeyNum >= pSign->w) return -3;//超过当前列范围选择了
    pSign->PosH = KeyNum;
    eState = eTImeSign_Final;  //用户选择完成了
    return 1; //响应按键并退出
  //case eTImeSign_Final: 
  //  return -1//用户选择完成,再次按键无效，上层处理错误!
  default:return -1;
  }
  pSign->eState = eState;
  return 0;  //响应按键未退出
}

//-----------------------翻页键响应函数---------------------------------
//响应上下翻页清求
//返回是否已响应了用户按键
signed char TImeSign_KeyPage(struct _TImeSign *pSign,
                             signed char DownFlag)//下键标志，否则为上键
{
  unsigned short Pages = pSign->w * pSign->h * 2;//每页字符个数*2
  pSign->eState = eTImeSign_Idie; //任何时候按翻页键均需重新开始选择
  
  if(DownFlag){//下一页
    if(pSign->Len <= Pages) return -1;//无下一页面
    if(pSign->PageStart < (pSign->Len - Pages)){//只要没到最后一页，均可下翻
        pSign->PageStart += Pages;
    } 
  }
  else{//上一页
    if(pSign->PageStart){//只要没到第一页，均可上翻
      if(pSign->PageStart > Pages)//还有上一页时
        pSign->PageStart -= Pages;
      else
        pSign->PageStart = 0;//已在第一页了
    } 
  }
  return -1;
}

