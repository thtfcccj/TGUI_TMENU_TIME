/*******************************************************************************

                   TscrnExcel在对接TscrnExcel中的接口实现

*******************************************************************************/

#ifndef __TEXCEL_SCRN__H 
#define __TEXCEL_SCRN__H

#include "TScrnExcel.h"

//------------------------------得到标题头实现----------------------------------
const char* TExcel_Scrn_pGetHeader(const struct _TScrnExcelData *pExceData);

//---------------------------得到数据行总数实现----------------------------------
unsigned short TExcel_Scrn_GetItemCount(const struct _TScrnExcelData *pExceData);

//--------------------------------得到行数据实现--------------------------------
//返回NULL表示结束
const char* TExcel_Scrn_pGetLine(const struct _TScrnExcelData *pExceData,
                                     unsigned short Line); //指定行

//----------------------------------更新数据实现---------------------------------
//返回是否更新完成(因数据更新有延时，永远返回0即当前数据未完成)
signed char TExcel_Scrn_UpdateData(const struct _TScrnExcelData *pExceData,
                                       unsigned short StartLine, //起始行 
                                       unsigned short Count);    //总数

//--------------------------------指定行转换为阵列--------------------------------
unsigned short TExcel_Scrn_LineToAryId(const struct _TScrnExcelData *pExceData,
                                       unsigned short Line); //指定行

//--------------------------------指定阵列ID转换为行----------------------------
//返回0xffff在本页未找到(稍后才执行), 否则返回找到的行数
unsigned short TExcel_Scrn_AryIdToLine(const struct _TScrnExcelData *pExceData,
                                        unsigned short AryId);


#endif //__TSCRN_TEXCEL_H