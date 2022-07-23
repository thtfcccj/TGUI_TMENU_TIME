/*****************************************************************************
                         TGUI上的输入法模块总实现
注:暂不支持自带字库全角对齐LCD显示屏的显示
*****************************************************************************/

#include "TImeMng.h"
#include "ClipBoard.h"  //剪切板
#include <string.h>

/*****************************************************************************
                           内部函数声明
*****************************************************************************/

//---------------------------首次刷屏函数--------------------------------
//初始化结束时先调用
static void _Refresh1st(struct _TImeMng *pIme);

//------------------刷新窗口显示函数---------------------------------
//刷新pWin窗口的部分需要实时更新的内容
static void _Refresh(struct _TImeMng *pIme);

//----------------------------状态改变函数---------------------------------
//刷新pWin窗口的部分需要实时更新的内容
#define _ChangeState(ime, nextState) do{(ime)->State = nextState; }while(0)

//----------------------键值转换为内部导航ID号函数-------------------------
//返回的GuideKey定义为：0上1左2确认3右4下,5退出，其它无效
static unsigned char _Key2GuideKey(char Key)
{
  //为避免键值可能的修改(不连接)，用 CASE实现
  switch(Key){
    case TGUI_KEY_UP:      return TIME_MNG_KEY_UP;
    case TGUI_KEY_LEFT:    return TIME_MNG_KEY_LEFT;
    case TGUI_KEY_ENTER:   return TIME_MNG_KEY_ENTER;
    case TGUI_KEY_RIGHT:   return TIME_MNG_KEY_RIGHT;
    case TGUI_KEY_DOWN:    return TIME_MNG_KEY_DOWN;
    case TGUI_KEY_ESCAPE:  return TIME_MNG_KEY_EXIT;
  }
  return 0xff; 
}

//----------------------------编辑模式按键实现---------------------------------
//返回0:不退出,非0保存退出
static signed char _EditKey(struct _TImeMng *pIme,
                            unsigned char GuideKey)//导航键值0-5
{
  switch(GuideKey){
    case TIME_MNG_KEY_UP: //插入↑,到输入法选择模式
      _ChangeState(pIme, TIME_MNG_STATE_SEL); break;  
    case TIME_MNG_KEY_LEFT: //左移←
      TImeEdit_CursorLeft(&pIme->Edit); break;
    case TIME_MNG_KEY_ENTER: //●保存退出 
      return 1; 
    case TIME_MNG_KEY_RIGHT: //→右移
      TImeEdit_CursorRight(&pIme->Edit); break;   
    case TIME_MNG_KEY_DOWN: //删除↓
      TImeEdit_Clr(&pIme->Edit); break;         
    case TIME_MNG_KEY_EXIT: // ○直接退出,到提示
      _ChangeState(pIme, TIME_MNG_STATE_FORCE_EXIT); break;
    default: break;
  }
  return 0; //0:不退出
}

//-------------------------强制退出提示模式按键实现-----------------------------
//返回0:不退出, 非0保存退出
static signed char _ForceExitKey(struct _TImeMng *pIme,
                                 unsigned char GuideKey)//导航键值0-5
{
  if(GuideKey == TIME_MNG_KEY_ENTER) return -1;//强制退出
  if(GuideKey == TIME_MNG_KEY_EXIT){//返回编辑模式
    _ChangeState(pIme, TIME_MNG_STATE_EDIT);
    return 0;//不退出  
  }
  
  //剪切板操作:粘贴
  if(GuideKey == TIME_MNG_KEY_RIGHT){
    const char *pClipBuf = ClipBoard.Buf;
    ClipBoardSizt_t StrLen = strlen(pClipBuf);
    for(; StrLen > 0; StrLen--){
      unsigned short Char = *pClipBuf++;
      if((Char >= 0x80) && (StrLen)){//全角时
        Char <<= 8;
        Char |= *pClipBuf++;
        StrLen--;
      }
      TImeEdit_Add(&pIme->Edit, Char);
    }
    return 0;//不退出
  }
  
  //剪切板操作:复制相关：
  ClipBoardSizt_t Len =  TImeEdit_GetCurLen(&pIme->Edit);  //所有
  ClipBoardSizt_t Cursor =  TImeEdit_GetCurCursor(&pIme->Edit);
  const char *pClipBuf = TImeEdit_pGetCurStr(&pIme->Edit);
  if(GuideKey == TIME_MNG_KEY_UP){//复制左侧
    Len = Cursor;
  }
  else if(GuideKey == TIME_MNG_KEY_DOWN){//复制右侧
    Len -= Cursor;
    pClipBuf += Cursor;
  }
    
  if(Len){//有字符时
    if(Len > (CLIP_BOARD_BUF_SIZE - 1)) //防止超限
      Len = (CLIP_BOARD_BUF_SIZE - 1);
    memcpy(ClipBoard.Buf, pClipBuf, Len);
    ClipBoard.Buf[Len] = '\0';//强制结束字符
  }
  return 0;//不退出  
}

