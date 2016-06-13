/* ----------------------------------------------------------------------------
 *                TMenu多值调整模式实现
 *因部分编译器显示方式不同,下列的窗口显示若不够清晰,请在其它文本编辑器里打开试试!
 * --------------------------------------------------------------------------*/

#ifndef __T_MENU_M_NUM_ADJ_H 
#define __T_MENU_M_NUM_ADJ_H

#include "TMenuBase.h"

//说明:
//此模块实现多值显示与调整
//显示示例为(若显示):
//  
//  ┌── 1002#设备参数设置──┐
//  │一次温度系数(k0): 0.1256  ↑  //浮点数调整模式
//  │    温度零点(℃): 12     ┃   //整数调整模式
//  │        功能掩码: 0b1001 ┃  //二进制数调整模式(暂不支持)
//  │        内部标识: 0xFD06 ┃  //十六进制数调整模式(暂不支持)
//  │   [保  存]    [返 回]    ↓
//  └←━━━━━━━━━━━→┘

//注:多值调整模式仅用于各种数值的调整，不能编辑与调整字符串

/**********************************************************************
                      配置区
**********************************************************************/

//定义复选按钮默认样式(见TWidget.h与复选控件):
#define   TM_M_NUM_ADJ_STYLE_DEFAULT    \
  (TWIDGET_EN_HEADER | TWIDGET_EN_VSCOLL | TWIDGET_EN_NOTIFY)

//注:可用的有:
//#define   TWIDGET_EN_FRAME    0x80  //该构件具有边框,当LC高度>-3行时此项有效
//#define   TWIDGET_EN_HEADER   0x40  //允许显示数据头
//#define   TWIDGET_EN_HSCOLL   0x20  //允许显示水平滚动条
//#define   TWIDGET_EN_VSCOLL   0x10  //允许显示垂滚动条
//#define   TWIDGET_EN_NOTIFY   0x04  //无滚动条与边框时允许在用户空时显示提示信息
//#define   TLISTBOXEX_EN_RETURN  0x02      //允许带返回字


/**********************************************************************
                     用户使用相关
**********************************************************************/

//当为多值调整模式时,菜单结构作如下定义:
//typedef struct _TMenu{
//  TMTYPE_MNUMADJ | TM_MNUMADJ_?? //菜单类型为多值调整模式模式与用户区标志
//  unsigned char Size;     //可供选择的调整项个数
//  LanguageCode_t *pgHeader; //菜单头,为NULL时从回调里读取
//  struct _TMenu *pParent;   //自已的父菜单
//  const LanguageCode_t*;   //存放每一个调整项的名称(结构)阵列
//  TMenuNotify_t Notify;     //与用户空间交互的通报函数
//}TMenu_t;

//菜单类型户区标志义为:
//数值调整模式,否则为显示模式
#define TM_MNUMADJ_WRITE     (0x01 << TMTYPE_USER_SHIFT)      
//循环调整模式,即此模式按左右键可在所在数据间循环切换,
//否则,按左右键时在调整项间切换,按确认时才进入调整模式
#define TM_MNUMADJ_LOOP_MODE  (0x02 << TMTYPE_USER_SHIFT)   
//调整模式时定义是否在确认键解释为右键，当用户无左右键时特别有效
#define TM_MNUMADJ_ENTER_KEY  (0x04 << TMTYPE_USER_SHIFT)  
//调整模式时定义创建时即进入调整模式
#define TM_MNUMADJ_CREATE_ENTER  (0x08 << TMTYPE_USER_SHIFT) 

//多数值的描述,由用户设定数据与固定配置(在MNumHeaderDesc里)构成
//这里是用户设定数据:
struct _MNumDesc{ //十进制，二进制，十六进制使用此结构
  unsigned char Flag;  //标志
  TSigned_t Min; //当前调整数的最小值
  TSigned_t Max; //当前调整数的最大值
};
//当为支持浮点数调整时,其描述结构等同于以下类型:
struct _MNumDescFloat{
  unsigned char Flag;  //标志
  float Min; //当前调整数的最小值
  float Max; //当前调整数的最大值
};

//可使用直接使用memcpy进行转换,也可使用此宏将struct _MNumDescFloat转换为
//struct _MNumDesc格式:
#define MNUM_DESC_FLOAT2NUM(pNumDesc, pDescFloat)\
do{\
  (pNumDesc)->Flag = (pDescFloat)->Flag;\
  (pNumDesc)->Min = Float2Data((pDescFloat)->Min);\
  (pNumDesc)->Max = Float2Data((pDescFloat)->Max);\
}while(0)
//直接使用时,应使用Float2Data转换数据,
//返回的浮点数使用Data2Float得到

//当为字符串调整类型时，使用struct _MNumDesc结构，但仅标志有效

