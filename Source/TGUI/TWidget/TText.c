/*******************************************************************************

                           TWidget֮��չ�ı���ؼ�ʵ��

*******************************************************************************/
#include "TText.h"
#include <string.h>

#include "TWidgetShare.h"
//------------------------�����ı�ָ���к���-----------------------------
//����NULL��ʾ����δ�ҵ�
static const char* _GoRow(TText_t *pText,
                          TItemSize_t Row)
{
  const char *pString =  pText->pString;
  do{
    pString = strchr(pString,'\n');
    if(!Row) return pString;//�ҵ���
    Row--;
  }while(pString);
  return NULL;
}

//-------------------------�ı����Իص�ʵ�ֺ���-----------------------------
static void _AutiNotify(TText_t *pText,
                        unsigned char Style,//��ʽ��־
                        TItemSize_t Item)    //��ǰ��
{
  TItemSize_t Len;
  //�ı��ɻ�ý����Ҳ��ڸ�������ʱ
  if((Item != (TListbox_GetItems((TText_t*)pText) - 1)) &&
     (pText->Style & TTEXT_EN_FOCUS_TEXT)){
    if(*(_GoRow(pText,Item) + pText->FocusX) == 0x80)
      Len = 2;//ȫ��
    else Len = 1;//���
    TListbox_SetFocus((TText_t*)pText, Item, pText->FocusX, Len);
    return;
  }
  
  Style &= TTEXT_EN_BOTTOM1 | TTEXT_EN_BOTTOM2;
  //�޸�����ʱ�޽���
  if(!Style) 
    TListbox_SetFocus((TText_t*)pText,0, 0, 0);
  
  //���������н���
  if(Style & TTEXT_EN_BOTTOM1) Len = strlen(pText->pBottom1);
  else Len = 0;
  if(Style & TTEXT_EN_BOTTOM2) Item = strlen(pText->pBottom2);
  else Item = 0;
  Append2_AutiNotify((TWidget_t*)pText,Style,
                          pText->Style & TTEXT_FOCUS2,Len,Item);
}


//-------------------------�ı���õ���ص�ʵ�ֺ���-----------------------------
static const void *_ItemNotify(TText_t *pText,
                               unsigned char Style,    //��ʽ��־
                               TItemSize_t Item)    //��ǰ��
{
  char *pBuf = TGUI_pGetBuf();       //ʹ�ù�����
  TItemSize_t ItemSpace,StrLen;
  unsigned char Align =Style & TTEXT_ALIGN_MASK;
  const char *pString;//�����ַ���
  
  Style &= TTEXT_EN_BOTTOM1 | TTEXT_EN_BOTTOM2;
  //����������������
  if((Style) && Item == (TListbox_GetItems((TText_t*)pText) - 1)){
    return Append2_GetString((TWidget_t*)pText,
                             Style & (TTEXT_EN_BOTTOM1 | TTEXT_EN_BOTTOM2),
                             pText->pBottom1,
                             pText->pBottom2);            
  }
  //�û��ռ����,���ݶ��뷽ʽ�������
  pString = _GoRow(pText,Item);
  StrLen = (strchr(pString,'\n') - pString);
  if(Align == TTEXT_ALIIGN_CENTER){
    ItemSpace = TGetAlignLenR((TListbox_GetW((TText_t*)pText) - StrLen) >> 1,0);
    memset(pBuf,' ',ItemSpace);pBuf += ItemSpace;//ǰ�����
    pBuf = Tstrcpy_ex(pBuf,pString);//�������
    //����ǰ����ʱ������ǰ����һ���ո�,����Ҫ����
    ItemSpace = TGetAlignLenR((TListbox_GetW((TText_t*)pText) - StrLen - 1) >> 1,0);
    memset(pBuf,' ',ItemSpace); pBuf += ItemSpace;//�������
  }
  else if(Align == TTEXT_ALIGN_LEFT){
    ItemSpace = TListbox_GetW((TText_t*)pText) - StrLen;
    pBuf = Tstrcpy_ex(pBuf,pString);//�������
  }
  else if(Align == TTEXT_ALIGN_RIGHT){
    pBuf = Tstrcpy_ex(pBuf,pString);//�������
    ItemSpace = TListbox_GetW((TText_t*)pText) - StrLen;
    memset(pBuf,' ',ItemSpace); pBuf += ItemSpace;//�������
  }
  //else{//��ɢ����ʱ
  // 
  //}
  *pBuf = '\n';//�����ַ�
  
  return TGUI_pGetBuf();
}

