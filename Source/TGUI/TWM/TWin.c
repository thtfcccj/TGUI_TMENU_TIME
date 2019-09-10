/*******************************************************************************

                           TGUI窗口相关实现
TGUI窗口是所有其它控件的基类窗口
*******************************************************************************/
#include "TWin.h"
#include <string.h>

/**********************************************************************
                          行为函数实现
**********************************************************************/

//-------------------------初始化窗口函数----------------------------
//开机时调用此函数初始化
void TWin_InitWin(TWin_t *pWin,       //需初始化的窗口
                  signed char w,    //原始宽度
                  signed char h,    //原始高度
                  char *pBuf,//缓冲区
                  unsigned short BufSize)//缓冲区大小
{
  memset(pWin,0,sizeof(TWin_t));
  pWin->Rect.w = w;
  pWin->Rect.h = h;
  pWin->pBuf = pBuf;
  memset(pBuf,' ',sizeof(TWin_t));  //初始化为空
  pWin->BufSize = BufSize;
}

//-------------------------设置标志函数----------------------------
static void _SetFlag(TWin_t *pWin,unsigned char Flag)
{
  pWin->Flag = Flag; 
  TWin_cbUpdateNotify(pWin->Rect.x,
                      pWin->Rect.y,
                      pWin->Rect.w,
                      pWin->Rect.h);//需要更新整个显示区
}



//-------------------------窗口隐藏函数----------------------------
void TWin_Hidden(TWin_t *pWin)
{
  _SetFlag(pWin,pWin->Flag | TWIN_HIDDEN); 
}

//-------------------------窗口显示函数----------------------------
void TWin_Disp(TWin_t *pWin)
{
  _SetFlag(pWin,pWin->Flag & (~TWIN_HIDDEN)); 
}

//-------------------------窗口移动函数----------------------------
//窗口移动,使用绝对坐标
void TWin_Move(TWin_t *pWin, signed char x, signed char y)
{
  signed char PrvX = pWin->Rect.x;  
  signed char PrvY = pWin->Rect.y;
  pWin->Rect.x = x;
  pWin->Rect.y = y;
  //需要更新前后两个显示区
  TWin_cbUpdateNotify(x,y,pWin->Rect.w,pWin->Rect.h);
  TWin_cbUpdateNotify(PrvX,PrvY,pWin->Rect.w,pWin->Rect.h);
}

//-------------------------窗口改变大小函数----------------------------
void TWin_ReSize(TWin_t *pWin, signed char w, signed char h)
{
  signed char PrvW = pWin->Rect.w;  
  signed char PrvH = pWin->Rect.h;
  pWin->Rect.w = w;
  pWin->Rect.h = h;
  //需要更新前后两个显示区
  TWin_cbUpdateNotify(pWin->Rect.x,pWin->Rect.y,w,h);
  TWin_cbUpdateNotify(pWin->Rect.x,pWin->Rect.y,PrvW,PrvH);
}

//-------------------------得到指定行缓冲区函数----------------------------
//此函数用于直接更新行缓冲区数据,注意更新长度小于窗体宽度
char *TWin_pGetString(TWin_t *pWin,
                               signed char y)//在窗口内的相对纵座标
{
  if(y >= pWin->Rect.h) return  NULL;//超出窗口显示了
  //通报该行需重绘
  TWin_cbUpdateNotify(pWin->Rect.x,pWin->Rect.y + y,
                      pWin->Rect.w,1);      
  return  pWin->pBuf + pWin->Rect.w * y;
}

//-------------------------★得到指定位置字符函数----------------------------
//x,y为窗口内的相对坐标
char TWin_GetChar(TWin_t *pWin,signed char x,signed char y)            
{
 return *(pWin->pBuf + pWin->Rect.w * y + x);
}


//-------------------------更新一行字符函数----------------------------
//当字符串不够时,后面将清为0,当字符串过多时,后面将不会显示
void TWin_SetString(TWin_t *pWin, 
                    signed char y,              //在窗口内的相对纵座标
                    const char *pString) //字符串
{
  signed char w;
  unsigned char String;
  char *pBuf;
  char *pEndBuf;
  if(y >= pWin->Rect.h) return; //不在窗口内
  
  w = pWin->Rect.w;
  pBuf = pWin->pBuf;
  pEndBuf = pBuf + pWin->BufSize;
  pBuf += y * pWin->Rect.w;//当前起始位置
  if(pBuf >= pEndBuf) return;   //出错了!
  if((pBuf + w) < pEndBuf) pEndBuf = pBuf + w;
  //else //防止内存超限
  //填充内容
  for(; pBuf < pEndBuf; pBuf++){
    String = *pString++;
    if(String < 0x20) break; //结束了
    *pBuf = String;
  }
  //不够时后面填充空格
  for(; pBuf < pEndBuf; pBuf++)
    *pBuf = ' ';
  //通报该行需重绘
  TWin_cbUpdateNotify(pWin->Rect.x,pWin->Rect.y + y,
                      pWin->Rect.w,1);  
}

//-------------------------设置焦点函数----------------------------
//移动或置焦点时调用此函数
//当宽度为0时,表示取消窗口焦点显示
void TWin_SetFocus(TWin_t *pWin,
                   signed char x, //在窗口中的x位置
                   signed char y, //在窗口中的y位置
                   signed char w) //宽度
{
  TWin_cbUpdateNotify(pWin->Rect.x + pWin->Focus.x,
                      pWin->Rect.y + pWin->Focus.y,
                      pWin->Focus.w,1);//原焦点区重绘
  //注:未检查焦点区的有效性!
  pWin->Focus.x = x;
  pWin->Focus.y = y; 
  pWin->Focus.w = w; 
  TWin_cbUpdateNotify(pWin->Rect.x + x,
                      pWin->Rect.y + y,
                      w,1);//新焦点区重绘
}
//-------------------------判断某个位置是否在焦点上函数----------------
//调用此函数确保该绝对位置在该窗口内
//是则返回1,否则返回0
unsigned char TWin_IsFocus(TWin_t *pWin,
                           signed char x,   //焦点相对坐标x
                           signed char y)   //焦点相对坐标y                
{
  signed char WinX;
  if(pWin->Focus.y == y){	//在焦点行上
    WinX = pWin->Focus.x;
    if((x >= WinX) && (x < (WinX + pWin->Focus.w)))
      return 1;
  }
  return 0;
}

/**********************************************************************
                        点阵模式处理
**********************************************************************/

//-------------------------★窗口置为点阵模式函数----------------------
void TWin_SetLattice(TWin_t *pWin)
{
  _SetFlag(pWin,pWin->Flag | TWIN_LATTICE); 
}

//-------------------------★窗口取消点阵模式函数----------------------
void TWin_ClrLattice(TWin_t *pWin)
{
  _SetFlag(pWin,pWin->Flag & (~TWIN_LATTICE)); 
}

//---------------------------点阵区域清屏函数---------------------------
//立即清除点阵区域的显示,交由用户画图
void TWin_ClrLatticeDisp(TWin_t *pWin)
{
  if(pWin->Flag  & TWIN_LATTICE){  //置为点阵模式有效
    TWin_cbClrNowNotify(pWin->Rect.x,
                        pWin->Rect.y,
                        pWin->Rect.w,
                        pWin->Rect.h);//需要更新整个显示区
	}
  _SetFlag(pWin,pWin->Flag);//重画该区域
}
