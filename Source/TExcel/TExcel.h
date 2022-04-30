/*******************************************************************************

                        Excel����������֯ʵ��
���ἴ��Ϊ�������в��ұ�ID�ţ���ʵ��ɸѡ(�ֲ�ҳ�ڲ��ұ��ⲿʵ��ɸѡ)
����Ϊ��������Ի�״̬, ��ʵ�ְ�ָ��˳������
��ģ�鸺�𻺴�Excel���͵�һ�����ݣ������UI�޹�
*******************************************************************************/
#ifndef __TEXCEL_H 
#define __TEXCEL_H

/*******************************************************************************
                            �������
*******************************************************************************/

//ϵͳ��һҳ���ܹ���ʾ�µ��������
#ifndef TEXCEL_LINE_COUNT
  #define TEXCEL_LINE_COUNT    32         
#endif

//һ���ڣ����ܵ�����ַ�������(�ɳ�����ʾ��)
#ifndef TEXCEL_LINE_SIZE
  #define TEXCEL_LINE_SIZE    96 
#endif

#define SUPPORT_COLOR //֧����ɫʱ����

/*******************************************************************************
                            ��ؽṹ
*******************************************************************************/

#ifdef SUPPORT_COLOR //֧����ɫʱ
  #include "Color.h"
#endif

//----------------------------------��������------------------------------------
//ParaId�� ������ID: ���ṹ�������еĳ�Ա�����������У��±꼴Ϊ����ID
//�˹���Ϊ���������ṹ��ͨ����������ṹ���񽻻�����
struct _TExcel;
struct _TExcelParaDesc{
  //����ID��ʾ��Ϣ��������
  const unsigned char *pDispInfo;
  //�õ�����ID����ͷ�ַ���
  const char *(*pGetHeader)(unsigned char ParaId); 
  //�õ�����Id��Ӧ���ַ�����ʾ�ַ�,����NULL���ݽ���
  const char *(*pGetData)(struct _TExcel *pUi,
                           unsigned short InLine, //AryId = pUi->IdLut[InLine] 
                           unsigned char ParaId);
};

//����ID��ʾ��Ϣ����Ϊ��
#define TEXCEL_DIS_SPACE    0x80     //���������ֹ������
#define TEXCEL_ALIGN_SHIRT  5         //���뷽ʽ
#define TEXCEL_ALIGN_MASK   0x60     //
#define TEXCEL_ALIGN_L      0x00     //�����
#define TEXCEL_ALIGN_C      0x20     //���ж���
#define TEXCEL_ALIGN_R      0x40     //�Ҷ���
#define TEXCEL_LEN_MASK     0x1f    //ռλ���

//-----------------------------��̬�ṹ-----------------------------
struct _TExcelStatic{
  const struct _TExcelParaDesc *pParaDesc;      //������Ϣ����
  //�в���ID���ұ���λΪPara����(b0-6)����ɫ��ʶ(b7)��
  //�±�(1��ʼ)Ϊ��˳�򣬶�ӦֵΪ����ID�� 
  const unsigned char *pParaIdLut;
  //�õ�������(������),���ظ�ֵ��ʾ�Զ��Ӳ��ұ��еó�,ֵ��Ϊ�ο�
  //�˳�ԱΪNULLʱ��ʾ��̬��ȡ����
  signed short(*GetLineCount)(void);
};

//-------------------------------������ɫ֧��----------------------------------
//��ϵͳ֧����ɫ���ұ���������ɫʱ������������Ҫ���벽�裺
//1. ʵ��TExcel_cbFullDefaultColor()��TExcel_cbFullColor()����
//2. pParaIdLut->��λ���ȵ����λ��λ
//3. ��struct _TExcelParaDesc->pGetFunString()������:
//  (1. ����TExcel_SetItemColor()�� ��䵱ǰ����ɫ
//  (2. �����TExcel_SetStrColor()���ɷֱ���䱾���ڲ�ͬ�ַ���ͬ��ɫ
//ע����ֻ֧����ǰ��ɫ

//---------------------------------���ṹ---------------------------------------
#include "Color.h"

struct _TExcel{
  //����������أ�
  const struct _TExcelStatic *pStatic; //��̬���ݽṹ
  
  unsigned char Handle;                    //�û�������
  unsigned char PageLine;                  //��ǰҳ��ʾ����
  //ʹ�þֲ�(��ҳ��)ID���ұ�:
  unsigned char IdCount;                   //��ҳ�ڲ�ѯ����ID����,<=PageLine
  unsigned short LineCount;                //������������0��ʾ�Զ���ȡ  
  unsigned short StartLine;                //��ҳ��Ӧ����ʼ��λ�� 
  unsigned short IdLut[TEXCEL_LINE_COUNT]; //ID���ұ�

  //�����ݻ��������:
  char LineBuf[TEXCEL_LINE_SIZE];           //��֯һ������ʱ���ַ���������
  unsigned char LineStrLen;                 //�������ַ�����
  
