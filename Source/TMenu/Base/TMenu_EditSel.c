/*******************************************************************************

                  TMenu�˵�֮�༭ѡ��ģʽʵ��
ע:����ֻʵ����ʾ,����ͨ����ģ��ʵ��
*******************************************************************************/
//����չ�б����ǰ��׺ʵ����ʾ����:
//  ��Ϊ���ɱ༭ʱ,��������ʾ��������,��ֵ�ں�׺��,��ʵ�ֲ鿴�л�
//  ��Ϊ�༭ѡ��ʱ,��������ʾ��ǰ׺��,��ֵ��������,��ʵ����ֵ�����л�

#include "TMenu.h"
#include "TMenu_EditSel.h"
#include "TGUIMath.h"
#include <string.h>

//����:��־����Ϊ:
#define _FLAG_ENTER    0x08       //�������
#define _FLAG_RETURN    0x10      //�ô˱�־��Ϊ���˳���һ��

//------------------------�õ���ͷ�ַ�������----------------------------
static char *_pGetItemHeader(const TMenu_t *pMenu,
                             TItemSize_t CurItem,
                             unsigned char HeaderW)    //ָ��Ĳ˵�ϵͳ
{
  //�����Ҷ���,����": "
  const char *pHeader = TM_GetSubMenuHeader(pMenu,CurItem);
  char *pStrStart = TMenu_pGetBuf();
  if(pHeader == pStrStart) pStrStart += (strlen(pHeader) + 1);//��ֹʹ�û����ظ�
  
  unsigned char Start = HeaderW - (strlen(pHeader) + 2);
  memset(pStrStart, ' ', Start);
  char *pString = pStrStart + Start;
  pString = Tstrcpy_ex(pString,pHeader);
  *pString++ = ':';
  *pString++ = ' ';
  *pString = '\0';
  return pStrStart;
}

//------------------------�õ������ݺ���----------------------------
static const char *_pGetItem(const TMenu_t *pMenu,//ָ��Ĳ˵�ϵͳ
                             TItemSize_t CurItem,
                             struct _EditSelData *pEditSelData)   
{
  //�õ��û��������ַ���
  TItemSize_t PrvItem = pEditSelData->User.CurItem;
  pEditSelData->User.CurItem = CurItem;
  TMENU_NOTIFY_RUN(pMenu->cbNotify, TM_NOTIFY_GET_DATA, &pEditSelData->User);
  pEditSelData->User.CurItem = PrvItem;
  return pEditSelData->User.pData;
}

//ͨ������Ӧ��Ӧ��������ͨ��:
//TGUI_NOTIFY_GET_CAPTION ��ñ���
//TGUI_NOTIFY_GET_ITEM ��������ʾ����
//TGUI_NOTIFY_KEY_ENTER ÿһ���ȷ�ϼ�,��ʱӦ���ѡ����񲢽���Ӧ�ò㴦��
//������趨ϣ��,��ʾ�ڸ��ӵı��潹����
//TGUI_NOTIFY_EKY_ESCAPE �˳�����һ��
//-----------------------�༭ѡ��ģʽ�п��ͨ���ص�ʵ�ֺ���------------------------
//�����ڲ�ͨ��ʹ��
const void*TMenu_EditSelListBoxNotify(void *pvSource,
                                      unsigned char Type,
                                      const void *pvData)
{
  const TMenu_t *pMenu = pPopMenu();
  TItemSize_t Item = *(TItemSize_t*)pvData;   //��ǰ��λ��
  struct _EditSelData *pEditSelData = (struct _EditSelData*)pPopData();
  
  if(Item < TM_GetSize(pMenu))  
  pvSource = pvSource;//��ֹ���ֱ���������
  //�õ�ϵͳ��ǰָ������ݻ�����
  switch(Type){
  case TGUI_NOTIFY_GET_CAPTION://��ñ���
    return TM_GetHeader(pMenu);
  case TLISTBOXEX_NOTIFY_PREFIX://���ǰ׺
    return _pGetItemHeader(pMenu, Item,
                           pEditSelData->HeaderW);
  case TGUI_NOTIFY_GET_ITEM:// �������ʾ����
    if(TM_GetType(pMenu) & TM_MEDITSEL_EN_EDIT) 
      return _pGetItem(pMenu,Item,pEditSelData);//��������
    return _pGetItemHeader(pMenu,Item,
                           pEditSelData->HeaderW);
  case TLISTBOXEX_NOTIFY_SUFFIX://��ú�׺
    return _pGetItem(pMenu,Item,pEditSelData);//��������
  case TGUI_NOTIFY_KEY_ENTER://ÿһ���ȷ�ϼ�
    pEditSelData->User.CurItem = Item;
    pEditSelData->Flag |= _FLAG_ENTER;//��̴���
    break;
  case TGUI_NOTIFY_KEY_ESCAPE://�˳�����һ��
    pEditSelData->Flag |= _FLAG_RETURN;
  default: break;
  }
  return NULL;
}

