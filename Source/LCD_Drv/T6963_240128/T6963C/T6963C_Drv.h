/*******************************************************************************

                 T6963C�ײ������ӿ�
��ͷ�ļ���װ��T6963C����ָ���LCD�����������
*******************************************************************************/
#ifndef __T6963C_DRV_H
#define __T6963C_DRV_H

/************************************************************************
						            ��ػص�����														 
************************************************************************/
//------------------------------д����------------------------------
void T6963C_cbWrCmd(unsigned char Cmd);

//------------------------------д����------------------------------
void T6963C_cbWrData(unsigned char Data);

/************************************************************************
					                 ָ�����																 
************************************************************************/

//---------------------------״̬�ֲ���-----------------------------
//״̬��λ����
#define  T6963C_CMD_BUSY		0x01	//ָ���дæ
#define  T6963C_DATA_BUSY		0x02	//���ݶ�дæ
#define  T6963C_CMD_AUTO_BUSY	0x04	//ָ���Զ���æ
#define  T6963C_DATA_AUTO_BUSY	0x08	//�����Զ���æ
#define  T6963C_RUN_CHECK_EN	0x20	//���������м�������1:����,0:������
#define  T6963C_SCREEN_COPY_ERR	0x40	//��������������״̬
#define  T6963C_FLASH_STATE		0x80	//��˸״̬���1:��ʾ,0:����ʾ

