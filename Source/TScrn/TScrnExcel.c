/*******************************************************************************

                           ��Ļ��ʾ֮���ӱ��Ԫʵ��

*******************************************************************************/

#include <String.h>

#include "TListbox.h"
#include "TScrnExcel.h"

#include "key.h"
/**********************************************************************
                       ģ���ڲ�
**********************************************************************/
//-------------------------TListBoxExͨ������ʵ��-------------------------
static const void* _Callback(void *pvSource,
                             unsigned char Type,
                             const void *pvData);

//-------------------------��ʼ������-------------------------
//ע������Ϊ�Ӵ�����ʾʱ��Ӧ�ÿհ״�����Ϊ�丸����
void TScrnExcel_Init(struct _TScrnExcel *pExcel,
                       const struct _TScrnExcelData *pData, //�û�����
                       TWinHandle_t   hMain,          //���������ڴ���
                       TWinHandle_t   hHeader,        //����ͷ���ڴ���
                       unsigned char Cfg)           //�������
{
  memset(pExcel, 0, sizeof(struct _TScrnExcel));  //��ʼ����0
  pExcel->pData = pData; 
  pExcel->hMain = hMain;
  pExcel->hHeader = hHeader;
  pExcel->Flag = Cfg;

  //��ʾ����ͷ���õ�����
  const char *pHeader = pData->pGetHeader(pData);
  TWin_SetString(TWM_pGetWin(hHeader),0,pHeader);
  
  //��ʾ������
  TListboxEx_Init(&pExcel->TListboxEx,         //���û�����Ĵ洢�ռ�
                  hMain,                       //�ѽ�������ʼ���õĴ���
                  pData->GetItemCount(pData),  //Ԥ������Ŀ����
                  strlen(pHeader),             //�ַ�������Ա���ͷΪ׼
                  TWIDGET_EN_RETURN,           //�ؼ���־
                  TLISTBOXEX_ALIGN_LEFT | TLISTBOXEX_ANTI_ROW,//��ʾ��ʽ
                  _Callback); //ͨ������
  
  //ͨ����Ҫ��������
  if(pData->UpdateData(pData, 
                       TListboxEx_GetY(&pExcel->TListboxEx),
                       TListboxEx_GetH(&pExcel->TListboxEx))){
    pExcel->Flag |= TSCRN_EXCEL_DATA_FINAL;
    //���ݸ�����ɣ��ٸ�����ʾ
    TScrnExcel_ReBulid(pExcel);
  }
}

//-------------------------��Ӧ��������-------------------------
//�����Ƿ��˳�(��0�˳�)
int TScrnExcel_Key(struct _TScrnExcel *pExcel, unsigned char Key)
{
  //����Ϣʱֻ��Ӧ�˳���
  if((Key != TGUI_KEY_ESCAPE) && (!TListboxEx_GetItems(&pExcel->TListboxEx)))
    return 0;
  
  unsigned short PrvSel = TListboxEx_GetSel(&pExcel->TListboxEx);  
  
  //ֱ�ӽ����䴦�������ڻص���ʵ��ת��
  TListboxEx_Key(&pExcel->TListboxEx,Key);

  if(PrvSel != TListboxEx_GetSel(&pExcel->TListboxEx)){//Y���ƶ��ˣ�������û�����
    TScrnExcel_ReBulid(pExcel);
    return 0;
  }
  
  const struct _TScrnExcelData *pData = pExcel->pData;
  //�����ƶ�ͬ����������
  if(((Key == TGUI_KEY_LEFT) || (Key == TGUI_KEY_RIGHT))){
    TWin_SetString(TWM_pGetWin(pExcel->hHeader), 0,
                   pData->pGetHeader(pData) + 
                   TListboxEx_GetX(&pExcel->TListboxEx));
  }
  //�������̴���
  else if(pExcel->Flag & TSCRN_EXCEL_IN_ENTER){ //������˵�
    pExcel->Flag &= ~TSCRN_EXCEL_IN_ENTER;
    if(pData->EnterKey != NULL){
      if(pData->LineToAryId != NULL) PrvSel = pData->LineToAryId(pData, PrvSel);//ת��
      pData->EnterKey(PrvSel);      
    }  
  }
  
  //�˳�����̴���
  else if(pExcel->Flag & TSCRN_EXCEL_IN_RETURN){
    pExcel->Flag &= ~TSCRN_EXCEL_IN_RETURN;
    if(pData->ExitKey != NULL) pData->ExitKey();
    return -1;
  }
  return 0;
}

//-------------------------�����������ͨ��-------------------------------------
//�Ӻ��������ʱ�����ô˺���˵�����ݸ������
void TScrnExcel_UpdateDataFinal(struct _TScrnExcel *pExcel)
{
  pExcel->Flag |= TSCRN_EXCEL_DATA_FINAL;
  const struct _TScrnExcelData *pData = pExcel->pData;
  //������ȸı���
  TListboxEx_SetItems(&pExcel->TListboxEx, pData->GetItemCount(pData));
  TScrnExcel_Refurbish(pExcel); //ˢ������
}