/*****************************************************************
                    ��ģ�����
*****************************************************************/

//-----------------------ȷ�ϼ���̴���----------------------------
//��ȷ�ϼ���Ӧʱ����
//����ֵ����Ϊ: ��ֵ:������Ӧ�Ӳ˵�,-1:�����ϲ�˵�
static signed char _EnterLaterPro(struct _EditSelData *pEditSelData,
                           const TMenu_t *pMenu)
{
  TItemSize_t MaxItem = TM_GetSize(pMenu);
  //>=����ʱ֪ͨ�û���������������,�����ʾͨ�������Ӧ��
  pEditSelData->User.DisEnter = 0;
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_SET_DATA,&pEditSelData->User);
  //ȷ�ϼ���̴���:�б����ʱ֪ͨ�û����洦��
  if(/*(TM_GetType(pMenu) & TM_MEDITSEL_GRP_SAVE) &&*/
     (pEditSelData->User.CurItem >= MaxItem)){
     //����û������ı������
     if(pEditSelData->User.CurItem >= MaxItem){//λ��û��,������ȷ���˳�λ��
       pEditSelData->User.CurItem = MaxItem;//��ֹ�û���д����
       TListboxEx_Key(&pEditSelData->ListboxEx,TGUI_KEY_DOWN);
     }
     else{ //��λ�����ô���,��굽����λ��
       TListboxEx_SetSel(&pEditSelData->ListboxEx,pEditSelData->User.CurItem);
       TListBoxEx_PaintAll(&pEditSelData->ListboxEx);//������ʾ
     }
  }
  else if(pEditSelData->User.DisEnter == 0){//�������ʱ
    if(TM_GetType(pMenu) & TM_MEDITSEL_EN_EDIT)//�������ʱ��ʾ�����Ӳ˵����е���
      return (signed char)(pEditSelData->User.CurItem);
  }
  else{//���������ʱ,�������ݱ���,ǿ�Ƹ���
    TListBoxEx_PaintAll(&pEditSelData->ListboxEx);
  }
  pEditSelData->Flag &= ~_FLAG_ENTER;
  return -1;
}

//------------------------��Ӧ��������ʵ��----------------------------
//2:��Ӧ��������������������û��������ݽ���,��������ʾ
//����ֵ����Ϊ: ��ֵ:������Ӧ�Ӳ˵�,-1,����,-2:�����ϲ�˵�
signed char TMenu_EditSelKey(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                             unsigned char Key,       //����ļ�ֵ
                             void *pv)      //�ѱ�ʹ�õ��������ݻ�����
{
  struct _EditSelData *pEditSelData = (struct _EditSelData *)pv;
  unsigned char Flag;
  PushMenu(pMenu);
  PushData(pv);
  
  //#ifdef TM_EN_RPC
  //  TItemSize_t Item = pEditSelData->User.CurItem;  
  //#endif
    
  TListboxEx_Key(&pEditSelData->ListboxEx,Key);
  
  //�������ڴ���
  Flag = pEditSelData->Flag;
  
  //���ּ�����:������ݼ�����

  if((Key >= '1') && (Key <= '9')){
    pEditSelData->User.CurItem = TListboxEx_GetSel(&pEditSelData->ListboxEx);
    Flag |= _FLAG_ENTER;
  }
  //���������
  if(Flag & _FLAG_ENTER){
      return _EnterLaterPro(pEditSelData, pMenu);
  }
  //����˳�
  else if(Flag & _FLAG_RETURN)
    return -2;

  //û�н�����˳�����֧��RPCʱ�����ѡ������ı���ͨ���ı������û�������׼��
  //#ifdef TM_EN_RPC
  //  if((Item != pEditSelData->User.CurItem) && (pEditSelData->User.CurItem < TM_GetSize(pMenu)))
  //    TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_CUSOR_CHANGED, &pEditSelData->User.CurItem);
  //#endif
  
  return -1;
}

