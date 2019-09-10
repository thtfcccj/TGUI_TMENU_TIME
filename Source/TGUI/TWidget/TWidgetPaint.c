/*******************************************************************************

                           TGUI窗口基本部件之绘图相关函数实现

*******************************************************************************/
#include "TWidget.h"
#include <string.h>
#include "TWidgetPrv.h"  //内部数据

//--------------------得到项剪切显示空间Y函数-------------*
//返回剪切后的位置,若不在显示空间内时,将返回-1
static TItemSize_t _GetClipY(TWidget_t *pWidget,
                               TItemSize_t VPageSize,
                               TItemSize_t Item)
{
  TItemSize_t y =  pWidget->ItemsRect.y;
  //确定所在项是否在显示范围
  if(Item < y) return (TItemSize_t)-1;//在显示区上方
  Item -= y;  //转换为窗口内坐标
  if(Item >= VPageSize)
    return (TItemSize_t)-1;//在显示区下方
  return Item;
}

//--------------------得到项剪切显示空间x函数-------------*
//返回剪切后的位置
static TItemSize_t _GetClipX(TWidget_t *pWidget,
                               unsigned char HPageSize,
                               TItemSize_t ItemX,//该项的X轴位置
                               TItemSize_t *pW) //送入宽度,返回剪切后宽度
{
  TItemSize_t x =  pWidget->ItemsRect.x;
  TItemSize_t w = *pW;
  
  //左侧剪切
  if(ItemX < x){  //左移了
    if(w >= x){//有显示时
      w -= x;//左移后余下宽度
      if(w > HPageSize)//仅显示一页
        w = HPageSize;
    }
    else w = 0;//移多了，不能显示了
    *pW = w;
    return 0;
  }
  
  ItemX -= x;  //转换为窗口内坐标
  if(ItemX > HPageSize) //超过为显示范围了
    w = 0;//不在显示区
  else if((ItemX + w) > HPageSize)
    w = HPageSize - ItemX;
  
  *pW = w;
  return ItemX;
}

//-------------------画标题头或上边框函数----------------------------*
//此函数画上侧边框,含标题头
void TWidget_PaintHeader(TWidget_t *pWidget,
                        const char *pHeader,   //显示头,无时为NULL
                        unsigned char Len)    //显示头数据长度,若为0,则应含结束字符
{
  TWin_t *pWin;
  char *pBuf;
  unsigned char w,i;
  unsigned char Flag = pWidget->Flag;
  if(!(Flag & TWIDGET_EN_HEADER)) return;//不需要显示头
  
  pWin = TWM_pGetWin(pWidget->hWin);
  pBuf = TWin_pGetString(pWin,0);
  
  if(Flag & TWIDGET_EN_FRAME){*pBuf++ = 0xa9;*pBuf++ = 0xb0;}//带框时,填充"┌"
  else {*pBuf++ = 0xa9;*pBuf++ = 0xa4;}//不带框时,填充"─"
  if(pHeader == NULL) Len = 0;//无头显示
  else if(!Len) Len = strlen((const char*)pHeader);
  
  w = TWin_GetW(pWin) - 4;
  if(Len > w) {Len = w; w = 0;}//w这里表示左侧位置
  else w = (w - Len) >> 2;//局中显示,双字节对齐
  
  //填充"─"
  for(i = 0; i < w; i++) {*pBuf++ = 0xa9;*pBuf++ = 0xa4;}//左侧填充"─"
  memcpy(pBuf,pHeader,Len);
  w = TWin_GetW(pWin) - (w << 1) - Len - 2;//右侧宽度
  pBuf += Len;
  if(w & 0x01){w += 1; *pBuf++= ' ';}//保证双字节对齐
  if(w < 2) return;//异常退出
  w -= 2;//用于填充最后┐预留
  w >>= 1;
  for(i = 0; i < w; i++) {*pBuf++ = 0xa9;*pBuf++ = 0xa4;}//右侧填充"─"
  
  if(Flag & TWIDGET_EN_FRAME){*pBuf++ = 0xa9;*pBuf++ = 0xb4;}//带框时,填充"┐"
  else {*pBuf++ = 0xa9;*pBuf = 0xa4;}//不带框时,填充"─"
}

