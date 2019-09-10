/**************************************************************************

             TMenuMember支持多国语言选择时定义

**************************************************************************/
#ifndef __T_MENU_MEMBER_MUTI_LAN_H 
#define __T_MENU_MEMBER_MUTI_LAN_H

//------------------------------相关类型定义-------------------------
//直接重定向为使用Language.h的定义：
#include "Language.h"

#define     LAN_COUNT        LANGUAGE_COUNT

//多国语言定义,若需实现此功能，则必须定义多国语言环境
#define eLan_t eLanguage_t
#define eEn eEnglish
#define eCh eChinese


//多国语言编码静态结构,即在程序存储器上实现的结构
#define LanCode_t LanguageCode_t

//--------------------由LanCode_t结构得到字符串----------------------
//const char *pLanCodeToChar(LanCode_t *pLanCode)
#include "GUI_Task.h"
#define pLanCodeToChar(lanCode) ((const char*)((lanCode)->pLan[GetLanguage()]))

#endif

