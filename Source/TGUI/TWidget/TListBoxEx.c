/*******************************************************************************

                           TWidget֮�б����չ�ؼ�ʵ��

*******************************************************************************/
#include "TListboxEx.h"
#include "TWidgetShare.h"
#include <string.h>

#define   _FOCUS_APPEND     0x80    //�ڸ��������־

//TListboxEx_t�ڲ���־����Ϊ:
//0-6bit��ʾ��Ч�ַ����
//7bit���б����뷵����ʱ,��ʾ����һλ��


//-------------------------�б���Իص�ʵ�ֺ���-----------------------------
static void _AutiNotify(TListboxEx_t *pListboxEx,
                        unsigned char Auti,     //��ʽ�еķ��Ա�־λ
                        unsigned char Style,    //��ʽ��־
                        unsigned char Flag,     //��������ı����뷵�ر�־
                        TGUINotify_t cbGUINotify,//�û��ص�����
                        const void *pvData)     //�û��ص���������������
{
  TItemSize_t ItemX,ItemW,ItemSpace,W;
  const char  *pString;
  TItemSize_t Sel = *(TItemSize_t*)pvData;//��ѡ��λ��
  TItemSize_t Prefix,Suffix;
  //�ڸ�������ʱ,���㴦��
  if((Style & TLISTBOXEX_EN_APPEND) && (Flag &  _FOCUS_APPEND)){
    //ֱ�ӱ��淵����,�ڱ��㴦����,�����������
    if(Flag & (TLISTBOXEX_EN_SAVE | TLISTBOXEX_EN_RETURN)){
      Append2_AutiNotify((TWidget_t*)pListboxEx,Flag,
                         pListboxEx->Flag & 0x80,
                         strlen(cbGetSaveBottom()),
                         strlen(cbGetReturnBottom())); 
    }
    else //�����ϲ�
      TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,TGUI_NOTIFY_GET_FOCUS,pvData);
    return;
  }
  //���з���
  if(Auti == TLISTBOXEX_ANTI_ROW){  
    TListbox_SetFocus((TListboxEx_t*)pListboxEx,
                      Sel, 0,0);
    return;
  }
  //��ֹ����
  if(Auti == TLISTBOXEX_AUTI_DIS){
    TListbox_SetFocus((TListboxEx_t*)pListboxEx,0,(TItemSize_t)-1,0);
    return;
  }
  //����һ����ʱ,��ʼ������
	if(Style & TLISTBOXEX_PREFIX)//ǰ׺����
	    Prefix = strlen((const char*)TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,TLISTBOXEX_NOTIFY_PREFIX,pvData));
  else  Prefix = 0;
  if(Style & TLISTBOXEX_SUFFIX)//��׺����
      Suffix = strlen(TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,TLISTBOXEX_NOTIFY_PREFIX,pvData));
  else Suffix = 0;
  //ǰ��׺��ֹ����ʱ
  if(Auti == TLISTBOXEX_ANTI_WORD2){
    ItemX =  Prefix;
    ItemW = 0;
  }
  else{
    ItemX =  0;
    ItemW = Prefix +  Suffix;
  }
  
  //�����ʱ(��Ų�����),������ų���,��Ÿ���һ��:".",��������ĳ���,
  if(Style & TLISTBOXEX_EN_NO)
    ItemX += TGetAlignLenR(TGetItemsPlace(TListbox_GetItems((TListboxEx_t*)pListboxEx)),1);
  //�õ��ַ�����
  pString = TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,TGUI_NOTIFY_GET_ITEM,pvData);
  ItemW += strlen(pString);

  //�����ʼλ�ü����ɶ��뷽ʽ����Ŀո�
  Style &= TLISTBOXEX_ALIGN_MASK;
  W = TListbox_GetDispW((TListboxEx_t*)pListboxEx);
  if(W < ItemW) W = ItemW; //���䳤����Ӧ����
  if(Style == TLISTBOXEX_ALIIGN_CENTER)//���ж���ʱ,�ų����հ���
    ItemX += TGetAlignLenR((W - ItemW) >> 1,0);
  else if(Style == TLISTBOXEX_ALIGN_RIGHT)//�Ҷ���ʱ,�ų����հ���
    ItemX += TGetAlignLenR(W - ItemW, 0);
  else if(Style == TLISTBOXEX_ALIGN_LEN){ //��ȶ���ʱ,�ų�ǰ��հ���
    W = pListboxEx->ValidW;
	  ItemSpace = TWidget_GetHPageSizeEnNotify((TWidget_t*)pListboxEx);
      if((W) <  ItemSpace){  //���ܳ�С��ҳ��ʱ,���ж���
      ItemX +=  TGetAlignLenL((ItemSpace - (W)) >> 1,0);
    }
  }
  else{ //�����ʱ,������һ���ǿ��ַ���Ϊ��ʼ����
    W = 0;
    while(*pString == ' ') {pString++; W++;}
    ItemX += W;
    ItemW -=W;
  }

  TListbox_SetFocus((TListboxEx_t*)pListboxEx, Sel, ItemX, ItemW);
}


