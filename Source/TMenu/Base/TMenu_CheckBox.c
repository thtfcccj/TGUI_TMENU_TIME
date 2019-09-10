/*******************************************************************************

                  TMenu�˵�֮��ѡ��ؼ�ʵ��

*******************************************************************************/
#include "TMenu.h"
#include "TMenu_Checkbox.h"
#include <string.h>

//����:��־����Ϊ:
#define _FLAG_RETURN    0x01      //�ô˱�־��Ϊ���˳���һ��
#define _FLAG_ENTER     0x02      //ȷ�ϼ�

//ͨ������Ӧ��Ӧ��������ͨ��:
//TGUI_NOTIFY_GET_CAPTION ��ñ���
//TGUI_NOTIFY_GET_ITEM ��������ʾ����
//TGUI_NOTIFY_KEY_ENTER ÿһ���ȷ�ϼ�,��ʱӦ���ѡ����񲢽���Ӧ�ò㴦��
//������趨ϣ��,��ʾ�ڸ��ӵı��潹����
//TGUI_NOTIFY_EKY_ESCAPE �˳�����һ��

//-------------------------��ѡ��ͨ���ص�ʵ�ֺ���-----------------------------
//�����ڲ�ͨ��ʹ��
const void*TMenu_CheckBoxNotify(void *pv,
                                unsigned char Type,
                                const void *pvData)
{
  const TMenu_t *pMenu = pPopMenu();
  TItemSize_t Item = *(TItemSize_t*)pvData;   //��ǰ��λ��
  struct _CheckboxData *pCheckboxData = (struct _CheckboxData*)pPopData();
  
  pv = pv;//��ֹ���ֱ���������
  //�õ�ϵͳ��ǰָ������ݻ�����
  switch(Type){
  case TGUI_NOTIFY_GET_CAPTION://��ñ���
    return TM_GetHeader(pMenu);
  case TGUI_NOTIFY_GET_ITEM:// ��������ʾ����
    return TM_GetItemString(pMenu,Item);
  case TGUI_NOTIFY_KEY_ENTER://ÿһ���ȷ�ϼ�
    pCheckboxData->User.ChanegedItem = Item;
    pCheckboxData->Flag |= _FLAG_ENTER;//�˳�����
    break;
  case TGUI_NOTIFY_KEY_ESCAPE://�˳�����һ��
    pCheckboxData->Flag |= _FLAG_RETURN;
  default: break;
  }
  return NULL;
}

//------------------------��������ʵ��----------------------------
//1:���������Ͳ˵�,���û���������ݲ�������ʾ
//���ش����ɹ�:
signed char TMenu_CheckBoxCreate(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                                  TWinHandle_t hWin,       //������˵��Ĵ���
                                  unsigned char Data,    //�������������,����������
                                  void *pv)      //�ɹ�ʹ�õ����ɻ�����
{
  unsigned char Flag;
  struct _CheckboxData *pCheckboxData = (struct _CheckboxData *)pv;
  TCheckbox_t  *pCheckbox = &pCheckboxData->Checkbox;
  //�õ����
  TItemSize_t ItemW = TM_GetItemMaxLen(pMenu);

  pMenu = pMenu;
  
  if(TM_GetType(pMenu) & TM_CHECKBOX_EN_SAVE)//�б����ʱ
     Flag =  TMenu_CheckBox_cbGetCurStyle() | TCHECKBOX_EN_SAVE;
  else Flag =  TMenu_CheckBox_cbGetCurStyle();

  Data = TM_GetSize(pMenu);//��ʱ��ʾ��С
  //ʹ�ø�ѡ��ʵ��:
  TCheckbox_Init(pCheckbox,
                    hWin,//�ѽ�������ʼ���õĴ���
                    Data,//Ԥ������Ŀ����
                    ItemW,         //Ԥ��������(���������)
                    Flag,//�ؼ���־,��������
                    TGUI_NOTIFY_PASS(TMenu_CheckBoxNotify,TGUI_CBFUNID_TMENU_CHECKBOX)); //ͨ������
  
  //���û��ռ��ò����õ�ǰ��ѡ��
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_DATA,&pCheckboxData->User);
  
  TCheckbox_SetAllCheck(pCheckbox,pCheckboxData->User.SelMask,Data);
                        
  //��ʼ����������
  pCheckboxData->Flag = 0;
  
  //������ʾ
  TMenu_ReBulidWin((TListboxEx_t *)pCheckbox,//���ô��ڴ�С
                   ItemW,
                   Data,
                   strlen(TM_GetHeader(pMenu)));
  TCheckbox_PaintAll(&pCheckboxData->Checkbox);
  TCheckbox_PaintAll(&pCheckboxData->Checkbox);//���ȶ����ػ�����  
  return -1;
}

//------------------------��Ӧ��������ʵ��----------------------------
//2:��Ӧ��������������������û��������ݽ���,��������ʾ
//����ֵ����Ϊ: ��ֵ:������Ӧ�Ӳ˵�,-1,����,-2:�����ϲ�˵� 
signed char TMenu_CheckBoxKey(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                              unsigned char Key,       //����ļ�ֵ
                              void *pv)      //�ѱ�ʹ�õ��������ݻ�����
{
  unsigned char Data;
  struct _CheckboxData *pCheckboxData = (struct _CheckboxData *)pv;
  PushMenu(pMenu);
  PushData(pv);
  TCheckbox_Key(&pCheckboxData->Checkbox,Key);

  //ȷ�ϼ���̴���
  if(pCheckboxData->Flag & _FLAG_ENTER){
    Data = TM_GetSize(pMenu);//����
    //��������
    TCheckbox_GetAllCheck(&pCheckboxData->Checkbox,
                            pCheckboxData->User.SelMask,Data);
    if(pCheckboxData->User.ChanegedItem < Data) 
      Data = TM_NOTIFY_CHANGED;
    else{
      Data = TM_NOTIFY_SET_DATA;
      TListboxEx_Key((TListboxEx_t*)&pCheckboxData->Checkbox,TGUI_KEY_DOWN); //���˳�λ��
    }
    //֪ͨ�û���
    TMENU_NOTIFY_RUN(pMenu->cbNotify,Data,&pCheckboxData->User);
    
    pCheckboxData->Flag &= ~_FLAG_ENTER;
  }
  //����˳�
  else if(pCheckboxData->Flag & _FLAG_RETURN)
    return -2;

  return -1;
}

