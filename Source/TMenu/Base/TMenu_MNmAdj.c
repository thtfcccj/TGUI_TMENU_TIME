/*******************************************************************************

                  TMenu�˵�֮��ֵ����ģʽʵ��
ע:����ֻʵ����ʾ,����ͨ����ģ��ʵ��
*******************************************************************************/

//��TGUI��û�ж�ֵ����ģʽ,����ʵ��Ϊ:MNumAdj������ʾ����,
//������ʱ,����ʾ�����ϵ���Win������ʾ��ǰ�����������.

//����չ�б����ǰ��׺ʵ����ʾ����:
//  ��Ϊ��ֵ��ʾʱ,��������ʾ��������,��ֵ�ں�׺��,��ʵ�ֲ鿴�л�
//  ��Ϊ��ֵ����ʱ,��������ʾ��ǰ׺��,��ֵ��������,��ʵ����ֵ�����л�
//��չ�б��Ӧ����Ϊ,�򿪱����뷵����,�����

#include "TMenu.h"
#include "TMenu_MNumAdj.h"
#include "TGUIMath.h"
#include <string.h>

#include "TEdit_Float.h"  //��֧�ָ�����ģʽʱ�����ڲ�����Ϊ��

//����:��־����Ϊ:
#define _FLAG_ENTER    0x08       //�������
#define _FLAG_RETURN    0x10      //�ô˱�־��Ϊ���˳���һ��
#define _FLAG_ADJ_MODE  0x20      //��ǰ�ڵ���ģʽ��־
#define _FLAG_LEFT_END  0x40      //ѭ��ģʽ����ʱ������߽��־
#define _FLAG_RIGHT_END  0x80      //ѭ��ģʽ����ʱ�����ұ߽��־


//----------------------------���������ṹ------------------------
#if TM_MNUMADJ_DEFAULT_DESC & 0x01
  const struct _MNumDesc MNumDesc_U8 = {MNUM_TYPE_DEC, 0, 255};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x02
  const struct _MNumDesc MNumDesc_B1 = {MNUM_TYPE_DEC, 0, 1};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x04
  const struct _MNumDesc MNumDesc_B2 = {MNUM_TYPE_DEC, 0, 3};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x08
  const struct _MNumDesc MNumDesc_9 = {MNUM_TYPE_DEC, 0, 9};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x10
  const struct _MNumDesc MNumDesc_99 = {MNUM_TYPE_DEC, 0, 99};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x20
  const struct _MNumDesc MNumDesc_999 = {MNUM_TYPE_DEC, 0, 999};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x40
  const struct _MNumDesc MNumDesc_9999 = {MNUM_TYPE_DEC, 0, 9999};
#endif

#if TM_MNUMADJ_DEFAULT_DESC & 0x80
  const struct _MNumDesc MNumDesc_U16 = {MNUM_TYPE_DEC, 0, 65535};
#endif  
  
/*****************************************************************
                      �༭ģʽ���
*****************************************************************/


//-----------------�༭ģʽͨ���ص�ʵ�ֺ���------------------------
//�����ڲ�ͨ��ʹ��
const void*TMenu_MNumAdjEditNotify(void *pvSource,
                                   unsigned char Type,
                                   const void *pvData)
{
  const TMenu_t *pMenu = pPopMenu();
  const TEdit_t *pEdit = (TEdit_t*)pvSource;
  
  struct _MNumAdjData *pMNumAdjData = (struct _MNumAdjData*)pPopData();
  unsigned char Flag = pMNumAdjData->Flag;
  
  pvData = pvData;
  switch(Type){
  case TGUI_NOTIFY_KEY_ESCAPE://��ȷ���˳�
    TWM_FreeHandle(TEdit_hGetWin(pEdit)); //ֱ���ͷŴ���
    Flag &= ~_FLAG_ADJ_MODE;    //�˳�����ģʽ
    break;
  case TEDIT_NOTIFY_LEFT_END://�������ұ߽�ͨ��
  case TEDIT_NOTIFY_RIGHT_END:
    if(!(TM_GetType(pMenu) & TM_MNUMADJ_LOOP_MODE)) break;
    //����ģʽΪѭ��ģʽʱ����
    if(Type == TEDIT_NOTIFY_LEFT_END) Flag |= _FLAG_LEFT_END;
    else Flag |= _FLAG_RIGHT_END; 
    //����,���赱��ȷ�ϼ�����
  case TGUI_NOTIFY_KEY_ENTER://ȷ�ϲ��˳�:
    //�õ���ֵ
    pMNumAdjData->User.Value[pMNumAdjData->User.CurItem] = TEdit_GetVolume(pEdit);
    //֪ͨ�û��������ݱ�������
    TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_CHANGED,&pMNumAdjData->User);
    TWM_FreeHandle(TEdit_hGetWin(pEdit)); //�ͷŴ���
    Flag &= ~_FLAG_ADJ_MODE;    //�˳�����ģʽ
    break;
  default: break;
  }
  pMNumAdjData->Flag = Flag;
  return NULL;
}

