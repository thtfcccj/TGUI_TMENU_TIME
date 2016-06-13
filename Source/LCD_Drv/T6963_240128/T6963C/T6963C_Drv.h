//*********************************************************************
//
//			T6963C底层驱动文件
//
//此头文件封装了T6963C所有指令，供LCD驱动程序调用
//*********************************************************************/

#ifndef __T6963C_DRV_H
#define __T6963C_DRV_H

/************************************************************************
						相关回调函数														 
************************************************************************/
//------------------------------写命令------------------------------
void T6963C_cbWrCmd(unsigned char Cmd);
//------------------------------写数据------------------------------
void T6963C_cbWrData(unsigned char Data);

/************************************************************************
					指令集																 
************************************************************************/

//---------------------------状态字操作-----------------------------
//状态字位定义
#define  T6963C_CMD_BUSY		0x01	//指令读写忙
#define  T6963C_DATA_BUSY		0x02	//数据读写忙
#define  T6963C_CMD_AUTO_BUSY	0x04	//指令自动读忙
#define  T6963C_DATA_AUTO_BUSY	0x08	//数据自动读忙
#define  T6963C_RUN_CHECK_EN	0x20	//控制器运行检测可能性1:可能,0:不可能
#define  T6963C_SCREEN_COPY_ERR	0x40	//屏读／拷贝出错状态
#define  T6963C_FLASH_STATE		0x80	//闪烁状态检测1:显示,0:不显示

