/*******************************************************************************

                           TWidget֮��ѡ��ؼ�ʵ��

*******************************************************************************/
#include "TCheckbox.h"
#include <string.h>
#include <math.h>

/**********************************************************************
                          ��Ա����
**********************************************************************/

//------------------------���ĳ���Ƿ�ѡ��----------------------------
//����0��ʾδѡ��,�����ʾѡ��
unsigned char TCheckbox_IsChecked(TCheckbox_t *pCheckbox,TItemSize_t Item)
{
  return pCheckbox->SelMask[Item >> 3] & U8ShiftLUT[Item & 0x07];
}
                
//------------------------��ĳ��Ϊѡ��״̬----------------------------
//����0��ʾδѡ��,�����ʾѡ��
void TCheckbox_SetChecked(TCheckbox_t *pCheckbox,TItemSize_t Item)
{
   pCheckbox->SelMask[Item >> 3] |= U8ShiftLUT[Item & 0x07];
}
                
//------------------------���ĳ��ѡ��״̬----------------------------
//����0��ʾδѡ��,�����ʾѡ��
void TCheckbox_ClrChecked(TCheckbox_t *pCheckbox,TItemSize_t Item)
{
   pCheckbox->SelMask[Item >> 3] &= ~U8ShiftLUT[Item & 0x07];
}

//------------------------��������״̬----------------------------
//ÿһλ��ʾһ��״̬,0��ʾδѡ��,�����ʾѡ��
void TCheckbox_SetAllCheck(TCheckbox_t *pCheckbox,
                           unsigned char *pCheck,
                           TItemSize_t Items)
{
  unsigned char Count = (Items + 7) >> 3;
  memcpy(pCheckbox->SelMask,pCheck,Count);
}

//--------------------------�õ�����״̬--------------------------------
void TCheckbox_GetAllCheck(TCheckbox_t *pCheckbox,
                           unsigned char *pCheck,
                           TItemSize_t Items)
{
  unsigned char Count = (Items + 7) >> 3;
  memcpy(pCheck,pCheckbox->SelMask,Count);
}


/**********************************************************************
                          ��Ϊ����
**********************************************************************/

//-------------------------��ѡ��ص�ʵ�ֺ���-----------------------------
//�����ڲ�ͨ��ʹ��
const void* TCheckBox_Notify(void *pvSource,
                             unsigned char Type,
                          const void *pvData)
{
  //��˿ؼ���ListboxΪ�����,�ʿ���ֱ��ת��Ϊ��չ����
  TCheckbox_t *pCheckbox = (TCheckbox_t*)pvSource;
  TItemSize_t Item = *(TItemSize_t*)pvData;   //��ǰ��λ��
  unsigned char *pCurItem;
  unsigned char Mask;
  char *pConstBuf;
  
  switch(Type){
  case TGUI_NOTIFY_KEY_ENTER: //ȷ�ϼ�ʱ��ѡ��ѡ��Ŀ
    //���ڸ�������ʱ,������������ȷ�ϼ�ʱ�ɵ���
    if((Item < TListboxEx_GetItems((TListboxEx_t*)pCheckbox)) || 
      !TListboxEx_HaveAppend((TListboxEx_t*)pCheckbox)){
      pCurItem = &(pCheckbox->SelMask[Item >> 3]);
      Mask = U8ShiftLUT[Item & 0x07];
      if(*pCurItem & Mask) *pCurItem &= ~Mask;
      else *pCurItem |= Mask;
    }
    //�����û��ռ��ȴ���
    TGUI_NOTIFY_RUN(pCheckbox->cbGUINotify,pvSource,Type,pvData);
    Mask = TGUI_PAINT_DATA;
    
    pConstBuf = TGUI_pGetConstBuf();
    *pConstBuf = (char)Mask;
    return pConstBuf;
    //return  &Mask;  //���ظ��±�־
  //case TGUI_NOTIFY_GET_FOCUS://�ϲ��Ѵ�����ȫ������   
  //  return NULL; //ֱ�ӷ���
  case TLISTBOXEX_NOTIFY_PREFIX://ǰ׺����
    //���"��"��"��";
    pConstBuf = TGUI_pGetConstBuf();//������ָ��
    *pConstBuf++ = 0xa1;
    if(pCheckbox->SelMask[Item >> 3] & U8ShiftLUT[Item & 0x07])
      *pConstBuf++ = 0xf6;  //��ѡ����
    else *pConstBuf++ = 0xf5;  //δ��ѡ��
    *pConstBuf = '\0';
    return TGUI_pGetConstBuf();
  //case TGUI_NOTIFY_GET_ITEM://�ϲ��Ѵ�����ȫ������
  //  break;
  //�㲻����,ֱ�ӽ����û���
  default: break;
  }
  return TGUI_NOTIFY_RUN(pCheckbox->cbGUINotify,pCheckbox,Type,pvData);
}

//-------------------------��ʼ������-----------------------------
//��ʼ���������������Ϊδѡ��״̬,Ӧѡ��ѡ��״̬���ػ�����
//ע:���Ǻܶ�ϵͳ��ʹ�þ�̬�ڲ����,�ʽ�����������������һ��,��
//���ڴ˻������������Ĵ�������
void TCheckbox_Init(TCheckbox_t *pCheckbox,  //���û�����Ĵ洢�ռ�
                     TWinHandle_t hWin,         //�ѽ�������ʼ���õĴ���
                     TItemSize_t Items,         //Ԥ������Ŀ����
                     TItemSize_t ItemW,         //Ԥ��������(���������)
                     unsigned char Flag,        //�ؼ���־
                     TGUINotify_t cbGUINotify) //ͨ������
{
  //����ʽ�ʺ��ڸ�ѡ��
  unsigned char Style = TLISTBOXEX_ALIGN_LEN | TLISTBOXEX_ANTI_WORD2 | 
    TLISTBOXEX_PREFIX | TLISTBOXEX_EN_NO;
  
  //�ȳ�ʼ����չ,�Ա�ص�����������������
  pCheckbox->cbGUINotify = cbGUINotify;
  memset(pCheckbox->SelMask, 0, sizeof(pCheckbox->SelMask));//��ʼ��Ϊ����ѡ��  

  //��Ԥ�����ȱ��û���ʾ���ڿ��(��һҳ)Сʱ,�������ж���
  //PageSize = TWidget_GetHPageSize((TWidget_t*)pCheckbox);
  ItemW += TGetAlignLenR(TGetItemsPlace(Items),1)+ 2;//����븴ѡ��ռλ

  TListboxEx_Init((TListboxEx_t*)pCheckbox,hWin,Items,ItemW,Flag,//�����ʼ��
                  Style,TGUI_NOTIFY_PASS(TCheckBox_Notify,TGUI_CBFUNID_CHECKBOX));
}


