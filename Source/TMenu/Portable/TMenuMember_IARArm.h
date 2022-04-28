/**************************************************************************
             TMenu结构实现及相关成员操作函数在KeilC51中的实现相关
//在原有默认结构的基础上,增加了强制多国语言支持与可选的项查找表支持
**************************************************************************/
#ifndef __T_MENU_MEMBER_IAR_ARM
#define __T_MENU_MEMBER_IAR_ARM

#include "TMenuCfg.h" 
#include "TMenuMember_Lan.h" 

//根据应用,定义菜单类型如下:
struct _TMenu{
  unsigned char Type;      //菜单类型及相关标志
  unsigned char Size;       //由菜单类型决定的相关数据大小
  const LanCode_t *pgHeader; //菜单头,为NULL时从回调里读取
  const struct _TMenu *pParent;   //自已的父菜单
  const void *pv;           //根据菜单类型确定的自由参数指针
  TMenuNotify_t cbNotify;     //与用户空间交互的通报函数
};

//为只读EditSel时,只需要菜单头即可：
struct _TMenuSelRO{
  unsigned char Type;      //菜单类型及相关标志
  unsigned char Size;       //由菜单类型决定的相关数据大小
  const LanCode_t *pgHeader; //菜单头,为NULL时从回调里读取  
};

/********************************************************************
                       可选的项查找表结构支持
********************************************************************/

#ifdef TM_EN_LUT   //查找表支持时:

  //复用Size结构最高位表示该结构附有项查找表
  #define   TM_TYPE_ITEM_LUT     0x80
  
  //查找表结构定义为一个数组:其中
  //首位: 此查找表大小,<= struct _TMenu中的Size
  //后跟此查大小个数据,如
  //static code const unsigned char _LUT = {2,  0,3};

  //当有项查找表时,用户程序增加响应下列用户通报获取查找表结构(填充传入的pv指针)
  //TM_TYPE_ITEM_LUT置位时，必须实现
  #define  TM_NOTIFY_USER_GET_LUT    (TM_NOTIFY_USER_BASE + 0)

  //定义项最大个数,用内部于将查找表缓冲
  #ifndef TM_ITEM_MAX
    #define   TM_ITEM_MAX         127 //最大支持,超过需启用动态查找表
  #endif

  //-------------------根据当前菜单查找表获得所选位置---------------------
  unsigned char TMenu_GetItemPosWithLUT(unsigned char Pos);

  //------------------------动态查找表支持-------------------------------
  //当项数 >=128时，用此项可实现1~65535项的支持
  #ifdef TM_EN_DYNC_LUT  //动态查找表支持时
    //获取动态查找有大小，响应此函数返回非0时，使用动态表获取参数
    #define  TM_NOTIFY_USER_GET_DLUT_SIZE    (TM_NOTIFY_USER_BASE + 1) 
    //获取动态查找表项，输入为当前项位置，输出为目标值,不响应时相当于无查找表
    #define  TM_NOTIFY_USER_GET_DLUT_ITEM    (TM_NOTIFY_USER_BASE + 2)
  #endif
#endif //#ifdef TM_EN_LUT


/********************************************************************
                       可选的动态菜单创建支持
********************************************************************/

#ifdef TM_DYNC_MENU   //动态菜单支持时:  
  
  //更新父菜单通报 更新父菜单结构(pv指针无用)
  #define    TM_NOTIFY_USER_UPDATE_PARENT    (TM_NOTIFY_USER_BASE + 1)

  //更新子菜单通报 更新指字子菜单结构(pv指针指向当前子菜单的ID号)
  #define    TM_NOTIFY_USER_UPDATE_SUB    (TM_NOTIFY_USER_BASE + 2)
  
#endif //#ifdef TM_DYNC_MENU

/********************************************************************
                       应用层回调函数
********************************************************************/

//-----------------------得到用户顶层菜单结构---------------------------
const TMenu_t *TM_pcbGetUserTopMenu(void);
  

#endif //__T_MENU_MEMBER_IAR_ARM


