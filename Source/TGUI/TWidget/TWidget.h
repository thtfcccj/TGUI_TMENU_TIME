/* ----------------------------------------------------------------------------
 *                TGUI窗口基本部件对外
*此模块基类:TWIN(名柄)
 * --------------------------------------------------------------------------*/

#ifndef __T_WIDGET_H 
#define __T_WIDGET_H

//窗口说明见TWindgetNote.h

//此控件在TWin基础上构建,主要实现了的用户内容区(ItemsRect)与
//用户内容剪切区,并根据标志具有相关的特性
//此控件不具有与应用程序前的交互功能,需以此构件为基类,构建更高层
//面的控件才能供用户使用.

//继承关系:
//TWidget继承关系为:
//  TWidget->(间接继承)TWin
//直接继承时可直接将该结构转换为基类结构后并调用基类相关函数
//间接继承通过该结构提供的间接转函数数转换为基类结构并可调用基类结构函数
//
//因控件已包装好,且基类无特殊说明外(注释带★标志),
//基类直接操作不当可能会影响系统工作,故不建议直接使用基类操作

//附加说明:
//每个控件具有以下属性:
//1:可以具有边框(窗口高度>=4时);
//2:可以具有显示头(窗口高度>=3时);
//3:可以具有水平(窗口宽度>=5时)与垂直滚动条指示:
//  (1): 滚动区高度<=4,有操作方向指示
//  (2): 滚动区高度>4,有滚动条指示
//4:每个控件在所选项内均具有焦点

//所有应用控件均以此控件为基类实现
//显示的内容及焦点位置由继承此结构的控件实现


/**********************************************************************
                      配置区
**********************************************************************/

      
/**********************************************************************
                         关连库
**********************************************************************/
#include "TWM.h"     //窗口管理器
#include "TGUIBase.h"    //基本数据

/**********************************************************************
                  内部结构
**********************************************************************/

typedef struct{
  TWinHandle_t hWin;     //所在窗口,因窗口为共享结构,故呈引用关系  
  //项区域空间,表示内内所占显示空间,在窗口内不能够显示时,将采用分页显示的方式
  //其中x,y表示窗口向左,向坐标,w为项内容最长的值,h表示项总数
  TItemsRect_t ItemsRect;

  TItemSize_t Sel;        //所选项
  unsigned char Flag;    //标志,见初始化函数声明中的定义
}TWidget_t;

/**********************************************************************
                         成员操作函数
**********************************************************************/
//--------------------由TWin名柄继承得到TWin结构函数---------------------
//★得到窗口名柄:
#define TWidget_hGetWin(pWidget) ((pWidget)->hWin)

//★因TWidget是通过Win句柄方式间接继承TWin类,所有控件需通过下列宏实现转换:
#define TWidget_pGetWin(pWidget) (TWM_pGetWin((pWidget)->hWin))
//-------------------项空间操作--------------------------
//★得到当前所选
#define TWidget_GetSel(pWidget) ((pWidget)->Sel)
//★设置当前所选
void TWidget_SetSel(TWidget_t *pWidget,TItemSize_t  Sel);
//★得到项总数,即项高度
#define TWidget_GetItems(pWidget) ((pWidget)->ItemsRect.h)
//设置项总数,供在控件初始化后允许使有
void TWidget_SetItems(TWidget_t *pWidget,TItemSize_t Items);
//★得到项在用户剪切区的起始位置x
#define TWidget_GetX(pWidget) ((pWidget)->ItemsRect.x)
//★得到项长度,即项宽度
#define TWidget_GetW(pWidget) ((pWidget)->ItemsRect.w)
//★预设项长度,即项宽度
void  TWidget_SetW(TWidget_t *pWidget,TItemSize_t w);
//★得到项在用户剪切区的起始位置y
#define TWidget_GetY(pWidget) ((pWidget)->ItemsRect.y)

//---------------用户剪切区域操作--------------------------
//★得到用户剪切区绝对坐标x
unsigned char TWidget_GetClipX(TWidget_t *pWidget);
//★得到用户剪切区绝对坐标y
unsigned char TWidget_GetClipY(TWidget_t *pWidget);
//★得到水平页大小,即显示窗口宽度w 
unsigned char TWidget_GetHPageSize(TWidget_t *pWidget);
//得到含允许提示外的水平页大小,,即不含提示的显示窗口宽度w 
unsigned char TWidget_GetHPageSizeEnNotify(TWidget_t *pWidget);
//★得到垂直页大小,即显示窗口高度h
unsigned char TWidget_GetVPageSize(TWidget_t *pWidget);

//------------------------其它------------------------------
//★得到标志
#define TWidget_GetFlag(pWidget) ((pWidget)->Flag)
//★得到用户标志
#define TWidget_GetUserFlag(pWidget) ((pWidget)->Flag & 0x03)
//设置用户标志
void TWidget_SetUserFlag(TWidget_t *pWidget,unsigned char Flag);

