/*******************************************************************************

                  TMenu�˵�֮��ֵ����ģʽ�ӿ�

*******************************************************************************/
#ifndef __T_MENU_M_NUM_ADJ_H 
#define __T_MENU_M_NUM_ADJ_H

#include "TMenuBase.h"

/*******************************************************************************
                              ˵��
*******************************************************************************/
//�򲿷ֱ�������ʾ��ʽ��ͬ,���еĴ�����ʾ����������,���������ı��༭���������!
//��ģ��ʵ�ֶ�ֵ��ʾ�����
//��ʾʾ��Ϊ(����ʾ):
//  
//  ������ 1002#�豸�������é�����
//  ��һ���¶�ϵ��(k0): 0.1256  ��  //����������ģʽ
//  ��    �¶����(��): 12     ��   //��������ģʽ
//  ��        ��������: 0b1001 ��  //������������ģʽ(�ݲ�֧��)
//  ��        �ڲ���ʶ: 0xFD06 ��  //ʮ������������ģʽ(�ݲ�֧��)
//  ��   [��  ��]    [�� ��]    ��
//  ������������������������������

//ע:��ֵ����ģʽ�����ڸ�����ֵ�ĵ��������ܱ༭������ַ���

/**********************************************************************
                      ������
**********************************************************************/

//���帴ѡ��ťĬ����ʽ(��TWidget.h�븴ѡ�ؼ�):
#define   TM_M_NUM_ADJ_STYLE_DEFAULT    \
  (TWIDGET_EN_HEADER | TWIDGET_EN_VSCOLL | TWIDGET_EN_NOTIFY)

//ע:���õ���:
//#define   TWIDGET_EN_FRAME    0x80  //�ù������б߿�,��LC�߶�>-3��ʱ������Ч
//#define   TWIDGET_EN_HEADER   0x40  //������ʾ����ͷ
//#define   TWIDGET_EN_HSCOLL   0x20  //������ʾˮƽ������
//#define   TWIDGET_EN_VSCOLL   0x10  //������ʾ��������
//#define   TWIDGET_EN_NOTIFY   0x04  //�޹�������߿�ʱ�������û���ʱ��ʾ��ʾ��Ϣ
//#define   TLISTBOXEX_EN_RETURN  0x02      //�����������

//------------------------------���������ṹ----------------------------
//MNumDescĬ�Ͻṹ֧�֣� b0:255, b1:0-1,  b2:0-3,    b3��0-9
//                       b4:0-99 b5:0-999,b6,0-9999
#ifndef TM_MNUMADJ_DEFAULT_DESC
  #define TM_MNUMADJ_DEFAULT_DESC  0   //Ĭ�ϲ�֧��
#endif 

#if TM_MNUMADJ_DEFAULT_DESC & 0x01
  extern  const struct _MNumDesc MNumDesc_U8;// = {MNUM_TYPE_DEC, 0, 255};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x02
  extern  const struct _MNumDesc MNumDesc_B1;// = {MNUM_TYPE_DEC, 0, 1};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x04
  extern  const struct _MNumDesc MNumDesc_B2;// = {MNUM_TYPE_DEC, 0, 3};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x08
  extern  const struct _MNumDesc MNumDesc_9;// = {MNUM_TYPE_DEC, 0, 9};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x10
  extern  const struct _MNumDesc MNumDesc_99;// = {MNUM_TYPE_DEC, 0, 99};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x20
  extern  const struct _MNumDesc MNumDesc_999;// = {MNUM_TYPE_DEC, 0, 999};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x40
  extern  const struct _MNumDesc MNumDesc_9999;// = {MNUM_TYPE_DEC, 0, 9999};
#endif
    
/**********************************************************************
                     �û�ʹ�����
**********************************************************************/

//��Ϊ��ֵ����ģʽʱ,�˵��ṹ�����¶���:
//typedef struct _TMenu{
//  TMTYPE_MNUMADJ | TM_MNUMADJ_?? //�˵�����Ϊ��ֵ����ģʽģʽ���û�����־
//  unsigned char Size;     //�ɹ�ѡ��ĵ��������
//  LanguageCode_t *pgHeader; //�˵�ͷ,ΪNULLʱ�ӻص����ȡ
//  struct _TMenu *pParent;   //���ѵĸ��˵�
//  const LanguageCode_t*;   //���ÿһ�������������(�ṹ)����
//  TMenuNotify_t Notify;     //���û��ռ佻����ͨ������
//}TMenu_t;

