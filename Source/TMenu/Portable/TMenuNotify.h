/* ----------------------------------------------------------------------------
 *                TMenu用户消息通报模块
 * --------------------------------------------------------------------------*/

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
//当菜单系统不能获得项(pv == NULL)时,从函数据得到项数据
//此时，通报函数pv形参指向的第一个数据即为为指定项
#define     TM_NOTIFY_GET_ITEM    5

//控件扩展通报起始,
//通报类型64-127为控件自身扩展的通报,控制间通报类型一般可重复可以重复
#define     TM_NOTIFY_WIDGET_BASE  64

//用户扩展通报起始
//通报类型128-255为部件自定义通报,由用户决定且一般不可以重复
#define     TM_NOTIFY_USER_BASE    128


/**********************************************************************
                      回调函数ID号分配
//范围:0-255,按执行时间效率分配:,越告前执行效率越高
**********************************************************************/

//内部控件回调函数ID号分配,
#define   TMENU_CBFUNID_NULL_MENU           0		//空ID号,内部保留,为子菜单等不需要回调函数时使用
#define   TMENU_CBFUNID_TOP_MENU            1   //顶层菜单通报
#define   TMENU_CBFUNID_SET_TIME            2   //时间设置
#define   TMENU_CBFUNID_SET_POINT_GLOBAL    3   //测点全局参数设置
#define   TMENU_CBFUNID_SEL_POINT           4   //测点选择
#define   TMENU_CBFUNID_POINT_ITEM_MENU     5   //测点项目选择
#define   TMENU_CBFUNID_SET_POINT_PARA      6   //测点参数设置
#define   TMENU_CBFUNID_POINT_HIDDEN_MENU   7   //测点屏蔽设置
#define   TMENU_CBFUNIDPOINT_STATUS_PARA    8   //测点状态
#define   TMENU_CBFUNID_SYS_INFO            9   //系统信息
#define   TMENU_CBFUNID_REC_MENU            10   //记录菜单
#define   TMENU_CBFUNID_REC_MENU_PER        11   //记录各自菜单
#define   TMENU_CBFUNID_SYS_PARA            12   //系统参数设置菜单
#define   TMENU_CBFUNID_IN_SPECTRUM         13   //进入光谱模式菜单
#define   TMENU_CBFUNID_SET_PASS            14   //修改当前密码
#define   TMENU_CBFUNID_VALID_CHANNEL       15   //有效通道设置
#define   TMENU_CBFUNID_GET_POWER           16   //获得权限
#define   TMENU_CBFUNID_FORCE_ACT           17   //强制动作
#define   TMENU_CBFUNID_HAND_ALARM          18   //手动报警
#define   TMENU_CBFUNID_SET_SLAVE           19   //手动报警
#define   TMENU_CBFUNID_POINT_USED_MENU     20   //手动报警




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
#endif

//----------------TMenu消息传递不支持函数指针的替换函数------------------

#ifdef TMENU_DIS_P_FUN //禁止使用函数指针时

void TMenupFunReplace(unsigned char Type,//通报类型
                      //根据菜单类型输入的输入输出数据区
                      //该区域结构在控件里定义,直接指针转换即可
                      void *pv,
                       TMenuNotify_t ID);   // 回调函数ID号

#else //可以使用函数指针时,该函数无用了

#define TMenupFunReplace(Type,pv,ID) do{}while(0)

#endif


#endif

