/* ----------------------------------------------------------------------------
 *                以TWidget为基类的应用相关共享函数
 * --------------------------------------------------------------------------*/

#ifndef __T_WIDGET_SHARE_H 
#define __T_WIDGET_SHARE_H

#include "TWidget.h"

//-------------附加最多2项的反显回调实现函数---------------------
//以TWidget_t为基类的控件中,管理附加项如:"   [保  存]    [返 回] "的焦点显示
void Append2_AutiNotify(TWidget_t *pWidget,//基类
                           //0x80:在附加项里标志,0x01:带保存标志,0x02:带返回标志
                           unsigned char Flag,
                           unsigned char InReturn,//两个按钮时焦点在返回上标志
                           unsigned char Len1st,  //首字长度
                           unsigned char Len2nd);  //次字长度


//----------------可能附带最多两个附加项的得到字符函数实现--------------------------
//以TWidget_t为基类的,最后一项可能有最多两具焦点的高级控件可调用此函数进行管理
//返回焦点在那个按钮上
const char* Append2_GetString(TWidget_t *pWidget,//基类
                              //0x01:首个按钮标志,0x02:第二个按钮标志
                              unsigned char Flag,
                              const char *p1st,
                              const char *p2nd);

//----------------可能附带最多两个附加项的键处理函数实现--------------------------
//以TWidget_t为基类的,最后一项可能有最多两具焦点的高级控件可调用此函数进行管理
//返回-1：需执行标准按键处理程序后再调用此函数
//正常返回0:焦点在第一个按钮上
//1:焦点在第二个按钮上
//2:焦点未发生变化
//否则返回焦点在那个按钮上,
unsigned char Append2_Key(TWidget_t *pWidget,//基类,
                          unsigned char Key,//键值
                          //0x01:首个按钮标志,0x02:第二个按钮标志
                          unsigned char Flag,
                          //-1:再次进入此函数
                          //其它:当前焦点在第二个按钮上标志
                          unsigned char In2);


#endif
