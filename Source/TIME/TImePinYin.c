/*****************************************************************************
                         TGUI上的输入法模块-符号输入法部分
*****************************************************************************/

#include "TImePinYin.h"
#include "TGUIBase.h" //KEY
//导航键
#include <string.h>

/*****************************************************************************
                        相关函数
*****************************************************************************/

//---------------------------初始化函数---------------------------------
//切换到符号输入法时调用
void TImePinYin_Init(struct _TImePinYin *pPinYin,//输入法结构
                     unsigned char DispW)        //显示宽度,>16
{
  memset(pPinYin, 0, sizeof(struct _TImePinYin));//先初始化到0
  pPinYin->DispW = DispW;
}

//----------------------填充拼音显示行字符串函数---------------------------
//填充示例:"1pen 2ren 3sen", 返回填充数量
unsigned char TImePinYin_GetPinYinChar(struct _TImePinYin *pPinYin,
                                         char *pBuf)              //被填充的字符
{
  unsigned char FindCount;
  const struct _PyIndex *const *ppPyIndex;
  unsigned char PyCount;  
  unsigned char Pos = 0; //拼音内部位置
  char *pEndBuf = pBuf + pPinYin->DispW;
  
  ppPyIndex = NumKeyPyIme_pGetCur(&pPinYin->Ime, &FindCount) + 
               pPinYin->PinYinStart;//指向所需索引表的有效起始位置
  FindCount -= pPinYin->PinYinStart;
  while(FindCount){//还有拼音要显示时,一个个填充拼音
    PyCount = strlen((const char*)(*ppPyIndex)->Py);//拼音
    if((PyCount + 1) <= (pEndBuf - pBuf)){//数字+拼音填充,直到拼音显示不下时为止
      //当前拼音字符位置时，缓冲此项以备用
      if(Pos == pPinYin->CurPinYin){
        pPinYin->CurPinYinStrStart = pBuf - (pEndBuf - pPinYin->DispW);
        pPinYin->CurPinYinStrCount =  PyCount + 1; //含序号提示       
      }
      //准备字符
      *pBuf++ = Pos + '1'; //拼音前提示
      memcpy(pBuf, (*ppPyIndex)->Py, PyCount);
      pBuf += PyCount;   
      if(pBuf < (pEndBuf - 1)) *pBuf++ = ' '; //有位置时空格间隔
      //为下次算做准备
      FindCount--;
      ppPyIndex++;
      Pos++;
    }
    else break; //一行显示不下下个拼音了
  }
  return pBuf - (pEndBuf - pPinYin->DispW);
}

//----------------------填充汉字显示行字符串函数---------------------------
//填充示例:"人1 仁2 壬3 忍4", 返回填充数量
unsigned char TImePinYin_GetChChar(struct _TImePinYin *pPinYin,
                                     char *pBuf)              //被填充的字符
{
  unsigned char FindCount;
  const unsigned char *Pymb;
  unsigned char Len;
  unsigned char Pos; //汉字内部位置
  unsigned char W = (pPinYin->DispW >> 2);//每页字符个数
  char *pStartBuf = pBuf;
  if(W > 9) W = 9; //一页最大允许显示9个以对应数字键
  
  Pymb = (*(NumKeyPyIme_pGetCur(&pPinYin->Ime, &FindCount) + 
               pPinYin->CurPinYin))->Pymb;//指向所需拼音码表位置
  Len = strlen((const char*)Pymb) - pPinYin->ChStart;//当前页字符长度
  Pymb += pPinYin->ChStart; //指向当前页的汉字
  
  //填充
  for(Pos = 0; Pos < W; Pos++){
    if(Len){//还有需显示的汉字时
      Len -= 2; //全角
      *pBuf++ = *Pymb++; //上半字
      *pBuf++ = *Pymb++; //下半字
      *pBuf++ = Pos + '1';//汉字提示
      *pBuf++ = ' ';//间隔 
    }
  }
  return pBuf - pStartBuf;
}

//----------------------得到当前用户选择字符函数---------------------------
//在输入完成状态后才能调用，调用后内部状态机自动复位到空闲状态
//返回汉字前两位位置
const char *TImePinYin_pGetChar(struct _TImePinYin *pPinYin)
{
  unsigned char FindCount;
  const unsigned char *pString;
  if(pPinYin->eState != eTImePinYin_Final) return NULL;
  
  //指向所需汉字位置
  pString = (*(NumKeyPyIme_pGetCur(&pPinYin->Ime, &FindCount) + 
               pPinYin->CurPinYin))->Pymb + pPinYin->CurCh;
  //最后内部重新初始化
  TImePinYin_Init(pPinYin, pPinYin->DispW);
  
  return (const char*)pString;
}

