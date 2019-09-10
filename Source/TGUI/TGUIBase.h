/*******************************************************************************

                           TGUI��������
��ģ�鶨����TGUI��Ҫ����ؽṹ����ʵ��
*******************************************************************************/
#ifndef __T_GUI_BASE_H 
#define __T_GUI_BASE_H

#include "TGUICfg.h"       //TGUI����
#include "TGUICallBack.h"  //TGUI�ص�
#include "TGUINotify.h"   //TGUI��Ϣͨ��

/**********************************************************************
                         ��ؽṹ
**********************************************************************/
//����������
typedef struct{
  signed char x;     //x����
  signed char y;     //y����  
  signed char w;    //���,ȡ��ֵ  
  signed char h;    //�߶�,ȡ��ֵ     
}TRect_t;

//����������,�������߶ȹ̶�Ϊ1
typedef struct{
  signed char x;     //x����
  signed char y;    //y����  
  signed char w;    //���,ȡ��ֵ    
}TFocus_t;

//��Ŀ����������
typedef struct{
  TItemSize_t x;    //x����
  TItemSize_t y;    //y����  
  TItemSize_t w;    //���  
  TItemSize_t h;    //�߶�    
}TItemsRect_t;

//��Ŀ����������,�������߶ȹ̶�Ϊ1
typedef struct{
  TItemSize_t x;     //x����
  TItemSize_t y;    //y����  
  TItemSize_t w;    //���,ȡ��ֵ    
}TItemsFocus_t;

/**********************************************************************
                         ����
**********************************************************************/
//��ֵ//��ӦASCII����ӡ�ַ�,��Χ0-31,���Ƽ�����Ϊ:
#define TGUI_KEY_BACKSPACE        20 
#define TGUI_KEY_TAB              21
#define TGUI_KEY_ENTER            22    //����
  
#define TGUI_KEY_LEFT             16    //����
#define TGUI_KEY_UP               17    //����
#define TGUI_KEY_RIGHT            18    //����
#define TGUI_KEY_DOWN             19    //����
#define TGUI_KEY_PAGEUP           23
#define TGUI_KEY_PAGEDOWN         24
#define TGUI_KEY_SHIFT            25
#define TGUI_KEY_CONTROL          26
#define TGUI_KEY_ESCAPE           27
#define TGUI_KEY_INSERT           29
#define TGUI_KEY_DELETE           30

/**********************************************************************
                     �ڲ�ʹ�õı������Ͷ���
**********************************************************************/

//��ʹ��Edit�ؼ���ֵ����ʱ,������ʹ����ֵ�������������(��ֹΪ����������)
//��ʹ�ø�����ʱ,Ӧ���ڸ�����ռλ
#if TGUI_EDIT_FLOAT_SUPPORT || TGUI_EDIT_INT32_SUPPORT
  #define TSIGNED_SIZE	4     //����ռλ
  typedef TS32 TSigned_t;     //�з�����
  typedef TU32 TUnSigned_t;   //�޷�����
#elif TGUI_EDIT_INT16_SUPPORT
  #define TSIGNED_SIZE	2     //����ռλ
  typedef TS16 TSigned_t;     //�з�����
  typedef TU16 TUnSigned_t;   //�޷�����
#else
  #define TSIGNED_SIZE	1     //����ռλ
  typedef signed char   TSigned_t;     //�з�����
  typedef unsigned char TUnSigned_t;   //�޷�����
#endif 

/**********************************************************************
                         ��ع�����
**********************************************************************/

//-----------------------���������õ�ռλ����-----------------------
unsigned char TGetItemsPlace(TItemSize_t Items);

//-----------------------�õ����ַ��Ҷ����ĳ��Ⱥ���-----------------------
unsigned char TGetAlignLenR(unsigned char Len,     //ԭ�ַ�����,<128
                          unsigned char Append); //�����ַ�����,<128

//-----------------------�õ����ַ�������ĳ��Ⱥ���-----------------------
unsigned char TGetAlignLenL(unsigned char Len,     //ԭ�ַ�����,<128,> 1
                          unsigned char Append); //�����ַ�����,<128

//-----------------------ת��Ϊ�ַ���-��ʾΪ�����----------------------
unsigned char TValue2StringMin(TSigned_t Value,
                            char *pString,//���ջ���
                            unsigned char Min);//��֤����Сλ��

//-----------------------�õ����ַ������ĳ��Ⱥ���-----------------------
//����Ҫ˫�ֽڶ���ʱ,δ����ʱ�ڸ�λ�����' '
//��������Ļ���
char *pTAlignString(TItemSize_t ItemLen,//�ַ�����
                   char *pString);//���ջ���  

//------------------�ַ�����չ��������-----------------------
//1:��Ӧ�����뻻���ַ�,
//2:���ؽ�����λ��
char *Tstrcpy_ex(char *s1, const char *s2);


#endif
