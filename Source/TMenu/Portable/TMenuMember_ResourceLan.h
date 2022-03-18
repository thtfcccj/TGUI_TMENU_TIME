/**************************************************************************

          TMenuMember 由资源文件决定的多国语言选择时定义

**************************************************************************/
#ifndef __T_MENU_MEMBER_RESOURCE_LAN_H 
#define __T_MENU_MEMBER_RESOURCE_LAN_H

//--------------------------相关类型定义------------------------
//存储的是ID号查找数组： [0]为系统分配的大类，[1]为内部小类
#define LanCode_t  unsigned char

//----------------------由LanCode_t结构得到字符串---------------
//此函数交由用户实现
const char *pLanCodeToChar(const LanCode_t *pLanCode);

#endif