//-------------------------����༭ģʽ����--------------------------
void _EnterEditMode(const TMenu_t *pMenu,//ָ��Ĳ˵�ϵͳ
                    struct _MNumAdjData *pMNumAdjData)   
{
  struct _MNumDesc *pDesc;
  unsigned char x,y,Flag;
  TWidget_t *pWidget = (TWidget_t*)(&pMNumAdjData->ListboxEx);//�̳й�ϵ
  TWinHandle_t hWin = TWidget_hGetWin(pWidget);//��ָ��TListbox  
  
  //�ҵ���ʾ��λ��,��hWin��x,y,��Desc��
  x = TWidget_GetClipX(pWidget) + 
    (pMNumAdjData->HeaderW - TWidget_GetX(pWidget));
  y = TWidget_GetClipY(pWidget) + 
    (TWidget_GetSel(pWidget) - TWidget_GetY(pWidget));
  //����һ�����ڲ���ʼλλ��
  hWin = cbTMenu_hGetEdit(hWin);
  TWin_Move(TWM_pGetWin(hWin),x,y);
  
  //������������׼���������
  pMNumAdjData->User.CurItem = TWidget_GetSel(pWidget);
  //�õ��û�����ǰ����������Ϣ
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_MNUM_GET_DESC,&pMNumAdjData->User);

  pDesc = &pMNumAdjData->User.Desc;
  if(pDesc->Flag & MNUM_TYPE_GRP_MODE)       //Ⱥ����ģʽʱǿ�ƴ���ʾ
    Flag = TEDIT_GRP_MODE | TEDIT_EN_NOTIFY ;
  else Flag = TEDIT_EN_NOTIFY;
  if(TM_GetType(pMenu) & TM_MNUMADJ_ENTER_KEY)
    Flag |= TEDIT_ENTER_RIGHT;

  
  if(pDesc->Flag & MNUM_EN_SIGN) Flag |= TEDIT_EN_SIGN;
  switch(pDesc->Flag & MNUM_TYPE_MASK){
  case MNUM_TYPE_DEC:
    TEditDec(&pMNumAdjData->Edit,
             hWin,    //�����Ĵ���,���ݻ�����>Len
             //��ʼ��ֵ
             (TSigned_t)pMNumAdjData->User.Value[pMNumAdjData->User.CurItem],    
             (TSigned_t)pDesc->Max,       //���ֵ,������ֵ����
             (TSigned_t)pDesc->Min,       //��Сֵ
             pMNumAdjData->User.Desc.Flag & MNUM_DOT_POS_MASK,//С����λ��,������������ģ��С����ʾ
             Flag,                  //�������
             TGUI_NOTIFY_PASS(TMenu_MNumAdjEditNotify,TGUI_CBFUNID_TMENU_MNUM_EDIT));//�ص�����
    break;
  case MNUM_TYPE_FLOAT:
    TEditFloat(&pMNumAdjData->Edit,
             hWin,    //�����Ĵ���,���ݻ�����>Len
             //��ʼ��ֵ
             TData2Float(pMNumAdjData->User.Value[pMNumAdjData->User.CurItem]),    
             TData2Float(pDesc->Max),       //���ֵ,������ֵ����
             TData2Float(pDesc->Min),       //��Сֵ
             pMNumAdjData->User.Desc.Flag & MNUM_DOT_POS_MASK,//С����λ��,������������ģ��С����ʾ
             Flag,                  //�������
             TGUI_NOTIFY_PASS(TMenu_MNumAdjEditNotify,TGUI_CBFUNID_TMENU_MNUM_EDIT));//�ص�����
    break;
  default:break;
  }
}

/*****************************************************************
                      ��ʾ(�б������)ģʽ���
*****************************************************************/
//------------------------�õ���ͷ�ַ�������----------------------------
static char *_pGetItemHeader(const TMenu_t *pMenu,
                             TItemSize_t CurItem,
                             unsigned char HeaderW)    //ָ��Ĳ˵�ϵͳ
{
  //�����Ҷ���,����": "
  char *pString = TMenu_pGetBuf();
  const char *pHeader = TM_GetItemString(pMenu,CurItem);
    
  unsigned char Start = HeaderW - (strlen(pHeader) + 2);
  memset(pString, ' ', Start);
  pString += Start;
  pString = Tstrcpy_ex(pString,pHeader);
  *pString++ = ':';
  *pString++ = ' ';
  *pString = '\0';
  return TMenu_pGetBuf();
}

