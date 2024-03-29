/*****************************************************************************
                         TGUI上的输入法模块管理器总接口
注:暂不支持自带字库用要求全角对齐才能显示中文的LCD显示屏的显示
此模块结构可使用共享或动态内存，不负责输入法窗口的显示与隐藏！！！
*****************************************************************************/
#ifndef __T_IME_MNG_H
#define __T_IME_MNG_H

//定义当前版本(原版本或其分支见git)
#define TIME_MNG_STR_VERSION    "九宫格输入法 V1.10"

/*****************************************************************************
                         说明
*****************************************************************************/
//1.输入法所在TGUI层次关系
//  输入法是一个直接工作在TWM之上的一个高级控件，可认为与TWidget在
//  同一层次，但其功能不可配置(或仅需少量配置)且单一
//2.输入法模块构成:
//  由输入法管理器IME拼音输入法模块，数字输入法模块，大小写输入
//  法模块与符号输入法模块构成，
//3:输入法对TWin窗口的要求:
//  仅需一个独立的TWin窗口，窗口大小最小限定为128*64点，即4行16字，
//  最大要求为192*96点，即6行12字，当给出的TWin超过6行时将在
//  上下显示边界时，将先填充边界，未填充部分填充空格。
//4.

/*****************************************************************************
                          用户配置区
*****************************************************************************/
#include "TGUICfg.h"//窗口显示最大大小用户定义



//#define SUPPORT_TIME_APPEND_NOTE      //支持附加提示行时定义

#define TIME_CAPITAL_TIME_OV      4    //定义在符号输入法时，字符输入起时时间

//TIME显示窗口显示最大大小，注意预留边框位置(宽+4,高+2)
#ifndef TIME_MNG_MIX_W   //定义输入法显示字符宽度,含左右边界， 需>=16,建议<=44
  #define TIME_MNG_MIX_W    (16 + 4)  //默认为带边框最小显示单位
#endif
#ifndef TIME_MNG_MAX_H //定义输入法显示字符高度,含上下边界， 需>=4,建议<=14
  #define TIME_MNG_MAX_H    (4 + 2)   //默认为带边框最小显示单位 
#endif

/*****************************************************************************
                        相关宏定义
*****************************************************************************/
//用来定义输入法类型,具体数值内部已固定好
#define TIME_TYPE_PINYIN    0    //拼音输入法
#define TIME_TYPE_NUM       1    //数字输入法
#define TIME_TYPE_LOWERCASE 2    //小写字母输入法
#define TIME_TYPE_CAPITAL   3    //大写字母输入法
#define TIME_TYPE_SIGN      4    //符号输入法

//默认可使用全部输入法时的定义
#define TIME_TYPE_MASK    0xff


/*****************************************************************************
                        相关结构
*****************************************************************************/
#include "TImeEdit.h"
#include "TImeCapital.h"
#include "TImeNum.h"
#include "TImePinYin.h"
#include "TImeSign.h"

//用于存放各输入法内部数据结构
union _TImeUnion{
  struct _TImeCapital Capital;//大小写输入法
  struct _TImeNum Num;   //数字键输入法
  struct _TImePinYin PinYin;   //拼音输入法  
  struct _TImeSign Sign;   //符号输入法    
};

//TIme主结构
#include "TWin.h"
#include "TImeBase.h" //TIme_String_t;

struct _TImeMng{
  TWin_t *pWin;             //当前挂接的窗口
  struct _TImeEdit Edit;    //首行编辑器,含被编号字符串的相关信息
  union _TImeUnion Data;     //用于存放各输入法内部数据结构
  
  const char *pSignTbl;     //暂存带入的符号输入表
  unsigned char DispOffsetX;//在显示区x偏移
  unsigned char DispOffsetY;//在显示区y偏移
  unsigned char CapitalTimer; //大小写输入法定时定时器
  unsigned char w;          //输入法宽度(含边界),>=16,>18时自动显示左右边界
  unsigned char h;          //输入法高度(含边界),>=4,>=6时自动显示上下边界
  
  unsigned char Type;       //当前输入法类型
  unsigned char TypeMask;   //可使用的输入法类型 
  #ifdef SUPPORT_TIME_APPEND_NOTE
    unsigned char NoteTimer;
  #endif
  unsigned char Flag;       //相关标志,见定义
};

//其中,相关标志定义为:
#define TIME_PRV_TYPE_MASK 0x0f  //进入符号输入法时，暂存进入前输入法状态
#define TIME_FLAG_STATE    0x10  //标志当前正在输入法内部工作状态
#define TIME_FLAG_SIGN     0x20   //标志当前正在符号输入状态

/*****************************************************************************
                            相关函数
*****************************************************************************/

//----------------------------------初始化函数---------------------------------
//返回是否成功初始化
signed char TImeMng_Init(struct _TImeMng *pIme,  //带入的输入法结构缓冲
                         TWin_t *pWin,             //当前挂接的窗口
                         char *pString,            //用户已存在的字符串,必须以'\0'结尾
                         TIme_String_t Size,     //字符串缓冲区允许大小+1
                         unsigned char DefaultType,   //默认输入法,<4
                         unsigned char TypeMask,//可使用的输入法类型 
                         const char *pSignTbl, //挂接的符号表,为空时使用默认
                         TIme_String_t Cursor); //默认光标位置,-1时为最后

//----------------------------按键处理函数---------------------------------
//用户按数字键区与导航键区时调用此函数
//返回值定义为: 0正常状态,-1:退出键直接退出;-2:确认键退出
signed char TImeMng_Key(struct _TImeMng *pIme,
                        char Key);          //键值


//--------------------------任务函数---------------------------------
//在有输入法时调用此函数实现时间要求
void TImeMng_Task(struct _TImeMng *pIme);

//-----------------------------退出函数---------------------------------
//用户输入字符确认退出后调用此函数
void TImeMng_Quit(struct _TImeMng *pIme);

/*****************************************************************************
                            相关回调函数
*****************************************************************************/

//-----------------------------填充字符串颜色通报----------------------------
//此函数主要为填充箭头及其字符使用，可以用于着色
//形参ImeType: 0xff开机初始化时调吸入,0xfe退出时调用，
//             0xf0: 固定提示行着色前部， 0xf1: 固定提示行着色后部，
//             r0x80~0xc1: 附加提示行着色
//            其它为输入法内部着色: 0~2b输入法类型:3~6b定义为:
//拼音输入法时：3b: 0拼音选择状态 1汉字选择状态 4b: 0拼音行，1汉字行 
//符号输入法时：4b: 1此行被选择，0未被选择
void TImeMng_cbFullStrColor(unsigned char ImeType,//见说明
                            unsigned char y,       //pWin内y坐标
                             unsigned char x,       //pWin内x坐标
                             unsigned char xLen);   //x长度

#endif //#define __T_IME_MNG_H

