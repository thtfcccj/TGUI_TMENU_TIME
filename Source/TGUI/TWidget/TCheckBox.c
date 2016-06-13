/* ----------------------------------------------------------------------------
 *                复选框控件实现
 * --------------------------------------------------------------------------*/

#include "TCheckbox.h"
#include "string.h"
#include "math.h"

/**********************************************************************
                          成员函数
**********************************************************************/

//------------------------检查某项是否选中----------------------------
//返回0表示未选中,否则表示选中
unsigned char TCheckbox_IsChecked(TCheckbox_t *pCheckbox,TItemSize_t Item)
{
  return pCheckbox->SelMask[Item >> 3] & U8ShiftLUT[Item & 0x07];
}
                
//------------------------置某项为选中状态----------------------------
//返回0表示未选中,否则表示选中
void TCheckbox_SetChecked(TCheckbox_t *pCheckbox,TItemSize_t Item)
{
   pCheckbox->SelMask[Item >> 3] |= U8ShiftLUT[Item & 0x07];
}
                
//------------------------清除某项选中状态----------------------------
//返回0表示未选中,否则表示选中
void TCheckbox_ClrChecked(TCheckbox_t *pCheckbox,TItemSize_t Item)
{
   pCheckbox->SelMask[Item >> 3] &= ~U8ShiftLUT[Item & 0x07];
}

//------------------------设置所有状态----------------------------
//每一位表示一个状态,0表示未选中,否则表示选中
void TCheckbox_SetAllCheck(TCheckbox_t *pCheckbox,
                           unsigned char *pCheck,
                           TItemSize_t Items)
{
  unsigned char Count = (Items + 7) >> 3;
  memcpy(pCheckbox->SelMask,pCheck,Count);
}

//--------------------------得到所有状态--------------------------------
void TCheckbox_GetAllCheck(TCheckbox_t *pCheckbox,
                           unsigned char *pCheck,
                           TItemSize_t Items)
{
  unsigned char Count = (Items + 7) >> 3;
  memcpy(pCheck,pCheckbox->SelMask,Count);
}


/**********************************************************************
                          行为函数
**********************************************************************/


//-------------------------复选框回调实现函数-----------------------------
//仅供内部通报使用
const void* TCheckBox_Notify(void *pvSource,
                             unsigned char Type,
                          const void *pvData)
{
  //因此控件以Listbox为基类的,故可以直接转换为扩展类型
  TCheckbox_t *pCheckbox = (TCheckbox_t*)pvSource;
  TItemSize_t Item = *(TItemSize_t*)pvData;   //当前项位置
  unsigned char *pCurItem;
  unsigned char Mask;
  char *pConstBuf;
  
  switch(Type){
  case TGUI_NOTIFY_KEY_ENTER: //确认键时反选所选项目
    //不在附加项里时,当附加项里有确认键时可到此
    if((Item < (TListboxEx_GetItems((TListboxEx_t*)pCheckbox)) - 1) || 
      !TListboxEx_HaveAppend((TListboxEx_t*)pCheckbox)){
      pCurItem = &(pCheckbox->SelMask[Item >> 3]);
      Mask = U8ShiftLUT[Item & 0x07];
      if(*pCurItem & Mask) *pCurItem &= ~Mask;
      else *pCurItem |= Mask;
    }
    //交给用户空间先处理
    TGUI_NOTIFY_RUN(pCheckbox->cbGUINotify,pvSource,Type,pvData);
    Mask = TGUI_PAINT_DATA;
    
    pConstBuf = TGUI_pGetConstBuf();
    *pConstBuf = (char)Mask;
    return pConstBuf;
    //return  &Mask;  //返回更新标志
  //case TGUI_NOTIFY_GET_FOCUS://上层已处理完全部焦点   
  //  return NULL; //直接返回
  case TLISTBOXEX_NOTIFY_PREFIX://前缀处理
    //填充"■"或"□";
    pConstBuf = TGUI_pGetConstBuf();//常量区指针
    *pConstBuf++ = 0xa1;
    if(pCheckbox->SelMask[Item >> 3] & U8ShiftLUT[Item & 0x07])
      *pConstBuf++ = 0xf6;  //被选中了
    else *pConstBuf++ = 0xf5;  //未被选中
    *pConstBuf = '\0';
    return TGUI_pGetConstBuf();
  //case TGUI_NOTIFY_GET_ITEM://上层已处理完全部焦点
  //  break;
  //搞不定的,直接交给用户吧
  default: break;
  }
  return TGUI_NOTIFY_RUN(pCheckbox->cbGUINotify,pCheckbox,Type,pvData);
}

//-------------------------初始化函数-----------------------------
//初始化后所有项均被置为未选中状态,应选置选中状态再重画窗口
//注:考虑很多系统仅使用静态内层分配,故将创建函数留给更高一层,即
//可在此基础上立真正的创建函数
void TCheckbox_Init(TCheckbox_t *pCheckbox,  //由用户分配的存储空间
                     TWinHandle_t hWin,         //已建立并初始化好的窗口
                     TItemSize_t Items,         //预定义项目总数
                     TItemSize_t ItemW,         //预定义项宽度(即项的最宽度)
                     unsigned char Flag,        //控件标志
                     TGUINotify_t cbGUINotify) //通报函数
{
  //置样式适合于复选框
  unsigned char Style = TLISTBOXEX_ALIGN_LEN | TLISTBOXEX_ANTI_WORD2 | 
    TLISTBOXEX_PREFIX | TLISTBOXEX_EN_NO;
  
  //先初始化扩展,以便回调函数可以正常工作
  pCheckbox->cbGUINotify = cbGUINotify;
  memset(pCheckbox->SelMask, 0, sizeof(pCheckbox->SelMask));//初始化为均不选中  

  //当预定义宽度比用户显示窗口宽度(即一页)小时,尽量局中对齐
  //PageSize = TWidget_GetHPageSize((TWidget_t*)pCheckbox);
  ItemW += GetAlignLenR(GetItemsPlace(Items),1)+ 2;//序号与复选框占位

  TListboxEx_Init((TListboxEx_t*)pCheckbox,hWin,Items,ItemW,Flag,//基类初始化
                  Style,TGUI_NOTIFY_PASS(TCheckBox_Notify,TGUI_CBFUNID_CHECKBOX));
}


