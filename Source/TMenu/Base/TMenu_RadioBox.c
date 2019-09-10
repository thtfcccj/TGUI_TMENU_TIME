/*******************************************************************************

                  TMenu菜单之单选框实现

*******************************************************************************/
#include "TMenu.h"
#include "TMenu_RadioBox.h"
#include <string.h>

//其中:标志定义为:
#define _FLAG_RETURN     0x01      //置此标志认为是退出上一层
#define _FLAG_ENTER      0x02      //确认键
#define _FLAG_LAST_ENTER 0x04      //最后一行按确认键,否则为其它行

//通报函数应响应下列类型通报:
//TGUI_NOTIFY_GET_CAPTION 获得标题
//TGUI_NOTIFY_GET_ITEM 若得项显示内容
//TGUI_NOTIFY_KEY_ENTER 每一项的确认键,此时应检查选中与否并进行应用层处理
//若项超过设定希数,表示在附加的保存焦点上
//TGUI_NOTIFY_EKY_ESCAPE 退出到上一层
//-------------------------单选框通报回调实现函数-----------------------------
//仅供内部通报使用
const void*TMenu_RadioBoxNotify(void *pvSource,
                                unsigned char Type,
                                const void *pvData)
{
  const TMenu_t *pMenu = pPopMenu();
  TItemSize_t Item = *(TItemSize_t*)pvData;   //当前项位置
  struct _RadioBoxData *pRadioBoxData = (struct _RadioBoxData*)pPopData();
  
  pvSource = pvSource;//防止部分编译器警告
  //得到系统当前指向的数据缓冲区
  switch(Type){
  case TGUI_NOTIFY_GET_CAPTION://获得标题
    return TM_GetHeader(pMenu);
  case TGUI_NOTIFY_GET_ITEM:// 若得项显示内容
    return TM_GetItemString(pMenu,Item);
  case TGUI_NOTIFY_KEY_ENTER://每一项的确认键
    if(Item < TM_GetSize(pMenu)){
      pRadioBoxData->User.Sel = Item;
      TRadioButtom_SetRadio(&pRadioBoxData->RadioButtom, Item);
      pRadioBoxData->Flag |= _FLAG_ENTER; 
    }
    else //保存行按的确认键
      pRadioBoxData->Flag |= _FLAG_LAST_ENTER |_FLAG_ENTER; 
    break;
  case TGUI_NOTIFY_KEY_ESCAPE://退出到上一层
    pRadioBoxData->Flag |= _FLAG_RETURN;
  default: break;
  }
  return NULL;
}

//------------------------创建函数实现----------------------------
//1:创建该类型菜单,从用户区获得数据并更新显示
//返回创建成功:
signed char TMenu_RadioBoxCreate(const TMenu_t *pMenu,    //指向的菜单系统
                                 TWinHandle_t hWin,       //分配给菜单的窗口
                                 unsigned char Data,    //带入的自由数据,这里无意义
                                 void *pv)      //可供使用的自由缓冲区
{
  struct _RadioBoxData *pRadioBoxData = (struct _RadioBoxData *)pv;
  TRadioButtom_t  *pRadioButtom = &pRadioBoxData->RadioButtom;
  //得到宽度
  TItemSize_t ItemW = TM_GetItemMaxLen(pMenu);
  
  if(TM_GetType(pMenu) & TM_RADIOBOX_EN_SAVE)//有保存键时
     Data =  TMenu_RadioBox_cbGetCurStyle() | TRADIO_BUTTOM_EN_SAVE;
  else Data =  TMenu_RadioBox_cbGetCurStyle();
  //使用单选框实现:
  TRadioButtom_Init(pRadioButtom,
                    hWin,//已建立并初始化好的窗口
                    TM_GetSize(pMenu),//预定义项目总数
                    ItemW,         //预定义项宽度(即项的最宽度)
                    Data,         //控件标志
                    TGUI_NOTIFY_PASS(TMenu_RadioBoxNotify,TGUI_CBFUNID_TMENU_RADIO_BUTTON));//通报函数
  
  //从用户空间获得并设置当前所选项
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_DATA,&pRadioBoxData->User);
  
  TListboxEx_SetSel(&pRadioButtom->ListboxEx,pRadioBoxData->User.Sel);//当前项
  TRadioButtom_SetRadio(pRadioButtom,TListboxEx_GetSel(&pRadioButtom->ListboxEx));  
  
  //初始化其它数据
  pRadioBoxData->Flag = 0;
  
  //更新显示
  TMenu_ReBulidWin((TListboxEx_t *)pRadioButtom,//重置窗口大小
                   ItemW,
                   TM_GetSize(pMenu),
                   strlen(TM_GetHeader(pMenu)));
  TRadioButtom_PaintAll(&pRadioBoxData->RadioButtom);
  TRadioButtom_PaintAll(&pRadioBoxData->RadioButtom);//长度对齐重画两次
  return -1;
}

//------------------------响应按键函数实现----------------------------
//2:响应按键操作并根据情况与用户进行数据交互,并更新显示
//返回值定义为: 正值:进入相应子菜单,-1,不管,-2:返回上层菜单
signed char TMenu_RadioBoxKey(const TMenu_t *pMenu,    //指向的菜单系统
                              unsigned char Key,       //送入的键值
                              void *pv)      //已被使用的自由数据缓冲区
{
  unsigned char Data;
  struct _RadioBoxData *pRadioBoxData = (struct _RadioBoxData *)pv;
  PushMenu(pMenu);
  PushData(pv);
  TRadioButtom_Key(&pRadioBoxData->RadioButtom,Key);
  
  if(pRadioBoxData->Flag & _FLAG_ENTER){
    if(!(pRadioBoxData->Flag & _FLAG_LAST_ENTER)) {
      Data = TM_NOTIFY_CHANGED; //若项被改变了
    }
    else{//保存行按确认键了
      Data = TM_NOTIFY_SET_DATA; //按保存键了
      TListboxEx_Key((TListboxEx_t*)&pRadioBoxData->RadioButtom,
                     TGUI_KEY_DOWN); //到退出位置
    }
    //通知用户区
    TMENU_NOTIFY_RUN(pMenu->cbNotify,Data,&pRadioBoxData->User);
    pRadioBoxData->Flag &= ~(_FLAG_ENTER | _FLAG_LAST_ENTER);
  }
  //检查退出
  else if(pRadioBoxData->Flag & _FLAG_RETURN)
    return -2;
  return -1;
}

