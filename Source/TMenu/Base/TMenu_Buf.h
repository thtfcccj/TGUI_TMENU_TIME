/* ----------------------------------------------------------------------------
 *                TMenu用户缓冲区菜单实现
 缓冲区菜单将菜单的整个显示缓冲区交给用户处理,所有的按键也交由用户进行处理
 * --------------------------------------------------------------------------*/

#ifndef __T_MENU_BUF_H 
#define __T_MENU_BUF_H 

#include "TMenuBase.h"

/**********************************************************************
                      配置区
**********************************************************************/
//无

/**********************************************************************
                     用户使用相关
**********************************************************************/
//当为单选框时,菜单结构作如下定义:
//typedef struct _TMenu{
//  TMTYPE_BUF;             //菜单类型为缓冲区模式
//  unsigned char Size;     //0
//  LanguageCode_t *pgHeader; //菜单头,为NULL时从回调里读取
//  struct _TMenu *pParent;   //自已的父菜单
//  const LanguageCode_t*;   //NULL
//  TMenuNotify_t Notify;     //与用户空间交互的通报函数
//}TMenu_t;

//此模式无用户区标志

//缓冲菜单数据结构:
struct _BufUser{
  unsigned char Key;               //用户按键
  unsigned char Notify;            //用户与菜单的通报
  unsigned char w;                 //缓冲区宽度
  unsigned char h;                 //缓冲区高度
  char *pBuf;			                 //缓冲区
};

//其中,按键时,Notify低4位定义为:
#define     TM_BUF_NOTIFY_MASK   0x0f
#define     TM_BUF_NOTIFY_NON    0 //无通报
#define     TM_BUF_NOTIFY_DISP   1 //用户通报需更新显示

//其它位定义为：
#define     TM_BUF_NOTIFY_EXIT_FLAG  0x80 //用户通报退出此菜单

//用户回调函数响应类型及需实现的功能有:
//1:类型为:TM_NOTIFY_GET_DATA时,通报更新整屏显示
//自定义类型为:
#define	TM_NOTIFY_BUF_KEY  (TM_NOTIFY_WIDGET_BASE + 0)//由用户响应的按键(退出键除外)


/**********************************************************************
                     内部数据结构
**********************************************************************/
struct _BufData{
 TWinHandle_t hWin;       //分配给菜单的窗口
 unsigned char Flag;      //内部标志
 struct _BufUser User;   //用户数据
};

//------------------------创建函数实现----------------------------
//1:创建该类型菜单,从用户区获得数据并更新显示
//返回创建成功:
signed char TMenu_BufCreate(const TMenu_t *pMenu,    //指向的菜单系统
                                 TWinHandle_t hWin,       //分配给菜单的窗口
                                 unsigned char Data,    //带入的自由数据
                                 void *pv);      //可供使用的自由缓冲区

//------------------------响应按键函数实现----------------------------
//2:响应按键操作并根据情况与用户进行数据交互,并更新显示
//返回值定义为: 正值:进入相应子菜单,-1,不管,-2:返回上层菜单
signed char TMenu_BufKey(const TMenu_t *pMenu,    //指向的菜单系统
                         unsigned char Key,       //送入的键值
                         void *pv);      //已被使用的自由数据缓冲区

//--------------------------强制刷新函数实现-----------------------------
void TMenu_BufRefresh(const TMenu_t *pMenu,    //指向的菜单系统
                      void *pv);      //已被使用的自由数据缓冲区


#endif
