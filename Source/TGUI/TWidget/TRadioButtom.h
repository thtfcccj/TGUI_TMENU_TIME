/* ----------------------------------------------------------------------------
 *                单选框控件实现
 *因部分编译器显示方式不同,下列的窗口显示若不够清晰,请在其它文本编辑器里打开试试!
 * --------------------------------------------------------------------------*/


//说明:
//RadioButtom不同于常用GUI仅是一个单选按钮,这里应叫做单选框控件,
//单选框控件已是一个完整的用户模块,如下图所示示例:
//第一屏
//┌────工作模式─────┐
//│  　　 1.○单测模式   　　┼
//│ 　　　2.○复测模式     　↓
//└←━━━━━━━━━━━→┘
//最后一屏(含返回附加字时):
//┌────工作模式─────┐
//│  　　 3.●自动模式   　　↑
//│ 　　　   [返 回]       　┼
//└←━━━━━━━━━━━→┘
//或(含保存附加字时,集中保存):
//┌────工作模式─────┐
//│  　　 3.●自动模式   　　↑
//│   [保  存]    [返 回]    ┼
//└←━━━━━━━━━━━→┘

//图中 :
//←━━━→为水平滚动条,当显示区超过5行时可以有垂直滚动条
//[返 回]为按钮,光标移动到上面可直接返回
//●表示当前选择项,○表示未选择该项

//继承关系:
//KRadioButtom继承关系为:
//  KRadioButtom->TListBoxEx->TListBox->TWidget->(间接继承)TWin
//直接继承时可直接将该结构转换为基类结构后并调用基类相关函数
//间接继承通过该结构提供的间接转函数数转换为基类结构并可调用基类结构函数
//
//因控件已包装好,且基类无特殊说明外(注释带★标志),
//基类直接操作不当可能会影响系统工作,故不建议直接使用基类操作

#ifndef __T_RADIO_BUTTOM_H 
#define __T_RADIO_BUTTOM_H

/**********************************************************************
                  内部结构
**********************************************************************/
#include "TListboxEx.h"

typedef struct{
  TListboxEx_t ListboxEx;       //基类
  TItemSize_t Radio;            //保存那项被选择了
  TGUINotify_t cbGUINotify;    //用户通报函数
}TRadioButtom_t;

//单选框在Flag里附加有两个自定义可选功能:
#define TRADIO_BUTTOM_EN_SAVE     0x01      //允许带保存字
#define TRADIO_BUTTOM_EN_RETURN   0x02      //允许带返回字
//注:当同时带两个字时,Offset的最高位用于表示焦点在保存(0)还是返回(1)上面

//通报函数应响应下列类型通报:
//TGUI_NOTIFY_GET_CAPTION 获得标题
//TGUI_NOTIFY_GET_ITEM 若得项显示内容
//TGUI_NOTIFY_KEY_ENTER 每一项的确认键,此时应检查选中与否并进行应用层处理
//若项超过设定希数,表示在附加的保存焦点上
//TGUI_NOTIFY_EKY_ESCAPE 退出到上一层


/**********************************************************************
                          成员函数
**********************************************************************/
//------------------------得到那项被选中了----------------------------
//返回0表示未选中,否则表示选中
//TItemSize_t TRadioButtom_GetRadio(TRadioButtom_t *pRadioButtom);
#define TRadioButtom_GetRadio(pRadioButtom) ((pRadioButtom)->Radio)


//------------------------预设选中状态----------------------------
//返回0表示未选中,否则表示选中
//void TRadioButtom_SetRadio(TRadioButtom_t *pRadioButtom,
//                           TItemSize_t Sel);
#define TRadioButtom_SetRadio(pRadioButtom,Sel) \
  do{(pRadioButtom)->Radio = Sel;}while(0)


/**********************************************************************
                          行为函数
**********************************************************************/

//-------------------------初始化函数-----------------------------
//初始化后预置第一项为选择状态,重置选择状态后应再重画窗口
//注:考虑很多系统仅使用静态内层分配,故将创建函数留给更高一层,即
//可在此基础上立真正的创建函数
void TRadioButtom_Init(TRadioButtom_t *pRadioButtom,  //由用户分配的存储空间
                     TWinHandle_t hWin,         //已建立并初始化好的窗口
                     TItemSize_t Items,         //预定义项目总数
                     TItemSize_t ItemW,         //预定义项宽度(即项的最宽度)
                     unsigned char Flag,        //控件标志
                     TGUINotify_t cbGUINotify); //通报函数

//-------------------------窗口重画所有函数-------------------------
//以此为基础的应用程序(非继承类)初始化完成后调用此函数重画所有
#define  TRadioButtom_PaintAll(pRadioButtom) \
  do{ListBox_PaintAll((TListbox_t*)(pRadioButtom));}while(0)


//-------------------------按键处理函数-----------------------------
//void TRadioButtom_Key(TRadioButtom_t *pRadioButtom,unsigned char Key);
//直接基类实现:
#define TRadioButtom_Key(pRadioButtom,Key)\
  do{TListboxEx_Key((TListboxEx_t*)pRadioButtom,Key);}while(0)

//-------------------------单选框回调实现函数-----------------------------
//仅供内部通报使用
const void*TRadioButtom_Notify(void *pv,
                               unsigned char Type,
                               const void *pvData);

#endif

