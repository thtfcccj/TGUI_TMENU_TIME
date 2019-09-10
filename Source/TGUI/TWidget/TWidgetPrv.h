/*******************************************************************************

                           TGUI���ڻ�������֮�ڲ�˽�нӿ�

*******************************************************************************/
#ifndef __T_WIDGET_PRV_H 
#define __T_WIDGET_PRV_H
#include "TWidget.h" 

/**********************************************************************
                         �ڲ���Ա��������
**********************************************************************/
//��ѡ����һ,���ص������ѡ����
TItemSize_t TWidget_IncSel(TWidget_t *pWidget);
//��ѡ���һ,���ص������ѡ����
TItemSize_t TWidget_DecSel(TWidget_t *pWidget);
//�õ���x
#define TWidget_GetItemX(pWidget) ((pWidget)->ItemsRect.x)
//�õ���y
#define TWidget_GetItemY(pWidget) ((pWidget)->ItemsRect.x)

/**********************************************************************
                         �ڲ���Ϊ����
**********************************************************************/

//----------------------�õ���ʾ����x--------------------------------
unsigned char TWidget_GetDispX(const TWin_t *pWin,
                               unsigned char Flag);

//----------------------�õ���ʾ������----------------------------
unsigned char TWidget_GetDispW(const TWin_t *pWin,
                               unsigned char Flag);

//----------------------�õ���ʾ����y--------------------------------
unsigned char TWidget_GetDispY(const TWin_t *pWin,
                               unsigned char Flag);

//----------------------�õ���ʾ����߶�--------------------------------
unsigned char TWidget_GetDispH(const TWin_t *pWin,
                               unsigned char Flag);


#endif //#ifndef __T_WIDGET_PRV_H 

