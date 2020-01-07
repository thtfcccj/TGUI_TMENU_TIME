/*****************************************************************************
                         TGUI上的输入法模块总实现
注:暂不支持自带字库全角对齐LCD显示屏的显示
*****************************************************************************/

#include "TImeMng.h"
#include <string.h>

/*****************************************************************************
                           内部宏定义
*****************************************************************************/
#define _BORDER_MIN_W   ((128 / 8) + 4)  //带边框最小宽度
#define _BORDER_MIN_H   ((64 / 16) + 2)  //带边框最小高度

#define _NOTE_MIN_W   (_BORDER_MIN_W + 1)  //带提示行最小宽度
#define _NOTE_MIN_H   (_BORDER_MIN_H + 1)  //带提示行最小高度

#define _SPACE_MIN_H   (_NOTE_MIN_H + 1)  //带间隔行最小高度

/*****************************************************************************
                           内部函数声明
*****************************************************************************/

//---------------------------首次刷屏函数--------------------------------
//初始化结束时先调用
static void _Refresh1st(struct _TImeMng *pIme);

//-------------------------得到下一个有效输入法函数---------------------------------
//根据可用掩码切换到下一个有效输入法
//返回下一个有效输入法
static unsigned char _SwitchType(const struct _TImeMng *pIme);

//--------------------------输入法切换函数---------------------------------
//切换到对应输入法并初始化
static void _SwitchIme(struct _TImeMng *pIme,
                       unsigned char Type);//需切到的输入法,0-4

//-----------------------输入法内部按键处理函数---------------------------------
//pIme->Flag中的TIME_FLAG_STATE置位时调用此函数
//返回值定义为0未响应
//2:输入法内部退出，有结果返回
//3:输入法内部退出，无效返回
static signed char _KeyInType(struct _TImeMng *pIme,
                              char Key);

//-----------------------编辑状态数字键0－9处理函数-----------------------------
//pIme->Flag中的TIME_FLAG_STATE未置位时调用此函数
//返回值定义为:
//0未响应,
//1:进入输入法内部但无结果返回
//2:未入输入法内部但有结果返回
static signed char _NumKeyInEdit(struct _TImeMng *pIme,
                                   char NumKey);//键值'0'-'9'

//------------------得到输入法内部符号函数------------------------------
//各输入法输入完成时，得到并更新编辑器符号
static void _GetImeChar(struct _TImeMng *pIme);

//------------------刷新窗口显示函数---------------------------------
//刷新pWin窗口的显示内容
static void _Refresh(struct _TImeMng *pIme);

/*****************************************************************************
                           对外函数实现
*****************************************************************************/

//----------------------------------初始化函数---------------------------------
//返回是否成功初始化
signed char TImeMng_Init(struct _TImeMng *pIme,  //带入的输入法结构缓冲
                         TWin_t *pWin,             //当前挂接的窗口
                         char *pString,            //用户已存在的字符串,必须以'\0'结尾
                         TIme_String_t Size,     //字符串缓冲区允许大小+1
                         unsigned char DefaultType,   //默认输入法<4
                         unsigned char TypeMask,      //可使用的输入法类型 
                         const char *pSignTbl) //挂接的符号表,为空时使用默认
{
  unsigned char w,h;
  //检查窗口是否够显示
  if(TWin_GetW(pWin) < 16) return -1;//不够显示
  if(TWin_GetH(pWin) < 4) return -1;//不够显示
  
  memset(pIme, 0, sizeof(struct _TImeMng));
  //计算显示偏移
  w = TWin_GetW(pWin);
  if(w > TIME_MNG_MIX_W){
    pIme->DispOffsetX = (w - TIME_MNG_MIX_W) / 2;
    w = TIME_MNG_MIX_W;
  }
  h = TWin_GetH(pWin);
  if(h > TIME_MNG_MAX_H){
    pIme->DispOffsetY = (h - TIME_MNG_MAX_H) / 2;
    h = TIME_MNG_MAX_H;
  }
  pIme->w = w;
  pIme->h = h;
  //初始化除Data区域的成员
  pIme->pWin = pWin;
  TImeEdit_Init(&pIme->Edit,pString,Size,pIme->w);//初始化编辑器  
  pIme->pSignTbl = pSignTbl;  
  if(DefaultType >= 4) DefaultType = 0;//强制纠错
  pIme->TypeMask = TypeMask;
  
  _SwitchIme(pIme,DefaultType);//根据默认输入法类型初始化Data区
  
  //最后控制窗口显示
  //去除可能不需要的颜色
  TImeMng_cbFullStrColor(0xff,0,0,TWin_GetW(pWin));
  _Refresh1st(pIme); //初始化时调用刷新固定不变不部分
  _Refresh(pIme);      //更新显示
  return 0;
}

