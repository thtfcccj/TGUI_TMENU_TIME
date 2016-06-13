/* ----------------------------------------------------------------------------
 *                TGUI窗口基本部件相关函数
 * --------------------------------------------------------------------------*/

#include "TListbox.h"
#include "string.h"

//-------------------------窗口画图函数-----------------------------
void ListBox_Paint(TListbox_t *pListbox,    //具体控件
                   //绘制标志,见按键处理函数返回定义,0xff表未绘制所有
                   unsigned char PaintFlag) 
{
  char *pBuf;
  const char *pString;
  TItemSize_t Items,Item;
  TItemSize_t StringStart,FullLen,CpyLen;
  TGUINotify_t cbGUINotify = pListbox->cbGUINotify;
  TItemSize_t  StringLen = 0;
  
  //1:若头改变,则调用TWidget_PaintHeader绘制头
  if(PaintFlag & TGUI_PAINT_HEADER){
    pString = TGUI_NOTIFY_RUN(cbGUINotify,pListbox,TGUI_NOTIFY_GET_CAPTION,&TListbox_GetSel(pListbox));
    TWidget_PaintHeader((TWidget_t *)pListbox,pString, 0);
  }
  //2:若内容(水平与垂直方向)发生变化,应调用TWidget_pGetPaintRow并在缓冲区绘制内容
  if(PaintFlag & (TGUI_PAINT_HCONTEXT | TGUI_PAINT_VCONTEXT | TGUI_PAINT_DATA)){
    Items = TListbox_GetItems(pListbox);
    for(Item = TWidget_GetPaintRowItemStart((TWidget_t *)pListbox); Item < Items; Item++){
      pBuf = TWidget_pGetPaintRow((TWidget_t *)pListbox,Item,StringLen);
      if(!pBuf) break;//结束了
      pString = TGUI_NOTIFY_RUN(cbGUINotify,pListbox,TGUI_NOTIFY_GET_ITEM,&Item);
      StringLen = strlen((const char*)pString);
      StringStart = TWidget_GetPaintRowStringStart((TWidget_t *)pListbox);
      FullLen = TWidget_GetPaintRowLen((TWidget_t *)pListbox);//假定全部填充
      if(StringLen >= StringStart){              //剪切需要的内容
        CpyLen = StringLen - StringStart;
        if(CpyLen > FullLen){CpyLen = FullLen; FullLen = 0;}//一屏显示不下
        else FullLen -=  CpyLen;    //填充一部分,后面补空格
        memcpy(pBuf, pString + StringStart,CpyLen); 
        pBuf += CpyLen;
      }
      //else //有效内容不能够显示了,全部填充
      //填充后面空格
      memset(pBuf,' ',FullLen);
    }
  }
  //3:若焦点发生变化,交上层处理
  if(PaintFlag & TGUI_PAINT_FOCUS){
    TGUI_NOTIFY_RUN(cbGUINotify,pListbox,TGUI_NOTIFY_GET_FOCUS,&TListbox_GetSel(pListbox));
  }
  //4:将内容水平与垂直方向变化的标志送入TWidget_PaintEnd,画图结束
  TWidget_PaintEnd((TWidget_t *)pListbox,PaintFlag,StringLen);
}

//-------------------------初始化函数-----------------------------
//注:考虑很多系统仅使用静态内层分配,故将创建函数留给更高一层,即
//可在此基础上立真正的创建函数
void TListbox_Init(TListbox_t *pListbox,  //由用户分配的存储空间
                   TWinHandle_t hWin,     //已建立并初始化好的窗口
                   unsigned char Flag,    //控件标志
                   TItemSize_t Items,         //预定义项目总数
                   TGUINotify_t cbGUINotify) //通报函数
{
  pListbox->cbGUINotify = cbGUINotify;
  TWidget_Init((TWidget_t*)pListbox,hWin,Flag);
  TListbox_SetItems(pListbox,Items);
}

//-------------------------按键处理函数-----------------------------
void TListbox_Key(TListbox_t *pListbox,unsigned char Key)
{
  unsigned char PaintFlag = 0;
  //先检查退出键,0键也当做退出键处理
  if((Key == TGUI_KEY_ESCAPE) || (Key == '0')){
    TGUI_NOTIFY_RUN(pListbox->cbGUINotify,pListbox,TGUI_NOTIFY_KEY_ESCAPE,&TListbox_GetSel(pListbox));
    return;
  }
  if(Key != TGUI_KEY_ENTER){
    PaintFlag = TWidget_Key((TWidget_t*)pListbox,Key);
    //if((Key >= '1') && (Key <= '9')) //数字键处理后,还需当做进入键处理
    //  Key = TGUI_KEY_ENTER; 
 }
 if(Key == TGUI_KEY_ENTER) 
    PaintFlag = *(unsigned char*)TGUI_NOTIFY_RUN(pListbox->cbGUINotify,pListbox,TGUI_NOTIFY_KEY_ENTER,
                                                &TListbox_GetSel(pListbox));

  //重绘相关
  ListBox_Paint(pListbox,PaintFlag);
}