//---------------------得到行缓冲区函数---------------------------*
//此函数画左侧边框,并输出行缓冲位置及长度供外部程序填充
//若返回空,表示不可接收字符
//注:可接收数据长度可调用TWidget_pGetPaintRowLen()得到
//项字符起始位置可调用TWidget_pGetPaintRowItemStart()得到
char *TWidget_pGetPaintRow(TWidget_t *pWidget,
                                    TItemSize_t Item,    //需要显示的项
                                    TItemSize_t PrvLen) //上一行用户字符长度
{
  char *pBuf;
  TWin_t *pWin = TWM_pGetWin(pWidget->hWin);
  unsigned char Flag = pWidget->Flag;
  TItemSize_t w;
  TItemSize_t VPageSize = TWidget_GetDispH(pWin,Flag);
  //统计项区宽度
  w = pWidget->ItemsRect.w;
  if(PrvLen > w) pWidget->ItemsRect.w = PrvLen;
  //Y轴剪切  
  Item = _GetClipY(pWidget,VPageSize,Item);
  if(Item == (TItemSize_t)-1) return NULL;//不在显示范围

  //显示缓冲区
  pBuf = TWin_pGetString(pWin,Item + TWidget_GetDispY(pWin,Flag));
  if(pBuf == NULL) return NULL;//显示缓冲出错
  
  //无边框时,左侧边框位置可省了
  if(!(pWidget->Flag & TWIDGET_EN_FRAME))
    return pBuf;
  
  //有边框时最前填充"│"
  *pBuf++= 0xa9;
  *pBuf++= 0xa6;
  return pBuf;
}

//---------------------设置焦点区函数------------------------*
//此函数负责设置焦点(一般被选项获得焦点)
void TWidget_SetFocus(TWidget_t *pWidget,
                      TItemSize_t y,     //在项空间纵向位置
                      TItemSize_t x,     //在项空间横向位置,>项空间时将不显示焦点
                      TItemSize_t w)     //焦点宽度,0表示x至整个项长度全为焦点
{

  
  unsigned char Flag;
  unsigned char VPageSize;
  TWin_t *pWin = TWM_pGetWin(pWidget->hWin);
  
  TItemSize_t ItemW = pWidget->ItemsRect.w; 
  //超出显示范围,不显不焦点
  if(x > ItemW){
    TWin_ClrFocus(pWin);
    return;
  }
  
  Flag = pWidget->Flag;
  pWin = TWM_pGetWin(pWidget->hWin);
  VPageSize = TWidget_GetDispH(pWin,Flag);

  //Y轴剪切
  y = _GetClipY(pWidget,VPageSize,y); 
  if(y == (unsigned char)-1){//焦点不在显示范围
    TWin_ClrFocus(pWin);
    return;
  }
  //休正范围合法性
  if(!w) w = ItemW - x;
  else if((w + x) > ItemW) w = ItemW - x;
  
  //X轴剪切
  x = _GetClipX(pWidget,TWidget_GetDispW(pWin,Flag),x,&w);

  if(w) TWin_SetFocus(pWin,x + TWidget_GetDispX(pWin,pWidget->Flag),
                       y + TWidget_GetDispY(pWin,pWidget->Flag),w);
  else TWin_ClrFocus(pWin); //在剪切区域外,不能显示
}

//---------------用于计算滚动条滚动柄长度函数------------------------
//引自UCGUI->Scollbar.c
unsigned short _DivideRound(unsigned short a, unsigned short b)
{
  unsigned short r = 0;
  if (b) {
    r = ((a + b/2) / b);
  }
  return r;
}

//---------------------画水平滚动条函数------------------------
//示例:带边框:"└←━━━━━━━━━━━→┘"
//不带边框:"  ←━━━━━━━━━━━→  "
static void _FullHScoll(TWin_t *pWin,
                        unsigned char Flag,   //标志
                        TItemSize_t ItemW,  //项窗口宽度
                        TItemSize_t ItemX,  //项窗口位置
                        TItemSize_t DispW)  //显示窗口宽度,>=10
{
  //在最后一行画图
  TItemSize_t ThumbSize,Thumb,Moveable,i;
  char *pBuf = TWin_pGetString(pWin,TWin_GetH(pWin) - 1);
  //填充左侧└←
  if(Flag & TWIDGET_EN_FRAME) {*pBuf++= 0xa9; *pBuf++= 0xb8;} //画"└"
  *pBuf++= 0xa1; *pBuf++= 0xfb;//画"←"

  //得到滚动条把柄轴长度与左测可滚动区域
  DispW >>= 1;  //水平滚动条将两个字看成一个字
  
  Moveable = DispW - 2; //得到可滚动区长度
    
  if(ItemW > DispW){  //分页显示时
    //水平滚动条将两个字看成一个字
    ItemW >>= 1; ItemX >>= 1;
    ThumbSize = _DivideRound(Moveable * DispW,ItemW);//滚动把柄轴长度
    if(!ThumbSize) ThumbSize = 1;
    Thumb = Moveable - ThumbSize;//得到不可滚动区域大小
    Thumb = _DivideRound(Thumb * ItemX,ItemW - DispW);//得到起始位置
  }
  else{//单页显示时,能显示完全
    ThumbSize = Moveable;
    Thumb = 0;
  }
  //填充左滚动区域
  for(i = 0; i < Thumb; i++) {*pBuf++= 0xa9; *pBuf++= 0xa4;} //画"─"
  //填充滚动把柄轴
  for(i = 0; i < ThumbSize; i++) {*pBuf++= 0xa9; *pBuf++= 0xa5;} //画"━"
  //填充右滚动区域
  Thumb += ThumbSize;
  if(Moveable > Thumb){
    Thumb = Moveable - Thumb;
    for(i = 0; i < Thumb; i++) {*pBuf++= 0xa9; *pBuf++= 0xa4;} //画"─"
  }
  //填充右侧→┘
  *pBuf++= 0xa1; *pBuf++= 0xfa;//画"→"
  if(Flag & TWIDGET_EN_FRAME) {*pBuf++= 0xa9; *pBuf++= 0xbc;} //画"┘"
}

