/*******************************************************************************

                           TWidget�ؼ�����
��ģ�����:TWIN(����)
*******************************************************************************/
#ifndef __T_WIDGET_H 
#define __T_WIDGET_H

/*******************************************************************************
                                �ؼ�˵��
*******************************************************************************/
//����˵����TWindgetNote.h
//�˿ؼ���TWin�����Ϲ���,��Ҫʵ���˵��û�������(ItemsRect)��
//�û����ݼ�����,�����ݱ�־������ص�����
//�˿ؼ���������Ӧ�ó���ǰ�Ľ�������,���Դ˹���Ϊ����,�������߲�
//��Ŀؼ����ܹ��û�ʹ��.

//�̳й�ϵ:
//TWidget�̳й�ϵΪ:
//  TWidget->(��Ӽ̳�)TWin
//ֱ�Ӽ̳�ʱ��ֱ�ӽ��ýṹת��Ϊ����ṹ�󲢵��û�����غ���
//��Ӽ̳�ͨ���ýṹ�ṩ�ļ��ת������ת��Ϊ����ṹ���ɵ��û���ṹ����
//
//��ؼ��Ѱ�װ��,�һ���������˵����(ע�ʹ����־),
//����ֱ�Ӳ����������ܻ�Ӱ��ϵͳ����,�ʲ�����ֱ��ʹ�û������

//����˵��:
//ÿ���ؼ�������������:
//1:���Ծ��б߿�(���ڸ߶�>=4ʱ);
//2:���Ծ�����ʾͷ(���ڸ߶�>=3ʱ);
//3:���Ծ���ˮƽ(���ڿ��>=5ʱ)�봹ֱ������ָʾ:
//  (1): �������߶�<=4,�в�������ָʾ
//  (2): �������߶�>4,�й�����ָʾ
//4:ÿ���ؼ�����ѡ���ھ����н���

//����Ӧ�ÿؼ����Դ˿ؼ�Ϊ����ʵ��
//��ʾ�����ݼ�����λ���ɼ̳д˽ṹ�Ŀؼ�ʵ��

/**********************************************************************
                           ��ؽṹ
**********************************************************************/
#include "TWM.h"     //���ڹ�����
#include "TGUIBase.h"    //��������

typedef struct{
  TWinHandle_t hWin;     //���ڴ���,�򴰿�Ϊ����ṹ,�ʳ����ù�ϵ  
  //������ռ�,��ʾ������ռ��ʾ�ռ�,�ڴ����ڲ��ܹ���ʾʱ,�����÷�ҳ��ʾ�ķ�ʽ
  //����x,y��ʾ��������,��������,wΪ���������ֵ,h��ʾ������
  TItemsRect_t ItemsRect;

  TItemSize_t Sel;        //��ѡ��
  unsigned char Flag;    //��־,����ʼ�����������еĶ���
}TWidget_t;

/**********************************************************************
                         ��Ա��������
**********************************************************************/
//--------------------��TWin�����̳еõ�TWin�ṹ����---------------------
//��õ���������:
#define TWidget_hGetWin(pWidget) ((pWidget)->hWin)

//����TWidget��ͨ��Win�����ʽ��Ӽ̳�TWin��,���пؼ���ͨ�����к�ʵ��ת��:
#define TWidget_pGetWin(pWidget) (TWM_pGetWin((pWidget)->hWin))

//-------------------����ز���--------------------------
//��õ���ǰ��ѡ
#define TWidget_GetSel(pWidget) ((pWidget)->Sel)
//�����õ�ǰ��ѡ
void TWidget_SetSel(TWidget_t *pWidget,TItemSize_t  Sel);
//��õ�������
#define TWidget_GetItems(pWidget) ((pWidget)->ItemsRect.h)
//����������,���ڿؼ���ʼ��������ʹ��
void TWidget_SetItems(TWidget_t *pWidget,TItemSize_t Items);

//---------------------��ռ����--------------------------
//������������ܵ����ұ߿��Լ�����ȹ������У��������û�������
//��õ���ʼλ��x
#define TWidget_GetX(pWidget) ((pWidget)->ItemsRect.x)
//��õ����,������
#define TWidget_GetW(pWidget) ((pWidget)->ItemsRect.w)
//��Ԥ�����,������
void  TWidget_SetW(TWidget_t *pWidget,TItemSize_t w);
//��õ������û�����������ʼλ��y
#define TWidget_GetY(pWidget) ((pWidget)->ItemsRect.y)

//---------------�û����ݼ�����ʾ��--------------------------
//������ָ�û���������
//��õ��û���������������x
unsigned char TWidget_GetClipX(TWidget_t *pWidget);
//��õ��û���������������y
unsigned char TWidget_GetClipY(TWidget_t *pWidget);
//��õ�ˮƽҳ��С,����ʾ���ڿ��w 
unsigned char TWidget_GetHPageSize(TWidget_t *pWidget);
//��õ���ֱҳ��С,����ʾ���ڸ߶�h
unsigned char TWidget_GetVPageSize(TWidget_t *pWidget);

//------------------------����------------------------------
//�õ���������ʾ���ˮƽҳ��С,,��������ʾ����ʾ���ڿ��w 
unsigned char TWidget_GetHPageSizeEnNotify(TWidget_t *pWidget);
//��õ���־
#define TWidget_GetFlag(pWidget) ((pWidget)->Flag)
//��õ��û���־
#define TWidget_GetUserFlag(pWidget) ((pWidget)->Flag & 0x03)
//�����û���־
void TWidget_SetUserFlag(TWidget_t *pWidget,unsigned char Flag);

/**********************************************************************
                          ��Ϊ����
**********************************************************************/

