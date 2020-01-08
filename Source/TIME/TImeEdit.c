/*****************************************************************************
                         TGUI�ϵ����뷨ģ�����б༭��ģ��
ע:�ݲ�֧���Դ��ֿ�ȫ�Ƕ���LCD��ʾ������ʾ
*****************************************************************************/

#include "TImeEdit.h"
#include <string.h>

/*****************************************************************************
                        ��س�Ա����
*****************************************************************************/

//---------------�жϵ�ǰλ���Ƿ�ȫ�Ƕ��뺯��-------------------
static signed char _IsAlign(const char *pString,    //�ַ���
                            TIme_String_t Cursor)//���λ��>=1;
{
  TIme_String_t Align;
  Cursor -= 1;//���֮ǰ���ַ�
  if(*(pString + Cursor) < 0x80) return 0;//������
  //ͳ�ƹ��֮ǰȫ���ַ�ռλ�������Ϊż������룬����ǰλ��δ����
  for(Align = 1, Cursor -= 1; Cursor > 0; Cursor--){
    if(*(pString + Cursor) >= 0x80) Align++;
    else break;//�ҵ�����ַ���
  }
  if(Align & 0x01) return -1; //δ����
  return 0;
}


//-----------------------��ʼ������-------------------------------
//�������뷨ģʽʱ����
void TImeEdit_Init(struct _TImeEdit *pEdit,
                  char *pString,       //���޸ĵ��ַ���
                  TIme_String_t Size,  //�ַ��������������С+1
                  TIme_String_t w)     //�༭����
{
  memset(pEdit, 0, sizeof(struct _TImeEdit));
  
  TIme_String_t Len;
  TIme_String_t DispStart;
  pEdit->pString = pString;
  pEdit->Size = Size;
  pEdit->w = w;
  Len = strlen(pString);
  if(Len >= Size){//�ַ������������ַ���ǿ������
    *(pString + Size - 1) = '\0';
    Len = Size - 1;
  }
  pEdit->Len = Len;
  pEdit->Cursor = Len;//Ĭ�ϴ����λ�ÿ�ʼ�༭ 
  //���ָ�����һ���༭���ȴ���,ע����ռһλ
  if(Len < w) DispStart = 0;
  else{
    DispStart = Len - w;
    //��ǰ���δȫ�Ƕ���ʱ����꿿��һλ��ʾ��ʹ��һ��ȫ���ַ�����������
    if(_IsAlign(pString,DispStart)) DispStart++;
  }
  pEdit->DispStart = DispStart;
}


//--------------------�õ���Թ��λ�ú���---------------------------
//���ڵ�ǰ��ʾ��ʼ��ʼ�Ĺ��λ�ã������ϲ㽹��������
TIme_String_t TImeEdit_GetCursor(const struct _TImeEdit *pEdit)
{
  return pEdit->Cursor - pEdit->DispStart;
}

//---------------------------������ƺ���---------------------------
//�����ڱ༭����ʱ����
//�������Ƹ���
unsigned char TImeEdit_CursorLeft(struct _TImeEdit *pEdit)
{
  unsigned char Left;
  TIme_String_t Cursor = pEdit->Cursor;
  
  if(!Cursor) return 0;//������߽���
  //ȷ�����ƶ��λ
  Cursor--;
  if(*(pEdit->pString + Cursor) >= 0x80) Left = 2;
  else Left = 1;
  if(pEdit->Cursor <= Left)
    Left = pEdit->Cursor; //�Ƶ���߽���

  pEdit->Cursor -= Left;  
  //������ʾ��,���һ���ƶ�
  if(Cursor < pEdit->DispStart) pEdit->DispStart = Cursor;
  return Left;
}
        
//---------------------------������ƺ���---------------------------
//�����ڱ༭����ʱ����
//�������Ƹ���
unsigned char TImeEdit_CursorRight(struct _TImeEdit *pEdit)
{
  unsigned char Right;
  TIme_String_t Cursor = pEdit->Cursor;
  
  if(Cursor >= pEdit->Len) return 0;//�����ұ߽���
  //ȷ�����ƶ��λ
  if(*(pEdit->pString + Cursor) >= 0x80) Right = 2;
  else Right = 1;
  Cursor++;
  if((pEdit->Cursor + Right) >= pEdit->Len)//�Ƶ��ұ߽���
    Right = pEdit->Len - pEdit->Cursor;
  
  pEdit->Cursor += Right;
  //������ʾ��,���һ���ƶ�
  if((Cursor - pEdit->DispStart) >= pEdit->w)
     pEdit->DispStart += Right;
  return Right;
}

