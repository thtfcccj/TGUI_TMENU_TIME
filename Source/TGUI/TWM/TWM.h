/*******************************************************************************

                           TWin窗口管理器模块接口

*******************************************************************************/
#ifndef __TWM_H 
#define __TWM_H

/*******************************************************************************
                           说明
*******************************************************************************/
//此模块界于窗口与需实例化的窗口管理器之间,为控件层面的一个虚拟接口
//用户应有根据应用进行具体窗口管理与分配的窗口管理器模块,该模块主要负责以下工作:
//1:负责系统窗口的分配和获得,
//2:通过名柄的方式获得或释放一个具体的窗口
//3:重新设置窗口的显示优先级(可先实现)

//窗口管理器还应实现以下功能:
//1:实现TWIN中的窗口更新通报函数,并记住被更新的窗口范围
//2:与底层LCD驱动连接,并负责更新已被更新部分数据
//注:因在窗口内已有需更新的数据,故不需回调上层被动更新.

//以窗口(名柄)为基类,可在该窗口上构建控件,该控件应完成下列功能:
//1:负责更新该窗口的显示数据
//2:暂无

#include "TGUICfg.h"
#include "TWin.h"

/**********************************************************************
                          虚拟<->实例基本接口
**********************************************************************/
#include "TWinMng_Row.h"  //在Row中的实现时(暂只支持此一种)

//------------------------定义句柄类型实现连接----------------------
//typedef TWin_t*       TWinHandle_t;
typedef unsigned char TWinHandle_t; //

//-----------------------由窗口句柄得到窗口------------------------
//TWin_t* TWM_pGetWin(TWinHandle_t);
#define  TWM_pGetWin(TWinHandle) (pCurTWinMng->pTWinAry + TWinHandle)

//-----------------------获得一个窗口----------------------------
//TWinHandle_t TWM_NewHandle(unsgined char Pri);  //可选的窗口优先级
#define  TWM_NewHandle(Pri) (Pri)        //固定优先级,由用户程序分配

//-------------------------释放一个窗口-------------------------------
//void TWM_pFreeHandle(TWinHandle_t);  //可选的窗口优先级
//固定优先级,不需要释放,隐藏即可
#define  TWM_FreeHandle(TWinHandle) \
  do{TWin_Hidden(TWM_pGetWin(TWinHandle));}while(0)       

/**********************************************************************
                          虚拟<->实例高级接口
**********************************************************************/
//动态窗口显示优先级时应实现下列接口

//----------------将某个窗口置为某窗口的父窗口---------------------
//注:将此窗口置为父窗口时,将成为该父窗口的子窗口中的一份子
//void TWM_SetParent(TWinHandle_t Parent,
//                   TWinHandle_t Child);
#define  TWM_SetParent(Parent,Child) do{}while(0) //不实现

//----------------得到某个窗口的父窗口-----------------------------
//TWinHandle_t TWM_GetParent(uTWinHandle_t);
#define  TWM_GetParent(Child) do{}while(0) //不实现

//----------------得到某个窗口的首个子窗口-------------------------
//即得到长子
//TWinHandle_t TWM_Get1stChild(uTWinHandle_t);
#define  TWM_Get1stChild(Parent) do{}while(0) //不实现

//----------------得到某个窗口置的后向同胞窗口----------------------
//TWinHandle_t TWM_GetNextSibling(uTWinHandle_t);
#define  TWM_GetNextSibling(TWinHandle) do{}while(0) //不实现

//--------------------将某个窗口置为顶层显示------------------------
//void TWM_SetTop(uTWinHandle_t);
#define  TWM_SetTop(TWinHandle) do{}while(0) //不实现

//--------------------将某个窗口置为底层显示------------------------
//void TWM_SetBottom(uTWinHandle_t);
#define  TWM_SetBottom(TWinHandle) do{}while(0) //不实现


#endif //#ifndef __TWM_H 

