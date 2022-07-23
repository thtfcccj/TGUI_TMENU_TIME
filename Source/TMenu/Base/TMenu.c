/*******************************************************************************

                           TMenu������������ʵ��

*******************************************************************************/
#include "TMenu.h"
#include "TMenu_CheckBox.h"
#include "TMenu_MNumAdj.h"
#include "TMenu_RadioBox.h"
#include "TMenu_SubMenu.h"
#include "TMenu_Buf.h"
#include "TMenu_EditSel.h"
#include <string.h>

#ifndef TMENU_DIS_EDIT_STRING 
  #include "TMenu_EditString.h"
#endif 

/**********************************************************************
                     �ڲ����ݽṹ-�˵�������
**********************************************************************/

//��һ����ʾ��ֻ����һ���˵���,����ʹ��һƬ�����ڴ�������˵�ϵͳ
union _MenuDataPv{
  struct _CheckboxData CheckBox;
  struct _MNumAdjData MNumAdj;
  struct _RadioBoxData RadioBox;
  struct _SubMenuData SubMenu;
  struct _EditSelData EditSel;
  struct _EditSelData EditString;  
};

//�����Ӳ˵����ٽṹ
struct _Trace{
  unsigned char Pos[TMENU_LAYER_COUNT]; //�����Ӳ˵��ڸ����λ��
  unsigned char Layer;                  //��ǰ�˵����ڲ���
};

//�˵�������
struct _TMenuMng{
  const TMenu_t *pCurMenu;    //ָ��ǰ�˵�,���ڲ˵���ʱΪNULL
  struct _Trace Trace;        //�˵�������
  union _MenuDataPv vData;    //��ǰ�˵�ʹ�õ�����������
};

/**********************************************************************
                     �˵�ϵͳ��̬�ڴ����
**********************************************************************/
//�ض��������ڸ�����ֹ����
#if TMENU_MWIN_SIGNAL > 1
  #undef TMENU_MWIN_SIGNAL
  #define TMENU_MWIN_SIGNAL  1
#endif

static struct _TMenuMng MenuMng;    //ֱ�Ӿ�̬����  
//������:
#define	TMenu_GetCurMenuMng() (&MenuMng)

//��̬���崰��,�����˵�����ռ����,һ���˵�����,һ���༭�ؼ�ʹ��
static TWin_t _Win[TMENU_FIX_WIN_COUNT + TMENU_MWIN_SIGNAL + TMENU_MSG_BOX + 2];

//Ϊ�˵����ڷ��仺���
#if(TMENU_MWIN_SIGNAL == 0)
static char _MenuBuf[TMENU_WIN_W * TMENU_WIN_H];
#endif

//Ϊ�˵����ڸ��ӵı༭�ؼ�����ռ�
static char _EditBuf[TMENU_WIN_W];

//�˵�������ʹ��TWinMng_Row,Ϊ�侲̬����ռ�:
#include "TWinMng_Row.h"//ʹ�ò�ʵ�����ù�����������ϵͳ
static TWinMng_t _TWinMng;

//��������
//Ϊ��ֹ����������ֵ������,ֱ��ʹ��case 
/*static TMenuOperate_t _Oprate[] = 
{
  {TMenu_SubMenuCreate, TMenu_SubMenuKey},      //�Ӳ˵�ģʽ
  {TMenu_RadioBoxCreate, TMenu_RadioBoxKey},       //��ѡ��ģʽ
  {TMenu_MNumAdjCreate,TMenu_MNumAdjKey},       //��ֵ����ģʽ
  {TMenu_CheckBoxCreate, TMenu_CheckBoxKey},       //��ѡ��ģʽ  
  //{},       //�б��ģʽ(��δʵ��)
};*/

/**********************************************************************
                     ��Ա��������
**********************************************************************/

//-----------------------�õ���ǰ�˵�ָ��----------------------------
//ָ��ΪNULLʱ��ʾ���ڲ˵���
const TMenu_t *pTMenu_GetCurMenu(void)
{
  return TMenu_GetCurMenuMng()->pCurMenu;
}

