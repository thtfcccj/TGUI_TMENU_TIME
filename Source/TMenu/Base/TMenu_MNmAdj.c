/*******************************************************************************

                  TMenu菜单之多值调整模式实现
注:这里只实现显示,调整通过子模块实现
*******************************************************************************/

//因TGUI里没有多值调整模式,这里实现为:MNumAdj用来显示内容,
//若调整时,在显示内容上叠加Win窗口显示当前调整项的内容.

//在扩展列表框用前后缀实现显示功能:
//  当为多值显示时,调整项提示在内容中,数值在后缀中,可实现查看切换
//  当为多值调整时,调整项提示在前缀中,数值在内容中,可实现数值调整切换
//扩展列表框应配置为,打开保存与返回项,左对齐

#include "TMenu.h"
#include "TMenu_MNumAdj.h"
#include "TGUIMath.h"
#include <string.h>

#include "TEdit_Float.h"  //不支持浮点数模式时，其内部编译为空

//其中:标志定义为:
#define _FLAG_ENTER    0x08       //按保存键
#define _FLAG_RETURN    0x10      //置此标志认为是退出上一层
#define _FLAG_ADJ_MODE  0x20      //当前在调整模式标志
#define _FLAG_LEFT_END  0x40      //循环模式调整时到达左边界标志
#define _FLAG_RIGHT_END  0x80      //循环模式调整时到达右边界标志


//----------------------------常用描述结构------------------------
#if TM_MNUMADJ_DEFAULT_DESC & 0x01
  const struct _MNumDesc MNumDesc_U8 = {MNUM_TYPE_DEC, 0, 255};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x02
  const struct _MNumDesc MNumDesc_B1 = {MNUM_TYPE_DEC, 0, 1};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x04
  const struct _MNumDesc MNumDesc_B2 = {MNUM_TYPE_DEC, 0, 3};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x08
  const struct _MNumDesc MNumDesc_9 = {MNUM_TYPE_DEC, 0, 9};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x10
  const struct _MNumDesc MNumDesc_99 = {MNUM_TYPE_DEC, 0, 99};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x20
  const struct _MNumDesc MNumDesc_999 = {MNUM_TYPE_DEC, 0, 999};
#endif
#if TM_MNUMADJ_DEFAULT_DESC & 0x40
  const struct _MNumDesc MNumDesc_9999 = {MNUM_TYPE_DEC, 0, 9999};
#endif

#if TM_MNUMADJ_DEFAULT_DESC & 0x80
  const struct _MNumDesc MNumDesc_U16 = {MNUM_TYPE_DEC, 0, 65535};
#endif  
  
/*****************************************************************
                      编辑模式相关
*****************************************************************/


//-----------------编辑模式通报回调实现函数------------------------
//仅供内部通报使用
const void*TMenu_MNumAdjEditNotify(void *pvSource,
                                   unsigned char Type,
                                   const void *pvData)
{
  const TMenu_t *pMenu = pPopMenu();
  const TEdit_t *pEdit = (TEdit_t*)pvSource;
  
  struct _MNumAdjData *pMNumAdjData = (struct _MNumAdjData*)pPopData();
  unsigned char Flag = pMNumAdjData->Flag;
  
  pvData = pvData;
  switch(Type){
  case TGUI_NOTIFY_KEY_ESCAPE://不确认退出
    TWM_FreeHandle(TEdit_hGetWin(pEdit)); //直接释放窗口
    Flag &= ~_FLAG_ADJ_MODE;    //退出调整模式
    break;
  case TEDIT_NOTIFY_LEFT_END://到达左右边界通报
  case TEDIT_NOTIFY_RIGHT_END:
    if(!(TM_GetType(pMenu) & TM_MNUMADJ_LOOP_MODE)) break;
    //调整模式为循环模式时处理
    if(Type == TEDIT_NOTIFY_LEFT_END) Flag |= _FLAG_LEFT_END;
    else Flag |= _FLAG_RIGHT_END; 
    //继续,还需当做确认键处理
  case TGUI_NOTIFY_KEY_ENTER://确认并退出:
    //得到数值
    pMNumAdjData->User.Value[pMNumAdjData->User.CurItem] = TEdit_GetVolume(pEdit);
    //通知用户该项数据被更新了
    TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_CHANGED,&pMNumAdjData->User);
    TWM_FreeHandle(TEdit_hGetWin(pEdit)); //释放窗口
    Flag &= ~_FLAG_ADJ_MODE;    //退出调整模式
    break;
  default: break;
  }
  pMNumAdjData->Flag = Flag;
  return NULL;
}

