/*****************************************************************************
                         TGUI上的输入法模块总实现
注:暂不支持自带字库全角对齐LCD显示屏的显示
*****************************************************************************/

#include "TImeMng.h"
#include <string.h>

/*****************************************************************************
                           内部函数声明
*****************************************************************************/
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
  unsigned char Data;
  //检查窗口是否够显示
  if(TWin_GetW(pWin) < 16) return -1;//不够显示
  if(TWin_GetH(pWin) < 4) return -1;//不够显示
  
  memset(pIme, 0, sizeof(struct _TImeMng));
  //有效宽度初始化
  Data = TWin_GetW(pWin);
  if(Data >= 18) Data-= 2;//去除左右边界符号"|"
  pIme->w = Data;
  //有效高度初始化
  Data = TWin_GetH(pWin);
  pIme->h = Data;
  //初始化除Data区域的成员
  pIme->pWin = pWin;
  TImeEdit_Init(&pIme->Edit,pString,Size,pIme->w);//初始化编辑器  
  pIme->pSignTbl = pSignTbl;  
  if(DefaultType >= 4) DefaultType = 0;//强制纠错
  pIme->TypeMask = TypeMask;
  
  _SwitchIme(pIme,DefaultType);//根据默认输入法类型初始化Data区
  
  //最后控制窗口显示

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

//--------------------------输入法切换函数---------------------------------
//切换到对应输入法并初始化
static void _SwitchIme(struct _TImeMng *pIme,
                       unsigned char Type)//需切到的输入法,0-4
{
  switch(Type){
  case TIME_TYPE_PINYIN:
    TImePinYin_Init(&pIme->Data.PinYin, pIme->w);
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
                  pIme->w,
                  3);
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

//------------------填充左右边界函数---------------------------------
//返回填充边界后的位置
static char *_pFullVLine(char *pBuf,
                         unsigned char VFlag,//坚分隔标志
                         unsigned char w)
{
  if(VFlag){
    *(pBuf + w - 1) = '|';   
    *pBuf++ = '|';
  }
  return pBuf;
}


//------------------刷新窗口显示函数---------------------------------
//刷新pWin窗口的显示内容
static void _Refresh(struct _TImeMng *pIme)
{
  unsigned char v;//行位置
  unsigned char VSpace;//有边界符号用占位标志
  unsigned char DispW,DispH;//显示宽度,显示高度
  char *pBuf;
  TWin_t *pWin = pIme->pWin;  //当前挂接的窗口
  
  DispW = TWin_GetW(pWin);
  if(DispW >= 18) VSpace = 2;//有边界符号且占两格
  else VSpace = 0;
  
  //==================显示上下边界=====================
  //IME需4行,>6行则可显示边界
  if(pIme->h >= 6){
    pBuf = TWin_pGetString(pWin, 0);
    memset(pBuf,'-',DispW);
    *pBuf = ' '; //前空格
    *(pBuf + DispW -1) = ' ';//后空格
    
    pBuf = TWin_pGetString(pWin, 5);
    memset(pBuf,'-',DispW);
    *pBuf = ' '; //前空格
    *(pBuf + DispW -1) = ' ';//后空格
    
    //>6直接填充空格
    for(v = 6; v < pIme->h; v++){
      pBuf = TWin_pGetString(pWin, v);
      memset(pBuf,' ',DispW);
    }
    v = 1; 
  }
  else{//无边界
    v = 0; 
  }
  
  //==================首行编辑器填充并置焦点=====================
  pBuf = TWin_pGetString(pWin, v);
  TImeEdit_FullBuf(&pIme->Edit, _pFullVLine(pBuf,VSpace,DispW));
  //大小写输入法时,若正在输入状态,则光标占两格提示正在输入过程中
  if(pIme->CapitalTimer){
    TWin_SetFocus(pWin,
                  TImeEdit_GetCursor(&pIme->Edit) + VSpace - 2,
                  v,2);//光标固占2格
  }
  else{
    TWin_SetFocus(pWin,
                  TImeEdit_GetCursor(&pIme->Edit) + VSpace - 1,
                  v,1);//光标固占1格
  }
  //==================提示行根据当前输入法状态填充=================
  v++;
  if(pIme->Type < TIME_TYPE_SIGN){
    pBuf = TWin_pGetString(pWin, v);
    pBuf = _pFullVLine(pBuf,VSpace,DispW);
    memcpy(pBuf, _pTypeDisp[pIme->Type], 4);
    pBuf += 4;//指向提示后面
    memset(pBuf, ' ', DispW - VSpace - 4);
    v++;
  }
  //============在编辑器内部时,其它行由具体模块完成填充或补充=========
  if(pIme->Flag & TIME_FLAG_STATE){
    if(pIme->Type == TIME_TYPE_PINYIN){
      //补充提示行字符
      enum _eTImePinYin ePinYin = TImePinYin_eGetState(&pIme->Data.PinYin);
      if((ePinYin >= eTImePinYin_Input) && (ePinYin <= eTImePinYin_ChSel))
        memcpy(pBuf,_pPinYinTypeDisp[ePinYin - 1],10);
      //内部行更新      
      pBuf = TWin_pGetString(pWin, v);
      pBuf = _pFullVLine(pBuf,VSpace,DispW);
      TImePinYin_pGetPinYinChar(&pIme->Data.PinYin, pBuf);
      pBuf = TWin_pGetString(pWin, v + 1);
      pBuf = _pFullVLine(pBuf,VSpace,DispW);
      TImePinYin_pGetChChar(&pIme->Data.PinYin,pBuf);
     return;
    }
    //符号输入法行填充
    if(pIme->Type == TIME_TYPE_SIGN){
      DispH = 0; //暂用
      for(; DispH  < 3; v++, DispH++){//固定为三行
        pBuf = TWin_pGetString(pWin, v);
        pBuf = _pFullVLine(pBuf,VSpace,DispW);
        TImeSign_pGetDispChar(&pIme->Data.Sign, pBuf, DispH);
      }
      return;
    }
  }
  //============不在输入法内部或其它输入法时其它行填充空格=========
  if(pIme->h >= 6) DispH = 5; //有效结束行
  else DispH = pIme->h;//无边界
  for(; v < DispH; v++){
    pBuf = TWin_pGetString(pWin, v);
    pBuf = _pFullVLine(pBuf,VSpace,DispW);
    memset(pBuf, ' ', DispW - VSpace);
  }
}
