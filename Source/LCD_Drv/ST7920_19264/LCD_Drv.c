//*********************************************************************
//
//			         LCDӲ����������
//
//
//
//*********************************************************************/

#include    "LCD_Drv.h"
#include    "ST7920_Command.h"

//-----------------�����ʾ����ʾ--------------------------
static void LcdClr(void)
{
	//����Ϊ��GDRAM��
	//unsigned char i;

	Lcd_UpperWrCommand(Extend8B());		//�����趨
	Lcd_UnderWrCommand(Extend8B());		 
	//����GDRAM�������ʾģʽ
	Lcd_UpperWrCommand(ExtendFunSet(bInterfaceCtlBit | bInstructionSetCtl | bGraphicDispCtl));//���书���趨,����ͼ����ʾ��
	Lcd_UpperWrCommand(ExtendFunSet(bInterfaceCtlBit | bInstructionSetCtl | bGraphicDispCtl));//���书���趨,����ͼ����ʾ��

	/*/�趨GDRAM��ַ��������������
	Lcd_UpperWrCommand(SetGDRAMAddr(0));	 //��ֱ��ַ
	Lcd_UpperWrCommand(SetGDRAMAddr(0));	//ˮƽ��ַ
	Lcd_UpperWrDataRdy();
	for(i = 0; i < (64 * 32); i++){//GDRAM������
		Lcd_UpperWrData(0);
	} 		
	Lcd_UnderWrCommand(SetGDRAMAddr(0));//��ֱ��ַ
	Lcd_UnderWrCommand(SetGDRAMAddr(0));//ˮƽ��ַ
	Lcd_UnderWrDataRdy();
	for(i = 0; i < (64 * 32); i++){//GDRAM��������
		Lcd_UnderWrData(0);
	}*/

  Lcd_UpperWrCommand(Base8B()); //�ص�8λ����ģʽ
  Lcd_UnderWrCommand(Base8B()); 

}

//-----------------lcd��ʼ��---------------------------------
//�Ƿ�����������־,������ʱ����ʾLOGO
void LcdInit(unsigned char EnInitScreen)	
{
   //===========================IOӲ����ʼ��========================
   Lcd_HwInit();

   //===========================����Ӳ����ʼ��========================
   Lcd_UpperWrCommand(Base8B()); //8λ����ģʽ
   Lcd_UnderWrCommand(Base8B());
   Lcd_cbDelayUs(120);
   Lcd_UpperWrCommand(Base8B()); //8λ����ģʽ,Ӳ��Ҫ������
   Lcd_UnderWrCommand(Base8B());
   Lcd_cbDelayUs(40);
   Lcd_UpperWrCommand(DisplaySet(0)); //����ʾ
   Lcd_UnderWrCommand(DisplaySet(0));
   Lcd_cbDelayUs(120);
   Lcd_UpperWrCommand(EntryModeSet(bAddNoMove)); //��������ʱ��ACֵ����ģʽ
   Lcd_UnderWrCommand(EntryModeSet(bAddNoMove));
   Lcd_cbDelayUs(120);
   Lcd_UpperWrCommand(ClearDisplay()); //��DDRAM��
   Lcd_UnderWrCommand(ClearDisplay());
   Lcd_cbDelayUs(12);//>10MS

   if(EnInitScreen) LcdClr();//����
   else Lcd_cbLogo();		//LOGO��ʾ

   //������ʾ����ʹ�ù��ϵͳ
   Lcd_UpperWrCommand(DisplaySet(bDispEn)); 			
   Lcd_UnderWrCommand(DisplaySet(bDispEn));

}

/****************************************************************************

							           TGUI��ʾ���ص�����ʵ��
																 
****************************************************************************/
#include "TGUIBase.h"

//-------------------------����һƬ�����ַ�����----------------------------
static void _UpdateString(unsigned char x,      //��Ҫ���µ�����ʼλ��
                          unsigned char y,      //��Ҫ���µ���
                          unsigned char w,      //���³���
                          const char *pBuf)    //������ַ�
{
  char String;
  //���ַ�����ʼֵ
	switch(y){
		case 0: Lcd_UpperWrCommand(SetDDRAMAddr(0x80 + x)); break;	  //0x80-0x8b
		case 1: Lcd_UpperWrCommand(SetDDRAMAddr(0x90 + x)); break;    //0x90-0x9b
		case 2: Lcd_UnderWrCommand(SetDDRAMAddr(0x80 + x)); break;	  //0x80-0x8b
		case 3: Lcd_UnderWrCommand(SetDDRAMAddr(0x90 + x)); break;    //0x90-0x9b
		default: return;	//���޲�����
	}
  //�ͳ��ַ�
  if(y < 2)Lcd_UpperWrDataRdy();
  else Lcd_UnderWrDataRdy();
  while(w--){
    String = *pBuf++;
	  if(y < 2) Lcd_UpperWrData(String);
    else Lcd_UnderWrData(String);
  } 
}

