/*******************************************************************************

                           TWidget之列表框控件

*******************************************************************************/
#ifndef __T_LISTBOX_H 
#define __T_LISTBOX_H

/*******************************************************************************
                                控件说明
*******************************************************************************/
//如下所示为列表框控件演示:
//  ┌──(可选显示头)───┐
//  │  　　  (第一项)      ┼
//  │ 　　   (第二项)      │
//  │ 　　   (第二项)    　↓
//  └←━━━━━━━━━→┘

//此控件在TWidget基础上构建,主要实现了TWidget的用户内容区(ItemsRect)(见TWindgetNote.h)
//的填充,和按键的响应,将在此层面实现的回调以于上层程序交互,可以独立工作

//继承关系:
//TListBox继承关系为:
//  TListBox->TWidget->(间接继承)TWin
//直接继承时可直接将该结构转换为基类结构后并调用基类相关函数
//间接继承通过该结构提供的间接转函数数转换为基类结构并可调用基类结构函数
//
//因控件已包装好,且基类无特殊说明外(注释带★标志),
//基类直接操作不当可能会影响系统工作,故不建议直接使用基类操作

/**********************************************************************
                             相关结构
**********************************************************************/
#include "TWidget.h"

typedef struct{
  TWidget_t Widget;   //基类
  TGUINotify_t cbGUINotify;    //通报函数
}TListbox_t;

//通报函数应响应下列类型通报:
//TGUI_NOTIFY_GET_CAPTION,TGUI_NOTIFY_GET_ITEM,TGUI_NOTIFY_GET_FOCUS
//TGUI_NOTIFY_KEY_ENTER,TGUI_NOTIFY_EKY_ESCAPE;

/**********************************************************************
                          属性函数
**********************************************************************/
//★得到当前所选项
#define TListbox_GetSel(pListbox) TWidget_GetSel((TWidget_t*)(pListbox))
//设置当前所选项,仅在创建后使用
#define TListbox_SetSel(pListbox,Sel) \
  TWidget_SetSel(((TWidget_t*)(pListbox)),Sel)
//★得到项总数
#define TListbox_GetItems(pListbox) TWidget_GetItems((TWidget_t*)(pListbox))
//重新设置项总数
#define TListbox_SetItems(pListbox,Items)\
  TWidget_SetItems(((TWidget_t*)(pListbox)),Items)
//★得到项宽度
#define TListbox_GetW(pListbox) TWidget_GetW((TWidget_t*)(pListbox))
//★预设项宽度
#define TListbox_SetW(pListbox,w) TWidget_SetW(((TWidget_t*)(pListbox)),w)
//★得到项显示宽度
#define TListbox_GetDispW(pListbox) TWidget_GetHPageSize((TWidget_t*)(pListbox))

/**********************************************************************
                          行为函数
**********************************************************************/

//-------------------------初始化函数-----------------------------
//注:考虑很多系统仅使用静态内层分配,故将创建函数留给更高一层,即
//可在此基础上立真正的创建函数
void TListbox_Init(TListbox_t *pListbox,  //由用户分配的存储空间
                   TWinHandle_t hWin,     //已建立并初始化好的窗口
                   unsigned char Flag,    //控件标志
                   TItemSize_t Items,         //预定义项目总数
                   TGUINotify_t cbGUINotify); //通报函数

//-------------------------按键处理函数-----------------------------
void TListbox_Key(TListbox_t *pListbox,unsigned char Key);

//-------------------------窗口画图函数-----------------------------
void ListBox_Paint(TListbox_t *pListbox,    //具体控件
                   //绘制标志,见按键处理函数返回定义,0xff表未绘制所有
                   unsigned char PaintFlag);

//-------------------------窗口重画所有函数-------------------------
//以此为基础的应用程序(非继承类)初始化完成后调用此函数重画所有
#define  ListBox_PaintAll(pListbox) \
  do{ListBox_Paint(pListbox,0xff);}while(0)


//-------------------------设置焦点函数-----------------------------
//★直接使用基类实现,仅供继承类使用
#define TListbox_SetFocus(pListbox,y,x,w) \
  do{TWidget_SetFocus((TWidget_t*)(pListbox),(y),(x),(w));}while(0)

#endif

