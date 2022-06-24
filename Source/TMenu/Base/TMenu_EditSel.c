/*******************************************************************************

                  TMenu菜单之编辑选择模式实现
注:这里只实现显示,调整通过子模块实现
*******************************************************************************/
//在扩展列表框用前后缀实现显示功能:
//  当为不可编辑时,调整项提示在内容中,数值在后缀中,可实现查看切换
//  当为编辑选择时,调整项提示在前缀中,数值在内容中,可实现数值调整切换

#include "TMenu.h"
#include "TMenu_EditSel.h"
#include "TGUIMath.h"
#include <string.h>

//其中:标志定义为:
#define _FLAG_ENTER    0x08       //按保存键
#define _FLAG_RETURN    0x10      //置此标志认为是退出上一层

//------------------------得到项头字符串函数----------------------------
static char *_pGetItemHeader(const TMenu_t *pMenu,
                             TItemSize_t CurItem,
                             unsigned char HeaderW)    //指向的菜单系统
{
  //数据右对齐,最后加": "
  const char *pHeader = TM_GetSubMenuHeader(pMenu,CurItem);
  char *pStrStart = TMenu_pGetBuf();
  if(pHeader == pStrStart) pStrStart += (strlen(pHeader) + 1);//防止使用缓存重复
  
  unsigned char Start = HeaderW - (strlen(pHeader) + 2);
  memset(pStrStart, ' ', Start);
  char *pString = pStrStart + Start;
  pString = Tstrcpy_ex(pString,pHeader);
  *pString++ = ':';
  *pString++ = ' ';
  *pString = '\0';
  return pStrStart;
}

//------------------------得到项数据函数----------------------------
static const char *_pGetItem(const TMenu_t *pMenu,//指向的菜单系统
                             TItemSize_t CurItem,
                             struct _EditSelData *pEditSelData)   
{
  //得到用户区数据字符串
  TItemSize_t PrvItem = pEditSelData->User.CurItem;
  pEditSelData->User.CurItem = CurItem;
  TMENU_NOTIFY_RUN(pMenu->cbNotify, TM_NOTIFY_GET_DATA, &pEditSelData->User);
  pEditSelData->User.CurItem = PrvItem;
  return pEditSelData->User.pData;
}

//通报函数应响应下列类型通报:
//TGUI_NOTIFY_GET_CAPTION 获得标题
//TGUI_NOTIFY_GET_ITEM 若得项显示内容
//TGUI_NOTIFY_KEY_ENTER 每一项的确认键,此时应检查选中与否并进行应用层处理
//若项超过设定希数,表示在附加的保存焦点上
//TGUI_NOTIFY_EKY_ESCAPE 退出到上一层
//-----------------------编辑选择模式列框框通报回调实现函数------------------------
//仅供内部通报使用
const void*TMenu_EditSelListBoxNotify(void *pvSource,
                                      unsigned char Type,
                                      const void *pvData)
{
  const TMenu_t *pMenu = pPopMenu();
  TItemSize_t Item = *(TItemSize_t*)pvData;   //当前项位置
  struct _EditSelData *pEditSelData = (struct _EditSelData*)pPopData();
  
  if(Item < TM_GetSize(pMenu))  
  pvSource = pvSource;//防止部分编译器警告
  //得到系统当前指向的数据缓冲区
  switch(Type){
  case TGUI_NOTIFY_GET_CAPTION://获得标题
    return TM_GetHeader(pMenu);
  case TLISTBOXEX_NOTIFY_PREFIX://获得前缀
    return _pGetItemHeader(pMenu, Item,
                           pEditSelData->HeaderW);
  case TGUI_NOTIFY_GET_ITEM:// 获得项显示内容
    if(TM_GetType(pMenu) & TM_MEDITSEL_EN_EDIT) 
      return _pGetItem(pMenu,Item,pEditSelData);//在数据上
    return _pGetItemHeader(pMenu,Item,
                           pEditSelData->HeaderW);
  case TLISTBOXEX_NOTIFY_SUFFIX://获得后缀
    return _pGetItem(pMenu,Item,pEditSelData);//在数据上
  case TGUI_NOTIFY_KEY_ENTER://每一项的确认键
    pEditSelData->User.CurItem = Item;
    pEditSelData->Flag |= _FLAG_ENTER;//后继处理
    break;
  case TGUI_NOTIFY_KEY_ESCAPE://退出到上一层
    pEditSelData->Flag |= _FLAG_RETURN;
  default: break;
  }
  return NULL;
}

