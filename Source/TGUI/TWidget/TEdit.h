/* ----------------------------------------------------------------------------
 *                TGUI的编辑框
*此模块基类:TWIN(名柄)
 * --------------------------------------------------------------------------*/

#ifndef __T_EDIT_H 
#define __T_EDIT_H

//说明:
//TEdit是一个在TWIN之上的行编辑器,以字符串为基本显示单元

/**********************************************************************
                      配置区
**********************************************************************/

      
/**********************************************************************
                         关连库
**********************************************************************/
#include "TWM.h"     //窗口管理器
#include "TGUIBase.h"    //基本数据

/**********************************************************************
                         成员操作函数
**********************************************************************/

//编辑框调整数类型定义:
enum TEditType_t{
  TEditType_Dec,    //十进制数模式
  TEditType_Float,  //浮点数模式
  //TEditType_Hex,    //十六进制数模式
  //TEditType_Bin,    //二进制数模式
  //注：字符横式不在此编辑器里实现
};


typedef struct _TEdit{
  TWinHandle_t hWin;      //窗口
  enum TEditType_t eType;	  //调整模式
  unsigned char Pos;      //光标位置
  unsigned char Flag;     //标志,见定义
  //调整数描述
  unsigned char DotPos;	  //小数点的位置
  unsigned char Len;      //最大调整位数
  TSigned_t Value;       //当前调整数
  TSigned_t Max;          //调整数最大值,
  TSigned_t Min;          //调整数最小值
  TGUINotify_t cbGUINotify; //通报函数
}TEdit_t;

//其中标志定义为:
//配置:
#define TEDIT_EN_SIGN     0x80    //允许显示正号,数值调整时
#define TEDIT_GRP_MODE    0x40    //群调整模式,否则为单个调整模式
#define TEDIT_EN_NOTIFY   0x20    //是否允许提示<>显示,如:<-XXX.XX>
#define TEDIT_ENTER_RIGHT 0x10    //定义是否将确认键解释为右键,当系统无左右键时可用于数值移位
#define TEDIT_PASS_IN     0x08    //字符串编辑模式时,为密码显示模式,即显示*

//需响应的系统通报:
//TGUI_NOTIFY_KEY_ENTER  确认并退出
//#define   TGUI_NOTIFY_KEY_ESCAPE  //不确认退出
//此控件自定义通报为:
#define   TEDIT_NOTIFY_LEFT_END   128    //到达左边界通报,传出调整后的值
#define   TEDIT_NOTIFY_RIGHT_END  129    //到达右边界通报,传出调整后的值
//通报函数应响应自定义通报

/**********************************************************************
                         成员操作函数
**********************************************************************/
//★得到附在的窗口
#define TEdit_hGetWin(pEdit)  ((pEdit)->hWin)
//★得到标志
#define TEdit_GetFlag(pEdit)  ((pEdit)->Flag)
//★判断是否在群调整模式
#define TEdit_IsGrpMode(pEdit)  ((pEdit)->Flag & TEDIT_GRP_MODE)
/**********************************************************************
                          相关函数
**********************************************************************/

//-----------------编辑器十进制数初始化函数----------------
void TEditDec(TEdit_t *pEdit,
              TWinHandle_t hWin,    //依赖的窗口,数据缓冲区>Len
              TSigned_t Value,            //初始化值
              TSigned_t Max,              //最大值,决定数值长度
              TSigned_t Min,              //最小值
              unsigned char DotPos,  //小数点位置,可用于用整数模拟小数显示
              unsigned char Flag,  //相关配置
              TGUINotify_t cbGUINotify); //通报函数

//----------------得到调整后的数函数--------------------
//获得当前已调整完成的数(浮点数需使用Data2Float)转换得到
TSigned_t TEdit_GetVolume(const TEdit_t *pEdit);

//-----------------编辑器字符串编辑初始化函数----------------
void TEditString(TEdit_t *pEdit,
              TWinHandle_t hWin,    //依赖的窗口,数据缓冲区>Len
              char *pString,        //需编辑的字符串
              unsigned char Len,    //字符串可编辑长度
              unsigned char Flag,  //相关配置
              TGUINotify_t cbGUINotify); //通报函数

//-----------------------按键处理函数------------------------------
void TEdit_Key(TEdit_t *pEdit,unsigned char Key);

//-------------------------画图函数--------------------------------
void TEdit_PaintAll(TEdit_t *pEdit);

#endif

