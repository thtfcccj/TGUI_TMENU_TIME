/***************************************************************************

                 TGUI������������ز�������
��ģ������ַ���ʾ������Lcd_pGetRowBuffer()
��������"Lcd_"��ͷ
****************************************************************************/
#ifndef __T_MAIN_H
#define __T_MAIN_H

#include "TGUI.h"
#include "TMenuCfg.h"

//---------------------------------�õ��л�����------------------------------
#define  Lcd_pGetRowBuffer(y)  \
  (TWin_pGetString(TWM_pGetWin(TMENU_FIX_WIN_COUNT),y))

//-------------------------------�õ�ĳλ�û�����------------------------------
#define  Lcd_pGetPosBuffer(y,x)  (Lcd_pGetRowBuffer(y) + (x))

//-----------------------------------���һ��---------------------------------
void Lcd_ClrRowBuffer(unsigned char y);

//----------------------------------����ĳ�и���------------------------------
#define  Lcd_SetUpdateRow(y)  do{ }while(0)

//---------------------------------���û�������ʾ-----------------------------
void Lcd_SetBuffer(unsigned char y,
              unsigned char x,
              const char *pString,
              unsigned char StringSize);

//---------------------------��ĳЩ���»���--------------------------------
//�ݲ�֧��
#define  Lcd_SetDownLine(y) do{}while(0)

//---------------------------��ĳ���з������--------------------------------
//�ݲ�֧��
#define Lcd_SetNega(y) do{}while(0)


#endif //__T_MAIN_H
