/*******************************************************************************

                           TMenu基本构件
TMenu是建立在TGUI之上的一套菜单系统,它可以完成单色屏上的参数调整与显示
*******************************************************************************/
#ifndef __T_MENU_H 
#define __T_MENU_H

/**********************************************************************
                    TMenu模组全局定义说明
**********************************************************************/
//内部缓冲区大小，默认显示屏两倍字符宽度
//在小屏或特殊应用，显示字符超过显示屏过多时，要单独加大定义
//#define TMENU_BUF_SIZE    TLCD_WIDTH

//定义当前版本(原版本或其分支见git)
//此软件模块采用MIT协议， 请在您在你设备适当位置放置此字符串的显示
#define TMENU_STR_VERSION    "TMENU V4.21"  

/**********************************************************************
                         不支持此模块时定义
**********************************************************************/
#include "TMenuBase.h"       //用户接口
//#define     NULL_TMENU        //临时不挂接菜单系统时定义 
 
#ifdef NULL_TMENU
  #define TMenuInit() do{}while(0)
  
  #define TMenuKey(Key) do{}while(0)
  
  #define TMenuTask() do{}while(0)
#else

/**********************************************************************
                         支持此模块时接口函数
**********************************************************************/
//用户的每个一菜单项增色需实现一个TMenu_t(在TMenuBase.h里);
//-------------------------------TMenu初始化函数----------------------
//开机时调用初始化菜单系统
void TMenuInit(void);

//-------------------------------TMenu按键处理函数----------------------------
//进菜单或在菜单内部时调整此函数
//返回非0时表示退出菜单系统
signed char TMenuKey(unsigned char Key);

//-------------------------------TMenu进程函数----------------------------
//10ms调用一次
void TMenuTask(void);

#endif //#ifdef NULL_TMENU

/**********************************************************************
                             成员操作函数
//支持常量指针时可直接调用
**********************************************************************/

//-----------------------得到当前菜单指针----------------------------
//指针为NULL时表示不在菜单内
const TMenu_t *pTMenu_GetCurMenu(void);

//-----------------------得到当前菜单层次----------------------------
unsigned char TMenu_GetCurMenuLayer(void);

//-----------------------强制进入指定菜单------------------------
void TMenu_GogoMenu(const TMenu_t *pCurMenu, //强制跳入的菜单结构
                    unsigned short MenuID,  //菜单ID,无效时为0
                    unsigned char Layer);    //该结构所在菜单内的层次

//-----------------------返回上级菜单------------------------
//已是顶层菜单时不会退出
void TMenu_ReturnUp(void);

//-----------------------强制刷新当前菜单函数--------------------------
//调用此函数将立即刷新当前菜单的所有显示内容
//通过在指定菜单周期调用此函数，可实现内容的动态刷新
void TMenu_Refresh(void);

//-------------------------窗口重画所有函数-------------------------
//以此为基础的应用程序(非继承类)初始化完成后调用此函数重画所有
#define TMenu_PaintAll(pListboxEx) \
  do{ListBox_PaintAll((TListbox_t *)(pListboxEx));}while(0)

//--------------------------------退出菜单函数--------------------------
void TMenu_ExitMenu(void);

//-----------------根据窗口ID得到窗口句柄得到函数-------------------
//辅助函数,仅在系统初始化时使用
TWin_t *TMenu_pGetTWin(unsigned char WinID);

//-----------------将光标强制移动到指定位置函数-------------------
void TMenu_MoveCousor(unsigned char Pos);

/**********************************************************************
                         TMenu用户回调函数
**********************************************************************/

//--------------得到TMenu所需要的自由数据缓冲------------------
//>一行用到是最长字符
extern void *TMenu_pGetBuf(void); 

//---------------------进入菜单通报函数------------------------
//#define TMenu_cbEnterMenuNotify() do{}while(0)
void TMenu_cbEnterMenuNotify(void);

//---------------------退出菜单通报函数------------------------
//#define TMenu_cbExitMenuNotify() do{}while(0)
void TMenu_cbExitMenuNotify(void);

//---------------------最大化窗口大小函数-----------------------
//可变窗口时,进入菜单前先将可变窗口最大化
#ifdef TMENU_WIN_VARIABLE   //可变窗口时在用户区定义
  void TMenu_MaxWin(unsigned char Type, //菜单类型
                  TWinHandle_t hWin);
