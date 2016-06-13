/**************************************************************************
                        TMenu相关成员操作函数实现
  当没有定义TMENU_USE_CONST_BUF时相关函数的实现,否则用户需实现下列函数
  Menu_t结构中的使用指针的所有成员通过此函数获得相关信息
**************************************************************************/
#include "TMenuCfg.h"
#include "TMenu.h"
#include "string.h"

extern code TMenu_t MGetPower; //定义在code区的顶层菜单

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
const TMenu_t *TM_pGetTopMenu(void)
{
  _GetItemLUT(&MGetPower); //缓存查找表
  return &MGetPower;      //指向缓存
}

//---------------------得到父菜单结构函数----------------------------
const TMenu_t *TM_pGetParent(const TMenu_t *pMenu)
{
  pMenu = pMenu->pParent;
  _GetItemLUT(pMenu); //缓存查找表
  return pMenu;
}

//-------------------------获得子菜单结构函数------------------------
const TMenu_t *TM_pGetSubMenu(const TMenu_t *pMenu,
                              unsigned char SubMenuID)
{
  TMenu_t code *code *PAry = (TMenu_t code *code *)pMenu->pv;  //C51的方式

  #ifdef TM_EN_LUT//有表时先查表
  if(pMenu->Size & TM_TYPE_ITEM_LUT) SubMenuID = _ItemLUT[SubMenuID + 1];
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
    return pMenu->pgHeader->pLan[GetLan()];

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
  TMenu_t code *code *PAry = (TMenu_t code *code *)pMenu->pv;  //C51的方式

  #ifdef TM_EN_LUT//有表时先查表
  if(pMenu->Size & TM_TYPE_ITEM_LUT) Item = _ItemLUT[Item + 1];
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

  if(pMenu->pv)
    return (((const LanCode_t *)pMenu->pv) + Item)->pLan[GetLan()];

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
  TItemSize_t ItemW = 0,CurW;
  if((pMenu->Type & TMTYPE_MASK) == TMTYPE_SUBMENU){//子菜单模式从子菜单读取长度
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



