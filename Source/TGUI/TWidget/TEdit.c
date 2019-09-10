/*******************************************************************************

                           TWidget之编辑框实现
*******************************************************************************/
#include "TEdit.h"
#include <string.h>
#include "TEditPrv.h"  //内部数据,仅供内部使用
#include "TEdit_Float.h"

//---------------------得到指定位置位置字符函数----------------
char TEdit_GetChar(const struct _TEdit *pEdit,unsigned char Pos)
{
  return TWin_GetChar(TWM_pGetWin((pEdit->hWin)),Pos,0);
}

//-----------------------得到字符串函数----------------------------
//此函数不适用于字符串调整模式
const char* TEdit_pGetString(const struct _TEdit *pEdit)
{
  return TWin_pGetString(TWM_pGetWin(pEdit->hWin),0);
}

//----------------得到调整后的数函数--------------------
//获得当前已调整完成的数(浮点数需使用TData2Float)转换得到
TSigned_t TEdit_GetVolume(const TEdit_t *pEdit)
{
  #if TGUI_EDIT_FLOAT_SUPPORT != 0
  float Value;
  if(pEdit->eType == TEditType_Float){
    Value = (float)((TSigned_t)pEdit->Value);
    Value /= (float)TMutiTUnigned[pEdit->DotPos];
    return TFloat2Data(Value);
  }
  #endif
  return pEdit->Value;
}

//------------------画图函数并更新缓冲区函数------------------
void TEdit_PaintAll(TEdit_t *pEdit)
{
  //因常量定义会导致不可兼容性问题,故直接使用case
  switch(pEdit->eType){
  case TEditType_Float:  //整数处理
  case TEditType_Dec:TEditDec_UpdateBuf(pEdit);
  default:break;
  }
}

//-----------------------按键处理函数------------------------------
void TEdit_Key(TEdit_t *pEdit,unsigned char Key)
{
  //因常量定义会导致不可兼容性问题,故直接使用case
  switch(pEdit->eType){
  case TEditType_Float:  //整数处理
  case TEditType_Dec:TEditDec_KeyPro(pEdit,Key);
  default:break;
  }
}