//------------------------��������ʵ��----------------------------
//1:���������Ͳ˵�,���û���������ݲ�������ʾ
//���ش����ɹ�:
#include "TWidgetPrv.h"
signed char TMenu_EditSelCreate(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                                TWinHandle_t hWin,       //������˵��Ĵ���
                                //�������������,�����ʾ�ϴβ˵�λ��
                                unsigned char Data,
                                void *pv)      //�ɹ�ʹ�õ����ɻ�����
{ 
  unsigned char W;
  unsigned char Item, CurLen;
  unsigned char Flag,Style;
  struct _EditSelData *pEditSelData = (struct _EditSelData *)pv;  
  
  //��ʼ����������
  pEditSelData->Flag = 0;
  
  //��дģʽʱ��ʽ����
  Flag = TMenu_EditSel_cbGetCurStyle();
  if(TM_GetType(pMenu) & TM_MEDITSEL_EN_EDIT){//����ģʽ
    Style = TLISTBOXEX_ALIGN_LEFT | TLISTBOXEX_ANTI_WORD2 | 
      TLISTBOXEX_PREFIX | TLISTBOXEX_EN_APPEND;
    if(!(TM_GetType(pMenu) & TM_MEDITSEL_DIS_GRP_SAVE))//Ⱥ����ʱǿ���б������˳���
      Flag |= TCHECKBOX_EN_SAVE | TCHECKBOX_EN_RETURN;
  }
  else{ 
    Style = TLISTBOXEX_ALIGN_LEFT | TLISTBOXEX_ANTI_WORD2 | 
      TLISTBOXEX_SUFFIX | TLISTBOXEX_EN_APPEND;
  }

  //�õ�����ͷ���
  W = TM_GetItemMaxLen(pMenu) + 2;
  pEditSelData->HeaderW = W;
  //������������
  if(Flag & TCHECKBOX_EN_SAVE) W = 18;//�����˳���ռ��С
  else W = 0;
  for(Item = TM_GetSize(pMenu); Item > 0; Item--){
    CurLen = strlen(_pGetItem(pMenu, Item - 1, pEditSelData));
    if(W < CurLen) W = CurLen;
  }
  pEditSelData->ItemsW = W;
  
  W += pEditSelData->HeaderW;//�����

  TListboxEx_Init(&pEditSelData->ListboxEx,  //���û�����Ĵ洢�ռ�
                  hWin,         //�ѽ�������ʼ���õĴ���
                  TM_GetSize(pMenu),  //Ԥ������Ŀ����
                  W,
                  //�ؼ���־
                  Flag,
                  //��ʾ��ʽ
                  Style,
                  TGUI_NOTIFY_PASS(TMenu_EditSelListBoxNotify,TGUI_CBFUNID_TMENU_MNUM_LISTBOX)); //ͨ������

  //���д���
  CurLen = TWidget_GetDispW(TMenu_pGetTWin(hWin),Flag);
  if(W < CurLen){ //�õ�������ʾ��ƫ��ֵ
    CurLen = (CurLen - W) >> 1;
    if(CurLen & 0x01) CurLen--;//˫�ֽڶ���
    pEditSelData->HeaderW += CurLen;
    pEditSelData->ItemsW += CurLen;
  }
  
  //֧��RPCʱ������Ĭ��λ�ü���Ҫ����RPC����,����ͨ��
  #ifdef TM_EN_RPC
    TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_CUSOR_CHANGED, &pEditSelData->User.CurItem);
  #endif
  
  //������ʾ
  TMenu_ReBulidWin(&pEditSelData->ListboxEx,//���ô��ڴ�С
                   W,
                   TM_GetSize(pMenu),
                   strlen(TM_GetHeader(pMenu)));
  
  TListboxEx_SetSel(&pEditSelData->ListboxEx,Data);//��ǰ�˵���
  TListBoxEx_PaintAll(&pEditSelData->ListboxEx);
  
  return -1;
}