//-------------------------进入编辑模式函数--------------------------
void _EnterEditMode(const TMenu_t *pMenu,//指向的菜单系统
                    struct _MNumAdjData *pMNumAdjData)   
{
  struct _MNumDesc *pDesc;
  unsigned char x,y,Flag;
  TWidget_t *pWidget = (TWidget_t*)(&pMNumAdjData->ListboxEx);//继承关系
  TWinHandle_t hWin = TWidget_hGetWin(pWidget);//暂指向TListbox  
  
  //找到显示的位置,即hWin的x,y,利Desc用
  x = TWidget_GetClipX(pWidget) + 
    (pMNumAdjData->HeaderW - TWidget_GetX(pWidget));
  y = TWidget_GetClipY(pWidget) + 
    (TWidget_GetSel(pWidget) - TWidget_GetY(pWidget));
  //申请一个窗口并初始位位置
  hWin = cbTMenu_hGetEdit(hWin);
  TWin_Move(TWM_pGetWin(hWin),x,y);
  
  //根据数据类型准备相关数据
  pMNumAdjData->User.CurItem = TWidget_GetSel(pWidget);
  //得到用户区当前数据描述信息
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_MNUM_GET_DESC,&pMNumAdjData->User);

  pDesc = &pMNumAdjData->User.Desc;
  if(pDesc->Flag & MNUM_TYPE_GRP_MODE)       //群调整模式时强制打开提示
    Flag = TEDIT_GRP_MODE | TEDIT_EN_NOTIFY ;
  else Flag = TEDIT_EN_NOTIFY;
  if(TM_GetType(pMenu) & TM_MNUMADJ_ENTER_KEY)
    Flag |= TEDIT_ENTER_RIGHT;

  
  if(pDesc->Flag & MNUM_EN_SIGN) Flag |= TEDIT_EN_SIGN;
  switch(pDesc->Flag & MNUM_TYPE_MASK){
  case MNUM_TYPE_DEC:
    TEditDec(&pMNumAdjData->Edit,
             hWin,    //依赖的窗口,数据缓冲区>Len
             //初始化值
             (TSigned_t)pMNumAdjData->User.Value[pMNumAdjData->User.CurItem],    
             (TSigned_t)pDesc->Max,       //最大值,决定数值长度
             (TSigned_t)pDesc->Min,       //最小值
             pMNumAdjData->User.Desc.Flag & MNUM_DOT_POS_MASK,//小数点位置,可用于用整数模拟小数显示
             Flag,                  //相关配置
             TGUI_NOTIFY_PASS(TMenu_MNumAdjEditNotify,TGUI_CBFUNID_TMENU_MNUM_EDIT));//回调函数
    break;
  case MNUM_TYPE_FLOAT:
    TEditFloat(&pMNumAdjData->Edit,
             hWin,    //依赖的窗口,数据缓冲区>Len
             //初始化值
             TData2Float(pMNumAdjData->User.Value[pMNumAdjData->User.CurItem]),    
             TData2Float(pDesc->Max),       //最大值,决定数值长度
             TData2Float(pDesc->Min),       //最小值
             pMNumAdjData->User.Desc.Flag & MNUM_DOT_POS_MASK,//小数点位置,可用于用整数模拟小数显示
             Flag,                  //相关配置
             TGUI_NOTIFY_PASS(TMenu_MNumAdjEditNotify,TGUI_CBFUNID_TMENU_MNUM_EDIT));//回调函数
    break;
  default:break;
  }
}

/*****************************************************************
                      显示(列表框区域)模式相关
*****************************************************************/
//------------------------得到项头字符串函数----------------------------
static char *_pGetItemHeader(const TMenu_t *pMenu,
                             TItemSize_t CurItem,
                             unsigned char HeaderW)    //指向的菜单系统
{
  //数据右对齐,最后加": "
  char *pString = TMenu_pGetBuf();
  const char *pHeader = TM_GetItemString(pMenu,CurItem);
    
  unsigned char Start = HeaderW - (strlen(pHeader) + 2);
  memset(pString, ' ', Start);
  pString += Start;
  pString = Tstrcpy_ex(pString,pHeader);
  *pString++ = ':';
  *pString++ = ' ';
  *pString = '\0';
  return TMenu_pGetBuf();
}