//-------------------------�ؽ�����-------------------------------------
//���¸��µ�ҳ�������ݲ���ʾ
void TScrnExcel_ReBulid(struct _TScrnExcel *pExcel)
{
  const struct _TScrnExcelData *pData = pExcel->pData;
  
  //�����������
  if(pData->UpdateData(pData, 
                    TListboxEx_GetY(&pExcel->TListboxEx),
                    TListboxEx_GetW(&pExcel->TListboxEx))){
    //������ȸı���
    TListboxEx_SetItems(&pExcel->TListboxEx, pData->GetItemCount(pData));
    TScrnExcel_Refurbish(pExcel); //ˢ������
  }
  else{//��������δ���
    pExcel->Flag &= ~TSCRN_EXCEL_DATA_FINAL;
  }
}

//--------------------ˢ�º���------------------------------
void TScrnExcel_Refurbish(const struct _TScrnExcel *pExcel)
{
  const struct _TScrnExcelData *pData = pExcel->pData;
  //���±���ͷ
  TWin_SetString(TWM_pGetWin(pExcel->hHeader), 0,
                 pData->pGetHeader(pData) + 
                   TListboxEx_GetX(&pExcel->TListboxEx));
  TListBoxEx_PaintAll(&(pExcel)->TListboxEx);
}

//-------------------------TListBoxExͨ������ʵ��-------------------------
//ͨ������Ӧ��Ӧ��������ͨ��:
//TGUI_NOTIFY_GET_CAPTION ��ñ���
//TGUI_NOTIFY_GET_ITEM ��������ʾ����
//TGUI_NOTIFY_KEY_ENTER ÿһ���ȷ�ϼ�,��ʱӦ���ѡ����񲢽���Ӧ�ò㴦��
//������趨ϣ��,��ʾ�ڸ��ӵı��潹����
//TGUI_NOTIFY_EKY_ESCAPE �˳�����һ��
static const void* _Callback(void *pvSource,
                             unsigned char Type,
                             const void *pvData)
{
  struct _TScrnExcel *pExcel = pcbGetExcel((TListboxEx_t *)pvSource); 
  TItemSize_t Item = *(TItemSize_t*)pvData;   //��ǰ��λ��
  const struct _TScrnExcelData *pData = pExcel->pData;
  
  //�õ�ϵͳ��ǰָ������ݻ�����
  switch(Type){
  //case TGUI_NOTIFY_GET_CAPTION://��ñ���
  //  break;
  case TGUI_NOTIFY_GET_ITEM:  //�������ʾ����
    return pData->pGetLine(pData, Item);
  case TGUI_NOTIFY_KEY_ENTER://ÿһ���ȷ�ϼ�
    pExcel->Flag |= TSCRN_EXCEL_IN_ENTER;//����ָ����,��̴����ֹ��ջ����
    break;
  case TGUI_NOTIFY_KEY_ESCAPE://���ذ���
    if(pExcel->Flag & (TSCRN_EXCEL_EN_EXIT | TSCRN_EXCEL_FORCE_EXIT))
      pExcel->Flag |= TSCRN_EXCEL_IN_RETURN;
    else //���ص���һ��
      TListboxEx_SetSel((TListboxEx_t *)pvSource,0);
    break;
  default: break;
  }
  return NULL;
}

//-----------------------�õ���ǰ��ѡ���Ӧ������ID-------------------------
unsigned short Excel_GetAryId(const struct _TScrnExcel *pExcel)
{
  const struct _TScrnExcelData *pData = pExcel->pData;
  unsigned short Sel = TScrnExcel_GetSel(pExcel);
  if(pData->LineToAryId == NULL) return Sel; //��Ӧ��ϵ
  return pData->LineToAryId(pData, Sel);
}

//----------------------��ת��ָ��AryIdλ�ú���-------------------------
//����0�ɹ�
signed char TScrnExcel_JumpToAryId(struct _TScrnExcel *pExcel,
                                    unsigned short AryId)
{
  const struct _TScrnExcelData *pData = pExcel->pData;
  unsigned short Line;
  if(pData->AryIdToLine == NULL) Line = AryId; //��֧��ʱ����һһ��Ӧ
  else Line = pData->AryIdToLine(pData, AryId);
  if(Line >= TScrnExcel_GetItemCount(pExcel)) //�쳣(��֧��ʱ���ڴ�ҳ)����δִ��
    return -1;

  //�ù�겢���µ�ǰλ��
  TListboxEx_SetSel(&pExcel->TListboxEx, Line);
  return 0;
}
