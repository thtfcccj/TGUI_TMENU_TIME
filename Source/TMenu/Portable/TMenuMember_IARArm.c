/**************************************************************************
                        TMenu相关成员操作函数实现
  当没有定义TMENU_USE_CONST_BUF时相关函数的实现,否则用户需实现下列函数
  Menu_t结构中的使用指针的所有成员通过此函数获得相关信息
**************************************************************************/
#include "TMenuCfg.h"
#include "TMenu.h"
#include <string.h>

extern const TMenu_t MGetPower; //定义在code区的顶层菜单

#ifdef TM_EN_LUT
static unsigned char _ItemLUT[TM_ITEM_MAX + 1];//缓冲当前查找表

#ifdef TM_EN_DYNC_LUT  //动态查找表支持时
  static unsigned short _DyncLutSize; //动态查找表大小,项>=128时定义
  static unsigned short _DyncLutItem; //动态查找表项
#endif
  
#endif //TM_EN_LUT

//----------------------缓冲查找表----------------------------
#ifdef TM_EN_LUT
void _GetItemLUT(const TMenu_t *pMenu)
{
  if(!((pMenu)->Size & TM_TYPE_ITEM_LUT)) return;
  
  #ifdef TM_EN_DYNC_LUT  //动态查找表支持时
    _DyncLutSize = 0; //用户不响应时不支持
    TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_GET_DLUT_SIZE, &_DyncLutSize);
    if(_DyncLutSize != 0) return; //使用动态查找表获取数据
  #endif
   
  //从用户空间获得数据
  TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_GET_LUT,_ItemLUT);
}
#else
  #define _GetItemLUT(pMenu) do{ }while(0)
#endif

//----------------------------得到查找表值--------------------------------
#ifdef TM_EN_DYNC_LUT  //动态查找表支持时
unsigned short _GetLutItem(const TMenu_t *pMenu,
                            unsigned short Item)
{
  if(!(pMenu->Size & TM_TYPE_ITEM_LUT)) return Item;
  
  if(_DyncLutSize){ //当前为动态查找表时从用户空间获取数据
    _DyncLutItem = Item;
    TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_GET_DLUT_ITEM, &_DyncLutItem);
    return _DyncLutItem;
  }
  return _ItemLUT[Item + 1];
}
  #define _GetItemVol(pmenu, item) _GetLutItem(pmenu, item);
#else
  #define _GetItemVol(pmenu, item) _ItemLUT[item + 1];
#endif

//-----------------------得到顶层菜单--------------------------
const TMenu_t *TM_pGetTopMenu(void)
{
  //根据界面决定各顶层菜单的结构
  const TMenu_t *pTopMenu = TM_pcbGetUserTopMenu();
      
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
  
  if(pMenu != NULL) _GetItemLUT(pMenu); //没有退出菜单时， 缓存查找表
  return pMenu;
}

//-------------------------获得子菜单结构函数------------------------
const TMenu_t *TM_pGetSubMenu(const TMenu_t *pMenu,
                              unsigned char SubMenuID)
{
  TMenu_t const *const *PAry = (TMenu_t const *const *)pMenu->pv;  //C51的方式

  #ifdef TM_EN_LUT//有表时先查表
    SubMenuID = _GetItemVol(pMenu, SubMenuID);
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
    return pLanCodeToChar(pMenu->pgHeader);

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
#ifdef TM_EN_DYNC_LUT  //动态查找表支持时
  unsigned short TM_GetSize(const TMenu_t *pMenu)
#else
  unsigned char TM_GetSize(const TMenu_t *pMenu)
#endif
{
  #ifdef TM_EN_LUT  //有查找表时重定向到查找表大小:
  if(pMenu->Size & TM_TYPE_ITEM_LUT){
    #ifdef TM_EN_DYNC_LUT  //动态查找表支持时
      if(_DyncLutSize) return _DyncLutSize; //为动态查找表时
    #endif
    return _ItemLUT[0];
  }
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
    #ifdef TM_EN_RPC
      unsigned char PrvItem = Item;//表前
    #endif
    Item = _GetItemVol(pMenu, Item);
  #endif

  //RPC模式时,子菜单功能可能不完整, 先从用户空间获得子菜单头,若不能获取再继续
  #ifdef TM_EN_RPC   //从完整子菜单中获取 
    char *pBuf = TMenu_pGetBuf();
    *pBuf = Item;
    *(pBuf + 1) = '\0';//结束标志
    TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_SUB_HEADER, pBuf);
    if(*(pBuf + 1) != '\0') return pBuf; //从用户空间直接获得了
    //否则继续
    #ifdef TM_EN_LUT//有表时,再尝试用原始值查表
      *pBuf = PrvItem;
      TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_SUB_HEADER_ORG, pBuf);
      if(*(pBuf + 1) != '\0') return pBuf; //从用户空间直接获得了
    #endif //TM_EN_LUT
    //否则继续  
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
    Item = _GetItemVol(pMenu, Item);
  #endif

  if(pMenu->pv){
    #ifdef TM_DYNC_MENU   //动态菜单支持时通报更新子菜单
     if(pMenu->cbNotify)
      TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_UPDATE_SUB, &Item);
    #endif
    const LanCode_t *pCode = *((const LanCode_t **)(pMenu->pv) + Item);
    return pLanCodeToChar(pCode);
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
    



