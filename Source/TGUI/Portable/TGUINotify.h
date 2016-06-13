/* ----------------------------------------------------------------------------
 *                TGUI消息通报模块
 * --------------------------------------------------------------------------*/

#ifndef __T_GUI_NOTIFY_H 
#define __T_GUI_NOTIFY_H

#include"TGUICfg.h"
/**********************************************************************
                     说明
**********************************************************************/
//TGUI使用直接使用回调函数处理消息通报,在控件初始化过程中将传入
//一个通报函数指针,适当的时候将调用该函数要求上层进行相应处理

//此模块用于编译器或单片机不支持或不能正确处理函数指针时的替换

//为实现支持或不支持函数指针间的兼容,完成一个回调需执行以下任务:
//1:在下列模块ID号分配里为该回调函数分配一个ID号
//2:使用TGUI_NOTIFY_PASS传道回调函数与上述分配的ID号
//3:在TGUINtify.c里实现ID号与回调函数的入口实现

//当调用一个回调函数时,使用TGUI_NOTIFY_RUN宏进行预处理

//-------------------------TGUI通报函数原型--------------------------
//返回:根据通报类型返回的数据
//const void* TGUINotify(void *pvRource,     //由谁发出的通报
//                       unsigned char Type, //通报类型
//                       //根据类型输入的数据通道
//                       const void *pvData);

//--------------------------按键通报类型定义为--------------------------------------
//0-127为系统通报预留:

//得到显示通报,pvData无效,返回数据头字符串
#define   TGUI_NOTIFY_GET_CAPTION   0
//得到内容通报,pvData指向当前所选项,返回该项字符串  
#define   TGUI_NOTIFY_GET_ITEM      1
//得到焦点通报,pvData指向当前被选项,应在适当位置处理焦点,返回值无效  
#define   TGUI_NOTIFY_GET_FOCUS     2
//确认键通报,pvData指向控件数据,返回值无效(内部返回绘图通报标志通知那些内容需重绘)  
#define   TGUI_NOTIFY_KEY_ENTER     3
//退出键通报,pvData指向控件数据,返回值无效
#define   TGUI_NOTIFY_KEY_ESCAPE    4  

//128-255为用户自定义通报类型

//--------------------------绘图通报类型定义为--------------------------------------
#define TGUI_PAINT_HCONTEXT  0x01    //内容水平方向变化
#define TGUI_PAINT_VCONTEXT  0x02    //内容垂直方向变化
#define TGUI_PAINT_FOCUS     0x04    //焦点发生变化
#define TGUI_PAINT_HEADER    0x08    //数据头发生变化
#define TGUI_PAINT_DATA      0x10    //数据发生变化

/**********************************************************************
                      回调函数ID号分配
//范围:0-255,按执行时间效率分配:,越告前执行效率越高
**********************************************************************/

//内部控件回调函数ID号分配,
#define   TGUI_CBFUNID_LISTBOX_EX   0
#define   TGUI_CBFUNID_TEXT         1
#define   TGUI_CBFUNID_CHECKBOX     2
#define   TGUI_CBFUNID_RADIO_BUTTON 3

//外部控件ID号分配:16开始:
#define   TGUI_CBFUNID_EX     4 //定义外部部ID开始位置

#define   TGUI_CBFUNID_TMENU_CHECKBOX         (TGUI_CBFUNID_EX + 0)
#define   TGUI_CBFUNID_TMENU_MNUM_LISTBOX     (TGUI_CBFUNID_EX + 1)
#define   TGUI_CBFUNID_TMENU_MNUM_EDIT        (TGUI_CBFUNID_EX + 2)
#define   TGUI_CBFUNID_TMENU_RADIO_BUTTON     (TGUI_CBFUNID_EX + 3)
#define   TGUI_CBFUNID_TMENU_SUB_MENU         (TGUI_CBFUNID_EX + 4)



/**********************************************************************
                         TGUI消息通报宏替换
**********************************************************************/
//考虑到多单片机或编译器对函数指针支持不够或根本不支持,使用下列宏实现连接:

//使用下列宏处理通报函数的传传递与执行:
#ifdef TGUI_DIS_P_FUN //禁止使用函数指针时
  //1:定义TGUI通报函数类型,用ID号类型替换通报函数:
  typedef unsigned char TGUINotify_t ;
  //2:传递通报函数的预处理,TGUINotifyFun为通报函数入口
  //ID为为每一个控件或(用户程序模块)分配的唯一编号
  #define TGUI_NOTIFY_PASS(TGUINotifyFun,ID) (ID) 
  //3:通报函数的执行宏
  #define TGUI_NOTIFY_RUN(TGUINotifyFun,pvRource,Type,pvData) \
    TGUIpFunReplace(pvRource,Type,pvData,TGUINotifyFun)
    
#else   //支持函数指针时
  //1:定义TGUI通报函数类型
  typedef const void*(*TGUINotify_t)(void *pvRource,     //由谁发出的通报
                                     unsigned char Type, //通报类型
                                     //根据类型输入的数据通道
                                     const void *pvData);
  //2:传递通报函数的预处理,TGUINotifyFun为通报函数入口
  //ID为为每一个控件或(用户程序模块)分配的唯一编号
  #define TGUI_NOTIFY_PASS(TGUINotifyFun,ID) (TGUINotifyFun) 
  //3:通报函数的执行宏
  #define TGUI_NOTIFY_RUN(TGUINotifyFun,pvRource,Type,pvData) \
    TGUINotifyFun(pvRource,Type,pvData)
#endif

//----------------TGUI消息传递不支持函数指针的替换函数------------------

#ifdef TGUI_DIS_P_FUN //禁止使用函数指针时
const void* TGUIpFunReplace(void *pvRource,     //由谁发出的通报
                            unsigned char Type, //通报类型
                            //根据类型输入的数据通道
                            const void *pvData,
                            TGUINotify_t ID);   // 回调函数ID号

#else //可以使用函数指针时,该函数无用了
#define TGUIpFunReplace(pvRource,Type,pvData,ID) do{}while(0)
#endif


#endif