//-------------------LCD设置光标指针---------------------------
//形参1:水平位置
//形参2:垂直位置
//函数实现方案:
//void T6963_SetCursor(uchar uHor,uchar uVert);
//宏实现方案
#define	T6963_SetCursor(uHor,uVert) \
do{\
	T6963C_cbWrData(uHor);\
	T6963C_cbWrData(uVert);\
	T6963C_cbWrCmd(0x21);\
}while(0)
//-------------------LCD设置CGRAM 偏置地址---------------------------
//形参:偏移地址
//函数实现方案:
//void T6963_SetCGRAMBaseAddr(uchar uBaseAddr);
//宏实现方案:
#define T6963_SetCGRAMBaseAddr(uBaseAddr)\
do{\
	T6963C_cbWrData(uBaseAddr);\
	T6963C_cbWrData(0);\
	T6963C_cbWrCmd(0x22);\
}while(0)
//-------------------LCD设置地址指针---------------------------
//形参:地址指针
#define T6963_SetAddr(uAddr)\
do{\
	T6963C_cbWrData(uAddr & 0x00ff);\
	T6963C_cbWrData(uAddr >> 8);\
	T6963C_cbWrCmd(0x24);\
}while(0)
//-------------------LCD设置文本区首址---------------------------
//形参:文本区首址
#define T6963_SetTextAddr(uAddr)\
do{\
	T6963C_cbWrData(uAddr & 0x00ff);\
	T6963C_cbWrData(uAddr >> 8);\
	T6963C_cbWrCmd(0x40);\
}while(0)
//-------------------LCD设置文本区宽度---------------------------
//形参:文本区宽度（字节数／行）
#define T6963_SetTextWidth(uWidth)\
do{\
	T6963C_cbWrData(uWidth);\
	T6963C_cbWrData(0);\
	T6963C_cbWrCmd(0x41);\
}while(0)
//-------------------LCD设置图形区首址---------------------------
//形参:图形区首址
#define T6963_SetGraphAddr(uAddr)\
do{\
	T6963C_cbWrData(uAddr & 0x00ff);\
	T6963C_cbWrData(uAddr >> 8);\
	T6963C_cbWrCmd(0x42);\
}while(0)
//-------------------LCD设置图形区宽度---------------------------
//形参:图形区宽度（字节数／行）
#define T6963_SetGraphWidth(uWidth)\
do{\
	T6963C_cbWrData(uWidth);\
	T6963C_cbWrData(0);\
	T6963C_cbWrCmd(0x43);\
}while(0)
//-------------------LCD设置合成显示方式---------------------------
//当设置文本方式和图形方式均打开时，合成显示方式设置才有效
//逻辑 “或” 合成
#define T6963_SetDispOrMode() 	T6963C_cbWrCmd(0x80)	
//逻辑 “异或” 合成
#define T6963_SetDispXorMode() 	T6963C_cbWrCmd(0x81)
//逻辑 “与” 合成	
#define T6963_SetDispAndMode() 	T6963C_cbWrCmd(0x83)
//文本特征,按LCD文本特征区的文本特征方式显示
#define T6963_SetDispTextMode T6963C_cbWrCmd(0x84)
//-------------------LCD文本特征方式定义---------------------------
//低4位有效
#define T6963C_FLASH_CTR		0x00		//闪动控制位,与下列方式组合
#define T6963C_TEXT_MODE_POS	0x00		//正向显示
#define T6963C_TEXT_MODE_NEGA	0x05		//负向显示
#define T6963C_TEXT_MODE_DIS	0x03		//禁止显示
//-------------------LCD设置相关显示开关---------------------------
//形参：显示开关
#define T6963C_SetDispSwitch(uSwitch) T6963C_cbWrCmd(0x90 | (uSwitch))	
//显示开关相关位定义为:
#define T6963C_CORSOR_FLASH	0x01	//光标闪动开启控制位
#define T6963C_CORSOR_EN	0x02	//光标显示开启控制位
#define T6963C_TEXT_EN		0x04	//文本显示开启控制位
#define T6963C_GRAPH_EN		0x08	//图形显示开启控制位
//-------------------LCD光标形状选择---------------------------
//形参：光标形状,为8 点（列）×uShape行，uShape 的值为0－7
#define T6963_SetCursorShape(uShape) T6963C_cbWrCmd(0xa0 | (uShape))	
//-------------------LCD数据自动读／写方式设置---------------------------
//开始自动写
#define T6963_StartAutoWr()	 T6963C_cbWrCmd(0xb0)
//开始自动读
#define T6963_StartAutoRd()	 T6963C_cbWrCmd(0xb1)
//结束自动读写
#define T6963_AutoEnd()	 	T6963C_cbWrCmd(0xb3)
//-------------------LCD数据、数据一次读／写方式设置---------------------------
//数据写，地址加1
#define T6963_DataWrInc(Data)  do{T6963C_cbWrData(Data);T6963C_cbWrCmd(0xc0);}while(0)
//数据读，地址加1
#define T6963_DataRdInc(Data)  do{T6963C_cbWrData(Data);T6963C_cbWrCmd(0xc1);}while(0)	 	T6963C_cbWrCmd(0xc1)
//数据写，地址减1
#define T6963_DataWrDec(Data)  do{T6963C_cbWrData(Data);T6963C_cbWrCmd(0xc2);}while(0)
//数据读，地址减1
#define T6963_DataRdDec(Data)  do{T6963C_cbWrData(Data);T6963C_cbWrCmd(0xc3);}while(0)
//数据写，地址不变
#define T6963_DataWrKeep(Data)  do{T6963C_cbWrData(Data);T6963C_cbWrCmd(0xc4);}while(0)
//数据读，地址不变
#define T6963_DataRdKeep(Data)  do{T6963C_cbWrData(Data);T6963C_cbWrCmd(0xc5);}while(0)
//-------------------LCD开始读屏-----------------------------
#define T6963_StartRdScr()	 	T6963C_cbWrCmd(0xe0)
//-------------------LCD开始屏拷贝---------------------------
#define T6963_StartScrCopy()	 T6963C_cbWrCmd(0xe8)
//-------------------LCD显示缓冲区位操作---------------------------
//形参1：指出需操作的位:第3位＝0 清零;第3位＝1 置位。0-2位:操作位对应单元的D0－D7 位。
#define T6963_DispBurSetBit(uBit)	 T6963C_cbWrCmd(0xf8 | uBit)


#endif