/*****************************************************************
                    本模块相关
*****************************************************************/

//-----------------------确认键后继处理----------------------------
//有确认键响应时调用
//返回值定义为: 正值:进入相应子菜单,-1:返回上层菜单
static signed char _EnterLaterPro(struct _EditSelData *pEditSelData,
                           const TMenu_t *pMenu)
{
  TItemSize_t MaxItem = TM_GetSize(pMenu);
  //>=项数时通知用户区保存所有数据,否则表示通报进入对应项
  pEditSelData->User.DisEnter = 0;
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_SET_DATA,&pEditSelData->User);
  //确认键后继处理:有保存键时通知用户保存处理
  if(/*(TM_GetType(pMenu) & TM_MEDITSEL_GRP_SAVE) &&*/
     (pEditSelData->User.CurItem >= MaxItem)){
     //检查用户反馈的保存情况
     if(pEditSelData->User.CurItem >= MaxItem){//位置没变,保存正确到退出位置
       pEditSelData->User.CurItem = MaxItem;//防止用户改写出错
       TListboxEx_Key(&pEditSelData->ListboxEx,TGUI_KEY_DOWN);
     }
     else{ //该位置设置错误,光标到错误位置
       TListboxEx_SetSel(&pEditSelData->ListboxEx,pEditSelData->User.CurItem);
       TListBoxEx_PaintAll(&pEditSelData->ListboxEx);//更新显示
     }
  }
  else if(pEditSelData->User.DisEnter == 0){//允许进入时
    if(TM_GetType(pMenu) & TM_MEDITSEL_EN_EDIT)//允许调整时表示进入子菜单进行调整
      return (signed char)(pEditSelData->User.CurItem);
  }
  else{//不允许进入时,可能内容变了,强制更新
    TListBoxEx_PaintAll(&pEditSelData->ListboxEx);
  }
  pEditSelData->Flag &= ~_FLAG_ENTER;
  return -1;
}

//------------------------响应按键函数实现----------------------------
//2:响应按键操作并根据情况与用户进行数据交互,并更新显示
//返回值定义为: 正值:进入相应子菜单,-1,不管,-2:返回上层菜单
signed char TMenu_EditSelKey(const TMenu_t *pMenu,    //指向的菜单系统
                             unsigned char Key,       //送入的键值
                             void *pv)      //已被使用的自由数据缓冲区
{
  struct _EditSelData *pEditSelData = (struct _EditSelData *)pv;
  unsigned char Flag;
  PushMenu(pMenu);
  PushData(pv);
  
  //#ifdef TM_EN_RPC
  //  TItemSize_t Item = pEditSelData->User.CurItem;  
  //#endif
    
  TListboxEx_Key(&pEditSelData->ListboxEx,Key);
  
  //按键后期处理
  Flag = pEditSelData->Flag;
  
  //数字键后处理:当做快捷键处理

  if((Key >= '1') && (Key <= '9')){
    pEditSelData->User.CurItem = TListboxEx_GetSel(&pEditSelData->ListboxEx);
    Flag |= _FLAG_ENTER;
  }
  //进入键处理
  if(Flag & _FLAG_ENTER){
      return _EnterLaterPro(pEditSelData, pMenu);
  }
  //检查退出
  else if(Flag & _FLAG_RETURN)
    return -2;

  //没有进入或退出，在支持RPC时，检查选择项，若改变则通报改变以让用户层作好准备
  //#ifdef TM_EN_RPC
  //  if((Item != pEditSelData->User.CurItem) && (pEditSelData->User.CurItem < TM_GetSize(pMenu)))
  //    TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_CUSOR_CHANGED, &pEditSelData->User.CurItem);
  //#endif
  
  return -1;
}

