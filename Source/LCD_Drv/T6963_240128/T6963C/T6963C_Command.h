/*******************************************************************************

//			T6963C�ײ���������ͷ�ļ�
//
//��ͷ�ļ���װ��T6963C�Ĳ����ӿڣ����û�LCD�������,��IOӲ���޹�
//�����Ʒ�ʽ�ı�ʱ,���޸Ĵ��ļ���ʵ��
*******************************************************************************/
#ifndef __T6963C_COMMAND_H
#define __T6963C_COMMAND_H

/************************************************************************
			                LCD����IO�ӿڲ�ʵ��											 
************************************************************************/
//�޸Ĵ˽ӿ���ʵ�ʶ�������Ӧ
//���޸�Ӳ����ʼ��IO����		

#include "Parallel.h"
#include "ParallelSel.h"
#include "ParallelCtrl.h"

//ѡ����ʾ��
#define		T6963C_CsEn()	do{ParallelSel_Dev(PARALLEL_SEL_LCD);}while(0)
//����ʾ��
#define		T6963C_CsDis()	do{ParallelSel_Idie();}while(0)

//ѡ������
#define		T6963C_SelData()   do{ParallelCtrl_ClrA0();}while(0)
//ѡ��ָ��
#define		T6963C_SelCmd()  do{ParallelCtrl_SetA0();}while(0)

//���ź�
#define		T6963C_SetRd()	do{ParallelCtrl_SetRd();}while(0)
#define		T6963C_ClrRd()	do{ParallelCtrl_ClrRd();}while(0)

//д�ź�
#define		T6963C_SetWr()	do{ParallelCtrl_SetWr();}while(0)
#define		T6963C_ClrWr()	do{ParallelCtrl_ClrWr();}while(0)

//��Ϊ���
#define		T6963C_SetOut()	do{Parallel_SetOutput();}while(0)
//��Ϊ����
#define		T6963C_SetIn()	do{Parallel_SetInput();}while(0)
//д����
#define		T6963C_SetData(Data) do{Parallel_WrData(Data);}while(0)

//������
#define		T6963C_GetData()	(Parallel_RdData())

/************************************************************************
			��д��ʱʱ��ʵ��												 
************************************************************************/

#include "math_2.h"
//��������ʱʱ��	
#define		T6963C_RdDelay() do{Delay40Ns(20);}while(0)
//д������ʱʱ��
#define		T6963C_WrDelay() do{Delay40Ns(20);}while(0)		


/************************************************************************
			������ʵ��												 
************************************************************************/
#include "T6963C_Drv.h"

//--------------------------�ײ�IO�ڳ�ʼ��-------------------------------
//���û���ʼ����IO�ڵ�Ĭ��״̬
//void T6963C_HwInit(void);
//�����ѵ�����ɳ�ʼ��
#define T6963C_HwInit() do{}while(0)

//--------------------------˳��д����----------------------------------
//����T6963_StartAutoWr()�ɵ��ô˺�������д����
void T6963C_SqureWrData(const unsigned char *pData,
                        unsigned short Len);


#endif //__T6963C_COMMAND_H
