/**************************************************************************
                        TMenu相关成员操作函数实现
  当没有定义TMENU_USE_CONST_BUF时相关函数的实现,否则用户需实现下列函数
  Menu_t结构中的使用指针的所有成员通过此函数获得相关信息
**************************************************************************/
#include "TMenuCfg.h"
#include "TMenu.h"
#include "string.h"

extern const TMenu_t MGetPower; //定义在code区的顶层菜单

#ifdef TM_EN_LUT
static unsigned char _ItemLUT[TM_ITEM_MAX + 1];//缓冲当前查找表
#endif

//----------------------缓冲查找表----------------------------
#ifdef TM_EN_LUT
#define _GetItemLUT(pMenu) \
do{\
  if((pMenu)->Size & TM_TYPE_ITEM_LUT)\
    TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_GET_LUT,_ItemLUT);\
}while(0)

#else
#define _GetItemLUT(pMenu) do{ }while(0)
#endif

//--------------------得到顶层菜单结构--------------------------
//各界面顶层菜单查找表
#include "GUI_Guide.h"
#include "SenMenu.h"
#include "DOutMenu.h"
#include "MidMenu.h"
#include "RecMenu.h"
#include "MainMenu.h"
#include "PowerMenu.h"
#include "ResetMenu.h"
#include "TestMenu.h"

//各界面顶层菜单查找表,(下标为_eGUI_Guide) 
const TMenu_t *_pTopMenu[] = {
  &SenMenu_Top,    //传感器界面,切换键或按1键可切换至此
  &SenMenu_Top,    //可燃筛选界面,切换键或按2键可切换至此
  &SenMenu_Top,    //有毒筛选界面,切换键或按3键可切换至此
  &DOutMenu_Top,    //输出界面,切换键或按4键可切换至此
  &DOutMenu_Top,    //动作输出界面,切换键或按5键可切换至此
  &DOutMenu_Top,    //请求界面,切换键或按6键可切换至此
  //报警键旗下界面
  &SenMenu_Top,    //报警界面 
  //故障键旗下界面
  &SenMenu_Top,    //输入故障,故障键或按*键可切换至此
  &DOutMenu_Top,    //输出故障,故障键或按0键可切换至此
  //菜单键旗下界面
  &MainMenu_Top,    //菜单界面,菜单键或按*键可切换至此
  &RecMenu_Top,    //记录界面,菜单键或按9键可切换至此  
  //其它
  &TestMenu_Top,    //自检界面,按8键可切换至此 
  &PowerMenu_Top,    //用户界面,按#键可切换至此
  &MidMenu_Top,    //中间设备界面,按7键可切换至此
  &MidMenu_Top,    //中间设备故障界面
};

const TMenu_t *TM_pGetTopMenu(void)
{
  //根据界面决定各顶层菜单的结构
  const TMenu_t *pTopMenu = _pTopMenu[GUI_Guide_eGetGuide()];
  //处理复位键
  if((pTopMenu == &PowerMenu_Top) && ResetMenu_IsResetKey())
    pTopMenu = &ResetMenu_GuestLogin;
      
  _GetItemLUT(pTopMenu); //缓存查找表
  return pTopMenu;      //指向缓存
}

//---------------------得到父菜单结构函数----------------------------
const TMenu_t *TM_pGetParent(const TMenu_t *pMenu)
{
  #ifdef TM_DYNC_MENU   //动态菜单支持时通报更新父菜单
   if(pMenu->cbNotify)
    TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_UPDATE_PARENT,NULL);
  #endif
   
  pMenu = pMenu->pParent;
  
  _GetItemLUT(pMenu); //缓存查找表
  return pMenu;
}

//-------------------------获得子菜单结构函数------------------------
const TMenu_t *TM_pGetSubMenu(const TMenu_t *pMenu,
                              unsigned char SubMenuID)
{
  TMenu_t const *const *PAry = (TMenu_t const *const *)pMenu->pv;  //C51的方式

  #ifdef TM_EN_LUT//有表时先查表
  if(pMenu->Size & TM_TYPE_ITEM_LUT) SubMenuID = _ItemLUT[SubMenuID + 1];
  #endif
  
  #ifdef TM_DYNC_MENU   //动态菜单支持时通报更新子菜单
   if(pMenu->cbNotify)
    TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_UPDATE_SUB,&SubMenuID);
  #endif
    
  pMenu = *(PAry + SubMenuID); 
  _GetItemLUT(pMenu); //缓存查找表
  return pMenu;
}

