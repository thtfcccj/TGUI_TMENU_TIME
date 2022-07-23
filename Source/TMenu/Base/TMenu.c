/*******************************************************************************

                           TMenu基本构件总体实现

*******************************************************************************/
#include "TMenu.h"
#include "TMenu_CheckBox.h"
#include "TMenu_MNumAdj.h"
#include "TMenu_RadioBox.h"
#include "TMenu_SubMenu.h"
#include "TMenu_Buf.h"
#include "TMenu_EditSel.h"
#include <string.h>

#ifndef TMENU_DIS_EDIT_STRING 
  #include "TMenu_EditString.h"
#endif 

/**********************************************************************
                     内部数据结构-菜单管理器
**********************************************************************/

//因一个显示上只存在一个菜单项,故障使用一片共享内存区分配菜单系统
union _MenuDataPv{
  struct _CheckboxData CheckBox;
  struct _MNumAdjData MNumAdj;
  struct _RadioBoxData RadioBox;
  struct _SubMenuData SubMenu;
  struct _EditSelData EditSel;
  struct _EditSelData EditString;  
};

//定义子菜单跟踪结构
struct _Trace{
  unsigned char Pos[TMENU_LAYER_COUNT]; //保存子菜单在各层的位置
  unsigned char Layer;                  //当前菜单所在层数
};

//菜单管理器
struct _TMenuMng{
  const TMenu_t *pCurMenu;    //指向当前菜单,不在菜单内时为NULL
  struct _Trace Trace;        //菜单跟踪器
  union _MenuDataPv vData;    //当前菜单使用的自由数据区
};

/**********************************************************************
                     菜单系统静态内存分配
**********************************************************************/
//重定义主窗口个数防止出错
#if TMENU_MWIN_SIGNAL > 1
  #undef TMENU_MWIN_SIGNAL
  #define TMENU_MWIN_SIGNAL  1
#endif

static struct _TMenuMng MenuMng;    //直接静态定义  
//操作宏:
#define	TMenu_GetCurMenuMng() (&MenuMng)

//静态窗义窗口,包含菜单窗口占两个,一个菜单界面,一个编辑控件使用
static TWin_t _Win[TMENU_FIX_WIN_COUNT + TMENU_MWIN_SIGNAL + TMENU_MSG_BOX + 2];

//为菜单窗口分配缓存间
#if(TMENU_MWIN_SIGNAL == 0)
static char _MenuBuf[TMENU_WIN_W * TMENU_WIN_H];
#endif

//为菜单窗口附加的编辑控件分配空间
static char _EditBuf[TMENU_WIN_W];

//菜单管理器使用TWinMng_Row,为其静态分配空间:
#include "TWinMng_Row.h"//使用并实例化该管理器管理窗口系统
static TWinMng_t _TWinMng;

//操作函数
//为防止常量不可移值性问题,直接使用case 
/*static TMenuOperate_t _Oprate[] = 
{
  {TMenu_SubMenuCreate, TMenu_SubMenuKey},      //子菜单模式
  {TMenu_RadioBoxCreate, TMenu_RadioBoxKey},       //单选框模式
  {TMenu_MNumAdjCreate,TMenu_MNumAdjKey},       //多值调整模式
  {TMenu_CheckBoxCreate, TMenu_CheckBoxKey},       //复选框模式  
  //{},       //列表框模式(暂未实现)
};*/

/**********************************************************************
                     成员操作函数
**********************************************************************/

//-----------------------得到当前菜单指针----------------------------
//指针为NULL时表示不在菜单内
const TMenu_t *pTMenu_GetCurMenu(void)
{
  return TMenu_GetCurMenuMng()->pCurMenu;
}

//-----------------------得到当前菜单层次----------------------------
unsigned char TMenu_GetCurMenuLayer(void)
{
  return TMenu_GetCurMenuMng()->Trace.Layer;
}

//-----------------------强制进入指定菜单------------------------
struct _GotoMenuData{
  const TMenu_t *pGogoMenu;//跳转的菜单
  unsigned short MenuID;  //菜单ID号
}GotoMenuData;

//-----------------------强制进入指定菜单------------------------
void TMenu_GogoMenu(const TMenu_t *pCurMenu, //强制跳入的菜单结构
                    unsigned short MenuID,  //菜单ID,无效时为0
                    unsigned char Layer)    //该结构所在菜单内的层次
{
  //跟踪菜单位置
  TMenu_GetCurMenuMng()->Trace.Layer = Layer;
  //暂存
  GotoMenuData.pGogoMenu = pCurMenu;
  GotoMenuData.MenuID = MenuID;
}

