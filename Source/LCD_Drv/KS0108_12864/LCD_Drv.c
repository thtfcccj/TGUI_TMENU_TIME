//*********************************************************************
//
//			LCDӲ����������-KS0108������128*64����֧��
//
//*********************************************************************/

#include    "LCD_Drv.h"
#include    "TRowDotBuf.h"
#include    "KS0108Cmd.h"
#include    "KS0108Drv.h"
#include    <string.h>

//-------------------------Ӳ����ʼ��---------------------------------------
static void _HwInit(void)
{
  unsigned char Screen;
  unsigned short i;
  //Ӳ����λ��ʾ��
  //(������Ӳ����λ��·)
  
  //��ģ���ʼ��
  LcdDev_CsInit();
  Lcd_Drv_Init();
  
  //=======================������ʼ��==========================
  for(Screen = 0; Screen < 2; Screen++){
    if(!Screen) LcdDev_ClrCs0();
    else  LcdDev_ClrCs1();
    
    //����Ƿ�����������״̬
    if(Ks0108Drv_RdState() & KS0108_CMD_STATE_RESET){
      //���ڸ�λ״̬,�쳣����
      Lcd_cbDelayMs(100);
    }
    //�����ʾ����ʾ
    Ks0108Drv_WrState(Ks0108Cmd_GetAdrX(0));
    Ks0108Drv_WrState(Ks0108Cmd_GetAdrY(0));
    for(i = (64*64/ 8); i > 0; i--)
      Ks0108Drv_WrData(0);
    
    //����ʾ����ʾ
    Ks0108Drv_WrState(Ks0108Cmd_GetDispOn());
    //����Ƿ���������ʾ״̬
    if(Ks0108Drv_RdState() & KS0108_CMD_STATE_OFF){
      //���ڹذ�״̬,�쳣����
      Ks0108Drv_WrState(Ks0108Cmd_GetDispOn());
    }
    //��ʼ������
    if(!Screen) LcdDev_SetCs0();
    else LcdDev_SetCs1();
  }
 
}

//-------------------------ģ���ʼ��---------------------------------------
void Lcd_Drv_Init(void)	
{
  _HwInit();        //Ӳ����ʼ��
  TRowDotBuf_Init();//�����ʼ��
  //LCDģ�������ʼ��


}

/****************************************************************************

							TGUI��ʾ���ص�����ʵ��
//ע:��ģ���ݲ�֧��DISP_CMD_DIS_UPDATE����,����,������������																
****************************************************************************/
#include "TGUIBase.h"


//-------------------------��ʾ������һ�����ݺ���--------------------
void TWinMng_cbUpdateRow(unsigned char Lcd,    //��ǰ�����Ǹ���ʾ��
                         unsigned char x,      //��Ҫ���µ�����ʼλ��
                         unsigned char y,      //��Ҫ���µ���
                         unsigned char w,      //���³���
                         char *pBuf,    //������ַ�
                         //ÿ���ַ���Ӧλ�ñ�־,�����ж���
                         unsigned char *pFlagBuf)
{
  unsigned char i;
  const unsigned char *pDotBuf;
  unsigned short CurAdr;

  //����֧��DISP_CMD_DIS_UPDATEʱ,���ڴ˽��зֿ鴦��!!!

  //��ȡ��������ģ
  pDotBuf = TRowDotBuf_pGetBuf(w,pBuf,pFlagBuf);
  //����ģ�����ͳ�,ÿ�θ��¸�����ȴ�С

  //============================������ͳ�=============================
  LcdDev_ClrCs0();
  for(i = 0; i < 128; i++){ //ÿ��������
    Ks0108Drv_WrState(Ks0108Cmd_GetAdrX(y));
    Ks0108Drv_WrState(Ks0108Cmd_GetAdrY(i));
    Ks0108Drv_WrData(*(pDotBuf + i));
    i++;
    Ks0108Drv_WrData(*(pDotBuf + i));
  }
  LcdDev_SetCs0();
  
  //============================�Ұ����ͳ�=============================
  LcdDev_ClrCs1();
  pDotBuf += 128;
  for(i = 0; i < 128; i++){ //ÿ��������
    Ks0108Drv_WrState(Ks0108Cmd_GetAdrX(y));
    Ks0108Drv_WrState(Ks0108Cmd_GetAdrY(i));
    Ks0108Drv_WrData(*(pDotBuf + i));
    i++;
    Ks0108Drv_WrData(*(pDotBuf + i));
  }
  LcdDev_SetCs1();
}


/****************************************************************************
                          TRowDotBuf�ص�����ʵ��
****************************************************************************/
//ʹ���Զ����ֿ�ʵ��:
#include "TWord.h"

//-----------------------�õ�ȫ������ģ--------------------------
void TRowDotBuf_cbGetWordModule(unsigned char *pBuf, //���ջ�����
                                unsigned short c)   //����
{
  unsigned short UserID = TWord_Word2UserID(c,
                                        TWord_GB2312LUT, 
                                        TWord_GetGB2312LUTSize());
  if(UserID == 0xffff) memset(pBuf, 0, 32);//δ�ҵ�ʱ���ո�
  else memcpy(pBuf,TWord_pGetWordModule(UserID); 32);//�õ���ģ
}

//-----------------------�õ��������ģ--------------------------
__arm void TRowDotBuf_cbGetHalfWordModule(unsigned char *pBuf, //���ջ�����
                                    unsigned char c)    //����
{
  if(c <= 32) //ǰ33���ַ�Ϊ������,�Ҹ��ſ��׶�ʧ,ֱ�����ո�(���ո�)
    memset(pBuf, 0, 16);
  else
    memcpy(pBuf, TWord_pGetAsciiModule(c), 16);
}




