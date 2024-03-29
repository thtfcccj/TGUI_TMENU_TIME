/*****************************************************************************

               TGUI上的输入法模块(导航输入法)管理器总接口
此模块结构可使用共享或动态内存，不负责输入法窗口的显示与隐藏！！！
*****************************************************************************/
#ifndef __T_IME_MNG_H
#define __T_IME_MNG_H

/*****************************************************************************
                          用户配置区
*****************************************************************************/

//定义当前版本(原版本或其分支见git)
//此软件模块采用MIT协议， 请在您在你设备适当位置放置此字符串的显示
#define TIME_MNG_STR_VERSION    "导航输入法 V1.20"

//导航输入法时的全局定义，可供外部使用以区分如带入的符号等
//#define SUPPORT_IME_GUIDE

#define TIME_MNG_DISP_W    (240 / 8)  //定义输入法显示字符宽度
#define TIME_MNG_DISP_H    (128 / 16)  //定义输入法显示字符高度

/*****************************************************************************
                        相关宏定义
*****************************************************************************/
//用来定义输入法类型,具体数值内部已固定好
#define TIME_MNG_TYPE_PINYIN    0    //拼音输入法
#define TIME_MNG_TYPE_NUM       1    //数字输入法
#define TIME_MNG_TYPE_LOWERCASE 2    //小写字母输入法
#define TIME_MNG_TYPE_CAPITAL   3    //大写字母输入法
#define TIME_MNG_TYPE_SIGN      4    //符号输入法

//默认可使用全部输入法时的定义
#define TIME_TYPE_MASK    0x1f

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
  struct _TImeCapital Capital;       //大写输入法
  struct _TImeNum Num;               //数字键输入法
  struct _TImePinYin PinYin;         //拼音输入法  
  struct _TImeSign Sign;             //符号输入法
};

//TIme主结构
#include "TWin.h"
#include "TImeBase.h" //TIme_String_t;

struct _TImeMng{
  TWin_t *pWin;                //当前挂接的窗口
  //无顺序要求的：
  struct _TImeEdit Edit;    //首行编辑器,含被编号字符串的相关信息
  union _TImeUnion Data;     //用于存放各输入法内部数据结构
  const char *pSignTbl;     //暂存带入的符号输入表
  
  unsigned char DispOffsetX;//在显示区x偏和移
  unsigned char DispOffsetY;//在显示区y偏和移
  
  unsigned char Type;       //当前输入法类型
  unsigned char TypeMask;   //可使用的输入法类型  
  unsigned char State;       //内部工作状态，见定义:
};

//内部工作状态，定义为:
#define TIME_MNG_STATE_IDIE        0   //空闲模式,未使用时为此模式
#define TIME_MNG_STATE_EDIT        1   //编辑模式
#define TIME_MNG_STATE_FORCE_EXIT  2   //强制退出提示
#define TIME_MNG_STATE_SEL         3  //输入法选择模式
#define TIME_MNG_STATE_IME         4  //输入法模式

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
//返回值定义为: 0正常状态,-1:退出键直接退出;-2:确认键退出
signed char TImeMng_Key(struct _TImeMng *pIme,
                        char Key);          //键值,仅响应上下左右进出6键

//--------------------------任务函数---------------------------------
//在有输入法时调用此函数实现时间要求,调用周期128ms
void TImeMng_Task(struct _TImeMng *pIme);

//-----------------------------退出函数---------------------------------
//用户输入字符确认退出后调用此函数
void TImeMng_Quit(struct _TImeMng *pIme);

/*****************************************************************************
                            相关回调函数
*****************************************************************************/

//-----------------------------填充字符串颜色通报----------------------------
//此函数主要为填充箭头及其字符使用，可以用于着色
//形参GuideKey: 0xff开机初始化时调吸入,0xf3退出时调用，否则为按键值
void TImeMng_cbFullStrColor(unsigned char GuideKey,//见说明
                            unsigned char y,       //pWin内y坐标
                             unsigned char x,       //pWin内x坐标
                             unsigned char xLen);   //x长度



#endif //#define __T_IME_MNG_H

