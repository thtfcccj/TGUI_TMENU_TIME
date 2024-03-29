/*******************************************************************************

                  TMenu菜单之字符串编辑接口(支持输入法时专用)

*******************************************************************************/
#ifndef __T_MENU_EDIT_STRING_H 
#define __T_MENU_EDIT_STRING_H

#include "TMenuBase.h"
#include "TImeBase.h"    //TIme_String_t
#include "TImeMng.h" 

/**********************************************************************
                      配置区
**********************************************************************/
//注：
//1.字符编辑默认样式已固定，不能也无需配置
//2.字符编辑无保存和返回按钮，用户可响应确认键回调或退出键时保存
//3.此模块可直接供用户使用，也可配合TMenu_EditSel模块使用


/**********************************************************************
                     用户使用相关
**********************************************************************/
//当为单选框时,菜单结构作如下定义:
//typedef struct _TMenu{
//  TMTYPE_EDITSTRING;        //菜单类型为字符编辑模式
//  unsigned char Size;       //项大小这里不使用，定义为0即可
//  LanguageCode_t *pgHeader; //菜单头
//  struct _TMenu *pParent;   //自已的父菜单
//  const char*;              //用于存放挂接的符号表，NULL默认，具本见TImeMng定义
//  TMenuNotify_t Notify;     //与用户空间交互的通报函数
//}TMenu_t;

//用户区数据结构:参考TImeMng定义
struct _EditStringUser{
  char *pString;              //用户已存在的字符串,必须以'\0'结尾
  TIme_String_t Size;         //字符串缓冲区允许大小
  unsigned char DefaultType; //默认输入法,<4
  unsigned char TypeMask;     //可使用的输入法类型
  TIme_String_t DefaultCursor; //默认光标位置,-1时为最后
};
//用户回调函数响应类型及需实现的功能有:
//1:类型为:TM_NOTIFY_GET_DATA时,需将当前User结构所有成员初始化
//2:类型为:TM_NOTIFY_CHANGED时,表示已修改完成但按的是退出键,
//用户可根据应用要求决定是否响应(如:保存或直接退出)
//3:类型为:TM_NOTIFY_SET_DATA时,表示已修改完成并按确认键退出

/**********************************************************************
                     内部数据结构
**********************************************************************/
struct _EditStringData{
 struct _TImeMng *pImeMng;                //管理器结构指针，由回调获得
 struct _EditStringUser User;     //用户数据
};

/**********************************************************************
                        相关函数
**********************************************************************/
//------------------------创建函数实现----------------------------
//1:创建该类型菜单,从用户区获得数据并更新显示
//返回创建成功:
signed char TMenu_EditStringCreate(const TMenu_t *pMenu,    //指向的菜单系统
                                 TWinHandle_t hWin,       //分配给菜单的窗口
                                 unsigned char Data,    //带入的自由数据,这里无意义
                                 void *pv);      //可供使用的自由缓冲区

//------------------------响应按键函数实现----------------------------
//2:响应按键操作并根据情况与用户进行数据交互,并更新显示
//返回值定义为: 正值:进入相应子菜单,-1,不管,-2:返回上层菜单
signed char TMenu_EditStringKey(const TMenu_t *pMenu,    //指向的菜单系统
                              unsigned char Key,       //送入的键值
                              void *pv);      //已被使用的自由数据缓冲区

/**********************************************************************
                      回调函数
**********************************************************************/

//----------------------得到输入法管理器指针-------------------------
//因输入法管理器所有空间较大，故空间分配交由用户实现，这里获得
//struct _TImeMng *TMenu_EditString_cbpGetImeMng(void);
extern struct _TImeMng TMenu_EditString_ImeMng; //直接定义时(需外部定义)直接使用宏定义:
#define TMenu_EditString_cbpGetImeMng() (&TMenu_EditString_ImeMng)
//关于输入法定时器
//因输入法需一个定时实现，需在系统进程适当位置定时调用下列函数:
//TImeMng_Task(TMenu_EditString_cbpGetImeMng())
//注意调用周期时间乘TImeMng_Task中的计数器总数配置(可重设)应＝1秒.

#endif
