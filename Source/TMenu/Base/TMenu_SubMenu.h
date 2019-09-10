/*******************************************************************************

                  TMenu菜单之子菜单接口

*******************************************************************************/
#ifndef __T_MENU_SUB_MENU_H 
#define __T_MENU_SUB_MENU_H

#include "TMenuBase.h"

/**********************************************************************
                      配置区
**********************************************************************/
//定义默认单选按钮样式(见TWidget.h与单选控件):
#define   TM_SUB_MENU_STYLE_DEFAULT \
  (TWIDGET_EN_HEADER | TWIDGET_EN_VSCOLL | TWIDGET_EN_NOTIFY)

//注:可用的有:
//#define   TWIDGET_EN_FRAME    0x80  //该构件具有边框,当LC高度>-3行时此项有效
//#define   TWIDGET_EN_HEADER   0x40  //允许显示数据头
//#define   TWIDGET_EN_HSCOLL   0x20  //允许显示水平滚动条
//#define   TWIDGET_EN_VSCOLL   0x10  //允许显示垂滚动条
//#define   TWIDGET_EN_RETURN   0x08  //允许回环标志
//#define   TWIDGET_EN_NOTIFY   0x04  //无滚动条与边框时允许在用户空时显示提示信息
//#define   TLISTBOXEX_EN_RETURN  0x02      //允许带返回字

/**********************************************************************
                     用户使用相关
**********************************************************************/
//当为子菜单时,菜单结构作如下定义:
//typedef struct _TMenu{
//  TMTYPE_SUBMENU;           //菜单类型为子菜单模式
//  unsigned char Size;       //可供选择的子菜单项个数
//  LanguageCode_t *pgHeader; //菜单头,为NULL时从回调里读取
//  struct _TMenu *pParent;   //自已的父菜单,没有时为NULL
//  const TMenu_t*;           //存放自已的子菜单阵列连接头
//  TMenuNotify_t Notify;     //与用户空间交互的通报函数
//}TMenu_t;

//此模式无用户区标志

/**********************************************************************
                     用户区数据结构
**********************************************************************/
struct _SubMenuUser{
 TItemSize_t Sel;  //当前子菜单所在位置
 //用户返回标志,见此模块回调函数说明
 unsigned char UserFlag; 
};

//子菜单模式可以无回调函数
//当有回调函数时,表示该菜单内包含命令,将响应下列通报:
//类型为:TM_NOTIFY_SET_DATA时,表示在Sel项内按确定键了

//在回调函数内执行对应项命令后，还需在UserFlag内置返回状态:
//当返回1时,表示该项为命令,否则可以不选择

/**********************************************************************
                     内部数据结构
**********************************************************************/
struct _SubMenuData{
 TListboxEx_t  ListboxEx;   //结构缓存
 unsigned char Flag;            //内部标志
 struct _SubMenuUser User;     //用户数据
};

/**********************************************************************
                     相关函数
**********************************************************************/
//------------------------创建函数实现----------------------------
//1:创建该类型菜单,从用户区获得数据并更新显示
//返回创建成功:
signed char TMenu_SubMenuCreate(const TMenu_t *pMenu,    //指向的菜单系统
                                TWinHandle_t hWin,       //分配给菜单的窗口
                                //带入的自由数据,这里表示上次菜单位置
                                unsigned char Data,
                                void *pv);      //可供使用的自由缓冲区


//------------------------响应按键函数实现----------------------------
//2:响应按键操作并根据情况与用户进行数据交互,并更新显示
//返回值定义为: 正值:进入相应子菜单,-1,不管,-2:返回上层菜单
signed char TMenu_SubMenuKey(const TMenu_t *pMenu,    //指向的菜单系统
                             unsigned char Key,       //送入的键值
                             void *pv);      //已被使用的自由数据缓冲区

//-------------------------子菜单通报回调实现函数-----------------------------
//仅供内部通报使用
const void*TMenu_SubMenu_Notify(void *pvPource,
                                unsigned char Type,
                                const void *pvData);

/**********************************************************************
                      回调函数
**********************************************************************/

//----------------------得到当前显示样式-------------------------
#ifdef TMENU_STYLE_VARIABLE   //可变样式时在用户区定义
  unsigned char TMenu_SubMenu_cbGetCurStyle(void);
#else
  #define TMenu_SubMenu_cbGetCurStyle() \
    (TM_SUB_MENU_STYLE_DEFAULT | TLISTBOXEX_EN_RETURN)
#endif

#endif
