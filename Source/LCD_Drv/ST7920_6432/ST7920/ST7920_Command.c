//*********************************************************************
//
//			ST7920底层驱动命令实现
//
//*********************************************************************/
				 
#include "ST7920_Command.h"

//-------------------底层IO口初始化------------------------
//由用户初始化其IO口到默认状态
void Lcd_HwInit(void)
{
	//用到的IO口：
	//并口->P4 
	//E1->PC2
	//E2->PC3
	//RS->PD2
	//RW->PD1

	//并口预置为输入状态
	Lcd_Set_In();

	clear_bit(LCD_CTRLPORT,LCD_E);
	clear_bit(LCD_CTRLPORT,LCD_RS);
	clear_bit(LCD_CTRLPORT,LCD_R_W);
	set_bit(LCD_CTRLPORT,LCD_RST);
	//clear_bit(LCD_CTRLPORT,LCD_RST);

	//控制端口全部置为输出低电平
	set_bit(LCD_CTRLDDR,LCD_RS);
	set_bit(LCD_CTRLDDR,LCD_R_W);
	set_bit(LCD_CTRLDDR,LCD_E);
	set_bit(LCD_CTRLDDR,LCD_RST);
	//vDelayMs(10);
	//set_bit(LCD_CTRLPORT,LCD_RST);
}


//-------------------等待空闲---------------------------
//可返回当前选择位置值

//等待上半屏空闲
static void Lcd_WaitUpperIdie(void)
{
	unsigned char Data;
	Lcd_RD_Select();	//读选择
	Lcd_I_Select();		//指令
	Lcd_Set_In();			//总线为输入
	do{
	 	Lcd_OnUpperScreen();	//E使能(选通)
		Lcd_RdDelay();
	 	Data = Lcd_Get_Data(); 
		Lcd_OffUpperScreen();
	}while(Data & BF_BIT);
} 

/*/等待下半屏空闲
static void Lcd_WaitUnderIdie(void)
{
	unsigned char Data;
 	Lcd_RD_Select();
	Lcd_I_Select();
	Lcd_Set_In();
	do{
	 	Lcd_OnUnderScreen();
		Lcd_RdDelay();
	 	Data = Lcd_Get_Data();  
		Lcd_OffUnderScreen();
	}while(Data & BF_BIT);
}*/



//-------------------LCD写指令---------------------------		
//上半屏写指令
void Lcd_UpperWrCommand(unsigned char Command)
{
	Lcd_WaitUpperIdie();
 	Lcd_WR_Select();
	Lcd_I_Select();
	Lcd_Set_Out();						

	Lcd_Set_Data(Command);
	Lcd_OnUpperScreen();
	Lcd_WrDelay();	  //延时等待数据写入
	Lcd_OffUpperScreen();
}

/*/下半屏写指令
void Lcd_UnderWrCommand(unsigned char Command)
{			
	Lcd_WaitUnderIdie();
 	Lcd_WR_Select();
	Lcd_I_Select();	
	Lcd_Set_Out();						

	Lcd_Set_Data(Command);
	Lcd_OnUnderScreen();
	Lcd_WrDelay();	 //延时等待数据写入
	Lcd_OffUnderScreen();
}*/

//-------------------LCD写数据准备---------------------------
//上半屏写数据前调用此函数
void Lcd_UpperWrDataRdy(void) 
{
	Lcd_WaitUpperIdie();
	Lcd_D_Select();	
 	Lcd_WR_Select();
	Lcd_Set_Out();						
}
/*/下半屏写数据前调用此函数
void Lcd_UnderWrDataRdy(void) 
{
	Lcd_WaitUnderIdie();
	Lcd_D_Select();	
 	Lcd_WR_Select();
	Lcd_Set_Out();						
}*/

//-------------------LCD写数据---------------------------
//写单个或连续数据,写第一个数据前必须调用写准备函数

//上半屏写数据
void Lcd_UpperWrData(unsigned char uData)
{
	Lcd_WrDelay();	  //延时等待数据写入//是否应该为Lcd_UpperWrDataRdy()?否！可能会连续写2字节

	Lcd_Set_Data(uData);
	Lcd_OnUpperScreen();
	Lcd_WrDelay();	  //延时等待数据写入
	Lcd_OffUpperScreen();	
}

/*/下半屏写数据
void Lcd_UnderWrData(uData) 
{
	Lcd_Set_Data(uData);
	Lcd_OnUnderScreen();
	Lcd_WrDelay();	 //延时等待数据写入
	Lcd_OffUnderScreen();
}*/


