/*******************************************************************************

        TWin���ڹ�����ģ��ӿ�-���и���,�̶����ȼ��Ĵ��ڵ�ʵ�ֽӿ�

*******************************************************************************/
#ifndef __T_WIN_MNG_ROW_H 
#define __T_WIN_MNG_ROW_H

//--------------------------TWinMng˵��--------------------------------
//TWinMngΪTWIN��һ��ʵ�������ڹ�����,��Ҫ��������ڵķ���͵Ĵ������ȼ���ʾ
//�˹�����������������:
//1:�����еĸ���,��ֱ�Ӹ��´���������
//2:�̶��Ĵ��ڸ����ʹ�����ʾ���ȼ�:ʹ�ô��ڳ��з��䴰��,�±�Խ��,���ȼ�Խ��
//3:����ʾ��֧��

//ʹ�ô�ʵ���Ĳ���Ӧ��Ӧ�ù̶���������ȼ��ʹ��ڵķ���
//��ϵͳ����ʱ����ô��ڵ���ʾ����λ��

/**********************************************************************
                      ���������
**********************************************************************/
#include "TWM.h"    //Ϊ��ģ���ʵ����

//������󴰿ڸ���,<=64��
#ifndef TWIN_MAX_COUNT
  #define       TWIN_MAX_COUNT    8  
#endif

//���崰������λ��(һλ��Ӧһ��)�����ұ�
#if(TWIN_MAX_COUNT > 32)
  #define WinMask_t unsigned long long
  #define WinShift2Mask(shift)  LlShift2Mask(shift) //��֧�ֲ��ұ�
#elif(TWIN_MAX_COUNT > 16)
  #define WinMask_t unsigned long
  #ifdef U32ShiftLUT
    #define WinShift2Mask(shift) U32ShiftLUT[shift]
  #else
    #define WinShift2Mask(shift) ((WinMask_t)1 << (shift))
  #endif
#elif(TWIN_MAX_COUNT > 8) 
  #define WinMask_t unsigned short
  #ifdef U16ShiftLUT
    #define WinShift2Mask(shift) U16ShiftLUT[shift]
  #else
    #define WinShift2Mask(shift) ((WinMask_t)1 << (shift))
  #endif
#else
  #define WinMask_t unsigned char
  #define WinShift2Mask(shift) U8ShiftLUT[shift]       //��֧�ֲ��ұ�
#endif

//����������λ��(һλ��Ӧһ��)�����ұ�
#if(TLCD_HIGH > 32)
  #define RowMask_t unsigned long long
  #define RowShift2Mask(shift)  LlShift2Mask(shift) //��֧�ֲ��ұ�
#elif(TLCD_HIGH > 16)
  #define RowMask_t unsigned long
  #ifdef U32ShiftLUT
    #define RowShift2Mask(shift) U32ShiftLUT[shift]
  #else
    #define RowShift2Mask(shift) ((RowMask_t)1 << (shift))
  #endif
#elif(TLCD_HIGH > 8) 
  #define RowMask_t unsigned short
  #ifdef U16ShiftLUT
    #define RowShift2Mask(shift) U16ShiftLUT[shift]
  #else
    #define RowShift2Mask(shift) ((RowMask_t)1 << (shift))
  #endif
#else
  #define RowMask_t unsigned char
  #define RowShift2Mask(shift) U8ShiftLUT[shift]      //��֧�ֲ��ұ�
#endif

/**********************************************************************
                           ��ؽṹ
**********************************************************************/
//���ڹ��������
typedef struct{
  TWin_t *pTWinAry;  //��ǰ��������
  unsigned char Count;//�������д�С  
  unsigned char Lcd;    //��ǰ�����Ǹ���ʾ��
  RowMask_t RowUpdate;  //�и��±�־
  unsigned char PrvUpdatedRow;  //�ϴ��ѱ����µ���
  unsigned char Flag;   //��־��������
  char RowBuf[TLCD_WIDTH]; //��ǰ������еĻ���
  unsigned char RowFlagBuf[TLCD_WIDTH]; //��ǰ�б�־����,��TGUICfg.h����
}TWinMng_t;

//���б�־����Ϊ��
#define		TWINMNG_CONTEXT_BUF_RDY	0x01	//�б�־׼����
   
extern TWinMng_t *pCurTWinMng;    //��ǰ������

/*****************************************************************************
                          ��Ϊ����
*****************************************************************************/

//-------------------------���ڹ�������ʼ������----------------------------
void TWinMng_Init(TWinMng_t *pTWinMng,
                  unsigned char Lcd,    //��ǰ�����Ǹ���ʾ��
                  TWin_t *pTWinAry,     //��ǰ��������ͷ
                  unsigned char Count); //��ǰ�������д�С,<=TWIN_MAX_COUNT

//------------------����ָ��GUI������Ϊ��ǰ����-----------------------
//����ʱ���ô˺�����ʼ��
void TWinMng_SetCur(TWinMng_t *pTWinMng);

//-------------------------�����л���������----------------------------
//�˺���һ�ν�����һ�л�����
//���ڵ��ô˺���������ʾTWinMng���ݵ��л�����
//����0�������,����1��Ҫ����
signed char TWinMng_UpdateRow(void);

//-------------------------������ʾ����----------------------------
//���л����������ͳ�
//���ڵ��ô˺�������TWinMng�л��������ݵ���ʾ��
void TWinMng_UpdateLcd(void);

#endif //#ifndef __T_WIN_MNG_ROW_H 

