/**************************************************************************
             TMenu相关成员操作函数在AVR GCC中的实现相关
//在原有默认结构的基础上,增加了强制多国语言支持

//固AVR GCC不支持代码区指针操作,故使用缓存实现
//所有的指针均使用ID号表示
**************************************************************************/

#ifndef __T_MENU_MEMBER_AVRGCC
#define __T_MENU_MEMBER_AVRGCC


#include "TMenuMember_Lan.h"        //强制多国语言支持

#include <avr/pgmspace.h>	//将变量定义在Flash空间
#define code PROGMEM



//定义菜单结构存储ID号结构,pvID与菜单结构存储ID号一样多
typedef unsigned char TMenuMemSize_t; 
//定义字符串存储ID号结构
typedef unsigned char TStringSize_t; 


//在AVRGCC中时,将TMenu_t定义为下列结构
struct _TMenu{
  unsigned char Type;      //菜单类型及相关标志
  unsigned char Size;       //由菜单类型决定的相关数据大小
  TStringSize_t HeaderID[LAN_COUNT]; //多国语言支持的菜单头字附串ID号,下标对应语种
  TMenuMemSize_t ParentID ;      //自已的父菜单ID号
  TMenuMemSize_t pvAryID;      //存储pv阵列的ID号(pvAryID):
  TMenuNotify_t cbNotify;     //与用户空间交互的通报函数
};


/********************************************************************
             由各自的ID号得到相应结构指针回调函数 
********************************************************************/
//从结构看,共使用下列种ID号,用户需实现由ID号得到相应的指向程序存储区
//相应存储位置的函数(因指针无法保存,该函数实现时只能用case)

//------------------------字符串ID号-------------------------
//字符串ID号用于存储菜单头及pvAry(见下)里的菜单项里的多国语言
//在不同语种下的字符串,操作函数为:

//由字符串ID号得到字符串头(即在程序存储区)：
const char *cbTM_pGetString(TStringSize_t StringID);

//-------------------菜单结构ID号-------------------------
//用于菜单结构,操作函数为:

//由菜单ID号得到菜单结构头(即在程序存储区)
const struct _TMenu *cbTM_pGet(TMenuMemSize_t TMenuID);

//----------------------pv结构ID号----------------------------
//pv结构指向pv阵列ID号的头，操作函数为

//由pvID号得到pv阵列头(即在程序存储区)
const unsigned char *cbTM_pGetPvAry(TMenuMemSize_t pvID);

//----------------------pv结构阵列ID号--------------------------
//pv阵列里存储的ID号根据当前菜单类型不同而不同,当为子菜单时
//为菜单结构ID号,否则一般为菜单项的多国语言
//字符串阵列(每一菜单项LAN_COUNT个字符串ID)


//得到pv阵列内相应下标ID号后,
//通过cbTM_pGetString即可得狡銲D号

//由pv阵列内相应下标ID号得到菜单结构ID号后,
//通过cbTM_pGet即可得到菜单结构头

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


