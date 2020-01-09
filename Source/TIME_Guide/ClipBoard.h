/*****************************************************************************

                         剪切板结构定义                   
此模块属于TImeMng的子模块，内部直接使用
*****************************************************************************/
#ifndef __CLIP_BOARD_H
#define __CLIP_BOARD_H

/*****************************************************************************
                          用户配置区
*****************************************************************************/

#ifndef CLIP_BOARD_BUF_SIZE
  #define CLIP_BOARD_BUF_SIZE   255
#endif

#if CLIP_BOARD_BUF_SIZE > 255
    #define ClipBoardSizt_t   unsigned char
#else
    #define ClipBoardSizt_t   unsigned short
#endif

/*****************************************************************************
                        相关结构
*****************************************************************************/

//剪切板定义
struct _ClipBoard{
  char Buf[CLIP_BOARD_BUF_SIZE]; //剪切缓冲区
};


extern struct _ClipBoard ClipBoard; //直接单例化(TImeMng中实例化)
 
/*****************************************************************************
                                 相关函数
*****************************************************************************/

//---------------------------初始化函数---------------------------------------
//开机时调用
void ClipBoard_Init(void);

#endif //#define CLIP_BOARD

