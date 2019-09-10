/*******************************************************************************

                           TWidget֮�б��ؼ�

*******************************************************************************/
#ifndef __T_LISTBOX_H 
#define __T_LISTBOX_H

/*******************************************************************************
                                �ؼ�˵��
*******************************************************************************/
//������ʾΪ�б��ؼ���ʾ:
//  ������(��ѡ��ʾͷ)��������
//  ��  ����  (��һ��)      ��
//  �� ����   (�ڶ���)      ��
//  �� ����   (�ڶ���)    ����
//  ��������������������������

//�˿ؼ���TWidget�����Ϲ���,��Ҫʵ����TWidget���û�������(ItemsRect)(��TWindgetNote.h)
//�����,�Ͱ�������Ӧ,���ڴ˲���ʵ�ֵĻص������ϲ���򽻻�,���Զ�������

//�̳й�ϵ:
//TListBox�̳й�ϵΪ:
//  TListBox->TWidget->(��Ӽ̳�)TWin
//ֱ�Ӽ̳�ʱ��ֱ�ӽ��ýṹת��Ϊ����ṹ�󲢵��û�����غ���
//��Ӽ̳�ͨ���ýṹ�ṩ�ļ��ת������ת��Ϊ����ṹ���ɵ��û���ṹ����
//
//��ؼ��Ѱ�װ��,�һ���������˵����(ע�ʹ����־),
//����ֱ�Ӳ����������ܻ�Ӱ��ϵͳ����,�ʲ�����ֱ��ʹ�û������

/**********************************************************************
                             ��ؽṹ
**********************************************************************/
#include "TWidget.h"

typedef struct{
  TWidget_t Widget;   //����
  TGUINotify_t cbGUINotify;    //ͨ������
}TListbox_t;

//ͨ������Ӧ��Ӧ��������ͨ��:
//TGUI_NOTIFY_GET_CAPTION,TGUI_NOTIFY_GET_ITEM,TGUI_NOTIFY_GET_FOCUS
//TGUI_NOTIFY_KEY_ENTER,TGUI_NOTIFY_EKY_ESCAPE;

/**********************************************************************
                          ���Ժ���
**********************************************************************/
//��õ���ǰ��ѡ��
#define TListbox_GetSel(pListbox) TWidget_GetSel((TWidget_t*)(pListbox))
//���õ�ǰ��ѡ��,���ڴ�����ʹ��
#define TListbox_SetSel(pListbox,Sel) \
  TWidget_SetSel(((TWidget_t*)(pListbox)),Sel)
//��õ�������
#define TListbox_GetItems(pListbox) TWidget_GetItems((TWidget_t*)(pListbox))
//��������������
#define TListbox_SetItems(pListbox,Items)\
  TWidget_SetItems(((TWidget_t*)(pListbox)),Items)
//��õ�����
#define TListbox_GetW(pListbox) TWidget_GetW((TWidget_t*)(pListbox))
//��Ԥ������
#define TListbox_SetW(pListbox,w) TWidget_SetW(((TWidget_t*)(pListbox)),w)
//��õ�����ʾ���
#define TListbox_GetDispW(pListbox) TWidget_GetHPageSize((TWidget_t*)(pListbox))

/**********************************************************************
                          ��Ϊ����
**********************************************************************/

//-------------------------��ʼ������-----------------------------
//ע:���Ǻܶ�ϵͳ��ʹ�þ�̬�ڲ����,�ʽ�����������������һ��,��
//���ڴ˻������������Ĵ�������
void TListbox_Init(TListbox_t *pListbox,  //���û�����Ĵ洢�ռ�
                   TWinHandle_t hWin,     //�ѽ�������ʼ���õĴ���
                   unsigned char Flag,    //�ؼ���־
                   TItemSize_t Items,         //Ԥ������Ŀ����
                   TGUINotify_t cbGUINotify); //ͨ������

//-------------------------����������-----------------------------
void TListbox_Key(TListbox_t *pListbox,unsigned char Key);

//-------------------------���ڻ�ͼ����-----------------------------
void ListBox_Paint(TListbox_t *pListbox,    //����ؼ�
                   //���Ʊ�־,���������������ض���,0xff��δ��������
                   unsigned char PaintFlag);

//-------------------------�����ػ����к���-------------------------
//�Դ�Ϊ������Ӧ�ó���(�Ǽ̳���)��ʼ����ɺ���ô˺����ػ�����
#define  ListBox_PaintAll(pListbox) \
  do{ListBox_Paint(pListbox,0xff);}while(0)


//-------------------------���ý��㺯��-----------------------------
//��ֱ��ʹ�û���ʵ��,�����̳���ʹ��
#define TListbox_SetFocus(pListbox,y,x,w) \
  do{TWidget_SetFocus((TWidget_t*)(pListbox),(y),(x),(w));}while(0)

#endif

