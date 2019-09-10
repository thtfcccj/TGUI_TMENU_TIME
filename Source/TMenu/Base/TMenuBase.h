/*******************************************************************************

                  TMenu基本构件

*******************************************************************************/
#ifndef __T_MENU_BASE_H 
#define __T_MENU_BASE_H

#include "TMenuCfg.h"
#include "TGUI.h"       //构建在TGUI之上
#include "TMenuNotify.h"  //消息通报机制


/****************************************************************
                  菜单主结构构建说明及定义
*****************************************************************/

//根据实际应用需要,菜单结构留给用户定义,其基本类型如下:
//struct _TMenu{
//  unsigned char Type;      //菜单类型及相关标志
//  unsigned char Size;       //由菜单类型决定的相关数据大小
//  const LanguageCode_t *pgHeader; //菜单头指针,为NULL时从回调里读取
//  const struct _TMenu *pParent;   //自已的父菜单
//  const void *pv;           //根据菜单类型确定的自由参数指针
//  TMenuNotify_t cbNotify;     //与用户空间交互的通报函数
//};
//其中:LanguageCode_t  可以为char*结构
//也可以为其它结构,如:多国语言支持时可用于保存不同语种对应的字符串数组指针。


//此结构的成员访问仅允许通过TMenu.h定义的函数访问
typedef struct _TMenu TMenu_t;
//TMenu_t由用户根据需要和情况定义：

#include "TMenuMember_IARArm.h" //通用型(ARM)定义
//#include "TMenuMember_KeilC51.h" //C51类型定义
//#include "TMenuMember_AVRGCC.h" //AVRGCC类型定义

/****************************************************************
                  菜单主结构内，类型及相关标志定义
*****************************************************************/
//在该结构其中,菜单类型及相关标志定义为：
#define   TMTYPE_MASK       0x0f //低4位为菜单类型,定义为:
  #define   TMTYPE_SUBMENU  0   //子菜单模式
  #define   TMTYPE_RADIOBOX 1   //单选框模式
  #define   TMTYPE_MNUMADJ  2   //多值调整模式
  #define   TMTYPE_CMD      3   //命令模式
  #define   TMTYPE_BUF      4   //用户缓冲区模式,整个显示与按键(除退出)均交由用户处理 
  #define   TMTYPE_CHECKBOX 5   //复选框模式
  #define   TMTYPE_LISTBOX  6   //列表框模式(暂未实现)
  #define   TMTYPE_EDITSEL  7   //编辑选择模式
  #define   TMTYPE_EDITSTRING 8   //编辑字符串模式

  #define	 TMTYPE_MAX    8	  //定义菜单类型可使用有效个数

//其它４在不同菜单类型模式下有不同的定义
#define   TMTYPE_USER_SHIFT 4
#define   TMTYPE_USER_MASK  0xf0 

/****************************************************************
        菜单主结构对应菜单类型需完成的操作函数定义
*****************************************************************/
typedef struct{
  //1:创建该类型菜单,从用户区获得数据并更新显示
  //返回创建成功:
  signed char (*Create)(const TMenu_t *pMenu,    //指向的菜单系统
                        TWinHandle_t hWin,       //分配给菜单的窗口
                        unsigned char Data,    //带入的自由数据
                        void *pv);      //可供使用的自由缓冲区
  //2:响应按键操作并根据情况与用户进行数据交互,并更新显示
  //返回值定义为: 正值:进入相应子菜单,-1,不管,-2:返回上层菜单
  signed char(*Key)(const TMenu_t *pMenu,    //指向的菜单系统
                    unsigned char Key,       //送入的键值
                    void *pv);              //已被使用的自由数据缓冲区
}TMenuOperate_t;

/****************************************************************
                        辅助函数
*****************************************************************/

//----------------------数据与菜单临时保存相关----------------
//在各部分按键处理中,使用的回调函数无法直接获得
//相关数据,且按键处理过程不会执行其它程序,故使用下列操作
//将菜单结构入栈
//void PushMenu(const TMenu_t *);
#define	PushMenu(pTMenu) do{pTMenu = pTMenu; }while(0)

//将菜单结构出栈
const TMenu_t *pPopMenu(void);

//当前指向的数据缓冲区入栈
//void PushData(const TMenu_t *);
#define	PushData(pv) do{pv = pv;}while(0)

//将当前指向的数据缓冲区出栈
void *pPopData(void);

//-------------------得到主窗口名柄函数----------------------
//TWinHandle_t  TM_hGetMWin(void);
#define  TM_hGetMWin()  (TMENU_FIX_WIN_COUNT) 

//-------------------得到菜单窗口名柄函数----------------------
//TWinHandle_t  TM_hGetMenuWin(void);
#define  TM_hGetMenuWin()  (TMENU_FIX_WIN_COUNT + TMENU_MWIN_SIGNAL) 

//----------------------得到编辑器窗口名柄函数---------------------
//TWinHandle_t  TM_hGetEditWin(void);
#define  TM_hGetEditWin() \
  (TMENU_FIX_WIN_COUNT + TMENU_MWIN_SIGNAL + TMENU_MSG_BOX + 1) 

//----------------------得到对话框起始窗口名柄函数---------------------
//TWinHandle_t  TM_hGetMsgBoxWin(void);
#define  TM_hGetMsgBoxWin()  \
  (TMENU_FIX_WIN_COUNT + TMENU_MWIN_SIGNAL + TMENU_MSG_BOX + 2) 

#endif
