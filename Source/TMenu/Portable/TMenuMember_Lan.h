/**************************************************************************

             TMenuMember的多国语言选择支持模块

**************************************************************************/
#ifndef __T_MENU_MEMBER_LAN_H 
#define __T_MENU_MEMBER_LAN_H

#include "TMenuCfg.h"

#ifdef TM_EN_MUTI_LAN            //常量选择型多国语言时
  #include "TMenuMember_MutiLan.h"
#elif defined(TM_RESOURCE_LAN)  //由资源文件获取多国语言时
  #include "TMenuMember_ResourceLan.h"
#else  //单一语言字符串时

/**************************************************************************
                       不支持多国语言时定义
**************************************************************************/

//--------------------------相关类型定义------------------------
#define LanCode_t char

//--------------------由LanCode_t结构得到字符串----------------------
//const char *pLanCodeToChar(LanCode_t *pLanCode)
#define pLanCodeToChar(lanCode) lanCode


#endif //#ifndef SUPPORT_MUTI_LAN 
#endif