#else
  #define TMenu_MaxWin(type, hWin) do{}while(0)
#endif

//---------------------重置窗口大小函数--------------------------
//可变窗口时,进入菜单后根据窗口内容大小自动变换窗口
#ifdef TMENU_WIN_VARIABLE   //可变窗口时在用户区定义
  void TMenu_ReBulidWin(TListboxEx_t *pListboxEx,//重建完成的列表框
                        unsigned char w,  //当前菜单系统所需宽度
                        unsigned char h,//当前菜单系统所需高度
                        unsigned char HeaderW); //头宽度
#else
  #define TMenu_ReBulidWin(hWin,w,h,HeaderW) do{}while(0)
#endif

/**********************************************************************
                         TGUI中未实现的回调函数
**********************************************************************/
//----------------------得到常量缓存结构函数-----------------------
//因某些单片机不支持code区指针,故在内核已避免了code区指针问题,
//内核需常量时,使用下列常量临时缓冲存常量数据>= 3;
extern char *TGUI_pGetConstBuf(void); 

//----------------------得到保存按钮字-----------------------
//如[保  存]
const char *cbGetSaveBottom(void); 

//----------------------得到返回按钮字-----------------------
//如[返  回]
const char *cbGetReturnBottom(void); 

/**************************************************************************
                        TMenu相关成员操作函数
  当没有定义TMENU_USE_CONST_BUF时下列函数已实现,否则用户需实现下列函数
  Menu_t结构中的使用指针的所有成员通过此函数获得相关信息
**************************************************************************/

//--------------------得到顶层菜单结构--------------------------
//TMENU系统通过此结构使当前菜单指向顶层菜单
//当存在多个主菜单时，可修改此函数根据系统应用跳转不同的主菜单
const TMenu_t *TM_pGetTopMenu(void);

//---------------------得到父菜单结构函数----------------------------
//TMENU系统通过此结构使当前菜单指向父菜单,NULL表示已退出
const TMenu_t *TM_pGetParent(const TMenu_t *pMenu);

//-------------------------获得子菜单结构函数------------------------
//TMENU系统通过此结构使当前菜单指向某个子菜单
const TMenu_t *TM_pGetSubMenu(const TMenu_t *pMenu,
                              unsigned char SubMenuID);

//-------------------------获得指定菜单结构函数------------------------
const TMenu_t *TM_pGetMenu(const TMenu_t *pMenu,   //强制跳转的菜单结构
                           unsigned short MenuID); //自由参数,可用于菜单识别

//-------------------从菜单结构获得菜单头函数------------------------
const char *TM_GetHeader(const TMenu_t *pMenu);

//-------------------从菜单结构获得类型函数------------------------
unsigned char TM_GetType(const TMenu_t *pMenu);

//-------------------从菜单结构获得大小函数------------------------
#ifdef TM_EN_DYNC_LUT  //动态查找表支持时
  unsigned short TM_GetSize(const TMenu_t *pMenu);
#else
  unsigned char TM_GetSize(const TMenu_t *pMenu);
#endif

//----------------得到指定子菜单项头函数-------------------
const char *TM_GetSubMenuHeader(const TMenu_t *pMenu,
                                unsigned char Item);

//-------------------从菜单结构获得项字符串函数-------------------
const char *TM_GetItemString(const TMenu_t *pMenu,
                             unsigned char Item);

//-------------------统计菜单项最大宽度函数-----------------------
TItemSize_t TM_GetItemMaxLen(const TMenu_t *pMenu);

//-------------------用户数据转换为Checkbox数据---------------------
//替换TCheckbox_SetAllCheck()
void TM_CheckboxSetAllCheck(const TMenu_t *pMenu,  //菜单
                            TCheckbox_t *pCheckbox, //pCheckbox
                            unsigned char *pCheck, //用户输入的掩码
                            TItemSize_t Items);     //项总数

//-------------------Checkbox数据转换为用户数据---------------------
//替换TCheckbox_GetAllCheck()
void TM_CheckboxGetAllCheck(const TMenu_t *pMenu,  //菜单
                            TCheckbox_t *pCheckbox, //pCheckbox
                            unsigned char *pCheck, //用户输入的掩码
                            TItemSize_t Items);      //项总数

#endif //#define __T_MENU_H

