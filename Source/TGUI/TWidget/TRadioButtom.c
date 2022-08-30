/*******************************************************************************

                           TWidget之单选框控件实现

*******************************************************************************/

#include "TRadioButtom.h"
#include "TWidgetShare.h"
#include <string.h>

/**********************************************************************
                          行为函数
**********************************************************************/

//-------------------------单选框回调实现函数-----------------------------
//仅供内部通报使用
const void*TRadioButtom_Notify(void *pv,
                               unsigned char Type,
                               const void *pvData)
{
  //因此控件以Listbox为基类的,故可以直接转换为扩展类型
  TRadioButtom_t *pRadioButtom = (TRadioButtom_t*)pv;
  TGUINotify_t cbGUINotify = pRadioButtom->cbGUINotify;//用户空间通报
  TItemSize_t Item = *(TItemSize_t*)pvData;   //当前项位置
  char *pConstBuf;
  unsigned char PaintFlag;
  
  switch(Type){
  case TGUI_NOTIFY_KEY_ENTER: //确认键时检查焦点位置
  if(pRadioButtom->Radio != Item){//当前项被选中了
    //不在附加项里时,当附加项里有确认键时可到此
    if((Item < TListboxEx_GetItems((TListboxEx_t*)pRadioButtom)) || 
      !TListboxEx_HaveAppend((TListboxEx_t*)pRadioButtom)){
	      pRadioButtom->Radio = Item;
    }
    //交给用户空间先处理
    TGUI_NOTIFY_RUN(cbGUINotify,pRadioButtom,Type,pvData);
    PaintFlag = TGUI_PAINT_DATA;
  }
  else PaintFlag = 0; //数据无更新
  pConstBuf = TGUI_pGetConstBuf();
  *pConstBuf = (char)PaintFlag;
  return pConstBuf;
  //return  &PaintFlag;  //返回更新标志
  //case TGUI_NOTIFY_GET_FOCUS://在本层处理焦点,仅处理附加项焦点  
  //  return NULL; //直接返回
  case TLISTBOXEX_NOTIFY_PREFIX://前缀处理
    //填充"●"或"○";
    pConstBuf = TGUI_pGetConstBuf();//常量区指针
    *pConstBuf++ = 0xa1;
    if(pRadioButtom->Radio == Item)//被选中了
      *pConstBuf++ = 0xf1;  
    else *pConstBuf++ = 0xf0;  //未被选中
    *pConstBuf = '\0';
    return TGUI_pGetConstBuf();
  //case TGUI_NOTIFY_GET_ITEM://仅处理附加项
  //  break;
  //搞不定的,直接交给用户吧
  default: break;
  }
  return TGUI_NOTIFY_RUN(cbGUINotify,pRadioButtom,Type,pvData);
}

//-------------------------初始化函数-----------------------------
//初始化后预置第一项为选择状态,重置选择状态后应再重画窗口
//注:考虑很多系统仅使用静态内层分配,故将创建函数留给更高一层,即
//可在此基础上立真正的创建函数
void TRadioButtom_Init(TRadioButtom_t *pRadioButtom,  //由用户分配的存储空间
                     TWinHandle_t hWin,         //已建立并初始化好的窗口
                     TItemSize_t Items,         //预定义项目总数
                     TItemSize_t ItemW,         //预定义项宽度(即项的最宽度)
                     unsigned char Flag,        //控件标志
                     TGUINotify_t cbGUINotify) //通报函数
{
  //置样式适合于单选框
  unsigned char Style = TLISTBOXEX_ALIGN_LEN | TLISTBOXEX_ANTI_WORD2 | 
    TLISTBOXEX_PREFIX | TLISTBOXEX_EN_NO;
  
  //先初始化扩展,以便回调函数可以正常工作
  pRadioButtom->cbGUINotify = cbGUINotify;
   pRadioButtom->Radio = 0;

  //当预定义宽度比用户显示窗口宽度(即一页)小时,尽量局中对齐
  ItemW += TGetAlignLenR(TGetItemsPlace(Items),1)+ 2;//序号与单选框占位

  TListboxEx_Init((TListboxEx_t*)pRadioButtom,hWin,Items,ItemW,Flag,//基类初始化
                  Style,TGUI_NOTIFY_PASS(TRadioButtom_Notify,TGUI_CBFUNID_RADIO_BUTTON));
}


