/*******************************************************************************

                   TGUI��������-��ع�����ʵ��

*******************************************************************************/
#include "TGUICfg.h"
#include "TGUIBase.h"
#include <string.h>

//-----------------------���������õ�ռλ����-----------------------
unsigned char TGetItemsPlace(TItemSize_t Items)
{
  if(Items <= 9) return 1;
  if(Items <= 99) return 2;
  //������ʱ
  #if TITEM_SIZE_T >= 2
    if(Items <= 999) return 3;
    if(Items <= 9999) return 4;
    else return 5;
 #else
   else return 3;
  #endif
}

//-----------------------�õ����ַ��Ҷ����ĳ��Ⱥ���-----------------------
unsigned char TGetAlignLenR(unsigned char Len,     //ԭ�ַ�����,<128
                          unsigned char Append) //�����ַ�����,<128
{
  Len += Append;
  #ifdef  TGUI_WORD_ALIGN
    if(Len & 0x01) return Len + 1;
  #endif
  return Len;
}

//-----------------------�õ����ַ�������ĳ��Ⱥ���-----------------------
unsigned char TGetAlignLenL(unsigned char Len,     //ԭ�ַ�����,<128,> 1
                          unsigned char Append) //�����ַ�����,<128
{
  Len += Append;
  #ifdef  TGUI_WORD_ALIGN
    if(Len & 0x01) return Len - 1;
  #endif
  return Len;
}

//-----------------------ת��Ϊ�ַ���-��ʾΪ�����----------------------
unsigned char TValue2StringMin(TSigned_t Value,
                            char *pString,//���ջ���
                            unsigned char Min)//��֤����Сλ��
{
  char *pOrgString = pString;
  
  //������ʱ
  #if(TSIGNED_SIZE == 4)
    if((Value >= 1000000000) || (Min >= 10)){
      *pString++ = Value / 1000000000 + '0';
      Value %= 1000000000;
	  Min = 10;
    }
    if((Value >= 100000000) || (Min >= 9)){
      *pString++ = Value / 100000000 + '0';
      Value %= 100000000;
	  Min = 10;
    }
    if((Value >= 10000000) || (Min >= 8)){
      *pString++ = Value / 10000000 + '0';
      Value %= 10000000;
	  Min = 10;
    }
    if((Value >= 1000000) || (Min >= 7)){
      *pString++ = Value / 1000000 + '0';
      Value %= 1000000;
	  Min = 10;
    } 
    if((Value >= 100000) || (Min >= 6)){
      *pString++ = Value / 100000 + '0';
      Value %= 100000;
	  Min = 10;
    }
  #endif

  //������ʱ
  #if(TSIGNED_SIZE >= 2)
    if((Value >= 10000) || (Min >= 5)){
      *pString++ = Value / 10000 + '0';
      Value %= 10000;
	  Min = 10;
    }
    if((Value >= 1000) || (Min >= 4)){
      *pString++ = Value / 1000 + '0';
      Value %= 1000;
	  Min = 10;
    }
  #endif

  if((Value >= 100) || (Min >= 3)){
    *pString++ = Value / 100 + '0';
    Value %= 100;
	Min = 10;
  }
  if((Value >= 10) || (Min >= 2)){
    *pString++ = Value / 10 + '0';
    Value %= 10;
	Min = 10;
  }
  if((Min >= 1) || (Value > 0)){
    *pString++ = Value + '0';
  }
  return pString - pOrgString;
}

//-----------------------�õ����ַ������ĳ��Ⱥ���-----------------------
//����Ҫ˫�ֽڶ���ʱ,δ����ʱ�ڸ�λ�����' '
//��������Ļ���
char *pTAlignString(TItemSize_t ItemLen,//�ַ�����
                   char *pString)//���ջ���     
{
  #ifdef  TGUI_WORD_ALIGN
    if(ItemLen & 0x01) *pString++ = ' ';
  #endif
  return pString;
}

//------------------�ַ�����չ��������-----------------------
//1:��Ӧ�����뻻���ַ�,
//2:���ؽ�����λ��
char *Tstrcpy_ex(char *s1, const char *s2)
{
  char s = *s2++;
  while((s != '\0') && (s != '\n')){
    *s1++ = s;
    s = *s2++;
  }
  //*s1 = '\0';//�Զ��������ַ�
  return s1;
}

