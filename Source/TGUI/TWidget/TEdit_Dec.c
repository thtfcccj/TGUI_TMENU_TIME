/*******************************************************************************

                           TWidget֮�༭��-ʮ����������ʵ��

*******************************************************************************/
#include "TEdit.h"
#include <string.h>
#include "TEditPrv.h"  //�ڲ�����,�����ڲ�ʹ��

//---------------------�����趨����ֵ����-----------------------
//�������趨,��������ʾ
static void _SetValue(struct _TEdit *pEdit, TSigned_t Value)
{
  if (pEdit->Value == Value) return;  
  if(Value < pEdit->Min) Value = pEdit->Min;
  else if(Value > pEdit->Max) Value = pEdit->Max;
  pEdit->Value = Value;
  TEdit_PaintAll(pEdit);              
}

//-----------------------����������������----------------------------
//���������Ƿ�ɹ�,0�ɹ�,�������ɹ�
static signed char _SetSign(struct _TEdit *pEdit,
                            unsigned char Key)      //����
{
  TSigned_t Value;
  unsigned char Pos;

  if(!(pEdit->Flag & TEDIT_EN_SIGN))  return -2;//������������ʾʱ

  if(pEdit->Flag & TEDIT_EN_NOTIFY) Pos = 1;//����ʾ
  else Pos = 0;
  if(pEdit->Pos != Pos) return -1;//������λ�ⲻ��������  
  
  Value = pEdit->Value;
  if((Key == TGUI_KEY_UP) || (Key == TGUI_KEY_DOWN))
    goto ValueChange;
  if((Key == '+') && (Value < 0)) //����С��0ʱ
    goto ValueChange;
  if((Key == '-') && (Value > 0)) //���Ŵ���0ʱ
    goto ValueChange;
  return -3;      //������Ч
    
ValueChange:
  Value = 0 - Value;//ȡ����ֵ
  _SetValue(pEdit,Value);//����
  return 0;
}

//-----------------------�õ���ʼ����λ����----------------------------
static unsigned char _GetStartPos(const struct _TEdit *pEdit)
{
  //���ݱ�־,λ������ֵ��Ϣ������Ч��
  unsigned char MaxLen;
  unsigned char Len;
  if(pEdit->Flag & TEDIT_EN_NOTIFY) Len = 1; //����ʾ
  else Len = 0;
  if(pEdit->Flag & TEDIT_EN_SIGN){ //������ʱ,������λ�Ƿ�ɵ���
   if((pEdit->Max < 0) || (pEdit->Min > 0))
     Len++;
  }
  MaxLen = TGetNumLen(pEdit->Min,pEdit->Max);
  if(pEdit->DotPos >= MaxLen) //<1��,�õ�����λ��ʼ����
    Len += (pEdit->DotPos - MaxLen) + 2;//0.ռ��λ

  return Len;
}

//-----------------------�õ�ָ��λ��ǰ����Ч�ַ�����----------------------------
//ȥ����ֵǰ�����Ч����ַ���:<-XXX.XX>,<0ʱ,�貹0:
//<-10.0>
//������Ч��,�������ֵ��ָ������ٸ�
static signed char _GetPreValidLen(const struct _TEdit *pEdit,
                         unsigned char Pos)
{
  //���ݱ�־,λ������ֵ��Ϣ������Ч��
  unsigned char Len;
  Pos++;    //��λ������ɳ���
  
  if(pEdit->Flag & TEDIT_EN_NOTIFY) Pos--;//����ʾ
  if(pEdit->Flag & TEDIT_EN_SIGN) Pos--;//������
  if(!pEdit->DotPos) return Pos;
  //��С����ʱ,�ȼ����ֵ�Ƿ�<1,����,����ǰ�油�˶��ٸ�0.��0.0
  Len = TGetNumLen(pEdit->Min,pEdit->Max);
  if(pEdit->DotPos >= Len) //<1��,0.
    Pos -= (pEdit->DotPos - Len) + 2;
  //��鵱ǰλ����С���������ұ�
  else{
    Len -= pEdit->DotPos;
    if(Pos > Len) Pos--;
  }
  return Pos;
}

/**********************************************************************
                         ��Ա��������
**********************************************************************/

