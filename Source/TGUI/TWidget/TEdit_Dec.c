/* ----------------------------------------------------------------------------
 *                TGUI的编辑框的十进制数调整实现
 * --------------------------------------------------------------------------*/

#include "TEdit.h"
#include "string.h"
#include "TEditPrv.h"  //内部数据,仅供内部使用

//---------------------重新设定新数值函数-----------------------
//仅重新设定,不更新显示
static void _SetValue(struct _TEdit *pEdit, TSigned_t Value)
{
  if (pEdit->Value == Value) return;  
  if(Value < pEdit->Min) Value = pEdit->Min;
  else if(Value > pEdit->Max) Value = pEdit->Max;
  pEdit->Value = Value;
  TEdit_PaintAll(pEdit);              
}

//-----------------------按键设置正负函数----------------------------
//返回设置是否成功,0成功,其它不成功
static signed char _SetSign(struct _TEdit *pEdit,
                            unsigned char Key)      //按键
{
  TSigned_t Value;
  unsigned char Pos;

  if(!(pEdit->Flag & TEDIT_EN_SIGN))  return -2;//不允许正负显示时

  if(pEdit->Flag & TEDIT_EN_NOTIFY) Pos = 1;//带提示
  else Pos = 0;
  if(pEdit->Pos != Pos) return -1;//除调整位外不允许设置  
  
  Value = pEdit->Value;
  if((Key == TGUI_KEY_UP) || (Key == TGUI_KEY_DOWN))
    goto ValueChange;
  if((Key == '+') && (Value < 0)) //正号小于0时
    goto ValueChange;
  if((Key == '-') && (Value > 0)) //负号大于0时
    goto ValueChange;
  return -3;      //按键无效
    
ValueChange:
  Value = 0 - Value;//取绝对值
  _SetValue(pEdit,Value);//置数
  return 0;
}

//-----------------------得到起始调整位函数----------------------------
static unsigned char _GetStartPos(const struct _TEdit *pEdit)
{
  //根据标志,位置与数值信息决定倍效率
  unsigned char MaxLen;
  unsigned char Len;
  if(pEdit->Flag & TEDIT_EN_NOTIFY) Len = 1; //带提示
  else Len = 0;
  if(pEdit->Flag & TEDIT_EN_SIGN){ //带符号时,检查符号位是否可调整
   if((pEdit->Max < 0) || (pEdit->Min > 0))
     Len++;
  }
  MaxLen = GetNumLen(pEdit->Min,pEdit->Max);
  if(pEdit->DotPos >= MaxLen) //<1了,得到从那位开始调整
    Len += (pEdit->DotPos - MaxLen) + 2;//0.占两位

  return Len;
}

//-----------------------得到指定位置前面有效字符函数----------------------------
//去掉数值前面的无效填充字符如:<-XXX.XX>,<0时,需补0:
//<-10.0>
//若无有效符,则输出负值并指明差多少个
static signed char _GetPreValidLen(const struct _TEdit *pEdit,
                         unsigned char Pos)
{
  //根据标志,位置与数值信息决定倍效率
  unsigned char Len;
  Pos++;    //将位置折算成长度
  
  if(pEdit->Flag & TEDIT_EN_NOTIFY) Pos--;//带提示
  if(pEdit->Flag & TEDIT_EN_SIGN) Pos--;//带符号
  if(!pEdit->DotPos) return Pos;
  //带小数点时,先检查数值是否<1,若是,则检查前面补了多少个0.或0.0
  Len = GetNumLen(pEdit->Min,pEdit->Max);
  if(pEdit->DotPos >= Len) //<1了,0.
    Pos -= (pEdit->DotPos - Len) + 2;
  //检查当前位置在小数点左还是右边
  else{
    Len -= pEdit->DotPos;
    if(Pos > Len) Pos--;
  }
  return Pos;
}

/**********************************************************************
                         成员操作函数
**********************************************************************/