//------------------------�õ������ݺ���----------------------------
static char *_pGetItem(const TMenu_t *pMenu,//ָ��Ĳ˵�ϵͳ
                       TItemSize_t CurItem,
                       struct _MNumAdjData *pMNumAdjData)   
{
  struct _MNumDesc *pDesc;
  unsigned char Flag;
  char *pString = (char*)TMenu_pGetBuf() + TLCD_WIDTH;  //��ֹ���û������غ�;

  //�õ��û�����ǰ����������Ϣ
  TItemSize_t PrvItem = pMNumAdjData->User.CurItem;
  pMNumAdjData->User.CurItem = CurItem;
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_MNUM_GET_DESC,&pMNumAdjData->User);
  pMNumAdjData->User.CurItem = PrvItem;

  //�ǵ���ģʽʱ����ȡ��ǰ������ʵ��ʵʱ����
  if(!(pMenu->Type & TM_MNUMADJ_WRITE))
    TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_DATA,&pMNumAdjData->User);
  
  pDesc = &pMNumAdjData->User.Desc;
  Flag = pDesc->Flag;
  //����������ʾֵ
  switch(Flag & MNUM_TYPE_MASK){
  case MNUM_TYPE_DEC:
    pString = pTNum2StringFlag(pMNumAdjData->User.Value[CurItem],
                              pString,
                              TGetNumLen(pDesc->Max,pDesc->Min),//��ʾ����
                              Flag);  //��־��С����
    break;
  case MNUM_TYPE_FLOAT:
    pString = pTFloat2StringFlag(TData2Float(pMNumAdjData->User.Value[CurItem]),
                              pString,
                              TGetFloatLen(TData2Float(pDesc->Min),
                                          TData2Float(pDesc->Max),
                                          Flag & 0x07),//��ʾ����
                              Flag);  //��־��С����

  default:break;  //����
  }
  *pString = '\0';
  return (char*)TMenu_pGetBuf() + TLCD_WIDTH;  //��ֹ���û������غ�;
}


//ͨ������Ӧ��Ӧ��������ͨ��:
//TGUI_NOTIFY_GET_CAPTION ��ñ���
//TGUI_NOTIFY_GET_ITEM ��������ʾ����
//TGUI_NOTIFY_KEY_ENTER ÿһ���ȷ�ϼ�,��ʱӦ���ѡ����񲢽���Ӧ�ò㴦��
//������趨ϣ��,��ʾ�ڸ��ӵı��潹����
//TGUI_NOTIFY_EKY_ESCAPE �˳�����һ��
//-----------------------��ֵ����ģʽ�п��ͨ���ص�ʵ�ֺ���------------------------
//�����ڲ�ͨ��ʹ��
const void*TMenu_MNumAdjListBoxNotify(void *pvSource,
                                      unsigned char Type,
                                      const void *pvData)
{
  const TMenu_t *pMenu = pPopMenu();
  TItemSize_t Item = *(TItemSize_t*)pvData;   //��ǰ��λ��
  struct _MNumAdjData *pMNumAdjData = (struct _MNumAdjData*)pPopData();
  
  if(Item < TM_GetSize(pMenu))  
  pvSource = pvSource;//��ֹ���ֱ���������
  //�õ�ϵͳ��ǰָ������ݻ�����
  switch(Type){
  case TGUI_NOTIFY_GET_CAPTION://��ñ���
    return TM_GetHeader(pMenu);
  case TLISTBOXEX_NOTIFY_PREFIX://���ǰ׺
    return _pGetItemHeader(pMenu, Item,
                           pMNumAdjData->HeaderW);
  case TGUI_NOTIFY_GET_ITEM:// �������ʾ����
    if(TM_GetType(pMenu) & TM_MNUMADJ_WRITE) 
      return _pGetItem(pMenu,Item,pMNumAdjData);//��������
    return _pGetItemHeader(pMenu,Item,
                           pMNumAdjData->HeaderW);
  case TLISTBOXEX_NOTIFY_SUFFIX://��ú�׺
    return _pGetItem(pMenu,Item,pMNumAdjData);//��������
  case TGUI_NOTIFY_KEY_ENTER://ÿһ���ȷ�ϼ�
    pMNumAdjData->User.CurItem = Item;
    pMNumAdjData->Flag |= _FLAG_ENTER;//��̴���
    break;
  case TGUI_NOTIFY_KEY_ESCAPE://�˳�����һ��
    pMNumAdjData->Flag |= _FLAG_RETURN;
  default: break;
  }
  return NULL;
}

