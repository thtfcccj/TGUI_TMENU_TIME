/*****************************************************************************
                         TGUI�����뷨ģ��Ļ���������ṹ
*****************************************************************************/

#ifndef __TIME_BASE_H
#define __TIME_BASE_H


//����༭�ַ����ַ����������ͣ�
//unsigned char ʱ�ɱ༭<255,unsigned shoft ʱ�ɱ༭<32768
typedef unsigned char TIme_String_t;

//----------------�ڲ�ʹ�õĵ�����ֵ(GuideKey)����(0-5)------------------
//˳���ѹ̶�
#define TIME_MNG_KEY_UP       0
#define TIME_MNG_KEY_LEFT     1
#define TIME_MNG_KEY_ENTER    2
#define TIME_MNG_KEY_RIGHT    3
#define TIME_MNG_KEY_DOWN     4
#define TIME_MNG_KEY_EXIT     5
#define TIME_MNG_KEY_COUNT    6

//---------------------------�����ַ���-----------------------------------
extern const char chReturn[];// =  {"����"};

//-------------------------GuideKey����ȷ�ϼ�ʱ�ļ�ֵ---------------------------
extern const unsigned char TIme_GuideKeyToNoEnterKey[];



#endif //#define __TIME_BASE_H

