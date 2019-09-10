/*****************************************************************************
                         TGUI上的输入法模块-数字键输入法部分
*****************************************************************************/
#include "TImeNum.h"
#include "TImeBase.h"
#include <string.h>
/*****************************************************************************
                              说明
*****************************************************************************/
//此函数内部不输出任何显示
//函数调用流程说明:
//1:切换到数字输入法时，调用初始化函数进行初始化操作
//2:用户按数字键时，调用按键处理函数响应按键
//3:得到字符串: 调用得到字符串函数得到显示与输入字符。




/*****************************************************************************
                                相关函数实现
*****************************************************************************/

//---------------------------初始化函数---------------------------------
//切换到数字输入法时调用
void TImeNum_Init(struct _TImeNum *pNum)
{
  memset(pNum, 0, sizeof(struct _TImeNum));
}

//-------------------------填充第四行用户提示-----------------------------------

static const char _chSelNum[] = {"0-9: (连接按两次为第二个)"};
void TImeNum_FullTitle(const struct _TImeNum *pNum,//输入法结构
                           char *pStr)
{
  strcpy(pStr, _chSelNum);
}

//-----------------------数字输入法相关字符资源----------------------------
static const char _chNum01[] =  {"01"};
static const char _chNum23[] =  {"23"};
static const char _chNum45[] =  {"45"};
static const char _chNum67[] =  {"67"};
static const char _chNum89[] =  {"89"};
static const char *const _NumArrow[] = {
  _chNum01, // TIME_MNG_KEY_UP 
  _chNum23, //TIME_MNG_KEY_LEFT
  _chNum45, //TIME_MNG_KEY_ENTER
  _chNum67, //TIME_MNG_KEY_RIGHT
  _chNum89, //TIME_MNG_KEY_DOWN
  chReturn, //TIME_MNG_KEY_EXIT,
};

//-------------------------得到对应键值字符函数-----------------------------------
//如：上键对应 返回NULL表示不需要字符
const char *TImeNum_pGetArrowNotice(struct _TImeNum *pNum,//输入法结构
                                         unsigned char GuideKey)//对应键值位置
{ 
  return  _NumArrow[GuideKey];
}

//--------------------------按键响应函数-----------------------------------
//若修改完成，则返回对应字符，否则0: 不修改返回,  0xffff: 内部操作过程中 
unsigned short TImeNum_Key(struct _TImeNum *pNum, unsigned char GuideKey)
{
  if(GuideKey == TIME_MNG_KEY_EXIT)//单独处理退出键
    return 0;
  
  if(pNum->NumTimer){//第二次选择时
    char Chr = '0' + (pNum->FirstKey * 2);
    if(GuideKey == pNum->FirstKey)//相同按键了
      Chr++;//为下一个
    //else //第二次按任意键认为是首个
    return (unsigned short)Chr;
  }
  //第一次时
  pNum->NumTimer = TIME_NUM_TIMER_OV;
  pNum->FirstKey = GuideKey;
  return 0xffff;
}

//-----------------------------任务函数----------------------------------
//128ms调用一次, 若修改完成，返回对应输入完成的数字，否则0不处理
unsigned short TImeNum_Task(struct _TImeNum *pNum)
{
  if(!pNum->NumTimer) return 0;
  pNum->NumTimer--;
  if(!pNum->NumTimer){//时间到自动确认
    return (unsigned short)('0' + (pNum->FirstKey * 2));
  }
  return 0;
}



