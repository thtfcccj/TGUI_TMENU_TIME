/*******************************************************************************

                        Excel类型数据组织实现
横轴即行为对像阵列查找表ID号，可实现筛选(局部页内查找表，外部实现筛选)
纵轴为对像的属性或状态, 可实现按指定顺序排列
此模块负责缓存Excel类型的一屏数据，与具体UI无关
*******************************************************************************/
#ifndef __TEXCEL_H 
#define __TEXCEL_H

/*******************************************************************************
                            相关配置
*******************************************************************************/

//系统在一页内能够显示下的最大行数
#ifndef TEXCEL_LINE_COUNT
  #define TEXCEL_LINE_COUNT    32         
#endif

//一行内，可能的最大字符串个数(可超过显示屏)
#ifndef TEXCEL_LINE_SIZE
  #define TEXCEL_LINE_SIZE    96 
#endif

#define SUPPORT_COLOR //支持颜色时定义

/*******************************************************************************
                            相关结构
*******************************************************************************/

#ifdef SUPPORT_COLOR //支持颜色时
  #include "Color.h"
#endif

//----------------------------------参数描述------------------------------------
//ParaId： 即功能ID: 将结构对像阵列的成员函数分类排列，下标即为功能ID
//此功能为参数描述结构，通过此描述与结构对像交换数据
struct _TExcel;
struct _TExcelParaDesc{
  //参数ID显示信息，见定义
  const unsigned char *pDispInfo;
  //得到参数ID标题头字符串
  const char *(*pGetHeader)(unsigned char ParaId); 
  //得到功能Id对应的字符串显示字符,返回NULL数据结束
  const char *(*pGetData)(struct _TExcel *pUi,
                           unsigned short InLine, //AryId = pUi->IdLut[InLine] 
                           unsigned char ParaId);
};

//参数ID显示信息定义为：
#define TEXCEL_DIS_SPACE    0x80     //参数后面禁止跟空行
#define TEXCEL_ALIGN_SHIRT  5         //对齐方式
#define TEXCEL_ALIGN_MASK   0x60     //
#define TEXCEL_ALIGN_L      0x00     //左对齐
#define TEXCEL_ALIGN_C      0x20     //局中对齐
#define TEXCEL_ALIGN_R      0x40     //右对齐
#define TEXCEL_LEN_MASK     0x1f    //占位宽度

//-----------------------------静态结构-----------------------------
struct _TExcelStatic{
  const struct _TExcelParaDesc *pParaDesc;      //参数信息描述
  //列参数ID查找表，首位为Para长度(b0-6)与颜色标识(b7)，
  //下标(1开始)为列顺序，对应值为参数ID， 
  const unsigned char *pParaIdLut;
  //得到项总数(总行数),返回负值表示自动从查找表中得出,值仅为参考
  //此成员为NULL时表示动态获取总数
  signed short(*GetLineCount)(void);
};

//-------------------------------关于颜色支持----------------------------------
//当系统支持颜色，且本表需在着色时，需满足下述要求与步骤：
//1. 实现TExcel_cbFullDefaultColor()与TExcel_cbFullColor()函数
//2. pParaIdLut->首位长度的最高位置位
//3. 在struct _TExcelParaDesc->pGetFunString()函数中:
//  (1. 调用TExcel_SetItemColor()， 填充当前项颜色
//  (2. 或调用TExcel_SetStrColor()，可分别填充本项内不同字符不同颜色
//注：暂只支持置前景色

//---------------------------------主结构---------------------------------------
#include "Color.h"

struct _TExcel{
  //数据描述相关：
  const struct _TExcelStatic *pStatic; //静态数据结构
  
  unsigned char Handle;                    //用户带入句柄
  unsigned char PageLine;                  //当前页显示行数
  //使用局部(本页面)ID查找表:
  unsigned char IdCount;                   //本页内查询到的ID总数,<=PageLine
  unsigned short LineCount;                //即总行数，非0表示自动获取  
  unsigned short StartLine;                //本页对应的起始行位置 
  unsigned short IdLut[TEXCEL_LINE_COUNT]; //ID查找表

  //行数据缓冲区相关:
  char LineBuf[TEXCEL_LINE_SIZE];           //组织一行数据时行字符串缓冲区
  unsigned char LineStrLen;                 //本次行字符长度
  
