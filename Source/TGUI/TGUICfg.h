/* ----------------------------------------------------------------------------
 *                TGUI用户配置模块
 * --------------------------------------------------------------------------*/

#ifndef __T_GUI_CFG_H 
#define __T_GUI_CFG_H


/**********************************************************************
                     可移植选项
**********************************************************************/

//1:因很多单片机常量读写数据受限,故TGUI系统不使用常量区数据
//2:因部分单片机指针函数不能或无法正常使用,故指针函数的使用将受限


/**********************************************************************
                      全局配置区
**********************************************************************/

//定义是否双字节对齐,部分LCD要求双字对齐才能正确显示数
//#define       TGUI_WORD_ALIGN

//显示屏宽字符数,8个点一个高度,<=128
//注：没有定义双字节对齐功能时,应值应+1用于支持全角时可显示半个字符
#define       TLCD_WIDTH      31    //显示屏宽字符数,8个点一个高度,<=128
//显示屏高字符数,16个点一个高度,<=128
#define       TLCD_HIGH       8    

//定义左右键水平移动窗口的大小，字节为单位
#define       TGUI_HORI       6  


//定义是否禁止函数指针传递机制,因很多单片机或编译器对函数指针支持
//不够(如:keil C51)或根本不支持,禁止后,TGUI将采用另类方式实现回调
//#define       TGUI_DIS_P_FUN

//定义控件里面的项目占位,当有控件字符>255或项>255时,应定义为unsigned short
//否则定义为unsigned char
typedef unsigned short TItemSize_t;
#define	TITEM_SIZE_T    2        //定义占位

//定义TGUI类型占位
#define	TU16	unsigned short
#define	TS16	signed short
#define	TU32	unsigned long
#define	TS32	signed long

/**********************************************************************
                      控件功能配置区
**********************************************************************/
//可禁止系统重未使用的控件以节省空间:
//禁止后,未使用的控件不在项目里包含相应C文件即可
//可禁止的控件有:
#define   TGUI_DIS_TEXT    	   //禁止文本框
//#define   TGUI_DIS_CHECKBOX        //禁止复选框
//#define   TGUI_DIS_RADIO_BUTTON    //禁止单选框
//#define   TGUI_DIS_EDIT    	   //禁止编辑框


//定义Edit控件支持的可调整数类型,未使用置0,使用置非0
#define      TGUI_EDIT_FLOAT_SUPPORT	1 //浮点数支持,最多显示位数7位
//整形数支持:选择最高支持位数		
#define      TGUI_EDIT_INT8_SUPPORT 	1 //8位数支持,调整范围+-127
#define      TGUI_EDIT_INT16_SUPPORT	1 //16位数支持,调整范围+-32768
#define      TGUI_EDIT_INT32_SUPPORT	1 //32位数支持,调整范围+-9999999

#define      TGUI_EDIT_HEX_SUPPORT	    0 //十六进制数调整支持,支持位数内整形数支持确定
#define      TGUI_EDIT_BIN_SUPPORT	    0 //二进制数支持,支持位数内整形数支持确定 


//定义使用的复选框最大项数
#define   TCHECKBOX_ITEM_COUNT      32

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
//高4位为命令,低4位根据命令字决定的配置字，分别定义为:
#define    DISP_CMD_MASK        (0xf << 4)

//1:普通更新标志,将当前字符内容根据配置更新:
#define    DISP_CMD_NOR        (0 << 4) //配置定义为:
#define    DISP_CFG_ANTI       0x01  //该位反显1/禁止反显0标志
#define    DISP_CFG_UP_LINE    0x02  //有上横线标志,可选，需LCD支持
#define    DISP_CFG_DOWN_LINE  0x04  //有下横线标志,可选，需LCD支持

//2:禁止更新标志
#define    DISP_CMD_DIS_UPDATE  (1 << 4) //无配置  

//3:清屏标志,清除该位置所有内容
#define    DISP_CMD_CLR        (2 << 4) //无配置

/**********************************************************************
                         内部使用
**********************************************************************/

//当使用Edit控件数值类型时,定义所使用数值的最大数据类型(禁止为浮点数类型)

//当使用浮点数时,应等于浮点数占位
#if TGUI_EDIT_FLOAT_SUPPORT || TGUI_EDIT_INT32_SUPPORT
  #define TSIGNED_SIZE	4     //定义占位
  typedef TS32 TSigned_t;     //有符号数
  typedef TU32 TUnSigned_t;   //无符号数
#elif TGUI_EDIT_INT16_SUPPORT
  #define TSIGNED_SIZE	2     //定义占位
  typedef TS16 TSigned_t;     //有符号数
  typedef TU16 TUnSigned_t;   //无符号数
#else
  #define TSIGNED_SIZE	1     //定义占位
  typedef signed char   TSigned_t;     //有符号数
  typedef unsigned char TUnSigned_t;   //无符号数
#endif 

#endif