//------------------------ɾ��һ���ַ�����---------------------------
//ɾ���������һ���ַ�,�����ڱ༭����ʱ����
//����ɾ������
unsigned char TImeEdit_Clr(struct _TImeEdit *pEdit)
{
  unsigned char Left;
  TIme_String_t Cursor = pEdit->Cursor;
  
  if(!Cursor) return 0;//������߽���,����ɾ��
  //ȷ�����ƶ�λ,��ɾ������
  if(*(pEdit->pString + (Cursor - 1)) >= 0x80) Left = 2;
  else Left = 1;
  if(Cursor <= Left) Left = Cursor; //�Ƶ���߽���
    
  //���Ҳ��ַ�������༴���ɾ��
  memcpy(pEdit->pString + Cursor - Left,
         pEdit->pString + Cursor,
         pEdit->Len - Cursor + 1);//�������ַ�'\0'һ����
  
  pEdit->Len -= Left;  //���ȼ�С��
  pEdit->Cursor -= Left;//���ͬʱ�ƶ�
  return Left;
}

//------------------�ж����������ַ���������---------------------------
//�ڵ�ǰ���λ������һ���ֻ��ַ�,�����ڱ༭����ʱ����
//���ؿ������ַ�����
unsigned char TImeEdit_EnAdd(const struct _TImeEdit *pEdit)
{
  return pEdit->Size - pEdit->Len - 1;//'\0'Ԥ��һλ 
}

//-------------------���λ������һ���ַ�����---------------------------
//�����ڱ༭����ʱ����
void TImeEdit_Add(struct _TImeEdit *pEdit,
                  unsigned short String)//���ӵ�ȫ�ֻ����
{
  unsigned char CurSize;
  TIme_String_t Len;
  TIme_String_t Cursor;  
  
  if(String > 0x80) CurSize = 2;//ȫ��
  else CurSize = 1;
  if(TImeEdit_EnAdd(pEdit) < CurSize) return;//�����������ַ���,����

  //�����λ���Ժ���ַ�һ���������ƶ�CurSize����С
  Cursor = pEdit->Len;  //��'\0'һ��
  Len = Cursor - pEdit->Cursor + CurSize;
  for(; Len > 0; Len--){
    *(pEdit->pString + Cursor + CurSize) = 
      *(pEdit->pString + Cursor);
    if(Cursor) Cursor--;
    //���ַ����ƶ���(ȫ��ʱ��Lel�ͺ�Cursorһ�����ʲ�����Len�˳�)
    else break; 
  }
  //���λ�������ַ�
  if(CurSize == 2){
    *(pEdit->pString + pEdit->Cursor) = (char)(String >> 8);
    *(pEdit->pString + pEdit->Cursor + 1) = (char)(String);
  }
  else *(pEdit->pString + pEdit->Cursor) = (char)(String);
  
  pEdit->Len += CurSize;//�ַ�������������
  pEdit->Cursor += CurSize;//���ͬʱ�ƶ�
  //������ʾ�ˣ���ʾ��һ���ƶ�
  if((pEdit->Cursor - pEdit->DispStart) >= pEdit->w)
     pEdit->DispStart += CurSize;
}

//-------------------------�����ַ�����-----------------------------
//������ʾ��ʼλ�õ���ȷ�Χ�ڵ��ַ�������Ĳ���ո�
void TImeEdit_FullBuf(const struct _TImeEdit *pEdit,
                      char *pBuf)    //�����Ļ�����
{
  //�����ǰ�ַ�
  TIme_String_t w;
  TIme_String_t Data;  
  
  w = pEdit->Cursor - pEdit->DispStart;
  memcpy(pBuf, pEdit->pString + pEdit->DispStart, w);
  if(*pBuf >= 0x80){//��һ��Ϊȫ���ַ�ʱ������Ƿ񱻲����
    for(Data = 1; Data < w; Data++){
      if(*(pBuf + Data) < 0x80) break;
    }
    if(Data & 0x01) *pBuf = ' ';//�����ʱ���ո�
  }
  pBuf += w;
  *pBuf++= ' ';//���λ�����ո�
  w = pEdit->w - (w + 1);//Ϊ����λ�����¿��
  
  //�������ַ�
  if(pEdit->Len > pEdit->Cursor){//�������ַ�
    if((pEdit->Len - pEdit->Cursor) < w) //����Ϊ�����ַ��Ŀ��
      Data = pEdit->Len - pEdit->Cursor;
    else Data = w;
    memcpy(pBuf, pEdit->pString + pEdit->Cursor, Data);
    if(_IsAlign(pBuf,w)) Data--;//����ȫ�ǽ�ֹ��Ϊ������ʾ�����������Ϊ��
    pBuf += Data;
    w -= Data;
  }
}

