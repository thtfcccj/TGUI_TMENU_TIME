/**************************************************************************
             TMenu结构实现及相关成员操作函数在KeilC51中的实现相关
//在原有默认结构的基础上,增加了强制多国语言支持与可选的项查找表支持
**************************************************************************/

#ifndef __T_MENU_MEMBER_KEIL_C51
#define __T_MENU_MEMBER_KEIL_C51

#include "TMenuMember_Lan.h"        //强制多国语言支持

//根据应用,定义菜单类型如下:
struct _TMenu{
  unsigned char Type;      //菜单类型及相关标志
  unsigned char Size;       //由菜单类型决定的相关数据大小
  const LanCode_t *pgHeader; //菜单头,为NULL时从回调里读取
  const struct _TMenu *pParent;   //自已的父菜单
  const void *pv;           //根据菜单类型确定的自由参数指针
  TMenuNotify_t cbNotify;     //与用户空间交互的通报函数
};

/********************************************************************
                       可选的项查找表结构支持
********************************************************************/

//为支持项内容根据系统登录用户权限决定是否显示的功能,允许增加项查找表支持
#define   TM_EN_LUT     //是否支持查找表(内部实现使用)

#ifdef TM_EN_LUT   //查找表支持时:
  //复用Size结构最高位表示该结构附有项查找表
  #define   TM_TYPE_ITEM_LUT     0x80
  
  //当有项查找表时,用户程序增加响应下列用户通报获取查找表结构(填充传入的pv指针)
  #define    TM_NOTIFY_USER_GET_LUT    128
  //查找表结构定义为一个数组:其中
  //首位: 此查找表大小,<= struct _TMenu中的Size
  //后跟此查大小个数据,如
  //static code const unsigned char _LUT = {2,  0,3}; 
  
  //定义项最大个数,用内部于将查找表缓冲
  #define   TM_ITEM_MAX         10
#endif

#endif


