/**************************************************************************

             TMenuMember֧�ֶ������ѡ��ʱ����

**************************************************************************/
#ifndef __T_MENU_MEMBER_MUTI_LAN_H 
#define __T_MENU_MEMBER_MUTI_LAN_H

//------------------------------������Ͷ���-------------------------
//ֱ���ض���Ϊʹ��Language.h�Ķ��壺
#include "Language.h"

#define     LAN_COUNT        LANGUAGE_COUNT

//������Զ���,����ʵ�ִ˹��ܣ�����붨�������Ի���
#define eLan_t eLanguage_t
#define eEn eEnglish
#define eCh eChinese


//������Ա��뾲̬�ṹ,���ڳ���洢����ʵ�ֵĽṹ
#define LanCode_t LanguageCode_t

//--------------------��LanCode_t�ṹ�õ��ַ���----------------------
//const char *pLanCodeToChar(LanCode_t *pLanCode)
#include "GUI_Task.h"
#define pLanCodeToChar(lanCode) ((const char*)((lanCode)->pLan[GetLanguage()]))

#endif