//-----------------十进制调整时更新到缓冲区处理函数------------------
void TEditDec_UpdateBuf(const struct _TEdit *pEdit)
{
  unsigned char Len;
  unsigned char Flag = pEdit->Flag;
  TWin_t *pWin = TWM_pGetWin(pEdit->hWin);
  char *pBuf = TWin_pGetString(pWin,0);
  char *pPrvBuf = pBuf;
  if(Flag & TEDIT_EN_NOTIFY) *pBuf++ = '<';//带提示
  //填充数值
  pBuf = pNum2StringFlag(pEdit->Value,pBuf,GetNumLen(pEdit->Min,pEdit->Max),
                         (Flag & TEDIT_EN_SIGN) | pEdit->DotPos);
  if(Flag & TEDIT_EN_NOTIFY) *pBuf++ = '>';//带提示
  Len = pBuf - pPrvBuf;
  if(Len < TWin_GetW(pWin))   //后面的强制填充空格
    memset(pBuf,' ',TWin_GetW(pWin) - Len);
  
  //填充焦点区
  if(Flag & TEDIT_GRP_MODE){
    if(Flag & TEDIT_EN_NOTIFY) //带提示时
      TWin_SetFocus(pWin,1,0,pEdit->Len - 2);
    else 
      TWin_SetFocus(pWin,0,0,pEdit->Len);
  }
  else{
    TWin_SetFocus(pWin,pEdit->Pos,0,1);//调整位置是焦点
  }
}

//-----------------单数值调整模式数值设定按键处理函数----------------
//单个数值调整模式时,上下键或数值调整时调用此函数
static void _SValueSetKeyPro(struct _TEdit *pEdit,unsigned char Key)
{
  unsigned char MutiPos;//倍率位置
  TUnSigned_t Value;      //当前数值
  TUnSigned_t Muti,Vol;  //指定位置倍率与当前位值值
  if(!_SetSign(pEdit,Key)) return;//符号处理有效时直接返回
  Value = GetAbs(pEdit->Value);
  //得到倍率数
  MutiPos = GetNumLen(pEdit->Min,pEdit->Max) - _GetPreValidLen(pEdit,pEdit->Pos);
  Vol = GetDigitalInPos(Value,MutiPos);  //得到指定位数值
  Muti = MutiTUnigned[MutiPos];
  Value -= Vol * Muti; //去掉该位了
  if(Key == TGUI_KEY_UP){
    if(Vol < 9) Vol++;
    else Vol = 0;
  }
  else if(Key == TGUI_KEY_DOWN){
    if(Vol) Vol--;
    else Vol =  9;
  }
  else{//数字键
    Vol = Key - '0';
  }
  Value += Vol * Muti; //新的该位数值

   //设定新数值
  if(pEdit->Value < 0) _SetValue(pEdit,0 - Value);
  else _SetValue(pEdit,Value);
  
}

//-----------------------单调整模式按键处理函数!---------------------------
static void _SKeyPro(struct _TEdit *pEdit,unsigned char Key)
{
  unsigned char Notify = 0xff;    //无效通报值
  char c;

  //单调整模式时先处理数字键
  if(!(pEdit->Flag & TEDIT_GRP_MODE)){
    //单调整模式时处理数字键
    if((Key >= '0') && (Key <= '9')){
      c = TEdit_GetChar(pEdit,pEdit->Pos);
      if ((c != '-') && (c != '+')){
         _SValueSetKeyPro(pEdit,Key);
       Key = TGUI_KEY_RIGHT;//向右走一格
      }
      else return;
    }
  }

  switch (Key){
  case '+':
  case '-': _SetSign(pEdit,Key); break;
  case TGUI_KEY_UP:
  case TGUI_KEY_DOWN:
    _SValueSetKeyPro(pEdit,Key);
    break;
  case TGUI_KEY_ENTER:
    if(!(pEdit->Flag & TEDIT_ENTER_RIGHT)){//正常退出
      Notify = TGUI_NOTIFY_KEY_ENTER;
      break;
    }//否则解释为右键,继续...
  case TGUI_KEY_RIGHT:
    c = pEdit->Pos + 1;
    if((c >= pEdit->Len) || (TEdit_GetChar(pEdit,c) == '>')){//右超限通报
      TGUI_NOTIFY_RUN(pEdit->cbGUINotify,pEdit,TEDIT_NOTIFY_RIGHT_END,&pEdit->Value);
      if(Key != TGUI_KEY_ENTER) return;
      Notify = TGUI_NOTIFY_KEY_ENTER; //否则再次处理确认键
      break;
    }
    if(TEdit_GetChar(pEdit,c) == '.') c++;
    pEdit->Pos = c; 
    break;
  case TGUI_KEY_LEFT:
    c = pEdit->Pos;
    if(c <= _GetStartPos(pEdit)){ //左超限通报
      Notify = TEDIT_NOTIFY_LEFT_END;
      break;   //位置不动或通报退出
    }
    c--;
    //按键有效时检查是否在.上
    if(TEdit_GetChar(pEdit,c) == '.') c--;
    pEdit->Pos = c; 
    break;
  case TGUI_KEY_ESCAPE://忽略退出
    Notify = TGUI_NOTIFY_KEY_ESCAPE;
    break;
  default:
    break;
  }
  TEditDec_UpdateBuf(pEdit);
  if(Notify != 0xff)  //通报
    TGUI_NOTIFY_RUN(pEdit->cbGUINotify,pEdit,Notify,&pEdit->Value);
}