//-------------画无水平滚动条的下边框函数------------------------
//示例:左靠齐,右有数据: "└━━━━━━━━━━━→┘
static void _FullHFrame(TWin_t *pWin,
                        TItemSize_t ItemW,  //项窗口宽度
                        TItemSize_t ItemX,  //项窗口位置
                        TItemSize_t DispW)  //显示窗口宽度,>=10
{
  //在最后一行画图
  TItemSize_t i,w;
  char *pBuf = TWin_pGetString(pWin,TWin_GetH(pWin) - 1);
  
  *pBuf++= 0xa9; *pBuf++= 0xb8; //画"└"

  if(!ItemX){*pBuf++= 0xa9; *pBuf++= 0xa5;} //画"━"表示左测无数据
  else{*pBuf++= 0xa1; *pBuf++= 0xfb;}//画"←"表示左测有数据

  w = DispW >> 1;
  for(i = (4/2); i < w; i++) {*pBuf++= 0xa9; *pBuf++= 0xa5;} //画"━"

  if((ItemX + DispW) >= ItemW){//画"━"表示右测无数据
    *pBuf++= 0xa9; *pBuf++= 0xa5;} 
  else{*pBuf++= 0xa1; *pBuf++= 0xfa;}//画"→"表示右测有数据

  *pBuf++= 0xa9; *pBuf++= 0xbc; //画"┘"
}

//-------------水平无滚动条无边框时画提示函数------------------------*
//示例:左靠齐,右有数据: "├　　5. 参数设置    　→  "
static void _FullHHint(TWin_t *pWin,
                        TItemSize_t ItemW,  //项窗口宽度
                        TItemSize_t ItemX,  //项窗口位置
                        TItemSize_t DispW)  //显示窗口宽度,>=10
{
  //在最后一行画图(此时应显示用户内容)
  char *pBuf;
  if(ItemW <= DispW) return;//够显,不提示

  pBuf = TWin_pGetString(pWin,TWin_GetH(pWin) - 1);
  //pBuf += TWidget_GetDispX(pWin,Flag);//有边框就不会有提示了

  if(!ItemX){*pBuf= 0xa9; *(pBuf + 1)= 0xc0;} //画"├"表示左测无数据
  else{*pBuf= 0xa1; *(pBuf + 1)= 0xfb;}//画"←"表示左测有数据

  pBuf += DispW - 4;
  if((ItemX + DispW) >= ItemW){//画"  ┤"表示右测无数据
    *pBuf++= 0xa9; *pBuf= 0xc8;} 
  else{*pBuf++= 0xa1; *pBuf= 0xfa;}//画"→"表示左测有数据
  
}