//----------------------得到跳转的菜单项-------------------
//返回NULL表示无后继菜单
static const TMenu_t *_GetGogoMenu(void)  //该结构所在菜单内的层次
{
  const TMenu_t *pMenu;
	if(GotoMenuData.pGogoMenu || GotoMenuData.MenuID){
    pMenu =  TM_pGetMenu(GotoMenuData.pGogoMenu,GotoMenuData.MenuID);
    memset(&GotoMenuData,0,sizeof(struct _GotoMenuData));//清零
  }
  else pMenu = NULL;
  return pMenu;
}

//-----------------------强制刷新当前菜单函数--------------------------
//调用此函数将立即刷新当前菜单的所有显示内容
//通过在指定菜单周期调用此函数，可实现内容的动态刷新
#include "TListBox.h"
void TMenu_Refresh(void)
{
  const TMenu_t *pCurMenu = pTMenu_GetCurMenu();
  struct _TMenuMng *pMng = TMenu_GetCurMenuMng();
  unsigned char Type = TM_GetType(pCurMenu) & TMTYPE_MASK;
  //缓冲区模式调用用户程序刷新
  if(Type == TMTYPE_BUF){
    #ifndef TMENU_DIS_BUF 
      TMenu_BufRefresh(pCurMenu,&pMng->vData);
    #endif
    return;
  }
  //编辑字符串模式暂无强制刷新功能
  if(Type == TMTYPE_EDITSTRING){
    return;
  }
  if(Type == TMTYPE_MNUMADJ){//多值模式
    TMenu_MNumAdjRefresh(pCurMenu,&pMng->vData);
    return;
  }  
  
  //其它模式直接利用继承关系
  ListBox_PaintAll((TListbox_t *)(&pMng->vData));
}

//-----------------根据窗口ID得到窗口句柄得到函数-------------------
//辅助函数,仅在系统初始化时使用
TWin_t *TMenu_pGetTWin(unsigned char WinID)
{
  return  &_Win[WinID];
}

//-----------------将光标强制移动到指定位置函数-------------------
void TMenu_MoveCousor(unsigned char Pos)
{
  //直接利用继承关系
  struct _TMenuMng *pMng = TMenu_GetCurMenuMng();
  TListbox_SetSel((TListbox_t *)(&pMng->vData), Pos);
  ListBox_PaintAll((TListbox_t *)(&pMng->vData));//更新显示
}

/**********************************************************************
                     行为函数
**********************************************************************/
#ifndef NULL_TMENU
//-------------------------------TMenu初始化函数-----------------------
//开机时调用初始化菜单系统
void TMenuInit(void)
{
  unsigned char i;
  memset(&MenuMng,0,sizeof(struct _TMenuMng));
  //初始化菜单窗口
  #if(TMENU_MWIN_SIGNAL == 0)
  TWin_InitWin(&_Win[TM_hGetMenuWin()],
               TMENU_WIN_W,TMENU_WIN_H,
               _MenuBuf,sizeof(_MenuBuf));
  #endif
  //初始化编辑控件窗口
  TWin_InitWin(&_Win[TM_hGetEditWin()],
               TMENU_WIN_W,1,
               _EditBuf,sizeof(_EditBuf));
  //菜单以上窗口默认置为隐藏状态
  for(i = TMENU_FIX_WIN_COUNT + TMENU_MWIN_SIGNAL + 1; 
      i < TMENU_FIX_WIN_COUNT + TMENU_MWIN_SIGNAL + TMENU_MSG_BOX + 2;
      i++){
    TWin_Hidden(&_Win[i]);

  }

  //初始化WM管理器
  TWinMng_Init(&_TWinMng,0,_Win,
               TMENU_FIX_WIN_COUNT + TMENU_MWIN_SIGNAL + TMENU_MSG_BOX + 2);
  //挂接管理器
  TWinMng_SetCur(&_TWinMng);
}


//------------------------------进入菜单函数处理----------------------------
//返回非0时表示退出菜单系统
static signed char _EnterMenu(struct _TMenuMng *pMng,
                              const TMenu_t *pCurMenu,//菜单位置
                              unsigned char Data)     //带入的自由数据

