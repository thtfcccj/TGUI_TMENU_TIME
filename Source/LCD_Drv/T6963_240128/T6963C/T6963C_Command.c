/*******************************************************************************

			                  T6963C底层驱动命令实现

*******************************************************************************/
#include "T6963C_Command.h"

//--------------------------底层IO口初始化-------------------------------
/*/由用户初始化其IO口到默认状态
void T6963C_HwInit(void)
{

  
  
  
}*/

//--------------------------等待空闲------------------------------------
__arm static void _WaitIdie(unsigned char WaitType)
{
  unsigned char Data;
  T6963C_SelCmd();		    //选择指令
  T6963C_SetIn();			//总线为输入  
  //T6963C_CsEn();	        //使能(选通)
  for(unsigned char i = 10; i > 0; i--){
    T6963C_ClrRd();	        //读选择
    T6963C_RdDelay();
    Data = T6963C_GetData();
    T6963C_SetRd();         //读取消
    if((Data & WaitType) != WaitType) break;
  };
  //T6963C_CsDis();         //取消片选  
} 

//------------------------------写命令------------------------------
__arm void T6963C_cbWrCmd(unsigned char Cmd)
{
  T6963C_CsEn();    
  _WaitIdie(T6963C_CMD_BUSY);//等待指令空闲
  //T6963C_SelCmd();         //选择指令,(已为指令状态)
  T6963C_SetOut();           //置为输出 
  T6963C_SetData(Cmd);       //置数据  
  T6963C_ClrWr();            //写数据状态
  T6963C_WrDelay();	         //延时等待数据写入
  T6963C_SetWr();            //取消写数据状态  
  T6963C_CsDis();
}

//--------------------------顺列写数据----------------------------------
//调用T6963_StartAutoWr()可调用此函数连续写数据
__arm void T6963C_SqureWrData(const unsigned char *pData,
                        unsigned short Len) 
{
  T6963C_CsEn();    
  _WaitIdie(T6963C_CMD_BUSY);//等待指令空闲
  T6963C_SelData();         //写数据方式  
  T6963C_SetOut();           //置为输出
  while(Len--){              //写指定数据长度
    T6963C_SetData(*pData++);   //置数据
    T6963C_ClrWr();            //写数据状态
    T6963C_WrDelay();	      //延时等待数据写入
    T6963C_SetWr();            //取消写数据状态
  }
  T6963C_CsDis();
}

//---------------------------------写数据----------------------------------
__arm void T6963C_cbWrData(unsigned char Data)
{
  T6963C_CsEn();    
  _WaitIdie(T6963C_DATA_BUSY);//等待数据空闲
  T6963C_SelData();         //写数据方式
  T6963C_SetOut();           //置为输出   
  T6963C_SetData(Data);     //置数据
  T6963C_ClrWr();            //写数据状态
  T6963C_WrDelay();	      //延时等待数据写入
  T6963C_SetWr();            //取消写数据状态
  T6963C_CsDis();
}

