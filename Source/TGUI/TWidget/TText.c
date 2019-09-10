/*******************************************************************************

                           TWidget之扩展文本框控件实现

*******************************************************************************/
#include "TText.h"
#include <string.h>

#include "TWidgetShare.h"
//------------------------到达文本指定行函数-----------------------------
//返回NULL表示该行未找到
static const char* _GoRow(TText_t *pText,
                          TItemSize_t Row)
{
  const char *pString =  pText->pString;
  do{
    pString = strchr(pString,'\n');
    if(!Row) return pString;//找到了
    Row--;
  }while(pString);
  return NULL;
}

//-------------------------文本框反显回调实现函数-----------------------------
static void _AutiNotify(TText_t *pText,
                        unsigned char Style,//样式标志
                        TItemSize_t Item)    //当前行
{
  TItemSize_t Len;
  //文本可获得焦点且不在附加项里时
  if((Item != (TListbox_GetItems((TText_t*)pText) - 1)) &&
     (pText->Style & TTEXT_EN_FOCUS_TEXT)){
    if(*(_GoRow(pText,Item) + pText->FocusX) == 0x80)
      Len = 2;//全角
    else Len = 1;//半角
    TListbox_SetFocus((TText_t*)pText, Item, pText->FocusX, Len);
    return;
  }
  
  Style &= TTEXT_EN_BOTTOM1 | TTEXT_EN_BOTTOM2;
  //无附加项时无焦点
  if(!Style) 
    TListbox_SetFocus((TText_t*)pText,0, 0, 0);
  
  //附加项里有焦点
  if(Style & TTEXT_EN_BOTTOM1) Len = strlen(pText->pBottom1);
  else Len = 0;
  if(Style & TTEXT_EN_BOTTOM2) Item = strlen(pText->pBottom2);
  else Item = 0;
  Append2_AutiNotify((TWidget_t*)pText,Style,
                          pText->Style & TTEXT_FOCUS2,Len,Item);
}


//-------------------------文本框得到项回调实现函数-----------------------------
static const void *_ItemNotify(TText_t *pText,
                               unsigned char Style,    //样式标志
                               TItemSize_t Item)    //当前行
{
  char *pBuf = TGUI_pGetBuf();       //使用共享缓冲
  TItemSize_t ItemSpace,StrLen;
  unsigned char Align =Style & TTEXT_ALIGN_MASK;
  const char *pString;//接收字符串
  
  Style &= TTEXT_EN_BOTTOM1 | TTEXT_EN_BOTTOM2;
  //附加项里的内容填充
  if((Style) && Item == (TListbox_GetItems((TText_t*)pText) - 1)){
    return Append2_GetString((TWidget_t*)pText,
                             Style & (TTEXT_EN_BOTTOM1 | TTEXT_EN_BOTTOM2),
                             pText->pBottom1,
                             pText->pBottom2);            
  }
  //用户空间填充,根据对齐方式填充内容
  pString = _GoRow(pText,Item);
  StrLen = (strchr(pString,'\n') - pString);
  if(Align == TTEXT_ALIIGN_CENTER){
    ItemSpace = TGetAlignLenR((TListbox_GetW((TText_t*)pText) - StrLen) >> 1,0);
    memset(pBuf,' ',ItemSpace);pBuf += ItemSpace;//前面填充
    pBuf = Tstrcpy_ex(pBuf,pString);//填充内容
    //考虑前对齐时可能往前移了一个空格,后需要补齐
    ItemSpace = TGetAlignLenR((TListbox_GetW((TText_t*)pText) - StrLen - 1) >> 1,0);
    memset(pBuf,' ',ItemSpace); pBuf += ItemSpace;//后面填充
  }
  else if(Align == TTEXT_ALIGN_LEFT){
    ItemSpace = TListbox_GetW((TText_t*)pText) - StrLen;
    pBuf = Tstrcpy_ex(pBuf,pString);//填充内容
  }
  else if(Align == TTEXT_ALIGN_RIGHT){
    pBuf = Tstrcpy_ex(pBuf,pString);//填充内容
    ItemSpace = TListbox_GetW((TText_t*)pText) - StrLen;
    memset(pBuf,' ',ItemSpace); pBuf += ItemSpace;//后面填充
  }
  //else{//分散对齐时
  // 
  //}
  *pBuf = '\n';//结束字符
  
  return TGUI_pGetBuf();
}

