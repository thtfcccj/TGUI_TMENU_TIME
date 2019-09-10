//*********************************************************************
//
//			LCD硬件驱动程序-KS0108控制器128*64点阵支持
//
//*********************************************************************/

#include    "LCD_Drv.h"
#include    "TRowDotBuf.h"
#include    "KS0108Cmd.h"
#include    "KS0108Drv.h"
#include    <string.h>

//-------------------------硬件初始化---------------------------------------
static void _HwInit(void)
{
  unsigned char Screen;
  unsigned short i;
  //硬件复位显示屏
  //(这里无硬件复位电路)
  
  //各模块初始化
  LcdDev_CsInit();
  Lcd_Drv_Init();
  
  //=======================各屏初始化==========================
  for(Screen = 0; Screen < 2; Screen++){
    if(!Screen) LcdDev_ClrCs0();
    else  LcdDev_ClrCs1();
    
    //检查是否在正常工作状态
    if(Ks0108Drv_RdState() & KS0108_CMD_STATE_RESET){
      //还在复位状态,异常处理
      Lcd_cbDelayMs(100);
    }
    //清除显示屏显示
    Ks0108Drv_WrState(Ks0108Cmd_GetAdrX(0));
    Ks0108Drv_WrState(Ks0108Cmd_GetAdrY(0));
    for(i = (64*64/ 8); i > 0; i--)
      Ks0108Drv_WrData(0);
    
    //打开显示屏显示
    Ks0108Drv_WrState(Ks0108Cmd_GetDispOn());
    //检查是否在正常显示状态
    if(Ks0108Drv_RdState() & KS0108_CMD_STATE_OFF){
      //还在关半状态,异常处理
      Ks0108Drv_WrState(Ks0108Cmd_GetDispOn());
    }
    //初始化结束
    if(!Screen) LcdDev_SetCs0();
    else LcdDev_SetCs1();
  }
 
}

//-------------------------模块初始化---------------------------------------
void Lcd_Drv_Init(void)	
{
  _HwInit();        //硬件初始化
  TRowDotBuf_Init();//缓冲初始化
  //LCD模块软件初始化


}

/****************************************************************************

							TGUI显示屏回调函数实现
//注:此模块暂不支持DISP_CMD_DIS_UPDATE命令,若有,当做清屏处理																
****************************************************************************/
#include "TGUIBase.h"


//-------------------------显示屏更新一行内容函数--------------------
void TWinMng_cbUpdateRow(unsigned char Lcd,    //当前操作那个显示屏
                         unsigned char x,      //需要更新的行起始位置
                         unsigned char y,      //需要更新的行
                         unsigned char w,      //更新长度
                         char *pBuf,    //需更新字符
                         //每个字符对应位置标志,见下列定义
                         unsigned char *pFlagBuf)
{
  unsigned char i;
  const unsigned char *pDotBuf;
  unsigned short CurAdr;

  //若需支持DISP_CMD_DIS_UPDATE时,需在此进行分块处理!!!

  //先取出整块字模
  pDotBuf = TRowDotBuf_pGetBuf(w,pBuf,pFlagBuf);
  //将字模按行送出,每次更新各屏宽度大小

  //============================左半屏送出=============================
  LcdDev_ClrCs0();
  for(i = 0; i < 128; i++){ //每次送两数
    Ks0108Drv_WrState(Ks0108Cmd_GetAdrX(y));
    Ks0108Drv_WrState(Ks0108Cmd_GetAdrY(i));
    Ks0108Drv_WrData(*(pDotBuf + i));
    i++;
    Ks0108Drv_WrData(*(pDotBuf + i));
  }
  LcdDev_SetCs0();
  
  //============================右半屏送出=============================
  LcdDev_ClrCs1();
  pDotBuf += 128;
  for(i = 0; i < 128; i++){ //每次送两数
    Ks0108Drv_WrState(Ks0108Cmd_GetAdrX(y));
    Ks0108Drv_WrState(Ks0108Cmd_GetAdrY(i));
    Ks0108Drv_WrData(*(pDotBuf + i));
    i++;
    Ks0108Drv_WrData(*(pDotBuf + i));
  }
  LcdDev_SetCs1();
}


/****************************************************************************
                          TRowDotBuf回调函数实现
****************************************************************************/
//使用自定义字库实现:
#include "TWord.h"

//-----------------------得到全角字字模--------------------------
void TRowDotBuf_cbGetWordModule(unsigned char *pBuf, //接收缓冲区
                                unsigned short c)   //字码
{
  unsigned short UserID = TWord_Word2UserID(c,
                                        TWord_GB2312LUT, 
                                        TWord_GetGB2312LUTSize());
  if(UserID == 0xffff) memset(pBuf, 0, 32);//未找到时填充空格
  else memcpy(pBuf,TWord_pGetWordModule(UserID); 32);//得到字模
}

//-----------------------得到半角字字模--------------------------
__arm void TRowDotBuf_cbGetHalfWordModule(unsigned char *pBuf, //接收缓冲区
                                    unsigned char c)    //字码
{
  if(c <= 32) //前33个字符为控制码,且干扰空易丢失,直接填充空格(含空格)
    memset(pBuf, 0, 16);
  else
    memcpy(pBuf, TWord_pGetAsciiModule(c), 16);
}




