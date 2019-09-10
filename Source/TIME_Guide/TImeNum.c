/*****************************************************************************
                         TGUI�ϵ����뷨ģ��-���ּ����뷨����
*****************************************************************************/
#include "TImeNum.h"
#include "TImeBase.h"
#include <string.h>
/*****************************************************************************
                              ˵��
*****************************************************************************/
//�˺����ڲ�������κ���ʾ
//������������˵��:
//1:�л����������뷨ʱ�����ó�ʼ���������г�ʼ������
//2:�û������ּ�ʱ�����ð�����������Ӧ����
//3:�õ��ַ���: ���õõ��ַ��������õ���ʾ�������ַ���




/*****************************************************************************
                                ��غ���ʵ��
*****************************************************************************/

//---------------------------��ʼ������---------------------------------
//�л����������뷨ʱ����
void TImeNum_Init(struct _TImeNum *pNum)
{
  memset(pNum, 0, sizeof(struct _TImeNum));
}

//-------------------------���������û���ʾ-----------------------------------

static const char _chSelNum[] = {"0-9: (���Ӱ�����Ϊ�ڶ���)"};
void TImeNum_FullTitle(const struct _TImeNum *pNum,//���뷨�ṹ
                           char *pStr)
{
  strcpy(pStr, _chSelNum);
}

//-----------------------�������뷨����ַ���Դ----------------------------
static const char _chNum01[] =  {"01"};
static const char _chNum23[] =  {"23"};
static const char _chNum45[] =  {"45"};
static const char _chNum67[] =  {"67"};
static const char _chNum89[] =  {"89"};
static const char *const _NumArrow[] = {
  _chNum01, // TIME_MNG_KEY_UP 
  _chNum23, //TIME_MNG_KEY_LEFT
  _chNum45, //TIME_MNG_KEY_ENTER
  _chNum67, //TIME_MNG_KEY_RIGHT
  _chNum89, //TIME_MNG_KEY_DOWN
  chReturn, //TIME_MNG_KEY_EXIT,
};

//-------------------------�õ���Ӧ��ֵ�ַ�����-----------------------------------
//�磺�ϼ���Ӧ ����NULL��ʾ����Ҫ�ַ�
const char *TImeNum_pGetArrowNotice(struct _TImeNum *pNum,//���뷨�ṹ
                                         unsigned char GuideKey)//��Ӧ��ֵλ��
{ 
  return  _NumArrow[GuideKey];
}

//--------------------------������Ӧ����-----------------------------------
//���޸���ɣ��򷵻ض�Ӧ�ַ�������0: ���޸ķ���,  0xffff: �ڲ����������� 
unsigned short TImeNum_Key(struct _TImeNum *pNum, unsigned char GuideKey)
{
  if(GuideKey == TIME_MNG_KEY_EXIT)//���������˳���
    return 0;
  
  if(pNum->NumTimer){//�ڶ���ѡ��ʱ
    char Chr = '0' + (pNum->FirstKey * 2);
    if(GuideKey == pNum->FirstKey)//��ͬ������
      Chr++;//Ϊ��һ��
    //else //�ڶ��ΰ��������Ϊ���׸�
    return (unsigned short)Chr;
  }
  //��һ��ʱ
  pNum->NumTimer = TIME_NUM_TIMER_OV;
  pNum->FirstKey = GuideKey;
  return 0xffff;
}

//-----------------------------������----------------------------------
//128ms����һ��, ���޸���ɣ����ض�Ӧ������ɵ����֣�����0������
unsigned short TImeNum_Task(struct _TImeNum *pNum)
{
  if(!pNum->NumTimer) return 0;
  pNum->NumTimer--;
  if(!pNum->NumTimer){//ʱ�䵽�Զ�ȷ��
    return (unsigned short)('0' + (pNum->FirstKey * 2));
  }
  return 0;
}



