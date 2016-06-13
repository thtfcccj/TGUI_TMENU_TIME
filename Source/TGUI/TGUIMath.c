/* ----------------------------------------------------------------------------
 *               TGUI数学处理相关函数实现
 * --------------------------------------------------------------------------*/

#include "TGUIMath.h"
#include "string.h"

//由位置查倍率,32位时
#if(TSIGNED_SIZE == 4)
  const TUnSigned_t MutiTUnigned[] = {
    1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
#elif(TSIGNED_SIZE ==2)
  //由位置查倍率,16位时
  const TUnSigned_t MutiTUnigned[] = {
  1,10,100,1000,10000};
#else
  //由位置查倍率,8位时
  const TUnSigned_t MutiTUnigned[] = {
  1,10,100};
#endif

//----------------得到一个有符号数的绝对值--------------------
TSigned_t GetAbs(TSigned_t Value)
{
  if(Value < 0) return 0 - Value;
  else return Value;
}


//----------------由数值得到占位个数,不含符号--------------------
unsigned char GetNumLen(TSigned_t Min,TSigned_t Max)   //小值与大值
{
  TUnSigned_t Data;
  unsigned char Len;

  Min = GetAbs(Min);
  Max = GetAbs(Max);
  if(Min > Max) Max = Min;
  Data = Max;
 
  for(Len = 0; Data; Len++) Data /= 10;
  if(Len > 7)  Len = 7;
  return Len;
}

//----------------得到指定位置数值函数------------------------
TUnSigned_t GetDigitalInPos(TUnSigned_t Value,
                           unsigned char Pos)
{
  while(Pos--){
    Value /= 10;
  }
  return Value % 10;
}

//-----------------带标志数转换为字符串函数------------------
//返回填充后最末未用缓冲区位置
char *pNum2StringFlag(TSigned_t Value,   //当前数值
                      char *pBuf,    //接收缓冲区
                      unsigned char Len,    //显示长度
                      //标志,定义为：低3bit:小数点位置,0x80:显示正负号
                      unsigned char Flag)
{
  unsigned char FullLen;

  if(Flag & 0x80){//带符号时
    if(Value < 0) *pBuf = '-';
    else *pBuf = '+';
    pBuf++;
  }
  if(Value < 0) Value = 0 - Value;  //正值显示
      
  //带小数点时,先检查数值是否<1,若是,则检查前面补了多少个0.或0.0
  Flag &= 0x07;
  if(Flag >= Len){ //<0了,0.
    FullLen = Flag - Len;
    *pBuf++ = '0';
    *pBuf++ = '.'; 
    while(FullLen--) *pBuf++ = '0';//填充无效的0
    pBuf += Value2StringMin(Value,pBuf,Len);
  }
  else{
    //>1了,检查小数点位置,当前位置在小数点左还是右边
    pBuf += Value2StringMin(Value,pBuf,Len);//先填充数据
    if(Flag){ //有小数点时填充小数点位置
      FullLen = Flag;//暂存
      for(; Flag > 0; Flag--) {*pBuf = *(pBuf - 1); pBuf--;}
      *pBuf= '.';
      pBuf += FullLen + 1;//含小数点
    }
  }
  return pBuf;
}