//--------------------------输入法选择模式按键实现------------------------------
static void _ImeSelKey(struct _TImeMng *pIme,
                       unsigned char GuideKey)//导航键值0-5
{
  //对应输出法未被开启时，按键无效
  switch(GuideKey){
    case TIME_MNG_KEY_UP: //A↑
      if(!(pIme->TypeMask & (1 << TIME_MNG_TYPE_CAPITAL))) return;
      pIme->Type = TIME_MNG_TYPE_CAPITAL;
      TImeCapital_Init(&pIme->Data.Capital, 1); break;  
    case TIME_MNG_KEY_LEFT: //,.←
      if(!(pIme->TypeMask & (1 << TIME_MNG_TYPE_SIGN))) return;
      pIme->Type = TIME_MNG_TYPE_SIGN;
      TImeSign_Init(&pIme->Data.Sign, pIme->pSignTbl); break;
    case TIME_MNG_KEY_ENTER: //●拼音
      if(!(pIme->TypeMask & (1 << TIME_MNG_TYPE_PINYIN))) return;
      pIme->Type = TIME_MNG_TYPE_PINYIN;      
      TImePinYin_Init(&pIme->Data.PinYin); break; 
    case TIME_MNG_KEY_RIGHT: //→0-9
      if(!(pIme->TypeMask & (1 << TIME_MNG_TYPE_NUM))) return;   
      pIme->Type = TIME_MNG_TYPE_NUM;         
      TImeNum_Init(&pIme->Data.Num); break;   
    case TIME_MNG_KEY_DOWN: //a↓
      if(!(pIme->TypeMask & (1 << TIME_MNG_TYPE_LOWERCASE))) return; 
      pIme->Type = TIME_MNG_TYPE_LOWERCASE;         
      TImeCapital_Init(&pIme->Data.Capital, 0); break;        
    case TIME_MNG_KEY_EXIT: // ○返回编辑模式
      _ChangeState(pIme, TIME_MNG_STATE_EDIT); return;
    default: break;
  }
  
  _ChangeState(pIme, TIME_MNG_STATE_IME);//到输入法模式了
}

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
                         const char *pSignTbl,        //挂接的符号表,为空时使用默认
                         TIme_String_t Cursor)//默认光标位置,-1时为最后
{
  //检查窗口是否够显示
  if(TWin_GetW(pWin) < TIME_MNG_DISP_W) return -1;//不够显示
  if(TWin_GetH(pWin) < TIME_MNG_DISP_H) return -1;//不够显示
  memset(pIme, 0,sizeof(struct _TImeMng)); 
  pIme->State = TIME_MNG_STATE_EDIT; //进入空闲模式
  
  //计算显示偏移
  if(TWin_GetW(pWin) > TIME_MNG_DISP_W)
    pIme->DispOffsetX = (TWin_GetW(pWin) - TIME_MNG_DISP_W) / 2;
  if(TWin_GetH(pWin)> TIME_MNG_DISP_H)
    pIme->DispOffsetY = (TWin_GetH(pWin) - TIME_MNG_DISP_H) / 2;
  //初始化除Data区域的成员
  pIme->pWin = pWin;
  TImeEdit_Init(&pIme->Edit,pString ,Size, //初始化编辑器  
                TIME_MNG_DISP_W - 4, Cursor);  //去除前后两制表符(全角)
  pIme->pSignTbl = pSignTbl;
  pIme->TypeMask = TypeMask;
  
  //去除可能不需要的颜色
  TImeMng_cbFullStrColor(0xff,0,0,TWin_GetW(pWin));
  _Refresh1st(pIme); //初始化时调用刷新固定不变不部分
  _Refresh(pIme);    //变动部分首次刷新
  return 0;
}