  #ifdef SUPPORT_COLOR //֧����ɫʱ
    unsigned char Data2ColorSize;         //֧����ɫ���ʱ,��ɫ��Ϣ������ 
    Color_t ItemColor; //Data2ColorSize = 255ʱ���ô�ָ������ɫ���
    Color_t Data2ColorBuf[TEXCEL_LEN_MASK]; //֧����ɫ���ʱ����ɫ��Ϣ���
  #endif

  unsigned short JumpAryId;             //����ת������ID��0xffff��ʾ�޴˹���
    
};

//Flag����Ϊ��
//UiComͨѶ�������(�����ȼ�����, ������ͨѶ���ָ���)
#define TEXCEL_VALID_LUT    0x80 //���ұ��Ѹ���
#define TEXCEL_UPDATE_FINAL 0x08 //�����ڸ�����ɱ�־

//����:
#define TEXCEL_NEXT_REFRESH 0x01 //�´�Ҫ�����������ݱ�־


/******************************************************************************
                                  ����к���
*******************************************************************************/

//---------------------------------��ʼ������-----------------------------------
void TExcel_Init(struct _TExcel *pUi,
                 unsigned char Handle,        //���
                 const struct _TExcelStatic *pStatic,//��̬���ݽṹ
                 unsigned char PageLine);     //��ǰҳ��ʾ������0��ʾ���� 

//------------------------------���õ�ǰ����ɫ-----------------------------
void TExcel_SetItemColor(struct _TExcel *pUi, Color_t ItemColor);

//--------------------------------�����ַ���ɫ-----------------------------
void TExcel_SetStrColor(struct _TExcel *pUi, 
                        Color_t Color, 
                        unsigned char StrSize);

//-----------------------------�ɵ�ǰ�еõ����ұ�IDֵ------------------------
unsigned short TExcel_GetIdLutVol(const void *pHandle,
                                   unsigned short Line);

//-----------------------------�����Ƿ�������-----------------------------
#define TExcel_IsCurFinal(ui) (1)
  
/******************************************************************************
                            ����к���-UI����
*******************************************************************************/  
//------------------------------�õ�����ͷʵ��----------------------------------
const char* TExcel_pGetHeader(struct _TExcel *pUi);

//---------------------------�õ�����������ʵ��----------------------------------
unsigned short TExcel_GetLineCount(struct _TExcel *pUi);

//--------------------------------�õ�������ʵ��--------------------------------
//����NULL��ʾ����
const char* TExcel_pGetLine(struct _TExcel *pUi,
                                 unsigned short Line); //ָ����

//----------------------------------��������ʵ��---------------------------------
//�����Ƿ�������(�����ݸ�������ʱ����Զ����0����ǰ����δ���)
signed char TExcel_UpdateData(struct _TExcel *pUi,
                                  unsigned short StartLine, //��ʼ�� 
                                  unsigned short Count);    //����

//--------------------------------ָ����ת��Ϊ����--------------------------------
unsigned short TExcel_LineToAryId(struct _TExcel *pUi,
                                      unsigned short Line); //ָ����

//--------------------------------ָ������IDת��Ϊ��----------------------------
//����0xffff�ڱ�ҳδ�ҵ�(�Ժ��ִ��), ���򷵻��ҵ�������
unsigned short TExcel_AryIdToLine(struct _TExcel *pUi, unsigned short AryId);

//----------------------------------ˢ�µ�ǰ����ʵ��----------------------------
//ǿ�Ƹ��µ�ǰҳ����
//void ExcelUcCom_Refresh(struct _ExcelUcRec *pUi);
#define TExcel_Refresh(ui)  do{(ui)->Flag |= TEXCEL_NEXT_REFRESH;}while(0)

/******************************************************************************
                                  �ص�����
*******************************************************************************/

//------------------------------���ز��ұ���----------------------------------
//NextStartLine: b15��λ��ʾ����Ϊ�豸ID���´��豣֤���ҵ����д�ID
//����ʵ����װ�ز��ұ�����, <= Count
unsigned char TExcel_cbReloadLut(unsigned char Handle,        //���
                                  unsigned short NextStartLine,//ҳ���µ���ʼ��
                                  unsigned short *pIdLut,      //�����Ĳ��ұ�                                 
                                  unsigned char Count);        //��װ�ز��ұ�����


//------------------------���ָ��������Ĭ����ɫ---------------------------------
#ifdef SUPPORT_COLOR //֧����ɫʱ
  void TExcel_cbFullDefaultColor(signed char y); //��ҳ�ڶ�Ӧ������,��Ϊ����
#endif

//-----------------------------����ָ��λ����ɫ---------------------------------
#ifdef SUPPORT_COLOR //֧����ɫʱ
  void TExcel_cbFullColor(unsigned char y,
                          unsigned char x,
                          Color_t *pColorId, //�û�ָ������ɫID(��ϵͳɫ)
                          unsigned char Len);
#endif 
  

#endif  //__TEXCEL_H

