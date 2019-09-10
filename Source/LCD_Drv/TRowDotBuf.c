/*******************************************************************************

                �����ֿ�ʱ��һ�У�����*16�����󻺳壭ʵ��
��ģ�������ڲ����ֿ��LCD��ʾ��
*******************************************************************************/

#include "TRowDotBuf.h"
#include "TGUIBase.h"
#include <string.h>

//���󻺳���,�Ӱ�������ֹȫ�ǰ���ַ��ڴ����
static unsigned char _RowBuf[(TLCD_WIDTH + 1) * 2 * 8];
static unsigned char _ModuleBuf[32];//��ģ������


//-----------------------����������������--------------------------
#define _ErrCheck(pBuf, Count)  do{}while(0)
/*static unsigned char _ErrCount = 0;//���������
//�������ȫ��0xffʱ�����ֿ����
static void _ErrCheck(const unsigned char *pBuf,//���ⲿ�������ģ
                      unsigned char Count)//��ģ����
{
  while(Count){
    if(*pBuf != 0xff){
      _ErrCount = 0;
      return;
    }
    *pBuf++;
    Count--;
  }
  //�����ˣ�
  _ErrCount++;
  if(_ErrCount == 0xff) while(1);//�ֿ�����ˣ��ȴ����Ź���λ
}*/

/****************************************************************************
                   ȡģ��ʽ1(����ȡģ,�ֽ�����)֧��
****************************************************************************/
#if T_ROW_DOT_BUF_MODULE == 0 //����ȡģ,�ֽ�����

//-----------------------�����ģ��־����--------------------------
static void _FillModuleFlag(unsigned char NorFlag,  //��ͨ���±�־ʱ��������־
                             unsigned char ModuleLen)//ȡģ����,ȫ��Ϊ2,���Ϊ1
{
  unsigned char LenCount;
  unsigned char Len;

  if(NorFlag & DISP_CFG_ANTI){//����ʱ
    LenCount = ModuleLen * 16;
    for(Len = 0; Len < LenCount; Len++)
      _ModuleBuf[Len] = ~_ModuleBuf[Len];
  }
  if(NorFlag & DISP_CFG_UP_LINE){//�Ϻ��߱�־֧��
    _ModuleBuf[0] = 0xff;
    if(ModuleLen == 2) _ModuleBuf[1] = 0xff;
  }
  if(NorFlag & DISP_CFG_DOWN_LINE){//�º��߱�־֧��
    if(ModuleLen == 2){
      _ModuleBuf[30] = 0xff;
      _ModuleBuf[31] = 0xff;
    }
    else{
      _ModuleBuf[15] = 0xff;
    }
  }
}


//-----------------------����ģ���뻺�庯��--------------------------
static void _FillBuf(unsigned char Pos,//ָ��λ��
                      unsigned char w,  //���³���
                      unsigned char ModuleLen)//ȡģ����,ȫ��Ϊ2,���Ϊ1
{
  unsigned char i;
  unsigned short CurPos;

  //��ģʽ��LCD������ʽΪ:����˳���ͳ�,���谴�÷�ʽ������֯:
  for(i = 0; i < 16; i++){
    CurPos = w * i + Pos;
    _RowBuf[CurPos] = _ModuleBuf[i * ModuleLen];
    if(ModuleLen == 2)
      _RowBuf[CurPos + 1] = _ModuleBuf[i * ModuleLen + 1];
  }
}

/****************************************************************************
                   ȡģ��ʽ4(����ȡģ,�ֽڵ���)֧��
****************************************************************************/
#elif T_ROW_DOT_BUF_MODULE == 3 //����ȡģ,�ֽ�����

//-----------------------�����ģ��־����--------------------------
static void _FillModuleFlag(unsigned char NorFlag,  //��ͨ���±�־ʱ��������־
                             unsigned char ModuleLen)//ȡģ����,ȫ��Ϊ2,���Ϊ1
{
  unsigned char LenCount;
  unsigned char Len;

  LenCount = ModuleLen * 16;
  if(NorFlag & DISP_CFG_ANTI){//����ʱ
    for(Len = 0; Len < LenCount; Len++)
      _ModuleBuf[Len] = ~_ModuleBuf[Len];
  }
  if(NorFlag & DISP_CFG_UP_LINE){//�Ϻ��߱�־֧��
    for(Len = 0; Len < LenCount; Len += 2)//ż��D0���
      _ModuleBuf[Len] |= 0x01;
  }
  if(NorFlag & DISP_CFG_DOWN_LINE){//�º��߱�־֧��
    for(Len = 1; Len < LenCount; Len += 2)//����D8���
      _ModuleBuf[Len] |= 0x80;
  }
}

//-----------------------����ģ���뻺�庯��--------------------------
static void _FillBuf(unsigned char Pos,//ָ��λ��
                      unsigned char w,  //���³���
                      unsigned char ModuleLen)//ȡģ����,ȫ��Ϊ2,���Ϊ1
{
  //��ģʽ��LCD������ʽΪ:�������ͳ�,ÿ��������->������->������->������
  memcpy(_RowBuf + (Pos << 4), _ModuleBuf, ModuleLen << 4);
}
/****************************************************************************
                   ����ȡģ��ʽ�ݲ�֧��
****************************************************************************/
#else
    //�ٺ���ʵ��,���뱨��!!!!
#endif

/****************************************************************************
                   �û��ӿں���
****************************************************************************/

//-----------------------�õ�һ���ַ����󻺳�--------------------------
//��������ʽ��һ��ȫ����16*16Ϊһ�������
const unsigned char *TRowDotBuf_pGetBuf(unsigned char w,      //���³���
                                        const char *pBuf,     //������ַ�
                                        //ÿ���ַ���Ӧλ�ñ�־,��TWinMng_cbUpdateRow����
                                        const unsigned char *pFlagBuf)
{
  unsigned char CurFlag;
  unsigned char Code;


  for(unsigned char Pos = 0; Pos < w; Pos++){
    CurFlag = *pFlagBuf++;
    Code = *pBuf++;
    //�õ���ģ
    switch(CurFlag & DISP_CMD_MASK){
    case DISP_CMD_NOR://�������ö���
      //��ȡ����ģ������ģ���뻺����
      if(Code >= 0x80){
        if((Pos + 1) >= w) break;//���ȫ���ַ�ȥ��
        TRowDotBuf_cbGetWordModule(_ModuleBuf,(unsigned short)Code << 8 | *pBuf);
        _ErrCheck(_ModuleBuf, 32);
        pBuf++; pFlagBuf++; //�¸��ַ�
        _FillModuleFlag(CurFlag,2);//���־
        _FillBuf(Pos,w,2);//����л���
        Pos++;//
      }
      else{
        TRowDotBuf_cbGetHalfWordModule(_ModuleBuf,Code);
        _ErrCheck(_ModuleBuf, 16);
        _FillModuleFlag(CurFlag,1);//�����־
        _FillBuf(Pos,w,1);//����л���
      }
      break;
    default://������������־,�����λ����������
      memset(_ModuleBuf,0,16);
      _FillModuleFlag(CurFlag,1);//�����־
      _FillBuf(Pos,w,1);//����л���
      break;
    }//end switch
  }//end for
  return _RowBuf;
}

//---------------------------�õ���ʾ������--------------------------
unsigned char *TRowDotBuf_pGetDispBuf(void)
{
  return _RowBuf;
}


