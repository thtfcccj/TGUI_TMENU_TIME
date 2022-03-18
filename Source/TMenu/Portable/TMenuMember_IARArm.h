/**************************************************************************
             TMenu�ṹʵ�ּ���س�Ա����������KeilC51�е�ʵ�����
//��ԭ��Ĭ�Ͻṹ�Ļ�����,������ǿ�ƶ������֧�����ѡ������ұ�֧��
**************************************************************************/
#ifndef __T_MENU_MEMBER_IAR_ARM
#define __T_MENU_MEMBER_IAR_ARM

#include "TMenuCfg.h" 
#include "TMenuMember_Lan.h" 

//����Ӧ��,����˵���������:
struct _TMenu{
  unsigned char Type;      //�˵����ͼ���ر�־
  unsigned char Size;       //�ɲ˵����;�����������ݴ�С
  const LanCode_t *pgHeader; //�˵�ͷ,ΪNULLʱ�ӻص����ȡ
  const struct _TMenu *pParent;   //���ѵĸ��˵�
  const void *pv;           //���ݲ˵�����ȷ�������ɲ���ָ��
  TMenuNotify_t cbNotify;     //���û��ռ佻����ͨ������
};

//Ϊֻ��EditSelʱ,ֻ��Ҫ�˵�ͷ���ɣ�
struct _TMenuSelRO{
  unsigned char Type;      //�˵����ͼ���ر�־
  unsigned char Size;       //�ɲ˵����;�����������ݴ�С
  const LanCode_t *pgHeader; //�˵�ͷ,ΪNULLʱ�ӻص����ȡ  
};

/********************************************************************
                       ��ѡ������ұ�ṹ֧��
********************************************************************/

#ifdef TM_EN_LUT   //���ұ�֧��ʱ:
  //����Size�ṹ���λ��ʾ�ýṹ��������ұ�
  #define   TM_TYPE_ITEM_LUT     0x80
  
  //��������ұ�ʱ,�û�����������Ӧ�����û�ͨ����ȡ���ұ�ṹ(��䴫���pvָ��)
  #define    TM_NOTIFY_USER_GET_LUT    (TM_NOTIFY_USER_BASE + 0)
  //���ұ�ṹ����Ϊһ������:����
  //��λ: �˲��ұ��С,<= struct _TMenu�е�Size
  //����˲��С������,��
  //static code const unsigned char _LUT = {2,  0,3}; 
  
  //������������,���ڲ��ڽ����ұ���
  #define   TM_ITEM_MAX         10

  //-------------------���ݵ�ǰ�˵����ұ�����ѡλ��---------------------
  unsigned char TMenu_GetItemPosWithLUT(unsigned char Pos);

#endif //#ifdef TM_EN_LUT


/********************************************************************
                       ��ѡ�Ķ�̬�˵�����֧��
********************************************************************/

#ifdef TM_DYNC_MENU   //��̬�˵�֧��ʱ:  
  
  //���¸��˵�ͨ�� ���¸��˵��ṹ(pvָ������)
  #define    TM_NOTIFY_USER_UPDATE_PARENT    (TM_NOTIFY_USER_BASE + 1)

  //�����Ӳ˵�ͨ�� ����ָ���Ӳ˵��ṹ(pvָ��ָ��ǰ�Ӳ˵���ID��)
  #define    TM_NOTIFY_USER_UPDATE_SUB    (TM_NOTIFY_USER_BASE + 2)
  
#endif //#ifdef TM_DYNC_MENU

/********************************************************************
                       Ӧ�ò�ص�����
********************************************************************/

//-----------------------�õ��û�����˵��ṹ---------------------------
const TMenu_t *TM_pcbGetUserTopMenu(void);
  

#endif //__T_MENU_MEMBER_IAR_ARM


