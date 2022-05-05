/*******************************************************************************

                           TWidget之列表框扩展控件实现

*******************************************************************************/
#include "TListboxEx.h"
#include "TWidgetShare.h"
#include <string.h>

#define   _FOCUS_APPEND     0x80    //在附加项里标志

//TListboxEx_t内部标志定义为:
//0-6bit表示有效字符宽度
//7bit在有保存与返回字时,表示在那一位上


//-------------------------列表框反显回调实现函数-----------------------------
static void _AutiNotify(TListboxEx_t *pListboxEx,
                        unsigned char Auti,     //样式中的反显标志位
                        unsigned char Style,    //样式标志
                        unsigned char Flag,     //附加项里的保存与返回标志
                        TGUINotify_t cbGUINotify,//用户回调函数
                        const void *pvData)     //用户回调函数所跟的数据
{
  TItemSize_t ItemX,ItemW,ItemSpace,W;
  const char  *pString;
  TItemSize_t Sel = *(TItemSize_t*)pvData;//被选项位置
  TItemSize_t Prefix,Suffix;
  //在附加项里时,焦点处理
  if((Style & TLISTBOXEX_EN_APPEND) && (Flag &  _FOCUS_APPEND)){
    //直接保存返回字,在本层处理焦点,仅处理附加项焦点
    if(Flag & (TLISTBOXEX_EN_SAVE | TLISTBOXEX_EN_RETURN)){
      Append2_AutiNotify((TWidget_t*)pListboxEx,Flag,
                         pListboxEx->Flag & 0x80,
                         strlen(cbGetSaveBottom()),
                         strlen(cbGetReturnBottom())); 
    }
    else //留给上层
      TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,TGUI_NOTIFY_GET_FOCUS,pvData);
    return;
  }
  //整行反显
  if(Auti == TLISTBOXEX_ANTI_ROW){  
    TListbox_SetFocus((TListboxEx_t*)pListboxEx,
                      Sel, 0,0);
    return;
  }
  //禁止反显
  if(Auti == TLISTBOXEX_AUTI_DIS){
    TListbox_SetFocus((TListboxEx_t*)pListboxEx,0,(TItemSize_t)-1,0);
    return;
  }
  //反显一部分时,初始化变量
	if(Style & TLISTBOXEX_PREFIX)//前缀长度
	    Prefix = strlen((const char*)TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,TLISTBOXEX_NOTIFY_PREFIX,pvData));
  else  Prefix = 0;
  if(Style & TLISTBOXEX_SUFFIX)//后缀长度
      Suffix = strlen(TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,TLISTBOXEX_NOTIFY_PREFIX,pvData));
  else Suffix = 0;
  //前后缀禁止反显时
  if(Auti == TLISTBOXEX_ANTI_WORD2){
    ItemX =  Prefix;
    ItemW = 0;
  }
  else{
    ItemX =  0;
    ItemW = Prefix +  Suffix;
  }
  
  //有序号时(序号不反显),加上序号长度,序号跟了一个:".",计算对齐后的长度,
  if(Style & TLISTBOXEX_EN_NO)
    ItemX += TGetAlignLenR(TGetItemsPlace(TListbox_GetItems((TListboxEx_t*)pListboxEx)),1);
  //得到字符长度
  pString = TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,TGUI_NOTIFY_GET_ITEM,pvData);
  ItemW += strlen(pString);

  //最后起始位置加上由对齐方式引起的空格
  Style &= TLISTBOXEX_ALIGN_MASK;
  W = TListbox_GetDispW((TListboxEx_t*)pListboxEx);
  if(W < ItemW) W = ItemW; //扩充长度适应内容
  if(Style == TLISTBOXEX_ALIIGN_CENTER)//局中对齐时,排除左测空白区
    ItemX += TGetAlignLenR((W - ItemW) >> 1,0);
  else if(Style == TLISTBOXEX_ALIGN_RIGHT)//右对齐时,排除左测空白区
    ItemX += TGetAlignLenR(W - ItemW, 0);
  else if(Style == TLISTBOXEX_ALIGN_LEN){ //项长度对齐时,排除前面空白区
    W = pListboxEx->ValidW;
	  ItemSpace = TWidget_GetHPageSizeEnNotify((TWidget_t*)pListboxEx);
      if((W) <  ItemSpace){  //项总长小于页宽时,局中对齐
      ItemX +=  TGetAlignLenL((ItemSpace - (W)) >> 1,0);
    }
  }
  else{ //左对齐时,以左测第一个非空字符作为起始反显
    W = 0;
    while(*pString == ' ') {pString++; W++;}
    ItemX += W;
    ItemW -=W;
  }

  TListbox_SetFocus((TListboxEx_t*)pListboxEx, Sel, ItemX, ItemW);
}