//-----------------------�õ���ǰ�˵����----------------------------
unsigned char TMenu_GetCurMenuLayer(void)
{
  return TMenu_GetCurMenuMng()->Trace.Layer;
}

//-----------------------ǿ�ƽ���ָ���˵�------------------------
struct _GotoMenuData{
  const TMenu_t *pGogoMenu;//��ת�Ĳ˵�
  unsigned short MenuID;  //�˵�ID��
}GotoMenuData;

//-----------------------ǿ�ƽ���ָ���˵�------------------------
void TMenu_GogoMenu(const TMenu_t *pCurMenu, //ǿ������Ĳ˵��ṹ
                    unsigned short MenuID,  //�˵�ID,��ЧʱΪ0
                    unsigned char Layer)    //�ýṹ���ڲ˵��ڵĲ��
{
  //���ٲ˵�λ��
  TMenu_GetCurMenuMng()->Trace.Layer = Layer;
  //�ݴ�
  GotoMenuData.pGogoMenu = pCurMenu;
  GotoMenuData.MenuID = MenuID;
}

//----------------------�õ���ת�Ĳ˵���-------------------
//����NULL��ʾ�޺�̲˵�
static const TMenu_t *_GetGogoMenu(void)  //�ýṹ���ڲ˵��ڵĲ��
{
  const TMenu_t *pMenu;
	if(GotoMenuData.pGogoMenu || GotoMenuData.MenuID){
    pMenu =  TM_pGetMenu(GotoMenuData.pGogoMenu,GotoMenuData.MenuID);
    memset(&GotoMenuData,0,sizeof(struct _GotoMenuData));//����
  }
  else pMenu = NULL;
  return pMenu;
}

//-----------------------ǿ��ˢ�µ�ǰ�˵�����--------------------------
//���ô˺���������ˢ�µ�ǰ�˵���������ʾ����
//ͨ����ָ���˵����ڵ��ô˺�������ʵ�����ݵĶ�̬ˢ��
#include "TListBox.h"
void TMenu_Refresh(void)
{
  const TMenu_t *pCurMenu = pTMenu_GetCurMenu();
  struct _TMenuMng *pMng = TMenu_GetCurMenuMng();
  unsigned char Type = TM_GetType(pCurMenu) & TMTYPE_MASK;
  //������ģʽ�����û�����ˢ��
  if(Type == TMTYPE_BUF){
    #ifndef TMENU_DIS_BUF 
      TMenu_BufRefresh(pCurMenu,&pMng->vData);
    #endif
    return;
  }
  //�༭�ַ���ģʽ����ǿ��ˢ�¹���
  if(Type == TMTYPE_EDITSTRING){
    return;
  }
  if(Type == TMTYPE_MNUMADJ){//��ֵģʽ
    TMenu_MNumAdjRefresh(pCurMenu,&pMng->vData);
    return;
  }  
  
  //����ģʽֱ�����ü̳й�ϵ
  ListBox_PaintAll((TListbox_t *)(&pMng->vData));
}

//-----------------���ݴ���ID�õ����ھ���õ�����-------------------
//��������,����ϵͳ��ʼ��ʱʹ��
TWin_t *TMenu_pGetTWin(unsigned char WinID)
{
  return  &_Win[WinID];
}

//-----------------�����ǿ���ƶ���ָ��λ�ú���-------------------
void TMenu_MoveCousor(unsigned char Pos)
{
  //ֱ�����ü̳й�ϵ
  struct _TMenuMng *pMng = TMenu_GetCurMenuMng();
  TListbox_SetSel((TListbox_t *)(&pMng->vData), Pos);
  ListBox_PaintAll((TListbox_t *)(&pMng->vData));//������ʾ
}