{
  unsigned char Type = TM_GetType(pCurMenu) & TMTYPE_MASK;
  if(Type > TMTYPE_MAX){//菜单系统出错
    TMenu_GetCurMenuMng()->pCurMenu = NULL;
    return -2;
  }
  TMenu_GetCurMenuMng()->pCurMenu = pCurMenu;//更新菜单

  TMenu_MaxWin(Type, TM_hGetMenuWin()); //最大化窗口(可变窗口哩)
  
  //创建当前菜单,为防止常量不可移值性问题,直接使用case:
  //_Oprate[Type].Create(pCurMenu,TM_hGetMenuWin(),
  //                      Data,&pMng->vData);
  memset(&pMng->vData, 0, sizeof(union _MenuDataPv));//使用前初始化
  switch(Type){
  #ifndef TMENU_DIS_SUBMENU
  case TMTYPE_SUBMENU://子菜单模式
    TMenu_SubMenuCreate(pCurMenu,TM_hGetMenuWin(),
                        Data,&pMng->vData);
    break;
  #endif
  #ifndef TMENU_DIS_RADIOBOX
  case TMTYPE_RADIOBOX://单选框模式
    TMenu_RadioBoxCreate(pCurMenu,TM_hGetMenuWin(),
                        Data,&pMng->vData);
    break;
  #endif
  #ifndef TMENU_DIS_MNUM_ADJ
  case TMTYPE_MNUMADJ://多值调整模式
    TMenu_MNumAdjCreate(pCurMenu,TM_hGetMenuWin(),
                        Data,&pMng->vData);
    break;
  #endif
  #ifndef TMENU_DIS_CHECKBOX 
  case TMTYPE_CHECKBOX://复选框模式
    TMenu_CheckBoxCreate(pCurMenu,TM_hGetMenuWin(),
                        Data,&pMng->vData);
    break;
  #endif
  #ifndef TMENU_DIS_BUF 
  case TMTYPE_BUF://缓冲区模式
    TMenu_BufCreate(pCurMenu,TM_hGetMenuWin(),
                        Data,&pMng->vData);
    break;
  #endif
  #ifndef TMENU_DIS_EDIT_SEL 
  case TMTYPE_EDITSEL://编辑选择模式
    TMenu_EditSelCreate(pCurMenu,TM_hGetMenuWin(),
                        Data,&pMng->vData);
    break;
  #endif    
  #ifndef TMENU_DIS_EDIT_STRING 
  case TMTYPE_EDITSTRING://编辑字符串模式
    TMenu_EditStringCreate(pCurMenu,TM_hGetMenuWin(),
                           Data,&pMng->vData);
    break;
  #endif 
  default:break;
  }
  return 0;
}

//--------------------------------退出菜单函数-------------------------------
void TMenu_ExitMenu(void)
{
  if(TMenu_GetCurMenuMng()->pCurMenu == NULL) return;//不在菜单中
  TMenu_GetCurMenuMng()->pCurMenu = NULL;
  #if(TMENU_MWIN_SIGNAL > 0)//独立主窗口时清除窗口
    TWM_FreeHandle(TM_hGetMenuWin());
  #else
    TWin_ClrFocus(TWM_pGetWin(TM_hGetMenuWin()));//取消菜单内的焦点显示
  #endif
  //其它窗口
  TWM_FreeHandle(TM_hGetEditWin());
    
  TMenu_cbExitMenuNotify();//最后通报退出菜单系统
}

