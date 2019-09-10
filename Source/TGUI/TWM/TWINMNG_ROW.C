/*******************************************************************************

        TWin窗口管理器模块接口-于行更新,固定优先级的窗口的实现

*******************************************************************************/
#include "TWinMng_Row.h"
#include <string.h>

TWinMng_t *pCurTWinMng;    //当前管理器
/**********************************************************************
                          行为函数实现
**********************************************************************/

//------------------设置指定GUI管理器为当前函数-----------------------
//开机时调用此函数初始化
void TWinMng_SetCur(TWinMng_t *pTWinMng)
{
  pCurTWinMng = pTWinMng;
}

//-------------------------窗口管理器初始化函数----------------------------
void TWinMng_Init(TWinMng_t *pTWinMng,
                  unsigned char Lcd,    //当前操作那个显示屏
                  TWin_t *pTWinAry,     //当前窗口阵列头
                  unsigned char Count) //当前窗口阵列大小,<=TWIN_MAX_COUNT
{
  memset(pTWinMng,0,sizeof(TWinMng_t));
  pTWinMng->Lcd = Lcd;
  pTWinMng->pTWinAry = pTWinAry;
  pTWinMng->Count = Count;
}

//-------------------------得到更新行函数----------------------------
static unsigned char _GetUpdatedRow(void)
{
  unsigned char Row;
  RowMask_t RowUpdate = pCurTWinMng->RowUpdate;
  unsigned char PrvUpdatedRow = pCurTWinMng->PrvUpdatedRow;
  //检查需要更新的有效行,从下行开始优先更新
  for(Row = PrvUpdatedRow + 1; Row < TLCD_HIGH; Row++){
    if(RowUpdate & RowShiftLUT[Row]) break; 
  }
  if(Row >= TLCD_HIGH){  //回环
    for(Row = 0; Row < PrvUpdatedRow; Row++){
      if(RowUpdate & RowShiftLUT[Row]) break; 
    }
  }
  //指出该行已更新
  pCurTWinMng->RowUpdate = RowUpdate & (~(RowShiftLUT[Row]));
  pCurTWinMng->PrvUpdatedRow = Row;//指出已更新到该行
  return Row;
}


//--------------------得到当前行的有效窗口函数-------------------------
static unsigned char _GetValidWinMask(unsigned char Row)
{
  WinMask_t ValidWinMask = 0;         //有效窗口
  TWin_t *pWin = pCurTWinMng->pTWinAry;
  WinMask_t i;
  unsigned char Count = pCurTWinMng->Count;
  for(i = 0; i < Count; i++){
   //没有隐藏且在显示范围内时更新
   if((!TWin_IsHidden(pWin)) && (TWin_GetY(pWin)<= Row) && 
      ((TWin_GetY(pWin) + TWin_GetH(pWin)) > Row)){
        ValidWinMask |= WinShiftLUT[i];
    }
   pWin++;
  }
  return ValidWinMask;
}

//-------------------------更新行缓冲区函数----------------------------
//此函数一次仅更新一行缓冲区
//周期调用此函数更新显示TWinMng数据到行缓冲区
//返回0无需更新,返回1需要更新
signed char TWinMng_UpdateRow(void)
{
  unsigned char Row;
  WinMask_t ValidWinMask;         //当前行有效影响的窗口
  unsigned char x,WinX;		     //填充位置,窗体内的位置
  TWin_t *pWin;	                 //当前窗口，下次有效窗口
  unsigned char WinCount = pCurTWinMng->Count;
  unsigned char i;
  
  //解决全角对齐问题增加：
  unsigned char Index;       //全角字符计数器
  char Sign;                     //字符 
  TWin_t *pPrvWin = NULL;	 //上次有效窗口  
  
  WinCount = pCurTWinMng->Count;
  if(pCurTWinMng->Flag & TWINMNG_CONTEXT_BUF_RDY) return 0;  //该行还未送显呢，暂不更新
  if(!pCurTWinMng->RowUpdate) return 0;  //无行需要更新

  Row = _GetUpdatedRow();		//找到更新行
  ValidWinMask = _GetValidWinMask(Row); //找到位于该行上的窗口
  
  //======================窗口重叠算法==========================
  //暂一个一个更新字符
  for(x = 0; x < TLCD_WIDTH; x++){
    //找到最前面窗口的字符
    pWin = pCurTWinMng->pTWinAry + (WinCount - 1);
    for(i = WinCount; i > 0; i--){
      if(ValidWinMask & WinShiftLUT[i - 1]){//有效的窗口时
        //得到该字符串,转换为相对坐标  
        Sign = TWin_GetChar(pWin,x - TWin_GetX(pWin),
                            Row - TWin_GetY(pWin));
        WinX = TWin_GetX(pWin);
        if((x >= WinX) && (x < (WinX + TWin_GetW(pWin)))){//在范围内
          //检查全角对齐检查
          if(pPrvWin != pWin){//跨窗口了
            if(Index & 0x01)//上个窗口最后一字全角未对齐
              pCurTWinMng->RowBuf[x - 1] = ' ';
            pPrvWin = pWin;
            Index = 0x80; //重新开始,首行标志
          }
          //全角统计
          if(Sign >= 0x80) Index++;
          else{
            //若第一次跨窗口且首个全角字未对齐时处理
            if((Index >= 0x80) && (Index & 0x01))
              pCurTWinMng->RowBuf[x - (Index - 0x80)] = ' ';
            Index = 0; //半角重新开始
          }
          
          if(!TWin_IsLattice(pWin)){
            //得到该字符串,转换为相对坐标  
            pCurTWinMng->RowBuf[x] = Sign;
            pCurTWinMng->RowFlagBuf[x] = 
               DISP_CMD_NOR | TWin_IsFocus(pWin,x - TWin_GetX(pWin),
                                                Row - TWin_GetY(pWin));
          }
          //点阵模式更新由用户处理
          else pCurTWinMng->RowFlagBuf[x] = DISP_CMD_DIS_UPDATE;
          break;//找到了
        }
      }
      pWin--;
    }//end i;
  }//end x;
  pCurTWinMng->Flag |= TWINMNG_CONTEXT_BUF_RDY;//该行已准备好了
  return 1;
}