/*****************************************************************
                    ��ģ�����
*****************************************************************/

//-----------------------ȷ�ϼ���̴���----------------------------
//��ȷ�ϼ���Ӧ�ǵ���
static void _EnterLaterPro(struct _MNumAdjData *pMNumAdjData,
                           const TMenu_t *pMenu)
{
  TItemSize_t MaxItem = TM_GetSize(pMenu);
  //ȷ�ϼ���̴���:֪ͨ�û����洦��
  if(pMNumAdjData->User.CurItem >= MaxItem){//֪ͨ�û�������
     TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_SET_DATA,&pMNumAdjData->User);
     //����û������ı������
     if(pMNumAdjData->User.CurItem >= MaxItem){//λ��û��,������ȷ���˳�λ��
       pMNumAdjData->User.CurItem = MaxItem;//��ֹ�û���д����
       TListboxEx_Key(&pMNumAdjData->ListboxEx,TGUI_KEY_DOWN);
     }
     else{ //��λ�����ô���,��굽����λ��
       TListboxEx_SetSel(&pMNumAdjData->ListboxEx,pMNumAdjData->User.CurItem);
       TListBoxEx_PaintAll(&pMNumAdjData->ListboxEx);//������ʾ
     }
  }
  else{
    //ȷ�ϼ��ص�ֻ���ڷ�ѭ��ģʽʱ��Ч,��ʾ�������ģʽ
    if(TM_GetType(pMenu) & TM_MNUMADJ_WRITE){
      pMNumAdjData->Flag |= _FLAG_ADJ_MODE;
      _EnterEditMode(pMenu,pMNumAdjData);
    }
  }
    pMNumAdjData->Flag &= ~_FLAG_ENTER;
}
    


//------------------------��Ӧ��������ʵ��----------------------------
//2:��Ӧ��������������������û��������ݽ���,��������ʾ
//����ֵ����Ϊ: ��ֵ:������Ӧ�Ӳ˵�,-1,����,-2:�����ϲ�˵�
signed char TMenu_MNumAdjKey(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                             unsigned char Key,       //����ļ�ֵ
                             void *pv)      //�ѱ�ʹ�õ��������ݻ�����
{
  struct _MNumAdjData *pMNumAdjData = (struct _MNumAdjData *)pv;
  unsigned char Flag;
  PushMenu(pMenu);
  PushData(pv);
  
  if(pMNumAdjData->Flag & _FLAG_ADJ_MODE){//����ģʽʱ�����ڱ༭��
    TEdit_Key(&pMNumAdjData->Edit,Key);
    if(!(pMNumAdjData->Flag & _FLAG_ADJ_MODE))//�˳��༭��ʱ��Ҫ������ʾֵ
      TListBoxEx_PaintAll(&pMNumAdjData->ListboxEx);
  }
  //�������б��,�б��ǿ��Ƽ�ֻ���ڷ�ѭ��ģʽ��Ч
  else if((Key <= 0x20) || !(TM_GetType(pMenu) & TM_MNUMADJ_LOOP_MODE))
    TListboxEx_Key(&pMNumAdjData->ListboxEx,Key);
  
  //�������ڴ���
  Flag = pMNumAdjData->Flag;
  if(Flag & _FLAG_ENTER) _EnterLaterPro(pMNumAdjData,pMenu);
  //����һ��,�ൽ���˳����Ȱ��¼�,�ٰ�ȷ�ϼ�,�˲��ֿ���Ҳ�����ȷ�ϼ�����
  else if(Flag & (_FLAG_LEFT_END | _FLAG_RIGHT_END)){
    if(Flag & _FLAG_LEFT_END)//��ػ�
      TListboxEx_Key(&pMNumAdjData->ListboxEx,TGUI_KEY_UP);
    else//�һػ�
      TListboxEx_Key(&pMNumAdjData->ListboxEx,TGUI_KEY_DOWN);
    //���ڵ���ģʽʱֱ�ӽ���
    if(TListboxEx_GetSel(&pMNumAdjData->ListboxEx) <  TM_GetSize(pMenu)) 
      TListboxEx_Key(&pMNumAdjData->ListboxEx,TGUI_KEY_ENTER);
    //�ٴμ��ȷ�ϼ�
    if(pMNumAdjData->Flag & _FLAG_ENTER)
       _EnterLaterPro(pMNumAdjData,pMenu); 
    pMNumAdjData->Flag &= ~(_FLAG_LEFT_END | _FLAG_RIGHT_END);
  }
  //����˳�
  else if(Flag & _FLAG_RETURN){
    if(!TListboxEx_HaveAppend(&pMNumAdjData->ListboxEx))//�޸�����ʱ�˳�ǿ�Ʊ���
      TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_SET_DATA,&pMNumAdjData->User);
    return -2;
  }
    

  return -1;
}