//----------------------------按键处理函数---------------------------------
//用户按数字键区与导航键区时调用此函数
//返回值定义为: 0正常状态,-1:退出键直接退出;-2:确认键退出
signed char TImeMng_Key(struct _TImeMng *pIme,
                        char Key)          //键值
{
  //只要不是数字键，均强制停止大小写定时器
  if((Key < '0') || (Key > '9'))  pIme->CapitalTimer = 0;
  
  if(!(pIme->Flag & TIME_FLAG_STATE)){//在编辑状态,进行编辑字符串操作
    switch(Key){
    case TGUI_KEY_LEFT:  TImeEdit_CursorLeft(&pIme->Edit); break;
    case TGUI_KEY_RIGHT: TImeEdit_CursorRight(&pIme->Edit); break;
    case TGUI_KEY_DELETE:TImeEdit_Clr(&pIme->Edit); break;
    case TGUI_KEY_ENTER:  return-2;//确认键退出编辑状态
    case TGUI_KEY_ESCAPE: return-1;//退出键直接退出编辑状态
    case '*': //切换到符号输入模式,并进入其内部
      _SwitchIme(pIme, TIME_TYPE_SIGN);
      pIme->Flag |= TIME_FLAG_STATE;
      break;
    case '#'://仅切换输入法
      _SwitchIme(pIme, _SwitchType(pIme));
    default:
      //数字键输入法响应
      if((Key >= '0') && (Key <= '9')){
        switch(_NumKeyInEdit(pIme, Key)){
        //case 0:break;// 0未响应
        case 1://进入输入法内部，置标志
          pIme->Flag |= TIME_FLAG_STATE;
          break;
        case 2: _GetImeChar(pIme);//有结果返回,取得结果
          break;
        default:break;
        }
      }
      break;
    }
  }
  else{//在输入法内部时
    switch(_KeyInType(pIme, Key)){
        //case 0:break;// 0未响应
        case 1: //1:输入法内部不退出，有结果返回
          _GetImeChar(pIme);
          break;
        case 2: //输入法内部退出，有结果返回
          _GetImeChar(pIme);
        case 3: //输入法内部退出，无效返回
          pIme->Flag &= ~TIME_FLAG_STATE;
          //若为符号输入法退出，则强制退到进入前输入法状态
          if(pIme->Type == TIME_TYPE_SIGN)
            _SwitchIme(pIme, pIme->Flag & TIME_PRV_TYPE_MASK);
          break;
        default:break;
    }
  }
  
  _Refresh(pIme);      //更新显示
  return 0;
}


//--------------------------任务函数---------------------------------
//在有输入法时调用此函数实现时间要求
void TImeMng_Task(struct _TImeMng *pIme)
{
  if(!pIme->CapitalTimer) return ;//定时器停止状态
  pIme->CapitalTimer--;
  if(pIme->CapitalTimer) return;//进行中
  //定时时间到时:
  if((pIme->Type == TIME_TYPE_LOWERCASE) || //防止异常操作
     (pIme->Type == TIME_TYPE_CAPITAL)){
       TImeCapital_SampReset(&pIme->Data.Capital);//同键值时复位
       _Refresh(pIme);//重新刷新显示
  }
}

