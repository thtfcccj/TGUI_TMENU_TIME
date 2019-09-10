/*******************************************************************************

                           TWidget之编辑框-十浮点数调整实现

*******************************************************************************/
#include "TEdit.h"
#include <string.h>
#include "TEditPrv.h"  //内部数据,仅供内部使用
#include "TEdit_Float.h"
#include <math.h>       //floor

/**********************************************************************
                          支持浮点数运算时
**********************************************************************/
#if TGUI_EDIT_FLOAT_SUPPORT 

//----------------编辑器浮点数初始化为函数--------------------
void TEditFloat(TEdit_t *pEdit,
                TWinHandle_t hWin,    //依赖的窗口,数据缓冲区>Len
                float Value,            //初始化值
                float Max,              //最大值
                float Min,              //最小值
                unsigned char DotPos,  //数确到小数的位数
                unsigned char Flag,  //相关配置
                TGUINotify_t cbGUINotify) //通报函数
{
  //将浮点数转换为整数运算,直接使用整数机制
  float Muti = (float)TMutiTUnigned[DotPos];  
  //转换为整数运算
  TEditDec(pEdit,hWin,
           (TSigned_t)floor(Muti * Value + 0.5),
           (TSigned_t)floor(Muti * Max + 0.5),
           (TSigned_t)floor(Muti * Min + 0.5),
           DotPos,Flag,cbGUINotify);

  pEdit->eType = TEditType_Float;//最后改类型
}

//----------------浮点数转换为TSigned_t类型占位函数--------------------
//此函数与TData2Float成对使用,TSigned_t占位应>=float占位
TSigned_t TFloat2Data(float Value)
{
  TSigned_t Data;
  memcpy(&Data,&Value,sizeof(float));
  return Data;  
}

//-----------TSigned_t类型浮点数占位转换回浮点数函数--------------------
//此函数与TFloat2Data成对使用,TSigned_t占位应>=float占位
float TData2Float(TSigned_t Data)
{
  float Value;
  memcpy(&Value,&Data,sizeof(float));
  return Value; 
}


//----------------由浮点数数值得到占位个数,不含符号--------------------
unsigned char TGetFloatLen(float Min,float Max,
                          unsigned char DotPos)  //数确到小数的位数
{
  float Muti = (float)TMutiTUnigned[DotPos & 0x07]; 
  return TGetNumLen((TSigned_t)floor(Muti * Min + 0.5),
                    (TSigned_t)floor(Muti * Max + 0.5));
}

//-----------------带标志浮点数转换为字符串函数------------------
//返回填充后最末未用缓冲区位置
char *pTFloat2StringFlag(float Value,   //当前数值
                      char *pBuf,    //接收缓冲区
                      unsigned char Len,    //显示长度
                      //标志,定义为：低3bit:小数点位置,0x80:显示正负号
                      unsigned char Flag)
{
  //转换为整数
  float Muti = (float)TMutiTUnigned[Flag & 0x07]; 
  Muti = Muti * Value + 0.5;
  return pTNum2StringFlag((TSigned_t)floor(Muti),pBuf,Len,Flag);
}


/**********************************************************************
    不支持浮点数运算时,为防止使用空函数宏时可能的编译警告,函数定义为空
**********************************************************************/
#else

//----------------编辑器浮点数初始化为函数--------------------
void TEditFloat(TEdit_t *pEdit,
                TWinHandle_t hWin,    //依赖的窗口,数据缓冲区>Len
                float Value,            //初始化值
                float Max,              //最大值
                float Min,              //最小值
                unsigned char DotPos,  //数确到小数的位数
                unsigned char Flag,  //相关配置
                TGUINotify_t cbGUINotify) //通报函数
{
  pEdit = pEdit;
  hWin = hWin;
  Value = Value;
  Max = Max;
  Min = Min;
  DotPos = DotPos;
  Flag = Flag;
  cbGUINotify = cbGUINotify;
}

//----------------浮点数转换为TSigned_t类型占位函数--------------------
//此函数与TData2Float成对使用,TSigned_t占位应>=float占位
TSigned_t TFloat2Data(float Value)
{
  return Value;  
}

//-----------TSigned_t类型浮点数占位转换回浮点数函数--------------------
//此函数与TFloat2Data成对使用,TSigned_t占位应>=float占位
float TData2Float(TSigned_t Data)
{
  return Data; 
}


//----------------由浮点数数值得到占位个数,不含符号--------------------
unsigned char TGetFloatLen(float Min,float Max,
                          unsigned char DotPos)  //数确到小数的位数
{
  Max = Max;
  Min = Min;
  DotPos = DotPos;
  return 0;
}

//-----------------带标志浮点数转换为字符串函数------------------
//返回填充后最末未用缓冲区位置
char *pTFloat2StringFlag(float Value,   //当前数值
                      char *pBuf,    //接收缓冲区
                      unsigned char Len,    //显示长度
                      //标志,定义为：低3bit:小数点位置,0x80:显示正负号
                      unsigned char Flag)
{
  //转换为整数
  Value = Value;
  Len = Len;
  Flag = Flag;
  return pBuf;
}
#endif