//�˵����ͻ�����־��Ϊ:
//��ֵ����ģʽ,����Ϊ��ʾģʽ
#define TM_MNUMADJ_WRITE     (0x01 << TMTYPE_USER_SHIFT)      
//ѭ������ģʽ,����ģʽ�����Ҽ������������ݼ�ѭ���л�,
//����,�����Ҽ�ʱ�ڵ�������л�,��ȷ��ʱ�Ž������ģʽ
#define TM_MNUMADJ_LOOP_MODE  (0x02 << TMTYPE_USER_SHIFT)   
//����ģʽʱ�����Ƿ���ȷ�ϼ�����Ϊ�Ҽ������û������Ҽ�ʱ�ر���Ч
#define TM_MNUMADJ_ENTER_KEY  (0x04 << TMTYPE_USER_SHIFT)  
//����ģʽʱ���崴��ʱ���������ģʽ
#define TM_MNUMADJ_CREATE_ENTER  (0x08 << TMTYPE_USER_SHIFT) 

//����ֵ������,���û��趨������̶�����(��MNumHeaderDesc��)����
//�������û��趨����:
struct _MNumDesc{ //ʮ���ƣ������ƣ�ʮ������ʹ�ô˽ṹ
  unsigned char Flag;  //��־
  TSigned_t Min; //��ǰ����������Сֵ
  TSigned_t Max; //��ǰ�����������ֵ
};
//��Ϊ֧�ָ���������ʱ,�������ṹ��ͬ����������:
struct _MNumDescFloat{
  unsigned char Flag;  //��־
  float Min; //��ǰ����������Сֵ
  float Max; //��ǰ�����������ֵ
};

//��ʹ��ֱ��ʹ��memcpy����ת��,Ҳ��ʹ�ô˺꽫struct _MNumDescFloatת��Ϊ
//struct _MNumDesc��ʽ:
#define MNUM_DESC_FLOAT2NUM(pNumDesc, pDescFloat)\
do{\
  (pNumDesc)->Flag = (pDescFloat)->Flag;\
  (pNumDesc)->Min = TFloat2Data((pDescFloat)->Min);\
  (pNumDesc)->Max = TFloat2Data((pDescFloat)->Max);\
}while(0)
//ֱ��ʹ��ʱ,Ӧʹ��TFloat2Dataת������,
//���صĸ�����ʹ��TData2Float�õ�

//��Ϊ�ַ�����������ʱ��ʹ��struct _MNumDesc�ṹ��������־��Ч

//����,��־����Ϊ��
//0-2λ��ʾС����λ��:
#define MNUM_DOT_POS_SHIFT   0
#define MNUM_DOT_POS_MASK    0x07
//3λ��ʾ����ģʽ,��1��ʾΪȺ����ģʽ
#define MNUM_TYPE_GRP_MODE   0x08
//4-6λ��ʾ������ģʽ:
#define MNUM_TYPE_SHIFT    4 
#define MNUM_TYPE_MASK     0x70
#define MNUM_TYPE_DEC      0x00   //ʮ������ģʽ
#define MNUM_TYPE_FLOAT    0x10   //������ģʽ
//#define MNUM_TYPE_HEX    0x20   //ʮ��������ģʽ
//#define MNUM_TYPE_BIN    0x30   //��������ģʽ
//���λΪ������ʾ���ű�־
#define MNUM_EN_SIGN      0x80

/**********************************************************************
                             �û������ݽṹ
**********************************************************************/
struct _MNumAdjUser{ 
 //1:֪ͨ�û������ݱ��ı�ʱ,��֪����ı���
 //2:��Ҫ��ȡ��ǰ����������ʱ,ָ����Ҫ��ȡ��һ��
 //��ǰ��������,����ʱ���д����򷵻ش������,���������ֹ��д
 TItemSize_t CurItem;   
 struct _MNumDesc Desc; //����ģʽʱ��ǰ������������
 //��ǰ������,�����ֵ,�ͳ��������ֵ(ע�⸡����Ӧʹ��TFloat2Data()��TData2Float()ת��)
 TSigned_t Value[TMENU_M_NUM_ADJ_MAX_ITEM]; 
};