//--------------------------���Ƴ�ʼ������-------------------------
//���������ؼ�ʱ���ô˺�����ʼ������
//�˺����Զ�������־����־,��������ʾ������Ϊ�����ڴ����ڵ������
//��ʼ��Ϊ����������,������0��
void TWidget_Init(TWidget_t *pWidget,
                  TWinHandle_t hWin,
                  unsigned char Flag);

//����,Flag��־����Ϊ:
#define   TWIDGET_EN_FRAME    0x80  //�ù������б߿�,��LC�߶�>-3��ʱ������Ч
#define   TWIDGET_EN_HEADER   0x40  //������ʾ����ͷ
#define   TWIDGET_EN_HSCOLL   0x20  //������ʾˮƽ������
#define   TWIDGET_EN_VSCOLL   0x10  //������ʾ��������
#define   TWIDGET_EN_RETURN   0x08  //����ػ���־
#define   TWIDGET_EN_NOTIFY   0x04  //���޹�������߿�ʱ�������û��ռ���ʾ��ʾ��Ϣ
//�û����Զ���ʹ�õı�־�� 
#define   TWIDGET_USER       0x03

//--------------------------��С�ı�ͨ������-------------------------
//���ı�TWidget_t��Ӧ��Win��С�ı�ʱ,�����Զ��ı���ʾ��ʽ
void TWidget_WinReSizeNotify(TWidget_t *pWidget,
                             unsigned char w,
                             unsigned char h);

//-----------------------����������---------------------------
//���¼�������л�,���Ҽ�ˮƽ�ƶ�����,���·�ҳ����ҳ,���ּ�ѡ����Ŀ
//���ر�־λ����Ϊ��λʱ�����仯(����ͼͨ��)
//ע:������0ʱ,��ʾ�ð�����ǰ������Ч
unsigned int TWidget_Key(TWidget_t *pWidget,unsigned char Key);

/**********************************************************************
                          ��ػ�ͼ����
**********************************************************************/
//���ƻ�ͼ����ʹ��˵��:
//���ؼ���������ͼ����,�Դ�Ϊ����Ŀؼ�����ʵ�ֻ�ͼ����,��:
//static void _Paint(TListbox_t *pListbox,    //����ؼ�
//                   unsigend char PaintFlag) //���Ʊ�־,���������������ض���
//�ڸô�������,Ϊ��߻�ͼЧ��,ԭ����Ӧ���ݻ��Ʊ�־������˳����л�ͼ����:
//1:��ͷ�ı�,�����TWidget_PaintHeader����ͷ
//2:������(ˮƽ�봹ֱ����)�����仯,Ӧ����TWidget_pGetPaintRow���ڻ�������������
//3:�����㷢���仯,Ӧ����Ӧ�ó������TWidget_SetSetFocus���ý�����ʾ
//4:������ˮƽ�봹ֱ����仯�ı�־����TWidget_PaintEnd,��ͼ����

//����������ػ�ؼ�:
//1:�ڿ��ƴ�����ɺ�,Ӧ����ͼ��־��Ϊ0xff�Ա��ػ�����
//2:�ڰ������������,������־���û����ܵĻ��Ʊ�־�����ػ�
//3:�û��ڿ��ܵ������,�ֹ��ػ���ز���


//-------------------�ﻭ����ͷ���ϱ߿���----------------------------
//�˺������ϲ�߿�,������ͷ
void TWidget_PaintHeader(TWidget_t *pWidget,
                        const char *pHeader,   //��ʾͷ,��ʱΪNULL
                        unsigned char Len);   //��ʾͷ���ݳ���,��Ϊ0,��Ӧ������ַ�

//---------------------��õ��л���������---------------------------
//�˺��������߿�,������л���λ�ü����ȹ��ⲿ�������
//�����ؿ�,��ʾ���ɽ����ַ�
//ע:�ɽ������ݳ��ȿɵ���TWidget_pGetPaintRowLen()�õ�
//���ַ���ʼλ�ÿɵ���TWidget_pGetPaintRowStringStart()�õ�
//��ʼ��ʾ��ɵ���TWidget_pGetPaintRowItemStart()�õ�
char *TWidget_pGetPaintRow(TWidget_t *pWidget,
                           TItemSize_t Item,    //��Ҫ��ʾ����
                           TItemSize_t PrvLen); //��һ���û��ַ�����

//---------------------��õ�����䳤�Ⱥ���------------------------
//unsigned char TWidget_GetPaintRowLen(TWidget_t *pWidget);
#define  TWidget_GetPaintRowLen(pWidget) TWidget_GetHPageSize(pWidget)

//---------------------��õ��ַ���ʼλ�ú���------------------------
#define TWidget_GetPaintRowStringStart(pWidget) ((pWidget)->ItemsRect.x)

//---------------------��õ�����ʼλ�ú���------------------------
#define TWidget_GetPaintRowItemStart(pWidget) ((pWidget)->ItemsRect.y)


//---------------------�����ý���������------------------------
//�˺����������ý���(һ�㱻ѡ���ý���)
void TWidget_SetFocus(TWidget_t *pWidget,
                      TItemSize_t y,     //����ռ�����λ��
                      TItemSize_t x,     //����ռ����λ��,>��ռ�ʱ������ʾ����
                      TItemSize_t w);     //������,0��ʾx���������ȫΪ����

//---------------------�������ͼ����---------------------------
//�˺��������Ҳ����²�߿�
void TWidget_PaintEnd(TWidget_t *pWidget,
                      unsigned char PaintFlag,//����Ļ�ͼ��־
                      TItemSize_t LastW);  //���һ�����ݳ���



#endif

