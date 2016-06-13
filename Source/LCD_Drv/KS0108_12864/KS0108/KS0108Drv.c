/******************************************************************************

                          KS0108底层驱动程序实现

*******************************************************************************/

#include "KS0108Drv.h"


/******************************************************************************
                            相关函数实现
*******************************************************************************/

//-----------------------------读状态函数----------------------------
//返回读出的状态码
unsigned char Ks0108Drv_RdState(void)
{
  unsigned char Data;
  //1.读数据前信号准备
  //Ks0108Drv_ClrE();  //先置为低电平,为E上升沿锁地址做准备->已在低电平状态
  cbKs0108Drv_ClrRS(); //指令操作
  //cbKs0108Drv_InDB();  //总线置输入态->函数调用前已在输入状态
  //2.为读数据写地址信息
  cbKs0108Drv_SetRW(); //高电平进入读状态
  cbKs0108Drv_Tasu();  //LCD地址建立延时
  cbKs0108Drv_SetE(); //LCD地址建立完成
  //3.读出数据,上升沿LCD输出数据到MCU
  cbKs0108Drv_Td();   //读延时
  Data = cbKs0108Drv_GetDB();//读取数据
  //4.结束释放总线
  cbKs0108Drv_ClrE();  //低电平LCD释放总线
  cbKs0108Drv_ClrRW(); //退出读状态,LCD释放总线

  return Data;
}

//-----------------------------写状态函数----------------------------
void Ks0108Drv_WrState(unsigned char Data)
{
  unsigned char Busy;
  unsigned char Count = cbKs0108Drv_GetBusyCount();
  //1.1. 读忙标志前信号准备
  //Ks0108Drv_ClrE();  //先置为低电平,为E上升沿锁地址做准备->已在低电平状态
  cbKs0108Drv_ClrRS(); //指令操作
  //cbKs0108Drv_InDB();  //总线置输入态->函数调用前已在输入状态
  //1.2.为读取忙标志写地址信息
  cbKs0108Drv_SetRW(); //高电平进入读状态
  cbKs0108Drv_Tasu();  //LCD地址建立延时
  //cbKs0108Drv_SetE(); //LCD地址建立完成->读忙标志会使用
  //1.3.读取忙标志
  for(; Count > 0; Count--){//忙判断
    cbKs0108Drv_SetE(); //上升沿LCD输出数据到MCU
    cbKs0108Drv_Td();   //读延时
    Busy = cbKs0108Drv_GetDB();//读取数据
    cbKs0108Drv_ClrE();  //为重新读忙标志做准备
    if(!(Busy & 0x80)) break; //低电平不在忙状态
  }
  //2.1.写数据前信号准备
  cbKs0108Drv_SetDB(); //数据先到总线
  cbKs0108Drv_OutDB();  //总线置输出态,数据出
  //cbKs0108Drv_ClrRS(); //指令操作->已在指令状态
  //2.2为写数据写地址信息
  cbKs0108Drv_ClrRW(); //低电平进入写状态
  cbKs0108Drv_Tasu();  //LCD地址建立延时
  cbKs0108Drv_SetE();  //LCD地址建立完成
  //2.3LCD写数据,下降沿LCD锁存数据
  cbKs0108Drv_Tdsu();   //等待LCD数据建立
  cbKs0108Drv_ClrE();   //下行沿LCD锁存数据
  cbKs0108Drv_Tdhr(); //写延时
  cbKs0108Drv_SetRW(); //退出写状态
  //2.4结束释放总线
  cbKs0108Drv_InDB();  //主动释放总线
}

//-----------------------------读数据函数----------------------------
//返回读出的数据
unsigned char Ks0108Drv_RdData(void)
{
  unsigned char Data;
  //1.读数据前信号准备
  //Ks0108Drv_ClrE();  //先置为低电平,为E上升沿锁地址做准备->已在低电平状态
  cbKs0108Drv_SetRS(); //数据操作
  //cbKs0108Drv_InDB();  //总线置输入态->函数调用前已在输入状态
  //2.为读数据写地址信息
  cbKs0108Drv_SetRW(); //高电平进入读状态
  cbKs0108Drv_Tasu();  //LCD地址建立延时
  cbKs0108Drv_SetE(); //LCD地址建立完成
  //3.读出数据,上升沿LCD输出数据到MCU
  cbKs0108Drv_Td();   //读延时
  Data = cbKs0108Drv_GetDB();//读取数据
  //4.结束释放总线
  cbKs0108Drv_ClrE();  //低电平LCD释放总线
  cbKs0108Drv_ClrRW(); //退出读状态,LCD释放总线

  return Data;
}

//-----------------------------写数据函数----------------------------
void Ks0108Drv_WrData(unsigned char Data)
{
  unsigned char Busy;
  unsigned char Count = cbKs0108Drv_GetBusyCount();
  //1.1. 读忙标志前信号准备
  //Ks0108Drv_ClrE();  //先置为低电平,为E上升沿锁地址做准备->已在低电平状态
  cbKs0108Drv_ClrRS(); //指令操作
  //cbKs0108Drv_InDB();  //总线置输入态->函数调用前已在输入状态
  //1.2.为读取忙标志写地址信息
  cbKs0108Drv_SetRW(); //高电平进入读状态
  cbKs0108Drv_Tasu();  //LCD地址建立延时
  //cbKs0108Drv_SetE(); //LCD地址建立完成->1.3读忙标志会使用
  //1.3.读取忙标志
  for(; Count > 0; Count--){//忙判断
    cbKs0108Drv_SetE(); //上升沿LCD输出数据到MCU
    cbKs0108Drv_Td();   //读延时
    Busy = cbKs0108Drv_GetDB();//读取数据
    cbKs0108Drv_ClrE();  //为重新读忙标志做准备
    if(!(Busy & 0x80)) break; //低电平不在忙状态
  }
  //2.1.写数据前信号准备
  cbKs0108Drv_SetDB(); //数据先到总线
  cbKs0108Drv_OutDB();  //总线置输出态,数据出
  cbKs0108Drv_SetRS(); //数据操作
  //2.2为写数据写地址信息
  cbKs0108Drv_ClrRW(); //低电平进入写状态
  cbKs0108Drv_Tasu();  //LCD地址建立延时
  cbKs0108Drv_SetE();  //LCD地址建立完成
  //2.3LCD写数据,下降沿LCD锁存数据
  cbKs0108Drv_Tdsu();   //等待LCD数据建立
  cbKs0108Drv_ClrE();   //下行沿LCD锁存数据
  cbKs0108Drv_Tdhr(); //写延时
  cbKs0108Drv_SetRW(); //退出写状态
  //2.4结束释放总线
  cbKs0108Drv_InDB();  //主动释放总线
}