//-----------------------由pIme->Type转换为GuideKey值--------------------------
static const unsigned char _TypeGuideKey[] = {
  TIME_MNG_KEY_ENTER,    //拼音输入法
  TIME_MNG_KEY_RIGHT,    //数字输入法
  TIME_MNG_KEY_DOWN,    //小写字母输入法
  TIME_MNG_KEY_UP,     //大写字母输入法
  TIME_MNG_KEY_LEFT,    //符号输入法
};

//----------------------------按键处理函数---------------------------------
//返回值定义为: 0正常状态,-1:退出键直接退出;-2:确认键退出
signed char TImeMng_Key(struct _TImeMng *pIme,
                         char Key)          //键值,仅响应上下左右进出6键
{
  unsigned char GuideKey = _Key2GuideKey(Key);
  if(GuideKey >= TIME_MNG_KEY_COUNT) return 0; //非导航键值
  
  unsigned char State = pIme->State;
  if(State == TIME_MNG_STATE_EDIT){//编辑模式时
    if(_EditKey(pIme,GuideKey)){//保存退出
      TImeMng_Quit(pIme);
      return -2; 
    }
  }
  else if(State == TIME_MNG_STATE_FORCE_EXIT){//强制退出按键
    if(_ForceExitKey(pIme, GuideKey)){//退出键直接退出
      TImeMng_Quit(pIme);
      return -1;
    }
  }
  else if(State == TIME_MNG_STATE_SEL)//输入法选择模式
    _ImeSelKey(pIme, GuideKey);
  else{
    //===================输入法模式由键值处理及后续========================
    //各子模块按键处理
    unsigned short ResumeString;
    switch(pIme->Type){
      case TIME_MNG_TYPE_CAPITAL: 
      case TIME_MNG_TYPE_LOWERCASE:         
        ResumeString = TImeCapital_Key(&pIme->Data.Capital, GuideKey); break;
      case TIME_MNG_TYPE_NUM:         
        ResumeString = TImeNum_Key(&pIme->Data.Num, GuideKey); break;        
      case TIME_MNG_TYPE_SIGN:         
        ResumeString = TImeSign_Key(&pIme->Data.Sign, GuideKey); break;  
      case TIME_MNG_TYPE_PINYIN:         
        ResumeString = TImePinYin_Key(&pIme->Data.PinYin, GuideKey); break;          
      default:break;
    }
    //返回结果处理：
    if(ResumeString == 0)//不修改返回时,即返回到上一级(输入法选择状态)
      _ChangeState(pIme, TIME_MNG_STATE_SEL);
    else 
      if(ResumeString == 0xffff){}//内部未完成
    else{//输入字符完成时
      TImeEdit_Add(&pIme->Edit, ResumeString); //插入输入的字符
      if(pIme->Type != TIME_MNG_TYPE_NUM)//数字时允许连续
        _ImeSelKey(pIme, _TypeGuideKey[pIme->Type]); //再次进入继续输入
    }
  }
  _Refresh(pIme);      //更新显示
  return 0;
}

//--------------------------任务函数---------------------------------
//在有输入法时调用此函数实现时间要求
void TImeMng_Task(struct _TImeMng *pIme)
{
  unsigned char State = pIme->State;
  if(State != TIME_MNG_STATE_IME) return;
  if(pIme->Type == TIME_MNG_TYPE_SIGN) return; //符号输入法不需要

  unsigned short ResumeString;
  switch(pIme->Type){
      case TIME_MNG_TYPE_CAPITAL: 
      case TIME_MNG_TYPE_LOWERCASE:         
       ResumeString = TImeCapital_Task(&pIme->Data.Capital); 
       break;
      case TIME_MNG_TYPE_NUM:         
       ResumeString = TImeNum_Task(&pIme->Data.Num);     
       break;  
      case TIME_MNG_TYPE_PINYIN:         
       ResumeString = TImePinYin_Task(&pIme->Data.PinYin);     
       break;         
      default:break;
  }
  if(ResumeString == 0xffff)//强制更新显示
    _Refresh(pIme);   
  else if(ResumeString != 0){//有自动确认结果了，表示输入字符完成
    TImeEdit_Add(&pIme->Edit, ResumeString); //插入输入的字符
    _ImeSelKey(pIme, _TypeGuideKey[pIme->Type]); //再次进入继续输入
    _Refresh(pIme);      //更新显示
  }
}