//------------------------------------填充序号函数--------------------------
static char *pFullNO(TListboxEx_t *pListboxEx,
                     TItemSize_t  Item,
                     char *pBuf)
{
  unsigned char Data;
  //先填充序号
  Data = TGetItemsPlace(TListbox_GetItems((TListboxEx_t*)pListboxEx));//占位
  pBuf += TValue2StringMin(Item + 1,pBuf,Data);//序号从1开始
  *pBuf++='.';  //1.内容
  pBuf = pTAlignString(Data + 1,pBuf);//序号对齐
  return pBuf;
}

//-------------------------列表框得到项回调实现函数-----------------------------
static const void *_ItemNotify(TListboxEx_t *pListboxEx,
                               unsigned char Style,    //样式标志
                               unsigned char Flag,     //附加项里的保存与返回标志
                               TGUINotify_t cbGUINotify,//用户回调函数
                               const void *pvData)     //用户回调函数所跟的数据
{
  char *pBuf = TGUI_pGetBuf();       //使用共享缓冲
  TItemSize_t Item = *(TItemSize_t*)pvData;   //当前项位置
  unsigned char Align = Style & TLISTBOXEX_ALIGN_MASK;
  TItemSize_t ItemW,ItemSpace,W;
  const char *pString,*pPrefix = NULL,*pSuffix = NULL;//接收字符串,=NULL防止部分编译器警告
  
  //在附加项里时,内容处理
  if((Style & TLISTBOXEX_EN_APPEND) && (Flag &  _FOCUS_APPEND)){
    //附加项处理
    if(Flag & (TLISTBOXEX_EN_SAVE | TLISTBOXEX_EN_RETURN)){
      Flag &= ~_FOCUS_APPEND;
      return Append2_GetString((TWidget_t*)pListboxEx,Flag,
                               cbGetSaveBottom(),
                               cbGetReturnBottom());  
    }
    else //留给上层
      return TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,TGUI_NOTIFY_GET_ITEM,pvData);
  }
  
  //得到用户空间字符串,含附加字
  pString = (const char *)TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,TGUI_NOTIFY_GET_ITEM,pvData);
  if(pString == NULL){//返回异常了
    *pBuf = '\0';//结束字符
    return pBuf;
  }
  ItemW = strlen(pString);
  //序号占位
  if(Style & TLISTBOXEX_EN_NO)
    ItemW += TGetAlignLenR(TGetItemsPlace(TListbox_GetItems((TListboxEx_t*)pListboxEx)) + 1,0);

  if(Style & TLISTBOXEX_PREFIX){//得到前缀
    pPrefix = (const char *)TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,TLISTBOXEX_NOTIFY_PREFIX,pvData);
    ItemW += strlen(pPrefix);
  }
  if(Style & TLISTBOXEX_SUFFIX){//得到后缀
    pSuffix = (const char *)TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,TLISTBOXEX_NOTIFY_SUFFIX,pvData);
    ItemW += strlen(pSuffix);
  }

  W = TListbox_GetDispW((TListboxEx_t*)pListboxEx);
  if(W < ItemW) W = ItemW; //扩充长度适应内容

  //根据对齐方式填充内容,前面填充:
  if(Align == TLISTBOXEX_ALIIGN_CENTER)
    ItemSpace = TGetAlignLenR((W - ItemW) >> 1,0);
  else if(Align == TLISTBOXEX_ALIGN_LEFT)
    ItemSpace = 0;
  else if(Align == TLISTBOXEX_ALIGN_RIGHT)
    ItemSpace = W - ItemW;
  else{// if(Align == TLISTBOXEX_ALIGN_LEN){ //项长度对齐前填充
	  ItemSpace = TWidget_GetHPageSizeEnNotify((TWidget_t*)pListboxEx);   
    if(pListboxEx->ValidW <  ItemSpace)  //项总长小于页宽时,局中对齐
      ItemSpace = TGetAlignLenL((ItemSpace - pListboxEx->ValidW) >> 1,0);
    else ItemSpace = 0;
  }
  memset(pBuf,' ',ItemSpace);pBuf += ItemSpace; ItemW += ItemSpace;

  if(Style & TLISTBOXEX_EN_NO) pBuf = pFullNO(pListboxEx,Item,pBuf);//序号填充
  if(Style & TLISTBOXEX_PREFIX) pBuf = Tstrcpy_ex(pBuf,pPrefix);//填充前缀
  pBuf = Tstrcpy_ex(pBuf,pString);//填充内容
  if(Style & TLISTBOXEX_SUFFIX) pBuf = Tstrcpy_ex(pBuf,pSuffix);//填充后缀
  //后空格填充
  if(W > ItemW){
    memset(pBuf,' ',W - ItemW); 
    pBuf += W - ItemW;//后面填充
  }
  *pBuf = '\0';//结束字符
  return TGUI_pGetBuf();
}

