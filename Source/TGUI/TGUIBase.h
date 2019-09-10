/*******************************************************************************

                           TGUI基本构件
此模块定义了TGUI需要的相关结构及其实现
*******************************************************************************/
#ifndef __T_GUI_BASE_H 
#define __T_GUI_BASE_H

#include "TGUICfg.h"       //TGUI配置
#include "TGUICallBack.h"  //TGUI回调
#include "TGUINotify.h"   //TGUI消息通报

/**********************************************************************
                         相关结构
**********************************************************************/
//矩形区域定义
typedef struct{
  signed char x;     //x坐标
  signed char y;     //y坐标  
  signed char w;    //宽度,取正值  
  signed char h;    //高度,取正值     
}TRect_t;

//焦点区域定义,焦点区高度固定为1
typedef struct{
  signed char x;     //x坐标
  signed char y;    //y坐标  
  signed char w;    //宽度,取正值    
}TFocus_t;

//项目矩形区域定义
typedef struct{
  TItemSize_t x;    //x坐标
  TItemSize_t y;    //y坐标  
  TItemSize_t w;    //宽度  
  TItemSize_t h;    //高度    
}TItemsRect_t;

//项目焦点区域定义,焦点区高度固定为1
typedef struct{
  TItemSize_t x;     //x坐标
  TItemSize_t y;    //y坐标  
  TItemSize_t w;    //宽度,取正值    
}TItemsFocus_t;

/**********************************************************************
                         按键
**********************************************************************/
//键值//响应ASCII可找印字符,范围0-31,控制键定义为:
#define TGUI_KEY_BACKSPACE        20 
#define TGUI_KEY_TAB              21
#define TGUI_KEY_ENTER            22    //常用
  
#define TGUI_KEY_LEFT             16    //常用
#define TGUI_KEY_UP               17    //常用
#define TGUI_KEY_RIGHT            18    //常用
#define TGUI_KEY_DOWN             19    //常用
#define TGUI_KEY_PAGEUP           23
#define TGUI_KEY_PAGEDOWN         24
#define TGUI_KEY_SHIFT            25
#define TGUI_KEY_CONTROL          26
#define TGUI_KEY_ESCAPE           27
#define TGUI_KEY_INSERT           29
#define TGUI_KEY_DELETE           30

/**********************************************************************
                     内部使用的变量类型定义
**********************************************************************/

//当使用Edit控件数值类型时,定义所使用数值的最大数据类型(禁止为浮点数类型)
//当使用浮点数时,应等于浮点数占位
#if TGUI_EDIT_FLOAT_SUPPORT || TGUI_EDIT_INT32_SUPPORT
  #define TSIGNED_SIZE	4     //定义占位
  typedef TS32 TSigned_t;     //有符号数
  typedef TU32 TUnSigned_t;   //无符号数
#elif TGUI_EDIT_INT16_SUPPORT
  #define TSIGNED_SIZE	2     //定义占位
  typedef TS16 TSigned_t;     //有符号数
  typedef TU16 TUnSigned_t;   //无符号数
#else
  #define TSIGNED_SIZE	1     //定义占位
  typedef signed char   TSigned_t;     //有符号数
  typedef unsigned char TUnSigned_t;   //无符号数
#endif 

/**********************************************************************
                         相关共享函数
**********************************************************************/

//-----------------------由项总数得到占位函数-----------------------
unsigned char TGetItemsPlace(TItemSize_t Items);

//-----------------------得到两字符右对齐后的长度函数-----------------------
unsigned char TGetAlignLenR(unsigned char Len,     //原字符长度,<128
                          unsigned char Append); //附加字符长度,<128

//-----------------------得到两字符左对齐后的长度函数-----------------------
unsigned char TGetAlignLenL(unsigned char Len,     //原字符长度,<128,> 1
                          unsigned char Append); //附加字符长度,<128

//-----------------------转换为字符串-显示为最简函数----------------------
unsigned char TValue2StringMin(TSigned_t Value,
                            char *pString,//接收缓冲
                            unsigned char Min);//保证的最小位数

//-----------------------得到两字符对齐后的长度函数-----------------------
//当需要双字节对齐时,未对齐时在该位置填充' '
//返回填充后的缓冲
char *pTAlignString(TItemSize_t ItemLen,//字符长度
                   char *pString);//接收缓冲  

//------------------字符串扩展拷贝函数-----------------------
//1:响应结束与换行字符,
//2:返回结束的位置
char *Tstrcpy_ex(char *s1, const char *s2);


#endif
