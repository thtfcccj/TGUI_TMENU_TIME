/**************************************************************************

             TMenu用户消息通报模块

**************************************************************************/
#ifndef __T_MENU_NOTIFY_H 
#define __T_MENU_NOTIFY_H 

#include"TMenuCfg.h"
/**********************************************************************
                     说明
**********************************************************************/
//TMenu使用直接使用回调函数处理消息通报,在TMenu_t结构中直接嵌入
//一个通报函数指针,适当的时候将调用该函数与用户空间交互

//此模块可用于编译器或单片机不支持或不能正确处理函数指针时的替换

//为实现支持或不支持函数指针间的兼容,完成一个回调需执行以下任务:
//1:若支持函数指针,则菜单结构直接嵌入回调函数即可,否则用户需为该菜单结构
//指定一个唯一的编号(在下列回调函数ID号分配中统一分配)。
//3:在TMenuNotify里实现ID号与回调函数的入口对应

//当调用一个回调函数时,使用TMenu_NOTIFY_RUN宏进行预处理

//-------------------------TMenu通报函数原型--------------------------
//返回:根据通报类型返回的数据
//void  TMenuNotify(unsigned char Type,//通报类型
//                  //根据菜单类型输入的输入输出数据区
//                  //该区域结构在控件里定义,直接指针转换即可
//                  void *pv);

//--------------------------通报类型定义--------------------------------------
//其中:通报类型0-63为通用通报,定义为:
//根据菜单类型从用户空间获得数据
#define     TM_NOTIFY_GET_DATA    1   
//用户根据菜单类型从菜单系统获得数据进行后继处理(保存等)
#define     TM_NOTIFY_SET_DATA    2 
//通知有数据被改变
#define     TM_NOTIFY_CHANGED     3
//当菜单系统不能获得菜单头(phHeader == NULL)时,从函数据得到菜单头
//通过通报函数pv形参获得，必须是一个完整字符
#define     TM_NOTIFY_GET_HEADER  4
//当菜单系统不能获得项(pv == NULL)时,从用户空间获得到项数据
//此时，通报函数pv形参指向的第一个数据即为为(已查表)指定项
#define     TM_NOTIFY_GET_ITEM    5

//通报光标改变，以用于在RPC客户端中，提前准备要进入菜单的内容
//pv为, 首个参数(TItemSize_t型)即为新的光标位置(现只响应子菜单模式)
#define     TM_NOTIFY_CUSOR_CHANGED  6

//(已查表)获得子菜单的菜单头,在RPC客户端中,子菜单可能不完整，故可能不能直接得到菜单名称
//故先调用以从用户空间获取,用户不处理时才调用子菜单里的菜单头
//pv为需填充的字符串指针，首个参数(unsigned char)即为(查表后)菜单位置
#define     TM_NOTIFY_GET_SUB_HEADER  7

//不查表获得子菜单的菜单头，除不查表外，其它同TM_NOTIFY_GET_SUB_HEADER
//此功能主要用于阵列项时，用一项替代菜单列表(需查找表指向同一项配合)。
#define     TM_NOTIFY_GET_SUB_HEADER_ORG  8

//控件扩展通报起始,
//通报类型64-127为控件自身扩展的通报,控制间通报类型一般可重复可以重复
#define     TM_NOTIFY_WIDGET_BASE  64

//用户扩展通报起始
//通报类型128-255为部件自定义通报,由用户决定且一般不可以重复
#define     TM_NOTIFY_USER_BASE    128

/**********************************************************************
                         TMenu消息通报宏替换
**********************************************************************/
//考虑到多单片机或编译器对函数指针支持不够或根本不支持,使用下列宏实现连接:

//使用下列宏处理通报函数的传传递与执行:
#ifdef TMENU_DIS_P_FUN //禁止使用函数指针时
  //1:定义TMenu通报函数类型,用ID号类型替换通报函数:
  typedef unsigned char TMenuNotify_t ;
  //2:传递通报函数的预处理,TMenuNotifyFun为通报函数入口
  //ID为为每一个控件或(用户程序模块)分配的唯一编号
  #define TMENU_NOTIFY_PASS(TMenuNotifyFun,ID) (ID) 
  //3:通报函数的执行宏
  #define TMENU_NOTIFY_RUN(TMenuNotifyFun,Type,pv) \
    TMenupFunReplace(Type,pv,TMenuNotifyFun)
    
#else   //支持函数指针时
  //1:定义TMenu通报函数类型
  typedef void(*TMenuNotify_t)(unsigned char Type,//通报类型
                              //根据菜单类型输入的输入输出数据区
                              //该区域结构在控件里定义,直接指针转换即可
                              void *pv);
  //2:传递通报函数的预处理,TMenuNotifyFun为通报函数入口
  //ID为为每一个控件或(用户程序模块)分配的唯一编号
  #define TMENU_NOTIFY_PASS(TMenuNotifyFun,ID) (TMenuNotifyFun) 
  //3:通报函数的执行宏
  #define TMENU_NOTIFY_RUN(TMenuNotifyFun,Type,pv) \
    TMenuNotifyFun(Type,pv)
  
#endif //TMENU_DIS_P_FUN

//----------------TMenu消息传递不支持函数指针的替换函数------------------

#ifdef TMENU_DIS_P_FUN //禁止使用函数指针时

void TMenupFunReplace(unsigned char Type,//通报类型
                      //根据菜单类型输入的输入输出数据区
                      //该区域结构在控件里定义,直接指针转换即可
                      void *pv,
                       TMenuNotify_t ID);   // 回调函数ID号

#else //可以使用函数指针时,该函数无用了

#define TMenupFunReplace(Type,pv,ID) do{}while(0)

#endif //TMENU_DIS_P_FUN


#endif