//-------------------------�ı���ص�ʵ�ֺ���-----------------------------
//�����ڲ�ͨ��ʹ��
const void*TText_Notify(void *pvSource,
                        unsigned char Type, 
                        const void *pvData)
{
  TItemSize_t Item = *(TItemSize_t*)pvData;
  unsigned char Style = ((TText_t *)pvSource)->Style;
  //��˿ؼ���ListboxΪ�����,�ʿ���ֱ��ת��Ϊ��չ����
  TText_t *pText = (TText_t*)pvSource;
  TGUINotify_t cbGUINotify = pText->cbGUINotify;//�û��ռ�ͨ��
  switch(Type){
  case TGUI_NOTIFY_KEY_ENTER: //ȷ�ϼ�ʱ��齹��λ��
    //�����û����幦��
    if((Style & (TTEXT_EN_BOTTOM1 | TTEXT_EN_BOTTOM2)) && 
       (Item == (TListbox_GetItems((TText_t*)pText) - 1))){
         if(Style & TTEXT_FOCUS2) 
           Type = TTEXT_NOTIFY_BOTTOM2;
         else Type = TTEXT_NOTIFY_BOTTOM1;
    }
    break;  //�����û��ռ䴦��
  case TGUI_NOTIFY_GET_FOCUS://�ڱ��㴦����
    _AutiNotify(pText,Style,Item);            
    return NULL; //ֱ�Ӹ���
  case TGUI_NOTIFY_GET_ITEM://�ڱ��㴦������
    return _ItemNotify(pText,Style,Item);
  //�㲻����,ֱ�ӽ����û���
  default: break;
  }
  return TGUI_NOTIFY_RUN(cbGUINotify,pText,Type,pvData);
}


//-------------------------��ʼ������-----------------------------
//ע:���Ǻܶ�ϵͳ��ʹ�þ�̬�ڲ����,�ʽ�����������������һ��,��
//���ڴ˻������������Ĵ�������
void TText_Init(TText_t *pText,  //���û�����Ĵ洢�ռ�
                TWinHandle_t hWin,         //�ѽ�������ʼ���õĴ���
                unsigned char Flag,        //�ؼ���־
                unsigned char Style,       //�ı���ʾ��ʽ
                const char *pBottom1,      //�׸���ť��,��ʱΪNULL
                const char *pBottom2,      //�ڶ�����ť��,��ʱΪNULL            
                TGUINotify_t cbGUINotify) //ͨ������
{
  //�ȳ�ʼ����չ,�Ա�ص�����������������
  pText->Style = Style;
  pText->cbGUINotify = cbGUINotify;
  pText->pBottom1 = pBottom1;
  pText->pBottom2 = pBottom2;

  TListbox_Init((TListbox_t*)pText,hWin,Flag,1,
                TGUI_NOTIFY_PASS(TText_Notify,TGUI_CBFUNID_TEXT));//�����ʼ��
  
  TText_UpdateText(pText);
}

//-------------------------�����ַ�����-----------------------------
//���ַ����ı�ʱ���ô˺���
//�ɿؼ����ѻ���ַ���
void TText_UpdateText(TText_t *pText)
{
  TItemSize_t Items= 0;         //Ԥ������Ŀ����
  const char *pString = (const char *)TGUI_NOTIFY_RUN(pText->cbGUINotify,
                                  pText,TTEXT_NOTIFY_GET_STRING,NULL);
  pText->pString = pString;
  //ͳ����������
  while(pString){
    pString = strchr(pString,'\n');
    Items++;
  };
  if(pText->Style & (TTEXT_EN_BOTTOM1 | TTEXT_EN_BOTTOM1)) Items++; //�฽��ѡ����
  TListbox_SetItems((TListbox_t*)pText,Items); 
}


//-------------------------����������-----------------------------
void TText_Key(TText_t *pText,unsigned char Key)
{
  TItemSize_t FocusX;
  unsigned char Flag = pText->Style & TTEXT_FOCUS2;
  do{
    Flag = Append2_Key((TWidget_t *)pText,Key,
                       TWidget_GetUserFlag((TWidget_t *)pText),Flag);
    if(Flag != (unsigned char)-1) break;
    TListbox_Key((TListbox_t*)(pText),Key);
	}while(1);
  if(Flag <= 1){
    if(Flag  == 1) pText->Style |= TTEXT_FOCUS2;                           
    else pText->Style &= ~TTEXT_FOCUS2;
    ListBox_Paint((TListbox_t*)(pText),TGUI_PAINT_FOCUS);
  }
  
  if(pText->Style & TTEXT_EN_FOCUS_TEXT){//�����ı������ƶ�
    FocusX = pText->FocusX;
    if(*(_GoRow(pText,TListbox_GetSel((TListbox_t*)pText)) +
         FocusX) == 0x80)//��鵱ǰλ��ȫ�ǰ��
      FocusX += 2;//ȫ��
    else FocusX += 1;//���
    if(FocusX >= TListbox_GetW((TListbox_t*)pText))
       FocusX = 0;//�ص�ǰ����
    pText->FocusX = FocusX;
  }
}