//-------------------------获得指定菜单结构函数------------------------
const TMenu_t *TM_pGetMenu(const TMenu_t *pMenu,   //强制跳转的菜单结构
                           unsigned short MenuID) //自由参数,可用于菜单识别
{
  MenuID = MenuID;
  _GetItemLUT(pMenu); //缓存查找表
  return pMenu;   //该结构即是指定的菜单
}

//-------------------从菜单结构获得菜单头函数-------------------
const char *TM_GetHeader(const TMenu_t *pMenu)
{
  char *pBuf;
  if(pMenu->pgHeader)
    return (const char*)(pMenu->pgHeader->pLan[GetLan()]);

  //菜单里没有,从用户空间获取:
  pBuf = TMenu_pGetBuf();
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_HEADER,pBuf);
  return pBuf;
}

//-------------------从菜单结构获得类型函数------------------------
unsigned char TM_GetType(const TMenu_t *pMenu)
{
  return pMenu->Type;
}

//-------------------从菜单结构获得大小函数------------------------
unsigned char TM_GetSize(const TMenu_t *pMenu)
{
  #ifdef TM_EN_LUT  //有查找表时重定向到查找表大小:
  if(pMenu->Size & TM_TYPE_ITEM_LUT) return _ItemLUT[0];
  #endif  

  return pMenu->Size;
  
}

//----------------得到指定子菜单项头函数-------------------
const char *TM_GetSubMenuHeader(const TMenu_t *pMenu,
                                unsigned char Item)
{
  //得到子菜单结构
  TMenu_t const *const *PAry = (TMenu_t const *const *)pMenu->pv;  //C51的方式

  #ifdef TM_EN_LUT//有表时先查表
  if(pMenu->Size & TM_TYPE_ITEM_LUT) Item = _ItemLUT[Item + 1];
  #endif

  #ifdef TM_DYNC_MENU   //动态菜单支持时通报更新子菜单
    if(pMenu->cbNotify)
      TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_UPDATE_SUB,&Item);
  #endif
    
  pMenu = *(PAry + Item);
  return TM_GetHeader(pMenu);
}

//-------------------从菜单结构获得项字符串函数-------------------
const char *TM_GetItemString(const TMenu_t *pMenu,
                             unsigned char Item)
{
  char *pBuf;
  #ifdef TM_EN_LUT  //有查找表时重定向到查找表
  if(pMenu->Size & TM_TYPE_ITEM_LUT) Item = _ItemLUT[Item + 1];
  #endif

  if(pMenu->pv){
    #ifdef TM_DYNC_MENU   //动态菜单支持时通报更新子菜单
     if(pMenu->cbNotify)
      TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_UPDATE_SUB,&Item);
    #endif
    const LanCode_t *pCode = *((const LanCode_t **)(pMenu->pv) + Item);
    return (const char*)(pCode->pLan[GetLan()]);
  }

  //从用户空间获得数据
  pBuf = TMenu_pGetBuf();
  *pBuf = Item;
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_ITEM,pBuf);
  return pBuf;
}

//-------------------统计菜单项最大宽度函数-----------------------
TItemSize_t TM_GetItemMaxLen(const TMenu_t *pMenu)
{
  //查找表支持
  unsigned char Size = TM_GetSize(pMenu);
  unsigned char i;
  unsigned char Type = pMenu->Type & TMTYPE_MASK;  
  TItemSize_t CurW, ItemW = 0;

  if((Type == TMTYPE_SUBMENU) || (Type == TMTYPE_EDITSEL)){//子菜单模式从子菜单读取长度
    for(i = 0; i < Size; i++){
      CurW = strlen(TM_GetSubMenuHeader(pMenu,i));
      if(CurW > ItemW) ItemW = CurW;
    }
  }
  else{//直接认为项pv里即是字符串
    for(i = 0; i < Size; i++){
      CurW = strlen(TM_GetItemString(pMenu,i));
      if(CurW > ItemW) ItemW = CurW;
    }
  }
  return ItemW;
}

//-------------------根据当前菜单查找表获得所选位置---------------------
unsigned char TMenu_GetItemPosWithLUT(unsigned char Pos)
{
  return _ItemLUT[Pos + 1];
  
}
    



