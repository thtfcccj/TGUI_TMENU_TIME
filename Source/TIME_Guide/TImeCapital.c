/*****************************************************************************
                         TGUI�ϵ����뷨ģ��-�������뷨����
*****************************************************************************/

#include "TImeCapital.h"
#include "TImeBase.h"
#include <string.h>

/*****************************************************************************
                                ��غ���ʵ��
*****************************************************************************/

//---------------------------��ʼ������---------------------------------
//�л����������뷨ʱ����
void TImeCapital_Init(struct _TImeCapital *pCapital,//���뷨�ṹ
                      unsigned char CapitalFlag)   //��д����ģʽ��־
{
  memset(pCapital, 0, sizeof(struct _TImeCapital));
  if(CapitalFlag) pCapital->Flag |= TIME_CAPITAL_CAPITAL; //��д״̬
}

//-------------------------���������û���ʾ-----------------------------------
static const char _chSelGroup[] = {"ѡ�����"};
static const char _chSelChar[] =  {"ѡ����ĸ"};
static const char _chNotice[] =  {"���١�����)"};

void TImeCapital_FullTitle(const struct _TImeCapital *pCapital,//���뷨�ṹ
                           char *pStr)
{
  //���:"A: "��"a: "
  if(pCapital->Flag & TIME_CAPITAL_CAPITAL) *pStr = 'A';
  else *pStr = 'a';
  pStr++;
  *pStr++ = ':';
  *pStr++ = ' ';
  //ѡ����ĸ
  if(pCapital->Flag & TIME_CAPITAL_SEL_CHAR){
    strcpy(pStr, _chSelChar);
    if(pCapital->FirstKey == TIME_MNG_KEY_DOWN){//���һ��ʱ��������ʾz:���١�����
      pStr += strlen(_chSelChar);
      *pStr++ = ' ';
      *pStr++ = '(';
      if(pCapital->Flag & TIME_CAPITAL_CAPITAL) *pStr = 'Z';
      else *pStr = 'z';
      pStr++;
      strcpy(pStr, _chNotice);  
    }
  }
  else//ѡ�����
    strcpy(pStr, _chSelGroup);
}

//-------------------------����ʱ��GuideKey��Ӧ�ַ�---------------------------
static const char _chCapitalUp[] =    {"ABCDE"};
static const char _chCapitalLeft[] =  {"FGHIJ"};
static const char _chCapitalSave[] =  {"KLMNO"};
static const char _chCapitalRight[] = {"PQRST"};
static const char _chCapitalDown[] =  {"UVWXYZ"};
static const char _chLowercaseUp[] =    {"abcde"};
static const char _chLowercaseLeft[] =  {"fghij"};
static const char _chLowercaseSave[] =  {"klmno"};
static const char _chLowercaseRight[] = {"pqrst"};
static const char _chLowercaseDown[] =  {"uvwxyz"};
static const char *const _CapitalArrow[] = {//��д
  _chCapitalUp, _chCapitalLeft,
  _chCapitalSave, _chCapitalRight,
  _chCapitalDown
};
static const char *const _LowercaseArrow[] = {//Сд
  _chLowercaseUp, _chLowercaseLeft,
  _chLowercaseSave, _chLowercaseRight,
  _chLowercaseDown
};

//-------------------------�õ���Ӧ��ֵ�ַ�����-----------------------------------
//�磺�ϼ���Ӧ ����NULL��ʾ����Ҫ�ַ�
const char *TImeCapital_pGetArrowNotice(struct _TImeCapital *pCapital,//���뷨�ṹ
                                         unsigned char GuideKey)//��Ӧ��ֵλ��
{
  //���������˳���
  if(GuideKey == TIME_MNG_KEY_EXIT) return chReturn;
  
  //ѡ����ĸʱ,���ػ�����
  if(pCapital->Flag & TIME_CAPITAL_SEL_CHAR){
    char Char = 'A' + (pCapital->FirstKey * 5 + GuideKey);
    char *pStr = pCapital->CharBuf;
    if(!(pCapital->Flag & TIME_CAPITAL_CAPITAL))//Сдʱ
      *pStr = Char + 32;
    else *pStr = Char;
    pStr++;
    //Yʱ������Z
    if(Char == 'Y'){
      if(!(pCapital->Flag & TIME_CAPITAL_CAPITAL))//Сдʱ
        *pStr = 'z';
      else *pStr = 'Z';
      pStr++;
    }
    *pStr = '\0';//�ַ��������ַ�
    return pCapital->CharBuf;
  }
  else{//ѡ�����ʱ
    if(!(pCapital->Flag & TIME_CAPITAL_CAPITAL))//Сдʱ
      return _LowercaseArrow[GuideKey];
    return _CapitalArrow[GuideKey];
  }
}

//--------------------------������Ӧ����-----------------------------------
//���޸���ɣ��򷵻ض�Ӧ�ַ�������0: ���޸ķ���,  0xffff: �ڲ����������� 
unsigned short TImeCapital_Key(struct _TImeCapital *pCapital,
                                 unsigned char GuideKey)//������ֵ0-5
{
  //���������˳���
  if(GuideKey == TIME_MNG_KEY_EXIT){
    //ѡ����ĸʱ����
    if(pCapital->Flag & TIME_CAPITAL_SEL_CHAR){
      pCapital->Flag &= ~TIME_CAPITAL_SEL_CHAR; //��������ѡ��
      return 0xffff;
    }
    return 0; //ѡ�����ʱ�� ���޸ķ���
  }
  
  //ѡ����ĸʱ����
  if(pCapital->Flag & TIME_CAPITAL_SEL_CHAR){
    if(pCapital->yzTimer){//yzѡ��ģʽʱ
      if(GuideKey == TIME_MNG_KEY_DOWN){//�ڶ���Ϊz
        if(pCapital->Flag & TIME_CAPITAL_CAPITAL)
          return (unsigned short)'Z';
        return (unsigned short)'z';
      }
      else{//������Ĭ��Ϊy
        if(pCapital->Flag & TIME_CAPITAL_CAPITAL)
          return (unsigned short)'Y';
        return (unsigned short)'y';        
      }
    }
    //��һ�ΰ���ʱ
    char Char = 'A' + (pCapital->FirstKey * 5 + GuideKey);
    if(Char == 'Y'){//��Ҫ�ڶ���ѡ��
      pCapital->yzTimer = TIME_CAPITAL_YZ_TIMER_OV;
      return 0xffff;
    }
    if(!(pCapital->Flag & TIME_CAPITAL_CAPITAL))//Сдʱ
      Char += 32;
    return (unsigned short)Char; //���ؽ��
  }
  else{//����ѡ��ʱ��ס��ֵ���л����ַ�ѡ��ģʽ
    pCapital->FirstKey = GuideKey;
    pCapital->Flag |= TIME_CAPITAL_SEL_CHAR;
    return 0xffff;
  }
}

//-----------------------------������----------------------------------
//128ms����һ��, ���޸���ɣ����ض�Ӧ������ɵ��ַ�������0������
unsigned short TImeCapital_Task(struct _TImeCapital *pCapital)
{
  //yzѡ��ģʽʱ�Զ�ȷ��Y��y
  if(!pCapital->yzTimer) return 0;
  pCapital->yzTimer--;
  if(!pCapital->yzTimer){//ʱ�䵽�Զ�ȷ��ΪY
    if(pCapital->Flag & TIME_CAPITAL_CAPITAL)
      return (unsigned short)'Y';
    return (unsigned short)'y';
  }
  return 0;
}


