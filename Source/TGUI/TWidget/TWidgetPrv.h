/*******************************************************************************

                           TGUI窗口基本部件之内部私有接口

*******************************************************************************/
#ifndef __T_WIDGET_PRV_H 
#define __T_WIDGET_PRV_H
#include "TWidget.h" 

/**********************************************************************
                         内部成员操作函数
**********************************************************************/
//所选项增一,返回调整后的选择项
TItemSize_t TWidget_IncSel(TWidget_t *pWidget);
//所选项减一,返回调整后的选择项
TItemSize_t TWidget_DecSel(TWidget_t *pWidget);
//得到项x
#define TWidget_GetItemX(pWidget) ((pWidget)->ItemsRect.x)
//得到项y
#define TWidget_GetItemY(pWidget) ((pWidget)->ItemsRect.x)

/**********************************************************************
                         内部行为函数
**********************************************************************/

//----------------------得到显示区域x--------------------------------
unsigned char TWidget_GetDispX(const TWin_t *pWin,
                               unsigned char Flag);

//----------------------得到显示区域宽度----------------------------
unsigned char TWidget_GetDispW(const TWin_t *pWin,
                               unsigned char Flag);

//----------------------得到显示区域y--------------------------------
unsigned char TWidget_GetDispY(const TWin_t *pWin,
                               unsigned char Flag);

//----------------------得到显示区域高度--------------------------------
unsigned char TWidget_GetDispH(const TWin_t *pWin,
                               unsigned char Flag);


#endif //#ifndef __T_WIDGET_PRV_H 

