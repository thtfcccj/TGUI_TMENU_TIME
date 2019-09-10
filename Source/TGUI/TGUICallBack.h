/*******************************************************************************

                           TGUI 回调函数接口
此文件声明的接口已被TGUI内核调用，但实现可能与应用相关，需在外部实现。
******************************************************************************/
#ifndef __T_GUI_CALL_BACK_H 
#define __T_GUI_CALL_BACK_H

/**********************************************************************
                         需实现的常量或变量
**********************************************************************/
//以下为加速运行使用的常量数组,当单片机不支持常量表时,可直接定义到数据区：

//字符串型移位查找表(当显示屏高度<=8时才定义)
extern const unsigned char  U8ShiftLUT[8];
//半字型移位查找表(当显示屏高度>8时才定义)
extern const unsigned short U16ShiftLUT[16]; 	
//全字型移位查找表(一般不做定义)
extern const unsigned long  U32ShiftLUT[32]; 	

//----------------------TGUI所需要的临时字符串缓冲-----------------------
extern char *TGUI_pGetBuf(void);

//----------------------得到常量缓存结构函数-----------------------
//因某些单片机不支持code区指针,故在内核已避免了code区指针问题,
//内核需常量时,使用下列常量临时缓冲存常量数据 >= 3;
extern char *TGUI_pGetConstBuf(void); 

/**********************************************************************
                         回调函数
**********************************************************************/

//----------------------得到保存按钮字-----------------------
//如[保  存]
const char *cbGetSaveBottom(void); 

//----------------------得到返回按钮字-----------------------
//如[返  回]
const char *cbGetReturnBottom(void); 

//-----------------------得到两按钮之间的间隔--------------------
//根据显示屏大小与当前项窗口宽度得到按钮之间的间隔
unsigned char cbGetBottomSpace(TItemSize_t ItemW);

//-------------------------显示屏更新一行内容函数--------------------
void TWinMng_cbUpdateRow(unsigned char Lcd,    //当前操作那个显示屏
                         unsigned char x,      //需要更新的行起始位置
                         unsigned char y,      //需要更新的行
                         unsigned char w,      //更新长度
                         const char *pBuf,    //需更新字符
                         //每个字符对应位置标志,见下列定义
                         const unsigned char *pFlagBuf);

//其中:每个字符对应位置标志定义为:
//高4位为命令,低4位根据命令字决定的配置字，
#define    DISP_CMD_MASK        (0xf << 4)//分别定义为:

//1:普通更新标志,将当前字符内容根据配置更新:
#define    DISP_CMD_NOR        (0 << 4) //配置定义为:
  #define    DISP_CFG_ANTI       0x01  //该位反显1/禁止反显0标志(固定定义为0x01，底层必须支持)
  #define    DISP_CFG_UP_LINE    0x02  //有上横线标志,应用层可选功能(暂不支持)
  #define    DISP_CFG_DOWN_LINE  0x04  //有下横线标志,应用层可选功能(暂不支持)

//2:禁止更新标志, 显示区域被TGUI外使用时，应实现此指令
#define    DISP_CMD_DIS_UPDATE  (1 << 4) //无配置  

//3:清屏标志, 清除该位置所有内容， 显示区域被TGUI外使用时，应实现此指令
#define    DISP_CMD_CLR        (2 << 4) //无配置



#endif //#ifndef __T_GUI_CALL_BACK_H 

