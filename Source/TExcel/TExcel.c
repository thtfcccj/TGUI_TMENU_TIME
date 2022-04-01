/*******************************************************************************

                    Excel����������֯ʵ��

********************************************************************************/
#include "TExcel.h"
#include "math_3.h"
#include <string.h>
#include "stringEx.h"

/******************************************************************************
                              ��غ���ʵ��
*******************************************************************************/

//---------------------------------��ʼ������-----------------------------------
void TExcel_Init(struct _TExcel *pUi,
                 unsigned char Handle,        //���
                 const struct _TExcelStatic *pStatic,//��̬���ݽṹ
                 unsigned char PageLine)     //��ǰҳ��ʾ������0��ʾ����
{
  memset(pUi, 0, sizeof(struct _TExcel));
  pUi->Handle = Handle;
  pUi->pStatic = pStatic;
  if((PageLine == 0) || (PageLine > TEXCEL_LINE_COUNT))//�쳣
    PageLine = TEXCEL_LINE_COUNT;
  pUi->PageLine = PageLine;
  //��ʼ��������
  pUi->JumpAryId = 0xffff;//�޴˹���
  //�״�װ��ҳ�ڲ��ұ�
  pUi->IdCount = TExcel_cbReloadLut(Handle, 0, pUi->IdLut, PageLine);
}

//-----------------------------�ɵ�ǰ�еõ����ұ�IDֵ------------------------
unsigned short TExcel_GetIdLutVol(const void *pHandle,
                                        unsigned short Line)
{
  const struct _TExcel *pUi = (struct _TExcel *)pHandle;
  return pUi->IdLut[Line - pUi->StartLine];  
}

/******************************************************************************
                            ����к���-UI����ʵ��
*******************************************************************************/  


//------------------------------�õ�����ͷʵ��----------------------------------
const char* TExcel_pGetHeader(struct _TExcel *pUi)
{
  const struct _TExcelStatic *pStatic = pUi->pStatic;
  //�ȵõ���ʾ���ұ�
  const unsigned char *pParaIdLut = pStatic->pParaIdLut;
  unsigned char ParaLen = *pParaIdLut++ & 0x7f;
  char *pBuf = pUi->LineBuf;
  //������ʾ��ʽ���
  const struct _TExcelParaDesc *pParaDesc = pStatic->pParaDesc;
  const unsigned char *pDispInfo = pParaDesc->pDispInfo;
  for(; ParaLen > 0; ParaLen--){
    unsigned char ParaId = *pParaIdLut++;
    unsigned char DispInfo = *(pDispInfo + ParaId);
    unsigned char Len = DispInfo & TEXCEL_LEN_MASK; //����
    const char *pHeader = pParaDesc->pGetHeader(ParaId);
    if(pHeader == NULL)//û�б���ʱ�ո����
      memset(pBuf, ' ', Len);
    else //���뷽ʽ����ַ�
      strFull(pBuf, //������
              pHeader,        //����Դ
              Len,
              (DispInfo & TEXCEL_ALIGN_MASK) >> TEXCEL_ALIGN_SHIRT);//���뷽ʽ
    pBuf += Len;
    if(!(DispInfo & TEXCEL_DIS_SPACE)){//û�н�ֹ���ʱ���ո��Լ��
      *pBuf++ = ' ';
    }
  }
  *pBuf = '\0';//����������ַ�
  if(!pUi->LineStrLen) pUi->LineStrLen = strlen(pUi->LineBuf); //��ȡ����
  return pUi->LineBuf;
}

//---------------------------�õ�����������ʵ��----------------------------------
unsigned short TExcel_GetLineCount(struct _TExcel *pUi)
{
  const struct _TExcelStatic *pStatic = pUi->pStatic;
  signed short Count;
  if(pStatic->GetLineCount != NULL){
    Count = pStatic->GetLineCount();
    if(Count >= 0){//ֱ���û��ռ��ȡ
      pUi->LineCount = Count; //�ֶ�������Ч
      return Count;
    }
  }
  else Count = -16383; //δ����ʱ�����������
  //�Զ���ȡ
  if(pUi->LineCount > 0) //���Զ�����ʱ
    return pUi->LineCount;
  return 0 - Count;//��ʹ��Ĭ��ֵ
}