/**********************************************************************
                     ��Ϊ����
**********************************************************************/
#ifndef NULL_TMENU
//-------------------------------TMenu��ʼ������-----------------------
//����ʱ���ó�ʼ���˵�ϵͳ
void TMenuInit(void)
{
  unsigned char i;
  memset(&MenuMng,0,sizeof(struct _TMenuMng));
  //��ʼ���˵�����
  #if(TMENU_MWIN_SIGNAL == 0)
  TWin_InitWin(&_Win[TM_hGetMenuWin()],
               TMENU_WIN_W,TMENU_WIN_H,
               _MenuBuf,sizeof(_MenuBuf));
  #endif
  //��ʼ���༭�ؼ�����
  TWin_InitWin(&_Win[TM_hGetEditWin()],
               TMENU_WIN_W,1,
               _EditBuf,sizeof(_EditBuf));
  //�˵����ϴ���Ĭ����Ϊ����״̬
  for(i = TMENU_FIX_WIN_COUNT + TMENU_MWIN_SIGNAL + 1; 
      i < TMENU_FIX_WIN_COUNT + TMENU_MWIN_SIGNAL + TMENU_MSG_BOX + 2;
      i++){
    TWin_Hidden(&_Win[i]);

  }

  //��ʼ��WM������
  TWinMng_Init(&_TWinMng,0,_Win,
               TMENU_FIX_WIN_COUNT + TMENU_MWIN_SIGNAL + TMENU_MSG_BOX + 2);
  //�ҽӹ�����
  TWinMng_SetCur(&_TWinMng);
}


//------------------------------����˵���������----------------------------
//���ط�0ʱ��ʾ�˳��˵�ϵͳ
static signed char _EnterMenu(struct _TMenuMng *pMng,
                              const TMenu_t *pCurMenu,//�˵�λ��
                              unsigned char Data)     //�������������

{
  unsigned char Type = TM_GetType(pCurMenu) & TMTYPE_MASK;
  if(Type > TMTYPE_MAX){//�˵�ϵͳ����
    TMenu_GetCurMenuMng()->pCurMenu = NULL;
    return -2;
  }
  TMenu_GetCurMenuMng()->pCurMenu = pCurMenu;//���²˵�

  TMenu_MaxWin(Type, TM_hGetMenuWin()); //��󻯴���(�ɱ䴰����)
  
  //������ǰ�˵�,Ϊ��ֹ����������ֵ������,ֱ��ʹ��case:
  //_Oprate[Type].Create(pCurMenu,TM_hGetMenuWin(),
  //                      Data,&pMng->vData);
  memset(&pMng->vData, 0, sizeof(union _MenuDataPv));//ʹ��ǰ��ʼ��
  switch(Type){
  #ifndef TMENU_DIS_SUBMENU
  case TMTYPE_SUBMENU://�Ӳ˵�ģʽ
    TMenu_SubMenuCreate(pCurMenu,TM_hGetMenuWin(),
                        Data,&pMng->vData);
    break;
  #endif
  #ifndef TMENU_DIS_RADIOBOX
  case TMTYPE_RADIOBOX://��ѡ��ģʽ
    TMenu_RadioBoxCreate(pCurMenu,TM_hGetMenuWin(),
                        Data,&pMng->vData);
    break;
  #endif
  #ifndef TMENU_DIS_MNUM_ADJ
  case TMTYPE_MNUMADJ://��ֵ����ģʽ
    TMenu_MNumAdjCreate(pCurMenu,TM_hGetMenuWin(),
                        Data,&pMng->vData);
    break;
  #endif
  #ifndef TMENU_DIS_CHECKBOX 
  case TMTYPE_CHECKBOX://��ѡ��ģʽ
    TMenu_CheckBoxCreate(pCurMenu,TM_hGetMenuWin(),
                        Data,&pMng->vData);
    break;
  #endif
  #ifndef TMENU_DIS_BUF 
  case TMTYPE_BUF://������ģʽ
    TMenu_BufCreate(pCurMenu,TM_hGetMenuWin(),
                        Data,&pMng->vData);
    break;
  #endif
  #ifndef TMENU_DIS_EDIT_SEL 
  case TMTYPE_EDITSEL://�༭ѡ��ģʽ
    TMenu_EditSelCreate(pCurMenu,TM_hGetMenuWin(),
                        Data,&pMng->vData);
    break;
  #endif    
  #ifndef TMENU_DIS_EDIT_STRING 
  case TMTYPE_EDITSTRING://�༭�ַ���ģʽ
    TMenu_EditStringCreate(pCurMenu,TM_hGetMenuWin(),
                           Data,&pMng->vData);
    break;
  #endif 
  default:break;
  }
  return 0;
}