//-----------------------------退出函数---------------------------------
//用户输入字符确认退出后调用此函数
void TImeMng_Quit(struct _TImeMng *pIme)
{
  TWin_Hidden(pIme->pWin);     //关闭显示
  //去除增加的颜色
  TImeMng_cbFullStrColor(0xfe,
                         pIme->DispOffsetY,   //pWin内y坐标
                         pIme->DispOffsetX,   //pWin内x坐标
                         pIme->w);       //x长度
}


/*****************************************************************************
                           内部函数实现
*****************************************************************************/

//-------------------------得到下一个有效输入法函数---------------------------------
//根据可用掩码切换到下一个有效输入法
//返回下一个有效输入法
static unsigned char _SwitchType(const struct _TImeMng *pIme)
{
  unsigned char Type = pIme->Type + 1;//下一输入法
  unsigned char TypeMask = 1 << Type;
  for(; Type < TIME_TYPE_SIGN; Type++,TypeMask <<= 1){
    if(TypeMask & pIme->TypeMask) return Type; //找到了
  }
  //没找到，回环到第一个开始找
  Type = 0;
  TypeMask = 1;
  for(; Type <= pIme->Type; Type++,TypeMask <<= 1){
    if(TypeMask & pIme->TypeMask) break;
  }
  return Type; //找到了
}

//--------------------------得到有效宽度---------------------------------
static unsigned char _GetValidW(const struct _TImeMng *pIme)
{
  unsigned char w = pIme->w;
  if(w >= _BORDER_MIN_H) w -= 2; //可显示左右边界时
  return w;
}

//--------------------------得到输入法内高度---------------------------------
static unsigned char _GetImeH(const struct _TImeMng *pIme)
{
  unsigned char h = pIme->h - 1;           //编辑占一行;
  if(h >= _SPACE_MIN_H) return h - 4;       //间隔+提示+左右边界
  else if(h >= _SPACE_MIN_H) return h - 3; //提示+左右边界
  if(h >= _BORDER_MIN_W) return h - 2;      //仅左右边界
  return h;
}

//--------------------------输入法切换函数---------------------------------
//切换到对应输入法并初始化
static void _SwitchIme(struct _TImeMng *pIme,
                       unsigned char Type)//需切到的输入法,0-4
{


  switch(Type){
  case TIME_TYPE_PINYIN:
    TImePinYin_Init(&pIme->Data.PinYin, _GetValidW(pIme));
    break;
  case TIME_TYPE_NUM:
    TImeNum_Init(&pIme->Data.Num);
    break;
  case TIME_TYPE_LOWERCASE:
    TImeCapital_Init(&pIme->Data.Capital, 0);
    break;
  case TIME_TYPE_CAPITAL:
    TImeCapital_Init(&pIme->Data.Capital, 1);
    break;
  default://符号输入法了
    pIme->Flag &= ~TIME_PRV_TYPE_MASK;//记住上次输入法状态
    pIme->Flag |= pIme->Type;
    TImeSign_Init(&pIme->Data.Sign, 
                  pIme->pSignTbl,
                  _GetValidW(pIme), _GetImeH(pIme));
    break;
  }
  pIme->Type = Type;
}

//-----------------------输入法内部按键处理函数---------------------------------
//pIme->Flag中的TIME_FLAG_STATE置位时调用此函数
//返回值定义为0未响应
//1:输入法内部不退出，有结果返回
//2:输入法内部退出，有结果返回
//3:输入法内部退出，无效返回
static signed char _KeyInType(struct _TImeMng *pIme,
                              char Key)
{
  if(pIme->Type == TIME_TYPE_PINYIN){
    switch(TImePinYin_eKey(&pIme->Data.PinYin, Key)){
    case eTImePinYin_Idie://退出的拼音输入法
       return 3;
    case eTImePinYin_Final://拼音输入法完成
       return 2;
    default: break;  //拼音输入法操作过程中
    }
  }
  else if(pIme->Type == TIME_TYPE_SIGN){
    if((Key >= '1') && (Key <= '9')){
      if(TImeSign_KeyNum(&pIme->Data.Sign, Key) == 1)//输入完成时
        return 1;
    }
    else if((Key == TGUI_KEY_DELETE) || (Key == TGUI_KEY_ESCAPE)){
      if(TImeSign_KeyClr(&pIme->Data.Sign))//无响应时退出输入法
        return 3;
    }
    if((Key == TGUI_KEY_PAGEUP) || (Key == TGUI_KEY_UP))//上一页符号
      TImeSign_KeyPage(&pIme->Data.Sign, 0);
    else if((Key == TGUI_KEY_PAGEDOWN) || (Key == TGUI_KEY_DOWN))//下一页符号
      TImeSign_KeyPage(&pIme->Data.Sign, 1);
    else if(Key == TGUI_KEY_ENTER)//确认键无条件退出
      return 3;
  }
  //TIME_TYPE_NUM://数字输入法不会进入输入法内部，无此状态
  //TIME_TYPE_LOWERCASE://小写输入法不会进入输入法内部，无此状态
  //TIME_TYPE_CAPITAL://大写输入法不会进入输入法内部，无此状态
  return 0;
}

