/*****************************************************************************
                         TGUI上的输入法模块拼音输入法部分
此模块继承PinYinIME模块完成拼音输入法的输入与显示
*****************************************************************************/
#ifndef __TIME_PINYIN_H
#define __TIME_PINYIN_H

/*****************************************************************************
                              说明
*****************************************************************************/
//内部显示见TIme_ReadMe.h拼音输入法部分
//函数调用流程说明:
//1:切换到拼音输入法或上个汉字输入完成后，调用初始化函数进行初始化操作
//2:上层进行模块内部字符显示,在每次按键后刷新:
//  (1)调用得到提示状态函数得到状态值后由上层显示提信息如:"拼音->拼音选择"
//  (2)调用得到拼音提示函数填充拼音行
//  (3)调用得到字符提示函数填充字符行
//3:用户按键时，调用按键键响应函数,根据该函数返回值刷新字符显示。
//4:得到输入字符串,在上步返回值中，若返回已获得字符时,获得字符
//5:获得字符后，应重新从第一步开始执行

/*****************************************************************************
                        相关结构
*****************************************************************************/
#include "PinYinIME.h" //PinYinIME_t;

//状态机
enum _eTImePinYin{
  //空闲状态，即未输出任何拼音状态，应提示"拼音      "
  eTImePinYin_Idie = 0,
  //拼音输入状态，即正在输入拼音字母，应提示"拼音->拼音输入"
  eTImePinYin_Input = 1,
  //拼音选择状态，即在输入拼音字母确认后选择那个拼音，应提示"拼音->拼音选择"
  eTImePinYin_PinYinSel = 2,  
  //汉字选择状态，即在输入拼音字母确认后选择那个拼音，应提示"拼音->汉字选择"
  eTImePinYin_ChSel = 3,
  //汉字选择完成状态,等待用户调用显示函数后,自动复位到空闲状态
  eTImePinYin_Final = 4,
};

struct _TImePinYin{
  PinYinIME_t Ime;           //继承的结构
  unsigned char DispW;           //显示宽度
  unsigned char PinYinStart;  //拼音显示起始位置,多个拼音显示不全时用于多页显示
  unsigned char ChStart;      //汉字显示起始位置,半字为单位
  unsigned char CurPinYin;    //当前选择的拼音字索引位置
  unsigned char CurCh;        //当前选择的汉字索引位置,半字为单位

  unsigned char CurPage;       //拼音选择时当前页位置
  unsigned char CurPinYingCount;//当前拼音数量
  unsigned char MaxPerPage[9]; //拼音选择时每页最大数量:从1开始
  
  
  enum _eTImePinYin eState;  //状态机
};

/*****************************************************************************
                        相关函数
*****************************************************************************/

//---------------------------初始化函数---------------------------------
//切换到拼音输入法或上个汉字输入完成后调用
void TImePinYin_Init(struct _TImePinYin *pPinYin,//输入法结构
                     unsigned char DispW);        //显示宽度,>16

//----------------------填充拼音显示行字符串函数---------------------------
//填充示例:"1pen 2ren 3sen", 返回填充数量
unsigned char TImePinYin_GetPinYinChar(struct _TImePinYin *pPinYin,
                                         char *pBuf);              //被填充的字符

//----------------------填充汉字显示行字符串函数---------------------------
//填充示例:"人1 仁2 壬3 忍4", 返回填充数量
unsigned char TImePinYin_GetChChar(struct _TImePinYin *pPinYin,
                                     char *pBuf);              //被填充的字符

//----------------------得到当前用户选择字符函数---------------------------
//在输入完成状态后才能调用，调用后内部状态机自动复位到空闲状态
//返回汉字前两位位置
const char *TImePinYin_pGetChar(struct _TImePinYin *pPinYin);

//-----------------------按键响应函数---------------------------------
//当用户按键时响应用户请求
//返回当前状态
enum _eTImePinYin TImePinYin_eKey(struct _TImePinYin *pPinYin,
                                 char KeyNum);//键值

//----------------------得到当前输入状态函数---------------------------------
//返回当前输入状状态
//enum _eTImePinYin TImePinYin_eGetState(const struct _TImePinYin *pPinYin);
#define TImePinYin_eGetState(pPinYin) ((pPinYin)->eState)

#endif //#define __TIME_PINYIN_H

