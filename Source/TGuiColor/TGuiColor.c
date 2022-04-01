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
void TGuiColor_Init(unsigned char TitleY)//Ĭ�ϱ�������λ��,255��ʾ��
{
  //��ʼ���ṹ
  memset(&TGuiColor, 0, sizeof(struct _TGuiColor));
  TGuiColor_SetDefault(TitleY);//�ָ�Ĭ����ɫ
}

//--------------------------------��ΪĬ�Ϻ���----------------------------------
//�ָ�Ĭ����ɫ
void TGuiColor_SetDefault(unsigned char TitleY)
{
  //��ɫ����Ĭ��
  memset(&TGuiColor.MainPenColor,TGUI_COLOR_DEFAULT_PEN_DATA, 
         sizeof(TGuiColor.MainPenColor));
  memset(&TGuiColor.MainBrushColor, TGUI_COLOR_DEFAULT_BRUSH_DATA, 
         sizeof(TGuiColor.MainPenColor));  
  //������
  if(TitleY < TLCD_HIGH){
    memset(&TGuiColor.MainPenColor[TitleY], TGUI_COLOR_DEFAULT_PEN_TITLE, 
           TLCD_WIDTH);  //Color_t=1byteʱ
    memset(&TGuiColor.MainBrushColor[TitleY], TGUI_COLOR_DEFAULT_BRUSH_TITLE,
           TLCD_WIDTH);//Color_t=1byteʱ
  }
}

//-------------------------------����������������ɫ����-------------------------
void TGuiColor_SetPenColor(unsigned char y,
                        unsigned char x,
                        unsigned char xLen,
                        Color_t Color)
{
  Color_Full(Color, &TGuiColor.MainPenColor[y][x], xLen);  
}

//-------------------------------�����������Ӧ��ɫ����-------------------------
void TGuiColor_SetPenColorP(unsigned char y,
                        unsigned char x,
                        unsigned char xLen,
                        const Color_t *pColor)
{
  Color_Copy(&TGuiColor.MainPenColor[y][x], pColor, xLen); 
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