//-------------------------------TMenu按键处理函数----------------------------
//进菜单或在菜单内部时调整此函数
//返回非0时表示退出菜单系统
signed char TMenuKey(unsigned char Key)
{
  struct _TMenuMng *pMng = TMenu_GetCurMenuMng();
  const TMenu_t *pMenu;
  const TMenu_t *pCurMenu = pMng->pCurMenu;
  signed char Return = 0;
  //未进菜单时
  if(!pCurMenu){
    TMenu_cbEnterMenuNotify();//先通报进入菜单系统
    if(Key == TGUI_KEY_ENTER){
      #if (TMENU_MWIN_SIGNAL > 0) //主窗口独立时先置为显示状态
        TWin_Disp(TMenu_pGetTWin(TM_hGetMenuWin()));
      #endif
      pCurMenu = TM_pGetTopMenu();//确认键进菜单
      pMng->Trace.Layer = 0; //跟踪初始化
      memset(pMng->Trace.Pos,0,TMENU_LAYER_COUNT);
      return _EnterMenu(pMng,pCurMenu,0);
    }
    //else //其它按键无效
    return 0;
  }
  //菜单内按键处理,为防止常量不可移值性问题,直接使用case:
  //Return = _Oprate[TM_GetType(pCurMenu) & TMTYPE_MASK].Key();
  switch(TM_GetType(pCurMenu) & TMTYPE_MASK){
  #ifndef TMENU_DIS_SUBMENU
  case TMTYPE_SUBMENU://子菜单模式
    Return = TMenu_SubMenuKey(pCurMenu,Key,&pMng->vData);
    break;
  #endif
  #ifndef TMENU_DIS_RADIOBOX
  case TMTYPE_RADIOBOX://单选框模式
    Return = TMenu_RadioBoxKey(pCurMenu,Key,&pMng->vData);
    break;
  #endif
  #ifndef TMENU_DIS_MNUM_ADJ
  case TMTYPE_MNUMADJ://多值调整模式
    Return = TMenu_MNumAdjKey(pCurMenu,Key,&pMng->vData);
    break;
  #endif
  #ifndef TMENU_DIS_CHECKBOX  
  case TMTYPE_CHECKBOX://复选框模式
    Return = TMenu_CheckBoxKey(pCurMenu,Key,&pMng->vData);
    break; 
  #endif
  #ifndef TMENU_DIS_BUF  
  case TMTYPE_BUF://缓冲区模式
    Return = TMenu_BufKey(pCurMenu,Key,&pMng->vData);
    break; 
  #endif
  #ifndef TMENU_DIS_EDIT_SEL 
  case TMTYPE_EDITSEL://编辑选择模式
    Return = TMenu_EditSelKey(pCurMenu, Key, &pMng->vData);
    break;
  #endif  
  #ifndef TMENU_DIS_EDIT_STRING 
  case TMTYPE_EDITSTRING://编辑字符串模式
    Return = TMenu_EditStringKey(pCurMenu, Key, &pMng->vData);
    break;
  #endif      
  default: return 0;//出错
  }
  
  //检查强制跳转菜单状态
  pMenu = _GetGogoMenu();
  if(pMenu)  //强制跳转
    _EnterMenu(pMng,pMenu,pMng->Trace.Pos[pMng->Trace.Layer]);
  //检查返回状态
  else if(Return >= 0){//进入子菜单或调整项
    //跟踪菜单位置
    pMng->Trace.Pos[pMng->Trace.Layer] = Return;
    pMng->Trace.Layer++;
    pCurMenu = TM_pGetSubMenu(pCurMenu,Return);
    return _EnterMenu(pMng,pCurMenu,0);
  }
  else if(Return == -2){//返回上级
    //跟踪菜单位置
    pMng->Trace.Layer--;
    pCurMenu = TM_pGetParent(pCurMenu);
    if(pCurMenu == NULL){
      #if (TMENU_MWIN_SIGNAL > 0) //主窗口独立时退出自动置为不显示状态
        TWin_Hidden(TMenu_pGetTWin(TM_hGetMenuWin()));
      #endif
      TMenu_ExitMenu();//退出菜单
      return -1;
    }
    return _EnterMenu(pMng,pCurMenu,pMng->Trace.Pos[pMng->Trace.Layer]);
  }
  return 0;//其它状态未处理完成
}

//-------------------------------TMenu进程函数----------------------------
//10ms调用一次
void TMenuTask(void)
{
  /*/此进程主要处理窗口管理器
  static unsigned char Timer;
  
  Timer++;
  if(Timer) TWinMng_UpdateRow();
  else TWinMng_UpdateLcd();*/

  while(TWinMng_UpdateRow()) 
    TWinMng_UpdateLcd();
}


#endif //#ifndef NULL_TMENU

/**********************************************************************
                        菜单进出栈实现
**********************************************************************/

//将菜单结构出栈
const TMenu_t *pPopMenu(void)
{
  return TMenu_GetCurMenuMng()->pCurMenu;
}


//将当前指向的数据缓冲区出栈
void *pPopData(void)
{
  return &TMenu_GetCurMenuMng()->vData;
}



/**********************************************************************
                         TGUI回调函数实现
**********************************************************************/

//以下为加速运行使用的常量数组,当单片机不支持常量表时,可直接定义到数据区：
//半字型移位查找表(当显示屏高度>8时才定义)

//字符串型移位查找表
const unsigned char  U8ShiftLUT[8] = { 	
  0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80
};

//-----------------TGUI所需要的临时字符串缓冲----------------------
#ifdef TGUI_BUF_SIZE //自定义大小时
  unsigned long _TGUI_Buf[TGUI_BUF_SIZE / 4];	//考虑到显示区会超过显示屏,故加大容量
#else
  unsigned long _TGUI_Buf[TLCD_WIDTH / 2];	//考虑到显示区会超过显示屏,故加大容量
#endif
  
char *TGUI_pGetBuf(void)      
{
  return (char*)_TGUI_Buf;
}   

//-----------------------得到两按钮之间的间隔--------------------
//根据显示屏大小与当前项窗口宽度得到按钮之间的间隔
unsigned char cbGetBottomSpace(TItemSize_t ItemW)
{
  ItemW = ItemW;
  return 2;
}














