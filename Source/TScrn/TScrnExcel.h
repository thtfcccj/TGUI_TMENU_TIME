/*******************************************************************************

                           屏幕显示之电子表格单元
 此模块在TGUI之上，实现一种通用表格形式的界面
 界面由左测固定栏和右侧可滚动栏组成，其中双分为标题和工作表格
 数据与显示分离，界面需要的数据通过回调函数实现
 支待延时更新功能
*******************************************************************************/

#ifndef __TSCRN_EXCEL_H 
#define __TSCRN_EXCEL_H

/*******************************************************************************
                            相关结构
*******************************************************************************/
#include "TGUI.h"

//-------------------------------数据及操作函数结构------------------------------
struct _TScrnExcelData{
  //挂接的用户数据内容
  void *pData;
  //确认键回调函数,形参为阵列ID,无时为NULL
  void (*EnterKey)(unsigned short AryId);
  //退出回调函数,无时为NULL，此时退出键执行固定回调 
  void (*ExitKey)(); 
  //得到标题头 
  const char*(*pGetHeader)(const struct _TScrnExcelData*);
  //得到数据行总数
  unsigned short(*GetItemCount)(const struct _TScrnExcelData*);      
  //得到行数据,返回NULL表示结束
  const char*(*pGetLine)(const struct _TScrnExcelData*,
                         unsigned short Line); //指定行   
  //更新数据(可实现为查找表项),返回是否更新完成
  signed char (*UpdateData)(const struct _TScrnExcelData*,
                             unsigned short StartLine, //起始行 
                             unsigned short Count);    //总数
  //当行位置与具体要表达的阵列位置为映射关系时,相互转换函数,若为对应关系定义为NULL
  //返回0xffff表示未找到
  unsigned short(*LineToAryId)(const struct _TScrnExcelData*,
                                unsigned short Line); //指定行转换为阵列ID
  unsigned short(*AryIdToLine)(const struct _TScrnExcelData*,
                                unsigned short AryId); //阵列ID转换为指定行
};

//---------------------------------主结构---------------------------------------
struct _TScrnExcel{
  //用户相关：
  const struct _TScrnExcelData *pData;  //挂接的用户数据内容及操作函数
  
  TListboxEx_t TListboxEx;        //工作窗口缓冲
  TWinHandle_t   hMain;           //主窗口所在窗口 
  TWinHandle_t   hHeader;         //标题头所在窗口
  unsigned char Flag;           //相关标志，见定义
};

//-相关标志定义为：
//用户配置位:
#define   TSCRN_EXCEL_CFG_MASK      0x07
//允许显示滚动条
#define   TSCRN_EXCEL_EN_SCROLLBAR  0x01
//退出键功能定义为自动退出-电子表格不是主界面时定义此
#define   TSCRN_EXCEL_EN_EXIT      0x02
//当退出键不定义为自动退出时,按退出键时强制执行回调函数
//此功能用于不定义为退出键时,用退出键返回任意位置
#define   TSCRN_EXCEL_FORCE_EXIT    0x04

//菜单进入标志(内部使用)
#define   TSCRN_EXCEL_IN_ENTER     0x80
//菜单返回标志(内部使用)
#define   TSCRN_EXCEL_IN_RETURN    0x40
//数据准备完成标志,当数据不能及时更新时，此标志用于同步(内部使用)
#define   TSCRN_EXCEL_DATA_FINAL   0x20


/**********************************************************************
                       相关函数
**********************************************************************/
//-------------------------初始化函数-------------------------
//注：当作为子窗口显示时，应让空白窗口作为其父窗口
void TScrnExcel_Init(struct _TScrnExcel *pExcel,
                       const struct _TScrnExcelData *pData, //用户数据
                       TWinHandle_t   hMain,                 //主窗口所在窗口
                       TWinHandle_t   hHeader,               //标题头所在窗口
                       unsigned char Cfg);                  //配置相关

//-----------------------------显示滚动条相关-------------------------------
#define TScrnExcel_EnScrollBar(pExcel) \
  do{(pExcel)->Cfg |= TSCRN_EXCEL_EN_SCROLLBAR;}while(0)
#define TScrnExcel_DisScrollBar(pExcel)\
  do{(pExcel)->Cfg &= ~TSCRN_EXCEL_EN_SCROLLBAR;}while(0)

//-------------------------响应按键函数-------------------------
//返回是否退出(非0退出)
int TScrnExcel_Key(struct _TScrnExcel *pExcel, unsigned char Key);

//-------------------------得到当前被选项-------------------------
#define TScrnExcel_GetSel(excel) TListboxEx_GetSel(&excel->TListboxEx)

//-----------------------得到当前被选项对应的阵列ID-------------------------
unsigned short Excel_GetAryId(const struct _TScrnExcel *pExcel);

//---------------------得到光标位置函数------------------------------
#define TScrnExcel_GetCursor(pexcel) TListboxEx_GetSel(&(pexcel)->TListboxEx)

//---------------------得到垂直页大小函数------------------------------
#define TScrnExcel_GetVPage(pexcel) TListboxEx_GetV((&(pexcel)->TListboxEx))

//----------------------------得到项总数函数------------------------------
#define TScrnExcel_GetItemCount(pExcel) TListboxEx_GetItems(&(pExcel)->TListboxEx)

//-------------------------更新数据完成通报-------------------------------------
//延后更新数据时，调用此函数说明数据更新完成
void TScrnExcel_UpdateDataFinal(struct _TScrnExcel *pExcel);

//-------------------------重建函数-------------------------------------
//重新更新当页所有数据并显示
void TScrnExcel_ReBulid(struct _TScrnExcel *pExcel);

//-----------------------------刷新函数---------------------------------
void TScrnExcel_Refurbish(const struct _TScrnExcel *pExcel);
    
//----------------------跳转到指定AryId位置函数-------------------------
void TScrnExcel_JumpToAryId(struct _TScrnExcel *pExcel,
                            unsigned short AryId);

/**********************************************************************
                       回调函数
**********************************************************************/

//----------------------由TListboxEx_t得到当前结构-------------------------
struct _TScrnExcel *pcbGetExcel(const TListboxEx_t *); 

#endif