/*****************************************************************************
                        键值处理相关函数
*****************************************************************************/

//------------拼音输入选择状态上下页键响应函数---------------------------------
static void _PinYinSelKeyUPDown(struct _TImePinYin *pPinYin,
                                char UpFlag) //上页标志,否则为下页
{
  unsigned char PinYinStart = pPinYin->PinYinStart;
  unsigned char CurPage = pPinYin->CurPage;
  
  if(UpFlag){//向上换页
    if(!PinYinStart) return; //已在第一页了
    CurPage--;    
    PinYinStart -= pPinYin->MaxPerPage[CurPage];    //减上页数
  }
  else{//向下换页
    PinYinStart += pPinYin->MaxPerPage[CurPage]; //加当前页数      
    CurPage++;
    if(!pPinYin->MaxPerPage[CurPage]) return; //无下一页了
  }
  pPinYin->PinYinStart = PinYinStart;
  pPinYin->CurPage = CurPage;
}
//------------拼音输入选择状态数字键响应函数---------------------------------
//返回： 0要响应    -1不响应
static signed char _PinYinSelKeyNum(struct _TImePinYin *pPinYin,
                             unsigned char Index) //数字键索引值,0-9
{
  unsigned char PinYinStart = pPinYin->PinYinStart;
  unsigned char CurPage = 0;
  unsigned char PyLen = 0;
  
  for(;PyLen < PinYinStart;CurPage++)
    PyLen += pPinYin->MaxPerPage[CurPage];//得到当前页:从0开始
  unsigned char CurPageCount = pPinYin->MaxPerPage[CurPage];
  
  if(Index < CurPageCount){//不超出时才响应
    pPinYin->CurPinYin = pPinYin->PinYinStart + Index;
    return 0;
  }
  return -1;
}


//----------------汉字输入选择状态上下页键响应函数-------------------------
static void _ChSelKeyUPDown(struct _TImePinYin *pPinYin,
                            char UpFlag) //上页标志,否则为下页
{
  unsigned short Len;
  unsigned char W;
  unsigned char ChNextStart;

  W = pPinYin->DispW;//一页能显示的字符个数
  if(W > 18) W = 18; //一页最大允许显示9个以对应数字键(半字为单位)
  if(UpFlag){//上页时
    if(!pPinYin->ChStart) return;//已在第一页了
    if(pPinYin->ChStart >= W)
      pPinYin->ChStart -= W;
    else pPinYin->ChStart = 0;
  }
  else{//下一页时
    //得到所需汉字位置的汉字个数
    Len = strlen((char*)(*(NumKeyPyIme_pGetCur(&pPinYin->Ime, &ChNextStart) + 
                    pPinYin->CurPinYin))->Pymb);    
    if(Len <= W) return;//无下页    

    ChNextStart = pPinYin->ChStart + W;
    if(ChNextStart >= Len) return; //最后一页到头了
    pPinYin->ChStart = ChNextStart;
  }
}

//------------汉字输入选择状态数字键响应函数---------------------------------
static void _ChSelKeyNum(struct _TImePinYin *pPinYin,
                         unsigned char Index) //数字键索引值,0-9
{
  unsigned char WCount = pPinYin->DispW >> 2; //得到最大字符个数
  if(Index < WCount){//不超出时才响应
    pPinYin->CurCh = pPinYin->ChStart + (Index << 1);//半角为单位
  }
}

//-----------------拼音选择前更新每页能够容纳的拼音数量-------------------------
extern void _UpdateMaxPerPage(struct _TImePinYin *pPinYin)
{
  unsigned char FindCount;
  const struct _PyIndex *const *ppPyIndex;
  unsigned char PyLen = 0;//拼音总长度
  unsigned char WinW = pPinYin->DispW; //得到窗口宽度
  unsigned char CurPage = 0;
  
  memset(pPinYin->MaxPerPage, 0, sizeof(sizeof(pPinYin->MaxPerPage)));//先清为0
  ppPyIndex = NumKeyPyIme_pGetCur(&pPinYin->Ime, &FindCount);//指向所需索引表的有效起始位置;得到数量
  unsigned char Count = 0;
  pPinYin->CurPinYingCount = FindCount;
  while(FindCount){//还有拼音要显示时,一个个填充拼音
    PyLen += strlen((const char*)(*ppPyIndex)->Py) + 1;//前导数字占位
    if((PyLen <= WinW) && (Count < 9)){//一页最大允许显示9个以对应数字键
      pPinYin->MaxPerPage[CurPage]++;//拼音个数增加
      FindCount--;
      Count++;
      ppPyIndex++;
      PyLen++; //后导空格间隔占位
    }
    else{//页增加,为下一页重新初始化
      Count = 0;
      CurPage++;
      //拼音容量数量超限了，不再填入
      if(CurPage > (sizeof(pPinYin->MaxPerPage) - 1)) return;
      PyLen = 0;//保留当前拼音长度到下页
    }
  }
}

