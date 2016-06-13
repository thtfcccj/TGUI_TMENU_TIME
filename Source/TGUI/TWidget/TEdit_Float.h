/* ----------------------------------------------------------------------------
 *                TGUI编辑框-浮点数调整及其相关辅助函数
 * --------------------------------------------------------------------------*/

#ifndef __T_EDIT_FLOAT_H 
#define __T_EDIT_FLOAT_H

//----------------编辑器浮点数初始化函数--------------------
void TEditFloat(TEdit_t *pEdit,
              TWinHandle_t hWin,    //依赖的窗口,数据缓冲区>Len
              float Value,            //初始化值
              float Max,              //最大值
              float Min,              //最小值
              unsigned char DotPos,  //数确到小数的位数
              unsigned char Flag,  //相关配置
              TGUINotify_t cbGUINotify); //通报函数

/**********************************************************************
                          辅助函数
**********************************************************************/

//----------------浮点数转换为TSigned_t类型占位函数--------------------
//此函数与Data2Float成对使用,TSigned_t占位应>=float占位
TSigned_t Float2Data(float Value);

//-----------TSigned_t类型浮点数占位转换回浮点数函数--------------------
//此函数与Float2Data成对使用,TSigned_t占位应>=float占位
float Data2Float(TSigned_t Data);

//----------------由浮点数数值得到占位个数,不含符号--------------------
unsigned char GetFloatLen(float Min,float Max,
                          unsigned char DotPos);  //数确到小数的位数

//-----------------带标志浮点数转换为字符串函数------------------
//返回填充后最末未用缓冲区位置
char *pFloat2StringFlag(float Value,   //当前数值
                      char *pBuf,    //接收缓冲区
                      unsigned char Len,    //显示长度
                      //标志,定义为：低3bit:小数点位置,0x80:显示正负号
                      unsigned char Flag);

#endif

