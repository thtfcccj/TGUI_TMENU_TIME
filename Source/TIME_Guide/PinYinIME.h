/*
           pyInput.c - 拼音输入法模块

           原作:张 凯
           改写:李 强(mail2li@21cn.com)
           重改写：dap_ccj
*/

#ifndef __PINYIN_IME_H 
#define __PINYIN_IME_H

struct _PyIndex
{
    const unsigned char *Py;      //拼音拼写
    const unsigned char *Pymb;    //对应字码
};

//数字键拼音输入法结构
typedef struct _PinYinIME{
  unsigned char NumCount;             //数值输入次数计数,索引值需减1
  unsigned char CurPos;              //当前缓冲位置
  unsigned char PerFindCount[6];      //每次已输入数字个数,拼音最多6个字母
  const struct _PyIndex *pPyTblIndex[24];      //缓冲找到的字符串及其位置
}PinYinIME_t;

//------------------------------按键响应程序--------------------------------
//按键时调用此函数
//返回找到的字母表
//若返回空,表示此次没找到
//注:这里只响应键值为: ' ':退格键 '2'-'9'数字键,其它键不响应
const struct _PyIndex **NumKeyPyIme_pKey(PinYinIME_t *pPy,
                                         unsigned char *pFindCount,//返回找到的过数      
                                         unsigned char Num);   //输入的按键值,响应2-9,退格键

//------------------------------得到当前字母表--------------------------------
//返回找到的字母表
const struct _PyIndex **NumKeyPyIme_pGetCur(const PinYinIME_t *pPy);

//------------------------------得到当前字母表总数--------------------------------
unsigned char NumKeyPyIme_GetCurCount(const PinYinIME_t *pPy);


#endif