//---------------------画垂直滚动条函数------------------------
//示例:与水平滚动条逆时针转90度相同
static void _FullVScoll(TWin_t *pWin,
                        unsigned char Flag,   //标志
                        TItemSize_t ItemH,  //项窗口高度
                        TItemSize_t ItemY,  //项窗口位置
                        TItemSize_t DispH)  //显示窗口高度,>=3
{
  TItemSize_t ThumbSize,Thumb,Moveable,i;
  unsigned char y;//当前操作位置,在内容行填充
  char *pBuf;
  
  if(DispH < 3) return;//上层出错了,忽略
  if(Flag & (TWIDGET_EN_FRAME | TWIDGET_EN_HEADER)) y = 1;
  else y = 0;
  
  pBuf = TWin_pGetString(pWin,y++);
  pBuf += pWin->Rect.w - 2;//在最后一行画
  if(DispH < 2){//只有一行时只画"│"
    *pBuf++= 0xa9; *pBuf= 0xa7;//画"┃"
    return;
  }
  *pBuf++= 0xa1; *pBuf = 0xfc;//画上侧"↑"
  
  //得到滚动条把柄轴长度与左测可滚动区域
  Moveable = DispH - 2; //可滚动区长度
  if(ItemH > DispH){  //分页显示时
    ThumbSize = _DivideRound(Moveable * DispH,ItemH);//滚动把柄轴长度
    if(!ThumbSize) ThumbSize = 1;
    Thumb = Moveable - ThumbSize;//得到不可滚动区域大小
    Thumb = _DivideRound(Thumb * ItemY,ItemH - DispH);//得到起始位置
  }
  else{//单页显示时,能显示完全
    ThumbSize = Moveable;
    Thumb = 0;
  }
  //填充上滚动区域
  for(i = 0; i < Thumb; i++){
    pBuf = TWin_pGetString(pWin,y++);
    pBuf += pWin->Rect.w - 2;//在最后一行画
    *pBuf++= 0xa9; *pBuf= 0xa6;;//画"│"
  }
  //填充滚动把柄轴
  for(i = 0; i < ThumbSize; i++) {
    pBuf = TWin_pGetString(pWin,y++);
    pBuf += pWin->Rect.w - 2;//在最后一行画
    *pBuf++= 0xa9; *pBuf= 0xa7;//画"┃"
  } 
  //填充下滚动区域
  Thumb += ThumbSize;
  if(Moveable > Thumb){
    Thumb = Moveable - Thumb;
    for(i = 0; i < Thumb; i++){
      pBuf = TWin_pGetString(pWin,y++);
      pBuf += pWin->Rect.w - 2;//在最后一行画
      *pBuf++= 0xa9; *pBuf= 0xa6;//画"│"
    } 
  }
  pBuf = TWin_pGetString(pWin,y);
  pBuf += pWin->Rect.w - 2;//在最后一行画
  *pBuf++= 0xa1; *pBuf= 0xfd;//画"↓"
  
}

//-------------画无垂直滚动条的右边框函数------------------------
//示例:与画无水平滚动条的下边框逆时针转90度相同
static void _FullVFrame(TWin_t *pWin,
                        unsigned char Flag,   //标志
                        TItemSize_t ItemH,  //项窗口高度
                        TItemSize_t ItemY,  //项窗口位置
                        TItemSize_t DispH)  //显示窗口高度,>=2
{
  TItemSize_t i;
  unsigned char y;//当前操作位置,在内容行填充
  char *pBuf;
  
  if(Flag & TWIDGET_EN_FRAME) y = 1;
  else y = 0;
  
  pBuf = TWin_pGetString(pWin,y++);
  pBuf += pWin->Rect.w - 2;//在最后一行画
  if(!ItemY){*pBuf++= 0xa9; *pBuf++= 0xa7;} //画"┃"表示上测无数据
  else{*pBuf++= 0xa1; *pBuf++= 0xfc;}//画"↑"表示上测有数据

  for(i = 2; i < DispH; i++){
    pBuf = TWin_pGetString(pWin,y++);
    pBuf += pWin->Rect.w - 2;//在最后一行画
    *pBuf++= 0xa9; *pBuf= 0xa7;//画"┃"
  }
  
  pBuf = TWin_pGetString(pWin,y++);
  pBuf += pWin->Rect.w - 2;//在最后一行画
  if((ItemY + DispH) >= ItemH){//画"┃"表示下测无数据
    *pBuf++= 0xa9; *pBuf++= 0xa7;} 
  else{*pBuf++= 0xa1; *pBuf++= 0xfd;}//画"↓"表示右测有数据
}


