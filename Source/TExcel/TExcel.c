/*******************************************************************************

                    Excel类型数据组织实现

********************************************************************************/
#include "TExcel.h"
#include "math_3.h"
#include <string.h>
#include "stringEx.h"

/******************************************************************************
                              相关函数实现
*******************************************************************************/

//---------------------------------初始化函数-----------------------------------
void TExcel_Init(struct _TExcel *pUi,
                 unsigned char Handle,        //句柄
                 const struct _TExcelStatic *pStatic,//静态数据结构
                 unsigned char PageLine)     //当前页显示行数，0表示所有
{
  memset(pUi, 0, sizeof(struct _TExcel));
  pUi->Handle = Handle;
  pUi->pStatic = pStatic;
  if((PageLine == 0) || (PageLine > TEXCEL_LINE_COUNT))//异常
    PageLine = TEXCEL_LINE_COUNT;
  pUi->PageLine = PageLine;
  //初始化变量区
  pUi->JumpAryId = 0xffff;//无此功能
  //首次装载页内查找表
  pUi->IdCount = TExcel_cbReloadLut(Handle, 0, pUi->IdLut, PageLine);
}

//-----------------------------由当前行得到查找表ID值------------------------
unsigned short TExcel_GetIdLutVol(const void *pHandle,
                                        unsigned short Line)
{
  const struct _TExcel *pUi = (struct _TExcel *)pHandle;
  return pUi->IdLut[Line - pUi->StartLine];  
}

/******************************************************************************
                            相关行函数-UI部分实现
*******************************************************************************/  


//------------------------------得到标题头实现----------------------------------
const char* TExcel_pGetHeader(struct _TExcel *pUi)
{
  const struct _TExcelStatic *pStatic = pUi->pStatic;
  //先得到显示查找表
  const unsigned char *pParaIdLut = pStatic->pParaIdLut;
  unsigned char ParaLen = *pParaIdLut++ & 0x7f;
  char *pBuf = pUi->LineBuf;
  //根据显示方式填充
  const struct _TExcelParaDesc *pParaDesc = pStatic->pParaDesc;
  const unsigned char *pDispInfo = pParaDesc->pDispInfo;
  for(; ParaLen > 0; ParaLen--){
    unsigned char ParaId = *pParaIdLut++;
    unsigned char DispInfo = *(pDispInfo + ParaId);
    unsigned char Len = DispInfo & TEXCEL_LEN_MASK; //长度
    const char *pHeader = pParaDesc->pGetHeader(ParaId);
    if(pHeader == NULL)//没有标题时空格填充
      memset(pBuf, ' ', Len);
    else //对齐方式填充字符
      strFull(pBuf, //缓冲区
              pHeader,        //数据源
              Len,
              (DispInfo & TEXCEL_ALIGN_MASK) >> TEXCEL_ALIGN_SHIRT);//对齐方式
    pBuf += Len;
    if(!(DispInfo & TEXCEL_DIS_SPACE)){//没有禁止间隔时跟空格以间隔
      *pBuf++ = ' ';
    }
  }
  *pBuf = '\0';//最后填充结束字符
  if(!pUi->LineStrLen) pUi->LineStrLen = strlen(pUi->LineBuf); //获取长度
  return pUi->LineBuf;
}

//---------------------------得到数据行总数实现----------------------------------
unsigned short TExcel_GetLineCount(struct _TExcel *pUi)
{
  const struct _TExcelStatic *pStatic = pUi->pStatic;
  signed short Count;
  if(pStatic->GetLineCount != NULL){
    Count = pStatic->GetLineCount();
    if(Count >= 0){//直接用户空间获取
      pUi->LineCount = Count; //手动更新有效
      return Count;
    }
  }
  else Count = -16383; //未定义时，置最大允许
  //自动获取
  if(pUi->LineCount > 0) //已自动更新时
    return pUi->LineCount;
  return 0 - Count;//先使用默认值
}

