/*******************************************************************************

                       TGUI需要的相关数学处理函数
此模块每个函数可独立使用不依赖其它结构或函数
*******************************************************************************/
#ifndef __T_GUI_MATH_H 
#define __T_GUI_MATH_H

#include "TGUIBase.h"

//---------------------------由位置查倍率-----------------------
//此表当TUnSigned_t变化时需修改
extern const TUnSigned_t TMutiTUnigned[];

//----------------得到一个有符号数的绝对值--------------------
TSigned_t TGetAbs(TSigned_t Value);

//----------------由数值得到占位个数,不含符号--------------------
unsigned char TGetNumLen(TSigned_t Min,TSigned_t Max);   //小值与大值

//----------------得到指定位置数值函数------------------------
TUnSigned_t TGetDigitalInPos(TUnSigned_t Value,
                           unsigned char Pos);

//-----------------带标志数转换为字符串函数------------------
//返回填充后最末未用缓冲区位置
char *pTNum2StringFlag(TSigned_t Value,   //当前数值
                      char *pBuf,    //接收缓冲区
                      unsigned char Len,    //显示长度
                      //标志,定义为：低3bit:小数点位置,0x80:显示正负号
                      unsigned char Flag);

#endif

