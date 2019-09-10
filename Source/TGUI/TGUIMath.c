/*******************************************************************************

                       TGUI��ѧ������غ���ʵ��

*******************************************************************************/
#include "TGUIMath.h"
#include <string.h>
//---------------------------��λ�ò鱶��-----------------------
//��λ�ò鱶��,32λʱ
#if(TSIGNED_SIZE == 4)
  const TUnSigned_t TMutiTUnigned[] = {
    1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
#elif(TSIGNED_SIZE ==2)
  //��λ�ò鱶��,16λʱ
  const TUnSigned_t TMutiTUnigned[] = {
  1,10,100,1000,10000};
#else
  //��λ�ò鱶��,8λʱ
  const TUnSigned_t TMutiTUnigned[] = {
  1,10,100};
#endif

//----------------�õ�һ���з������ľ���ֵ--------------------
TSigned_t TGetAbs(TSigned_t Value)
{
  if(Value < 0) return 0 - Value;
  else return Value;
}

//----------------����ֵ�õ�ռλ����,��������--------------------
unsigned char TGetNumLen(TSigned_t Min,TSigned_t Max)   //Сֵ���ֵ
{
  TUnSigned_t Data;
  unsigned char Len;

  Min = TGetAbs(Min);
  Max = TGetAbs(Max);
  if(Min > Max) Max = Min;
  Data = Max;
 
  for(Len = 0; Data; Len++) Data /= 10;
  if(Len > 7)  Len = 7;
  return Len;
}

//----------------�õ�ָ��λ����ֵ����------------------------
TUnSigned_t TGetDigitalInPos(TUnSigned_t Value,
                           unsigned char Pos)
{
  while(Pos--){
    Value /= 10;
  }
  return Value % 10;
}

//-----------------����־��ת��Ϊ�ַ�������------------------
//����������ĩδ�û�����λ��
char *pTNum2StringFlag(TSigned_t Value,   //��ǰ��ֵ
                      char *pBuf,    //���ջ�����
                      unsigned char Len,    //��ʾ����
                      //��־,����Ϊ����3bit:С����λ��,0x80:��ʾ������
                      unsigned char Flag)
{
  unsigned char FullLen;

  if(Flag & 0x80){//������ʱ
    if(Value < 0) *pBuf = '-';
    else *pBuf = '+';
    pBuf++;
  }
  if(Value < 0) Value = 0 - Value;  //��ֵ��ʾ
      
  //��С����ʱ,�ȼ����ֵ�Ƿ�<1,����,����ǰ�油�˶��ٸ�0.��0.0
  Flag &= 0x07;
  if(Flag >= Len){ //<0��,0.
    FullLen = Flag - Len;
    *pBuf++ = '0';
    *pBuf++ = '.'; 
    while(FullLen--) *pBuf++ = '0';//�����Ч��0
    pBuf += TValue2StringMin(Value,pBuf,Len);
  }
  else{
    //>1��,���С����λ��,��ǰλ����С���������ұ�
    pBuf += TValue2StringMin(Value,pBuf,Len);//���������
    if(Flag){ //��С����ʱ���С����λ��
      FullLen = Flag;//�ݴ�
      for(; Flag > 0; Flag--) {*pBuf = *(pBuf - 1); pBuf--;}
      *pBuf= '.';
      pBuf += FullLen + 1;//��С����
    }
  }
  return pBuf;
}
