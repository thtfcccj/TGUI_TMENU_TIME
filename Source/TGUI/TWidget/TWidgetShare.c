/* ----------------------------------------------------------------------------
 *                TGUI通用型函数
 * --------------------------------------------------------------------------*/



#include "TWidgetShare.h"
#include "string.h"

//-------------得到附加最多2项的起始位置函数---------------------
//以TWidget_t为基类的控件中,管理附加项如:"   [保  存]    [返 回] "的焦点显示
static TItemSize_t _Append2_GetX(//0x80:在附加项里标志,0x01:带保存标志,0x02:带返回标志
                                 unsigned char Flag,
                                 unsigned char Len1st,  //首字长度
                                 unsigned char Len2nd,  //次字长度
                                 unsigned char Space,   //两个按键时,其间隔
                                 TItemSize_t ItemLen)    //项总长
                  
{
  unsigned char Len;
  //得到首字位置Pos
  if(Flag  & 0x01) Len = Len1st;
  else Len = 0;
  if(Flag  & 0x02) Len += Len2nd;
  if(Flag == 0x03) 
    Len += Space;//两个按钮时
  if(ItemLen < Len) return 0;//不够显示
  return GetAlignLenR((ItemLen - Len) >> 1,0); //对齐
}



//-------------附加最多2项的反显回调实现函数---------------------
//以TWidget_t为基类的控件中,管理附加项如:"   [保  存]    [返 回] "的焦点显示
void Append2_AutiNotify(TWidget_t *pWidget,//基类
                           //0x80:在附加项里标志,0x01:带保存标志,0x02:带返回标志
                           unsigned char Flag,
                           unsigned char InReturn,//两个按钮时焦点在返回上标志
                           unsigned char Len1st,  //首字长度
                           unsigned char Len2nd)  //次字长度
{
  TItemSize_t Pos;
  unsigned char Len,Space;
  if(!(Flag & 0x80)) return; //不处理,也不能到此
  Flag &=  0x03;
  
  //得到首字位置Pos
  Space = cbGetBottomSpace(TWidget_GetW(pWidget));
  //固定以窗口宽度局中对齐，若显示窗口大，则以显示窗口为准
  Pos = TWin_GetW(TWM_pGetWin(pWidget->hWin));
  if(pWidget->ItemsRect.w > Pos) Pos = pWidget->ItemsRect.w;
  Pos = _Append2_GetX(Flag, Len1st, Len2nd, Space, Pos);
  if(Pos == 0) Space = 0;//强制去除以获得足够宽度
  //得到焦点长度
  if(Flag == 0x03){//同时两个键时
    if(InReturn){//焦点在返回上
      Pos += Len1st + Space;
      Len = Len2nd;
    }
    else Len = Len1st;
  }
  else if(Flag == 0x01) //仅保存按钮
    Len = Len1st;
  else //返回按钮
    Len = Len2nd;
  //置焦点
  TWidget_SetFocus(pWidget, TWidget_GetSel(pWidget), Pos, Len);
}

//----------------可能附带最多两个附加项的得到字符函数实现--------------------------
//以TWidget_t为基类的,最后一项可能有最多两具焦点的高级控件可调用此函数进行管理
//返回焦点在那个按钮上
const char* Append2_GetString(TWidget_t *pWidget,//基类
                              //0x01:首个按钮标志,0x02:第二个按钮标志
                              unsigned char Flag,
                              const char *p1st,
                              const char *p2nd)
{
  unsigned char Space;
  unsigned char Pos;
  char *pBuf;
  //得到首字位置Pos
  Space = cbGetBottomSpace(TWidget_GetW(pWidget));
  //固定以窗口宽度局中对齐，若显示窗口大，则以显示窗口为准
  Pos = TWin_GetW(TWM_pGetWin(pWidget->hWin));
  if(pWidget->ItemsRect.w > Pos) Pos = pWidget->ItemsRect.w;
  Pos = _Append2_GetX(Flag,strlen(p1st),strlen(p2nd),Space, Pos);
  if(Pos == 0) Space = 0;//强制去除以获得足够宽度
  
  pBuf = TGUI_pGetBuf();       //使用共享缓冲
  memset(pBuf,' ', Pos); pBuf += Pos;   //前面填充空格
  if(Flag & 0x01) pBuf = strcpy_ex(pBuf,p1st);//首字填充
  if(Flag == 0x03){//按键间空格
    memset(pBuf,' ', Space);  
    pBuf += Space;
  }
  if(Flag & 0x02)pBuf = strcpy_ex(pBuf,p2nd);//末字填充
  
  //尾空格填充
  if((TGUI_pGetBuf() + TWidget_GetW(pWidget)) > pBuf){
    Space = TGUI_pGetBuf() + TWidget_GetW(pWidget) - pBuf;
    memset(pBuf,' ', Space);
    pBuf += Space;  
  }
  *pBuf = '\0'; //结束字符
  
  return TGUI_pGetBuf();
}

//----------------可能附带最多两个附加项的键处理函数实现--------------------------
//以TWidget_t为基类的,最后一项可能有最多两具焦点的高级控件可调用此函数进行管理
//返回-1：需执行标准按键处理程序后再调用此函数
//正常返回0:焦点在第一个按钮上
//1:焦点在第二个按钮上
//2:焦点未发生变化
//否则返回焦点在那个按钮上,
unsigned char Append2_Key(TWidget_t *pWidget,//基类,
                          unsigned char Key,//键值
                          //0x01:首个按钮标志,0x02:第二个按钮标志
                          unsigned char Flag,
                          //-1:再次进入此函数
                          //其它:当前焦点在第二个按钮上标志
                          unsigned char In2)
{
  TItemSize_t MaxItem = TWidget_GetItems(pWidget) - 1;
  static TItemSize_t Sel;//上次选择

  if(In2 == (unsigned char)-1){ //再次进入
    //所选项未发生变化时
    if(Sel == TWidget_GetSel(pWidget))
      return 2;
    Sel = TWidget_GetSel(pWidget);
    //没到附加项,不处理
    if(Sel != MaxItem) return 2;
    //其它地方到附加项时,只有按下键才能到保存键上
    if(Key == TGUI_KEY_DOWN) 
      return 0;
    else 
      return 1;
  }

  //首次进入:
  Sel = TWidget_GetSel(pWidget);//记住上次选择
  //一个按钮或无附加时,或不是上下键时,底层直接就可以搞定了
  if(Flag != 0x03) 
    return (unsigned char)-1;

  //不在最后一行时,由底层搞定
  if(TWidget_GetSel(pWidget) != MaxItem)
    return (unsigned char)-1;

  //最后一行时,左右键当做上下键处理
  if(Key == TGUI_KEY_LEFT) Key = TGUI_KEY_UP;
  else if(Key == TGUI_KEY_RIGHT) Key = TGUI_KEY_DOWN;
  //两个按钮不是上下键,由底层搞定
  if((Key != TGUI_KEY_UP) && (Key != TGUI_KEY_DOWN))
    return (unsigned char)-1;
  
  //两个按钮按上键或下键
  //焦点已在最后一行,两个按钮时按上键或下键的处理:

  //焦点在第二个按钮上,向上按键了,焦点转至保存
  if(In2 && (Key == TGUI_KEY_UP))
    In2 = 0;
  //焦点在保存,向下按键了,焦点转至返回
  else if((!In2) && (Key == TGUI_KEY_DOWN))
    In2 = 1;
  else return (unsigned char)-1; //底层处理

  return In2;
}
