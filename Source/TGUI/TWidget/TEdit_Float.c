/*******************************************************************************

                           TWidget֮�༭��-ʮ����������ʵ��

*******************************************************************************/
#include "TEdit.h"
#include <string.h>
#include "TEditPrv.h"  //�ڲ�����,�����ڲ�ʹ��
#include "TEdit_Float.h"
#include <math.h>       //floor

/**********************************************************************
                          ֧�ָ���������ʱ
**********************************************************************/
#if TGUI_EDIT_FLOAT_SUPPORT 

//----------------�༭����������ʼ��Ϊ����--------------------
void TEditFloat(TEdit_t *pEdit,
                TWinHandle_t hWin,    //�����Ĵ���,���ݻ�����>Len
                float Value,            //��ʼ��ֵ
                float Max,              //���ֵ
                float Min,              //��Сֵ
                unsigned char DotPos,  //��ȷ��С����λ��
                unsigned char Flag,  //�������
                TGUINotify_t cbGUINotify) //ͨ������
{
  //��������ת��Ϊ��������,ֱ��ʹ����������
  float Muti = (float)TMutiTUnigned[DotPos];  
  //ת��Ϊ��������
  TEditDec(pEdit,hWin,
           (TSigned_t)floor(Muti * Value + 0.5),
           (TSigned_t)floor(Muti * Max + 0.5),
           (TSigned_t)floor(Muti * Min + 0.5),
           DotPos,Flag,cbGUINotify);

  pEdit->eType = TEditType_Float;//��������
}

//----------------������ת��ΪTSigned_t����ռλ����--------------------
//�˺�����TData2Float�ɶ�ʹ��,TSigned_tռλӦ>=floatռλ
TSigned_t TFloat2Data(float Value)
{
  TSigned_t Data;
  memcpy(&Data,&Value,sizeof(float));
  return Data;  
}

//-----------TSigned_t���͸�����ռλת���ظ���������--------------------
//�˺�����TFloat2Data�ɶ�ʹ��,TSigned_tռλӦ>=floatռλ
float TData2Float(TSigned_t Data)
{
  float Value;
  memcpy(&Value,&Data,sizeof(float));
  return Value; 
}


//----------------�ɸ�������ֵ�õ�ռλ����,��������--------------------
unsigned char TGetFloatLen(float Min,float Max,
                          unsigned char DotPos)  //��ȷ��С����λ��
{
  float Muti = (float)TMutiTUnigned[DotPos & 0x07]; 
  return TGetNumLen((TSigned_t)floor(Muti * Min + 0.5),
                    (TSigned_t)floor(Muti * Max + 0.5));
}

//-----------------����־������ת��Ϊ�ַ�������------------------
//����������ĩδ�û�����λ��
char *pTFloat2StringFlag(float Value,   //��ǰ��ֵ
                      char *pBuf,    //���ջ�����
                      unsigned char Len,    //��ʾ����
                      //��־,����Ϊ����3bit:С����λ��,0x80:��ʾ������
                      unsigned char Flag)
{
  //ת��Ϊ����
  float Muti = (float)TMutiTUnigned[Flag & 0x07]; 
  Muti = Muti * Value + 0.5;
  return pTNum2StringFlag((TSigned_t)floor(Muti),pBuf,Len,Flag);
}


/**********************************************************************
    ��֧�ָ���������ʱ,Ϊ��ֹʹ�ÿպ�����ʱ���ܵı��뾯��,��������Ϊ��
**********************************************************************/
#else

//----------------�༭����������ʼ��Ϊ����--------------------
void TEditFloat(TEdit_t *pEdit,
                TWinHandle_t hWin,    //�����Ĵ���,���ݻ�����>Len
                float Value,            //��ʼ��ֵ
                float Max,              //���ֵ
                float Min,              //��Сֵ
                unsigned char DotPos,  //��ȷ��С����λ��
                unsigned char Flag,  //�������
                TGUINotify_t cbGUINotify) //ͨ������
{
  pEdit = pEdit;
  hWin = hWin;
  Value = Value;
  Max = Max;
  Min = Min;
  DotPos = DotPos;
  Flag = Flag;
  cbGUINotify = cbGUINotify;
}

//----------------������ת��ΪTSigned_t����ռλ����--------------------
//�˺�����TData2Float�ɶ�ʹ��,TSigned_tռλӦ>=floatռλ
TSigned_t TFloat2Data(float Value)
{
  return Value;  
}

//-----------TSigned_t���͸�����ռλת���ظ���������--------------------
//�˺�����TFloat2Data�ɶ�ʹ��,TSigned_tռλӦ>=floatռλ
float TData2Float(TSigned_t Data)
{
  return Data; 
}


//----------------�ɸ�������ֵ�õ�ռλ����,��������--------------------
unsigned char TGetFloatLen(float Min,float Max,
                          unsigned char DotPos)  //��ȷ��С����λ��
{
  Max = Max;
  Min = Min;
  DotPos = DotPos;
  return 0;
}

//-----------------����־������ת��Ϊ�ַ�������------------------
//����������ĩδ�û�����λ��
char *pTFloat2StringFlag(float Value,   //��ǰ��ֵ
                      char *pBuf,    //���ջ�����
                      unsigned char Len,    //��ʾ����
                      //��־,����Ϊ����3bit:С����λ��,0x80:��ʾ������
                      unsigned char Flag)
{
  //ת��Ϊ����
  Value = Value;
  Len = Len;
  Flag = Flag;
  return pBuf;
}
#endif



