/**************************************************************************
             TMenu相关成员操作函数在AVR GCC中的实现
**************************************************************************/
//固AVR GCC不支持代码区指针操作,故使用缓存实现

#include "TMenuCfg.h"
#include "TMenu.h"

#include "string.h"

//后加，得到成员偏移
#define struct_offset(type, member) ((unsigned char)(&((type *)0)->member))      


//#define memcpy_P(pv,pv1,size) memcpy(pv,pv1,size) //测试
//#define strcpy_P(pv,pv1) strcpy(pv,pv1) //测试

#define   TMENU_MEM_ID_NULL    0   //无菜单项时固定为0
#define   TMENU_MEM_STRING_ID_NULL    0   //字符串ID为0表示为空

//----------------------缓冲查找表----------------------------
#ifdef TM_EN_LUT
static unsigned char _ItemLUT[TM_ITEM_MAX + 1];//缓冲当前查找表
#endif

#ifdef TM_EN_LUT
#define _GetItemLUT(pMenu) \
do{\
  if((pMenu)->Size & TM_TYPE_ITEM_LUT)\
    TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_GET_LUT,_ItemLUT);\
}while(0)

#else
#define _GetItemLUT(pMenu) do{ }while(0)
#endif



/**********************************************************************
                       AVR GCC菜单缓存加载
**********************************************************************/


static TMenu_t _MenuBuf;              //当前菜单结构缓冲
static char _StringBuf[TLCD_WIDTH + 10];  //一行的字符串缓冲

//--------------------得到指定ID号的菜单结构-------------------
static const TMenu_t *_pGetTMenu(TMenuMemSize_t TMenuID)
{
  memcpy_P(&_MenuBuf,cbTM_pGet(TMenuID),
             sizeof(TMenu_t));  //读内存结构
  _GetItemLUT(&_MenuBuf); //缓存查找表
  return &_MenuBuf;
}

//--------------------得到子菜单存储结构ID号-------------------
static TMenuMemSize_t _GetSubMenuID(unsigned char Item)
{
  TMenuMemSize_t TMenuID;
  //得到在程序存储区的阵列头
  const unsigned char *pPvAry = 
    (const unsigned char *)cbTM_pGetPvAry(_MenuBuf.pvAryID);                 
  //找到当前项位置
  pPvAry += Item * sizeof(TMenuMemSize_t);
  //读取菜单存储结构ID号
  memcpy_P(&TMenuID,pPvAry,sizeof(TMenuMemSize_t));
  return TMenuID;
}

//--------------------得到顶层菜单结构--------------------------
//当存在多个主菜单时，可修改此函数根据系统应用跳转不同的主菜单
#include "AKeyState.h"
const TMenu_t *TM_pGetTopMenu(void)
{
  //if((ePKState == PKS_PManual) || (ePKState == PKS_KManual)) return _pGetTMenu(11);
  return _pGetTMenu(1);
}

//---------------------得到父菜单结构函数----------------------------
const TMenu_t *TM_pGetParent(const TMenu_t *pMenu)
{
  pMenu = pMenu;//防止编译警告
  if(!_MenuBuf.ParentID) return NULL;//到顶了
	return _pGetTMenu(_MenuBuf.ParentID);
}

//-------------------从菜单结构获得子菜单结构函数-------------------
const TMenu_t *TM_pGetSubMenu(const TMenu_t *pMenu,
                               unsigned char SubMenuID)
{
  pMenu = pMenu;//防止编译警告
  
  #ifdef TM_EN_LUT//有表时先查表
  if(pMenu->Size & TM_TYPE_ITEM_LUT) SubMenuID = _ItemLUT[SubMenuID + 1];
  #endif
  
  return _pGetTMenu(_GetSubMenuID(SubMenuID));
}


//-------------------------获得指定菜单结构函数------------------------
const TMenu_t *TM_pGetMenu(const TMenu_t *pMenu,   //强制跳转的菜单结构
                           unsigned short MenuID) //自由参数,这里为菜单的ID号
{
  pMenu = pMenu;
  return _pGetTMenu(MenuID); 
}

