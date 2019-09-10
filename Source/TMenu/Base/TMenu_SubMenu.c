/*******************************************************************************

                  TMenu菜单之子菜单实现

*******************************************************************************/
#include "TMenu.h"
#include "TMenu_SubMenu.h"
#include <string.h>

//其中:标志定义为:
#define _FLAG_RETURN    0x01      //置此标志认为是退出上一层
#define _FLAG_ENTER     0x02      //置此标志认为是进入指定层

//通报函数应响应下列类型通报:
//TGUI_NOTIFY_GET_CAPTION 获得标题
//TGUI_NOTIFY_GET_ITEM 若得项显示内容
//TGUI_NOTIFY_KEY_ENTER 每一项的确认键,此时应检查选中与否并进行应用层处理
//若项超过设定希数,表示在附加的保存焦点上
//TGUI_NOTIFY_EKY_ESCAPE 退出到上一层
//-------------------------子菜单通报回调实现函数-----------------------------
//仅供内部通报使用
const void*TMenu_SubMenu_Notify(void *pvSource,
                                unsigned char Type,
                                const void *pvData)
{
  const TMenu_t *pMenu = pPopMenu();
  TItemSize_t Item = *(TItemSize_t*)pvData;   //当前项位置
  struct _SubMenuData *pSubMenuData = (struct _SubMenuData*)pPopData();
  
  pvSource = pvSource;//防止部分编译器警告 
  //得到系统当前指向的数据缓冲区
  switch(Type){
  case TGUI_NOTIFY_GET_CAPTION://获得标题
    return TM_GetHeader(pMenu);
  case TGUI_NOTIFY_GET_ITEM:// 若得项显示内容,从子菜单那得到
    return TM_GetSubMenuHeader(pMenu,Item);
  case TGUI_NOTIFY_KEY_ENTER://每一项的确认键
    pSubMenuData->User.Sel = Item;//记住当前位置
    pSubMenuData->Flag |= _FLAG_ENTER;//进入指定层,后继处理防止堆栈过多
    break;
  case TGUI_NOTIFY_KEY_ESCAPE://退出到上一层
    pSubMenuData->Flag |= _FLAG_RETURN;
  default: break;
  }
  return NULL;
}

//------------------------创建函数实现----------------------------
//1:创建该类型菜单,从用户区获得数据并更新显示
//返回创建成功:
signed char TMenu_SubMenuCreate(const TMenu_t *pMenu,    //指向的菜单系统
                                TWinHandle_t hWin,       //分配给菜单的窗口
                                //带入的自由数据,这里表示上次菜单位置
                                unsigned char Data,
                                void *pv)      //可供使用的自由缓冲区
{
  struct _SubMenuData *pSubMenuData = (struct _SubMenuData *)pv;
  TListboxEx_t *pListboxEx = &pSubMenuData->ListboxEx;
  //得到宽度
  TItemSize_t ItemW = TM_GetItemMaxLen(pMenu);
  
  //使用扩展列表框实现:
  TListboxEx_Init(pListboxEx, //由用户分配的存储空间
                  hWin,//已建立并初始化好的窗口
                  TM_GetSize(pMenu),//预定义项目总数
                  ItemW,
                  //控件标志,带返回字
                  TMenu_SubMenu_cbGetCurStyle(),
                  //显示样式
                  TLISTBOXEX_ALIGN_LEN | TLISTBOXEX_ANTI_WORD | 
                    TLISTBOXEX_EN_NO | TLISTBOXEX_EN_APPEND,
                  TGUI_NOTIFY_PASS(TMenu_SubMenu_Notify,TGUI_CBFUNID_TMENU_SUB_MENU)); //通报函数
  
  TMenu_ReBulidWin(pListboxEx,//重置窗口大小
                   ItemW,
                   TM_GetSize(pMenu),
                   strlen(TM_GetHeader(pMenu)));
  TListboxEx_SetSel(pListboxEx,Data);//当前菜单项
  pSubMenuData->User.Sel = Data;
  
  //初始化其它数据
  pSubMenuData->Flag = 0;
  
  //支持RPC时，可能默认位置即需要更新RPC数据,故先通报
  #ifdef TM_EN_RPC
    TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_CUSOR_CHANGED, &pSubMenuData->User.Sel);
  #endif
  
  //更新显示
  TListBoxEx_PaintAll(&pSubMenuData->ListboxEx);
  TListBoxEx_PaintAll(&pSubMenuData->ListboxEx);//长度对齐重画两次
  return -1;
}

//------------------------响应按键函数实现----------------------------
//2:响应按键操作并根据情况与用户进行数据交互,并更新显示
//返回值定义为: 正值:进入相应子菜单,-1,不管,-2:返回上层菜单
signed char TMenu_SubMenuKey(const TMenu_t *pMenu,    //指向的菜单系统
                             unsigned char Key,       //送入的键值
                             void *pv)      //已被使用的自由数据缓冲区
{
  unsigned char Flag;
  unsigned char Sel;
  struct _SubMenuData *pSubMenuData = (struct _SubMenuData *)pv;
  pMenu = pMenu;//防止部分编译器警告 
  PushMenu(pMenu);
  PushData(pv);
  #ifdef TM_EN_RPC //记住上次光标位置
    Sel = TListboxEx_GetSel(&pSubMenuData->ListboxEx); //上次位置 
  #endif
  TListboxEx_Key(&pSubMenuData->ListboxEx,Key);//处理按键

  Flag = pSubMenuData->Flag;

  //不支持RPC时，数字键后处理:当做快捷键处理
  #ifndef TM_EN_RPC
  if((Key >= '1') && (Key <= '9')){
    //不处理超过项目的按键
    if((Key - '1') >= TListboxEx_GetItems(&pSubMenuData->ListboxEx)) return -1;
    Flag |= _FLAG_ENTER;
  }
  #endif
  pSubMenuData->User.Sel = TListboxEx_GetSel(&pSubMenuData->ListboxEx);//更新

  //进入指定层菜单后继处理
  if(Flag & _FLAG_ENTER){ //检查进入菜单
    pSubMenuData->Flag &= ~_FLAG_ENTER;

    Sel = pSubMenuData->User.Sel;
    pSubMenuData->User.UserFlag = 0;    //初始化
    if(pMenu->cbNotify){ //有回调函数时执行回调
      TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_SET_DATA,&pSubMenuData->User);
      pSubMenuData->User.Sel = Sel;//防止用户误选
      if(pSubMenuData->User.UserFlag == 1)
        return -1;//返回1时表示为命令,执行后不进入菜单
    }
    return (signed char)Sel;
  }

  //检查返回上层菜单
  if(Flag & _FLAG_RETURN)
    return -2;
  
  //没有进入或退出，在支持RPC时，检查选择项，若改变则通报改变以让用户层作好准备
  #ifdef TM_EN_RPC
    if((Sel != pSubMenuData->User.Sel) && (pSubMenuData->User.Sel < TM_GetSize(pMenu)))
      TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_CUSOR_CHANGED, &pSubMenuData->User.Sel);
  #endif
  
  return -1;
}




