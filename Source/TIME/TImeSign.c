/*****************************************************************************
                         TGUI�ϵ����뷨ģ��-�������뷨����
*****************************************************************************/

#include "TImeSign.h"
#include <string.h>


//Ĭ�ϵ�ȫ�ǻ��Ƿ��ű�(��Ǻ��һ�ո�)
static const char _SignTbl[] = {
"\
   . , ? ! : * | ~ ^ ` < > - ( ) @ / _ ; +\
 & % # = [ ] { } $ &��������������������\
������������������������ܡݣ����\
��������"
};

/*****************************************************************************
                        ��غ���
*****************************************************************************/

//---------------------------��ʼ������---------------------------------
//�л����������뷨ʱ����
void TImeSign_Init(struct _TImeSign *pSign,//���뷨�ṹ
                   //�ҽӵķ��������,ȫ�ֶ��룬ΪNULLʱ��Ĭ�Ϸ��ű�
                   const char *pSignTbl,
                   unsigned char DispW,        //��ʾ���,>16
                   unsigned char DispH)        //��ʾ�߶�>=3
{
  memset(pSign, 0, sizeof(struct _TImeSign));//�ȳ�ʼ����0
  if(pSignTbl == NULL){//ʹ��Ĭ�Ϸ��ű�
    pSign->pSignTbl = _SignTbl;
    pSign->Len = sizeof(_SignTbl);
  }
  else{
    pSign->pSignTbl = pSignTbl;
    pSign->Len = strlen(pSignTbl);
  }
  //���ߣ�����ȹ�ʱ��ǰ���Զ������к�
  if((DispH > 3) && (DispW > 16)){
    pSign->EnDispLine = 1;
    DispW -= 4; //ռ����ʾ������
  }
  DispW >>= 2; //һ��ȫ���ַ�;
  if(DispW > 9) DispW = 9; //һҳ���������ʾ9���Զ�Ӧ���ּ�
  pSign->w = DispW;
  if(DispH > 9) DispH = 9; //һҳ���������ʾ9���Զ�Ӧ���ּ�  
  pSign->h = DispH;      //����ʾ�߶���ͬ
}


//----------------------�������ʾ�ַ�������---------------------------
//�����������
unsigned char TImeSign_GetDispChar(struct _TImeSign *pSign,
                                    char *pBuf,              //�������ַ�
                                    unsigned char VNum)     //�к�0-8
{
  if(VNum > 8) VNum = 8;//�쳣
  const char *pPos, *pEndPos;
  unsigned short Pos;
  Pos = VNum * pSign->w * 2;//ת��Ϊ���
  Pos += pSign->PageStart;
  if(Pos >= pSign->Len) return 0; //����
  
  pPos = pSign->pSignTbl + Pos; 
  pEndPos = pSign->pSignTbl + pSign->Len;
  
  char *pStartBuf = pBuf;
  if(pSign->EnDispLine){//�����к�
    *pBuf++ = '[';
    *pBuf++ = '1' + VNum;
    *pBuf++ = ']';
    *pBuf++ = ' ';
  } 
  //��䱾���ַ�
  for(unsigned char Num = 0; Num < pSign->w; Num++){
    if(pPos < pEndPos){//�ַ�δ����ʱ
      *pBuf++ = *pPos++;    //��һ���ַ��ո�����ǰ���
      *pBuf++ = *pPos++;    //��һ���ַ������
      *pBuf++ = Num + '1';  //ָʾ��λ��
      *pBuf++ = ' ';        //���ַ������ո�
    }
    else break;
  }//end for
  return pBuf - pStartBuf;
}

//----------------------�ж�һҳ�Ƿ�����ʾ�º���---------------------------
signed char TImeSign_IsOnePage(const struct _TImeSign *pSign)
{
  unsigned short Pages = pSign->w * pSign->h * 2;//ÿҳ�ַ�����*2
  if(pSign->Len <= Pages) return 1;//����һҳ��
  return 0;
}
         