//-------------------------����һƬ�����־����----------------------------
//����±�־����Ϊ:0 ����ʾ���б�־
//DISP_CFG_ANTI:���Ը�λ��
//����:�ݲ�����
static void _UpdateFlag(unsigned char x,      //��Ҫ���µ�����ʼλ��
                          unsigned char y,      //��Ҫ���µ���
                          unsigned char w,      //���³���
                          const unsigned char *pFlag)    //����ı�־
{
  char Data;
  unsigned char i,j;
  //��ʹ����չģʽ�Ļ�ͼ����
	if(y < 2){
    Lcd_UpperWrCommand(Extend8B());
    Lcd_UpperWrCommand(ExtendFunSet(bInterfaceCtlBit | bInstructionSetCtl | bGraphicDispCtl));					//���书���趨
  }
  else{
    Lcd_UnderWrCommand(Extend8B());
    Lcd_UnderWrCommand(ExtendFunSet(bInterfaceCtlBit | bInstructionSetCtl | bGraphicDispCtl));					//���书���趨
  }

  //�����ڰ�ÿ���и��� 
  for(i = 0; i < 16; i++){  //i��ʾ����
    //��������
    if(y < 2){
  		Lcd_UpperWrCommand(SetGDRAMAddr((y << 4) + i));	//��ֱ��ַ
  	  Lcd_UpperWrCommand(SetGDRAMAddr(x));	          //ˮƽ��ַ
      Lcd_UpperWrDataRdy();
    }
    else{
  		Lcd_UnderWrCommand(SetGDRAMAddr(((y - 2) << 4) + i));	//��ֱ��ַ
  	  Lcd_UnderWrCommand(SetGDRAMAddr(x));	          //ˮƽ��ַ
      Lcd_UnderWrDataRdy();
    }
    //��һ��������������,�����ʲô��
    for(j = 0; j < w; j++){
      Data = *(pFlag + j);
      if(Data & DISP_CFG_ANTI) Data = 0xff;  //����
      else Data = 0;  //����
      //������ 
	    if(y < 2) Lcd_UpperWrData(Data);
      else Lcd_UnderWrData(Data);
    }//end for j;
	}//end for i

  if(y < 2) Lcd_UpperWrCommand(Base8B()); //�ص�8λ������ͨģʽ
  else Lcd_UnderWrCommand(Base8B());
}

//-------------------------��ʾ������һ�����ݺ���--------------------
void TWinMng_cbUpdateRow(unsigned char Lcd,    //��ǰ�����Ǹ���ʾ��
                         unsigned char x,      //��Ҫ���µ�����ʼλ��
                         unsigned char y,      //��Ҫ���µ���
                         unsigned char w,      //���³���
                         char *pBuf,    //������ַ�
                         //ÿ���ַ���Ӧλ�ñ�־,�����ж���
                         unsigned char *pFlagBuf)
{
  //�����ݷ�Ϊ�������ֹ���¶ηֱ���
  unsigned char i;
  unsigned char Flag,PrvFlag;
  unsigned char CurW;

  Lcd = Lcd;    //����һ����ʾ��
  //�õ����±�־
  if(*pFlagBuf == DISP_CMD_DIS_UPDATE)
    PrvFlag = DISP_CMD_DIS_UPDATE;
  else  PrvFlag = 0;
  CurW = 1;
  for(i = 1; i < w; i++){   //ɨ�����
    //=================Ԥ����==================
    switch(*(pFlagBuf + i) & 0xf0){ 
      case DISP_CMD_DIS_UPDATE://��ֹ����
        Flag = DISP_CMD_DIS_UPDATE;
        break;
      case DISP_CMD_CLR: //����
        *(pBuf + i) = ' ';
        *(pFlagBuf + i) = 0;
      case DISP_CMD_NOR://��������
      default:
        Flag = 0;
        break;
	  }
    //=================�δ���==================
    if(Flag != PrvFlag){//���л���,
      if(!PrvFlag){ //���ڸ���λ�ø����ַ����־
        PrvFlag =  i - CurW;  //��ʱ��������ʼλ��
        _UpdateString(x + PrvFlag, y, CurW, pBuf + PrvFlag);
        _UpdateFlag(x + PrvFlag, y, CurW, pFlagBuf + PrvFlag);
      }
      //else �����²�����
      //��λ
      PrvFlag = Flag;  
      CurW = 1;//���¿�ʼ,����һ���ַ���                      
    }
    else CurW++;
  }

  //================�������һ��===============
  if(!PrvFlag){ //���ڸ���λ�ø����ַ�
    PrvFlag =  w - CurW;  //��ʱ��������ʼλ��
	  _UpdateString(x + PrvFlag, y, CurW, pBuf + PrvFlag);
	  _UpdateFlag(x + PrvFlag, y, CurW, pFlagBuf + PrvFlag);
  }
  //else �����²�����
}