//-----------------------------退出函数---------------------------------
//用户输入字符确认退出后调用此函数
void TImeMng_Quit(struct _TImeMng *pIme)
{
  if(pIme->State == TIME_MNG_STATE_IDIE) return; //空闲模式
  //去除增加的颜色
  TImeMng_cbFullStrColor(0xfe,
                         pIme->DispOffsetY + 4,   //pWin内y坐标,4为箭头起始行
                         pIme->DispOffsetX,       //pWin内x坐标
                         TIME_MNG_DISP_W);       //x长度
  pIme->State = TIME_MNG_STATE_IDIE; //空闲了
}

/*****************************************************************************
                           内部刷屏函数相关实现
*****************************************************************************/

//---------------------------默认字符填充------------------------------------
  //示例:
  //  ┌─────────────── ┐起始行
  //  │　输入设备| 　　　　　　　　　 | 编辑行
  //  ├───────────────┤中间行
  //  │编辑模式: 　　　　　　        | 用户提示
  //  │　　　　插入 ↑ ●保存退出    | 上箭头行
  //  │　　　左移←●→右移　        | 左右箭头行
  //  │　　　　删除↓ ○直接退出     | 下箭头行
  //  └───────────────┘最末行
//起始行
static const char _TopLine[] = \
  {"┌─────────────┐"};
//中间行
static const char _MiddleLine[] = \
  {"├─────────────┤"};
//上箭头行
static const char _UpArrowLine[] = \
  {"│           ↑●           │"};
//左右箭头行
static const char _LeftRightArrowLine[] = \
  {"│         ←●→           │"};
//左右箭头行,需要字符较多时(符号输入法使用)
static const char _LeftRightArrowLine2[] = \
  {"│          ←→            │"};
//下箭头行
static const char _DownArrowLne[] = \
  {"│           ↓○           │"};
//最末行
static const char _BottomLine[] = \
  {"└─────────────┘"};
//箭头标识在字符中的位置
static const unsigned char _ArrowPosLUT[] = {
  13,17,
  11,17,
  13,17,
};

static const char *const _ArrowLine[] = {
  _UpArrowLine,
  _LeftRightArrowLine,
  _DownArrowLne,
};

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
  //填充第一行
  memcpy(TWin_pGetString(pWin, y + 0) + x, _TopLine, TIME_MNG_DISP_W);  
  //填充第三行
  memcpy(TWin_pGetString(pWin, y + 2) + x, _MiddleLine, TIME_MNG_DISP_W);
  //填充第八行: 最末行
  memcpy(TWin_pGetString(pWin, y + 7) + x, _BottomLine, TIME_MNG_DISP_W);
}

//-------------------------填充左右边界函数---------------------------------
//返回填充起始边界位置
static char* _pFullVLine(char *pStr)
{
  *pStr++ = 0xa9; //│左半
  *pStr++ = 0xa6;//│右半
  memset(pStr, ' ', TIME_MNG_DISP_W - 4);//清除中间部分
  *(pStr + TIME_MNG_DISP_W - 4)= 0xa9; //│左半
  *(pStr + TIME_MNG_DISP_W - 3) = 0xa6;//│右半
  return pStr;
}

//------------------------------编辑模式相关字符资源--------------------------
static const char _chEditMode[] = {"编辑模式:"};
static const char _chEditUp[] =    {"插入"};
static const char _chEditSave[] =  {"保存退出"};
static const char _chEditLeft[] =  {"左移"};
static const char _chEditRight[] = {"右移"};
static const char _chEditDown[] =  {"删除"};
static const char _chEditQuit[] =  {"直接退出"};
static const char *const _EditArrow[] = {
  _chEditUp, _chEditSave,
  _chEditLeft, _chEditRight,
  _chEditDown, _chEditQuit,
};

//------------------------------强制退出提示模式字符资源--------------------------
static const char _chFourceExitMode[] = {"剪切板操作:"};// 不保存退出吗?
static const char _chFourceExitEnter[] = {"不保存退出!"};
static const char _chFourceExitReturn[] =  {"返回继续"};
//剪切操作
static const char _chCopyLeft[] =       {"复制光标前"};
static const char _chCopyRight[] =      {"复制光标后"};
static const char _chCopyAll[] =        {"复制所有"};
static const char _chPease[] =          {"粘贴"};