//-----------------ʮ���Ƶ���ʱ���µ�������������------------------
void TEditDec_UpdateBuf(const struct _TEdit *pEdit)
{
  unsigned char Len;
  unsigned char Flag = pEdit->Flag;
  TWin_t *pWin = TWM_pGetWin(pEdit->hWin);
  char *pBuf = TWin_pGetString(pWin,0);
  char *pPrvBuf = pBuf;
  if(Flag & TEDIT_EN_NOTIFY) *pBuf++ = '<';//����ʾ
  //�����ֵ
  pBuf = pTNum2StringFlag(pEdit->Value,pBuf,TGetNumLen(pEdit->Min,pEdit->Max),
                         (Flag & TEDIT_EN_SIGN) | pEdit->DotPos);
  if(Flag & TEDIT_EN_NOTIFY) *pBuf++ = '>';//����ʾ
  Len = pBuf - pPrvBuf;
  if(Len < TWin_GetW(pWin))   //�����ǿ�����ո�
    memset(pBuf,' ',TWin_GetW(pWin) - Len);
  
  //��佹����
  if(Flag & TEDIT_GRP_MODE){
    if(Flag & TEDIT_EN_NOTIFY) //����ʾʱ
      TWin_SetFocus(pWin,1,0,pEdit->Len - 2);
    else 
      TWin_SetFocus(pWin,0,0,pEdit->Len);
  }
  else{
    TWin_SetFocus(pWin,pEdit->Pos,0,1);//����λ���ǽ���
  }
}

//-----------------����ֵ����ģʽ��ֵ�趨����������----------------
//������ֵ����ģʽʱ,���¼�����ֵ����ʱ���ô˺���
static void _SValueSetKeyPro(struct _TEdit *pEdit,unsigned char Key)
{
  unsigned char MutiPos;//����λ��
  TUnSigned_t Value;      //��ǰ��ֵ
  TUnSigned_t Muti,Vol;  //ָ��λ�ñ����뵱ǰλֵֵ
  if(!_SetSign(pEdit,Key)) return;//���Ŵ�����Чʱֱ�ӷ���
  Value = TGetAbs(pEdit->Value);
  //�õ�������
  MutiPos = TGetNumLen(pEdit->Min,pEdit->Max) - _GetPreValidLen(pEdit,pEdit->Pos);
  Vol = TGetDigitalInPos(Value,MutiPos);  //�õ�ָ��λ��ֵ
  Muti = TMutiTUnigned[MutiPos];
  Value -= Vol * Muti; //ȥ����λ��
  if(Key == TGUI_KEY_UP){
    if(Vol < 9) Vol++;
    else Vol = 0;
  }
  else if(Key == TGUI_KEY_DOWN){
    if(Vol) Vol--;
    else Vol =  9;
  }
  else{//���ּ�
    Vol = Key - '0';
  }
  Value += Vol * Muti; //�µĸ�λ��ֵ

   //�趨����ֵ
  if(pEdit->Value < 0) _SetValue(pEdit,0 - Value);
  else _SetValue(pEdit,Value);
  
}

//-----------------------������ģʽ����������!---------------------------
static void _SKeyPro(struct _TEdit *pEdit,unsigned char Key)
{
  unsigned char Notify = 0xff;    //��Чͨ��ֵ
  char c;

  //������ģʽʱ�ȴ������ּ�
  if(!(pEdit->Flag & TEDIT_GRP_MODE)){
    //������ģʽʱ�������ּ�
    if((Key >= '0') && (Key <= '9')){
      c = TEdit_GetChar(pEdit,pEdit->Pos);
      if ((c != '-') && (c != '+')){
         _SValueSetKeyPro(pEdit,Key);
       Key = TGUI_KEY_RIGHT;//������һ��
      }
      else return;
    }
  }

  switch (Key){
  case '+':
  case '-': _SetSign(pEdit,Key); break;
  case TGUI_KEY_UP:
  case TGUI_KEY_DOWN:
    if(_SetSign(pEdit,Key)) _SValueSetKeyPro(pEdit,Key);
    break;
  case TGUI_KEY_ENTER:
    if(!(pEdit->Flag & TEDIT_ENTER_RIGHT)){//�����˳�
      Notify = TGUI_NOTIFY_KEY_ENTER;
      break;
    }//�������Ϊ�Ҽ�,����...
  case TGUI_KEY_RIGHT:
    c = pEdit->Pos + 1;
    if((c >= pEdit->Len) || (TEdit_GetChar(pEdit,c) == '>')){//�ҳ���ͨ��
      TGUI_NOTIFY_RUN(pEdit->cbGUINotify,pEdit,TEDIT_NOTIFY_RIGHT_END,&pEdit->Value);
      if(Key != TGUI_KEY_ENTER) return;
      Notify = TGUI_NOTIFY_KEY_ENTER; //�����ٴδ���ȷ�ϼ�
      break;
    }
    if(TEdit_GetChar(pEdit,c) == '.') c++;
    pEdit->Pos = c; 
    break;
  case TGUI_KEY_LEFT:
    c = pEdit->Pos;
    if(c <= _GetStartPos(pEdit)){ //����ͨ��
      Notify = TEDIT_NOTIFY_LEFT_END;
      break;   //λ�ò�����ͨ���˳�
    }
    c--;
    //������Чʱ����Ƿ���.��
    if(TEdit_GetChar(pEdit,c) == '.') c--;
    pEdit->Pos = c; 
    break;
  case TGUI_KEY_ESCAPE://�����˳�
    Notify = TGUI_NOTIFY_KEY_ESCAPE;
    break;
  default:
    break;
  }
  TEditDec_UpdateBuf(pEdit);
  if(Notify != 0xff)  //ͨ��
    TGUI_NOTIFY_RUN(pEdit->cbGUINotify,pEdit,Notify,&pEdit->Value);
}