//--------------------------------�õ�������ʵ��--------------------------------
//����NULL��ʾ����
const char* TExcel_pGetLine(struct _TExcel *pUi,
                                 unsigned short Line) //ָ����(�����ж�ҳ)
{
  const struct _TExcelStatic *pStatic = pUi->pStatic;
  
  //�õ���ǰ������λ��
  unsigned short StartLine = pUi->StartLine;
  if((Line < StartLine) || (Line >= (StartLine + pUi->IdCount))){
    //�쳣���ݲ��ڻ�������
    memset(pUi->LineBuf, ' ', TEXCEL_LINE_SIZE - 1);
    *(pUi->LineBuf + (TEXCEL_LINE_SIZE - 1)) = '\0';//�����ַ�
    return pUi->LineBuf; 
  }
  const unsigned char *pParaIdLut = pStatic->pParaIdLut;
  unsigned char ParaLen = *pParaIdLut++;
  unsigned char HaveColor = ParaLen & 0x80;
  ParaLen &= ~0x80;
  
  char *pBuf = pUi->LineBuf;
  
  //֧����ɫ���ʱ��ͨ���û����Ĭ��ɫ
  #ifdef SUPPORT_COLOR //֧����ɫʱ
    TExcel_cbFullDefaultColor(Line);
  #endif
  
  const struct _TExcelParaDesc *pParaDesc = pStatic->pParaDesc;
  //���ݶ��뷽ʽ���
  for(; ParaLen > 0; ParaLen--){
    unsigned char ParaId = *pParaIdLut++;
    #ifdef SUPPORT_COLOR //֧����ɫʱ
      pUi->Data2ColorSize = 0;//Ĭ�ϲ����
    #endif
    const char *pDispStr = pParaDesc->pGetData(pUi, Line, ParaId); //��ʾ�ַ���
    if(pDispStr == NULL){//������
      pUi->LineCount = pUi->StartLine + Line;//��������
      return NULL;
    }
    unsigned char DispInfo = *(pParaDesc->pDispInfo + ParaId);
    unsigned char Space = strFull(pBuf, //������
                                  pDispStr,        //����Դ
                                  DispInfo & TEXCEL_LEN_MASK, //��󳤶�
               (DispInfo & TEXCEL_ALIGN_MASK) >> TEXCEL_ALIGN_SHIRT);//���뷽ʽ
    //֧����ɫ������û��������ɫʱ�����û���䵱ǰ���ָ��λ����ɫ
    #ifdef SUPPORT_COLOR //֧����ɫʱ
     if(HaveColor){
       unsigned char ColorSize= pUi->Data2ColorSize;
       if(ColorSize == 255){//ʹ��ͳһ��ɫʱԤ����
         ColorSize = strlen(pDispStr);
         Color_Full(pUi->ItemColor, pUi->Data2ColorBuf, ColorSize);
       }
       if(ColorSize > 0){//���������ɫ
          TExcel_cbFullColor(Line, 
                             (pBuf - pUi->LineBuf) + Space,
                             pUi->Data2ColorBuf, 
                             pUi->Data2ColorSize);          

       }
     }
    #endif //SUPPORT_COLOR
    pBuf += DispInfo & TEXCEL_LEN_MASK;
    if(!(DispInfo & TEXCEL_DIS_SPACE)){//û�н�ֹ���ʱ���ո��Լ��
      *pBuf++ = ' ';
    }
  }
  *pBuf = '\0';//����������ַ�
  return pUi->LineBuf;
}

//----------------------------------��������ʵ��---------------------------------
//�����Ƿ�������
signed char TExcel_UpdateData(struct _TExcel *pUi,
                                  unsigned short StartLine, //��ʼ�� 
                                  unsigned short Count)    //����
{
  pUi->StartLine = StartLine;
  //����ҳ�ڲ��ұ�
  pUi->IdCount = TExcel_cbReloadLut(pUi->Handle, StartLine, 
                                    pUi->IdLut, Count);
  return 1; //ֱ�����
}

//--------------------------------ָ����ת��Ϊ����--------------------------------
unsigned short TExcel_LineToAryId(struct _TExcel *pUi,
                                      unsigned short Line) //ָ����
{
  if(Line < pUi->StartLine) return 0xffff;//�쳣
  Line -= pUi->StartLine;
  if(Line >= pUi->IdCount) return 0xffff;//�쳣
  return pUi->IdLut[Line];
}

//--------------------------------ָ������IDת��Ϊ��----------------------------
//����0xffff�ڱ�ҳδ�ҵ�(�Ժ��ִ��), ���򷵻��ҵ�������
unsigned short TExcel_AryIdToLine(struct _TExcel *pUi, unsigned short AryId)
{
  //����Ƿ��ڱ�ҳ
  for(unsigned char Line = 0; Line < pUi->IdCount; Line++){
    if(AryId == pUi->IdLut[Line]) return Line;//�ҵ���
  }
  
  //����ҳ�ڲ��ұ����豸ID��ʽ
  pUi->IdCount = TExcel_cbReloadLut(pUi->Handle,  0x8000 | AryId, 
                                    pUi->IdLut, pUi->PageLine);
  //�ٴμ���Ƿ��ڱ�ҳ
  for(unsigned char Line = 0; Line < pUi->IdCount; Line++){
    if(AryId == pUi->IdLut[Line]) return Line;//�ҵ���
  }
  return 0xffff;//δ�ҵ��豸
  
}

//------------------------------���õ�ǰ����ɫ-----------------------------
void TExcel_SetItemColor(struct _TExcel *pUi, Color_t ItemColor)
{
  pUi->ItemColor = ItemColor;
  pUi->Data2ColorSize = 255;
}

//--------------------------------�����ַ���ɫ-----------------------------
void TExcel_SetStrColor(struct _TExcel *pUi, 
                        Color_t Color, 
                        unsigned char StrSize)
{
  Color_Full(Color, pUi->Data2ColorBuf, StrSize);
  pUi->Data2ColorSize = StrSize;
}




