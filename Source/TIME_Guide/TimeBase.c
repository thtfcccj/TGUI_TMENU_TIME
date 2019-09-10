/*****************************************************************************

                         TImeBase相关实现

*****************************************************************************/

#include "TImeBase.h"
//---------------------------公共字符串-----------------------------------
const char chReturn[] =  {"返回"};



//-------------------------GuideKey至无确认键时的键值---------------------------
const unsigned char TIme_GuideKeyToNoEnterKey[] = {
  0,  //#define TIME_MNG_KEY_UP       0
  1,  //#define TIME_MNG_KEY_LEFT     1
  4,  //#define TIME_MNG_KEY_ENTER    2
  2,  //#define TIME_MNG_KEY_RIGHT    3
  3,  //#define TIME_MNG_KEY_DOWN     4
};