//-------------------------列表框回调实现函数-----------------------------
//仅供内部通报使用
const void*TListBox_Notify(void *pv,
                           unsigned char Type,
                           const void *pvData)
{
  //因此控件以Listbox为基类的,故可以直接转换为扩展类型
  TListboxEx_t *pListboxEx = (TListboxEx_t*)pv;
  unsigned char Style = pListboxEx->Style;
  TGUINotify_t cbGUINotify = pListboxEx->cbGUINotify;//用户空间通报
  unsigned char Flag = TWidget_GetUserFlag((TWidget_t *)pListboxEx);//基类关系
  TItemSize_t Item = *(TItemSize_t*)pvData;   //当前项位置
  
  if((Style & TLISTBOXEX_EN_APPEND) && //有附加项时
     (Item == (TListbox_GetItems((TListbox_t*)pListboxEx) - 1)))
    Flag |= _FOCUS_APPEND; //焦点表示在附加项里  
  
  switch(Type){
  case TGUI_NOTIFY_KEY_ENTER: //确认键时检查焦点位置
    if(Style & TLISTBOXEX_EN_APPEND){//有附加时
      //焦点在返回上时解释为返回键
      if(Flag == (_FOCUS_APPEND | TLISTBOXEX_EN_RETURN))
        Type = TGUI_NOTIFY_KEY_ESCAPE;
      else if(Flag == (_FOCUS_APPEND | TLISTBOXEX_EN_SAVE | TLISTBOXEX_EN_RETURN)){
        if(pListboxEx->Flag & 0x80) //返回上时,解释为返回键
          Type = TGUI_NOTIFY_KEY_ESCAPE;
      }
    }
    //else //交给用户空间处理
    break;  //交给用户空间处理
  case TGUI_NOTIFY_GET_FOCUS://在本层处理焦点
    _AutiNotify(pListboxEx,
                Style & TLISTBOXEX_AUTI_MASK,Style,Flag,
                cbGUINotify,pvData);   
    return NULL; //直接附加
  case TGUI_NOTIFY_GET_ITEM://在本层处理数据
    return _ItemNotify(pListboxEx,Style,Flag,cbGUINotify,pvData);
  //搞不定的,直接交给用户吧
  default: break;
  }
  return TGUI_NOTIFY_RUN(cbGUINotify,pListboxEx,Type,pvData);
}

//-------------------------初始化函数-----------------------------
//注:考虑很多系统仅使用静态内层分配,故将创建函数留给更高一层,即
//可在此基础上立真正的创建函数
void TListboxEx_Init(TListboxEx_t *pListboxEx,  //由用户分配的存储空间
                     TWinHandle_t hWin,         //已建立并初始化好的窗口
                     TItemSize_t Items,         //预定义项目总数
                     TItemSize_t ItemW,         //字符最大宽度
                     unsigned char Flag,        //控件标志
                     unsigned char Style,       //显示样式
                     TGUINotify_t cbGUINotify) //通报函数
{
  //先初始化扩展,以便回调函数可以正常工作
  pListboxEx->cbGUINotify = cbGUINotify;
  
  if(Flag & (TLISTBOXEX_EN_SAVE | TLISTBOXEX_EN_RETURN)) //强置置附加项
    Style |=TLISTBOXEX_EN_APPEND;
  else //强置去除
     Style &= ~TLISTBOXEX_EN_APPEND;
  
  pListboxEx->Style = Style;

  //多附加的选择项   
  if(Style & TLISTBOXEX_EN_APPEND) Items++; 
     
  TListbox_Init((TListbox_t*)pListboxEx,hWin,Flag,Items,
                TGUI_NOTIFY_PASS(TListBox_Notify,TGUI_CBFUNID_LISTBOX_EX));//基类初始化

  pListboxEx->ValidW = ItemW;
  TListbox_SetW((TListbox_t*)pListboxEx,ItemW);
}

//-------------------------按键处理函数-----------------------------
void TListboxEx_Key(TListboxEx_t *pListboxEx,unsigned char Key)
{
  unsigned char Flag = pListboxEx->Flag & 0x80;
  //可能带附加项的处理
  if(pListboxEx->Style & TLISTBOXEX_EN_APPEND){
    do{
      Flag = Append2_Key((TWidget_t *)pListboxEx,Key,
                       TWidget_GetUserFlag((TWidget_t *)pListboxEx),
                       Flag);
      if(Flag != (unsigned char)-1) break;
      TListbox_Key((TListbox_t*)(pListboxEx),Key);
    }while(1);
    
    if(Flag <= 1){
      if(Flag == 1) pListboxEx->Flag |= 0x80;     
      else pListboxEx->Flag &= ~0x80; 
      ListBox_Paint((TListbox_t*)(pListboxEx),TGUI_PAINT_FOCUS);
    }
  }
  //基类处理
  else TListbox_Key((TListbox_t*)(pListboxEx),Key);
    
}

//---------------------------★得到项总数-------------------------------
TItemSize_t TListboxEx_GetItems(TListboxEx_t *pListboxEx)
{
  TItemSize_t Items = TListbox_GetItems(pListboxEx);
  if(TListboxEx_HaveAppend(pListboxEx)) Items--;
  return Items;
}