//--------------------------------�˳��˵�����-------------------------------
void TMenu_ExitMenu(void)
{
  if(TMenu_GetCurMenuMng()->pCurMenu == NULL) return;//���ڲ˵���
  TMenu_GetCurMenuMng()->pCurMenu = NULL;
  #if(TMENU_MWIN_SIGNAL > 0)//����������ʱ�������
    TWM_FreeHandle(TM_hGetMenuWin());
  #else
    TWin_ClrFocus(TWM_pGetWin(TM_hGetMenuWin()));//ȡ���˵��ڵĽ�����ʾ
  #endif
  //��������
  TWM_FreeHandle(TM_hGetEditWin());
    
  TMenu_cbExitMenuNotify();//���ͨ���˳��˵�ϵͳ
}

//-------------------------------TMenu����������----------------------------
//���˵����ڲ˵��ڲ�ʱ�����˺���
//���ط�0ʱ��ʾ�˳��˵�ϵͳ
signed char TMenuKey(unsigned char Key)
{
  struct _TMenuMng *pMng = TMenu_GetCurMenuMng();
  const TMenu_t *pMenu;
  const TMenu_t *pCurMenu = pMng->pCurMenu;
  signed char Return = 0;
  //δ���˵�ʱ
  if(!pCurMenu){
    TMenu_cbEnterMenuNotify();//��ͨ������˵�ϵͳ
    if(Key == TGUI_KEY_ENTER){
      #if (TMENU_MWIN_SIGNAL > 0) //�����ڶ���ʱ����Ϊ��ʾ״̬
        TWin_Disp(TMenu_pGetTWin(TM_hGetMenuWin()));
      #endif
      pCurMenu = TM_pGetTopMenu();//ȷ�ϼ����˵�
      pMng->Trace.Layer = 0; //���ٳ�ʼ��
      memset(pMng->Trace.Pos,0,TMENU_LAYER_COUNT);
      return _EnterMenu(pMng,pCurMenu,0);
    }
    //else //����������Ч
    return 0;
  }
  //�˵��ڰ�������,Ϊ��ֹ����������ֵ������,ֱ��ʹ��case:
  //Return = _Oprate[TM_GetType(pCurMenu) & TMTYPE_MASK].Key();
  switch(TM_GetType(pCurMenu) & TMTYPE_MASK){
  #ifndef TMENU_DIS_SUBMENU
  case TMTYPE_SUBMENU://�Ӳ˵�ģʽ
    Return = TMenu_SubMenuKey(pCurMenu,Key,&pMng->vData);
    break;
  #endif
  #ifndef TMENU_DIS_RADIOBOX
  case TMTYPE_RADIOBOX://��ѡ��ģʽ
    Return = TMenu_RadioBoxKey(pCurMenu,Key,&pMng->vData);
    break;
  #endif
  #ifndef TMENU_DIS_MNUM_ADJ
  case TMTYPE_MNUMADJ://��ֵ����ģʽ
    Return = TMenu_MNumAdjKey(pCurMenu,Key,&pMng->vData);
    break;
  #endif
  #ifndef TMENU_DIS_CHECKBOX  
  case TMTYPE_CHECKBOX://��ѡ��ģʽ
    Return = TMenu_CheckBoxKey(pCurMenu,Key,&pMng->vData);
    break; 
  #endif
  #ifndef TMENU_DIS_BUF  
  case TMTYPE_BUF://������ģʽ
    Return = TMenu_BufKey(pCurMenu,Key,&pMng->vData);
    break; 
  #endif
  #ifndef TMENU_DIS_EDIT_SEL 
  case TMTYPE_EDITSEL://�༭ѡ��ģʽ
    Return = TMenu_EditSelKey(pCurMenu, Key, &pMng->vData);
    break;
  #endif  
  #ifndef TMENU_DIS_EDIT_STRING 
  case TMTYPE_EDITSTRING://�༭�ַ���ģʽ
    Return = TMenu_EditStringKey(pCurMenu, Key, &pMng->vData);
    break;
  #endif      
  default: return 0;//����
  }
  
  //���ǿ����ת�˵�״̬
  pMenu = _GetGogoMenu();
  if(pMenu)  //ǿ����ת
    _EnterMenu(pMng,pMenu,pMng->Trace.Pos[pMng->Trace.Layer]);
  //��鷵��״̬
  else if(Return >= 0){//�����Ӳ˵��������
    //���ٲ˵�λ��
    pMng->Trace.Pos[pMng->Trace.Layer] = Return;
    pMng->Trace.Layer++;
    pCurMenu = TM_pGetSubMenu(pCurMenu,Return);
    return _EnterMenu(pMng,pCurMenu,0);
  }
  else if(Return == -2){//�����ϼ�
    //���ٲ˵�λ��
    pMng->Trace.Layer--;
    pCurMenu = TM_pGetParent(pCurMenu);
    if(pCurMenu == NULL){
      #if (TMENU_MWIN_SIGNAL > 0) //�����ڶ���ʱ�˳��Զ���Ϊ����ʾ״̬
        TWin_Hidden(TMenu_pGetTWin(TM_hGetMenuWin()));
      #endif
      TMenu_ExitMenu();//�˳��˵�
      return -1;
    }
    return _EnterMenu(pMng,pCurMenu,pMng->Trace.Pos[pMng->Trace.Layer]);
  }
  return 0;//����״̬δ�������
}