//-------------------从菜单结构获得菜单头函数-------------------
const char *TM_GetHeader(const TMenu_t *pMenu)
{
  TStringSize_t StringID = _MenuBuf.HeaderID[GetLan()];
  
  pMenu = pMenu;//防止编译警告
  if(!StringID){//字符为空时,从用户空间获取:
    TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_HEADER,_StringBuf);
    return _StringBuf;
  }
  //从字符串中读取
  strcpy_P(_StringBuf,cbTM_pGetString(StringID));
  return _StringBuf;
}

//-------------------从菜单结构获得类型函数------------------------
unsigned char TM_GetType(const TMenu_t *pMenu)
{
  pMenu = pMenu;
  return _MenuBuf.Type;
}

//-------------------从菜单结构获得大小函数------------------------
unsigned char TM_GetSize(const TMenu_t *pMenu)
{
  pMenu = pMenu;
  
  #ifdef TM_EN_LUT  //有查找表时重定向到查找表大小:
  if(_MenuBuf.Size & TM_TYPE_ITEM_LUT) return _ItemLUT[0];
  #endif
  
  return _MenuBuf.Size; 
}

//----------------得到指定子菜单项头函数-------------------
const char *TM_GetSubMenuHeader(const TMenu_t *pMenu,
                                unsigned char Item)
{
  TStringSize_t StringID;
  const unsigned char *pPos;
  TMenuNotify_t cbCurNotify;
  
  #ifdef TM_EN_LUT//有表时先查表
  if(pMenu->Size & TM_TYPE_ITEM_LUT) Item = _ItemLUT[Item + 1];
  #endif
  
  //得到子菜单存储位置
  const TMenu_t *pTMenuMem = cbTM_pGet(_GetSubMenuID(Item));
  //得到HeaderID所在项起始位置
  pPos = (unsigned char*)pTMenuMem + struct_offset(TMenu_t,HeaderID);
  //得到当前语种对应位置
  pPos += GetLan() * sizeof(TStringSize_t);
  //得到字符串ID
  memcpy_P(&StringID,pPos,sizeof(TStringSize_t));

  if(!StringID){//从用户空间获取
    pPos = (unsigned char*)pTMenuMem + struct_offset(TMenu_t,cbNotify);
    memcpy_P(&cbCurNotify,pPos,sizeof(TMenuNotify_t));
    TMENU_NOTIFY_RUN(cbCurNotify,TM_NOTIFY_GET_HEADER,_StringBuf);
  }
  //得到字符串
  else{
    strcpy_P(_StringBuf,cbTM_pGetString(StringID));
    pMenu = pMenu;//防止编译警告
  }
  return _StringBuf;
}

//-------------------从菜单结构获得项字符串函数-------------------
const char *TM_GetItemString(const TMenu_t *pMenu,
                             unsigned char Item)
{
  TStringSize_t StringID;
  
  #ifdef TM_EN_LUT  //有查找表时重定向到查找表
  if(pMenu->Size & TM_TYPE_ITEM_LUT) Item = _ItemLUT[Item + 1];
  #endif
  
  //得到在程序存储区的阵列头
  const unsigned char *pPvAry = 
    (const unsigned char *)cbTM_pGetPvAry(_MenuBuf.pvAryID); 
  //找到当前项位置
  pPvAry += Item * (LAN_COUNT * sizeof(TStringSize_t));
  //读取字符串ID号对应字符
  memcpy_P(&StringID,pPvAry,sizeof(TStringSize_t));
  //得到字符串
  strcpy_P(_StringBuf,cbTM_pGetString(StringID));
  pMenu = pMenu;//防止编译警告

  return _StringBuf;
}

//-------------------统计菜单项最大宽度函数-----------------------
TItemSize_t TM_GetItemMaxLen(const TMenu_t *pMenu)
{
  unsigned char Size = pMenu->Size;
  unsigned char i;
  TItemSize_t ItemW = 0,CurW;

  //当前菜单为子菜单类型时，子菜单头即是当前项
  if((pMenu->Type & TMTYPE_MASK) == TMTYPE_SUBMENU){
    for(i = 0; i < Size; i++){
      CurW = strlen(TM_GetSubMenuHeader(pMenu,i));
      if(CurW > ItemW) ItemW = CurW;
    }
  }
  //其它直接就是字符中结构
  else{
    for(i = 0; i < Size; i++){
      CurW = strlen(TM_GetItemString(pMenu,i));
      if(CurW > ItemW) ItemW = CurW;
    }
  } 
  return ItemW;
}









