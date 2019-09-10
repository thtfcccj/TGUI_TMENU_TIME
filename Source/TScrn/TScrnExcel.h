/*******************************************************************************

                           ��Ļ��ʾ֮���ӱ��Ԫ
 ��ģ����TGUI֮�ϣ�ʵ��һ��ͨ�ñ����ʽ�Ľ���
 ���������̶������Ҳ�ɹ�������ɣ�����˫��Ϊ����͹������
 ��������ʾ���룬������Ҫ������ͨ���ص�����ʵ��
 ֧����ʱ���¹���
*******************************************************************************/

#ifndef __TSCRN_EXCEL_H 
#define __TSCRN_EXCEL_H

/*******************************************************************************
                            ��ؽṹ
*******************************************************************************/
#include "TGUI.h"

//-------------------------------���ݼ����������ṹ------------------------------
struct _TScrnExcelData{
  //�ҽӵ��û���������
  void *pData;
  //ȷ�ϼ��ص�����,�β�Ϊ����ID,��ʱΪNULL
  void (*EnterKey)(unsigned short AryId);
  //�˳��ص�����,��ʱΪNULL����ʱ�˳���ִ�й̶��ص� 
  void (*ExitKey)(); 
  //�õ�����ͷ 
  const char*(*pGetHeader)(const struct _TScrnExcelData*);
  //�õ�����������
  unsigned short(*GetItemCount)(const struct _TScrnExcelData*);      
  //�õ�������,����NULL��ʾ����
  const char*(*pGetLine)(const struct _TScrnExcelData*,
                         unsigned short Line); //ָ����   
  //��������(��ʵ��Ϊ���ұ���),�����Ƿ�������
  signed char (*UpdateData)(const struct _TScrnExcelData*,
                             unsigned short StartLine, //��ʼ�� 
                             unsigned short Count);    //����
  //����λ�������Ҫ��������λ��Ϊӳ���ϵʱ,�໥ת������,��Ϊ��Ӧ��ϵ����ΪNULL
  //����0xffff��ʾδ�ҵ�
  unsigned short(*LineToAryId)(const struct _TScrnExcelData*,
                                unsigned short Line); //ָ����ת��Ϊ����ID
  unsigned short(*AryIdToLine)(const struct _TScrnExcelData*,
                                unsigned short AryId); //����IDת��Ϊָ����
};

//---------------------------------���ṹ---------------------------------------
struct _TScrnExcel{
  //�û���أ�
  const struct _TScrnExcelData *pData;  //�ҽӵ��û��������ݼ���������
  
  TListboxEx_t TListboxEx;        //�������ڻ���
  TWinHandle_t   hMain;           //���������ڴ��� 
  TWinHandle_t   hHeader;         //����ͷ���ڴ���
  unsigned char Flag;           //��ر�־��������
};

//-��ر�־����Ϊ��
//�û�����λ:
#define   TSCRN_EXCEL_CFG_MASK      0x07
//������ʾ������
#define   TSCRN_EXCEL_EN_SCROLLBAR  0x01
//�˳������ܶ���Ϊ�Զ��˳�-���ӱ����������ʱ�����
#define   TSCRN_EXCEL_EN_EXIT      0x02
//���˳���������Ϊ�Զ��˳�ʱ,���˳���ʱǿ��ִ�лص�����
//�˹������ڲ�����Ϊ�˳���ʱ,���˳�����������λ��
#define   TSCRN_EXCEL_FORCE_EXIT    0x04

//�˵������־(�ڲ�ʹ��)
#define   TSCRN_EXCEL_IN_ENTER     0x80
//�˵����ر�־(�ڲ�ʹ��)
#define   TSCRN_EXCEL_IN_RETURN    0x40
//����׼����ɱ�־,�����ݲ��ܼ�ʱ����ʱ���˱�־����ͬ��(�ڲ�ʹ��)
#define   TSCRN_EXCEL_DATA_FINAL   0x20


/**********************************************************************
                       ��غ���
**********************************************************************/
//-------------------------��ʼ������-------------------------
//ע������Ϊ�Ӵ�����ʾʱ��Ӧ�ÿհ״�����Ϊ�丸����
void TScrnExcel_Init(struct _TScrnExcel *pExcel,
                       const struct _TScrnExcelData *pData, //�û�����
                       TWinHandle_t   hMain,                 //���������ڴ���
                       TWinHandle_t   hHeader,               //����ͷ���ڴ���
                       unsigned char Cfg);                  //�������

//-----------------------------��ʾ���������-------------------------------
#define TScrnExcel_EnScrollBar(pExcel) \
  do{(pExcel)->Cfg |= TSCRN_EXCEL_EN_SCROLLBAR;}while(0)
#define TScrnExcel_DisScrollBar(pExcel)\
  do{(pExcel)->Cfg &= ~TSCRN_EXCEL_EN_SCROLLBAR;}while(0)

//-------------------------��Ӧ��������-------------------------
//�����Ƿ��˳�(��0�˳�)
int TScrnExcel_Key(struct _TScrnExcel *pExcel, unsigned char Key);

//-------------------------�õ���ǰ��ѡ��-------------------------
#define TScrnExcel_GetSel(excel) TListboxEx_GetSel(&excel->TListboxEx)

//-----------------------�õ���ǰ��ѡ���Ӧ������ID-------------------------
unsigned short Excel_GetAryId(const struct _TScrnExcel *pExcel);

//---------------------�õ����λ�ú���------------------------------
#define TScrnExcel_GetCursor(pexcel) TListboxEx_GetSel(&(pexcel)->TListboxEx)

//---------------------�õ���ֱҳ��С����------------------------------
#define TScrnExcel_GetVPage(pexcel) TListboxEx_GetV((&(pexcel)->TListboxEx))

//----------------------------�õ�����������------------------------------
#define TScrnExcel_GetItemCount(pExcel) TListboxEx_GetItems(&(pExcel)->TListboxEx)

//-------------------------�����������ͨ��-------------------------------------
//�Ӻ��������ʱ�����ô˺���˵�����ݸ������
void TScrnExcel_UpdateDataFinal(struct _TScrnExcel *pExcel);

//-------------------------�ؽ�����-------------------------------------
//���¸��µ�ҳ�������ݲ���ʾ
void TScrnExcel_ReBulid(struct _TScrnExcel *pExcel);

//-----------------------------ˢ�º���---------------------------------
void TScrnExcel_Refurbish(const struct _TScrnExcel *pExcel);
    
//----------------------��ת��ָ��AryIdλ�ú���-------------------------
void TScrnExcel_JumpToAryId(struct _TScrnExcel *pExcel,
                            unsigned short AryId);

/**********************************************************************
                       �ص�����
**********************************************************************/

//----------------------��TListboxEx_t�õ���ǰ�ṹ-------------------------
struct _TScrnExcel *pcbGetExcel(const TListboxEx_t *); 

#endif

