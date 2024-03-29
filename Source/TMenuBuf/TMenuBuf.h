/*******************************************************************************

                           菜单编辑临时缓冲区

*******************************************************************************/

#ifndef __TMENU_BUF_H 
#define __TMENU_BUF_H


/*******************************************************************************
                           相关结构
*******************************************************************************/
struct _TMenuBuf{
  char Str[64];             //当前编辑字符串缓冲区
  unsigned char Lut[64];   //菜单查找表缓冲区
  unsigned long Para;      //指令参数交换缓冲区
};

extern struct _TMenuBuf TMenuBuf;


/*******************************************************************************
                                  相关函数
*******************************************************************************/
//-----------------------------头部插入查找表函数--------------------------------
//将原有查找表插入已存在的TMenuBuf.Lut中，并返回TMenuBuf.Lut
const unsigned char *TMenuBuf_pLutInsertH(unsigned char NewCount,//.Lut已有大小
                                           const unsigned char *pLut);//原表



#endif