  #ifdef SUPPORT_COLOR //支持颜色时
    unsigned char Data2ColorSize;         //支持颜色填充时,颜色信息填充个数 
    Color_t ItemColor; //Data2ColorSize = 255时，用此指定的颜色填充
    Color_t Data2ColorBuf[TEXCEL_LEN_MASK]; //支持颜色填充时，颜色信息填充
  #endif

  unsigned short JumpAryId;             //需跳转的阵列ID，0xffff表示无此功能
    
};

//Flag定义为：
//UiCom通讯类型相关(按优先级排列, 仅允许通讯部分更改)
#define TEXCEL_VALID_LUT    0x80 //查找表已更新
#define TEXCEL_UPDATE_FINAL 0x08 //本周期更新完成标志

//其它:
#define TEXCEL_NEXT_REFRESH 0x01 //下次要更新所有数据标志


/******************************************************************************
                                  相关行函数
*******************************************************************************/

//---------------------------------初始化函数-----------------------------------
void TExcel_Init(struct _TExcel *pUi,
                 unsigned char Handle,        //句柄
                 const struct _TExcelStatic *pStatic,//静态数据结构
                 unsigned char PageLine);     //当前页显示行数，0表示所有 

//------------------------------设置当前项颜色-----------------------------
void TExcel_SetItemColor(struct _TExcel *pUi, Color_t ItemColor);

//--------------------------------设置字符颜色-----------------------------
void TExcel_SetStrColor(struct _TExcel *pUi, 
                        Color_t Color, 
                        unsigned char StrSize);

//-----------------------------由当前行得到查找表ID值------------------------
unsigned short TExcel_GetIdLutVol(const void *pHandle,
                                   unsigned short Line);

//-----------------------------本次是否更新完成-----------------------------
#define TExcel_IsCurFinal(ui) (1)
  
/******************************************************************************
                            相关行函数-UI部分
*******************************************************************************/  
//------------------------------得到标题头实现----------------------------------
const char* TExcel_pGetHeader(struct _TExcel *pUi);

//---------------------------得到数据行总数实现----------------------------------
unsigned short TExcel_GetLineCount(struct _TExcel *pUi);

//--------------------------------得到行数据实现--------------------------------
//返回NULL表示结束
const char* TExcel_pGetLine(struct _TExcel *pUi,
                                 unsigned short Line); //指定行

//----------------------------------更新数据实现---------------------------------
//返回是否更新完成(因数据更新有延时，永远返回0即当前数据未完成)
signed char TExcel_UpdateData(struct _TExcel *pUi,
                                  unsigned short StartLine, //起始行 
                                  unsigned short Count);    //总数

//--------------------------------指定行转换为阵列--------------------------------
unsigned short TExcel_LineToAryId(struct _TExcel *pUi,
                                      unsigned short Line); //指定行

//--------------------------------指定阵列ID转换为行----------------------------
//返回0xffff在本页未找到(稍后才执行), 否则返回找到的行数
unsigned short TExcel_AryIdToLine(struct _TExcel *pUi, unsigned short AryId);

//----------------------------------刷新当前数据实现----------------------------
//强制更新当前页数据
//void ExcelUcCom_Refresh(struct _ExcelUcRec *pUi);
#define TExcel_Refresh(ui)  do{(ui)->Flag |= TEXCEL_NEXT_REFRESH;}while(0)

/******************************************************************************
                                  回调函数
*******************************************************************************/

//------------------------------重载查找表函数----------------------------------
//NextStartLine: b15置位表示此行为设备ID，下次需保证查找到内有此ID
//返回实际需装载查找表数量, <= Count
unsigned char TExcel_cbReloadLut(unsigned char Handle,        //句柄
                                  unsigned short NextStartLine,//页内新的起始行
                                  unsigned short *pIdLut,      //需填充的查找表                                 
                                  unsigned char Count);        //需装载查找表数量


//------------------------填充指定数据行默认颜色---------------------------------
#ifdef SUPPORT_COLOR //支持颜色时
  void TExcel_cbFullDefaultColor(signed char y); //本页内对应数据行,负为标题
#endif

//-----------------------------更新指定位置颜色---------------------------------
#ifdef SUPPORT_COLOR //支持颜色时
  void TExcel_cbFullColor(unsigned char y,
                          unsigned char x,
                          Color_t *pColorId, //用户指定的颜色ID(非系统色)
                          unsigned char Len);
#endif 
  

#endif  //__TEXCEL_H

