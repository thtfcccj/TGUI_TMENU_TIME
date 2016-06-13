/* ----------------------------------------------------------------------------
 *                TMenu单选框实现
 * --------------------------------------------------------------------------*/
#include "TMenu.h"
#include "TMenu_Buf.h"
#include "string.h"

//其中:标志定义为:
#define _FLAG_RETURN    0x01      //置此标志认为是退出上一层

//------------------------创建函数实现----------------------------
//1:创建该类型菜单,从用户区获得数据并更新显示
//返回创建成功:
signed char TMenu_BufCreate(const TMenu_t *pMenu,    //指向的菜单系统
                                 TWinHandle_t hWin,       //分配给菜单的窗口
                                 unsigned char Data,    //带入的自由数据,这里无意义
                                 void *pv)      //可供使用的自由缓冲区
{
  struct _BufData *pBufData = (struct _BufData *)pv;
  TWin_t *pWin = TWM_pGetWin(hWin);

  pMenu = pMenu;
  Data = Data;
  pBufData->hWin = hWin;
  pBufData->Flag = 0;  
  pBufData->User.Notify = 0;
  pBufData->User.w = TWin_GetW(pWin);
  pBufData->User.h = TWin_GetH(pWin);
  pBufData->User.pBuf = TWin_pGetString(pWin,0);

  //从用户空间获得显示
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_DATA,&pBufData->User);
  TWin_ClrFocus(pWin);//禁止显示焦点区
  TWin_Disp(pWin); //更新整屏显示
  
  return -1;
}

//--------------------------强制刷新函数实现-----------------------------
void TMenu_BufRefresh(const TMenu_t *pMenu,    //指向的菜单系统
                      void *pv)      //已被使用的自由数据缓冲区
{
  struct _BufData *pBufData = (struct _BufData *)pv;
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_DATA,&pBufData->User);//更新显示缓冲区
  TWin_Disp(TWM_pGetWin(pBufData->hWin));
}

//------------------------响应按键函数实现----------------------------
//2:响应按键操作并根据情况与用户进行数据交互,并更新显示
//返回值定义为: 正值:进入相应子菜单,-1,不管,-2:返回上层菜单
signed char TMenu_BufKey(const TMenu_t *pMenu,    //指向的菜单系统
                              unsigned char Key,       //送入的键值
                              void *pv)      //已被使用的自由数据缓冲区
{
  unsigned char Notify;
  struct _BufData *pBufData = (struct _BufData *)pv;

  //通知用户区按键处理
  pBufData->User.Key = Key;
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_BUF_KEY,&pBufData->User);
  Notify = pBufData->User.Notify;

  pBufData->User.Notify = 0;
  switch(Notify & TM_BUF_NOTIFY_MASK){
  //更新整屏显示
  case TM_BUF_NOTIFY_DISP:
    TMenu_BufRefresh(pMenu,pv);
    break;

  //其它功能扩展

  default:break;
  }

  //最后检查退出此菜单
  if(Notify & TM_BUF_NOTIFY_EXIT_FLAG)
    return -2;
  


  return -1;
}

