/*******************************************************************************

                           TWidget֮�༭��
TEdit��һ����TWIN֮�ϵ��б༭��,���ַ���Ϊ������ʾ��Ԫ
*******************************************************************************/
#ifndef __T_EDIT_H 
#define __T_EDIT_H

/**********************************************************************
                            ��ؽṹ
**********************************************************************/
#include "TWM.h"     //���ڹ�����
#include "TGUIBase.h"    //��������

//�༭����������Ͷ���:
enum TEditType_t{
  TEditType_Dec,    //ʮ������ģʽ
  TEditType_Float,  //������ģʽ
  //TEditType_Hex,    //ʮ��������ģʽ
  //TEditType_Bin,    //��������ģʽ
  //ע���ַ���ʽ���ڴ˱༭����ʵ��
};

typedef struct _TEdit{
  TWinHandle_t hWin;      //����
  enum TEditType_t eType;	  //����ģʽ
  unsigned char Pos;      //���λ��
  unsigned char Flag;     //��־,������
  //����������
  unsigned char DotPos;	  //С�����λ��
  unsigned char Len;      //������λ��
  TSigned_t Value;       //��ǰ������
  TSigned_t Max;          //���������ֵ,
  TSigned_t Min;          //��������Сֵ
  TGUINotify_t cbGUINotify; //ͨ������
}TEdit_t;

//���б�־����Ϊ:
//����:
#define TEDIT_EN_SIGN     0x80    //������ʾ����,��ֵ����ʱ
#define TEDIT_GRP_MODE    0x40    //Ⱥ����ģʽ,����Ϊ��������ģʽ
#define TEDIT_EN_NOTIFY   0x20    //�Ƿ�������ʾ<>��ʾ,��:<-XXX.XX>
#define TEDIT_ENTER_RIGHT 0x10    //�����Ƿ�ȷ�ϼ�����Ϊ�Ҽ�,��ϵͳ�����Ҽ�ʱ��������ֵ��λ
#define TEDIT_PASS_IN     0x08    //�ַ����༭ģʽʱ,Ϊ������ʾģʽ,����ʾ*

//����Ӧ��ϵͳͨ��:
//TGUI_NOTIFY_KEY_ENTER  ȷ�ϲ��˳�
//#define   TGUI_NOTIFY_KEY_ESCAPE  //��ȷ���˳�
//�˿ؼ��Զ���ͨ��Ϊ:
#define   TEDIT_NOTIFY_LEFT_END   128    //������߽�ͨ��,�����������ֵ
#define   TEDIT_NOTIFY_RIGHT_END  129    //�����ұ߽�ͨ��,�����������ֵ
//ͨ������Ӧ��Ӧ�Զ���ͨ��

/**********************************************************************
                         ��Ա��������
**********************************************************************/
//��õ����ڵĴ���
#define TEdit_hGetWin(pEdit)  ((pEdit)->hWin)
//��õ���־
#define TEdit_GetFlag(pEdit)  ((pEdit)->Flag)
//���ж��Ƿ���Ⱥ����ģʽ
#define TEdit_IsGrpMode(pEdit)  ((pEdit)->Flag & TEDIT_GRP_MODE)

/**********************************************************************
                          �����Ϊ����
**********************************************************************/
//-----------------�༭��ʮ��������ʼ������----------------
void TEditDec(TEdit_t *pEdit,
              TWinHandle_t hWin,    //�����Ĵ���,���ݻ�����>Len
              TSigned_t Value,            //��ʼ��ֵ
              TSigned_t Max,              //���ֵ,������ֵ����
              TSigned_t Min,              //��Сֵ
              unsigned char DotPos,  //С����λ��,������������ģ��С����ʾ
              unsigned char Flag,  //�������
              TGUINotify_t cbGUINotify); //ͨ������

//----------------�õ��������������--------------------
//��õ�ǰ�ѵ�����ɵ���(��������ʹ��TData2Float)ת���õ�
TSigned_t TEdit_GetVolume(const TEdit_t *pEdit);

//-----------------�༭���ַ����༭��ʼ������----------------
void TEditString(TEdit_t *pEdit,
              TWinHandle_t hWin,    //�����Ĵ���,���ݻ�����>Len
              char *pString,        //��༭���ַ���
              unsigned char Len,    //�ַ����ɱ༭����
              unsigned char Flag,  //�������
              TGUINotify_t cbGUINotify); //ͨ������

//-----------------------����������------------------------------
void TEdit_Key(TEdit_t *pEdit,unsigned char Key);

//-------------------------��ͼ����--------------------------------
void TEdit_PaintAll(TEdit_t *pEdit);

#endif

