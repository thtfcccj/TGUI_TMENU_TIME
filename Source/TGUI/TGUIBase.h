/* ----------------------------------------------------------------------------
 *                TGUI基本构件
 * --------------------------------------------------------------------------*/

#ifndef __T_GUI_BASE_H 
#define __T_GUI_BASE_H

#include "TGUICfg.h"      //TGUI配置
#include "TGUINotify.h"   //TGUI消息通报

/**********************************************************************
                         相关结构定义
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
                         相关共享函数
**********************************************************************/

//-----------------------由项总数得到占位函数-----------------------
unsigned char GetItemsPlace(TItemSize_t Items);

//-----------------------得到两字符右对齐后的长度函数-----------------------
unsigned char GetAlignLenR(unsigned char Len,     //原字符长度,<128
                          unsigned char Append); //附加字符长度,<128

//-----------------------得到两字符左对齐后的长度函数-----------------------
unsigned char GetAlignLenL(unsigned char Len,     //原字符长度,<128,> 1
                          unsigned char Append); //附加字符长度,<128

//-----------------------转换为字符串-显示为最简函数----------------------
unsigned char Value2StringMin(TSigned_t Value,
                            char *pString,//接收缓冲
                            unsigned char Min);//保证的最小位数

//-----------------------得到两字符对齐后的长度函数-----------------------
//当需要双字节对齐时,未对齐时在该位置填充' '
//返回填充后的缓冲
char *pAlignString(TItemSize_t ItemLen,//字符长度
                   char *pString);//接收缓冲  

//------------------字符串查找函数-----------------------
//由C++库中提出,若string库中有,则需将其注释掉
//const char *strchr(const char *s, char ch);

//------------------字符串扩展拷贝函数-----------------------
//1:响应结束与换行字符,
//2:返回结束的位置
char *strcpy_ex(char *s1, const char *s2);


#endif