static const char *const _FourceExitArrow[] = {
  _chCopyLeft, _chFourceExitEnter,  //第一行
  _chCopyAll, _chPease,             //第二行
  _chCopyRight, _chFourceExitReturn,//第三行
};

//---------------------------输入法选择模式相关字符资源--------------------------
static const char _chImeSel[] = {"输入法选择:"};
static const char _chImeSelUp[] =    {"ABC"};
static const char _chImeSelSave[] =  {"拼音"};
static const char _chImeSelLeft[] =  {"符号"};
static const char _chImeSelRight[] = {"0-9"};
static const char _chImeSelDown[] =  {"abc"};
static const char *const _ImeSelArrow[] = {
  _chImeSelUp, _chImeSelSave,
  _chImeSelLeft, _chImeSelRight,
  _chImeSelDown, chReturn,
};

//--------------------------------填充第四行用户提示-----------------------------
static void _FullNotice(struct _TImeMng *pIme, 
                        char *pStr)
{
  unsigned char State = pIme->State;
  if(State == TIME_MNG_STATE_EDIT)//编辑模式时
    memcpy(pStr, _chEditMode, sizeof(_chEditMode) - 1);
  else if(State == TIME_MNG_STATE_FORCE_EXIT)//强制退出模式时
    memcpy(pStr, _chFourceExitMode, sizeof(_chFourceExitMode) - 1);
  else if(State == TIME_MNG_STATE_SEL)//输入法选择模式
    memcpy(pStr, _chImeSel, sizeof(_chImeSel) - 1);
  else{//输入法模式由各子模块决定(★各模块实现调用)
    switch(pIme->Type){
      case TIME_MNG_TYPE_CAPITAL:    //大写字母输入法
      case TIME_MNG_TYPE_LOWERCASE:  //小写字母输入法      
        TImeCapital_FullTitle(&pIme->Data.Capital, pStr); break;
      case TIME_MNG_TYPE_NUM:        //数字输入法
        TImeNum_FullTitle(&pIme->Data.Num, pStr); break;
      case TIME_MNG_TYPE_SIGN:       //符号输入法
        TImeSign_FullTitle(&pIme->Data.Sign, pStr); break;
      case TIME_MNG_TYPE_PINYIN:     //拼音输入法 
        TImePinYin_FullTitle(&pIme->Data.PinYin, pStr); break; 
      default:break;
    }
  }
}

//-----------------------由箭头提示行转转换为GuideKeyId--------------------------
static const unsigned char _Arrow2GuideKey[] = {
  TIME_MNG_KEY_UP, TIME_MNG_KEY_ENTER,  
  TIME_MNG_KEY_LEFT, TIME_MNG_KEY_RIGHT,
  TIME_MNG_KEY_DOWN, TIME_MNG_KEY_EXIT
};

//--------------------------由GuideKey查找对应输入法掩码-----------------------
static const unsigned char _GuideKey2TypeMask[] = {
  (1 <<TIME_MNG_TYPE_CAPITAL),  // TIME_MNG_KEY_UP 
  (1 <<TIME_MNG_TYPE_SIGN),     //TIME_MNG_KEY_LEFT
  (1 <<TIME_MNG_TYPE_PINYIN),   //TIME_MNG_KEY_ENTER
  (1 <<TIME_MNG_TYPE_NUM),      //TIME_MNG_KEY_RIGHT
  (1 <<TIME_MNG_TYPE_LOWERCASE), //TIME_MNG_KEY_DOWN
};

