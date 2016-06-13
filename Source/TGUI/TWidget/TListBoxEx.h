/* ----------------------------------------------------------------------------
 *                列表框扩展控件
 *因部分编译器显示方式不同,下列的窗口显示若不够清晰,请在其它文本编辑器里打开试试!
 * --------------------------------------------------------------------------*/

//控件说明:
//如下所示为列表框扩展控件演示:
//  ┌──(可选显示头)───┐
//  │  　　1.(第一项)      ┼
//  │ 　　 2.(第二项)      │
//  │   [确定]    [返回]   ↓
//  └←━━━━━━━━━→┘

//除具有列表框基本控件功能外,增加了以下特性和显示样式:
//1: 文本对齐方式可配置
//2: 焦点(反显位置)显示方式可配置
//3: 可增加前后缀,并可决定前后缀是否带焦点
//4: 可以在内容前面自动增加序号显示功能
//5: 最后一栏用户附加功能(具体由用户决定显示和需完成的功能)

//继承关系:
//TListBoxEx继承关系为:
//  TListBoxEx->TListBox->TWidget->(间接继承)TWin
//直接继承时可直接将该结构转换为基类结构后并调用基类相关函数
//间接继承通过该结构提供的间接转函数数转换为基类结构并可调用基类结构函数
//
//因控件已包装好,且基类无特殊说明外(注释带★标志),
//基类直接操作不当可能会影响系统工作,故不建议直接使用基类操作

#ifndef __T_LISTBOX_EX_H 
#define __T_LISTBOX_EX_H

/**********************************************************************
                  内部结构
**********************************************************************/
#include "TListbox.h"

typedef struct{
  TListbox_t Listbox;           //基类
  unsigned char Style;         //样式
  unsigned char Flag;           //相关内部标志,见定义
  TItemSize_t  ValidW;          //有效字符宽度(即不含对齐过程的产生的空格)
  TGUINotify_t cbGUINotify;    //扩展通报函数
}TListboxEx_t;

//通报函数应响应下列类型通报:
//TGUI_NOTIFY_GET_CAPTION,TGUI_NOTIFY_GET_ITEM
//TGUI_NOTIFY_KEY_ENTER,TGUI_NOTIFY_EKY_ESCAPE
//当有附加项时,焦点留给通报函数处理:
//TGUI_NOTIFY_GET_FOCUS
//其中,若带有附加时,TGUI_NOTIFY_GET_ITEM项超限表示获得附加行内容
//附加行内容整行不受样式控制,由回调函数决定

//注:当附加行不只一个焦点时,应注意按键处理程序的预先处理

//此控件自定义通报为:
#define   TLISTBOXEX_NOTIFY_PREFIX   128    //增加前缀通报
#define   TLISTBOXEX_NOTIFY_SUFFIX   129    //增加后缀通报
//通报函数应响应自定义通报

//--------------------------文本样式------------------------
//其中,样式定义为:
//文字对齐方式:(0-1bit)
#define   TLISTBOXEX_ALIGN_MASK      0x03    //屏蔽位
#define   TLISTBOXEX_ALIGN_LEFT      0x00    //左对齐
#define   TLISTBOXEX_ALIIGN_CENTER   0x01    //局中对齐
#define   TLISTBOXEX_ALIGN_RIGHT     0x02    //右对齐
#define   TLISTBOXEX_ALIGN_LEN       0x03    //项长度对齐,即以最长项为标志对齐
//反显方式:(2-3)
#define   TLISTBOXEX_AUTI_MASK       0x0c    //屏蔽位
#define   TLISTBOXEX_ANTI_ROW        0x00    //整行反显
#define   TLISTBOXEX_ANTI_WORD       0x04    //仅文字反显,包含前后缀
#define   TLISTBOXEX_ANTI_WORD2      0x08    //仅文字反显,但不包含前后缀
#define   TLISTBOXEX_AUTI_DIS        0x0c    //禁止焦点显示
//其它标志
#define   TLISTBOXEX_PREFIX          0x10     //增加前缀,由通报区获得
#define   TLISTBOXEX_SUFFIX          0x20     //增加后缀,由通报区获得
#define   TLISTBOXEX_EN_NO           0x40    //允许序号显示
#define   TLISTBOXEX_EN_APPEND       0x80    //允许增加附加项
//注:当有且Flag保存或返回字未使能时,可重写附回项,此时应在附加
//项回调函数中,用户应负责实现焦点,按键与显示的响应

//--------------------------标志------------------------
//在Flag里附加有两个自定义可选功能,当此项功能使能时,
#define TLISTBOXEX_EN_SAVE     0x01      //允许带保存字
#define TLISTBOXEX_EN_RETURN   0x02      //允许带返回字

/**********************************************************************
                          属性函数
**********************************************************************/
//★得到当前所选项
#define TListboxEx_GetSel(pListbox) TWidget_GetSel((TWidget_t*)(pListbox))
//设置当前所选项,仅在创建后使用
#define TListboxEx_SetSel(pListbox,Sel) \
  do{TWidget_SetSel((TWidget_t*)(pListbox),Sel);}while(0)
//★得到项总数
//#define TListboxEx_GetItems(pListbox) TWidget_GetItems((TWidget_t*)(pListbox))
TItemSize_t TListboxEx_GetItems(TListboxEx_t *pListboxEx);
//重新设置项总数
#define TListboxEx_SetItems(pListbox,Items)\
  TWidget_SetItems(((TWidget_t*)(pListbox)),Items)
//★得到项宽度(含占位符)
#define TListboxEx_GetW(pListbox) TWidget_GetW((TWidget_t*)(pListbox))
//判断是否有附加项
#define TListboxEx_HaveAppend(pListbox) ((pListbox)->Style & TLISTBOXEX_EN_APPEND)
/**********************************************************************
                          行为函数
**********************************************************************/

//-------------------------初始化函数-----------------------------
//注:考虑很多系统仅使用静态内层分配,故将创建函数留给更高一层,即
//可在此基础上立真正的创建函数
void TListboxEx_Init(TListboxEx_t *pListboxEx,  //由用户分配的存储空间
                     TWinHandle_t hWin,         //已建立并初始化好的窗口
                     TItemSize_t Items,         //预定义项目总数
                     TItemSize_t ItemW,         //字符最大宽度
                     unsigned char Flag,        //控件标志
                     unsigned char Style,       //显示样式
                     TGUINotify_t cbGUINotify); //通报函数

//-------------------------窗口重画所有函数-------------------------
//以此为基础的应用程序(非继承类)初始化完成后调用此函数重画所有
//注：当文件样式为长度对齐时，TListboxEx继承类初始化完成后需调用此函数显示
#define TListBoxEx_PaintAll(pListboxEx) \
  do{ListBox_PaintAll((TListbox_t *)(pListboxEx));}while(0)


//-------------------------按键处理函数-----------------------------
void TListboxEx_Key(TListboxEx_t *pListboxEx,unsigned char Key);

//-------------------------列表框回调实现函数-----------------------------
//仅供内部通报使用
const void*TListBox_Notify(void *pv,
                           unsigned char Type,
                           const void *pvData);
#endif

