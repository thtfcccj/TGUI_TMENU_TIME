/*******************************************************************************

                  TMenu菜单之命令模式接口
命令模式只占一个普通的子菜单项,当用户选择该项功能时,将执行相应的操作
*******************************************************************************/
#ifndef __T_MENU_CMD_H 
#define __T_MENU_CMD_H 

/**********************************************************************
                     用户使用相关
**********************************************************************/
//当为命令模式时,菜单结构作如下定义:
//typedef struct _TMenu{
//  TMTYPE_CMD;                //菜单类型为命令模式
//  unsigned char Size;       //个数为0
//  LanguageCode_t *pgHeader; //菜单头,为NULL时从回调里读取
//  struct _TMenu *pParent;   //自已的父菜单,没有时为NULL
//  const TMenu_t*;           //为空
//  TMenuNotify_t Notify;     //与用户空间交互的通报函数,可以为空
//}TMenu_t;

//此模式无用户区标志

//用户区数据结构:无:

//当定义有菜单头时,无回调函数
//当菜单头定义为NULL时需定义回调函数,响应获得菜单头信息

/**********************************************************************
                     内部数据结构
**********************************************************************/
//命令模式依附在子菜单模式上,在子菜单模式的回调函数里实现该项命令(详
//见子菜单模式回调函数说明)



#endif
