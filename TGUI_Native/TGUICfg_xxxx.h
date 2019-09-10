/*******************************************************************************

              TGUI用户相关配置模块-产品相关部分在xxxx中的定义

*******************************************************************************/
#ifndef __T_GUI_CFG_xxxx_H 
#define __T_GUI_CFG_xxxx_H

/**********************************************************************
                         参数配置区
**********************************************************************/

//定义是否双字节对齐,部分LCD要求双字对齐才能正确显示数
//#define       TGUI_WORD_ALIGN

#include "UiBase.h"

//显示屏宽字符数,8个点一个高度,<=128
//注：没有定义双字节对齐功能时,应值应+1用于支持全角时可显示半个字符
#define       TLCD_WIDTH      UI_BASE_LINE_STR_COUNT    //显示屏宽字符数,8个点一个高度,<=128
//显示屏高字符数,16个点一个高度,<=128
#define       TLCD_HIGH       (UI_BASE_LINE_COUNT - 3) //不含状态栏的三行   

//定义左右键水平移动窗口的大小，字节为单位
#define       TGUI_HORI       6  

//定义是否禁止函数指针传递机制,因很多单片机或编译器对函数指针支持
//不够(如:keil C51)或根本不支持,禁止后,TGUI将采用另类方式实现回调
//#define       TGUI_DIS_P_FUN

//定义控件里面的项目占位,当有控件字符>255或项>255时,应定义为unsigned short
//否则定义为unsigned char
typedef unsigned short TItemSize_t;
#define	TITEM_SIZE_T    2        //定义占位

//定义使用的复选框最大项数
#define   TCHECKBOX_ITEM_COUNT      32

//定义TGUI类型占位
#define	TU16	unsigned short
#define	TS16	signed short
#define	TU32	unsigned long
#define	TS32	signed long

/**********************************************************************
                             功能配置区
**********************************************************************/

//---------------------禁止未使用到的控件------------------------------
//可禁止系统重未使用的控件以节省空间:
//禁止后,未使用的控件不在项目里包含相应C文件即可
//可禁止的控件有:
#define   TGUI_DIS_TEXT    	   //禁止文本框
//#define   TGUI_DIS_CHECKBOX        //禁止复选框
//#define   TGUI_DIS_RADIO_BUTTON    //禁止单选框
//#define   TGUI_DIS_EDIT    	   //禁止编辑框

//---------------------定义Edit控件支持的可调整数类型--------------------
//未使用置0,使用置非0
#define      TGUI_EDIT_FLOAT_SUPPORT	1 //浮点数支持,最多显示位数7位
//整形数支持:选择最高支持位数		
#define      TGUI_EDIT_INT8_SUPPORT 	1 //8位数支持,调整范围+-127
#define      TGUI_EDIT_INT16_SUPPORT	1 //16位数支持,调整范围+-32768
#define      TGUI_EDIT_INT32_SUPPORT	1 //32位数支持,调整范围+-9999999

#define      TGUI_EDIT_HEX_SUPPORT	    0 //十六进制数调整支持,支持位数内整形数支持确定
#define      TGUI_EDIT_BIN_SUPPORT	    0 //二进制数支持,支持位数内整形数支持确定 


#endif //#ifndef __T_GUI_CFG_xxxx_H 