//------------------------得到项数据函数----------------------------
static char *_pGetItem(const TMenu_t *pMenu,//指向的菜单系统
                       TItemSize_t CurItem,
                       struct _MNumAdjData *pMNumAdjData)   
{
  struct _MNumDesc *pDesc;
  unsigned char Flag;
  char *pString = (char*)TMenu_pGetBuf() + TLCD_WIDTH;  //防止共用缓冲区重合;

  //得到用户区当前数据描述信息
  TItemSize_t PrvItem = pMNumAdjData->User.CurItem;
  pMNumAdjData->User.CurItem = CurItem;
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_MNUM_GET_DESC,&pMNumAdjData->User);
  pMNumAdjData->User.CurItem = PrvItem;

  //非调整模式时，读取当前数据以实现实时更新
  if(!(pMenu->Type & TM_MNUMADJ_WRITE))
    TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_DATA,&pMNumAdjData->User);
  
  pDesc = &pMNumAdjData->User.Desc;
  Flag = pDesc->Flag;
  //根据类型显示值
  switch(Flag & MNUM_TYPE_MASK){
  case MNUM_TYPE_DEC:
    pString = pTNum2StringFlag(pMNumAdjData->User.Value[CurItem],
                              pString,
                              TGetNumLen(pDesc->Max,pDesc->Min),//显示长度
                              Flag);  //标志与小数点
    break;
  case MNUM_TYPE_FLOAT:
    pString = pTFloat2StringFlag(TData2Float(pMNumAdjData->User.Value[CurItem]),
                              pString,
                              TGetFloatLen(TData2Float(pDesc->Min),
                                          TData2Float(pDesc->Max),
                                          Flag & 0x07),//显示长度
                              Flag);  //标志与小数点

  default:break;  //错误
  }
  *pString = '\0';
  return (char*)TMenu_pGetBuf() + TLCD_WIDTH;  //防止共用缓冲区重合;
}


//通报函数应响应下列类型通报:
//TGUI_NOTIFY_GET_CAPTION 获得标题
//TGUI_NOTIFY_GET_ITEM 若得项显示内容
//TGUI_NOTIFY_KEY_ENTER 每一项的确认键,此时应检查选中与否并进行应用层处理
//若项超过设定希数,表示在附加的保存焦点上
//TGUI_NOTIFY_EKY_ESCAPE 退出到上一层
//-----------------------多值调整模式列框框通报回调实现函数------------------------
//仅供内部通报使用
const void*TMenu_MNumAdjListBoxNotify(void *pvSource,
                                      unsigned char Type,
                                      const void *pvData)
{
  const TMenu_t *pMenu = pPopMenu();
  TItemSize_t Item = *(TItemSize_t*)pvData;   //当前项位置
  struct _MNumAdjData *pMNumAdjData = (struct _MNumAdjData*)pPopData();
  
  if(Item < TM_GetSize(pMenu))  
  pvSource = pvSource;//防止部分编译器警告
  //得到系统当前指向的数据缓冲区
  switch(Type){
  case TGUI_NOTIFY_GET_CAPTION://获得标题
    return TM_GetHeader(pMenu);
  case TLISTBOXEX_NOTIFY_PREFIX://获得前缀
    return _pGetItemHeader(pMenu, Item,
                           pMNumAdjData->HeaderW);
  case TGUI_NOTIFY_GET_ITEM:// 获得项显示内容
    if(TM_GetType(pMenu) & TM_MNUMADJ_WRITE) 
      return _pGetItem(pMenu,Item,pMNumAdjData);//在数据上
    return _pGetItemHeader(pMenu,Item,
                           pMNumAdjData->HeaderW);
  case TLISTBOXEX_NOTIFY_SUFFIX://获得后缀
    return _pGetItem(pMenu,Item,pMNumAdjData);//在数据上
  case TGUI_NOTIFY_KEY_ENTER://每一项的确认键
    pMNumAdjData->User.CurItem = Item;
    pMNumAdjData->Flag |= _FLAG_ENTER;//后继处理
    break;
  case TGUI_NOTIFY_KEY_ESCAPE://退出到上一层
    pMNumAdjData->Flag |= _FLAG_RETURN;
  default: break;
  }
  return NULL;
}

