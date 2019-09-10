/*****************************************************************************
                         TGUI上的输入法模块-符号输入法部分
*****************************************************************************/

#include "TImePinYin.h"
#include "TImeBase.h"
#include <string.h>

/*****************************************************************************
                              相关函数
*****************************************************************************/

//---------------------------初始化函数---------------------------------
//切换到拼音输入法或上个汉字输入完成后调用
void TImePinYin_Init(struct _TImePinYin *pPinYin) //输入法结构
{
  memset(pPinYin, 0, sizeof(struct _TImePinYin));//先初始化到0
}

//----------------------填充拼音显示行字符串函数---------------------------
//填充示例:"pen ren sen", 超过显示时如：ji jia li liu lin liu l...
static void _FullPinYinChar(const struct _TImePinYin *pPinYin,
                             char *pBuf)              //被填充的字符
{
  const struct _PyIndex *const *ppPyIndex;
  unsigned char FindCount;
  unsigned char PyCount;  
  unsigned char Pos = 0; //拼音内部位置
  char *pEndBuf = pBuf + TIME_PINGYING_DISP_W; //行结束位置
  
  ppPyIndex = NumKeyPyIme_pGetCur(&pPinYin->Ime) + 
              pPinYin->PinYinStart;//指向所需索引表的有效起始位置
  FindCount = NumKeyPyIme_GetCurCount(&pPinYin->Ime) - pPinYin->PinYinStart;
  while(FindCount){//还有拼音要显示时,一个个填充拼音
    PyCount = strlen((const char*)(*ppPyIndex)->Py);//拼音
    if((PyCount + 1) <= (pEndBuf - pBuf)){//数字+拼音填充,直到拼音显示不下时为止
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
  if(pEndBuf > pBuf){//后面填充空格
    memset(pBuf, ' ', pEndBuf - pBuf);
  }  
  if(FindCount){//一页显示不下时,最后...提示(可能会被强制截断拼音)
    memset(pEndBuf - 3, '.', 3);
  }
}

//-------------------------首屏抬头固定提示-----------------------------------
static const char _chIdie1st[]   = {"拼音: 快速按两次为第二组"};
static const char _chIdie2nd[]   = {"全部完成仍未显示按●选择"};
static const char _chPinYinSel[] = {"拼音: 拼音选择"};
static const char _chChSel[]     = {"拼音: 汉字选择"};

//-------------------------填充用户提示(抬头)-----------------------------------
void TImePinYin_FullTitle(const struct _TImePinYin *pPinYin,//输入法结构
                           char *pStr)
{
  switch(pPinYin->eState){
    case eTImePinYin_Idie:
      if(pPinYin->Timer > (TIME_PINGYING_IDIE_DISP_OV / 2))//首屏提示
        strcpy(pStr, _chIdie1st); 
      else strcpy(pStr, _chIdie2nd); 
      break;
    case eTImePinYin_Input:
      _FullPinYinChar(pPinYin, pStr); 
      break;
    case eTImePinYin_PinYinSel: 
      strcpy(pStr, _chPinYinSel);
      break;
    case eTImePinYin_ChSel:      
      strcpy(pStr, _chChSel);    
      break;
    default: break;
  }
  return;  
}

//-----------------------拼音字母相关字符资源----------------------------
static const char _ch_a2f[] =  {"abc def"};
static const char _ch_g2l[] =  {"ghi jkl"};
static const char _ch_m2s[] =  {"mno pqrs"};
static const char _ch_t2z[] =  {"tuv wxyz"};
static const char _chPinYinFinal[] = {"拼音完成"};    
static const char *const _IdieAndInput[] = {
  _ch_a2f,        // TIME_MNG_KEY_UP 
  _ch_g2l,        //TIME_MNG_KEY_LEFT
  _chPinYinFinal, //TIME_MNG_KEY_ENTER
  _ch_m2s,       //TIME_MNG_KEY_RIGHT
  _ch_t2z,       //TIME_MNG_KEY_DOWN
};
static const char _chDeletel[] = {"删除"};
static const char _chNextGrp[] = {"下一组"};

//-------------------------得到对应键值字符函数-----------------------------------
//如：上键对应 返回NULL表示不需要字符
const char *TImePinYin_pGetArrowNotice(struct _TImePinYin *pPinYin,//输入法结构
                                         unsigned char GuideKey)//对应键值位置
{
  //单独处理返回键
  if(GuideKey == TIME_MNG_KEY_EXIT){
    if(pPinYin->eState == eTImePinYin_Input)//此状态为删除功能
      return _chDeletel;
    return chReturn;
  }
  
  //拼音输入状态提示
  if(pPinYin->eState <= eTImePinYin_Input)
    return _IdieAndInput[GuideKey];
  
  //==================拼音选择状态或汉字选择状态时=============================
  if(GuideKey == TIME_MNG_KEY_ENTER){//单独处理确认键为下一组
    return _chNextGrp;
  }
  GuideKey = TIme_GuideKeyToNoEnterKey[GuideKey];

  //拼音选择状态显示拼音
  if(pPinYin->eState == eTImePinYin_PinYinSel){
    unsigned char FindCount;
    volatile const struct _PyIndex *const *ppPyIndex;
    ppPyIndex = NumKeyPyIme_pGetCur(&pPinYin->Ime) + 
                 pPinYin->PinYinStart;//指向所需索引表的有效起始位置
    FindCount = NumKeyPyIme_GetCurCount(&pPinYin->Ime) - pPinYin->PinYinStart;
    if(GuideKey >= FindCount) return NULL; //拼音超限了。
    ppPyIndex += GuideKey;
    //unsigned char PyCount = strlen((const char*)(*ppPyIndex)->Py);//拼音
    //memcpy(pPinYin->ChBuf, (*ppPyIndex)->Py, PyCount);
    //pPinYin->ChBuf[PyCount] = '\0';
    //return pPinYin->ChBuf;
    return (const char*)(*ppPyIndex)->Py;
  }
  
  //汉字选择状态时提示汉字
  unsigned char ChOffset = GuideKey * 2;//页内偏页
  const char *pCh;
  pCh = (const char *)(*(NumKeyPyIme_pGetCur(&pPinYin->Ime) + 
                        pPinYin->CurPinYin))->Pymb;//指向所需拼音码表位置
    
  unsigned char CurLen = strlen((const char*)pCh) - pPinYin->ChStart;//当前页字符长度
  if(CurLen < ChOffset) return NULL; //汉字超限了。
      
  pCh += (pPinYin->ChStart + ChOffset); //指向当前页中的当前需要填充的汉字
  //因汉字连续无结束字符，使用缓冲区
  pPinYin->ChBuf[0] = *pCh++;
  pPinYin->ChBuf[1] = *pCh;
  pPinYin->ChBuf[2] = '\0';
  return pPinYin->ChBuf;
}

//-----------------------------任务函数----------------------------------
//128ms调用一次, 若修改完成，返回对应输入完成的数字，否则0不处理, 0xffff:强制刷屏
unsigned short TImePinYin_Task(struct _TImePinYin *pPinYin)
{
  //空闲状态用于循环显示提示信息
  if(pPinYin->eState == eTImePinYin_Idie){
    if(!pPinYin->Timer){ 
      pPinYin->Timer = TIME_PINGYING_IDIE_DISP_OV;
      return 0xffff;//强制刷屏
    }
    pPinYin->Timer--;
    if(pPinYin->Timer == (TIME_PINGYING_IDIE_DISP_OV / 2))
      return 0xffff;//强制刷屏
    return 0;
  }  
  
  //仅处理输入状态
  if(pPinYin->eState != eTImePinYin_Input) return 0;
  if(!pPinYin->Timer) return 0; //无任务
  pPinYin->Timer--;
  if(pPinYin->Timer) return 0;//未结束
  //没有选择第二个字符,确认为第一个拼音字母。
  unsigned char Count;
  NumKeyPyIme_pKey(&pPinYin->Ime, &Count, pPinYin->FirstKey * 2 + '2');
  return 0xffff;//强制刷屏
}

/*****************************************************************************
                        键值处理相关函数
*****************************************************************************/

//--------------------------确认按键响应函数-----------------------------------
//若修改完成，则返回对应字符，否则0: 不修改返回,  0xffff: 内部操作过程中 
static unsigned short _EnterKey(struct _TImePinYin *pPinYin)
{
 switch(pPinYin->eState){
    case eTImePinYin_Idie: //一个拼音都没输入，接键无效
      break;
   case eTImePinYin_Input:{ //拼音确认
     if(pPinYin->Timer){//已输入首个时,表示为第一个
      unsigned char Count;
      NumKeyPyIme_pKey(&pPinYin->Ime, &Count, pPinYin->FirstKey * 2 + '2');
      pPinYin->Timer = 0; //关闭定时器
     }
     pPinYin->eState = eTImePinYin_PinYinSel;//切换到拼音选择状态
     pPinYin->PinYinStart = 0;//初始化到首个(若不要则保持上次不变)
     break;
   }
   case eTImePinYin_PinYinSel:{ //拼音选择,切换下一组
      unsigned char Count = NumKeyPyIme_GetCurCount(&pPinYin->Ime) - 
                             pPinYin->PinYinStart;
      if(Count > 4) pPinYin->PinYinStart += 4; //4个为一组
      else pPinYin->PinYinStart = 0; //允许回环
      pPinYin->ChStart = 0; //复位以重新选择汉字
      break;    
    }
    case eTImePinYin_ChSel:{ //汉字选择,切换下一组
      //先得到汉字
      const char *pCh = (const char *)(*(NumKeyPyIme_pGetCur(&pPinYin->Ime) + 
                                       pPinYin->CurPinYin))->Pymb;//指向所需拼音码表位置
      unsigned char Count = strlen((const char*)pCh) - pPinYin->ChStart;//当前页字符长度(半字)
      if(Count > 8) pPinYin->ChStart += 8; //4个为一组,半字为单位
      else pPinYin->ChStart = 0; //允许回环
      break;    
    }
  }
  return 0xffff; //内部操作过程中 
}

//--------------------------按键响应函数-----------------------------------
//若修改完成，则返回对应字符，否则0: 不修改返回,  0xffff: 内部操作过程中 
unsigned short TImePinYin_Key(struct _TImePinYin *pPinYin, 
                               unsigned char GuideKey)
{
  //单独处理退出键
  if(GuideKey == TIME_MNG_KEY_EXIT){
    if(pPinYin->eState == eTImePinYin_Idie)
      return 0;
    if(pPinYin->eState == eTImePinYin_Input){//删除
      unsigned char FindCount;
      if(NumKeyPyIme_pKey(&pPinYin->Ime, &FindCount, ' ') == NULL)//清除完了
        pPinYin->eState = eTImePinYin_Idie;//到空闲
    }
    else pPinYin->eState--; //一层层返回
    
    return 0xffff; //内部操作过程中 
  }
  
  //单独处理确认键
  if(GuideKey == TIME_MNG_KEY_ENTER) 
     return _EnterKey(pPinYin);
  
  //其它4个接键时
  GuideKey = TIme_GuideKeyToNoEnterKey[GuideKey];
  unsigned char Offset = GuideKey * 2;//页内偏移
  
  //====================空闲与输入状态处理=====================================
  if(pPinYin->eState <= eTImePinYin_Input){
    unsigned char NumKey;  //先获得键值,  '2'即可与NumKeyPyIme_pKey对应
    if((pPinYin->eState == eTImePinYin_Input) && (pPinYin->Timer)){//第二次输入时
      if(pPinYin->FirstKey == GuideKey){//两次键值相同时,第二次为第二组
        NumKey = Offset + 1;
        pPinYin->Timer = 0; //选择完成
      }
      else{//两次键不同时
        NumKey = pPinYin->FirstKey * 2; //表示首个字符已确认
        pPinYin->FirstKey = GuideKey; //同时将这个字符作为首个字符
        pPinYin->Timer = TIME_NUM_TIMER_OV;
      }
    }
    else{ //首次输入时,记住选择即可
      pPinYin->FirstKey = GuideKey; //同时将这个字符作为首个字符
      pPinYin->eState = eTImePinYin_Input; //空闲到输入状态
      pPinYin->Timer = TIME_NUM_TIMER_OV;
      return 0xffff; //内部操作过程中
    }
    NumKeyPyIme_pKey(&pPinYin->Ime, &NumKey, NumKey + '2');
  }
  //===========================拼音与汉字选择状态===============================
  else if(pPinYin->eState == eTImePinYin_PinYinSel){
    unsigned char FindCount = NumKeyPyIme_GetCurCount(&pPinYin->Ime) - pPinYin->PinYinStart;
    if(GuideKey >= FindCount) return 0xffff; //拼音超限了,选择无效
    pPinYin->CurPinYin = pPinYin->PinYinStart + GuideKey;
    pPinYin->eState = eTImePinYin_ChSel; //到汉字选择状态
    pPinYin->ChStart = 0;//初始化到首个(若不要则保持上次汉字选择不变)
  }
  else/* if(pPinYin->eState == eTImePinYin_ChSel)*/{//汉字选择状态选汉字返回结束
    const char *pCh = (const char *)(*(NumKeyPyIme_pGetCur(&pPinYin->Ime) + 
                          pPinYin->CurPinYin))->Pymb;//指向所需拼音码表位置
    unsigned char CurLen = strlen((const char*)pCh) - pPinYin->ChStart;//当前页字符长度
    if(CurLen <= Offset) return 0xffff; //汉字超限了,选择无效
    pCh += (pPinYin->ChStart + Offset); //指向当前页中的当前需要填充的汉字
    unsigned short Ch = ((unsigned short)*pCh << 8); //高位在前
    Ch |= *(pCh + 1);
    pPinYin->eState = eTImePinYin_Idie; //到汉字选择状态
    return Ch;
  }
  return 0xffff; //内部操作过程中  
}