//--------------------------------得到行数据实现--------------------------------
//返回NULL表示结束
const char* TExcel_pGetLine(struct _TExcel *pUi,
                                 unsigned short Line) //指定行(可能有多页)
{
  const struct _TExcelStatic *pStatic = pUi->pStatic;
  
  //得到当前行数据位置
  unsigned short StartLine = pUi->StartLine;
  if((Line < StartLine) || (Line >= (StartLine + pUi->IdCount))){
    //异常数据不在缓冲区中
    memset(pUi->LineBuf, ' ', TEXCEL_LINE_SIZE - 1);
    *(pUi->LineBuf + (TEXCEL_LINE_SIZE - 1)) = '\0';//结束字符
    return pUi->LineBuf; 
  }
  const unsigned char *pParaIdLut = pStatic->pParaIdLut;
  unsigned char ParaLen = *pParaIdLut++;
  unsigned char HaveColor = ParaLen & 0x80;
  ParaLen &= ~0x80;
  
  char *pBuf = pUi->LineBuf;
  
  //支持颜色填充时，通报用户填充默认色
  #ifdef SUPPORT_COLOR //支持颜色时
    TExcel_cbFullDefaultColor(Line);
  #endif
  
  const struct _TExcelParaDesc *pParaDesc = pStatic->pParaDesc;
  //根据对齐方式填充
  for(; ParaLen > 0; ParaLen--){
    unsigned char ParaId = *pParaIdLut++;
    #ifdef SUPPORT_COLOR //支持颜色时
      pUi->Data2ColorSize = 0;//默认不填充
    #endif
    const char *pDispStr = pParaDesc->pGetData(pUi, Line, ParaId); //显示字符串
    if(pDispStr == NULL){//结束了
      pUi->LineCount = pUi->StartLine + Line;//更新总数
      return NULL;
    }
    unsigned char DispInfo = *(pParaDesc->pDispInfo + ParaId);
    unsigned char Space = strFull(pBuf, //缓冲区
                                  pDispStr,        //数据源
                                  DispInfo & TEXCEL_LEN_MASK, //最大长度
               (DispInfo & TEXCEL_ALIGN_MASK) >> TEXCEL_ALIGN_SHIRT);//对齐方式
    //支持颜色填充且用户填充有颜色时，让用户填充当前表格指定位置颜色
    #ifdef SUPPORT_COLOR //支持颜色时
     if(HaveColor){
       unsigned char ColorSize= pUi->Data2ColorSize;
       if(ColorSize == 255){//使用统一颜色时预处理
         ColorSize = strlen(pDispStr);
         Color_Full(pUi->ItemColor, pUi->Data2ColorBuf, ColorSize);
       }
       if(ColorSize > 0){//独立填充颜色
          TExcel_cbFullColor(Line, 
                             (pBuf - pUi->LineBuf) + Space,
                             pUi->Data2ColorBuf, 
                             pUi->Data2ColorSize);          

       }
     }
    #endif //SUPPORT_COLOR
    pBuf += DispInfo & TEXCEL_LEN_MASK;
    if(!(DispInfo & TEXCEL_DIS_SPACE)){//没有禁止间隔时跟空格以间隔
      *pBuf++ = ' ';
    }
  }
  *pBuf = '\0';//最后填充结束字符
  return pUi->LineBuf;
}

//----------------------------------更新数据实现---------------------------------
//返回是否更新完成
signed char TExcel_UpdateData(struct _TExcel *pUi,
                                  unsigned short StartLine, //起始行 
                                  unsigned short Count)    //总数
{
  pUi->StartLine = StartLine;
  //重载页内查找表
  pUi->IdCount = TExcel_cbReloadLut(pUi->Handle, StartLine, 
                                    pUi->IdLut, Count);
  return 1; //直接完成
}

//--------------------------------指定行转换为阵列--------------------------------
unsigned short TExcel_LineToAryId(struct _TExcel *pUi,
                                      unsigned short Line) //指定行
{
  if(Line < pUi->StartLine) return 0xffff;//异常
  Line -= pUi->StartLine;
  if(Line >= pUi->IdCount) return 0xffff;//异常
  return pUi->IdLut[Line];
}

//--------------------------------指定阵列ID转换为行----------------------------
//返回0xffff在本页未找到(稍后才执行), 否则返回找到的行数
unsigned short TExcel_AryIdToLine(struct _TExcel *pUi, unsigned short AryId)
{
  //检查是否在本页
  for(unsigned char Line = 0; Line < pUi->IdCount; Line++){
    if(AryId == pUi->IdLut[Line]) return Line;//找到了
  }
  
  //重载页内查找表，以设备ID方式
  pUi->IdCount = TExcel_cbReloadLut(pUi->Handle,  0x8000 | AryId, 
                                    pUi->IdLut, pUi->PageLine);
  //再次检查是否在本页
  for(unsigned char Line = 0; Line < pUi->IdCount; Line++){
    if(AryId == pUi->IdLut[Line]) return Line;//找到了
  }
  return 0xffff;//未找到设备
  
}

//------------------------------设置当前项颜色-----------------------------
void TExcel_SetItemColor(struct _TExcel *pUi, Color_t ItemColor)
{
  pUi->ItemColor = ItemColor;
  pUi->Data2ColorSize = 255;
}

//--------------------------------设置字符颜色-----------------------------
void TExcel_SetStrColor(struct _TExcel *pUi, 
                        Color_t Color, 
                        unsigned char StrSize)
{
  Color_Full(Color, pUi->Data2ColorBuf, StrSize);
  pUi->Data2ColorSize = StrSize;
}




