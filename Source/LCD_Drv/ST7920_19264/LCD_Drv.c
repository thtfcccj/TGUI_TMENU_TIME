//*********************************************************************
//
//			         LCD硬件驱动程序
//
//
//
//*********************************************************************/

#include    "LCD_Drv.h"
#include    "ST7920_Command.h"

//-----------------清除显示屏显示--------------------------
static void LcdClr(void)
{
	//这里为清GDRAM区
	//unsigned char i;

	Lcd_UpperWrCommand(Extend8B());		//功能设定
	Lcd_UnderWrCommand(Extend8B());		 
	//启用GDRAM区混合显示模式
	Lcd_UpperWrCommand(ExtendFunSet(bInterfaceCtlBit | bInstructionSetCtl | bGraphicDispCtl));//扩充功能设定,开启图形显示区
	Lcd_UpperWrCommand(ExtendFunSet(bInterfaceCtlBit | bInstructionSetCtl | bGraphicDispCtl));//扩充功能设定,开启图形显示区

	/*/设定GDRAM地址并将该区域清零
	Lcd_UpperWrCommand(SetGDRAMAddr(0));	 //垂直地址
	Lcd_UpperWrCommand(SetGDRAMAddr(0));	//水平地址
	Lcd_UpperWrDataRdy();
	for(i = 0; i < (64 * 32); i++){//GDRAM区清零
		Lcd_UpperWrData(0);
	} 		
	Lcd_UnderWrCommand(SetGDRAMAddr(0));//垂直地址
	Lcd_UnderWrCommand(SetGDRAMAddr(0));//水平地址
	Lcd_UnderWrDataRdy();
	for(i = 0; i < (64 * 32); i++){//GDRAM区区清零
		Lcd_UnderWrData(0);
	}*/

  Lcd_UpperWrCommand(Base8B()); //回到8位并口模式
  Lcd_UnderWrCommand(Base8B()); 

}

//-----------------lcd初始化---------------------------------
//是否允许清屏标志,不清屏时将显示LOGO
void LcdInit(unsigned char EnInitScreen)	
{
   //===========================IO硬件初始化========================
   Lcd_HwInit();

   //===========================基础硬件初始化========================
   Lcd_UpperWrCommand(Base8B()); //8位并口模式
   Lcd_UnderWrCommand(Base8B());
   Lcd_cbDelayUs(120);
   Lcd_UpperWrCommand(Base8B()); //8位并口模式,硬件要求两次
   Lcd_UnderWrCommand(Base8B());
   Lcd_cbDelayUs(40);
   Lcd_UpperWrCommand(DisplaySet(0)); //关显示
   Lcd_UnderWrCommand(DisplaySet(0));
   Lcd_cbDelayUs(120);
   Lcd_UpperWrCommand(EntryModeSet(bAddNoMove)); //输入数据时，AC值增加模式
   Lcd_UnderWrCommand(EntryModeSet(bAddNoMove));
   Lcd_cbDelayUs(120);
   Lcd_UpperWrCommand(ClearDisplay()); //清DDRAM区
   Lcd_UnderWrCommand(ClearDisplay());
   Lcd_cbDelayUs(12);//>10MS

   if(EnInitScreen) LcdClr();//清屏
   else Lcd_cbLogo();		//LOGO显示

   //开启显示，不使用光标系统
   Lcd_UpperWrCommand(DisplaySet(bDispEn)); 			
   Lcd_UnderWrCommand(DisplaySet(bDispEn));

}

/****************************************************************************

							           TGUI显示屏回调函数实现
																 
****************************************************************************/
#include "TGUIBase.h"

//-------------------------更新一片区域字符函数----------------------------
static void _UpdateString(unsigned char x,      //需要更新的行起始位置
                          unsigned char y,      //需要更新的行
                          unsigned char w,      //更新长度
                          const char *pBuf)    //需更新字符
{
  char String;
  //置字符置起始值
	switch(y){
		case 0: Lcd_UpperWrCommand(SetDDRAMAddr(0x80 + x)); break;	  //0x80-0x8b
		case 1: Lcd_UpperWrCommand(SetDDRAMAddr(0x90 + x)); break;    //0x90-0x9b
		case 2: Lcd_UnderWrCommand(SetDDRAMAddr(0x80 + x)); break;	  //0x80-0x8b
		case 3: Lcd_UnderWrCommand(SetDDRAMAddr(0x90 + x)); break;    //0x90-0x9b
		default: return;	//超限不处理
	}
  //送出字符
  if(y < 2)Lcd_UpperWrDataRdy();
  else Lcd_UnderWrDataRdy();
  while(w--){
    String = *pBuf++;
	  if(y < 2) Lcd_UpperWrData(String);
    else Lcd_UnderWrData(String);
  } 
}