/*****************************************************************
                    本模块相关
*****************************************************************/

//-----------------------确认键后继处理----------------------------
//有确认键响应是调用
static void _EnterLaterPro(struct _MNumAdjData *pMNumAdjData,
                           const TMenu_t *pMenu)
{
  TItemSize_t MaxItem = TM_GetSize(pMenu);
  //确认键后继处理:通知用户保存处理
  if(pMNumAdjData->User.CurItem >= MaxItem){//通知用户区保存
     TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_SET_DATA,&pMNumAdjData->User);
     //检查用户反馈的保存情况
     if(pMNumAdjData->User.CurItem >= MaxItem){//位置没变,保存正确到退出位置
       pMNumAdjData->User.CurItem = MaxItem;//防止用户改写出错
       TListboxEx_Key(&pMNumAdjData->ListboxEx,TGUI_KEY_DOWN);
     }
     else{ //该位置设置错误,光标到错误位置
       TListboxEx_SetSel(&pMNumAdjData->ListboxEx,pMNumAdjData->User.CurItem);
       TListBoxEx_PaintAll(&pMNumAdjData->ListboxEx);//更新显示
     }
  }
  else{
    //确认键回调只有在非循环模式时有效,表示进入调整模式
    if(TM_GetType(pMenu) & TM_MNUMADJ_WRITE){
      pMNumAdjData->Flag |= _FLAG_ADJ_MODE;
      _EnterEditMode(pMenu,pMNumAdjData);
    }
  }
    pMNumAdjData->Flag &= ~_FLAG_ENTER;
}
    


//------------------------响应按键函数实现----------------------------
//2:响应按键操作并根据情况与用户进行数据交互,并更新显示
//返回值定义为: 正值:进入相应子菜单,-1,不管,-2:返回上层菜单
signed char TMenu_MNumAdjKey(const TMenu_t *pMenu,    //指向的菜单系统
                             unsigned char Key,       //送入的键值
                             void *pv)      //已被使用的自由数据缓冲区
{
  struct _MNumAdjData *pMNumAdjData = (struct _MNumAdjData *)pv;
  unsigned char Flag;
  PushMenu(pMenu);
  PushData(pv);
  
  if(pMNumAdjData->Flag & _FLAG_ADJ_MODE){//调整模式时焦点在编辑框
    TEdit_Key(&pMNumAdjData->Edit,Key);
    if(!(pMNumAdjData->Flag & _FLAG_ADJ_MODE))//退出编辑框时需要更新显示值
      TListBoxEx_PaintAll(&pMNumAdjData->ListboxEx);
  }
  //否则在列表框,列表框非控制键只有在非循环模式有效
  else if((Key <= 0x20) || !(TM_GetType(pMenu) & TM_MNUMADJ_LOOP_MODE))
    TListboxEx_Key(&pMNumAdjData->ListboxEx,Key);
  
  //按键后期处理
  Flag = pMNumAdjData->Flag;
  if(Flag & _FLAG_ENTER) _EnterLaterPro(pMNumAdjData,pMenu);
  //到下一项,相到于退出后先按下键,再按确认键,此部分可能也需进行确认键处理
  else if(Flag & (_FLAG_LEFT_END | _FLAG_RIGHT_END)){
    if(Flag & _FLAG_LEFT_END)//左回环
      TListboxEx_Key(&pMNumAdjData->ListboxEx,TGUI_KEY_UP);
    else//右回环
      TListboxEx_Key(&pMNumAdjData->ListboxEx,TGUI_KEY_DOWN);
    //还在调整模式时直接进入
    if(TListboxEx_GetSel(&pMNumAdjData->ListboxEx) <  TM_GetSize(pMenu)) 
      TListboxEx_Key(&pMNumAdjData->ListboxEx,TGUI_KEY_ENTER);
    //再次检查确认键
    if(pMNumAdjData->Flag & _FLAG_ENTER)
       _EnterLaterPro(pMNumAdjData,pMenu); 
    pMNumAdjData->Flag &= ~(_FLAG_LEFT_END | _FLAG_RIGHT_END);
  }
  //检查退出
  else if(Flag & _FLAG_RETURN){
    if(!TListboxEx_HaveAppend(&pMNumAdjData->ListboxEx))//无附加项时退出强制保存
      TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_SET_DATA,&pMNumAdjData->User);
    return -2;
  }
    

  return -1;
}

