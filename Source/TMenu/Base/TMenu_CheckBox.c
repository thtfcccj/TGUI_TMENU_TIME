/*******************************************************************************

                  TMenu菜单之复选框控件实现

*******************************************************************************/
#include "TMenu.h"
#include "TMenu_Checkbox.h"
#include <string.h>

//其中:标志定义为:
#define _FLAG_RETURN    0x01      //置此标志认为是退出上一层
#define _FLAG_ENTER     0x02      //确认键

//通报函数应响应下列类型通报:
//TGUI_NOTIFY_GET_CAPTION 获得标题
//TGUI_NOTIFY_GET_ITEM 若得项显示内容
//TGUI_NOTIFY_KEY_ENTER 每一项的确认键,此时应检查选中与否并进行应用层处理
//若项超过设定希数,表示在附加的保存焦点上
//TGUI_NOTIFY_EKY_ESCAPE 退出到上一层

//-------------------------复选框通报回调实现函数-----------------------------
//仅供内部通报使用
const void*TMenu_CheckBoxNotify(void *pv,
                                unsigned char Type,
                                const void *pvData)
{
  const TMenu_t *pMenu = pPopMenu();
  TItemSize_t Item = *(TItemSize_t*)pvData;   //当前项位置
  struct _CheckboxData *pCheckboxData = (struct _CheckboxData*)pPopData();
  
  pv = pv;//防止部分编译器警告
  //得到系统当前指向的数据缓冲区
  switch(Type){
  case TGUI_NOTIFY_GET_CAPTION://获得标题
    return TM_GetHeader(pMenu);
  case TGUI_NOTIFY_GET_ITEM:// 若得项显示内容
    return TM_GetItemString(pMenu,Item);
  case TGUI_NOTIFY_KEY_ENTER://每一项的确认键
    pCheckboxData->User.ChanegedItem = Item;
    pCheckboxData->Flag |= _FLAG_ENTER;//退出后处理
    break;
  case TGUI_NOTIFY_KEY_ESCAPE://退出到上一层
    pCheckboxData->Flag |= _FLAG_RETURN;
  default: break;
  }
  return NULL;
}

//------------------------创建函数实现----------------------------
//1:创建该类型菜单,从用户区获得数据并更新显示
//返回创建成功:
signed char TMenu_CheckBoxCreate(const TMenu_t *pMenu,    //指向的菜单系统
                                  TWinHandle_t hWin,       //分配给菜单的窗口
                                  unsigned char Data,    //带入的自由数据,这里无意义
                                  void *pv)      //可供使用的自由缓冲区
{
  unsigned char Flag;
  struct _CheckboxData *pCheckboxData = (struct _CheckboxData *)pv;
  TCheckbox_t  *pCheckbox = &pCheckboxData->Checkbox;
  //得到宽度
  TItemSize_t ItemW = TM_GetItemMaxLen(pMenu);

  pMenu = pMenu;
  
  if(TM_GetType(pMenu) & TM_CHECKBOX_EN_SAVE)//有保存键时
     Flag =  TMenu_CheckBox_cbGetCurStyle() | TCHECKBOX_EN_SAVE;
  else Flag =  TMenu_CheckBox_cbGetCurStyle();

  Data = TM_GetSize(pMenu);//临时表示大小
  //使用复选框实现:
  TCheckbox_Init(pCheckbox,
                    hWin,//已建立并初始化好的窗口
                    Data,//预定义项目总数
                    ItemW,         //预定义项宽度(即项的最宽度)
                    Flag,//控件标志,带返回字
                    TGUI_NOTIFY_PASS(TMenu_CheckBoxNotify,TGUI_CBFUNID_TMENU_CHECKBOX)); //通报函数
  
  //从用户空间获得并设置当前所选项
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_DATA,&pCheckboxData->User);
  
  TCheckbox_SetAllCheck(pCheckbox,pCheckboxData->User.SelMask,Data);
                        
  //初始化其它数据
  pCheckboxData->Flag = 0;
  
  //更新显示
  TMenu_ReBulidWin((TListboxEx_t *)pCheckbox,//重置窗口大小
                   ItemW,
                   Data,
                   strlen(TM_GetHeader(pMenu)));
  TCheckbox_PaintAll(&pCheckboxData->Checkbox);
  TCheckbox_PaintAll(&pCheckboxData->Checkbox);//长度对齐重画两次  
  return -1;
}

//------------------------响应按键函数实现----------------------------
//2:响应按键操作并根据情况与用户进行数据交互,并更新显示
//返回值定义为: 正值:进入相应子菜单,-1,不管,-2:返回上层菜单 
signed char TMenu_CheckBoxKey(const TMenu_t *pMenu,    //指向的菜单系统
                              unsigned char Key,       //送入的键值
                              void *pv)      //已被使用的自由数据缓冲区
{
  unsigned char Data;
  struct _CheckboxData *pCheckboxData = (struct _CheckboxData *)pv;
  PushMenu(pMenu);
  PushData(pv);
  TCheckbox_Key(&pCheckboxData->Checkbox,Key);

  //确认键后继处理
  if(pCheckboxData->Flag & _FLAG_ENTER){
    Data = TM_GetSize(pMenu);//复用
    //更新数据
    TCheckbox_GetAllCheck(&pCheckboxData->Checkbox,
                            pCheckboxData->User.SelMask,Data);
    if(pCheckboxData->User.ChanegedItem < Data) 
      Data = TM_NOTIFY_CHANGED;
    else{
      Data = TM_NOTIFY_SET_DATA;
      TListboxEx_Key((TListboxEx_t*)&pCheckboxData->Checkbox,TGUI_KEY_DOWN); //到退出位置
    }
    //通知用户区
    TMENU_NOTIFY_RUN(pMenu->cbNotify,Data,&pCheckboxData->User);
    
    pCheckboxData->Flag &= ~_FLAG_ENTER;
  }
  //检查退出
  else if(pCheckboxData->Flag & _FLAG_RETURN)
    return -2;

  return -1;
}