//�û��ص�������Ӧ���ͼ���ʵ�ֵĹ�����:
//1:����Ϊ:TM_NOTIFY_GET_DATAʱ,�谴�˵��������˳��һ��һ��װ��Value��
//2:����Ϊ:TM_NOTIFY_CHANGEDʱ,��ʾCurItemָ����Value��ı���
//�û��ɸ���Ӧ��Ҫ������Ƿ���Ӧ(��:�����ֱ�Ӹ���Ӧ��)
//3:����Ϊ:TM_NOTIFY_SET_DATAʱ,��ʾ���޸����,�û�Ӧ���б���Ȳ���

//4:�ؼ��Զ�������:�õ���ǰ����������:��CurItemָ�������������ȡ��Desc��
//����ǰ���ݶ���Value��
#define     TM_NOTIFY_MNUM_GET_DESC     (TM_NOTIFY_WIDGET_BASE + 0)

/**********************************************************************
                             �ڲ����ݽṹ
**********************************************************************/

//�����ڲ����ݽṹ:
struct _MNumAdjData{
 TListboxEx_t  ListboxEx;       //�ṹ����
 TEdit_t Edit;			//�༭�򻺴�
 unsigned char HeaderW;        //�����ʾͷ���
 volatile unsigned char Flag;  //�ڲ���־
 struct _MNumAdjUser User;    //�û�����
};

/**********************************************************************
                             ��غ���
**********************************************************************/
//------------------------��������ʵ��----------------------------
//1:���������Ͳ˵�,���û���������ݲ�������ʾ
//���ش����ɹ�:
signed char TMenu_MNumAdjCreate(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                                TWinHandle_t hWin,       //������˵��Ĵ���
                                unsigned char Data, //�������������,����������
                                void *pv);      //�ɹ�ʹ�õ����ɻ�����

//------------------------��Ӧ��������ʵ��----------------------------
//2:��Ӧ��������������������û��������ݽ���,��������ʾ
//����ֵ����Ϊ: ��ֵ:������Ӧ�Ӳ˵�,-1,����,-2:�����ϲ�˵�
signed char TMenu_MNumAdjKey(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                             unsigned char Key,       //����ļ�ֵ
                             void *pv);      //�ѱ�ʹ�õ��������ݻ�����

//-----------------------��ֵ����ģʽ�п��ͨ���ص�ʵ�ֺ���------------------------
//�����ڲ�ͨ��ʹ��
const void*TMenu_MNumAdjListBoxNotify(void *pvPource,
                                      unsigned char Type,
                                      const void *pvData);

//-----------------�༭ģʽͨ���ص�ʵ�ֺ���------------------------
//�����ڲ�ͨ��ʹ��
const void*TMenu_MNumAdjEditNotify(void *pvPource,
                                   unsigned char Type,
                                   const void *pvData);

/**********************************************************************
                     �ص�����
**********************************************************************/

//----------------------����һ��TEdit����----------------------------
//TWinHandle_t cbTMenu_hGetEdit(TWinHandle_t hParent);
#define cbTMenu_hGetEdit(hParent) TM_hGetEditWin()

//----------------------�õ���ǰ��ʾ��ʽ-------------------------
#ifdef TMENU_STYLE_VARIABLE   //�ɱ���ʽʱ���û�������
  unsigned char TMenu_MNumAdj_cbGetCurStyle(void);
#else
  #define TMenu_MNumAdj_cbGetCurStyle() \
    (TM_M_NUM_ADJ_STYLE_DEFAULT | TLISTBOXEX_EN_RETURN)
#endif

//----------------------��ǰ�����ֱ�־-------------------------
//���޸�ģʽʱ���ɾ����Ƿ��б�����TCHECKBOX_EN_SAVE
#ifdef TMENU_STYLE_VARIABLE   //�ɱ���ʽʱ���û�������
  unsigned char TMenu_MNumAdj_cbGetCurSaveStyle(void);
#else
  #define TMenu_MNumAdj_cbGetCurSaveStyle() TCHECKBOX_EN_SAVE
#endif
  
#endif //#ifndef __T_MENU_M_NUM_ADJ_H 
