/*****************************************************************************

                      TGUI上的输入法模块-符号输入法部分

*****************************************************************************/
#ifndef __TIME_SIGN_H
#define __TIME_SIGN_H

/*****************************************************************************
                              说明
*****************************************************************************/
//一页内(25个字符)能显示下时，一页完成，否则确认键表示为下一页





/*****************************************************************************
                        相关结构
*****************************************************************************/
#include "TImeBase.h" //TIme_String_t;

struct _TImeSign{
  const char *pSignTbl;  //挂接的符号输入表,全字对齐，为半字时需前面加空格补齐
  TIme_String_t Len;     //挂接的符号输入表长度,有于加速判断
  TIme_String_t GrpStart;   //多页时，组为单位的字符串起始位置
  unsigned char FirstKey;  //第一次输入的键值,0-4
  unsigned char Flag;  //相关标志，见定义
  char SignBuf[2 + 2];      //用于返回字符的临时缓冲区
};

#define TIME_SIGN_SEL_SIGN  0x40    //选择符号状态，否则为选择分组

/*****************************************************************************
                        相关函数
*****************************************************************************/


//---------------------------初始化函数---------------------------------
//切换到符号输入法时调用
void TImeSign_Init(struct _TImeSign *pSign,
                   const char *pSignTbl); //挂接的符号输入表,NULL使用默认
                     

//-------------------------填充用户提示(抬头)-----------------------------------
//如：选择分组：
void TImeSign_FullTitle(const struct _TImeSign *pSign,//输入法结构
                         char *pStr);

//-------------------------得到对应键值字符函数---------------------------------
//如：上键对应 返回NULL表示不需要字符
const char *TImeSign_pGetArrowNotice(struct _TImeSign *pSign,//输入法结构
                                         unsigned char GuideKey);//对应键值位置

//--------------------------按键响应函数-----------------------------------
//若修改完成，则返回对应字符，否则0: 不修改返回,  0xffff: 内部操作过程中 
unsigned short TImeSign_Key(struct _TImeSign *pSign,
                            unsigned char GuideKey);//导航键值0-5

//-----------------------翻页键响应函数---------------------------------
//响应翻页清求
//返回是否已响应了用户按键
//signed char TImeSign_KeyPage(struct _TImeSign *pSign);
                           
#endif //#define __TIME_SIGN_H

