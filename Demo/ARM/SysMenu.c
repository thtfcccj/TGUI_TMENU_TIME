/*******************************************************************************

                 TGUI系统菜单实现中需要的相关函数实现

*******************************************************************************/
#include "TMenu.h"
#include "TMenu_CheckBox.h"
#include "TMenu_MNumAdj.h"
#include "TMenu_RadioBox.h"
#include "TMenu_SubMenu.h"

#include <string.h>

/**********************************************************************
                         TMenu用户回调函数实现
**********************************************************************/


/*/-----------------------得到系统当前语种------------------------
enum eLan_t GetLan(void)
{ 
  return (enum eLan_t)0; //一个语种 
}*/

//--------------TMenu所需要的自由数据缓冲------------------
//,>一行用到是最长字符,最后值用于部分中间值的缓冲(如MNumAdj中)
#ifdef TMENU_BUF_SIZE  //定义有缓冲区大小时
  unsigned long _Buf[TMENU_BUF_SIZE / 4];
#else
  unsigned long _Buf[(TLCD_WIDTH + 16) / 4];
#endif 
void *TMenu_pGetBuf(void){
  return _Buf;
}

/**********************************************************************
                         TGUI中部分未实现的回调函数
**********************************************************************/
//----------------------得到常量缓存结构函数-----------------------
//因某些单片机不支持code区指针,故在内核已避免了code区指针问题,
//内核需常量时,使用下列常量临时缓冲存常量数据>= 3;
static char _ConstBuf[8]; 
extern char *TGUI_pGetConstBuf(void)
{
  return _ConstBuf;
}

/*/-------------------------显示屏更新一行内容函数--------------------
void TWinMng_cbUpdateRow(unsigned char Lcd,    //当前操作那个显示屏
                         unsigned char x,      //需要更新的行起始位置
                         unsigned char y,      //需要更新的行
                         unsigned char w,      //更新长度
                         char *pBuf,    //需更新字符
                         //每个字符对应位置标志,见下列定义
                         unsigned char *pFlagBuf)
{
  
  
}*/