//------------------------创建函数实现----------------------------
//1:创建该类型菜单,从用户区获得数据并更新显示
//返回创建成功:
#include "TWidgetPrv.h"
signed char TMenu_EditSelCreate(const TMenu_t *pMenu,    //指向的菜单系统
                                TWinHandle_t hWin,       //分配给菜单的窗口
                                //带入的自由数据,这里表示上次菜单位置
                                unsigned char Data,
                                void *pv)      //可供使用的自由缓冲区
{ 
  unsigned char W;
  unsigned char Item, CurLen;
  unsigned char Flag,Style;
  struct _EditSelData *pEditSelData = (struct _EditSelData *)pv;  
  
  //初始化其它数据
  pEditSelData->Flag = 0;
  
  //读写模式时样式配置
  Flag = TMenu_EditSel_cbGetCurStyle();
  if(TM_GetType(pMenu) & TM_MEDITSEL_EN_EDIT){//调整模式
    Style = TLISTBOXEX_ALIGN_LEFT | TLISTBOXEX_ANTI_WORD2 | 
      TLISTBOXEX_PREFIX | TLISTBOXEX_EN_APPEND;
    if(!(TM_GetType(pMenu) & TM_MEDITSEL_DIS_GRP_SAVE))//群保存时强制有保存与退出键
      Flag |= TCHECKBOX_EN_SAVE | TCHECKBOX_EN_RETURN;
  }
  else{ 
    Style = TLISTBOXEX_ALIGN_LEFT | TLISTBOXEX_ANTI_WORD2 | 
      TLISTBOXEX_SUFFIX | TLISTBOXEX_EN_APPEND;
  }

  //得到参数头宽度
  W = TM_GetItemMaxLen(pMenu) + 2;
  pEditSelData->HeaderW = W;
  //编计数据区宽度
  if(Flag & TCHECKBOX_EN_SAVE) W = 18;//保存退出所占大小
  else W = 0;
  for(Item = TM_GetSize(pMenu); Item > 0; Item--){
    CurLen = strlen(_pGetItem(pMenu, Item - 1, pEditSelData));
    if(W < CurLen) W = CurLen;
  }
  pEditSelData->ItemsW = W;
  
  W += pEditSelData->HeaderW;//最大宽度

  TListboxEx_Init(&pEditSelData->ListboxEx,  //由用户分配的存储空间
                  hWin,         //已建立并初始化好的窗口
                  TM_GetSize(pMenu),  //预定义项目总数
                  W,
                  //控件标志
                  Flag,
                  //显示样式
                  Style,
                  TGUI_NOTIFY_PASS(TMenu_EditSelListBoxNotify,TGUI_CBFUNID_TMENU_MNUM_LISTBOX)); //通报函数

  //局中处理
  CurLen = TWidget_GetDispW(TMenu_pGetTWin(hWin),Flag);
  if(W < CurLen){ //得到局中显示左偏移值
    CurLen = (CurLen - W) >> 1;
    if(CurLen & 0x01) CurLen--;//双字节对齐
    pEditSelData->HeaderW += CurLen;
    pEditSelData->ItemsW += CurLen;
  }
  
  //支持RPC时，可能默认位置即需要更新RPC数据,故先通报
  #ifdef TM_EN_RPC
    TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_CUSOR_CHANGED, &pEditSelData->User.CurItem);
  #endif
  
  //更新显示
  TMenu_ReBulidWin(&pEditSelData->ListboxEx,//重置窗口大小
                   W,
                   TM_GetSize(pMenu),
                   strlen(TM_GetHeader(pMenu)));
  
  TListboxEx_SetSel(&pEditSelData->ListboxEx,Data);//当前菜单项
  TListBoxEx_PaintAll(&pEditSelData->ListboxEx);
  
  return -1;
}