//-----------------------Ⱥ����ģʽ����������---------------------------
static void _GKeyPro(struct _TEdit *pEdit,unsigned char Key)
{
  unsigned char Notify = 0xff;    //��Чͨ��ֵ
  switch (Key){
  case TGUI_KEY_UP:
    if((TSigned_t)pEdit->Value < pEdit->Max) pEdit->Value++;
    break;
  case TGUI_KEY_DOWN:
    if((TSigned_t)pEdit->Value > pEdit->Min) pEdit->Value--;
    break;
  case TGUI_KEY_ENTER:
    if(!(pEdit->Flag & TEDIT_ENTER_RIGHT)){//�����˳�
      Notify = TGUI_NOTIFY_KEY_ENTER;
      break;
    }//�������Ϊ�Ҽ�,����...
  case TGUI_KEY_RIGHT:
    TGUI_NOTIFY_RUN(pEdit->cbGUINotify,pEdit,TEDIT_NOTIFY_RIGHT_END,&pEdit->Value);
    if(Key != TGUI_KEY_ENTER) return;
    Notify = TGUI_NOTIFY_KEY_ENTER; //�����ٴδ���ȷ�ϼ�
    break;
  case TGUI_KEY_LEFT:
    Notify = TEDIT_NOTIFY_LEFT_END;
    break;   //λ�ò�����ͨ���˳�    
  case TGUI_KEY_ESCAPE://�����˳�
    Notify = TGUI_NOTIFY_KEY_ESCAPE;
    break;
  default:
    break;
  }
  TEditDec_UpdateBuf(pEdit);
  if(Notify != 0xff)  //ͨ��
    TGUI_NOTIFY_RUN(pEdit->cbGUINotify,pEdit,Notify,&pEdit->Value);
}

//-------------------ʮ���Ƶ���ʱ����������--------------------
void TEditDec_KeyPro(struct _TEdit *pEdit,unsigned char Key)
{
  if(pEdit->Flag & TEDIT_GRP_MODE) _GKeyPro(pEdit,Key);
  else _SKeyPro(pEdit,Key);
}

//-----------------�༭��ʮ��������ʼ��Ϊ����----------------
void TEditDec(TEdit_t *pEdit,
              TWinHandle_t hWin,    //�����Ĵ���,���ݻ�����>Len
              TSigned_t Value,            //��ʼ��ֵ
              TSigned_t Max,              //���ֵ
              TSigned_t Min,              //��Сֵ
              unsigned char DotPos,  //С����λ��,������������ģ��С����ʾ
              unsigned char Flag,  //�������
              TGUINotify_t cbGUINotify) //ͨ������
{
  unsigned char Len;
  TWin_t *pWin;
  //��ʼ�����
  pEdit->hWin = hWin;
  pEdit->eType = TEditType_Dec;
  pEdit->Flag = Flag;
  pEdit->DotPos = DotPos;
  pEdit->Value = Value; 
  pEdit->Max = Max;
  pEdit->Min = Min;
  pEdit->cbGUINotify = cbGUINotify;  
 
  
  pEdit->Pos = _GetStartPos(pEdit); //�õ���ʼ����λ��
  //�������賤��
  Len =  253 - _GetPreValidLen(pEdit,252);//�����ַ���Ч�����λ����(��>)
  if(Flag & TEDIT_EN_NOTIFY) Len++;//β��>
  Len = TGetAlignLenR(Len,TGetNumLen(Min, Max));//����
  pEdit->Len = Len;
  //�����ô��ڸ߿�
  pWin = TWM_pGetWin(hWin);
  if(TWin_GetBufSize(pWin) < Len) return; //���ڹ�С!!!!
  TWin_ReSize(pWin,Len,1);
  TWin_Disp(pWin);//��Ϊ��ʾ״̬
  TEditDec_UpdateBuf(pEdit);//���»�����
}
