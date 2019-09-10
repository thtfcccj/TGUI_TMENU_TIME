/*******************************************************************************

                           TWidget控件基类实现
此模块基类:TWIN(名柄)
*******************************************************************************/
#include "TWidget.h"
#include <string.h>
#include "TWidgetPrv.h"  //内部数据,仅供内部使用
/**********************************************************************
                         成员操作函数
**********************************************************************/
//-----------------------设置用户剪切区的起始位置y---------------------
void TWidget_SetY(TWidget_t *pWidget,TItemSize_t Y)
{
  unsigned char Page = TWidget_GetVPageSize(pWidget);
  TItemSize_t MaxSel = pWidget->ItemsRect.h - 1;
  if(MaxSel <= Page) Y = 0;//一页够显示了
  else{
    //MaxY -= Page;//最后一页需显示全时
    if(Y > MaxSel) Y = MaxSel;//最后一页
  }
  pWidget->ItemsRect.y = Y;
}


//-----------------------设置当前所选项------------------------
void TWidget_SetSel(TWidget_t *pWidget,TItemSize_t Sel)
{
  unsigned char Page = TWidget_GetVPageSize(pWidget);
  TItemSize_t MaxSel = pWidget->ItemsRect.h - 1;
  if(Sel > MaxSel) Sel = MaxSel;
  pWidget->Sel = Sel;
  //让其能够在当前页显示
  if(Sel >= (Page + pWidget->ItemsRect.y))
    pWidget->ItemsRect.y = Sel - Page + 1;
  //在第一页时,让其能够显示
  else if(Sel < pWidget->ItemsRect.y)
    pWidget->ItemsRect.y = Sel;
}
//------------------所选项增一,返回调整后的选择项---------------
TItemSize_t TWidget_IncSel(TWidget_t *pWidget)
{
  TItemSize_t  Sel = pWidget->Sel;
  TItemSize_t MaxSel = pWidget->ItemsRect.h - 1;
  if(Sel >= MaxSel){  //已在最大项了
    if(pWidget->Flag & TWIDGET_EN_RETURN)
      Sel = 0;
    else
      Sel = MaxSel; //不回环
  }
  else Sel++;
  pWidget->Sel = Sel;
  return Sel;
}
//----------------所选项减一,返回调整后的选择项--------------
TItemSize_t TWidget_DecSel(TWidget_t *pWidget)
{
  TItemSize_t  Sel = pWidget->Sel;
  if(!Sel){
    if(pWidget->Flag & TWIDGET_EN_RETURN)
      Sel = pWidget->ItemsRect.h - 1; //到最后一项
  }
  else Sel--;
  pWidget->Sel = Sel;
  return Sel;
}

//-------------------------设置项总数-----------------------------
void TWidget_SetItems(TWidget_t *pWidget,TItemSize_t Items)
{
  //if(!Items) return;  //禁止为0
  pWidget->ItemsRect.h = Items;
  //休正选择
  if(pWidget->Sel > Items) pWidget->Sel = Items;
}

//--------------------预设项长度,即项宽度----------------------
void  TWidget_SetW(TWidget_t *pWidget,TItemSize_t w)
{
  //if(pWidget->ItemsRect.w < w)
     pWidget->ItemsRect.w = w;
}

//----------------------得到显示区域x--------------------------------
unsigned char TWidget_GetDispX(const TWin_t *pWin,
                               unsigned char Flag)
{
  pWin = pWin;
  if(Flag & TWIDGET_EN_FRAME) return 2;
  return 0;
}

//----------------------得到显示区域宽度--------------------------------
unsigned char TWidget_GetDispW(const TWin_t *pWin,
                               unsigned char Flag)
{
  unsigned char w = TWin_GetW(pWin);
  if(Flag & TWIDGET_EN_FRAME) return w - 4;
  if(Flag & TWIDGET_EN_VSCOLL) return w - 2;
  return w; 
}

//----------------------得到显示区域y--------------------------------
unsigned char TWidget_GetDispY(const TWin_t *pWin,
                               unsigned char Flag)
{
  pWin = pWin;
  if(Flag & TWIDGET_EN_HEADER) return 1;
  return 0;
}

//----------------------得到显示区域高度--------------------------------
unsigned char TWidget_GetDispH(const TWin_t *pWin,
                               unsigned char Flag)
{
  unsigned char h = TWin_GetH(pWin);
  if(Flag & TWIDGET_EN_HEADER) h--;
  if(Flag & (TWIDGET_EN_FRAME | TWIDGET_EN_HSCOLL)) h--;
  return h;
}

//----------------------得到水平页大小--------------------------------
unsigned char TWidget_GetHPageSize(TWidget_t *pWidget)
{
  return TWidget_GetDispW(TWM_pGetWin(pWidget->hWin),pWidget->Flag);
}

