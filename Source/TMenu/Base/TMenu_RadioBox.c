/*******************************************************************************

                  TMenu�˵�֮��ѡ��ʵ��

*******************************************************************************/
#include "TMenu.h"
#include "TMenu_RadioBox.h"
#include <string.h>

//����:��־����Ϊ:
#define _FLAG_RETURN     0x01      //�ô˱�־��Ϊ���˳���һ��
#define _FLAG_ENTER      0x02      //ȷ�ϼ�
#define _FLAG_LAST_ENTER 0x04      //���һ�а�ȷ�ϼ�,����Ϊ������

//ͨ������Ӧ��Ӧ��������ͨ��:
//TGUI_NOTIFY_GET_CAPTION ��ñ���
//TGUI_NOTIFY_GET_ITEM ��������ʾ����
//TGUI_NOTIFY_KEY_ENTER ÿһ���ȷ�ϼ�,��ʱӦ���ѡ����񲢽���Ӧ�ò㴦��
//������趨ϣ��,��ʾ�ڸ��ӵı��潹����
//TGUI_NOTIFY_EKY_ESCAPE �˳�����һ��
//-------------------------��ѡ��ͨ���ص�ʵ�ֺ���-----------------------------
//�����ڲ�ͨ��ʹ��
const void*TMenu_RadioBoxNotify(void *pvSource,
                                unsigned char Type,
                                const void *pvData)
{
  const TMenu_t *pMenu = pPopMenu();
  TItemSize_t Item = *(TItemSize_t*)pvData;   //��ǰ��λ��
  struct _RadioBoxData *pRadioBoxData = (struct _RadioBoxData*)pPopData();
  
  pvSource = pvSource;//��ֹ���ֱ���������
  //�õ�ϵͳ��ǰָ������ݻ�����
  switch(Type){
  case TGUI_NOTIFY_GET_CAPTION://��ñ���
    return TM_GetHeader(pMenu);
  case TGUI_NOTIFY_GET_ITEM:// ��������ʾ����
    return TM_GetItemString(pMenu,Item);
  case TGUI_NOTIFY_KEY_ENTER://ÿһ���ȷ�ϼ�
    if(Item < TM_GetSize(pMenu)){
      pRadioBoxData->User.Sel = Item;
      TRadioButtom_SetRadio(&pRadioBoxData->RadioButtom, Item);
      pRadioBoxData->Flag |= _FLAG_ENTER; 
    }
    else //�����а���ȷ�ϼ�
      pRadioBoxData->Flag |= _FLAG_LAST_ENTER |_FLAG_ENTER; 
    break;
  case TGUI_NOTIFY_KEY_ESCAPE://�˳�����һ��
    pRadioBoxData->Flag |= _FLAG_RETURN;
  default: break;
  }
  return NULL;
}

//------------------------��������ʵ��----------------------------
//1:���������Ͳ˵�,���û���������ݲ�������ʾ
//���ش����ɹ�:
signed char TMenu_RadioBoxCreate(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                                 TWinHandle_t hWin,       //������˵��Ĵ���
                                 unsigned char Data,    //�������������,����������
                                 void *pv)      //�ɹ�ʹ�õ����ɻ�����
{
  struct _RadioBoxData *pRadioBoxData = (struct _RadioBoxData *)pv;
  TRadioButtom_t  *pRadioButtom = &pRadioBoxData->RadioButtom;
  //�õ����
  TItemSize_t ItemW = TM_GetItemMaxLen(pMenu);
  
  if(TM_GetType(pMenu) & TM_RADIOBOX_EN_SAVE)//�б����ʱ
     Data =  TMenu_RadioBox_cbGetCurStyle() | TRADIO_BUTTOM_EN_SAVE;
  else Data =  TMenu_RadioBox_cbGetCurStyle();
  //ʹ�õ�ѡ��ʵ��:
  TRadioButtom_Init(pRadioButtom,
                    hWin,//�ѽ�������ʼ���õĴ���
                    TM_GetSize(pMenu),//Ԥ������Ŀ����
                    ItemW,         //Ԥ��������(���������)
                    Data,         //�ؼ���־
                    TGUI_NOTIFY_PASS(TMenu_RadioBoxNotify,TGUI_CBFUNID_TMENU_RADIO_BUTTON));//ͨ������
  
  //���û��ռ��ò����õ�ǰ��ѡ��
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_DATA,&pRadioBoxData->User);
  
  TListboxEx_SetSel(&pRadioButtom->ListboxEx,pRadioBoxData->User.Sel);//��ǰ��
  TRadioButtom_SetRadio(pRadioButtom,TListboxEx_GetSel(&pRadioButtom->ListboxEx));  
  
  //��ʼ����������
  pRadioBoxData->Flag = 0;
  
  //������ʾ
  TMenu_ReBulidWin((TListboxEx_t *)pRadioButtom,//���ô��ڴ�С
                   ItemW,
                   TM_GetSize(pMenu),
                   strlen(TM_GetHeader(pMenu)));
  TRadioButtom_PaintAll(&pRadioBoxData->RadioButtom);
  TRadioButtom_PaintAll(&pRadioBoxData->RadioButtom);//���ȶ����ػ�����
  return -1;
}

//------------------------��Ӧ��������ʵ��----------------------------
//2:��Ӧ��������������������û��������ݽ���,��������ʾ
//����ֵ����Ϊ: ��ֵ:������Ӧ�Ӳ˵�,-1,����,-2:�����ϲ�˵�
signed char TMenu_RadioBoxKey(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                              unsigned char Key,       //����ļ�ֵ
                              void *pv)      //�ѱ�ʹ�õ��������ݻ�����
{
  unsigned char Data;
  struct _RadioBoxData *pRadioBoxData = (struct _RadioBoxData *)pv;
  PushMenu(pMenu);
  PushData(pv);
  TRadioButtom_Key(&pRadioBoxData->RadioButtom,Key);
  
  if(pRadioBoxData->Flag & _FLAG_ENTER){
    if(!(pRadioBoxData->Flag & _FLAG_LAST_ENTER)) {
      Data = TM_NOTIFY_CHANGED; //����ı���
    }
    else{//�����а�ȷ�ϼ���
      Data = TM_NOTIFY_SET_DATA; //���������
      TListboxEx_Key((TListboxEx_t*)&pRadioBoxData->RadioButtom,
                     TGUI_KEY_DOWN); //���˳�λ��
    }
    //֪ͨ�û���
    TMENU_NOTIFY_RUN(pMenu->cbNotify,Data,&pRadioBoxData->User);
    pRadioBoxData->Flag &= ~(_FLAG_ENTER | _FLAG_LAST_ENTER);
  }
  //����˳�
  else if(pRadioBoxData->Flag & _FLAG_RETURN)
    return -2;
  return -1;
}