//------------------------------------�����ź���--------------------------
static char *pFullNO(TListboxEx_t *pListboxEx,
                     TItemSize_t  Item,
                     char *pBuf)
{
  unsigned char Data;
  //��������
  Data = TGetItemsPlace(TListbox_GetItems((TListboxEx_t*)pListboxEx));//ռλ
  pBuf += TValue2StringMin(Item + 1,pBuf,Data);//��Ŵ�1��ʼ
  *pBuf++='.';  //1.����
  pBuf = pTAlignString(Data + 1,pBuf);//��Ŷ���
  return pBuf;
}

//-------------------------�б��õ���ص�ʵ�ֺ���-----------------------------
static const void *_ItemNotify(TListboxEx_t *pListboxEx,
                               unsigned char Style,    //��ʽ��־
                               unsigned char Flag,     //��������ı����뷵�ر�־
                               TGUINotify_t cbGUINotify,//�û��ص�����
                               const void *pvData)     //�û��ص���������������
{
  char *pBuf = TGUI_pGetBuf();       //ʹ�ù�����
  TItemSize_t Item = *(TItemSize_t*)pvData;   //��ǰ��λ��
  unsigned char Align = Style & TLISTBOXEX_ALIGN_MASK;
  TItemSize_t ItemW,ItemSpace,W;
  const char *pString,*pPrefix = NULL,*pSuffix = NULL;//�����ַ���,=NULL��ֹ���ֱ���������
  
  //�ڸ�������ʱ,���ݴ���
  if((Style & TLISTBOXEX_EN_APPEND) && (Flag &  _FOCUS_APPEND)){
    //�������
    if(Flag & (TLISTBOXEX_EN_SAVE | TLISTBOXEX_EN_RETURN)){
      Flag &= ~_FOCUS_APPEND;
      return Append2_GetString((TWidget_t*)pListboxEx,Flag,
                               cbGetSaveBottom(),
                               cbGetReturnBottom());  
    }
    else //�����ϲ�
      return TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,TGUI_NOTIFY_GET_ITEM,pvData);
  }
  
  //�õ��û��ռ��ַ���,��������
  pString = (const char *)TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,TGUI_NOTIFY_GET_ITEM,pvData);
  if(pString == NULL){//�����쳣��
    *pBuf = '\0';//�����ַ�
    return pBuf;
  }
  ItemW = strlen(pString);
  //���ռλ
  if(Style & TLISTBOXEX_EN_NO)
    ItemW += TGetAlignLenR(TGetItemsPlace(TListbox_GetItems((TListboxEx_t*)pListboxEx)) + 1,0);

  if(Style & TLISTBOXEX_PREFIX){//�õ�ǰ׺
    pPrefix = (const char *)TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,TLISTBOXEX_NOTIFY_PREFIX,pvData);
    ItemW += strlen(pPrefix);
  }
  if(Style & TLISTBOXEX_SUFFIX){//�õ���׺
    pSuffix = (const char *)TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,TLISTBOXEX_NOTIFY_SUFFIX,pvData);
    ItemW += strlen(pSuffix);
  }

  W = TListbox_GetDispW((TListboxEx_t*)pListboxEx);
  if(W < ItemW) W = ItemW; //���䳤����Ӧ����

  //���ݶ��뷽ʽ�������,ǰ�����:
  if(Align == TLISTBOXEX_ALIIGN_CENTER)
    ItemSpace = TGetAlignLenR((W - ItemW) >> 1,0);
  else if(Align == TLISTBOXEX_ALIGN_LEFT)
    ItemSpace = 0;
  else if(Align == TLISTBOXEX_ALIGN_RIGHT)
    ItemSpace = W - ItemW;
  else{// if(Align == TLISTBOXEX_ALIGN_LEN){ //��ȶ���ǰ���
	  ItemSpace = TWidget_GetHPageSizeEnNotify((TWidget_t*)pListboxEx);   
    if(pListboxEx->ValidW <  ItemSpace)  //���ܳ�С��ҳ��ʱ,���ж���
      ItemSpace = TGetAlignLenL((ItemSpace - pListboxEx->ValidW) >> 1,0);
    else ItemSpace = 0;
  }
  memset(pBuf,' ',ItemSpace);pBuf += ItemSpace; ItemW += ItemSpace;

  if(Style & TLISTBOXEX_EN_NO) pBuf = pFullNO(pListboxEx,Item,pBuf);//������
  if(Style & TLISTBOXEX_PREFIX) pBuf = Tstrcpy_ex(pBuf,pPrefix);//���ǰ׺
  pBuf = Tstrcpy_ex(pBuf,pString);//�������
  if(Style & TLISTBOXEX_SUFFIX) pBuf = Tstrcpy_ex(pBuf,pSuffix);//����׺
  //��ո����
  if(W > ItemW){
    memset(pBuf,' ',W - ItemW); 
    pBuf += W - ItemW;//�������
  }
  *pBuf = '\0';//�����ַ�
  return TGUI_pGetBuf();
}