//----------------------得到含允许提示外的水平页大小--------------------------------
unsigned char TWidget_GetHPageSizeEnNotify(TWidget_t *pWidget)
{
  unsigned char w = TWidget_GetDispW(TWM_pGetWin(pWidget->hWin), 
                                     pWidget->Flag);
  //若只含提示,排除提示
  if((w == pWidget->ItemsRect.w) && (pWidget->Flag & TWIDGET_EN_NOTIFY))
    w -= 2;
  return w;
}

//----------------------得到垂直页大小--------------------------------
unsigned char TWidget_GetVPageSize(TWidget_t *pWidget)
{
  return TWidget_GetDispH(TWM_pGetWin(pWidget->hWin),pWidget->Flag);
}

//----------------------得到用户剪切区绝对坐标x--------------------------------
unsigned char TWidget_GetClipX(TWidget_t *pWidget)
{
  TWin_t *pWin = TWM_pGetWin(pWidget->hWin);
  return TWin_GetX(pWin) + TWidget_GetDispX(pWin,pWidget->Flag);
}

//----------------------得到用户剪切区绝对坐标y--------------------------------
unsigned char TWidget_GetClipY(TWidget_t *pWidget)
{
  TWin_t *pWin = TWM_pGetWin(pWidget->hWin);
  return TWin_GetY(pWin) + TWidget_GetDispY(pWin,pWidget->Flag);
}


//设置用户标志
void TWidget_SetUserFlag(TWidget_t *pWidget,unsigned char Flag)
{
  pWidget->Flag &= ~TWIDGET_USER;
  pWidget->Flag |= (Flag & TWIDGET_USER);
}


/*/----------------------得到显示剪切区域函数-------------------------
//根据窗口大小和标志确认在窗口内的显示式区域
void _GetDispClip(TRect_t *pClip,
                  const TWin_t *pWin,
                  unsigned char Flag)
{
  unsigned char Data = TWin_GetY(pWin);
  unsigned char Len = TWin_GetH(pWin);
  if(Flag & TWIDGET_EN_FRAME){
    Len -= 2; //边框占两行
    Data += 1;
  }
  else if(Flag & TWIDGET_EN_HEADER){
    Len -= 1; //头占一行
    Data += 1;
  }
  pClip->h = Len;
  pClip->y = Data;
  Data = TWin_GetX(pWin);
  Len = TWin_GetW(pWin);
  if(Flag & TWIDGET_EN_FRAME){
    Data += 2;
    Len -= 4; //横边框要占4行
  }
  pClip->w = Len;
  pClip->x = Data;
}*/

//--------------------------大小改变通报函数-------------------------
//当改变TWidget_t对应的Win大小改变时,用于自动改变显示样式
void TWidget_WinReSizeNotify(TWidget_t *pWidget,
                             unsigned char w,
                             unsigned char h)
{
  unsigned char Flag = pWidget->Flag;
  //窗口过小时使用边框无效
  if((w < 10) || (h < 4)) Flag &= ~TWIDGET_EN_FRAME;
  //水平过小时使用水平滚动条无效
  if(w < 10) Flag &= ~TWIDGET_EN_HSCOLL;
  //高度过小时显示头无效
  if(h < 2) Flag &= ~TWIDGET_EN_HEADER;
  //高度过小时垂直滚动条无效
  if(h < 5) Flag &= ~TWIDGET_EN_VSCOLL;
  pWidget->Flag = Flag;
}

//--------------------------控制初始化函数-------------------------
//创建其它控件时调用此函数初始化基类
//此函数自动休正标志区标志,并将项显示区域置为允许在窗口内的最大宽度
//初始化为至少两个项,置认在0项
void TWidget_Init(TWidget_t *pWidget,
                  TWinHandle_t hWin,
                  unsigned char Flag)
{
  TWin_t *pWin;
  memset(pWidget,0,sizeof(TWidget_t));
  pWidget->hWin = hWin;
  pWidget->Sel = 0;
  pWidget->Flag = Flag;
  
  pWin = TWM_pGetWin(hWin);
  TWidget_WinReSizeNotify(pWidget,
                          TWin_GetW(pWin),
                          TWin_GetH(pWin));
  
  //项区域默认占满显示区域
  pWidget->ItemsRect.x = 0;//TWidget_GetDispX(pWin,Flag);
  pWidget->ItemsRect.y = 0;//TWidget_GetDispY(pWin,Flag);
  pWidget->ItemsRect.w = TWidget_GetDispW(pWin,Flag);//默认宽度
  pWidget->ItemsRect.h = 2; //项数默认为2项
}

