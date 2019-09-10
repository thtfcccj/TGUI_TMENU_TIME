/*******************************************************************************

                           TWidget֮�б����չ�ؼ�

*******************************************************************************/
#ifndef __T_LISTBOX_EX_H 
#define __T_LISTBOX_EX_H

/*******************************************************************************
                                �ؼ�˵��
*******************************************************************************/
//�򲿷ֱ�������ʾ��ʽ��ͬ,���еĴ�����ʾ����������,���������ı��༭���������!
//������ʾΪ�б����չ�ؼ���ʾ:
//  ������(��ѡ��ʾͷ)��������
//  ��  ����1.(��һ��)      ��
//  �� ���� 2.(�ڶ���)      ��
//  ��   [ȷ��]    [����]   ��
//  ��������������������������

//�������б������ؼ�������,�������������Ժ���ʾ��ʽ:
//1: �ı����뷽ʽ������
//2: ����(����λ��)��ʾ��ʽ������
//3: ������ǰ��׺,���ɾ���ǰ��׺�Ƿ������
//4: ����������ǰ���Զ����������ʾ����
//5: ���һ���û����ӹ���(�������û�������ʾ������ɵĹ���)

//�̳й�ϵ:
//TListBoxEx�̳й�ϵΪ:
//  TListBoxEx->TListBox->TWidget->(��Ӽ̳�)TWin
//ֱ�Ӽ̳�ʱ��ֱ�ӽ��ýṹת��Ϊ����ṹ�󲢵��û�����غ���
//��Ӽ̳�ͨ���ýṹ�ṩ�ļ��ת������ת��Ϊ����ṹ���ɵ��û���ṹ����
//
//��ؼ��Ѱ�װ��,�һ���������˵����(ע�ʹ����־),
//����ֱ�Ӳ����������ܻ�Ӱ��ϵͳ����,�ʲ�����ֱ��ʹ�û������

/**********************************************************************
                        ��ؽṹ
**********************************************************************/
#include "TListbox.h"

typedef struct{
  TListbox_t Listbox;           //����
  unsigned char Style;         //��ʽ
  unsigned char Flag;           //����ڲ���־,������
  TItemSize_t  ValidW;          //��Ч�ַ����(������������̵Ĳ����Ŀո�)
  TGUINotify_t cbGUINotify;    //��չͨ������
}TListboxEx_t;

//ͨ������Ӧ��Ӧ��������ͨ��:
//TGUI_NOTIFY_GET_CAPTION,TGUI_NOTIFY_GET_ITEM
//TGUI_NOTIFY_KEY_ENTER,TGUI_NOTIFY_EKY_ESCAPE
//���и�����ʱ,��������ͨ����������:
//TGUI_NOTIFY_GET_FOCUS
//����,�����и���ʱ,TGUI_NOTIFY_GET_ITEM��ޱ�ʾ��ø���������
//�������������в�����ʽ����,�ɻص���������

//ע:�������в�ֻһ������ʱ,Ӧע�ⰴ����������Ԥ�ȴ���

//�˿ؼ��Զ���ͨ��Ϊ:
#define   TLISTBOXEX_NOTIFY_PREFIX   128    //����ǰ׺ͨ��
#define   TLISTBOXEX_NOTIFY_SUFFIX   129    //���Ӻ�׺ͨ��
//ͨ������Ӧ��Ӧ�Զ���ͨ��

//--------------------------�ı���ʽ------------------------
//����,��ʽ����Ϊ:
//���ֶ��뷽ʽ:(0-1bit)
#define   TLISTBOXEX_ALIGN_MASK      0x03    //����λ
#define   TLISTBOXEX_ALIGN_LEFT      0x00    //�����
#define   TLISTBOXEX_ALIIGN_CENTER   0x01    //���ж���
#define   TLISTBOXEX_ALIGN_RIGHT     0x02    //�Ҷ���
#define   TLISTBOXEX_ALIGN_LEN       0x03    //��ȶ���,�������Ϊ��־����
//���Է�ʽ:(2-3)
#define   TLISTBOXEX_AUTI_MASK       0x0c    //����λ
#define   TLISTBOXEX_ANTI_ROW        0x00    //���з���
#define   TLISTBOXEX_ANTI_WORD       0x04    //�����ַ���,����ǰ��׺
#define   TLISTBOXEX_ANTI_WORD2      0x08    //�����ַ���,��������ǰ��׺
#define   TLISTBOXEX_AUTI_DIS        0x0c    //��ֹ������ʾ
//������־
#define   TLISTBOXEX_PREFIX          0x10     //����ǰ׺,��ͨ�������
#define   TLISTBOXEX_SUFFIX          0x20     //���Ӻ�׺,��ͨ�������
#define   TLISTBOXEX_EN_NO           0x40    //���������ʾ
#define   TLISTBOXEX_EN_APPEND       0x80    //�������Ӹ�����
//ע:������Flag����򷵻���δʹ��ʱ,����д������,��ʱӦ�ڸ���
//��ص�������,�û�Ӧ����ʵ�ֽ���,��������ʾ����Ӧ