//-------------------------更新显示函数----------------------------
//将行缓冲区数据送出
//周期调用此函数更新TWinMng行缓冲区数据到显示屏
void TWinMng_UpdateLcd(void)
{
  if(!(pCurTWinMng->Flag & TWINMNG_CONTEXT_BUF_RDY)) return;//行数据未准备好
  
  //最左侧为全角字时检查左全角字是否被截掉，若是则填充空
  if(pCurTWinMng->RowBuf[0] >= 0x80){
   for(unsigned char i = 1; i < TLCD_WIDTH; i++){
     if(pCurTWinMng->RowBuf[i] < 0x80){
       if(i & 0x01) pCurTWinMng->RowBuf[0] = ' ';
       break;
     }
   }
  }
  
  //调用回调函数送显
  TWinMng_cbUpdateRow(pCurTWinMng->Lcd,0, pCurTWinMng->PrvUpdatedRow,
                      TLCD_WIDTH,pCurTWinMng->RowBuf,pCurTWinMng->RowFlagBuf);
  
  pCurTWinMng->Flag &= ~TWINMNG_CONTEXT_BUF_RDY;//更新完成
}

/**********************************************************************
                      TWin回调函数实现
**********************************************************************/


//---------------------通报窗口某些区域被更新函数----------------------
void TWin_cbUpdateNotify(signed char x,signed char y, 
                         signed char w, signed char h)
{
  //改变时,整行均更新
  RowMask_t RowUpdate = 0;

  x = x; w = w;//防止部分编译器警告
  if(y < 0){//超过显示屏部分不更新
    if(h <= y) return;//在显示屏外
    h -= y;
    y = 0; 
  }
    
  h += y;//结束位置
  if(h > TLCD_HIGH) h = TLCD_HIGH;
  for(; y < h; y++)
    RowUpdate |= RowShiftLUT[y];
  pCurTWinMng->RowUpdate |= RowUpdate; //行更新标志
}

//---------------------通报窗口马上清屏函数实现----------------------------
void TWin_cbClrNowNotify(signed char x,signed char y, 
                         signed char w, signed char h)
{
  unsigned char i;
  unsigned char *pRowFlagBuf;
  if(y < 0){//超过显示屏部分不更新
    if(h <= y) return;//在显示屏外
    h -= y;
    y = 0; 
  }
  h += y;//主表示结束位置
  if(h > TLCD_HIGH) h = TLCD_HIGH;

  if(x < 0){//超过显示屏部分不更新
    if(w <= x) return;//在显示屏外
    w -= x;
    x = 0; 
  }
  if((w + x) > TLCD_WIDTH) w = TLCD_WIDTH - x;

  //先置禁止更新标志,因该区域不允许更新
  //pRowFlagBuf = &pCurTWinMng->RowFlagBuf; 
  //i = TLCD_WIDTH;
  //while(i--) *pRowFlagBuf++ = DISP_CMD_DIS_UPDATE;
  //设置清屏标志
  pRowFlagBuf = &pCurTWinMng->RowFlagBuf[x];
  i = w;
  while(i--) *pRowFlagBuf++ = DISP_CMD_CLR;
  //发出清屏命令，立即清屏
  pRowFlagBuf = pCurTWinMng->RowFlagBuf;
  for(; y < h; y++){
    //调用回调函数送显
    TWinMng_cbUpdateRow(pCurTWinMng->Lcd,x, y, w,
                        pCurTWinMng->RowBuf,pRowFlagBuf);
     
  }
}