//其中,标志定义为：
//0-2位表示小数点位置:
#define MNUM_DOT_POS_SHIFT   0
#define MNUM_DOT_POS_MASK    0x07
//3位表示调整模式,置1表示为群调整模式
#define MNUM_TYPE_GRP_MODE   0x08
//4-6位表示调整数模式:
#define MNUM_TYPE_SHIFT    4 
#define MNUM_TYPE_MASK     0x70
#define MNUM_TYPE_DEC      0x00   //十进制数模式
#define MNUM_TYPE_FLOAT    0x10   //浮点数模式
//#define MNUM_TYPE_HEX    0x20   //十六进制数模式
//#define MNUM_TYPE_BIN    0x30   //二进制数模式
//最高位为允许显示符号标志
#define MNUM_EN_SIGN      0x80

//用户区数据结构
struct _MNumAdjUser{ 
 //1:通知用户有数据被改变时,告知那项被改变了
 //2:需要读取当前调整数描述时,指明需要读取那一项
 //当前操作的项,保存时若有错误，则返回错误的项,其它情况禁止改写
 TItemSize_t CurItem;   
 struct _MNumDesc Desc; //调整模式时当前调整数的描述
 //当前调整数,送入初值,送出调整后的值(注意浮点数应使用Float2Data()或Data2Float()转换)
 TSigned_t Value[TMENU_M_NUM_ADJ_MAX_ITEM]; 
};


//用户回调函数响应类型及需实现的功能有:
//1:类型为:TM_NOTIFY_GET_DATA时,需按菜单里给定的顺序一项一项装入Value中
//2:类型为:TM_NOTIFY_CHANGED时,表示CurItem指定的Value项被改变了
//用户可根据应用要求决定是否响应(如:保存或直接更改应用)
//3:类型为:TM_NOTIFY_SET_DATA时,表示已修改完成,用户应进行保存等操作

//4:控件自定义类型:得到当前调整数描述:将CurItem指向的数据描述读取到Desc中
//将当前数据读入Value中
#define     TM_NOTIFY_MNUM_GET_DESC     (TM_NOTIFY_WIDGET_BASE + 0)

/**********************************************************************
                     内部数据结构
**********************************************************************/

//定义内部数据结构:
struct _MNumAdjData{
 TListboxEx_t  ListboxEx;       //结构缓存
 TEdit_t Edit;			//编辑框缓存
 unsigned char HeaderW;        //项的显示头宽度
 volatile unsigned char Flag;  //内部标志
 struct _MNumAdjUser User;    //用户数据
};

//------------------------创建函数实现----------------------------
//1:创建该类型菜单,从用户区获得数据并更新显示
//返回创建成功:
signed char TMenu_MNumAdjCreate(const TMenu_t *pMenu,    //指向的菜单系统
                                TWinHandle_t hWin,       //分配给菜单的窗口
                                unsigned char Data, //带入的自由数据,这里无意义
                                void *pv);      //可供使用的自由缓冲区

//------------------------响应按键函数实现----------------------------
//2:响应按键操作并根据情况与用户进行数据交互,并更新显示
//返回值定义为: 正值:进入相应子菜单,-1,不管,-2:返回上层菜单
signed char TMenu_MNumAdjKey(const TMenu_t *pMenu,    //指向的菜单系统
                             unsigned char Key,       //送入的键值
                             void *pv);      //已被使用的自由数据缓冲区

//-----------------------多值调整模式列框框通报回调实现函数------------------------
//仅供内部通报使用
const void*TMenu_MNumAdjListBoxNotify(void *pvPource,
                                      unsigned char Type,
                                      const void *pvData);

//-----------------编辑模式通报回调实现函数------------------------
//仅供内部通报使用
const void*TMenu_MNumAdjEditNotify(void *pvPource,
                                   unsigned char Type,
                                   const void *pvData);

/**********************************************************************
                     回调函数
**********************************************************************/

//----------------------申请一个TEdit窗口----------------------------
//TWinHandle_t cbTMenu_hGetEdit(TWinHandle_t hParent);
#define cbTMenu_hGetEdit(hParent) TM_hGetEditWin()

//----------------------得到当前显示样式-------------------------
#ifdef TMENU_STYLE_VARIABLE   //可变样式时在用户区定义
  unsigned char TMenu_MNumAdj_cbGetCurStyle(void);
#else
  #define TMenu_MNumAdj_cbGetCurStyle() \
    (TM_M_NUM_ADJ_STYLE_DEFAULT | TLISTBOXEX_EN_RETURN)
#endif

//----------------------当前保存字标志-------------------------
//在修改模式时，可决定是否有保存字TCHECKBOX_EN_SAVE
#ifdef TMENU_STYLE_VARIABLE   //可变样式时在用户区定义
  unsigned char TMenu_MNumAdj_cbGetCurSaveStyle(void);
#else
  #define TMenu_MNumAdj_cbGetCurSaveStyle() TCHECKBOX_EN_SAVE
#endif
  
#endif