//-----------------------群调整模式按键处理函数---------------------------
static void _GKeyPro(struct _TEdit *pEdit,unsigned char Key)
{
  unsigned char Notify = 0xff;    //无效通报值
  switch (Key){
  case TGUI_KEY_UP:
    if((TSigned_t)pEdit->Value < pEdit->Max) pEdit->Value++;
    break;
  case TGUI_KEY_DOWN:
    if((TSigned_t)pEdit->Value > pEdit->Min) pEdit->Value--;
    break;
  case TGUI_KEY_ENTER:
    if(!(pEdit->Flag & TEDIT_ENTER_RIGHT)){//正常退出
      Notify = TGUI_NOTIFY_KEY_ENTER;
      break;
    }//否则解释为右键,继续...
  case TGUI_KEY_RIGHT:
    TGUI_NOTIFY_RUN(pEdit->cbGUINotify,pEdit,TEDIT_NOTIFY_RIGHT_END,&pEdit->Value);
    if(Key != TGUI_KEY_ENTER) return;
    Notify = TGUI_NOTIFY_KEY_ENTER; //否则再次处理确认键
    break;
  case TGUI_KEY_LEFT:
    Notify = TEDIT_NOTIFY_LEFT_END;
    break;   //位置不动或通报退出    
  case TGUI_KEY_ESCAPE://忽略退出
    Notify = TGUI_NOTIFY_KEY_ESCAPE;
    break;
  default:
    break;
  }
  TEditDec_UpdateBuf(pEdit);
  if(Notify != 0xff)  //通报
    TGUI_NOTIFY_RUN(pEdit->cbGUINotify,pEdit,Notify,&pEdit->Value);
}

//-------------------十进制调整时按键处理函数--------------------
void TEditDec_KeyPro(struct _TEdit *pEdit,unsigned char Key)
{
  if(pEdit->Flag & TEDIT_GRP_MODE) _GKeyPro(pEdit,Key);
  else _SKeyPro(pEdit,Key);
}

//-----------------编辑器十进制数初始化为函数----------------
void TEditDec(TEdit_t *pEdit,
              TWinHandle_t hWin,    //依赖的窗口,数据缓冲区>Len
              TSigned_t Value,            //初始化值
              TSigned_t Max,              //最大值
              TSigned_t Min,              //最小值
              unsigned char DotPos,  //小数点位置,可用于用整数模拟小数显示
              unsigned char Flag,  //相关配置
              TGUINotify_t cbGUINotify) //通报函数
{
  unsigned char Len;
  TWin_t *pWin;
  //初始化相关
  pEdit->hWin = hWin;
  pEdit->eType = TEditType_Dec;
  pEdit->Flag = Flag;
  pEdit->DotPos = DotPos;
  pEdit->Value = Value; 
  pEdit->Max = Max;
  pEdit->Min = Min;
  pEdit->cbGUINotify = cbGUINotify;  
 
  
  pEdit->Pos = _GetStartPos(pEdit); //得到起始调整位置
  //计算所需长度
  Len =  253 - _GetPreValidLen(pEdit,252);//所有字符无效的填充位长度(除>)
  if(Flag & TEDIT_EN_NOTIFY) Len++;//尾部>
  Len = GetAlignLenR(Len,GetNumLen(Min, Max));//对齐
  pEdit->Len = Len;
  //重设置窗口高宽
  pWin = TWM_pGetWin(hWin);
  if(TWin_GetBufSize(pWin) < Len) return; //窗口过小!!!!
  TWin_ReSize(pWin,Len,1);
  TWin_Disp(pWin);//置为显示状态
  TEditDec_UpdateBuf(pEdit);//更新缓冲区
}