//-------------------------文本框回调实现函数-----------------------------
//仅供内部通报使用
const void*TText_Notify(void *pvSource,
                        unsigned char Type, 
                        const void *pvData)
{
  TItemSize_t Item = *(TItemSize_t*)pvData;
  unsigned char Style = ((TText_t *)pvSource)->Style;
  //因此控件以Listbox为基类的,故可以直接转换为扩展类型
  TText_t *pText = (TText_t*)pvSource;
  TGUINotify_t cbGUINotify = pText->cbGUINotify;//用户空间通报
  switch(Type){
  case TGUI_NOTIFY_KEY_ENTER: //确认键时检查焦点位置
    //返回用户定义功能
    if((Style & (TTEXT_EN_BOTTOM1 | TTEXT_EN_BOTTOM2)) && 
       (Item == (TListbox_GetItems((TText_t*)pText) - 1))){
         if(Style & TTEXT_FOCUS2) 
           Type = TTEXT_NOTIFY_BOTTOM2;
         else Type = TTEXT_NOTIFY_BOTTOM1;
    }
    break;  //交给用户空间处理
  case TGUI_NOTIFY_GET_FOCUS://在本层处理焦点
    _AutiNotify(pText,Style,Item);            
    return NULL; //直接附加
  case TGUI_NOTIFY_GET_ITEM://在本层处理数据
    return _ItemNotify(pText,Style,Item);
  //搞不定的,直接交给用户吧
  default: break;
  }
  return TGUI_NOTIFY_RUN(cbGUINotify,pText,Type,pvData);
}


//-------------------------初始化函数-----------------------------
//注:考虑很多系统仅使用静态内层分配,故将创建函数留给更高一层,即
//可在此基础上立真正的创建函数
void TText_Init(TText_t *pText,  //由用户分配的存储空间
                TWinHandle_t hWin,         //已建立并初始化好的窗口
                unsigned char Flag,        //控件标志
                unsigned char Style,       //文本显示样式
                const char *pBottom1,      //首个按钮字,无时为NULL
                const char *pBottom2,      //第二个按钮字,无时为NULL            
                TGUINotify_t cbGUINotify) //通报函数
{
  //先初始化扩展,以便回调函数可以正常工作
  pText->Style = Style;
  pText->cbGUINotify = cbGUINotify;
  pText->pBottom1 = pBottom1;
  pText->pBottom2 = pBottom2;

  TListbox_Init((TListbox_t*)pText,hWin,Flag,1,
                TGUI_NOTIFY_PASS(TText_Notify,TGUI_CBFUNID_TEXT));//基类初始化
  
  TText_UpdateText(pText);
}

//-------------------------更新字符函数-----------------------------
//当字符被改变时调用此函数
//由控件自已获得字符串
void TText_UpdateText(TText_t *pText)
{
  TItemSize_t Items= 0;         //预定义项目总数
  const char *pString = (const char *)TGUI_NOTIFY_RUN(pText->cbGUINotify,
                                  pText,TTEXT_NOTIFY_GET_STRING,NULL);
  pText->pString = pString;
  //统计所需行数
  while(pString){
    pString = strchr(pString,'\n');
    Items++;
  };
  if(pText->Style & (TTEXT_EN_BOTTOM1 | TTEXT_EN_BOTTOM1)) Items++; //多附加选择项
  TListbox_SetItems((TListbox_t*)pText,Items); 
}


//-------------------------按键处理函数-----------------------------
void TText_Key(TText_t *pText,unsigned char Key)
{
  TItemSize_t FocusX;
  unsigned char Flag = pText->Style & TTEXT_FOCUS2;
  do{
    Flag = Append2_Key((TWidget_t *)pText,Key,
                       TWidget_GetUserFlag((TWidget_t *)pText),Flag);
    if(Flag != (unsigned char)-1) break;
    TListbox_Key((TListbox_t*)(pText),Key);
	}while(1);
  if(Flag <= 1){
    if(Flag  == 1) pText->Style |= TTEXT_FOCUS2;                           
    else pText->Style &= ~TTEXT_FOCUS2;
    ListBox_Paint((TListbox_t*)(pText),TGUI_PAINT_FOCUS);
  }
  
  if(pText->Style & TTEXT_EN_FOCUS_TEXT){//处理文本焦点移动
    FocusX = pText->FocusX;
    if(*(_GoRow(pText,TListbox_GetSel((TListbox_t*)pText)) +
         FocusX) == 0x80)//检查当前位置全角半角
      FocusX += 2;//全角
    else FocusX += 1;//半角
    if(FocusX >= TListbox_GetW((TListbox_t*)pText))
       FocusX = 0;//回到前面了
    pText->FocusX = FocusX;
  }
}




