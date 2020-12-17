/*******************************************************************************

                           屏幕显示之电子表格单元实现

*******************************************************************************/

#include <String.h>

#include "TListbox.h"
#include "TScrnExcel.h"

#include "key.h"
/**********************************************************************
                       模块内部
**********************************************************************/
//-------------------------TListBoxEx通报函数实现-------------------------
static const void* _Callback(void *pvSource,
                             unsigned char Type,
                             const void *pvData);

//-------------------------初始化函数-------------------------
//注：当作为子窗口显示时，应让空白窗口作为其父窗口
void TScrnExcel_Init(struct _TScrnExcel *pExcel,
                       const struct _TScrnExcelData *pData, //用户数据
                       TWinHandle_t   hMain,          //主窗口所在窗口
                       TWinHandle_t   hHeader,        //标题头所在窗口
                       unsigned char Cfg)           //配置相关
{
  memset(pExcel, 0, sizeof(struct _TScrnExcel));  //初始化到0
  pExcel->pData = pData; 
  pExcel->hMain = hMain;
  pExcel->hHeader = hHeader;
  pExcel->Flag = Cfg;

  //显示标题头并得到长度
  const char *pHeader = pData->pGetHeader(pData);
  TWin_SetString(TWM_pGetWin(hHeader),0,pHeader);
  
  //显示主界面
  TListboxEx_Init(&pExcel->TListboxEx,         //由用户分配的存储空间
                  hMain,                       //已建立并初始化好的窗口
                  pData->GetItemCount(pData),  //预定义项目总数
                  strlen(pHeader),             //字符最大宽度以标题头为准
                  TWIDGET_EN_RETURN,           //控件标志
                  TLISTBOXEX_ALIGN_LEFT | TLISTBOXEX_ANTI_ROW,//显示样式
                  _Callback); //通报函数
  
  //通报需要更新数据
  if(pData->UpdateData(pData, 
                       TListboxEx_GetY(&pExcel->TListboxEx),
                       TListboxEx_GetH(&pExcel->TListboxEx))){
    pExcel->Flag |= TSCRN_EXCEL_DATA_FINAL;
    //数据更新完成，再更新显示
    TScrnExcel_ReBulid(pExcel);
  }
}

//-------------------------响应按键函数-------------------------
//返回是否退出(非0退出)
int TScrnExcel_Key(struct _TScrnExcel *pExcel, unsigned char Key)
{
  //无信息时只响应退出键
  if((Key != TGUI_KEY_ESCAPE) && (!TListboxEx_GetItems(&pExcel->TListboxEx)))
    return 0;
  
  unsigned short PrvSel = TListboxEx_GetSel(&pExcel->TListboxEx);  
  
  //直接交给其处理按键，在回调里实现转移
  TListboxEx_Key(&pExcel->TListboxEx,Key);

  if(PrvSel != TListboxEx_GetSel(&pExcel->TListboxEx)){//Y轴移动了，需更新用户数据
    TScrnExcel_ReBulid(pExcel);
    return 0;
  }
  
  const struct _TScrnExcelData *pData = pExcel->pData;
  //左右移动同步标题栏，
  if(((Key == TGUI_KEY_LEFT) || (Key == TGUI_KEY_RIGHT))){
    TWin_SetString(TWM_pGetWin(pExcel->hHeader), 0,
                   pData->pGetHeader(pData) + 
                   TListboxEx_GetX(&pExcel->TListboxEx));
  }
  //进入键后继处理
  else if(pExcel->Flag & TSCRN_EXCEL_IN_ENTER){ //检查进入菜单
    pExcel->Flag &= ~TSCRN_EXCEL_IN_ENTER;
    if(pData->EnterKey != NULL){
      if(pData->LineToAryId != NULL) PrvSel = pData->LineToAryId(pData, PrvSel);//转换
      pData->EnterKey(PrvSel);      
    }  
  }
  
  //退出键后继处理
  else if(pExcel->Flag & TSCRN_EXCEL_IN_RETURN){
    pExcel->Flag &= ~TSCRN_EXCEL_IN_RETURN;
    if(pData->ExitKey != NULL) pData->ExitKey();
    return -1;
  }
  return 0;
}

//-------------------------更新数据完成通报-------------------------------------
//延后更新数据时，调用此函数说明数据更新完成
void TScrnExcel_UpdateDataFinal(struct _TScrnExcel *pExcel)
{
  pExcel->Flag |= TSCRN_EXCEL_DATA_FINAL;
  const struct _TScrnExcelData *pData = pExcel->pData;
  //可能项长度改变了
  TListboxEx_SetItems(&pExcel->TListboxEx, pData->GetItemCount(pData));
  TScrnExcel_Refurbish(pExcel); //刷新内容
}