//------------------------创建函数实现----------------------------
//1:创建该类型菜单,从用户区获得数据并更新显示
//返回创建成功:
#include "TWidgetPrv.h"
signed char TMenu_MNumAdjCreate(const TMenu_t *pMenu,    //指向的菜单系统
                                TWinHandle_t hWin,       //分配给菜单的窗口
                                unsigned char Data,//带入的自由数据,这里无意义
                                void *pv)      //可供使用的自由缓冲区
{
  
  struct _MNumAdjData *pMNumAdjData = (struct _MNumAdjData *)pv;
  TItemSize_t w;  
  unsigned char Flag;
  
   //初始化数据
  pMNumAdjData->Flag = 0;
  //读写模式时样式配置
  if(TM_GetType(pMenu) & TM_MNUMADJ_WRITE){//调整模式
    Data = TLISTBOXEX_ALIGN_LEFT | TLISTBOXEX_ANTI_WORD2 | 
      TLISTBOXEX_PREFIX | TLISTBOXEX_EN_APPEND;
    Flag = TMenu_MNumAdj_cbGetCurStyle() | TMenu_MNumAdj_cbGetCurSaveStyle();
  }
  else{ 
    Data = TLISTBOXEX_ALIGN_LEFT | TLISTBOXEX_ANTI_WORD2 | 
      TLISTBOXEX_SUFFIX | TLISTBOXEX_EN_APPEND;
    Flag = TMenu_MNumAdj_cbGetCurStyle();
  }
  
  //得到参数头宽度
  unsigned char HeaderW = TM_GetItemMaxLen(pMenu) + 2;
  pMNumAdjData->HeaderW = HeaderW;
  HeaderW += 6; //数值最大宽度
  w = TWidget_GetDispW(TMenu_pGetTWin(hWin),Flag);  
  if(HeaderW < w){ //得到局中显示左偏移值
    HeaderW = (w - HeaderW) >> 1;
    if(HeaderW & 0x01) HeaderW--;//双字节对齐
    pMNumAdjData->HeaderW += HeaderW; 
  }


  //读取当前数据
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_DATA,&pMNumAdjData->User);


  TListboxEx_Init(&pMNumAdjData->ListboxEx,  //由用户分配的存储空间
                  hWin,         //已建立并初始化好的窗口
                  TM_GetSize(pMenu),  //预定义项目总数
                  pMNumAdjData->HeaderW + 6,//数值调整位置最多位数
                  //控件标志,带返回字
                  Flag,
                  //显示样式
                  Data,
                  TGUI_NOTIFY_PASS(TMenu_MNumAdjListBoxNotify,TGUI_CBFUNID_TMENU_MNUM_LISTBOX)); //通报函数

  //更新显示
  #ifdef TMENU_WIN_VARIABLE
    if(Flag & (TLISTBOXEX_EN_RETURN | TLISTBOXEX_EN_SAVE)){//有附加项时
      TMenu_ReBulidWin(&pMNumAdjData->ListboxEx,//重置窗口大小
                       pMNumAdjData->HeaderW + 8,//数值调整位置最多位数,附加项大小
                       TM_GetSize(pMenu) + 1,//附加项个数
                       strlen(TM_GetHeader(pMenu)));
    }
    else{
      TMenu_ReBulidWin(&pMNumAdjData->ListboxEx,//重置窗口大小
                       pMNumAdjData->HeaderW + 6,
                       TM_GetSize(pMenu),
                       strlen(TM_GetHeader(pMenu)));
    }
  #endif
  TListBoxEx_PaintAll(&pMNumAdjData->ListboxEx);
  //循环调整模式模拟按键直接进入
  Flag = TM_GetType(pMenu);//Type
  if((Flag & TM_MNUMADJ_CREATE_ENTER) || //创建时强制进入
     ((Flag & (TM_MNUMADJ_LOOP_MODE | TM_MNUMADJ_WRITE)) == 
       (TM_MNUMADJ_LOOP_MODE | TM_MNUMADJ_WRITE))) 
    TMenu_MNumAdjKey(pMenu,TGUI_KEY_ENTER,pv);
  return -1;
}