//=============================按键响应函数================================
//当用户按键时响应用户请求
//返回当前状态
enum _eTImePinYin TImePinYin_eKey(struct _TImePinYin *pPinYin,
                                 char KeyNum)//键值
{
  unsigned char FindCount;
  enum _eTImePinYin eState = pPinYin->eState;  //状态机
  if((KeyNum == TGUI_KEY_DELETE) || (KeyNum == TGUI_KEY_LEFT) || 
     (KeyNum == TGUI_KEY_ESCAPE))
    KeyNum = ' ';//预先将系统清除键转换为输入要求的键值
  
  switch(eState){//状态机处理
  case eTImePinYin_Idie://空闲状态时,响应用户输入数字键输入首字母
    if((KeyNum >= '2') && (KeyNum <= '9')){
      //键值处理，首次一定能找到
      NumKeyPyIme_pKey(&pPinYin->Ime, &FindCount, KeyNum);
      eState = eTImePinYin_Input;//切换到拼音输入状态
    }
    break;
  case eTImePinYin_Input://拼音输入状态
    if((KeyNum == TGUI_KEY_ENTER) || (KeyNum == TGUI_KEY_RIGHT)){//确认拼音输入完成时
      _UpdateMaxPerPage(pPinYin); //更新每页个数
      if(pPinYin->CurPinYingCount > 1)//需要拼音选择时
        eState = eTImePinYin_PinYinSel;//切换到拼音选择状态
      else //只有一个拼音时直接确认
        eState = eTImePinYin_ChSel;   //直接到汉字选择状态      
    }
    else{//响应拼音输入法
      //用户按清除键清除完成后，重新到空闲状态
      if(NumKeyPyIme_pKey(&pPinYin->Ime, &FindCount, KeyNum) == NULL)
        eState = eTImePinYin_Idie;
    }
    break;
  case eTImePinYin_PinYinSel://拼音选择状态
    if((KeyNum == TGUI_KEY_PAGEUP) || (KeyNum == TGUI_KEY_UP))//选择上一页拼音
      _PinYinSelKeyUPDown(pPinYin, 1);
    else if((KeyNum == TGUI_KEY_PAGEDOWN) || (KeyNum == TGUI_KEY_DOWN))//选择下一页拼音
      _PinYinSelKeyUPDown(pPinYin, 0);
    else if((KeyNum >= '1') && (KeyNum <= '9')){//数字键选择拼音时
      if(!_PinYinSelKeyNum(pPinYin, KeyNum - '1'))
        eState = eTImePinYin_ChSel;//切换到汉字选择状态
    }
    else if(KeyNum == ' ')//退出键退至拼音输入状态
      eState = eTImePinYin_Input;
    break;
  case eTImePinYin_ChSel://汉字选择状态
    if((KeyNum == TGUI_KEY_PAGEUP) || (KeyNum == TGUI_KEY_UP))//选择上一页拼音
      _ChSelKeyUPDown(pPinYin, 1);
    else if((KeyNum == TGUI_KEY_PAGEDOWN) || (KeyNum == TGUI_KEY_DOWN))//选择下一页拼音
      _ChSelKeyUPDown(pPinYin, 0);
    else if((KeyNum >= '1') && (KeyNum <= '9')){//数字键选择拼音时
      _ChSelKeyNum(pPinYin, KeyNum - '1');
      eState = eTImePinYin_Final;//切换到完成状态
    }
    else if(KeyNum == ' '){//退出键
      if(pPinYin->CurPinYingCount > 1)//需要拼音选择时
        eState = eTImePinYin_PinYinSel;//退出到到拼音选择状态
      else //只有一个拼音时直接上级
        eState = eTImePinYin_Input;   //退出键退至拼音输入状态 
    }
    break;
  default: break; //保持当前状态不变
  }
  pPinYin->eState = eState;
  return eState;
}



