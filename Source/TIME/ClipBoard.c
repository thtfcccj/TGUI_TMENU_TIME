/*****************************************************************************

                         剪切板模块实现                   

*****************************************************************************/

#include "ClipBoard.h"  //剪切板
#include <string.h>

struct _ClipBoard ClipBoard; //直接单例化
 
/*****************************************************************************
                                 相关函数
*****************************************************************************/

//---------------------------初始化函数---------------------------------------
//开机时调用
void ClipBoard_Init(void)
{
  memset(&ClipBoard, 0, sizeof(ClipBoard));
  ClipBoard.PrvSel = -2; //未使用状态
}
