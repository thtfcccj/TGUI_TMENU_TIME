/*******************************************************************************

        TWin窗口管理器模块接口-于行更新,固定优先级的窗口的实现接口

*******************************************************************************/
#ifndef __T_WIN_MNG_ROW_H 
#define __T_WIN_MNG_ROW_H

//--------------------------TWinMng说明--------------------------------
//TWinMng为TWIN的一个实例化窗口管理器,主要负责管理窗口的分配和的窗口优先级显示
//此管理器具有以下特性:
//1:基于行的更新,即直接更新窗口所在行
//2:固定的窗口个数和窗口显示优先级:使用窗口陈列分配窗口,下标越大,优先级越高
//3:多显示屏支持

//使用此实例的部分应按应用固定分配好优先级和窗口的分配
//在系统开机时分配好窗口的显示缓存位置

/**********************************************************************
                      相关配置区
**********************************************************************/
#include "TWM.h"    //为该模块的实例化

//定义最大窗口个数,<=64个
#ifndef TWIN_MAX_COUNT
  #define       TWIN_MAX_COUNT    8  
#endif

//定义窗口屏蔽位数(一位对应一行)及查找表
#if(TWIN_MAX_COUNT > 32)
  #define WinMask_t unsigned long long
  #define WinShift2Mask(shift)  LlShift2Mask(shift) //不支持查找表
#elif(TWIN_MAX_COUNT > 16)
  #define WinMask_t unsigned long
  #ifdef U32ShiftLUT
    #define WinShift2Mask(shift) U32ShiftLUT[shift]
  #else
    #define WinShift2Mask(shift) ((WinMask_t)1 << (shift))
  #endif
#elif(TWIN_MAX_COUNT > 8) 
  #define WinMask_t unsigned short
  #ifdef U16ShiftLUT
    #define WinShift2Mask(shift) U16ShiftLUT[shift]
  #else
    #define WinShift2Mask(shift) ((WinMask_t)1 << (shift))
  #endif
#else
  #define WinMask_t unsigned char
  #define WinShift2Mask(shift) U8ShiftLUT[shift]       //仅支持查找表
#endif

//定义行屏蔽位数(一位对应一行)及查找表
#if(TLCD_HIGH > 32)
  #define RowMask_t unsigned long long
  #define RowShift2Mask(shift)  LlShift2Mask(shift) //不支持查找表
#elif(TLCD_HIGH > 16)
  #define RowMask_t unsigned long
  #ifdef U32ShiftLUT
    #define RowShift2Mask(shift) U32ShiftLUT[shift]
  #else
    #define RowShift2Mask(shift) ((RowMask_t)1 << (shift))
  #endif
#elif(TLCD_HIGH > 8) 
  #define RowMask_t unsigned short
  #ifdef U16ShiftLUT
    #define RowShift2Mask(shift) U16ShiftLUT[shift]
  #else
    #define RowShift2Mask(shift) ((RowMask_t)1 << (shift))
  #endif
#else
  #define RowMask_t unsigned char
  #define RowShift2Mask(shift) U8ShiftLUT[shift]      //仅支持查找表
#endif

/**********************************************************************
                           相关结构
**********************************************************************/
//窗口管理管理器
typedef struct{
  TWin_t *pTWinAry;  //当前窗口阵列
  unsigned char Count;//窗口阵列大小  
  unsigned char Lcd;    //当前操作那个显示屏
  RowMask_t RowUpdate;  //行更新标志
  unsigned char PrvUpdatedRow;  //上次已被更新的行
  unsigned char Flag;   //标志，见定义
  char RowBuf[TLCD_WIDTH]; //当前需更新行的缓冲
  unsigned char RowFlagBuf[TLCD_WIDTH]; //当前行标志缓冲,见TGUICfg.h定义
}TWinMng_t;

//其中标志定义为：
#define		TWINMNG_CONTEXT_BUF_RDY	0x01	//行标志准备好
   
extern TWinMng_t *pCurTWinMng;    //当前管理器

/*****************************************************************************
                          行为函数
*****************************************************************************/

//-------------------------窗口管理器初始化函数----------------------------
void TWinMng_Init(TWinMng_t *pTWinMng,
                  unsigned char Lcd,    //当前操作那个显示屏
                  TWin_t *pTWinAry,     //当前窗口阵列头
                  unsigned char Count); //当前窗口阵列大小,<=TWIN_MAX_COUNT

//------------------设置指定GUI管理器为当前函数-----------------------
//开机时调用此函数初始化
void TWinMng_SetCur(TWinMng_t *pTWinMng);

//-------------------------更新行缓冲区函数----------------------------
//此函数一次仅更新一行缓冲区
//周期调用此函数更新显示TWinMng数据到行缓冲区
//返回0无需更新,返回1需要更新
signed char TWinMng_UpdateRow(void);

//-------------------------更新显示函数----------------------------
//将行缓冲区数据送出
//周期调用此函数更新TWinMng行缓冲区数据到显示屏
void TWinMng_UpdateLcd(void);

#endif //#ifndef __T_WIN_MNG_ROW_H 

