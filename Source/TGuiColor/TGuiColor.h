/*******************************************************************************

                     TGUI��ɫʵ��
��ģ�����Lcd_Drv/TFT�ص�����ʵ���ı����߿����ɫ����
*******************************************************************************/

#ifndef __TGUI_COLOR_H
#define __TGUI_COLOR_H

/*******************************************************************************
                            �������
*******************************************************************************/

#include "TGUI.h"
#include "Color.h" //ComGui�ֿ��ﶨ��

//-----------------------------��ɫ����------------------------------

//Ĭ���б�����ǰ��ɫ
#ifndef TGUI_COLOR_DEFAULT_PEN_DATA
  #define   TGUI_COLOR_DEFAULT_PEN_DATA      RGBFFFFFF  //��ɫ
#endif
//Ĭ���б����ǰ��ɫ
#ifndef TGUI_COLOR_DEFAULT_PEN_TITLE
  #define   TGUI_COLOR_DEFAULT_PEN_TITLE     RGBFFFFFF  //��ɫ
#endif
//Ĭ���б����ݱ���ɫ
#ifndef TGUI_COLOR_DEFAULT_BRUSH_DATA
  #define   TGUI_COLOR_DEFAULT_BRUSH_DATA    RGB000000   //��ɫ
#endif
//Ĭ���б���ⱳ��ɫ
#ifndef TGUI_COLOR_DEFAULT_BRUSH_TITLE
  #define   TGUI_COLOR_DEFAULT_BRUSH_TITLE   RGB333333   //���ɫ
#endif
//����������ɫ
#ifndef TGUI_COLOR_DEFAULT_BRUSH_FOCUS
  #define   TGUI_COLOR_DEFAULT_BRUSH_FOCUS   RGB3333FF   //ƫ��ɫ
#endif
/*******************************************************************************
                            ��ؽṹ
*******************************************************************************/

//������ʱ���ռ����ṹ
struct _TGuiColor{
  //������ɫ���ƻ�����:
  Color_t MainPenColor[TLCD_HIGH][TLCD_WIDTH];      //����ʾǰ��ɫ����
  Color_t MainBrushColor[TLCD_HIGH][TLCD_WIDTH];    //����ʾ����ɫ����
  RowMask_t LcdLineEnMask;   //TGUI�У���ʾ���и�����������  
};

extern struct _TGuiColor TGuiColor;

/*******************************************************************************
                              ��غ���
*******************************************************************************/

//--------------------------------��ʼ������----------------------------------
//����ʱ����
void TGuiColor_Init(unsigned char TitleY);//Ĭ�ϱ�������λ��,255��ʾ��

//--------------------------------��ΪĬ�Ϻ���----------------------------------
//�ָ�Ĭ����ɫ
void TGuiColor_SetDefault(unsigned char TitleY);

//---------------------------�������ֹTGUI��������ʾ--------------------------
#define TGuiColor_EnTGuiDispAll() do{TGuiColor.LcdLineEnMask = (RowMask_t)-1;}while(0)
#define TGuiColor_DisTGuiDispAll() do{TGuiColor.LcdLineEnMask = 0;}while(0)

//---------------------------�������ֹTGUI�Ĳ�����ʾ--------------------------
#define TGuiColor_EnTGuiDisp(mask)  do{TGuiColor.LcdLineEnMask |= (mask);}while(0)
#define TGuiColor_DisTGuiDisp(mask) do{TGuiColor.LcdLineEnMask &= ~(mask);}while(0)

//-------------------------------����������������ɫ����--------------------------
void TGuiColor_SetPenColor(unsigned char y,
                        unsigned char x,
                        unsigned char xLen,
                        Color_t Color);

//-------------------------------�����������Ӧ��ɫ����--------------------------
void TGuiColor_SetPenColorP(unsigned char y,
                        unsigned char x,
                        unsigned char xLen,
                        const Color_t *pColor);


#endif //__TGUI_COLOR_H