//--------------------------��־------------------------
//��Flag�︽���������Զ����ѡ����,�������ʹ��ʱ,
#define TLISTBOXEX_EN_SAVE     0x01      //�����������
#define TLISTBOXEX_EN_RETURN   0x02      //�����������

/**********************************************************************
                          ���Ժ���
**********************************************************************/
//��õ���ǰ��ѡ��
#define TListboxEx_GetSel(pListbox) TWidget_GetSel((TWidget_t*)(pListbox))
//���õ�ǰ��ѡ��,���ڴ�����ʹ��
#define TListboxEx_SetSel(pListbox,Sel) \
  do{TWidget_SetSel((TWidget_t*)(pListbox),Sel);}while(0)
//��õ�������
//#define TListboxEx_GetItems(pListbox) TWidget_GetItems((TWidget_t*)(pListbox))
TItemSize_t TListboxEx_GetItems(TListboxEx_t *pListboxEx);
//��������������
#define TListboxEx_SetItems(pListbox,Items)\
  TWidget_SetItems(((TWidget_t*)(pListbox)),Items)
//��õ�ÿ���׸��ַ�����ʼλ��
#define TListboxEx_GetX(pListbox) TWidget_GetX((TWidget_t*)(pListbox))     
//��õ�ÿ���ַ�����(��ռλ��)
#define TListboxEx_GetW(pListbox) TWidget_GetHPageSize((TWidget_t*)(pListbox))
//��õ�����ʼλ��
#define TListboxEx_GetY(pListbox) TWidget_GetY((TWidget_t*)(pListbox))  
//��õ���߶�(��һ������ʾ����)
#define TListboxEx_GetH(pListbox) TWidget_GetVPageSize((TWidget_t*)(pListbox))      
//�ж��Ƿ��и�����
#define TListboxEx_HaveAppend(pListbox) ((pListbox)->Style & TLISTBOXEX_EN_APPEND)
/**********************************************************************
                          ��Ϊ����
**********************************************************************/

//-------------------------��ʼ������-----------------------------
//ע:���Ǻܶ�ϵͳ��ʹ�þ�̬�ڲ����,�ʽ�����������������һ��,��
//���ڴ˻������������Ĵ�������
void TListboxEx_Init(TListboxEx_t *pListboxEx,  //���û�����Ĵ洢�ռ�
                     TWinHandle_t hWin,         //�ѽ�������ʼ���õĴ���
                     TItemSize_t Items,         //Ԥ������Ŀ����
                     TItemSize_t ItemW,         //�ַ������
                     unsigned char Flag,        //�ؼ���־
                     unsigned char Style,       //��ʾ��ʽ
                     TGUINotify_t cbGUINotify); //ͨ������

//-------------------------�����ػ����к���-------------------------
//�Դ�Ϊ������Ӧ�ó���(�Ǽ̳���)��ʼ����ɺ���ô˺����ػ�����
//ע�����ļ���ʽΪ���ȶ���ʱ��TListboxEx�̳����ʼ����ɺ�����ô˺�����ʾ
#define TListBoxEx_PaintAll(pListboxEx) \
  do{ListBox_PaintAll((TListbox_t *)(pListboxEx));}while(0)


//-------------------------����������-----------------------------
void TListboxEx_Key(TListboxEx_t *pListboxEx,unsigned char Key);

//-------------------------�б��ص�ʵ�ֺ���-----------------------------
//�����ڲ�ͨ��ʹ��
const void*TListBox_Notify(void *pv,
                           unsigned char Type,
                           const void *pvData);


#endif //#ifndef __T_LISTBOX_EX_H 

