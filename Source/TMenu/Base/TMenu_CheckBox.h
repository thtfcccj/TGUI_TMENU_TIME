/*******************************************************************************

                  TMenu�˵�֮��ѡ��ؼ��ӿ�

*******************************************************************************/
#ifndef __T_MENU_CHECK_BOX_H 
#define __T_MENU_CHECK_BOX_H

#include "TMenuBase.h"

/**********************************************************************
                      ������
**********************************************************************/
//���帴ѡ��ťĬ����ʽ(��TWidget.h�븴ѡ�ؼ�):
#define   TM_CHECK_BOX_STYLE_DEFAULT    \
  (TWIDGET_EN_HEADER | TWIDGET_EN_VSCOLL | TWIDGET_EN_NOTIFY)

//ע:���õ���:
//#define   TWIDGET_EN_FRAME    0x80  //�ù������б߿�,��LC�߶�>-3��ʱ������Ч
//#define   TWIDGET_EN_HEADER   0x40  //������ʾ����ͷ
//#define   TWIDGET_EN_HSCOLL   0x20  //������ʾˮƽ������
//#define   TWIDGET_EN_VSCOLL   0x10  //������ʾ��������
//#define   TWIDGET_EN_NOTIFY   0x04  //�޹�������߿�ʱ�������û���ʱ��ʾ��ʾ��Ϣ
//#define   TCHECKBOX_EN_RETURN  0x02      //�����������

/**********************************************************************
                     �û�ʹ�����
**********************************************************************/
//��Ϊ��ѡ��ʱ,�˵��ṹ�����¶���:
//typedef struct _TMenu{
//  TMTYPE_CHECKBOX;        //�˵�����Ϊ��ѡ��ģʽ
//  unsigned char Size;     //�ɹ�ѡ��ĵ��������
//  LanguageCode_t *pgHeader; //�˵�ͷ,ΪNULLʱ�ӻص����ȡ
//  struct _TMenu *pParent;   //���ѵĸ��˵�
//  const LanguageCode_t*;   //���ÿһ�������������(�ṹ)����
//  TMenuNotify_t Notify;     //���û��ռ佻����ͨ������
//}TMenu_t;

//��ѡ��ģʽ�û�����־Ϊ
#define   TM_CHECKBOX_EN_SAVE   0x80  //�б��水ť��־

//�û������ݽṹ:
struct _CheckboxUser{ 
 //�����û���ѡ��Ӧ���ѡ��״̬
 unsigned char SelMask[(TCHECKBOX_ITEM_COUNT + 7) >> 3]; 
 //֪ͨ�û������ݱ�����ʱ,��֪����ı���,�û�ֻ��
 TItemSize_t ChanegedItem;     
};
//�û��ص�������Ӧ���ͼ���ʵ�ֵĹ�����:
//1:����Ϊ:TM_NOTIFY_GET_DATAʱ,�谴�˵��������˳��һ��һ��װ��SelMask��
//2:����Ϊ:TM_NOTIFY_CHANGEDʱ,��ʾChanegedItemָ������ı���
//�û��ɸ���Ӧ��Ҫ������Ƿ���Ӧ(��:�����ֱ�Ӹ���Ӧ��)
//3:����Ϊ:TM_NOTIFY_SET_DATAʱ,��ʾ���޸����,�û�Ӧ���б���Ȳ���

/**********************************************************************
                           �ڲ����ݽṹ
**********************************************************************/
struct _CheckboxData{
 TCheckbox_t  Checkbox;         //�ṹ����
 unsigned char Flag;            //�ڲ���־
 struct _CheckboxUser User;     //�û�����
};

/******************************************************************************
                           ��غ���
******************************************************************************/
//------------------------��������ʵ��----------------------------
//1:���������Ͳ˵�,���û���������ݲ�������ʾ
//���ش����ɹ�:
signed char TMenu_CheckBoxCreate(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                                  TWinHandle_t hWin,       //������˵��Ĵ���
                                  unsigned char Data,    //�������������,����������
                                  void *pv);      //�ɹ�ʹ�õ����ɻ�����

//------------------------��Ӧ��������ʵ��----------------------------
//2:��Ӧ��������������������û��������ݽ���,��������ʾ
//����ֵ����Ϊ: ��ֵ:������Ӧ�Ӳ˵�,-1,����,-2:�����ϲ�˵�
signed char TMenu_CheckBoxKey(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                              unsigned char Key,       //����ļ�ֵ
                              void *pv);      //�ѱ�ʹ�õ��������ݻ�����

//-------------------------��ѡ��ͨ���ص�ʵ�ֺ���-----------------------------
//�����ڲ�ͨ��ʹ��
const void*TMenu_CheckBoxNotify(void *pvPource,
                                unsigned char Type,
                                const void *pvData);

/******************************************************************************
                      �ص�����
******************************************************************************/

//----------------------�õ���ǰ��ʾ��ʽ-------------------------
#ifdef TMENU_STYLE_VARIABLE   //�ɱ���ʽʱ���û�������
  unsigned char TMenu_CheckBox_cbGetCurStyle(void);
#else
  #define TMenu_CheckBox_cbGetCurStyle() \
    (TM_CHECK_BOX_STYLE_DEFAULT | TCHECKBOX_EN_RETURN)
#endif

#endif

