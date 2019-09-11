/*****************************************************************************

                         剪切板结构定义                   

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
  ClipBoardSizt_t ValidLen;                //已剪切区域大小,0时无定义
};

#endif //#define CLIP_BOARD