//-------------------------------TMenu���̺���----------------------------
//10ms����һ��
void TMenuTask(void)
{
  /*/�˽�����Ҫ�����ڹ�����
  static unsigned char Timer;
  
  Timer++;
  if(Timer) TWinMng_UpdateRow();
  else TWinMng_UpdateLcd();*/

  while(TWinMng_UpdateRow()) 
    TWinMng_UpdateLcd();
}


#endif //#ifndef NULL_TMENU

/**********************************************************************
                        �˵�����ջʵ��
**********************************************************************/

//���˵��ṹ��ջ
const TMenu_t *pPopMenu(void)
{
  return TMenu_GetCurMenuMng()->pCurMenu;
}


//����ǰָ������ݻ�������ջ
void *pPopData(void)
{
  return &TMenu_GetCurMenuMng()->vData;
}



/**********************************************************************
                         TGUI�ص�����ʵ��
**********************************************************************/

//����Ϊ��������ʹ�õĳ�������,����Ƭ����֧�ֳ�����ʱ,��ֱ�Ӷ��嵽��������
//��������λ���ұ�(����ʾ���߶�>8ʱ�Ŷ���)

//�ַ�������λ���ұ�
const unsigned char  U8ShiftLUT[8] = { 	
  0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80
};

//-----------------TGUI����Ҫ����ʱ�ַ�������----------------------
#ifdef TGUI_BUF_SIZE //�Զ����Сʱ
  unsigned long _TGUI_Buf[TGUI_BUF_SIZE / 4];	//���ǵ���ʾ���ᳬ����ʾ��,�ʼӴ�����
#else
  unsigned long _TGUI_Buf[TLCD_WIDTH / 2];	//���ǵ���ʾ���ᳬ����ʾ��,�ʼӴ�����
#endif
  
char *TGUI_pGetBuf(void)      
{
  return (char*)_TGUI_Buf;
}   

//-----------------------�õ�����ť֮��ļ��--------------------
//������ʾ����С�뵱ǰ��ڿ�ȵõ���ť֮��ļ��
unsigned char cbGetBottomSpace(TItemSize_t ItemW)
{
  ItemW = ItemW;
  return 2;
}














