/*******************************************************************************

                           TEdit的内部相关部分接口
此模块基类:TWIN(名柄)
*******************************************************************************/
#ifndef __T_EDIT_PRV_H 
#define __T_EDIT_PRV_H

#include "TGUIMath.h"

//根据调整类型的功能性操作集,若需实现一个数的调整与显示，除完成初始化外,还需完成此函数
struct _TEditFun{
  void(*UpdateBuf)(const struct _TEdit*);//根据调整类型更新数据到缓冲区
  void(*Key)(struct _TEdit*,unsigned char Key);//按键处理函数
};

//---------------------得到指定位置位置字符函数----------------
char TEdit_GetChar(const struct _TEdit *pEdit,unsigned char Pos);

//-----------------------得到编辑位置字符串函数-------------------
//此函数不适用于字符串调整模式
const char* TEdit_pGetString(const struct _TEdit *pEdit);

//-----------------十进制调整时更新到缓冲区处理函数------------------
void TEditDec_UpdateBuf(const struct _TEdit *pEdit);

//-------------------十进制调整时按键处理函数--------------------
void TEditDec_KeyPro(struct _TEdit *pEdit,unsigned char Key);


#endif

