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
  Color_Full(TGUI_COLOR_DEFAULT_PEN_DATA,
             TGuiColor.MainPenColor[0],
             sizeof(TGuiColor.MainPenColor) / sizeof(Color_t));
  Color_Full(TGUI_COLOR_DEFAULT_BRUSH_DATA, 
             TGuiColor.MainBrushColor[0],
             sizeof(TGuiColor.MainPenColor) / sizeof(Color_t));   
  //������
  if(TitleY < TLCD_HIGH){
    Color_Full(TGUI_COLOR_DEFAULT_PEN_TITLE, 
               TGuiColor.MainPenColor[0],
               TGuiColor_cbGetCurMainW());
    Color_Full(TGUI_COLOR_DEFAULT_BRUSH_TITLE, 
               TGuiColor.MainBrushColor[0],
               TGuiColor_cbGetCurMainW()); 
  }
}

//-------------------------------��������������ǰ��ɫ����-----------------------
void TGuiColor_SetPenColor(unsigned char y,
                        unsigned char x,
                        unsigned char xLen,
                        Color_t Color)
{
  Color_Full(Color, &TGuiColor.MainPenColor[y][x], xLen);  
}


//-------------------------------�������������屳��ɫ����-----------------------
void TGuiColor_SetBrushColor(unsigned char y,
                        unsigned char x,
                        unsigned char xLen,
                        Color_t Color)
{
  Color_Full(Color, &TGuiColor.MainBrushColor[y][x], xLen);  
}

//---------------------------�����������Ӧǰ��ɫ����-------------------------
void TGuiColor_SetPenColorP(unsigned char y,
                        unsigned char x,
                        unsigned char xLen,
                        const Color_t *pColor)
{
  Color_Copy(&TGuiColor.MainPenColor[y][x], pColor, xLen); 
}

//---------------------------�����������Ӧ����ɫ����-------------------------
void TGuiColor_SetBrushColorP(unsigned char y,
                        unsigned char x,
                        unsigned char xLen,
                        const Color_t *pColor)
{
  Color_Copy(&TGuiColor.MainBrushColor[y][x], pColor, xLen); 
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