//-----------------------按键处理函数---------------------------
//上下键在项间切换,左右键水平移动窗口,上下翻页键翻页,数字键选择项目
//返回标志位定义为置位时发生变化(见定义)
unsigned int TWidget_Key(TWidget_t *pWidget,unsigned char Key)
{
  TItemSize_t  Sel;
  TItemSize_t PrvSel = TWidget_GetSel(pWidget);//记住上次位置
  const TWin_t *pWin = TWM_pGetWin(pWidget->hWin);
  unsigned char Flag = pWidget->Flag;
  unsigned char PageSize;
  
  Sel = 0;//初始化防止部分编译器警告
  
  switch(Key){
  case TGUI_KEY_LEFT:{  //往左移动两个字
    Sel = pWidget->ItemsRect.x;
    if(!Sel) return 0;  //已在左边了
    if(Sel >= TGUI_HORI) Sel-= TGUI_HORI;
    else Sel = 0;
    pWidget->ItemsRect.x = Sel;
    return TGUI_PAINT_HCONTEXT | TGUI_PAINT_FOCUS;
  }
  case TGUI_KEY_RIGHT:{//若显示不完全,则可以往右移动字
    Sel = pWidget->ItemsRect.x;
    if((pWidget->ItemsRect.w - Sel) <= TWidget_GetDispW(pWin,Flag))
      return 0;
    pWidget->ItemsRect.x += TGUI_HORI;
    return TGUI_PAINT_HCONTEXT | TGUI_PAINT_FOCUS;
  }
  case TGUI_KEY_DOWN:{
    Sel = TWidget_IncSel(pWidget);
    if(Sel == PrvSel) return 0;   //未变化
    if(Sel > PrvSel){//下移了,检查窗口移动
      if(Sel >= (pWidget->ItemsRect.y + TWidget_GetDispH(pWin,Flag))){
        pWidget->ItemsRect.y++;
        return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;
      }
      return  TGUI_PAINT_FOCUS;//焦点变化
    }
    pWidget->ItemsRect.y = 0;   //回环了
    return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;
  }
  case TGUI_KEY_UP:{
    Sel = TWidget_DecSel(pWidget);
    if(Sel == PrvSel) return 0;   //未变化
    if(Sel < PrvSel){//上移了,检查窗口移动
      if(Sel < pWidget->ItemsRect.y){
        pWidget->ItemsRect.y--;
        return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;
      }
      return  TGUI_PAINT_FOCUS;//焦点变化
    }
    PageSize = TWidget_GetDispH(pWin,Flag);
    if(pWidget->ItemsRect.h > PageSize)
      pWidget->ItemsRect.y = pWidget->ItemsRect.h - PageSize;   //回环到最后了
    return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;  
  }
  case TGUI_KEY_PAGEUP:{
    PageSize = TWidget_GetDispH(pWin,Flag);
    if(pWidget->ItemsRect.h <= PageSize) return 0;//一页够显示了
    if(!PrvSel){//在第一行
      if(Flag & TWIDGET_EN_RETURN){//回环到最后一行
        TWidget_SetSel(pWidget,pWidget->ItemsRect.h - 1);
        return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;
      }
      return 0;
    }
    //同时Y轴改变
    if(pWidget->ItemsRect.y > PageSize)
      TWidget_SetY(pWidget,pWidget->ItemsRect.y - PageSize);
    //其它行
    if(PrvSel > PageSize) Sel = PrvSel - PageSize;
    else Sel = 0;
    TWidget_SetSel(pWidget,Sel);

    return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;
  }
  case TGUI_KEY_PAGEDOWN:{     //后加,下一页
    PageSize = TWidget_GetDispH(pWin,Flag);
    //超过最后一页了
    if((pWidget->ItemsRect.y + PageSize) >= pWidget->ItemsRect.h){
      if(Flag & TWIDGET_EN_RETURN){//回环到第一页
        TWidget_SetSel(pWidget,0);
        return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;
      }
      return 0;
    }
    //下一页,同时Y轴改变
    Sel = PrvSel + PageSize;       
    TWidget_SetY(pWidget,pWidget->ItemsRect.y + PageSize);
    TWidget_SetSel(pWidget,Sel);
    return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;
  } 
  default:{
    if((Key >= '1') && (Key <= '9')){
      TWidget_SetSel(pWidget, Key - '1');
      //确保能够显示所选项
      Sel = TWidget_GetSel(pWidget);
      if(PrvSel == Sel) break;
      //选项改变时
      PrvSel = pWidget->ItemsRect.y;//暂存
      if(Sel < PrvSel){
        pWidget->ItemsRect.y = Sel;
      }
      else{
        PrvSel += TWidget_GetDispH(pWin,Flag);
        if(Sel >= PrvSel)//在下面页时显示在最后一行
         pWidget->ItemsRect.y = PrvSel;
      }
      return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;
    }
  }
  }
  return 0;
}

