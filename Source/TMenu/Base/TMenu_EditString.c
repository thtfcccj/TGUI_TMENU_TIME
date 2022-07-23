/*******************************************************************************

                  TMenu�˵�֮�ַ����༭�ӿ�(֧�����뷨ʱר��)

*******************************************************************************/
#include "TMenu.h"
#include "TMenu_EditString.h"
#include <string.h>

//------------------------------��������ʵ��-------------------------------
//1:���������Ͳ˵�,���û���������ݲ�������ʾ
//���ش����ɹ�:
signed char TMenu_EditStringCreate(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                                 TWinHandle_t hWin,       //������˵��Ĵ���
                                 unsigned char Data,    //�������������,����������
                                 void *pv)      //�ɹ�ʹ�õ����ɻ�����
{
  struct _EditStringData *pEditStringData = (struct _EditStringData *)pv;
  //���û��ռ��ò����õ�ǰ��ѡ��
  pEditStringData->User.DefaultCursor = (TIme_String_t)-1;//Ĭ�������
  
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_DATA,&pEditStringData->User);
  pEditStringData->pImeMng = TMenu_EditString_cbpGetImeMng();
  
  if(TImeMng_Init(pEditStringData->pImeMng,  //��������뷨�ṹ����
                  TWM_pGetWin(hWin),             //��ǰ�ҽӵĴ���
                  pEditStringData->User.pString, //�û��Ѵ��ڵ��ַ���,������'\0'��β
                  pEditStringData->User.Size,     //�ַ��������������С
                  pEditStringData->User.DefaultType,   //Ĭ�����뷨,<4
                  pEditStringData->User.TypeMask,//��ʹ�õ����뷨���� 
                  (const char*)(pMenu->pv), //�ҽӵķ��ű�,Ϊ��ʱʹ��Ĭ��
                  pEditStringData->User.DefaultCursor))//Ĭ�Ϲ��λ��
    return 0;//�������ɹ�!
  return -1;
}

//------------------------��Ӧ��������ʵ��----------------------------
//2:��Ӧ��������������������û��������ݽ���,��������ʾ
//����ֵ����Ϊ: ��ֵ:������Ӧ�Ӳ˵�,-1,����,-2:�����ϲ�˵�
signed char TMenu_EditStringKey(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                              unsigned char Key,       //����ļ�ֵ
                              void *pv)      //�ѱ�ʹ�õ��������ݻ�����
{
  struct _EditStringData *pEditStringData = (struct _EditStringData *)pv;
  //PushMenu(pMenu);
  //PushData(pv);
  switch(TImeMng_Key(pEditStringData->pImeMng,Key)){
  //case 0: return -1;//����״̬
  case -1://�˳���ֱ���˳�      
    TMENU_NOTIFY_RUN(pMenu->cbNotify,
                     TM_NOTIFY_CHANGED,
                     &pEditStringData->User);
    break;
  case -2://ȷ�ϼ��˳�      
    TMENU_NOTIFY_RUN(pMenu->cbNotify,
                     TM_NOTIFY_SET_DATA,
                     &pEditStringData->User);
    break;
  default:return -1;//����״̬
  }
  return -2;
}

