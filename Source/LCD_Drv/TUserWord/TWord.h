/* ----------------------------------------------------------------------------
 *                用户自定义字模－实现
 *此模块适用于不带字库的LCD显示屏，用自定义字时实现
 * --------------------------------------------------------------------------*/

#ifndef __T_WORD_H
#define __T_WORD_H

/****************************************************************************
                       内部字模及符号声明
****************************************************************************/

//-------------------------ASCII字符字模------------------------------
//定义ASC字符是否含控制符号位置字模,允许时,前0-31可用作用户自定义符号
//#define TWORD_ASC_EN_CTRL   

//此字模与ASCII码值呈对应关系
//ASCII码0-31为控制码，可禁止控制码以节省空间
extern const unsigned char TWord_AscLUT[];   //在TWord_Asc.c中实现

//-------------用户自定义全角字符-GB2312查找表(顺序排序)------------------
//此查找表以GB2312为基础接先后顺序排列,每两个字当成一个字符
//其中,带"★"标识符的为系统保留字符
//附GB2312字符查找表
extern const char TWord_GB2312LUT[];     //在TWord_GB2312.c中实现
unsigned short TWord_GetGB2312LUTSize(void); //得到其大小

//-------------用户自定义字符GB2312字模(以TWord_GB2312LUT对应)------------
//此字模为TWord_GB2312.c中的TWord_GB2312LUT位置对应的宋体16X16点阵字模
//此查找表以GB2312为基础接先后顺序排列,每两个字当成一个字符
//其中,带"★"标识符的为系统保留字符
//附GB2312字符查找表
extern const unsigned char TWord_GB2312SongLUT[]; //在TWord_GB2312Song.c中实现


/****************************************************************************
                      对外变量及函数声明
****************************************************************************/

//------------------------得到Ascii码字模函数--------------------------
//返回该字字模起始位置,前16个有效
//const unsigned char *TWord_pGetAsciiModule(char Asc);//Asc必须<127
//直接宏实现:
#ifdef TWORD_ASC_EN_CTRL
  #define TWord_pGetAsciiModule(Asc) (&TWord_AscLUT[(Asc) << 4])
#else
  #define TWord_pGetAsciiModule(Asc) (&TWord_AscLUT[(Asc - 32) << 4])
#endif

//-----------得到将GB2312标准区位码转换为自定义索引值函数----------------
//此函数需配合用户自定义字模相关操作函数才能得到字模
//返回0xffff表示未找到
unsigned short TWord_Word2UserID(unsigned short Word,//前半字与后半字必须>=128
                                 const char *pLUT,    //该字对应的自定义字符
                                 unsigned short Count);//自定义字符大小

//-----------------由自定义索引值得到GB2312字模函数--------------------------
//形参必须是由TWord_Word2UserID得到的自定义索引值
//返回该字字模起始位置,前32个有效
//const unsigned char *TWord_pGetWordModule(unsigned short UserID);
//直接宏实现:
#define TWord_pGetWordModule(UserID) (&TWord_GB2312SongLUT[(UserID) << 5])
    
    
#endif

