/*******************************************************************************

                           TWidget֮�༭��ʵ��
*******************************************************************************/
#include "TEdit.h"
#include <string.h>
#include "TEditPrv.h"  //�ڲ�����,�����ڲ�ʹ��
#include "TEdit_Float.h"

//---------------------�õ�ָ��λ��λ���ַ�����----------------
char TEdit_GetChar(const struct _TEdit *pEdit,unsigned char Pos)
{
  return TWin_GetChar(TWM_pGetWin((pEdit->hWin)),Pos,0);
}

//-----------------------�õ��ַ�������----------------------------
//�˺������������ַ�������ģʽ
const char* TEdit_pGetString(const struct _TEdit *pEdit)
{
  return TWin_pGetString(TWM_pGetWin(pEdit->hWin),0);
}

//----------------�õ��������������--------------------
//��õ�ǰ�ѵ�����ɵ���(��������ʹ��TData2Float)ת���õ�
TSigned_t TEdit_GetVolume(const TEdit_t *pEdit)
{
  #if TGUI_EDIT_FLOAT_SUPPORT != 0
  float Value;
  if(pEdit->eType == TEditType_Float){
    Value = (float)((TSigned_t)pEdit->Value);
    Value /= (float)TMutiTUnigned[pEdit->DotPos];
    return TFloat2Data(Value);
  }
  #endif
  return pEdit->Value;
}

//------------------��ͼ���������»���������------------------
void TEdit_PaintAll(TEdit_t *pEdit)
{
  //��������ᵼ�²��ɼ���������,��ֱ��ʹ��case
  switch(pEdit->eType){
  case TEditType_Float:  //��������
  case TEditType_Dec:TEditDec_UpdateBuf(pEdit);
  default:break;
  }
}

//-----------------------����������------------------------------
void TEdit_Key(TEdit_t *pEdit,unsigned char Key)
{
  //��������ᵼ�²��ɼ���������,��ֱ��ʹ��case
  switch(pEdit->eType){
  case TEditType_Float:  //��������
  case TEditType_Dec:TEditDec_KeyPro(pEdit,Key);
  default:break;
  }
}