//-----------------------编辑状态数字键0－9处理函数-----------------------------
//pIme->Flag中的TIME_FLAG_STATE未置位时调用此函数
//返回值定义为:
//0未响应,
//1:进入输入法内部但无结果返回
//2:未入输入法内部但有结果返回
static signed char _NumKeyInEdit(struct _TImeMng *pIme,
                                   char NumKey)//键值'0'-'9'
{
  signed char Resume = 0;
  switch(pIme->Type){
  case TIME_TYPE_PINYIN:
    if(NumKey >= '2'){//进入拼音内部并响应首个按键
       TImePinYin_eKey(&pIme->Data.PinYin, NumKey);
       Resume =  1;
    }
    break;
  case TIME_TYPE_NUM://全响应,不进入内部但响应按键
    TImeNum_KeyNum(&pIme->Data.Num, NumKey);
    Resume =  2;
    break;
  case TIME_TYPE_LOWERCASE:
  case TIME_TYPE_CAPITAL:
    TImeCapital_KeyNum(&pIme->Data.Capital, NumKey);
    pIme->CapitalTimer = TIME_CAPITAL_TIME_OV;//重新复位并启动定时器
    Resume =  2; //无条件先获得字符串
    break;
  default:
    break;
  }
  return Resume;
}

//------------------得到输入法内部符号函数------------------------------
//各输入法输入完成时，得到并更新编辑器符号
static void _GetImeChar(struct _TImeMng *pIme)
{
  const char *pChar;
  unsigned short String;
  
  switch(pIme->Type){
  case TIME_TYPE_PINYIN://拼音与符号输入法时
  case TIME_TYPE_SIGN:
    if(pIme->Type == TIME_TYPE_PINYIN)
      pChar = TImePinYin_pGetChar(&pIme->Data.PinYin);
    else
      pChar = TImeSign_pGetChar(&pIme->Data.Sign);
    if(*pChar >= 0x80)//全角字符时
      String = (unsigned short)(*pChar << 8) | *(pChar + 1);
    else
      String = (unsigned short)*(pChar + 1);
    break;
  case TIME_TYPE_NUM:  //数字时，直接获得
    String = (unsigned short)TImeNum_GetNum(&pIme->Data.Num);
    break;
  case TIME_TYPE_LOWERCASE://大小写时,直接获得
  case TIME_TYPE_CAPITAL:
    String = (unsigned short)TImeCapital_GetChar(&pIme->Data.Capital);
    //同一按键有多次时，用新字符替换当前位置字符(先清除之前)
    if(TImeCapital_GetSameKeyIndex(&pIme->Data.Capital) >= 2)
      TImeEdit_Clr(&pIme->Edit);
    break;
  default: break;
  }
  TImeEdit_Add(&pIme->Edit, String);//增加键值
}

//------------------刷新窗口显示函数内部资源---------------------------------
//提示行字符串
static const char *const _pTypeDisp[] = {
 "拼音",
 "123 ",
 "abc ",
 "ABC ",
};

//拼音输入提示行增加字符串
static const char *const _pPinYinTypeDisp[] = {
 "->拼音输入",
 "->拼音选择",
 "->汉字选择",
};

