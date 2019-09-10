/*******************************************************************************

                           TGUI��Ϣͨ��ģ��

*******************************************************************************/
#ifndef __T_GUI_NOTIFY_H 
#define __T_GUI_NOTIFY_H

/**********************************************************************
                     ˵��
**********************************************************************/
//TGUIʹ��ֱ��ʹ�ûص�����������Ϣͨ��,�ڿؼ���ʼ�������н�����
//һ��ͨ������ָ��,�ʵ���ʱ�򽫵��øú���Ҫ���ϲ������Ӧ����

//��ģ�����ڱ�������Ƭ����֧�ֻ�����ȷ������ָ��ʱ���滻

//Ϊʵ��֧�ֻ�֧�ֺ���ָ���ļ���,���һ���ص���ִ����������:
//1:������ģ��ID�ŷ�����Ϊ�ûص���������һ��ID��
//2:ʹ��TGUI_NOTIFY_PASS�����ص����������������ID��
//3:��TGUINtify.c��ʵ��ID����ص����������ʵ��

//������һ���ص�����ʱ,ʹ��TGUI_NOTIFY_RUN�����Ԥ����

//-------------------------TGUIͨ������ԭ��--------------------------
//����:����ͨ�����ͷ��ص�����
//const void* TGUINotify(void *pvRource,     //��˭������ͨ��
//                       unsigned char Type, //ͨ������
//                       //�����������������ͨ��
//                       const void *pvData);

/**********************************************************************
                               ��ض���
**********************************************************************/
#include"TGUICfg.h"

//--------------------------����ͨ�����Ͷ���--------------------------------
//0-127Ϊϵͳͨ��Ԥ��:

//�õ���ʾͨ��,pvData��Ч,��������ͷ�ַ���
#define   TGUI_NOTIFY_GET_CAPTION   0
//�õ�����ͨ��,pvDataָ��ǰ��ѡ��,���ظ����ַ���  
#define   TGUI_NOTIFY_GET_ITEM      1
//�õ�����ͨ��,pvDataָ��ǰ��ѡ��,Ӧ���ʵ�λ�ô�����,����ֵ��Ч  
#define   TGUI_NOTIFY_GET_FOCUS     2
//ȷ�ϼ�ͨ��,pvDataָ��ؼ�����,����ֵ��Ч(�ڲ����ػ�ͼͨ����־֪ͨ��Щ�������ػ�)  
#define   TGUI_NOTIFY_KEY_ENTER     3
//�˳���ͨ��,pvDataָ��ؼ�����,����ֵ��Ч
#define   TGUI_NOTIFY_KEY_ESCAPE    4  

//128-255Ϊ�û��Զ���ͨ������

//--------------------------��ͼͨ�����Ͷ���----------------------------------
#define TGUI_PAINT_HCONTEXT  0x01    //����ˮƽ����仯
#define TGUI_PAINT_VCONTEXT  0x02    //���ݴ�ֱ����仯
#define TGUI_PAINT_FOCUS     0x04    //���㷢���仯
#define TGUI_PAINT_HEADER    0x08    //����ͷ�����仯
#define TGUI_PAINT_DATA      0x10    //���ݷ����仯

/**********************************************************************
                      �ص�����ID�ŷ���
//��Χ:0-255,��ִ��ʱ��Ч�ʷ���:,Խ��ǰִ��Ч��Խ��
**********************************************************************/

//�ڲ��ؼ��ص�����ID�ŷ���,
#define   TGUI_CBFUNID_LISTBOX_EX   0
#define   TGUI_CBFUNID_TEXT         1
#define   TGUI_CBFUNID_CHECKBOX     2
#define   TGUI_CBFUNID_RADIO_BUTTON 3

//�ⲿ�ؼ�ID�ŷ���:16��ʼ:
#define   TGUI_CBFUNID_EX     4 //�����ⲿ��ID��ʼλ��

#define   TGUI_CBFUNID_TMENU_CHECKBOX         (TGUI_CBFUNID_EX + 0)
#define   TGUI_CBFUNID_TMENU_MNUM_LISTBOX     (TGUI_CBFUNID_EX + 1)
#define   TGUI_CBFUNID_TMENU_MNUM_EDIT        (TGUI_CBFUNID_EX + 2)
#define   TGUI_CBFUNID_TMENU_RADIO_BUTTON     (TGUI_CBFUNID_EX + 3)
#define   TGUI_CBFUNID_TMENU_SUB_MENU         (TGUI_CBFUNID_EX + 4)

/**********************************************************************
                         TGUI��Ϣͨ�����滻
**********************************************************************/
//���ǵ��൥Ƭ����������Ժ���ָ��֧�ֲ����������֧��,ʹ�����к�ʵ������:

//ʹ�����к괦��ͨ�������Ĵ�������ִ��:
#ifdef TGUI_DIS_P_FUN //��ֹʹ�ú���ָ��ʱ
  //1:����TGUIͨ����������,��ID�������滻ͨ������:
  typedef unsigned char TGUINotify_t ;
  //2:����ͨ��������Ԥ����,TGUINotifyFunΪͨ���������
  //IDΪΪÿһ���ؼ���(�û�����ģ��)�����Ψһ���
  #define TGUI_NOTIFY_PASS(TGUINotifyFun,ID) (ID) 
  //3:ͨ��������ִ�к�
  #define TGUI_NOTIFY_RUN(TGUINotifyFun,pvRource,Type,pvData) \
    TGUIpFunReplace(pvRource,Type,pvData,TGUINotifyFun)
    
#else   //֧�ֺ���ָ��ʱ
  //1:����TGUIͨ����������
  typedef const void*(*TGUINotify_t)(void *pvRource,     //��˭������ͨ��
                                     unsigned char Type, //ͨ������
                                     //�����������������ͨ��
                                     const void *pvData);
  //2:����ͨ��������Ԥ����,TGUINotifyFunΪͨ���������
  //IDΪΪÿһ���ؼ���(�û�����ģ��)�����Ψһ���
  #define TGUI_NOTIFY_PASS(TGUINotifyFun,ID) (TGUINotifyFun) 
  //3:ͨ��������ִ�к�
  #define TGUI_NOTIFY_RUN(TGUINotifyFun,pvRource,Type,pvData) \
    TGUINotifyFun(pvRource,Type,pvData)
#endif

//----------------TGUI��Ϣ���ݲ�֧�ֺ���ָ����滻����------------------

#ifdef TGUI_DIS_P_FUN //��ֹʹ�ú���ָ��ʱ
const void* TGUIpFunReplace(void *pvRource,     //��˭������ͨ��
                            unsigned char Type, //ͨ������
                            //�����������������ͨ��
                            const void *pvData,
                            TGUINotify_t ID);   // �ص�����ID��

#else //����ʹ�ú���ָ��ʱ,�ú���������
#define TGUIpFunReplace(pvRource,Type,pvData,ID) do{}while(0)
#endif


#endif