//-------------垂直无滚动条无边框时画提示函数------------------------*
//示例:与水平无滚动条无边框时逆时针转90度相同
static void _FullVHint(TWin_t *pWin,
                        unsigned char Flag, 
                        TItemSize_t ItemH,  //项窗口高度
                        TItemSize_t ItemY,  //项窗口位置
                        TItemSize_t DispH)  //显示窗口高度,>=2
{
  char *pBuf;
  TItemSize_t DispW =  TWidget_GetDispW(pWin,Flag);
  if(ItemH <= DispH) return;//够显,不提示
  pBuf = TWin_pGetString(pWin,TWidget_GetDispY(pWin,Flag)) + DispW - 2;

  if(!ItemY){*pBuf= 0xa9; *(pBuf + 1)= 0xd0;} //画"┬"表示上测无数据
  else{*pBuf= 0xa1; *(pBuf + 1) = 0xfc;}//画"↑"表示上测有数据
  

  pBuf = TWin_pGetString(pWin, 
                  TWidget_GetDispY(pWin,Flag) + DispH - 1) + DispW - 2;
  if((ItemY + DispH) >= ItemH){//画"┴"表示下测无数据
    *pBuf++= 0xa9; *pBuf++= 0xd8;} 
  else{*pBuf++= 0xa1; *pBuf++= 0xfd;}//画"↓"表示右测有数据
}

//---------------------结束画图函数---------------------------*?
//此函数负责画右侧与下侧边框
void TWidget_PaintEnd(TWidget_t *pWidget,
                      unsigned char PaintFlag,//定义的绘图标志
                      TItemSize_t LastW)  //最后一行数据长度
{
  TItemSize_t ItemLen,ItemPos;//项高或宽,x轴或y轴
  TItemSize_t DispLen;//显示区高或宽
  unsigned char Flag;
  TWin_t *pWin;
  char *pBuf;
  
  //============先统计项区宽度=========
  ItemLen = pWidget->ItemsRect.w;
  if(LastW > ItemLen){
    ItemLen = LastW;
    pWidget->ItemsRect.w = ItemLen;
  }

  Flag = pWidget->Flag;
  pWin = TWM_pGetWin(pWidget->hWin);

  //若项空间项数小于显示窗口,影响该区域时后面的区域清零
  DispLen = TWidget_GetDispH(pWin,Flag);
  ItemLen = TWidget_GetDispY(pWin,Flag); //复用作显示区x
  ItemPos = pWidget->ItemsRect.h - pWidget->ItemsRect.y;
  if(PaintFlag & (TGUI_PAINT_VCONTEXT | TGUI_PAINT_DATA)){
    for(; ItemPos < DispLen; ItemPos++){
      pBuf = TWin_pGetString(pWin,ItemPos + ItemLen);
      memset(pBuf, ' ',TWin_GetW(pWin));
      if(Flag & TWIDGET_EN_FRAME){//补充左边框有边框时最前补充"│
        *pBuf++= 0xa9;
        *pBuf = 0xa6;
      }
    } 
  }
  
  //无边框与水平滚动条时,因水平提示内容在最后一内容栏,需强制重画提示
  if(!(Flag & (TWIDGET_EN_FRAME | TWIDGET_EN_HSCOLL)))
    PaintFlag |= TGUI_PAINT_HCONTEXT;
  if(!(Flag & (TWIDGET_EN_FRAME | TWIDGET_EN_VSCOLL)))
    PaintFlag |= TGUI_PAINT_VCONTEXT;
  

  
  //==============填充水平边框===========
  if(PaintFlag & TGUI_PAINT_HCONTEXT){
    ItemPos = pWidget->ItemsRect.x;         //项X位置  
    DispLen = TWidget_GetDispW(pWin,Flag);//显示项宽度
    if(Flag & TWIDGET_EN_HSCOLL)//画水平滚动条
      _FullHScoll(pWin,Flag,ItemLen,ItemPos,DispLen);
    else if(Flag & TWIDGET_EN_FRAME)//无水平滚动条的下边框
      _FullHFrame(pWin,ItemLen,ItemPos,DispLen);
    else if(Flag & TWIDGET_EN_NOTIFY)  //无滚动条无边框时,用户空间提示
      _FullHHint(pWin,ItemLen,ItemPos,DispLen);
  }
  //==============填充右测边框===========
  if(PaintFlag & TGUI_PAINT_VCONTEXT){
    ItemLen = pWidget->ItemsRect.h; //项高度
    ItemPos = pWidget->ItemsRect.y; //项Y位置
    DispLen = TWidget_GetDispH(pWin,Flag);//显示项高度  
    if(Flag & TWIDGET_EN_VSCOLL)//画垂直滚动条
      _FullVScoll(pWin,Flag,ItemLen,ItemPos,DispLen);
    else if(Flag & TWIDGET_EN_FRAME)//画右边框
      _FullVFrame(pWin,Flag,ItemLen,ItemPos,DispLen);
    else if(Flag & TWIDGET_EN_NOTIFY)  //无滚动条无边框时,用户空间提示
      _FullVHint(pWin,Flag,ItemLen,ItemPos,DispLen);
  }
}