//-------------------------更新一片区域标志函数----------------------------
//需更新标志定义为:0 不显示所有标志
//DISP_CFG_ANTI:反显该位置
//其它:暂不处理
static void _UpdateFlag(unsigned char x,      //需要更新的行起始位置
                          unsigned char y,      //需要更新的行
                          unsigned char w,      //更新长度
                          const unsigned char *pFlag)    //需更的标志
{
  char Data;
  unsigned char i,j;
  //启使用扩展模式的画图功能
	if(y < 2){
    Lcd_UpperWrCommand(Extend8B());
    Lcd_UpperWrCommand(ExtendFunSet(bInterfaceCtlBit | bInstructionSetCtl | bGraphicDispCtl));					//扩充功能设定
  }
  else{
    Lcd_UnderWrCommand(Extend8B());
    Lcd_UnderWrCommand(ExtendFunSet(bInterfaceCtlBit | bInstructionSetCtl | bGraphicDispCtl));					//扩充功能设定
  }

  //该行内按每点行更新 
  for(i = 0; i < 16; i++){  //i表示点行
    //设置坐标
    if(y < 2){
  		Lcd_UpperWrCommand(SetGDRAMAddr((y << 4) + i));	//垂直地址
  	  Lcd_UpperWrCommand(SetGDRAMAddr(x));	          //水平地址
      Lcd_UpperWrDataRdy();
    }
    else{
  		Lcd_UnderWrCommand(SetGDRAMAddr(((y - 2) << 4) + i));	//垂直地址
  	  Lcd_UnderWrCommand(SetGDRAMAddr(x));	          //水平地址
      Lcd_UnderWrDataRdy();
    }
    //送一点行内连续数据,检查送什么数
    for(j = 0; j < w; j++){
      Data = *(pFlag + j);
      if(Data & DISP_CFG_ANTI) Data = 0xff;  //反显
      else Data = 0;  //反显
      //送数据 
	    if(y < 2) Lcd_UpperWrData(Data);
      else Lcd_UnderWrData(Data);
    }//end for j;
	}//end for i

  if(y < 2) Lcd_UpperWrCommand(Base8B()); //回到8位并口普通模式
  else Lcd_UnderWrCommand(Base8B());
}

//-------------------------显示屏更新一行内容函数--------------------
void TWinMng_cbUpdateRow(unsigned char Lcd,    //当前操作那个显示屏
                         unsigned char x,      //需要更新的行起始位置
                         unsigned char y,      //需要更新的行
                         unsigned char w,      //更新长度
                         char *pBuf,    //需更新字符
                         //每个字符对应位置标志,见下列定义
                         unsigned char *pFlagBuf)
{
  //将数据分为更新与禁止更新段分别处理
  unsigned char i;
  unsigned char Flag,PrvFlag;
  unsigned char CurW;

  Lcd = Lcd;    //仅有一个显示屏
  //得到更新标志
  if(*pFlagBuf == DISP_CMD_DIS_UPDATE)
    PrvFlag = DISP_CMD_DIS_UPDATE;
  else  PrvFlag = 0;
  CurW = 1;
  for(i = 1; i < w; i++){   //扫描该行
    //=================预处理==================
    switch(*(pFlagBuf + i) & 0xf0){ 
      case DISP_CMD_DIS_UPDATE://禁止更新
        Flag = DISP_CMD_DIS_UPDATE;
        break;
      case DISP_CMD_CLR: //清屏
        *(pBuf + i) = ' ';
        *(pFlagBuf + i) = 0;
      case DISP_CMD_NOR://正常送显
      default:
        Flag = 0;
        break;
	  }
    //=================段处理==================
    if(Flag != PrvFlag){//段切换了,
      if(!PrvFlag){ //不在更新位置更新字符与标志
        PrvFlag =  i - CurW;  //临时复用作起始位置
        _UpdateString(x + PrvFlag, y, CurW, pBuf + PrvFlag);
        _UpdateFlag(x + PrvFlag, y, CurW, pFlagBuf + PrvFlag);
      }
      //else 不更新不处理
      //复位
      PrvFlag = Flag;  
      CurW = 1;//重新开始,已有一个字符了                      
    }
    else CurW++;
  }

  //================更新最后一段===============
  if(!PrvFlag){ //不在更新位置更新字符
    PrvFlag =  w - CurW;  //临时复用作起始位置
	  _UpdateString(x + PrvFlag, y, CurW, pBuf + PrvFlag);
	  _UpdateFlag(x + PrvFlag, y, CurW, pFlagBuf + PrvFlag);
  }
  //else 不更新不处理
}
