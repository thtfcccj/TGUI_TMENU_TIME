//*********************************************************************
//
//			ST7920底层驱动文件
//
//此头文件封装了ST7920所有指令，供LCD驱动程序调用
//*********************************************************************/

#ifndef __ST7920_DRV_H
#define __ST7920_DRV_H


#define  BF_BIT  0x80      	   //LCD处于忙状态标志

//#define LCD_LINE_LENGTH  12		//LCD单行显示汉字的最大长度
#define LCD_LINE_LENGTH  8		//LCD单行显示汉字的最大长度

/*----------------------------------------------------------

						基本指令集

-----------------------------------------------------------*/

//-------------------清除显示指令---------------------------
#define ClearDisplay()	0x01

//-------------------DDRAM地址复位指令---------------------------
#define AddressRest		0x02   //光标回原点

//-------------------进入设定点指令---------------------------
//指定在数据的读取与写入时，设定游标的移动方向及指定显示的移位
#define EntryModeSet(uMode) (0x04 | (uMode))

//uMode定义为
#define bDisplayLeftMove		0x01	//整体左移
#define bDisplayRightMove		0x03	//整体右移
#define bAddNoMove			0x02	//光标增加，整体不移动
#define bSubNoMove			0x00	//光标减少，整体不移动
//-------------------显示开关设置---------------------------
#define DisplaySet(uSwitch) (0x08 | (uSwitch))

//uSwitch定义为
#define bCursorBlinkEn	0x01	//光标位置反白且闪烁控制位
#define bCursorDispEn		0x02	//光标显示开启控制位
#define bDispEn			0x04	//整体开启控制位
//-------------------光标或显示移位控制---------------------------
//光标左移
#define CursorLeft() 	0x10
//整体左移，光标跟随移动
#define DiplayLeft() 	0x18
//光标右移
#define CursorRight() 	0x14
//整体右移，光标跟随移动
#define DiplayRight() 	0x1c

//-------------------功能设置---------------------------
//在状态切换时，不要同时更改位数和指令集类型，必须先更改位数，然后更改指令集类型
//4位控制接口，基本指令集
#define Base4B() 	0x20
//8位控制接口，基本指令集
#define Base8B() 	0x30
//4位控制接口，扩充指令集
#define Extend4B() 	0x24
//8位控制接口，扩充指令集
#define Extend8B()	0x34

//-------------------设定CGRAM地址---------------------------
//需确定扩充指令中SR=0
//形参：CGRAM地址
#define SetCGRAMAddr(uCG_ADDR) ((0x40)| (uCG_ADDR))

//-------------------设定DDRAM地址---------------------------
//形参：DDRAM地址
#define SetDDRAMAddr(uDD_ADDR) ((0x80)|(uDD_ADDR))


//----------------------扩展指令集--------------------------------


//-------------------待命模式---------------------------
//待命模式不更改RAM的内容
#define StandBy() 	0x01

//-------------------卷动位置或RAM地址选择---------------------------
//允许输入垂直卷动地址
#define VerticalScroll		0x03
//允许输入IRAM及CGRAM地址
#define RamAddrSelect		0x02

//-------------------反白显示---------------------------
//反白功能无法实现？？？？？？？？？？？？？？？？
//反白第一行
#define ReserveFirst		0x04
//反白第二行
#define ReserveSecond		0x05
//反白第三行
#define ReserveThird		0x06
//反白第四行
#define ReserveFourth		0x07

//-------------------睡眠模式---------------------------
//进入睡眠模式
#define IntoSleepMode		0x08
//脱离睡眠模式
#define OutSleepMode		0x0c

/*----------------------------------------------------------

						扩充指令集

-----------------------------------------------------------*/
//同一指令的动作不能同时改变RE 及DL、G，需先改变DL 或G 再改变RE 才能确保设置正确
#define ExtendFunSet(uFun) (0x20 | (uFun))

//uFun定义为
#define bInterfaceCtlBit			0x10	//8/4 位接口控制位.DL=1,8 位MPU 接口；DL=0,4 位MPU 接口	  
#define bInstructionSetCtl		0x04	//指令集选择控制位.	RE＝1，扩充指令集；RE＝0，基本指令集
#define bGraphicDispCtl			0x02	//绘图显示控制位.G＝1，绘图显示开；G＝0，绘图显示关

//-------------------设定IRAM 地址或卷动地址---------------------------
//形参：IRAM 地址或卷动地址
//SR=1,A5～A0 为垂直卷动地址；SR=0,A3～A0 为IRAM 地址
#define SetIRAMAddr(uIRAM_ADDR) ((0x40)| (uIRAM_ADDR))

//-------------------设定绘图RAM 地址---------------------------
//形参：GDRAM地址
//垂直地址范围:AC6～AC0,水平地址范围:AC3～AC0
//连续写入2 字节数据来完成垂直与水平坐标的设置
#define SetGDRAMAddr(uGD_ADDR) ((0x80)|(uGD_ADDR))

#endif