static const char _Line[] = {"─"};
static const char _TopLeft[] = {"┌"};
static const char _TopRight[] = {"┐"};
static const char _BottomLeft[] = {"└"};
static const char _BottomRight[] = {"┘"};
static const char _CenterLeft[] = {"├"};
static const char _CenterRight[] = {"┤"};
static const char *const pLeft[] = {
  _Line,
  _TopLeft,
  _BottomLeft,
  _CenterLeft,
};
static const char *const pRight[] = {
  _Line,
  _TopRight,
  _BottomRight,
  _CenterRight,
};

//-------------------------填充左右边界函数---------------------------------
//返回填充起始边界位置
static char* _pFullVLine(unsigned char w, char *pStr)
{
  if(w >= _BORDER_MIN_W){//可显示左右边界时
    *pStr++ = 0xa9; //│左半
    *pStr++ = 0xa6;//│右半
    memset(pStr, ' ', w - 4);//清除中间部分
    *(pStr + w - 4)= 0xa9; //│左半
    *(pStr + w - 3) = 0xa6;//│右半
  }
  return pStr;
}

//-------------------------填充水平边框函数---------------------------------
//返回填充起始边界位置
static void _FullHBorder(unsigned char w,//填充总宽度
                          char *pStr,      //填充位置
                          unsigned char BorderType)//边界类型,0中间1上2下3中
{
  //填充左边界
  memcpy(pStr, pLeft[BorderType], 2);
  pStr += 2;
  //填充中间
  w = (w - 4) & 0xfe;//双字对齐
  for(; w > 0; w-= 2){
    *pStr++= 0xa9; //─左半
    *pStr++ = 0xa4;//─右半
  }
  //填充右边界
  memcpy(pStr, pRight[BorderType], 2);
}

//---------------------------首次刷屏函数--------------------------------
//初始化结束时先调用
static void _Refresh1st(struct _TImeMng *pIme)
{
  TWin_t *pWin = pIme->pWin;  //当前挂接的窗口
  //清屏(填充空格)
  for(unsigned char Line = 0; Line < TWin_GetH(pWin); Line++){
    memset(TWin_pGetString(pWin, Line),' ',TWin_GetW(pWin));
  }
  unsigned char x = pIme->DispOffsetX;
  unsigned char y = pIme->DispOffsetY;
  unsigned char w = pIme->w;
  
  //可显示上下边界时
  if(pIme->h >= _BORDER_MIN_H){
    if(w >= _BORDER_MIN_W){//可显示左右边界时
      _FullHBorder(w, TWin_pGetString(pWin, y + 0) + x, 1);
      _FullHBorder(w, TWin_pGetString(pWin, y + pIme->h - 1) + x, 2);
      if(pIme->h >= _SPACE_MIN_H)//带间隔行时
        _FullHBorder(w, TWin_pGetString(pWin, y + 2) + x, 3);
    }
    else{//无左右边界
      _FullHBorder(w, TWin_pGetString(pWin, y + 0) + x, 0);
      _FullHBorder(w, TWin_pGetString(pWin, y + pIme->h - 1) + x, 0);
      if(pIme->h >= _SPACE_MIN_H)//带间隔行时
        _FullHBorder(w, TWin_pGetString(pWin, y + 2) + x, 0);
    }
  }
}

//-----------------------------提示行填充函数--------------------------------
static const char _ImeKeyNote[] = {"*或#键切换输入法"};//16字符



//返回填充数量
static unsigned char  _FullNote(struct _TImeMng *pIme,
                                  char *pBuf)
{
  //根据输入法状态，以及可显示长度做提示！
  
  memcpy(pBuf, _ImeKeyNote, 16);
  return 16;
}