//-------------------------�б��ص�ʵ�ֺ���-----------------------------
//�����ڲ�ͨ��ʹ��
const void*TListBox_Notify(void *pv,
                           unsigned char Type,
                           const void *pvData)
{
  //��˿ؼ���ListboxΪ�����,�ʿ���ֱ��ת��Ϊ��չ����
  TListboxEx_t *pListboxEx = (TListboxEx_t*)pv;
  unsigned char Style = pListboxEx->Style;
  TGUINotify_t cbGUINotify = pListboxEx->cbGUINotify;//�û��ռ�ͨ��
  unsigned char Flag = TWidget_GetUserFlag((TWidget_t *)pListboxEx);//�����ϵ
  TItemSize_t Item = *(TItemSize_t*)pvData;   //��ǰ��λ��
  
  if((Style & TLISTBOXEX_EN_APPEND) && //�и�����ʱ
     (Item == (TListbox_GetItems((TListbox_t*)pListboxEx) - 1)))
    Flag |= _FOCUS_APPEND; //�����ʾ�ڸ�������  
  
  switch(Type){
  case TGUI_NOTIFY_KEY_ENTER: //ȷ�ϼ�ʱ��齹��λ��
    if(Style & TLISTBOXEX_EN_APPEND){//�и���ʱ
      //�����ڷ�����ʱ����Ϊ���ؼ�
      if(Flag == (_FOCUS_APPEND | TLISTBOXEX_EN_RETURN))
        Type = TGUI_NOTIFY_KEY_ESCAPE;
      else if(Flag == (_FOCUS_APPEND | TLISTBOXEX_EN_SAVE | TLISTBOXEX_EN_RETURN)){
        if(pListboxEx->Flag & 0x80) //������ʱ,����Ϊ���ؼ�
          Type = TGUI_NOTIFY_KEY_ESCAPE;
      }
    }
    //else //�����û��ռ䴦��
    break;  //�����û��ռ䴦��
  case TGUI_NOTIFY_GET_FOCUS://�ڱ��㴦����
    _AutiNotify(pListboxEx,
                Style & TLISTBOXEX_AUTI_MASK,Style,Flag,
                cbGUINotify,pvData);   
    return NULL; //ֱ�Ӹ���
  case TGUI_NOTIFY_GET_ITEM://�ڱ��㴦������
    return _ItemNotify(pListboxEx,Style,Flag,cbGUINotify,pvData);
  //�㲻����,ֱ�ӽ����û���
  default: break;
  }
  return TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,Type,pvData);
}

