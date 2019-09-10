/*******************************************************************************

                  TMenu菜单之单选框接口

*******************************************************************************/
#ifndef __T_MENU_RADIO_BOX_H 
#define __T_MENU_RADIO_BOX_H

#include "TMenuBase.h"
/**********************************************************************
                      配置区
**********************************************************************/
//定义单选按钮默认样式(见TWidget.h与单选控件):
#define   TM_RADIO_BOX_STYLE_DEFAULT    \
  (TWIDGET_EN_HEADER | TWIDGET_EN_VSCOLL | TWIDGET_EN_NOTIFY)

//注:可用的有:
//#define   TWIDGET_EN_FRAME    0x80  //该构件具有边框,当LC高度>-3行时此项有效
//#define   TWIDGET_EN_HEADER   0x40  //允许显示数据头
//#define   TWIDGET_EN_HSCOLL   0x20  //允许显示水平滚动条
//#define   TWIDGET_EN_VSCOLL   0x10  //允许显示垂滚动条
//#define   TWIDGET_EN_NOTIFY   0x04  //无滚动条与边框时允许在用户空时显示提示信息
//#define   TRADIO_BUTTOM_EN_RETURN  0x02      //允许带返回字

/**********************************************************************
                     用户使用相关
**********************************************************************/
//当为单选框时,菜单结构作如下定义:
//typedef struct _TMenu{
//  TMTYPE_RADIOBOX;        //菜单类型为单选框模式
//  unsigned char Size;     //可供选择的调整项个数
//  LanguageCode_t *pgHeader; //菜单头,为NULL时从回调里读取
//  struct _TMenu *pParent;   //自已的父菜单
//  const LanguageCode_t*;   //存放每一个调整项的名称(结构)阵列
//  TMenuNotify_t Notify;     //与用户空间交互的通报函数
//}TMenu_t;

//复选框模式用户区标志为
#define   TM_RADIOBOX_EN_SAVE   0x80  //有保存按钮标志


/**********************************************************************
                     用户区数据结构
**********************************************************************/
struct _RadioBoxUser{
 TItemSize_t Sel;               //选择项
};
//用户回调函数响应类型及需实现的功能有:
//1:类型为:TM_NOTIFY_GET_DATA时,需将当前所选装入Sel中
//2:类型为:TM_NOTIFY_CHANGED时,表示Sel项被临时改变了,
//用户可根据应用要求决定是否响应(如:保存或直接更改应用)
//3:类型为:TM_NOTIFY_SET_DATA时,表示已修改完成,用户应进行保存等操作

/**********************************************************************
                     内部数据结构
**********************************************************************/
struct _RadioBoxData{
 TRadioButtom_t  RadioButtom;   //结构缓存
 unsigned char Flag;            //内部标志
 struct _RadioBoxUser User;     //用户数据
};

/**********************************************************************
                     相关函数
**********************************************************************/
//------------------------创建函数实现----------------------------
//1:创建该类型菜单,从用户区获得数据并更新显示
//返回创建成功:
signed char TMenu_RadioBoxCreate(const TMenu_t *pMenu,    //指向的菜单系统
                                 TWinHandle_t hWin,       //分配给菜单的窗口
                                 unsigned char Data,    //带入的自由数据,这里无意义
                                 void *pv);      //可供使用的自由缓冲区

//------------------------响应按键函数实现----------------------------
//2:响应按键操作并根据情况与用户进行数据交互,并更新显示
//返回值定义为: 正值:进入相应子菜单,-1,不管,-2:返回上层菜单
signed char TMenu_RadioBoxKey(const TMenu_t *pMenu,    //指向的菜单系统
                              unsigned char Key,       //送入的键值
                              void *pv);      //已被使用的自由数据缓冲区

//-------------------------单选框通报回调实现函数-----------------------------
//仅供内部通报使用
const void*TMenu_RadioBoxNotify(void *pvPource,
                                unsigned char Type,
                                const void *pvData);

/**********************************************************************
                      回调函数
**********************************************************************/

//----------------------得到当前显示样式-------------------------
#ifdef TMENU_STYLE_VARIABLE   //可变样式时在用户区定义
  unsigned char TMenu_RadioBox_cbGetCurStyle(void);
#else
  #define TMenu_RadioBox_cbGetCurStyle() \
    (TM_RADIO_BOX_STYLE_DEFAULT | TRADIO_BUTTOM_EN_RETURN)
#endif


#endif
