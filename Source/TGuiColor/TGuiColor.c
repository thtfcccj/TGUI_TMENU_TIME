/*******************************************************************************

                     TGUI��ɫʵ��
��ģ�����Lcd_Drv/TFT�ص�����ʵ���ı����߿����ɫ����
*******************************************************************************/
#include "TGuiColor.h"
#include <string.h>

struct _TGuiColor TGuiColor;

/****************************************************************************
							             ��Ҫ����ʵ��																 
****************************************************************************/

//--------------------------------��ʼ������----------------------------------
//����ʱ����
void TGuiColor_Init(void)
{
  //��ʼ���ṹ
  memset(&TGuiColor, 0, sizeof(struct _TGuiColor));
  //��ɫ����Ĭ��
  memset(&TGuiColor.MainPenColor,TGUI_COLOR_DEFAULT_PEN_DATA, 
         sizeof(TGuiColor.MainPenColor));
  memset(&TGuiColor.MainPenColor, TGUI_COLOR_DEFAULT_PEN_TITLE, //������ 
         TLCD_WIDTH);  
  memset(&TGuiColor.MainBrushColor, TGUI_COLOR_DEFAULT_BRUSH_DATA, 
         sizeof(TGuiColor.MainPenColor));
  memset(&TGuiColor.MainBrushColor, TGUI_COLOR_DEFAULT_BRUSH_TITLE, 
         TLCD_WIDTH);  
}


//-------------------------------����������������ɫ����--------------------------
void TGuiColor_SetPenColor(unsigned char y,
                        unsigned char x,
                        unsigned char xLen,
                        Color_t Color)
{
  memset(&TGuiColor.MainPenColor[y][x], Color, xLen);
}

//-------------------------------�����������Ӧ��ɫ����--------------------------
void TGuiColor_SetPenColorP(unsigned char y,
                        unsigned char x,
                        unsigned char xLen,
                        const Color_t *pColor)
{
  memcpy(&TGuiColor.MainPenColor[y][x], pColor,xLen);  
}

/****************************************************************************
							             Lcd_Drv�ص�����ʵ��																 
****************************************************************************/

//-------------------------�����Ƿ��������----------------------------------
//�����ڲ���ģʽ����ĳ�����ض�����µ���������;ʹ��
RowMask_t LCD_Drv_cbLineIsEn(unsigned short y)
{
  return TGuiColor.LcdLineEnMask & RowShift2Mask(y);
}

//-------------------------�õ�ָ��λ��������ɫ��Ϣ��--------------------------
//ÿ��Color_t��ʾ��Ӧλ����ʼ�ֵ���ɫ�������ڶ���ʾ�ֽ�����ɫ
const Color_t *LCD_Drv_pcbGetPenColor(unsigned char x,      //��Ҫ���µ�����ʼλ��
                                      unsigned char y)      //��Ҫ���µ���
{
  return &TGuiColor.MainPenColor[y][x];
}

//--------------------------�õ�ָ��λ�ñ�����ɫ��Ϣ��--------------------------
//ÿ��Color_t��ʾ��Ӧλ����ʼ�ֵı�����ɫ�������ڶ���ʾ�ֽ�����ɫ
const Color_t *LCD_Drv_pcbGetBrushColor(unsigned char x,      //��Ҫ���µ�����ʼλ��
                                        unsigned char y)      //��Ҫ���µ���
{
  return &TGuiColor.MainBrushColor[y][x];
}

//-------------------------------�õ�����ʱ�ı���ɫ-----------------------------
//����λ�õı���ɫ���Դ�Ϊ��׼
Color_t LCD_Drv_cbGetForcusBrushColor(void)
{
  return TGUI_COLOR_DEFAULT_BRUSH_FOCUS;
}