//-------------------LCD���ù��ָ��---------------------------
//�β�1:ˮƽλ��
//�β�2:��ֱλ��
//����ʵ�ַ���:
//void T6963_SetCursor(uchar uHor,uchar uVert);
//��ʵ�ַ���
#define	T6963_SetCursor(uHor,uVert) \
do{\
	T6963C_cbWrData(uHor);\
	T6963C_cbWrData(uVert);\
	T6963C_cbWrCmd(0x21);\
}while(0)
//-------------------LCD����CGRAM ƫ�õ�ַ---------------------------
//�β�:ƫ�Ƶ�ַ
//����ʵ�ַ���:
//void T6963_SetCGRAMBaseAddr(uchar uBaseAddr);
//��ʵ�ַ���:
#define T6963_SetCGRAMBaseAddr(uBaseAddr)\
do{\
	T6963C_cbWrData(uBaseAddr);\
	T6963C_cbWrData(0);\
	T6963C_cbWrCmd(0x22);\
}while(0)
//-------------------LCD���õ�ַָ��---------------------------
//�β�:��ַָ��
#define T6963_SetAddr(uAddr)\
do{\
	T6963C_cbWrData(uAddr & 0x00ff);\
	T6963C_cbWrData(uAddr >> 8);\
	T6963C_cbWrCmd(0x24);\
}while(0)
//-------------------LCD�����ı�����ַ---------------------------
//�β�:�ı�����ַ
#define T6963_SetTextAddr(uAddr)\
do{\
	T6963C_cbWrData(uAddr & 0x00ff);\
	T6963C_cbWrData(uAddr >> 8);\
	T6963C_cbWrCmd(0x40);\
}while(0)
//-------------------LCD�����ı������---------------------------
//�β�:�ı�����ȣ��ֽ������У�
#define T6963_SetTextWidth(uWidth)\
do{\
	T6963C_cbWrData(uWidth);\
	T6963C_cbWrData(0);\
	T6963C_cbWrCmd(0x41);\
}while(0)
//-------------------LCD����ͼ������ַ---------------------------
//�β�:ͼ������ַ
#define T6963_SetGraphAddr(uAddr)\
do{\
	T6963C_cbWrData(uAddr & 0x00ff);\
	T6963C_cbWrData(uAddr >> 8);\
	T6963C_cbWrCmd(0x42);\
}while(0)
//-------------------LCD����ͼ�������---------------------------
//�β�:ͼ������ȣ��ֽ������У�
#define T6963_SetGraphWidth(uWidth)\
do{\
	T6963C_cbWrData(uWidth);\
	T6963C_cbWrData(0);\
	T6963C_cbWrCmd(0x43);\
}while(0)
//-------------------LCD���úϳ���ʾ��ʽ---------------------------
//�������ı���ʽ��ͼ�η�ʽ����ʱ���ϳ���ʾ��ʽ���ò���Ч
//�߼� ���� �ϳ�
#define T6963_SetDispOrMode() 	T6963C_cbWrCmd(0x80)	
//�߼� ����� �ϳ�
#define T6963_SetDispXorMode() 	T6963C_cbWrCmd(0x81)
//�߼� ���롱 �ϳ�	
#define T6963_SetDispAndMode() 	T6963C_cbWrCmd(0x83)
//�ı�����,��LCD�ı����������ı�������ʽ��ʾ
#define T6963_SetDispTextMode T6963C_cbWrCmd(0x84)
//-------------------LCD�ı�������ʽ����---------------------------
//��4λ��Ч
#define T6963C_FLASH_CTR		0x00		//��������λ,�����з�ʽ���
#define T6963C_TEXT_MODE_POS	0x00		//������ʾ
#define T6963C_TEXT_MODE_NEGA	0x05		//������ʾ
#define T6963C_TEXT_MODE_DIS	0x03		//��ֹ��ʾ
//-------------------LCD���������ʾ����---------------------------
//�βΣ���ʾ����
#define T6963C_SetDispSwitch(uSwitch) T6963C_cbWrCmd(0x90 | (uSwitch))	
//��ʾ�������λ����Ϊ:
#define T6963C_CORSOR_FLASH	0x01	//���������������λ
#define T6963C_CORSOR_EN	0x02	//�����ʾ��������λ
#define T6963C_TEXT_EN		0x04	//�ı���ʾ��������λ
#define T6963C_GRAPH_EN		0x08	//ͼ����ʾ��������λ
//-------------------LCD�����״ѡ��---------------------------
//�βΣ������״,Ϊ8 �㣨�У���uShape�У�uShape ��ֵΪ0��7
#define T6963_SetCursorShape(uShape) T6963C_cbWrCmd(0xa0 | (uShape))	
//-------------------LCD�����Զ�����д��ʽ����---------------------------
//��ʼ�Զ�д
#define T6963_StartAutoWr()	 T6963C_cbWrCmd(0xb0)
//��ʼ�Զ���
#define T6963_StartAutoRd()	 T6963C_cbWrCmd(0xb1)
//�����Զ���д
#define T6963_AutoEnd()	 	T6963C_cbWrCmd(0xb3)
//-------------------LCD���ݡ�����һ�ζ���д��ʽ����---------------------------
//����д����ַ��1
#define T6963_DataWrInc(Data)  do{T6963C_cbWrData(Data);T6963C_cbWrCmd(0xc0);}while(0)
//���ݶ�����ַ��1
#define T6963_DataRdInc(Data)  do{T6963C_cbWrData(Data);T6963C_cbWrCmd(0xc1);}while(0)	 	T6963C_cbWrCmd(0xc1)
//����д����ַ��1
#define T6963_DataWrDec(Data)  do{T6963C_cbWrData(Data);T6963C_cbWrCmd(0xc2);}while(0)
//���ݶ�����ַ��1
#define T6963_DataRdDec(Data)  do{T6963C_cbWrData(Data);T6963C_cbWrCmd(0xc3);}while(0)
//����д����ַ����
#define T6963_DataWrKeep(Data)  do{T6963C_cbWrData(Data);T6963C_cbWrCmd(0xc4);}while(0)
//���ݶ�����ַ����
#define T6963_DataRdKeep(Data)  do{T6963C_cbWrData(Data);T6963C_cbWrCmd(0xc5);}while(0)
//-------------------LCD��ʼ����-----------------------------
#define T6963_StartRdScr()	 	T6963C_cbWrCmd(0xe0)
//-------------------LCD��ʼ������---------------------------
#define T6963_StartScrCopy()	 T6963C_cbWrCmd(0xe8)
//-------------------LCD��ʾ������λ����---------------------------
//�β�1��ָ���������λ:��3λ��0 ����;��3λ��1 ��λ��0-2λ:����λ��Ӧ��Ԫ��D0��D7 λ��
#define T6963_DispBurSetBit(uBit)	 T6963C_cbWrCmd(0xf8 | uBit)


#endif

