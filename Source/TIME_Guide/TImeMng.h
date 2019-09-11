/*****************************************************************************

                     TGUI�ϵ����뷨ģ��(�������뷨)�������ܽӿ�

*****************************************************************************/
#ifndef __T_IME_MNG_H
#define __T_IME_MNG_H

/*****************************************************************************
                          �û�������
*****************************************************************************/
#define TIME_MNG_DISP_W    (240 / 8)  //�������뷨��ʾ�ַ����
#define TIME_MNG_DISP_H    (128 / 16)  //�������뷨��ʾ�ַ��߶�

/*****************************************************************************
                        ��غ궨��
*****************************************************************************/
//�����������뷨����,������ֵ�ڲ��ѹ̶���
#define TIME_MNG_TYPE_PINYIN    0    //ƴ�����뷨
#define TIME_MNG_TYPE_NUM       1    //�������뷨
#define TIME_MNG_TYPE_LOWERCASE 2    //Сд��ĸ���뷨
#define TIME_MNG_TYPE_CAPITAL   3    //��д��ĸ���뷨
#define TIME_MNG_TYPE_SIGN      4    //�������뷨

//Ĭ�Ͽ�ʹ��ȫ�����뷨ʱ�Ķ���
#define TIME_TYPE_MASK    0x1f

/*****************************************************************************
                        ��ؽṹ
*****************************************************************************/
#include "TImeEdit.h"
#include "TImeCapital.h"
#include "TImeNum.h"
#include "TImePinYin.h"
#include "TImeSign.h"
#include "ClipBoard.h"  //���а�

//���ڴ�Ÿ����뷨�ڲ����ݽṹ
union _TImeUnion{
  struct _TImeCapital Capital;       //��д���뷨
  struct _TImeNum Num;               //���ּ����뷨
  struct _TImePinYin PinYin;         //ƴ�����뷨  
  struct _TImeSign Sign;             //�������뷨
};

//TIme���ṹ
#include "TWin.h"
#include "TImeBase.h" //TIme_String_t;



struct _TImeMng{
  TWin_t *pWin;             //��ǰ�ҽӵĴ���
  struct _TImeEdit Edit;    //���б༭��,��������ַ����������Ϣ
  struct _ClipBoard ClipBoard;////���а�
  union _TImeUnion Data;     //���ڴ�Ÿ����뷨�ڲ����ݽṹ
  const char *pSignTbl;     //�ݴ����ķ��������
  
  unsigned char DispOffsetX;//����ʾ��xƫ����
  unsigned char DispOffsetY;//����ʾ��yƫ����
  
  unsigned char Type;       //��ǰ���뷨����
  unsigned char TypeMask;   //��ʹ�õ����뷨����  
  unsigned char State;       //�ڲ�����״̬��������:
};

//�ڲ�����״̬������Ϊ:
#define TIME_MNG_STATE_EDIT        0   //�༭ģʽ
#define TIME_MNG_STATE_FORCE_EXIT  1   //ǿ���˳���ʾ
#define TIME_MNG_STATE_SEL         2  //���뷨ѡ��ģʽ
#define TIME_MNG_STATE_IME         3  //���뷨ģʽ

/*****************************************************************************
                            ��غ���
*****************************************************************************/

//----------------------------------��ʼ������---------------------------------
//�����Ƿ�ɹ���ʼ��
signed char TImeMng_Init(struct _TImeMng *pIme,  //��������뷨�ṹ����
                         TWin_t *pWin,             //��ǰ�ҽӵĴ���
                         char *pString,            //�û��Ѵ��ڵ��ַ���,������'\0'��β
                         TIme_String_t Size,     //�ַ��������������С+1
                         unsigned char DefaultType,   //Ĭ�����뷨,<4
                         unsigned char TypeMask,//��ʹ�õ����뷨���� 
                         const char *pSignTbl); //�ҽӵķ��ű�,Ϊ��ʱʹ��Ĭ��

//----------------------------����������---------------------------------
//����ֵ����Ϊ: 0����״̬,-1:�˳���ֱ���˳�;-2:ȷ�ϼ��˳�
signed char TImeMng_Key(struct _TImeMng *pIme,
                        char Key);          //��ֵ,����Ӧ�������ҽ���6��

//--------------------------������---------------------------------
//�������뷨ʱ���ô˺���ʵ��ʱ��Ҫ��,��������128ms
void TImeMng_Task(struct _TImeMng *pIme);

//-----------------------------�˳�����---------------------------------
//�û������ַ�ȷ���˳�����ô˺���
void TImeMng_Quit(struct _TImeMng *pIme);

#endif //#define __T_IME_MNG_H

