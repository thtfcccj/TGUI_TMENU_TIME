/*******************************************************************************

                  TMenu菜单之编辑选择模式接口

*******************************************************************************/
#ifndef __T_MENU_EDIT_SEL_H 
#define __T_MENU_EDIT_SEL_H

#include "TMenuBase.h"
#include "TEdit_Float.h"
/*******************************************************************************
                                说明
*******************************************************************************/
//因部分编译器显示方式不同,下列的窗口显示若不够清晰,请在其它文本编辑器里打开试试!
//编辑选择模式是指可编辑的内容在此进行编一显示和选择，选中后可进入相应子菜单进行调整
//显示示例为(若显示):
//  
//  ┌─────相关参数────┐
//  │    序号: 002             ↑   //数值调整型
//  │    名称: 设备间         ┃    //字符调整型
//  │    类型: 输入模块       ┃    //字符不可调整型
//  │报警方式: 高-高限报警    ┃    //单选框型
//  │    备注: 位于设备间旁边 ┃    //字符调整型
//  │    [保  存] [退  出]     ↓
//  └←━━━━━━━━━━━→┘

/**********************************************************************
                             配置区
**********************************************************************/

//定义默认样式(见TWidget.h与复选控件):
#define   TM_M_EDIT_SEL_STYLE_DEFAULT    \
  (TWIDGET_EN_HEADER | TWIDGET_EN_VSCOLL | TWIDGET_EN_NOTIFY)

//注:可用的有:
//#define   TWIDGET_EN_FRAME    0x80  //该构件具有边框,当LC高度>-3行时此项有效
//#define   TWIDGET_EN_HEADER   0x40  //允许显示数据头
//#define   TWIDGET_EN_HSCOLL   0x20  //允许显示水平滚动条
//#define   TWIDGET_EN_VSCOLL   0x10  //允许显示垂滚动条
//#define   TWIDGET_EN_NOTIFY   0x04  //无滚动条与边框时允许在用户空时显示提示信息
//#define   TLISTBOXEX_EN_RETURN  0x02      //允许带返回字

/**********************************************************************
                     用户使用相关
**********************************************************************/

//当为编辑选择模式时,菜单结构作如下定义:
//typedef struct _TMenu{
//  TMTYPE_EDITSEL |TM_MEDITSEL_EN_EDIT //菜单类型为编辑选择模式模式与用户区标志
//  unsigned char Size;     //可供选择的调整项个数
//  LanguageCode_t *pgHeader; //菜单头,为NULL时从回调里读取
//  struct _TMenu *pParent;   //自已的父菜单
//  const TMenu_t*;           //存放自已的子菜单阵列连接头,只读时可为struct _TMenuSelRO
//  TMenuNotify_t Notify;     //与用户空间交互的通报函数
//}TMenu_t;

//菜单类型户区标志义为:
//数值可调整,否则不可调整
#define TM_MEDITSEL_EN_EDIT     (0x01 << TMTYPE_USER_SHIFT)
//数值可调整时,是否禁止群保存模式，在群保存模式时将强制有保存与退出键
#define TM_MEDITSEL_DIS_GRP_SAVE (0x02 << TMTYPE_USER_SHIFT)
//注：群保存模式需用户在应用层自已缓存需修改或保存的数据,这里只起
//通知的作用
#define TM_MEDITSEL_GRP_SAVE  (0x04 << TMTYPE_USER_SHIFT)   

/**********************************************************************
                        用户区数据结构
**********************************************************************/
struct _EditSelUser{ 
 //1:对应选择项
 //2:需要读取当前调整数字符串时,指明需要读取那一项
 TItemSize_t CurItem;
 const char *pData;//对应子菜单项位置的当前数值
};

//用户回调函数响应类型及需实现的功能有:
//1:类型为:TM_NOTIFY_GET_DATA时,需装入pData

//2:类型为:TM_NOTIFY_SET_DATA时,>=项数时通知用户区保存所有数据,
//  否则表示通报进入对应项

/**********************************************************************
                     内部数据结构
**********************************************************************/

//定义内部数据结构:
struct _EditSelData{
 TListboxEx_t  ListboxEx;       //结构缓存
 unsigned char HeaderW;        //项的显示头宽度
 unsigned char ItemsW;        //项的内容宽度
 volatile unsigned char Flag;  //内部标志
 struct _EditSelUser User;    //用户数据
};

/**********************************************************************
                             相关函数
**********************************************************************/
//------------------------创建函数实现----------------------------
//1:创建该类型菜单,从用户区获得数据并更新显示
//返回创建成功:
signed char TMenu_EditSelCreate(const TMenu_t *pMenu,    //指向的菜单系统
                                TWinHandle_t hWin,       //分配给菜单的窗口
                                //带入的自由数据,这里表示上次菜单位置
                                unsigned char Data,
                                void *pv);      //可供使用的自由缓冲区

//------------------------响应按键函数实现----------------------------
//2:响应按键操作并根据情况与用户进行数据交互,并更新显示
//返回值定义为: 正值:进入相应子菜单,-1,不管,-2:返回上层菜单
signed char TMenu_EditSelKey(const TMenu_t *pMenu,    //指向的菜单系统
                             unsigned char Key,       //送入的键值
                             void *pv);      //已被使用的自由数据缓冲区

//-----------------------编辑选择模式列框框通报回调实现函数------------------------
//仅供内部通报使用
const void*TMenu_EditSelListBoxNotify(void *pvPource,
                                      unsigned char Type,
                                      const void *pvData);

//-----------------编辑模式通报回调实现函数------------------------
//仅供内部通报使用
const void*TMenu_EditSelEditNotify(void *pvPource,
                                   unsigned char Type,
                                   const void *pvData);

/**********************************************************************
                              回调函数
**********************************************************************/

//----------------------得到当前显示样式-------------------------
#ifdef TMENU_STYLE_VARIABLE   //可变样式时在用户区定义
  unsigned char TMenu_EditSel_cbGetCurStyle(void);
#else
  #define TMenu_EditSel_cbGetCurStyle() \
    (TM_M_EDIT_SEL_STYLE_DEFAULT | TLISTBOXEX_EN_RETURN)
#endif

#endif //__T_MENU_EDIT_SEL_H
