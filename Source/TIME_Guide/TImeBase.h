/*****************************************************************************
                         TGUI上输入法模块的基本函数或结构
*****************************************************************************/

#ifndef __TIME_BASE_H
#define __TIME_BASE_H


//定义编辑字符串字符串长度类型：
//unsigned char 时可编辑<255,unsigned shoft 时可编辑<32768
typedef unsigned char TIme_String_t;

//----------------内部使用的导航键值(GuideKey)定义(0-5)------------------
//顺序已固定
#define TIME_MNG_KEY_UP       0
#define TIME_MNG_KEY_LEFT     1
#define TIME_MNG_KEY_ENTER    2
#define TIME_MNG_KEY_RIGHT    3
#define TIME_MNG_KEY_DOWN     4
#define TIME_MNG_KEY_EXIT     5
#define TIME_MNG_KEY_COUNT    6

//---------------------------公共字符串-----------------------------------
extern const char chReturn[];// =  {"返回"};

//-------------------------GuideKey至无确认键时的键值---------------------------
extern const unsigned char TIme_GuideKeyToNoEnterKey[];



#endif //#define __TIME_BASE_H

