/*******************************************************************************

                  TMenu菜单之字符串编辑接口(支持输入法时专用)

*******************************************************************************/
#include "TMenu.h"
#include "TMenu_EditString.h"
#include <string.h>

//------------------------------创建函数实现-------------------------------
//1:创建该类型菜单,从用户区获得数据并更新显示
//返回创建成功:
signed char TMenu_EditStringCreate(const TMenu_t *pMenu,    //指向的菜单系统
                                 TWinHandle_t hWin,       //分配给菜单的窗口
                                 unsigned char Data,    //带入的自由数据,这里无意义
                                 void *pv)      //可供使用的自由缓冲区
{
  struct _EditStringData *pEditStringData = (struct _EditStringData *)pv;
  //从用户空间获得并设置当前所选项
  pEditStringData->User.DefaultCursor = (TIme_String_t)-1;//默认在最后
  
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_DATA,&pEditStringData->User);
  pEditStringData->pImeMng = TMenu_EditString_cbpGetImeMng();
  
  if(TImeMng_Init(pEditStringData->pImeMng,  //带入的输入法结构缓冲
                  TWM_pGetWin(hWin),             //当前挂接的窗口
                  pEditStringData->User.pString, //用户已存在的字符串,必须以'\0'结尾
                  pEditStringData->User.Size,     //字符串缓冲区允许大小
                  pEditStringData->User.DefaultType,   //默认输入法,<4
                  pEditStringData->User.TypeMask,//可使用的输入法类型 
                  (const char*)(pMenu->pv), //挂接的符号表,为空时使用默认
                  pEditStringData->User.DefaultCursor))//默认光标位置
    return 0;//创建不成功!
  return -1;
}

//------------------------响应按键函数实现----------------------------
//2:响应按键操作并根据情况与用户进行数据交互,并更新显示
//返回值定义为: 正值:进入相应子菜单,-1,不管,-2:返回上层菜单
signed char TMenu_EditStringKey(const TMenu_t *pMenu,    //指向的菜单系统
                              unsigned char Key,       //送入的键值
                              void *pv)      //已被使用的自由数据缓冲区
{
  struct _EditStringData *pEditStringData = (struct _EditStringData *)pv;
  //PushMenu(pMenu);
  //PushData(pv);
  switch(TImeMng_Key(pEditStringData->pImeMng,Key)){
  //case 0: return -1;//正常状态
  case -1://退出键直接退出      
    TMENU_NOTIFY_RUN(pMenu->cbNotify,
                     TM_NOTIFY_CHANGED,
                     &pEditStringData->User);
    break;
  case -2://确认键退出      
    TMENU_NOTIFY_RUN(pMenu->cbNotify,
                     TM_NOTIFY_SET_DATA,
                     &pEditStringData->User);
    break;
  default:return -1;//正常状态
  }
  return -2;
}