//------------------刷新窗口显示函数---------------------------------
//刷新pWin窗口的显示内容
static void _Refresh(struct _TImeMng *pIme)
{
  char *pBuf;
  TWin_t *pWin = pIme->pWin;  //当前挂接的窗口
  unsigned char w = pIme->w;
  unsigned char h = pIme->h;
  unsigned char x = pIme->DispOffsetX;
  unsigned char y = pIme->DispOffsetY; //当前行
  unsigned char xColorStart = x;
  if(w >= _BORDER_MIN_W){//可显示左右边界时
    xColorStart += 2; //左边框开始
  }
  if(h >= _BORDER_MIN_H){//可显示上下边界时去除
    h -= 2;//去上下边框
    y += 1;//边框下一行开始
  }

  //填充第一行:编辑行
  TImeEdit_FullBuf(&pIme->Edit, _pFullVLine(w, TWin_pGetString(pWin, y) + x));
  if(pIme->CapitalTimer){
    //大小写输入法时,若正在输入状态,则光标占两格提示正在输入过程中
    TWin_SetFocus(pWin,
                  TImeEdit_GetCursor(&pIme->Edit) + xColorStart - 1,
                  y, 2);//光标固占2格
  }
  else{
    TWin_SetFocus(pWin,
                  TImeEdit_GetCursor(&pIme->Edit) + xColorStart,
                  y, 1);//光标固占1格
  }
  h--; //去除首行
  y++;//到第二行了
  if(pIme->h >= _SPACE_MIN_H){//带间隔行时不忽略此行
    h--;
    y++;//到下行了    
  }
  if(pIme->h >= _NOTE_MIN_H){//带附加提示行时填充
    unsigned char Len = _FullNote(pIme, 
                                  _pFullVLine(w, TWin_pGetString(pWin, y) + x));
    TImeMng_cbFullStrColor(0xf1, y, xColorStart, 0);//附加提示行着色
    h--;
    y++;//到下行了    
  }
  
  //符号输入法时，填充接下来的所有行
  if(pIme->Type == TIME_TYPE_SIGN){
    for(unsigned char VNum = 0; VNum  < h; VNum++, y++){
      unsigned char Len = TImeSign_GetDispChar(&pIme->Data.Sign, 
                              _pFullVLine(w, TWin_pGetString(pWin, y) + x), VNum);
      TImeMng_cbFullStrColor(TIME_TYPE_SIGN, y, xColorStart,  Len);//由用户处理可支持分别着色
    }
    return;
  }
  //填充固定提示行: 根据当前输入法状态填充
  pBuf = _pFullVLine(w, TWin_pGetString(pWin, y) + x);
  memcpy(pBuf, _pTypeDisp[pIme->Type], 4);
  TImeMng_cbFullStrColor(0xf0, y, xColorStart,  4);//固定提示行着色
  h--;//去除第二行  
  y++;//到第三行了

  //填充编辑器内部: 由具体模块完成填充或补充
  if(pIme->Flag & TIME_FLAG_STATE){
    if(pIme->Type == TIME_TYPE_PINYIN){
      //补充第二行的提示行字符
      enum _eTImePinYin ePinYin = TImePinYin_eGetState(&pIme->Data.PinYin);
      if((ePinYin >= eTImePinYin_Input) && (ePinYin <= eTImePinYin_ChSel)){
        memcpy(pBuf + 4, _pPinYinTypeDisp[ePinYin - 1], 10);
        TImeMng_cbFullStrColor(TIME_TYPE_PINYIN, y, xColorStart + 4,  10);//提示行着色
      }
      unsigned char Len;
      //内部行更新      
      Len = TImePinYin_GetPinYinChar(&pIme->Data.PinYin, 
                                       _pFullVLine(w, TWin_pGetString(pWin, y) + x));
      TImeMng_cbFullStrColor(TIME_TYPE_PINYIN, y, xColorStart,  Len);//由用户处理可支持分别着色
      y++;//到第四行了
      Len = TImePinYin_GetChChar(&pIme->Data.PinYin,
                            _pFullVLine(w, TWin_pGetString(pWin, y) + x));
      TImeMng_cbFullStrColor(TIME_TYPE_PINYIN, y, xColorStart,  Len);//由用户处理可支持分别着色
      h -= 2; //已填充数量
      y++;//到第五行了
    }
  }
  //最后清除余下未用行
  for(; h > 0; h--, y++){
    _pFullVLine(w, TWin_pGetString(pWin, y) + x);
  }
}



