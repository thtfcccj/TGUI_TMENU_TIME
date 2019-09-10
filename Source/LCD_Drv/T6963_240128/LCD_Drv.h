/*******************************************************************************

 			            LCDӲ����TGUI��ʾ�ӿ�֮t6963����ʵ�ֽӿ�
 
*******************************************************************************/
#ifndef __LCD_DRV_H
#define __LCD_DRV_H

/****************************************************************************
							              �������															 
****************************************************************************/
#define     LCD_DEV_WIDTH    240 //��ʾ�����
#define     LCD_DEV_HIGH     128 //��ʾ���߶�    

//֧�ֲ��Թ���ʱ����
#define SUPPORT_LCD_TEST

/****************************************************************************
							               ��غ���																 
****************************************************************************/

//-------------------------ģ���ʼ��---------------------------------------
void Lcd_Drv_Init(void);	


/****************************************************************************
							               ��ʾ���������																 
****************************************************************************/
#ifdef SUPPORT_LCD_TEST

//������ʾ���������ʱ��,0.5sΪ��λ
#define LCD_DEV_TEST_COUNT     25  //������ʱ��
#define LCD_DEV_TEST_FULL      20  //�������Խ���ʱ��
#define LCD_DEV_TEST_NULL      15  //�������Խ���ʱ��
#define LCD_DEV_TEST_REFRESH   (LCD_DEV_TEST_NULL - 7)//ˢ�����Խ���ʱ��
//#define LCD_DEV_TEST_END_WAIT  0   //�����ȴ�ʱ��

extern unsigned char Lcd_Drv_TestTimer;  //���Զ�ʱ��
//-------------------------����������---------------------------------------
//������,0.5sһ��
void Lcd_Drv_TestTask(void);	

//-------------------------�������״̬---------------------------------------
//void Lcd_Drv_EnterTest(void);	
#define Lcd_Drv_EnterTest() \
  do{Lcd_Drv_TestTimer = LCD_DEV_TEST_COUNT;}while(0)

//-------------------------�˳�����״̬---------------------------------------
void Lcd_Drv_QuitTest(void);	
#define Lcd_Drv_QuitTest() do{Lcd_Drv_TestTimer = 0;}while(0)

#endif

/****************************************************************************
							              �ص�����																 
****************************************************************************/

//----------------------��ʱʱ�����--------------------------
//void Lcd_cbDelayUs(unsigned char Us);
//void Lcd_cbDelayMs(unsigned char Ms);
//ֱ��ʵ��:
#include    "math_2.h"
#define Lcd_cbDelayUs(Us)  Delay40Ns((unsigned int)(Us) * 10)
#define Lcd_cbDelayMs(Ms)  Delay40Ns((unsigned int)(Ms) * 10000)

//--------------------�������ͨ��------------------------
//void Lcd_Drv_cbTestFinalNotify(void);
//ֱ��ʵ��Ϊ:
#include    "Gui_Task.h"
#define Lcd_Drv_cbTestFinalNotify()  do{GUI_Task_ReturnMain();}while(0)

#endif

