/* ----------------------------------------------------------------------------
 *                TGUI消息通报实现
 * --------------------------------------------------------------------------*/

#include"TGUINotify.h"

#ifdef TGUI_DIS_P_FUN //禁止使用函数指针时

#include "TGUI.h"
#include "string.h"

//包含的回调函数文件:
#include "TMenu_CheckBox.h"
#include "TMenu_MNumAdj.h"
#include "TMenu_RadioBox.h"
#include "TMenu_SubMenu.h"



//----------------TGUI消息传递不支持函数指针的替换函数实现------------------
const void* TGUIpFunReplace(void *pvRource,     //由谁发出的通报
                            unsigned char Type, //通报类型
                            //根据类型输入的数据通道
                            const void *pvData,
                            TGUINotify_t ID)   // 回调函数ID号
{
  //为防止常量不可移值性问题,直接使用case:
  switch(ID){
  //内部控件回调函数
  case TGUI_CBFUNID_LISTBOX_EX: return TListBox_Notify(pvRource,Type,pvData);
  #ifndef TGUI_DIS_TEXT
  case TGUI_CBFUNID_TEXT: return TText_Notify(pvRource,Type,pvData);
  #endif
  #ifndef TGUI_DIS_CHECKBOX
  case TGUI_CBFUNID_CHECKBOX: return TCheckBox_Notify(pvRource,Type,pvData);
  #endif
  #ifndef TGUI_DIS_RADIO_BUTTON
  case TGUI_CBFUNID_RADIO_BUTTON: return TRadioButtom_Notify(pvRource,Type,pvData);
  #endif
  //外部控件回调函数
  case TGUI_CBFUNID_TMENU_CHECKBOX: return TMenu_CheckBoxNotify(pvRource,Type,pvData);
  case TGUI_CBFUNID_TMENU_MNUM_LISTBOX: return TMenu_MNumAdjListBoxNotify(pvRource,Type,pvData);//多值调整的列表框
  case TGUI_CBFUNID_TMENU_MNUM_EDIT: return TMenu_MNumAdjEditNotify(pvRource,Type,pvData);//多值调整的编辑框
  case TGUI_CBFUNID_TMENU_RADIO_BUTTON: return TMenu_RadioBoxNotify(pvRource,Type,pvData);
  case TGUI_CBFUNID_TMENU_SUB_MENU: return TMenu_SubMenu_Notify(pvRource,Type,pvData);
  default: break;
  }
  return NULL;
}







#endif

