/*******************************************************************************

 			            LCDӲ����TGUI��ʾ�ӿ�֮t6963����ʵ��
 
*******************************************************************************/
#include    "LCD_Drv.h"
#include    "PlotPB.h" //�����ַ���
#include    <string.h>

/****************************************************************************
							       TGUI��ʾ���ص�����ʵ��	
****************************************************************************/
//ע:��ģ����ֻ֧��	 DISP_CFG_ANTI ����
#include "TGUIBase.h"

//-------------------------��ʾ������һ�����ݺ���--------------------
void TWinMng_cbUpdateRow(unsigned char Lcd,    //��ǰ�����Ǹ���ʾ��
                         unsigned char x,      //��Ҫ���µ�����ʼλ��
                         unsigned char y,      //��Ҫ���µ���
                         unsigned char w,      //���³���
                         const char *pBuf,    //������ַ�
                         //ÿ���ַ���Ӧλ�ñ�־,������
                         const unsigned char *pFlagBuf)
{
  if(!LCD_Drv_cbLineIsEn(y)) return; //�в��������ʱ
  
  const Color_t *pPen = LCD_Drv_pcbGetPenColor(x, y);         //ȡ��������ɫ
  const Color_t *pBrush = LCD_Drv_pcbGetBrushColor(x, y);     //ȡ��������ɫ
  Color_t ForcusBrush = LCD_Drv_cbGetForcusBrushColor();       //ȡ�����㱳����ɫ
  unsigned char xEnd = x + w;
  unsigned short yPixel = (unsigned short )y << 4;
  for(; x < xEnd; x++){
    char c = *pBuf;
    //��ǰ����ɫ
    Color_t Brush;
    if(*pFlagBuf & DISP_CFG_ANTI) Brush = ForcusBrush;//��ѡ����
    else Brush = *pBrush;
    //ASCIIֱ��ת��
    if(c < 0x80){
      PlotPB_Asc((unsigned short )x << 3, yPixel, c, *pPen, Brush);
    }
    //δ���GB2312��ȷ�ԣ�������
    else{
      if((x + 1) >= xEnd) break;//������ֽ����ˣ��쳣 
      pBuf++; //ȡ�°����
      PlotPB_GB2312((unsigned short )x << 3, yPixel,
                    ((unsigned char)c << 8) | *pBuf, *pPen, Brush); //��λ��ǰ
      //�������
      x++;
      pFlagBuf++;
      pPen++;
      pBrush++;      
    }
    //��x�⣬����������ͳһ����
    pBuf++;
    pFlagBuf++;
    pPen++;
    pBrush++;
  }
}