//------------------------------填充箭头所在行----------------------------------
//此函数只填充一行
static void _FullArrowLine(struct _TImeMng *pIme, 
                            unsigned char Arrow,//0-2行
                            unsigned char DispOffset,//偏移位置
                            char *pStr)
{
  unsigned char State = pIme->State;
  //获得左右侧字符串如："插入↑●保存退出",左: 插入   右:保存退出
  unsigned char y = pIme->DispOffsetY + Arrow + 4; //所在行
  Arrow *= 2;
  //====================输入法模式由各子模块要对应位置字符======================
  for(unsigned char ArrowR = Arrow + 1; Arrow <= ArrowR; Arrow++){
    unsigned char GuideKey = _Arrow2GuideKey[Arrow];
    const char *pRdStr = NULL;
    if(State == TIME_MNG_STATE_EDIT){//编辑模式时
      pRdStr = _EditArrow[Arrow];     
    }
    else if(State == TIME_MNG_STATE_FORCE_EXIT){//强制退出模式时
      pRdStr = _FourceExitArrow[Arrow];     
    }
    else if(State == TIME_MNG_STATE_SEL){//输入法选择模式
      //非退出键时，在输入法被允许时，才能选择
      if((GuideKey == TIME_MNG_KEY_EXIT) || 
        (pIme->TypeMask & _GuideKey2TypeMask[GuideKey]))
          pRdStr = _ImeSelArrow[Arrow];
    }
    else{//输入法模式时
      switch(pIme->Type){
      case TIME_MNG_TYPE_CAPITAL:
      case TIME_MNG_TYPE_LOWERCASE: 
        pRdStr = TImeCapital_pGetArrowNotice(&pIme->Data.Capital, GuideKey); 
        break;
      case TIME_MNG_TYPE_NUM: 
        pRdStr = TImeNum_pGetArrowNotice(&pIme->Data.Num, GuideKey); 
        break;      
      case TIME_MNG_TYPE_SIGN: 
        pRdStr = TImeSign_pGetArrowNotice(&pIme->Data.Sign, GuideKey); 
        break;
      case TIME_MNG_TYPE_PINYIN:{ 
        pRdStr = TImePinYin_pGetArrowNotice(&pIme->Data.PinYin, GuideKey); 
        break; 
       }
       default:break;
      }
    }
    if(pRdStr == NULL) continue; //不需填充
    
    if(Arrow < ArrowR){//左侧填充
      if(pRdStr != NULL){
        unsigned char StrLLen = strlen(pRdStr);
        unsigned char Pos = _ArrowPosLUT[Arrow] + DispOffset - StrLLen;
        memcpy(pStr + Pos, pRdStr, StrLLen);
        TImeMng_cbFullStrColor(GuideKey,
                               y, pIme->DispOffsetX + Pos, StrLLen + 2);//箭头一起
      }
    }
    else if(pRdStr != NULL){//右侧填充
      unsigned char StrLLen = strlen(pRdStr);
      unsigned char Pos = _ArrowPosLUT[Arrow] - DispOffset;
      memcpy(pStr + Pos, pRdStr, strlen(pRdStr));
      TImeMng_cbFullStrColor(GuideKey, 
                             y, pIme->DispOffsetX + Pos - 2, StrLLen + 2);//箭头一起
    }
  }
}

//-----------------------------刷新窗口显示函数---------------------------------
//刷新pWin窗口的部分需要实时更新的内容(即只更新变化部分)
static void _Refresh(struct _TImeMng *pIme)
{
  TWin_t *pWin = pIme->pWin;  //当前挂接的窗口
  unsigned char x = pIme->DispOffsetX;
  unsigned char y = pIme->DispOffsetY;

  //填充第二行:编辑行
  TImeEdit_FullBuf(&pIme->Edit, _pFullVLine(TWin_pGetString(pWin, y + 1) + x));
  //光标位置
  TWin_SetFocus(pWin,
                  x + TImeEdit_GetCursor(&pIme->Edit) + 2,
                  y + 1, 1);//光标固占1格

  //填充第四行: 用户提示行
  _FullNotice(pIme, _pFullVLine(TWin_pGetString(pWin, y + 3) + x));
  //填充第五行:上箭头行、第六行:左右箭头行与第七行:下箭头行
  y += 4;  
  for(unsigned char ArrowLne = 0; ArrowLne < 3; ArrowLne++){
    char *pStr = TWin_pGetString(pWin, y + ArrowLne) + x;
    if((ArrowLne == 1) && //左右箭头行时，在符号输入法模式需精简以显示下符号
       (pIme->State == TIME_MNG_STATE_IME) && (pIme->Type == TIME_MNG_TYPE_SIGN)){
      memcpy(pStr, _LeftRightArrowLine2, TIME_MNG_DISP_W);
      _FullArrowLine(pIme, ArrowLne, 1, pStr);
    }
    else{
      memcpy(pStr, _ArrowLine[ArrowLne], TIME_MNG_DISP_W);//填充默认
      _FullArrowLine(pIme, ArrowLne, 0, pStr);
    }
    
  }
}
