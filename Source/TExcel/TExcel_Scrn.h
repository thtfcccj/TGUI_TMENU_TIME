/*******************************************************************************

                   TscrnExcel�ڶԽ�TscrnExcel�еĽӿ�ʵ��

*******************************************************************************/

#ifndef __TEXCEL_SCRN__H 
#define __TEXCEL_SCRN__H

#include "TScrnExcel.h"

//------------------------------�õ�����ͷʵ��----------------------------------
const char* TExcel_Scrn_pGetHeader(const struct _TScrnExcelData *pExceData);

//---------------------------�õ�����������ʵ��----------------------------------
unsigned short TExcel_Scrn_GetItemCount(const struct _TScrnExcelData *pExceData);

//--------------------------------�õ�������ʵ��--------------------------------
//����NULL��ʾ����
const char* TExcel_Scrn_pGetLine(const struct _TScrnExcelData *pExceData,
                                     unsigned short Line); //ָ����

//----------------------------------��������ʵ��---------------------------------
//�����Ƿ�������(�����ݸ�������ʱ����Զ����0����ǰ����δ���)
signed char TExcel_Scrn_UpdateData(const struct _TScrnExcelData *pExceData,
                                       unsigned short StartLine, //��ʼ�� 
                                       unsigned short Count);    //����

//--------------------------------ָ����ת��Ϊ����--------------------------------
unsigned short TExcel_Scrn_LineToAryId(const struct _TScrnExcelData *pExceData,
                                       unsigned short Line); //ָ����

//--------------------------------ָ������IDת��Ϊ��----------------------------
//����0xffff�ڱ�ҳδ�ҵ�(�Ժ��ִ��), ���򷵻��ҵ�������
unsigned short TExcel_Scrn_AryIdToLine(const struct _TScrnExcelData *pExceData,
                                        unsigned short AryId);


#endif //__TSCRN_TEXCEL_H