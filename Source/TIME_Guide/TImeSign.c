/*****************************************************************************
                         TGUI�ϵ����뷨ģ��-�������뷨����
*****************************************************************************/

#include "TImeSign.h"
#include "TImeBase.h"
#include <string.h>


//Ĭ�ϵ�ȫ�ǻ��Ƿ��ű�(��Ǻ��һ�ո�)
static const char _SignTbl[] = {
"\
   . , ? ! : * | ~ ^ ` < > - ( ) @ / _ ; +\
 & % # = [ ] { } $ &��������������������\
������������������������ܡݣ����\
��������"
};

//����һҳ����ʾ�µ�����ַ�����(���Ϊ��λ)
#define _SIGNAL_PAGE_MAX   (2 * 5 * 5)
//����һҳʱ, һҳ����
#define _MUTI_PAGE_MAX   (2 * 5 * 4)

/*****************************************************************************
                        ��غ���
*****************************************************************************/

//---------------------------��ʼ������---------------------------------
//�л����������뷨ʱ����
void TImeSign_Init(struct _TImeSign *pSign,
                   const char *pSignTbl) //�ҽӵķ��������,NULLʹ��Ĭ��
{
  memset(pSign, 0, sizeof(struct _TImeSign));//�ȳ�ʼ����0
  if(pSignTbl == NULL) //NULLʹ��Ĭ��
    pSignTbl = _SignTbl; 
  pSign->pSignTbl = pSignTbl;
  pSign->Len = strlen(pSignTbl);
}

//-------------------------���������û���ʾ-----------------------------------
static const char _chSelGroup[] = {"����: ѡ�����"};
static const char _chSelSign[] = {"����: ѡ�����"};
void TImeSign_FullTitle(const struct _TImeSign *pSign,//���뷨�ṹ
                           char *pStr)
{
   if(pSign->Flag & TIME_SIGN_SEL_SIGN)
     strcpy(pStr, _chSelSign);
   else
     strcpy(pStr, _chSelGroup);
}

//----------------------------�õ���ǰ�ַ�---------------------------------------
//����NULL����������
const char *_pGetCurStr(struct _TImeSign *pSign,//���뷨�ṹ
                        unsigned char GuideKey)//��Ӧ��ֵλ
{
  unsigned char StrPos = pSign->GrpStart;
  if(pSign->Len > _SIGNAL_PAGE_MAX)//һҳ������ʾ��ʱ,����һҳ
      StrPos += (pSign->FirstKey * 4 + TIme_GuideKeyToNoEnterKey[GuideKey]) * 2;
  else StrPos += (pSign->FirstKey * 5 + GuideKey) * 2;//һҳ����ʾ����
  
  if((StrPos + 2) > pSign->Len) return NULL;//����������
  //copy������������
  const char *pStr = pSign->pSignTbl + StrPos;
  return pStr;
}

//-------------------------�õ���Ӧ��ֵ�ַ�����---------------------------------
//�磺�ϼ���Ӧ ����NULL��ʾ����Ҫ�ַ�
static const char _chNextGroup[] = {"��һ��"};
const char *TImeSign_pGetArrowNotice(struct _TImeSign *pSign,//���뷨�ṹ
                                     unsigned char GuideKey)//��Ӧ��ֵλ��
{
  //���������˳���
  if(GuideKey == TIME_MNG_KEY_EXIT) return chReturn;
    
  //========================��������ʱ=========================
  if(pSign->Flag & TIME_SIGN_SEL_SIGN){
    const char *pStr = _pGetCurStr(pSign, GuideKey);
    if(pStr == NULL) return NULL;//����������
    //copy������������
    pSign->SignBuf[0] = *pStr++;
    pSign->SignBuf[1] = *pStr;
    pSign->SignBuf[2] = ' ';//�����ո�������
    pSign->SignBuf[3] = '\0';
    return pSign->SignBuf;
  }
  
  //============================ѡ�����ʱ======================
  unsigned char GroupCount; //һҳ����ʾ�ַ�����,
  if(pSign->Len > _SIGNAL_PAGE_MAX){//һҳ������ʾ��ʱ,����һҳ
    //ѡȷ�ϼ�ʱ��ȷ�ϼ���ʾ��һ��
    if(GuideKey == TIME_MNG_KEY_ENTER) 
      return _chNextGroup;
    GuideKey = TIme_GuideKeyToNoEnterKey[GuideKey]; //ת��Ϊ4��
    GroupCount = 4;
  }
  else GroupCount = 5; //һҳ��ʾ��ʱ��ȷ�ϼ�Ϊ����ѡ��
  
  unsigned char StrPos = pSign->GrpStart + GuideKey * (2 * GroupCount);
  if((StrPos + 2) > pSign->Len) return NULL;//����������(1��Ҳ��һ��)
  unsigned char Len = pSign->Len - StrPos;
  if(Len > 10) Len = 10;
  memcpy(pSign->SignBuf, pSign->pSignTbl + StrPos, Len);
  pSign->SignBuf[Len] = '\0';
  return pSign->SignBuf;
}

//--------------------------������Ӧ����-----------------------------------
//���޸���ɣ��򷵻ض�Ӧ�ַ�������0: ���޸ķ���,  0xffff: �ڲ����������� 
unsigned short TImeSign_Key(struct _TImeSign *pSign,
                              unsigned char GuideKey)//������ֵ0-5
{
  //���������˳���
  if(GuideKey == TIME_MNG_KEY_EXIT){
    //ѡ�����ʱ����
    if(pSign->Flag & TIME_SIGN_SEL_SIGN){
      pSign->Flag &= ~TIME_SIGN_SEL_SIGN; //��������ѡ��
      return 0xffff;
    }
    return 0; //ѡ�����ʱ�� ���޸ķ���
  }
  
  //============================ѡ�����ʱ============================
  if(pSign->Flag & TIME_SIGN_SEL_SIGN){
    const char *pStr = _pGetCurStr(pSign, GuideKey);
    if(pStr == NULL) return 0xffff;//��λ���ַ�,������Ч
    unsigned short Str = *pStr;
    if(Str == ' ') Str = 0; //�ո�ʱֻ�к�벿�֣�Ϊ���
    else Str <<= 8; //ȫ���Ǹ�λ��ǰ
    return Str | *(pStr + 1); //��λ�ں�
  }
  
  //============================ѡ�����ʱ============================
  if(pSign->Len > _SIGNAL_PAGE_MAX){//һҳ������ʾ��ʱ,����һҳ
    //ѡȷ�ϼ�ʱ��ȷ�ϼ��л�����һ��(ĩβ�ػ�)
    if(GuideKey == TIME_MNG_KEY_ENTER){
      pSign->GrpStart += _MUTI_PAGE_MAX;
      if(pSign->GrpStart > pSign->Len)//�ػ�����һҳ��
        pSign->GrpStart = 0;
      return 0xffff;//�ڲ�������
    }
    GuideKey = TIme_GuideKeyToNoEnterKey[GuideKey]; //ת��Ϊ4��
  }
  //else GuideKey����
  
  //��ס����λ�ò��л�������ѡ��ģʽ
  pSign->FirstKey = GuideKey;
  pSign->Flag |= TIME_SIGN_SEL_SIGN; //��������ѡ��
  return 0xffff;//�ڲ�������
}