//-------------------------��ʼ������-----------------------------
//ע:���Ǻܶ�ϵͳ��ʹ�þ�̬�ڲ����,�ʽ�����������������һ��,��
//���ڴ˻������������Ĵ�������
void TListboxEx_Init(TListboxEx_t *pListboxEx,  //���û�����Ĵ洢�ռ�
                     TWinHandle_t hWin,         //�ѽ�������ʼ���õĴ���
                     TItemSize_t Items,         //Ԥ������Ŀ����
                     TItemSize_t ItemW,         //�ַ������
                     unsigned char Flag,        //�ؼ���־
                     unsigned char Style,       //��ʾ��ʽ
                     TGUINotify_t cbGUINotify) //ͨ������
{
  //�ȳ�ʼ����չ,�Ա�ص�����������������
  pListboxEx->cbGUINotify = cbGUINotify;
  
  if(Flag & (TLISTBOXEX_EN_SAVE | TLISTBOXEX_EN_RETURN)) //ǿ���ø�����
    Style |=TLISTBOXEX_EN_APPEND;
  else //ǿ��ȥ��
     Style &= ~TLISTBOXEX_EN_APPEND;
  
  pListboxEx->Style = Style;

  //�฽�ӵ�ѡ����   
  if(Style & TLISTBOXEX_EN_APPEND) Items++; 
     
  TListbox_Init((TListbox_t*)pListboxEx,hWin,Flag,Items,
                TGUI_NOTIFY_PASS(TListBox_Notify,TGUI_CBFUNID_LISTBOX_EX));//�����ʼ��

  pListboxEx->ValidW = ItemW;
  TListbox_SetW((TListbox_t*)pListboxEx,ItemW);
}

//-------------------------����������-----------------------------
void TListboxEx_Key(TListboxEx_t *pListboxEx,unsigned char Key)
{
  unsigned char Flag = pListboxEx->Flag & 0x80;
  //���ܴ�������Ĵ���
  if(pListboxEx->Style & TLISTBOXEX_EN_APPEND){
    do{
      Flag = Append2_Key((TWidget_t *)pListboxEx,Key,
                       TWidget_GetUserFlag((TWidget_t *)pListboxEx),
                       Flag);
      if(Flag != (unsigned char)-1) break;
      TListbox_Key((TListbox_t*)(pListboxEx),Key);
    }while(1);
    
    if(Flag <= 1){
      if(Flag == 1) pListboxEx->Flag |= 0x80;     
      else pListboxEx->Flag &= ~0x80; 
      ListBox_Paint((TListbox_t*)(pListboxEx),TGUI_PAINT_FOCUS);
    }
  }
  //���ദ��
  else TListbox_Key((TListbox_t*)(pListboxEx),Key);
    
}

//---------------------------��õ�������-------------------------------
TItemSize_t TListboxEx_GetItems(TListboxEx_t *pListboxEx)
{
  TItemSize_t Items = TListbox_GetItems(pListboxEx);
  if(TListboxEx_HaveAppend(pListboxEx)) Items--;
  return Items;
}

