/*****************************************************************************
                         TGUI上的输入法模块首行编辑框模块
注:暂不支持自带字库全角对齐LCD显示屏的显示
*****************************************************************************/

#include "TImeEdit.h"
#include <string.h>

/*****************************************************************************
                        相关成员函数
*****************************************************************************/

//---------------判断当前位置是否全角对齐函数-------------------
static signed char _IsAlign(const char *pString,    //字符串
                            TIme_String_t Cursor)//光标位置>=1;
{
  TIme_String_t Align;
  Cursor -= 1;//检查之前的字符
  if(*(pString + Cursor) < 0x80) return 0;//对齐了
  //统计光标之前全角字符占位情况，若为偶数则对齐，否则当前位置未对齐
  for(Align = 1, Cursor -= 1; Cursor > 0; Cursor--){
    if(*(pString + Cursor) >= 0x80) Align++;
    else break;//找到半角字符了
  }
  if(Align & 0x01) return -1; //未对齐
  return 0;
}


//-----------------------初始化函数-------------------------------
//进入输入法模式时调用
void TImeEdit_Init(struct _TImeEdit *pEdit,
                  char *pString,       //被修改的字符串
                  TIme_String_t Size,  //字符串缓冲区允许大小+1
                  TIme_String_t w)     //编辑框宽度
{
  memset(pEdit, 0, sizeof(struct _TImeEdit));
  
  TIme_String_t Len;
  TIme_String_t DispStart;
  pEdit->pString = pString;
  pEdit->Size = Size;
  pEdit->w = w;
  Len = strlen(pString);
  if(Len >= Size){//字符串不含结束字符，强制增加
    *(pString + Size - 1) = '\0';
    Len = Size - 1;
  }
  pEdit->Len = Len;
  pEdit->Cursor = Len;//默认从最后位置开始编辑 
  //光标指向最后一个编辑框宽度窗口,注意光标占一位
  if(Len < w) DispStart = 0;
  else{
    DispStart = Len - w;
    //当前光标未全角对齐时，光标靠后一位显示以使第一个全角字符不至于乱码
    if(_IsAlign(pString,DispStart)) DispStart++;
  }
  pEdit->DispStart = DispStart;
}


//--------------------得到相对光标位置函数---------------------------
//即在当前显示起始开始的光标位置，用于上层焦点区反显
TIme_String_t TImeEdit_GetCursor(const struct _TImeEdit *pEdit)
{
  return pEdit->Cursor - pEdit->DispStart;
}

//---------------------------光标左移函数---------------------------
//焦点在编辑框内时调用
//返回左移个数
unsigned char TImeEdit_CursorLeft(struct _TImeEdit *pEdit)
{
  unsigned char Left;
  TIme_String_t Cursor = pEdit->Cursor;
  
  if(!Cursor) return 0;//已在左边界了
  //确定左移多个位
  Cursor--;
  if(*(pEdit->pString + Cursor) >= 0x80) Left = 2;
  else Left = 1;
  if(pEdit->Cursor <= Left)
    Left = pEdit->Cursor; //移到左边界了

  pEdit->Cursor -= Left;  
  //不能显示了,光标一起移动
  if(Cursor < pEdit->DispStart) pEdit->DispStart = Cursor;
  return Left;
}
        
//---------------------------光标右移函数---------------------------
//焦点在编辑框内时调用
//返回右移个数
unsigned char TImeEdit_CursorRight(struct _TImeEdit *pEdit)
{
  unsigned char Right;
  TIme_String_t Cursor = pEdit->Cursor;
  
  if(Cursor >= pEdit->Len) return 0;//已在右边界了
  //确定右移多个位
  if(*(pEdit->pString + Cursor) >= 0x80) Right = 2;
  else Right = 1;
  Cursor++;
  if((pEdit->Cursor + Right) >= pEdit->Len)//移到右边界了
    Right = pEdit->Len - pEdit->Cursor;
  
  pEdit->Cursor += Right;
  //不能显示了,光标一起移动
  if((Cursor - pEdit->DispStart) >= pEdit->w)
     pEdit->DispStart += Right;
  return Right;
}

