/**************************************************************************

             TMenuMember�Ķ������ѡ��֧��ģ��

**************************************************************************/
#ifndef __T_MENU_MEMBER_LAN_H 
#define __T_MENU_MEMBER_LAN_H

#include "TMenuCfg.h"

#ifdef TM_EN_MUTI_LAN            //����ѡ���Ͷ������ʱ
  #include "TMenuMember_MutiLan.h"
#elif defined(TM_RESOURCE_LAN)  //����Դ�ļ���ȡ�������ʱ
  #include "TMenuMember_ResourceLan.h"
#else  //��һ�����ַ���ʱ

/**************************************************************************
                       ��֧�ֶ������ʱ����
**************************************************************************/

//--------------------------������Ͷ���------------------------
#define LanCode_t char

//--------------------��LanCode_t�ṹ�õ��ַ���----------------------
//const char *pLanCodeToChar(LanCode_t *pLanCode)
#define pLanCodeToChar(lanCode) lanCode


#endif //#ifndef SUPPORT_MUTI_LAN 
#endif