//-------------------------重建函数-------------------------------------
//重新更新当页所有数据并显示
void TScrnExcel_ReBulid(struct _TScrnExcel *pExcel)
{
  const struct _TScrnExcelData *pData = pExcel->pData;
  
  //更新数据完成
  if(pData->UpdateData(pData, 
                    TListboxEx_GetY(&pExcel->TListboxEx),
                    TListboxEx_GetW(&pExcel->TListboxEx))){
    //可能项长度改变了
    TListboxEx_SetItems(&pExcel->TListboxEx, pData->GetItemCount(pData));
    TScrnExcel_Refurbish(pExcel); //刷新内容
  }
  else{//更新数据未完成
    pExcel->Flag &= ~TSCRN_EXCEL_DATA_FINAL;
  }
}

//--------------------刷新函数------------------------------
void TScrnExcel_Refurbish(const struct _TScrnExcel *pExcel)
{
  const struct _TScrnExcelData *pData = pExcel->pData;
  //更新标题头
  TWin_SetString(TWM_pGetWin(pExcel->hHeader), 0,
                 pData->pGetHeader(pData) + 
                   TListboxEx_GetX(&pExcel->TListboxEx));
  TListBoxEx_PaintAll(&(pExcel)->TListboxEx);
}

//-------------------------TListBoxEx通报函数实现-------------------------
//通报函数应响应下列类型通报:
//TGUI_NOTIFY_GET_CAPTION 获得标题
//TGUI_NOTIFY_GET_ITEM 若得项显示内容
//TGUI_NOTIFY_KEY_ENTER 每一项的确认键,此时应检查选中与否并进行应用层处理
//若项超过设定希数,表示在附加的保存焦点上
//TGUI_NOTIFY_EKY_ESCAPE 退出到上一层
static const void* _Callback(void *pvSource,
                             unsigned char Type,
                             const void *pvData)
{
  struct _TScrnExcel *pExcel = pcbGetExcel((TListboxEx_t *)pvSource); 
  TItemSize_t Item = *(TItemSize_t*)pvData;   //当前项位置
  const struct _TScrnExcelData *pData = pExcel->pData;
  
  //得到系统当前指向的数据缓冲区
  switch(Type){
  //case TGUI_NOTIFY_GET_CAPTION://获得标题
  //  break;
  case TGUI_NOTIFY_GET_ITEM:  //获得项显示内容
    return pData->pGetLine(pData, Item);
  case TGUI_NOTIFY_KEY_ENTER://每一项的确认键
    pExcel->Flag |= TSCRN_EXCEL_IN_ENTER;//进入指定层,后继处理防止堆栈过多
    break;
  case TGUI_NOTIFY_KEY_ESCAPE://返回按键
    if(pExcel->Flag & (TSCRN_EXCEL_EN_EXIT | TSCRN_EXCEL_FORCE_EXIT))
      pExcel->Flag |= TSCRN_EXCEL_IN_RETURN;
    else //返回到第一项
      TListboxEx_SetSel((TListboxEx_t *)pvSource,0);
    break;
  default: break;
  }
  return NULL;
}

//-----------------------得到当前被选项对应的阵列ID-------------------------
unsigned short Excel_GetAryId(const struct _TScrnExcel *pExcel)
{
  const struct _TScrnExcelData *pData = pExcel->pData;
  unsigned short Sel = TScrnExcel_GetSel(pExcel);
  if(pData->LineToAryId == NULL) return Sel; //对应关系
  return pData->LineToAryId(pData, Sel);
}

//----------------------跳转到指定AryId位置函数-------------------------
//返回0成功
signed char TScrnExcel_JumpToAryId(struct _TScrnExcel *pExcel,
                                    unsigned short AryId)
{
  const struct _TScrnExcelData *pData = pExcel->pData;
  unsigned short Line;
  if(pData->AryIdToLine == NULL) Line = AryId; //不支持时，则一一对应
  else Line = pData->AryIdToLine(pData, AryId);
  if(Line >= TScrnExcel_GetItemCount(pExcel)) //异常(不支持时不在此页)或暂未执行
    return -1;

  //置光标并更新当前位置
  TListboxEx_SetSel(&pExcel->TListboxEx, Line);
  return 0;
}
