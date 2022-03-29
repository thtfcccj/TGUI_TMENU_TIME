/*******************************************************************************

                         TscrnExcel�ڶԽ�TscrnExcel�еĽӿ�ʵ��
*******************************************************************************/

#include "TExcel_Scrn.h"
#include "TExcel.h"

//------------------------------�õ�����ͷʵ��----------------------------------
const char* TExcel_Scrn_pGetHeader(const struct _TScrnExcelData *pExceData)
{
  return TExcel_pGetHeader((struct _TExcel *)pExceData->pData);
}

//---------------------------�õ�����������ʵ��----------------------------------
unsigned short TExcel_Scrn_GetItemCount(const struct _TScrnExcelData *pExceData)
{
  return TExcel_GetItemCount((struct _TExcel *)pExceData->pData);
}

//--------------------------------�õ�������ʵ��--------------------------------
//����NULL��ʾ����
const char* TExcel_Scrn_pGetLine(const struct _TScrnExcelData *pExceData,
                                     unsigned short Line) //ָ����
{
  return TExcel_pGetLine((struct _TExcel *)pExceData->pData, Line);
}

//----------------------------------��������ʵ��---------------------------------
//�����Ƿ�������(�����ݸ�������ʱ����Զ����0����ǰ����δ���)
signed char TExcel_Scrn_UpdateData(const struct _TScrnExcelData *pExceData,
                                       unsigned short StartLine, //��ʼ�� 
                                       unsigned short Count)    //����
{
  return TExcel_UpdateData((struct _TExcel *)pExceData->pData, 
                               StartLine, Count);
}

//--------------------------------ָ����ת��Ϊ����--------------------------------
unsigned short TExcel_Scrn_LineToAryId(const struct _TScrnExcelData *pExceData,
                                       unsigned short Line) //ָ����
{
  return TExcel_LineToAryId((struct _TExcel *)pExceData->pData, Line);
}

//--------------------------------ָ������IDת��Ϊ��----------------------------
//����0xffff�ڱ�ҳδ�ҵ�(�Ժ��ִ��), ���򷵻��ҵ�������
unsigned short TExcel_Scrn_AryIdToLine(const struct _TScrnExcelData *pExceData,
                                        unsigned short AryId)
{
  return TExcel_AryIdToLine((struct _TExcel *)pExceData->pData, AryId);
}
