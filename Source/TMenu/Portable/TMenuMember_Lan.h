/* ----------------------------------------------------------------------------
 *       TMenuMember的多国语言定义
 * --------------------------------------------------------------------------*/

#ifndef __T_MENU_MEMBER_LAN_H 
#define __T_MENU_MEMBER_LAN_H

//直接重定向为使用Language.h的定义：
#include "Language.h"

#define     LAN_COUNT        LANGUAGE_COUNT

//多国语言定义,若需实现此功能，则必须定义多国语言环境
#define eLan_t eLanguage_t
#define eEn eEnglish
#define eCh eChinese


//多国语言编码静态结构,即在程序存储器上实现的结构
#define LanCode_t LanguageCode_t


//----------------------判断字符串是否全角对齐函数-----------------
//未对齐时返回0
int Lan_IsCheckSBCCaseAlign(const char *pString,unsigned int Len);


/********************************************************************
                       回调函数
********************************************************************/
//-----------------------得到系统当前语种------------------------
//enum eLan_t GetLan(void); 
#include "GUI_Task.h"
#define GetLan() GetLanguage()


#endif