//----------------------�õ���ǰ�û�ѡ���ַ�����---------------------------
//���ص�ǰ�û�ѡ����ַ�,ǰ��λ��Ч
//��ȡ�ַ���ǿ�Ʒ��ؿ���״̬
//ע������û�������ϲ������eTImeSign_Final���ܻ���ַ���
const char *TImeSign_pGetChar(struct _TImeSign *pSign)
{
  unsigned short Pos;
  
  Pos = pSign->PosV * pSign->w + pSign->PosH;
  Pos <<= 1; //ת��Ϊ���
  Pos += pSign->PageStart;//��ʼҳ
  pSign->eState = eTImeSign_Idie;
  if(Pos >= pSign->Len) //����ʱѡ�����һ��
    return pSign->pSignTbl + pSign->Len - 1;
  
  return pSign->pSignTbl + Pos;
}

//-------------------------�������Ӧ����---------------------------------
//�������������ѡ��ĳ��ʱȡ����ѡ��
//�û���������˳���ʱ���ã������Ƿ�����Ӧ�������
signed char TImeSign_KeyClr(struct _TImeSign *pSign)
{
  if(pSign->eState != eTImeSign_SelV) return -1;//û��Ӧ����
  pSign->eState = eTImeSign_Idie;
  return 0;  //����Ӧ
}

//-----------------------���ּ���Ӧ����---------------------------------
//���û������ּ�1-9ʱ��Ӧ�û�����
//����1:��Ӧ�������˳�; 0:��Ӧ����δ�˳�
//��ֵ����Ӧ����ֵ��Ч
signed char TImeSign_KeyNum(struct _TImeSign *pSign,
                            char KeyNum)//��ֵ��Ӧȷ��Ϊ'1'-'9'
{
  enum _eTImeSign eState = pSign->eState;
  
  KeyNum -= '1';
  switch(eState){
  case eTImeSign_Idie://���У��������ѡ
    if(KeyNum >= pSign->h) return -2;//������ǰ�з�Χѡ����
    pSign->PosV = KeyNum;
    eState = eTImeSign_SelV;  //�û�ѡ�������
    break; 
  case eTImeSign_SelV://�ѽ�������ѡ����ѡѡ�����ĳ����
    if(KeyNum >= pSign->w) return -3;//������ǰ�з�Χѡ����
    pSign->PosH = KeyNum;
    eState = eTImeSign_Final;  //�û�ѡ�������
    return 1; //��Ӧ�������˳�
  //case eTImeSign_Final: 
  //  return -1//�û�ѡ�����,�ٴΰ�����Ч���ϲ㴦�����!
  default:return -1;
  }
  pSign->eState = eState;
  return 0;  //��Ӧ����δ�˳�
}

//-----------------------��ҳ����Ӧ����---------------------------------
//��Ӧ���·�ҳ����
//�����Ƿ�����Ӧ���û�����
signed char TImeSign_KeyPage(struct _TImeSign *pSign,
                             signed char DownFlag)//�¼���־������Ϊ�ϼ�
{
  unsigned short Pages = pSign->w * pSign->h * 2;//ÿҳ�ַ�����*2
  pSign->eState = eTImeSign_Idie; //�κ�ʱ�򰴷�ҳ���������¿�ʼѡ��
  
  if(DownFlag){//��һҳ
    if(pSign->Len <= Pages) return -1;//����һҳ��
    if(pSign->PageStart < (pSign->Len - Pages)){//ֻҪû�����һҳ�������·�
        pSign->PageStart += Pages;
    } 
  }
  else{//��һҳ
    if(pSign->PageStart){//ֻҪû����һҳ�������Ϸ�
      if(pSign->PageStart > Pages)//������һҳʱ
        pSign->PageStart -= Pages;
      else
        pSign->PageStart = 0;//���ڵ�һҳ��
    } 
  }
  return -1;
}

