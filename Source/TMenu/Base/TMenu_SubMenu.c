/*******************************************************************************

                  TMenu�˵�֮�Ӳ˵�ʵ��

*******************************************************************************/
#include "TMenu.h"
#include "TMenu_SubMenu.h"
#include <string.h>

//����:��־����Ϊ:
#define _FLAG_RETURN    0x01      //�ô˱�־��Ϊ���˳���һ��
#define _FLAG_ENTER     0x02      //�ô˱�־��Ϊ�ǽ���ָ����

//ͨ������Ӧ��Ӧ��������ͨ��:
//TGUI_NOTIFY_GET_CAPTION ��ñ���
//TGUI_NOTIFY_GET_ITEM ��������ʾ����
//TGUI_NOTIFY_KEY_ENTER ÿһ���ȷ�ϼ�,��ʱӦ���ѡ����񲢽���Ӧ�ò㴦��
//������趨ϣ��,��ʾ�ڸ��ӵı��潹����
//TGUI_NOTIFY_EKY_ESCAPE �˳�����һ��
//-------------------------�Ӳ˵�ͨ���ص�ʵ�ֺ���-----------------------------
//�����ڲ�ͨ��ʹ��
const void*TMenu_SubMenu_Notify(void *pvSource,
                                unsigned char Type,
                                const void *pvData)
{
  const TMenu_t *pMenu = pPopMenu();
  TItemSize_t Item = *(TItemSize_t*)pvData;   //��ǰ��λ��
  struct _SubMenuData *pSubMenuData = (struct _SubMenuData*)pPopData();
  
  pvSource = pvSource;//��ֹ���ֱ��������� 
  //�õ�ϵͳ��ǰָ������ݻ�����
  switch(Type){
  case TGUI_NOTIFY_GET_CAPTION://��ñ���
    return TM_GetHeader(pMenu);
  case TGUI_NOTIFY_GET_ITEM:// ��������ʾ����,���Ӳ˵��ǵõ�
    return TM_GetSubMenuHeader(pMenu,Item);
  case TGUI_NOTIFY_KEY_ENTER://ÿһ���ȷ�ϼ�
    pSubMenuData->User.Sel = Item;//��ס��ǰλ��
    pSubMenuData->Flag |= _FLAG_ENTER;//����ָ����,��̴����ֹ��ջ����
    break;
  case TGUI_NOTIFY_KEY_ESCAPE://�˳�����һ��
    pSubMenuData->Flag |= _FLAG_RETURN;
  default: break;
  }
  return NULL;
}

//------------------------��������ʵ��----------------------------
//1:���������Ͳ˵�,���û���������ݲ�������ʾ
//���ش����ɹ�:
signed char TMenu_SubMenuCreate(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                                TWinHandle_t hWin,       //������˵��Ĵ���
                                //�������������,�����ʾ�ϴβ˵�λ��
                                unsigned char Data,
                                void *pv)      //�ɹ�ʹ�õ����ɻ�����
{
  struct _SubMenuData *pSubMenuData = (struct _SubMenuData *)pv;
  TListboxEx_t *pListboxEx = &pSubMenuData->ListboxEx;
  //�õ����
  TItemSize_t ItemW = TM_GetItemMaxLen(pMenu);
  
  //ʹ����չ�б��ʵ��:
  TListboxEx_Init(pListboxEx, //���û�����Ĵ洢�ռ�
                  hWin,//�ѽ�������ʼ���õĴ���
                  TM_GetSize(pMenu),//Ԥ������Ŀ����
                  ItemW,
                  //�ؼ���־,��������
                  TMenu_SubMenu_cbGetCurStyle(),
                  //��ʾ��ʽ
                  TLISTBOXEX_ALIGN_LEN | TLISTBOXEX_ANTI_WORD | 
                    TLISTBOXEX_EN_NO | TLISTBOXEX_EN_APPEND,
                  TGUI_NOTIFY_PASS(TMenu_SubMenu_Notify,TGUI_CBFUNID_TMENU_SUB_MENU)); //ͨ������
  
  TMenu_ReBulidWin(pListboxEx,//���ô��ڴ�С
                   ItemW,
                   TM_GetSize(pMenu),
                   strlen(TM_GetHeader(pMenu)));
  TListboxEx_SetSel(pListboxEx,Data);//��ǰ�˵���
  pSubMenuData->User.Sel = Data;
  
  //��ʼ����������
  pSubMenuData->Flag = 0;
  
  //֧��RPCʱ������Ĭ��λ�ü���Ҫ����RPC����,����ͨ��
  #ifdef TM_EN_RPC
    TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_CUSOR_CHANGED, &pSubMenuData->User.Sel);
  #endif
  
  //������ʾ
  TListBoxEx_PaintAll(&pSubMenuData->ListboxEx);
  TListBoxEx_PaintAll(&pSubMenuData->ListboxEx);//���ȶ����ػ�����
  return -1;
}

//------------------------��Ӧ��������ʵ��----------------------------
//2:��Ӧ��������������������û��������ݽ���,��������ʾ
//����ֵ����Ϊ: ��ֵ:������Ӧ�Ӳ˵�,-1,����,-2:�����ϲ�˵�
signed char TMenu_SubMenuKey(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                             unsigned char Key,       //����ļ�ֵ
                             void *pv)      //�ѱ�ʹ�õ��������ݻ�����
{
  unsigned char Flag;
  unsigned char Sel;
  struct _SubMenuData *pSubMenuData = (struct _SubMenuData *)pv;
  pMenu = pMenu;//��ֹ���ֱ��������� 
  PushMenu(pMenu);
  PushData(pv);
  #ifdef TM_EN_RPC //��ס�ϴι��λ��
    Sel = TListboxEx_GetSel(&pSubMenuData->ListboxEx); //�ϴ�λ�� 
  #endif
  TListboxEx_Key(&pSubMenuData->ListboxEx,Key);//������

  Flag = pSubMenuData->Flag;

  //��֧��RPCʱ�����ּ�����:������ݼ�����
  #ifndef TM_EN_RPC
  if((Key >= '1') && (Key <= '9')){
    //����������Ŀ�İ���
    if((Key - '1') >= TListboxEx_GetItems(&pSubMenuData->ListboxEx)) return -1;
    Flag |= _FLAG_ENTER;
  }
  #endif
  pSubMenuData->User.Sel = TListboxEx_GetSel(&pSubMenuData->ListboxEx);//����

  //����ָ����˵���̴���
  if(Flag & _FLAG_ENTER){ //������˵�
    pSubMenuData->Flag &= ~_FLAG_ENTER;

    Sel = pSubMenuData->User.Sel;
    pSubMenuData->User.UserFlag = 0;    //��ʼ��
    if(pMenu->cbNotify){ //�лص�����ʱִ�лص�
      TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_SET_DATA,&pSubMenuData->User);
      pSubMenuData->User.Sel = Sel;//��ֹ�û���ѡ
      if(pSubMenuData->User.UserFlag == 1)
        return -1;//����1ʱ��ʾΪ����,ִ�к󲻽���˵�
    }
    return (signed char)Sel;
  }

  //��鷵���ϲ�˵�
  if(Flag & _FLAG_RETURN)
    return -2;
  
  //û�н�����˳�����֧��RPCʱ�����ѡ������ı���ͨ���ı������û�������׼��
  #ifdef TM_EN_RPC
    if((Sel != pSubMenuData->User.Sel) && (pSubMenuData->User.Sel < TM_GetSize(pMenu)))
      TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_CUSOR_CHANGED, &pSubMenuData->User.Sel);
  #endif
  
  return -1;
}