//------------------------��������ʵ��----------------------------
//1:���������Ͳ˵�,���û���������ݲ�������ʾ
//���ش����ɹ�:
#include "TWidgetPrv.h"
signed char TMenu_MNumAdjCreate(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                                TWinHandle_t hWin,       //������˵��Ĵ���
                                unsigned char Data,//�������������,����������
                                void *pv)      //�ɹ�ʹ�õ����ɻ�����
{
  
  struct _MNumAdjData *pMNumAdjData = (struct _MNumAdjData *)pv;
  TItemSize_t w;  
  unsigned char Flag;
  
   //��ʼ������
  pMNumAdjData->Flag = 0;
  //��дģʽʱ��ʽ����
  if(TM_GetType(pMenu) & TM_MNUMADJ_WRITE){//����ģʽ
    Data = TLISTBOXEX_ALIGN_LEFT | TLISTBOXEX_ANTI_WORD2 | 
      TLISTBOXEX_PREFIX | TLISTBOXEX_EN_APPEND;
    Flag = TMenu_MNumAdj_cbGetCurStyle() | TMenu_MNumAdj_cbGetCurSaveStyle();
  }
  else{ 
    Data = TLISTBOXEX_ALIGN_LEFT | TLISTBOXEX_ANTI_WORD2 | 
      TLISTBOXEX_SUFFIX | TLISTBOXEX_EN_APPEND;
    Flag = TMenu_MNumAdj_cbGetCurStyle();
  }
  
  //�õ�����ͷ���
  unsigned char HeaderW = TM_GetItemMaxLen(pMenu) + 2;
  pMNumAdjData->HeaderW = HeaderW;
  HeaderW += 6; //��ֵ�����
  w = TWidget_GetDispW(TMenu_pGetTWin(hWin),Flag);  
  if(HeaderW < w){ //�õ�������ʾ��ƫ��ֵ
    HeaderW = (w - HeaderW) >> 1;
    if(HeaderW & 0x01) HeaderW--;//˫�ֽڶ���
    pMNumAdjData->HeaderW += HeaderW; 
  }


  //��ȡ��ǰ����
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_DATA,&pMNumAdjData->User);


  TListboxEx_Init(&pMNumAdjData->ListboxEx,  //���û�����Ĵ洢�ռ�
                  hWin,         //�ѽ�������ʼ���õĴ���
                  TM_GetSize(pMenu),  //Ԥ������Ŀ����
                  pMNumAdjData->HeaderW + 6,//��ֵ����λ�����λ��
                  //�ؼ���־,��������
                  Flag,
                  //��ʾ��ʽ
                  Data,
                  TGUI_NOTIFY_PASS(TMenu_MNumAdjListBoxNotify,TGUI_CBFUNID_TMENU_MNUM_LISTBOX)); //ͨ������

  //������ʾ
  #ifdef TMENU_WIN_VARIABLE
    if(Flag & (TLISTBOXEX_EN_RETURN | TLISTBOXEX_EN_SAVE)){//�и�����ʱ
      TMenu_ReBulidWin(&pMNumAdjData->ListboxEx,//���ô��ڴ�С
                       pMNumAdjData->HeaderW + 8,//��ֵ����λ�����λ��,�������С
                       TM_GetSize(pMenu) + 1,//���������
                       strlen(TM_GetHeader(pMenu)));
    }
    else{
      TMenu_ReBulidWin(&pMNumAdjData->ListboxEx,//���ô��ڴ�С
                       pMNumAdjData->HeaderW + 6,
                       TM_GetSize(pMenu),
                       strlen(TM_GetHeader(pMenu)));
    }
  #endif
  TListBoxEx_PaintAll(&pMNumAdjData->ListboxEx);
  //ѭ������ģʽģ�ⰴ��ֱ�ӽ���
  Flag = TM_GetType(pMenu);//Type
  if((Flag & TM_MNUMADJ_CREATE_ENTER) || //����ʱǿ�ƽ���
     ((Flag & (TM_MNUMADJ_LOOP_MODE | TM_MNUMADJ_WRITE)) == 
       (TM_MNUMADJ_LOOP_MODE | TM_MNUMADJ_WRITE))) 
    TMenu_MNumAdjKey(pMenu,TGUI_KEY_ENTER,pv);
  return -1;
}