/**********************************************************************
                          相关函数
**********************************************************************/

//--------------------------控制初始化函数-------------------------
//创建其它控件时调用此函数初始化基类
//此函数自动休正标志区标志,并将项显示区域置为允许在窗口内的最大宽度
//初始化为至少两个项,置认在0项
void TWidget_Init(TWidget_t *pWidget,
                  TWinHandle_t hWin,
                  unsigned char Flag);

//其中,Flag标志定义为:
#define   TWIDGET_EN_FRAME    0x80  //该构件具有边框,当LC高度>-3行时此项有效
#define   TWIDGET_EN_HEADER   0x40  //允许显示数据头
#define   TWIDGET_EN_HSCOLL   0x20  //允许显示水平滚动条
#define   TWIDGET_EN_VSCOLL   0x10  //允许显示垂滚动条
#define   TWIDGET_EN_RETURN   0x08  //允许回环标志
#define   TWIDGET_EN_NOTIFY   0x04  //当无滚动条与边框时允许在用户空时显示提示信息
//用户可自定义使用的标志区 
#define   TWIDGET_USER       0x03

//--------------------------大小改变通报函数-------------------------
//当改变TWidget_t对应的Win大小改变时,用于自动改变显示样式
void TWidget_WinReSizeNotify(TWidget_t *pWidget,
                             unsigned char w,
                             unsigned char h);

//-----------------------按键处理函数---------------------------
//上下键在项间切换,左右键水平移动窗口,上下翻页键翻页,数字键选择项目
//返回标志位定义为置位时发生变化(见绘图通报)
//注:当返回0时,表示该按键当前处理无效
unsigned int TWidget_Key(TWidget_t *pWidget,unsigned char Key);

/**********************************************************************
                          相关画图函数
**********************************************************************/
//控制画图功能使用说明:
//本控件无主动绘图功能,以此为基类的控件必须实现画图函数,如:
//static void _Paint(TListbox_t *pListbox,    //具体控件
//                   unsigend char PaintFlag) //绘制标志,见按键处理函数返回定义
//在该处理函数中,为提高绘图效率,原则上应根据绘制标志按下列顺序进行绘图处理:
//1:若头改变,则调用TWidget_PaintHeader绘制头
//2:若内容(水平与垂直方向)发生变化,应调用TWidget_pGetPaintRow并在缓冲区绘制内容
//3:若焦点发生变化,应根据应用程序调用TWidget_SetSetFocus设置焦点显示
//4:将内容水平与垂直方向变化的标志送入TWidget_PaintEnd,画图结束

//下列情况需重绘控件:
//1:在控制创建完成后,应将绘图标志置为0xff以便重绘所有
//2:在按键处理结束后,将返标志与用户可能的绘制标志带入重绘
//3:用户在可能的情况下,手工重绘相关部分


//-------------------★画标题头或上边框函数----------------------------
//此函数画上侧边框,含标题头
void TWidget_PaintHeader(TWidget_t *pWidget,
                        const char *pHeader,   //显示头,无时为NULL
                        unsigned char Len);   //显示头数据长度,若为0,则应含结符字符

//---------------------★得到行缓冲区函数---------------------------
//此函数画左侧边框,并输出行缓冲位置及长度供外部程序填充
//若返回空,表示不可接收字符
//注:可接收数据长度可调用TWidget_pGetPaintRowLen()得到
//项字符起始位置可调用TWidget_pGetPaintRowStringStart()得到
//起始显示项可调用TWidget_pGetPaintRowItemStart()得到
char *TWidget_pGetPaintRow(TWidget_t *pWidget,
                           TItemSize_t Item,    //需要显示的项
                           TItemSize_t PrvLen); //上一行用户字符长度

//---------------------★得到行填充长度函数------------------------
//unsigned char TWidget_GetPaintRowLen(TWidget_t *pWidget);
#define  TWidget_GetPaintRowLen(pWidget) TWidget_GetHPageSize(pWidget)

//---------------------★得到字符起始位置函数------------------------
#define TWidget_GetPaintRowStringStart(pWidget) ((pWidget)->ItemsRect.x)

//---------------------★得到项起始位置函数------------------------
#define TWidget_GetPaintRowItemStart(pWidget) ((pWidget)->ItemsRect.y)


//---------------------★设置焦点区函数------------------------
//此函数负责设置焦点(一般被选项获得焦点)
void TWidget_SetFocus(TWidget_t *pWidget,
                      TItemSize_t y,     //在项空间纵向位置
                      TItemSize_t x,     //在项空间横向位置,>项空间时将不显示焦点
                      TItemSize_t w);     //焦点宽度,0表示x至整个项长度全为焦点

//---------------------★结束画图函数---------------------------
//此函数负责画右侧与下侧边框
void TWidget_PaintEnd(TWidget_t *pWidget,
                      unsigned char PaintFlag,//定义的绘图标志
                      TItemSize_t LastW);  //最后一行数据长度



#endif

