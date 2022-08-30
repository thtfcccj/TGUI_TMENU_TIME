/*******************************************************************************

                           TWidget֮��ѡ��ؼ�ʵ��

*******************************************************************************/

#include "TRadioButtom.h"
#include "TWidgetShare.h"
#include <string.h>

/**********************************************************************
                          ��Ϊ����
**********************************************************************/

//-------------------------��ѡ��ص�ʵ�ֺ���-----------------------------
//�����ڲ�ͨ��ʹ��
const void*TRadioButtom_Notify(void *pv,
                               unsigned char Type,
                               const void *pvData)
{
  //��˿ؼ���ListboxΪ�����,�ʿ���ֱ��ת��Ϊ��չ����
  TRadioButtom_t *pRadioButtom = (TRadioButtom_t*)pv;
  TGUINotify_t cbGUINotify = pRadioButtom->cbGUINotify;//�û��ռ�ͨ��
  TItemSize_t Item = *(TItemSize_t*)pvData;   //��ǰ��λ��
  char *pConstBuf;
  unsigned char PaintFlag;
  
  switch(Type){
  case TGUI_NOTIFY_KEY_ENTER: //ȷ�ϼ�ʱ��齹��λ��
  if(pRadioButtom->Radio != Item){//��ǰ�ѡ����
    //���ڸ�������ʱ,������������ȷ�ϼ�ʱ�ɵ���
    if((Item < TListboxEx_GetItems((TListboxEx_t*)pRadioButtom)) || 
      !TListboxEx_HaveAppend((TListboxEx_t*)pRadioButtom)){
	      pRadioButtom->Radio = Item;
    }
    //�����û��ռ��ȴ���
    TGUI_NOTIFY_RUN(cbGUINotify,pRadioButtom,Type,pvData);
    PaintFlag = TGUI_PAINT_DATA;
  }
  else PaintFlag = 0; //�����޸���
  pConstBuf = TGUI_pGetConstBuf();
  *pConstBuf = (char)PaintFlag;
  return pConstBuf;
  //return  &PaintFlag;  //���ظ��±�־
  //case TGUI_NOTIFY_GET_FOCUS://�ڱ��㴦����,�����������  
  //  return NULL; //ֱ�ӷ���
  case TLISTBOXEX_NOTIFY_PREFIX://ǰ׺����
    //���"��"��"��";
    pConstBuf = TGUI_pGetConstBuf();//������ָ��
    *pConstBuf++ = 0xa1;
    if(pRadioButtom->Radio == Item)//��ѡ����
      *pConstBuf++ = 0xf1;  
    else *pConstBuf++ = 0xf0;  //δ��ѡ��
    *pConstBuf = '\0';
    return TGUI_pGetConstBuf();
  //case TGUI_NOTIFY_GET_ITEM://����������
  //  break;
  //�㲻����,ֱ�ӽ����û���
  default: break;
  }
  return TGUI_NOTIFY_RUN(cbGUINotify,pRadioButtom,Type,pvData);
}

//-------------------------��ʼ������-----------------------------
//��ʼ����Ԥ�õ�һ��Ϊѡ��״̬,����ѡ��״̬��Ӧ���ػ�����
//ע:���Ǻܶ�ϵͳ��ʹ�þ�̬�ڲ����,�ʽ�����������������һ��,��
//���ڴ˻������������Ĵ�������
void TRadioButtom_Init(TRadioButtom_t *pRadioButtom,  //���û�����Ĵ洢�ռ�
                     TWinHandle_t hWin,         //�ѽ�������ʼ���õĴ���
                     TItemSize_t Items,         //Ԥ������Ŀ����
                     TItemSize_t ItemW,         //Ԥ��������(���������)
                     unsigned char Flag,        //�ؼ���־
                     TGUINotify_t cbGUINotify) //ͨ������
{
  //����ʽ�ʺ��ڵ�ѡ��
  unsigned char Style = TLISTBOXEX_ALIGN_LEN | TLISTBOXEX_ANTI_WORD2 | 
    TLISTBOXEX_PREFIX | TLISTBOXEX_EN_NO;
  
  //�ȳ�ʼ����չ,�Ա�ص�����������������
  pRadioButtom->cbGUINotify = cbGUINotify;
   pRadioButtom->Radio = 0;

  //��Ԥ�����ȱ��û���ʾ���ڿ��(��һҳ)Сʱ,�������ж���
  ItemW += TGetAlignLenR(TGetItemsPlace(Items),1)+ 2;//����뵥ѡ��ռλ

  TListboxEx_Init((TListboxEx_t*)pRadioButtom,hWin,Items,ItemW,Flag,//�����ʼ��
                  Style,TGUI_NOTIFY_PASS(TRadioButtom_Notify,TGUI_CBFUNID_RADIO_BUTTON));
}