//------------------------删除一个字符函数---------------------------
//删除光标左侧的一个字符,焦点在编辑框内时调用
//返回删除个数
unsigned char TImeEdit_Clr(struct _TImeEdit *pEdit)
{
  unsigned char Left;
  TIme_String_t Cursor = pEdit->Cursor;
  
  if(!Cursor) return 0;//已在左边界了,不能删除
  //确定左移多位,即删除个数
  if(*(pEdit->pString + (Cursor - 1)) >= 0x80) Left = 2;
  else Left = 1;
  if(Cursor <= Left) Left = Cursor; //移到左边界了
    
  //将右侧字符拷到左侧即完成删除
  memcpy(pEdit->pString + Cursor - Left,
         pEdit->pString + Cursor,
         pEdit->Len - Cursor + 1);//将结束字符'\0'一起拷走
  
  pEdit->Len -= Left;  //长度减小了
  pEdit->Cursor -= Left;//光标同时移动
  return Left;
}

//------------------判断允许增加字符个数函数---------------------------
//在当前光标位置增加一个字或字符,焦点在编辑框内时调用
//返回可增加字符个数
unsigned char TImeEdit_EnAdd(const struct _TImeEdit *pEdit)
{
  return pEdit->Size - pEdit->Len - 1;//'\0'预留一位 
}

//-------------------光标位置增加一个字符函数---------------------------
//焦点在编辑框内时调用
void TImeEdit_Add(struct _TImeEdit *pEdit,
                  unsigned short String)//增加的全字或半字
{
  unsigned char CurSize;
  TIme_String_t Len;
  TIme_String_t Cursor;  
  
  if(String > 0x80) CurSize = 2;//全字
  else CurSize = 1;
  if(TImeEdit_EnAdd(pEdit) < CurSize) return;//不能再增加字符了,出错

  //将光标位置以后的字符一个个向右移动CurSize个大小
  Cursor = pEdit->Len;  //含'\0'一起
  Len = Cursor - pEdit->Cursor + CurSize;
  for(; Len > 0; Len--){
    *(pEdit->pString + Cursor + CurSize) = 
      *(pEdit->pString + Cursor);
    if(Cursor) Cursor--;
    //无字符可移动了(全角时，Lel滞后Cursor一步，故不能用Len退出)
    else break; 
  }
  //光标位置增加字符
  if(CurSize == 2){
    *(pEdit->pString + pEdit->Cursor) = (char)(String >> 8);
    *(pEdit->pString + pEdit->Cursor + 1) = (char)(String);
  }
  else *(pEdit->pString + pEdit->Cursor) = (char)(String);
  
  pEdit->Len += CurSize;//字符串长度增加了
  pEdit->Cursor += CurSize;//光标同时移动
  //不能显示了，显示行一起移动
  if((pEdit->Cursor - pEdit->DispStart) >= pEdit->w)
     pEdit->DispStart += CurSize;
}

//-------------------------增充字符函数-----------------------------
//填充从显示起始位置到宽度范围内的字符，不足的补充空格
void TImeEdit_FullBuf(const struct _TImeEdit *pEdit,
                      char *pBuf)    //被填充的缓冲区
{
  //填充光标前字符
  TIme_String_t w;
  TIme_String_t Data;  
  
  w = pEdit->Cursor - pEdit->DispStart;
  memcpy(pBuf, pEdit->pString + pEdit->DispStart, w);
  if(*pBuf >= 0x80){//第一字为全角字符时，检查是否被拆分了
    for(Data = 1; Data < w; Data++){
      if(*(pBuf + Data) < 0x80) break;
    }
    if(Data & 0x01) *pBuf = ' ';//被拆分时填充空格
  }
  pBuf += w;
  *pBuf++= ' ';//光标位置填充空格
  w = pEdit->w - (w + 1);//为光标后位置余下宽度
  
  //填充光标后字符
  if(pEdit->Len > pEdit->Cursor){//光标后还有字符
    if((pEdit->Len - pEdit->Cursor) < w) //修正为后面字符的宽度
      Data = pEdit->Len - pEdit->Cursor;
    else Data = w;
    memcpy(pBuf, pEdit->pString + pEdit->Cursor, Data);
    if(_IsAlign(pBuf,w)) Data--;//最后的全角禁止分为两半显示，若是则填充为空
    pBuf += Data;
    w -= Data;
  }
}

