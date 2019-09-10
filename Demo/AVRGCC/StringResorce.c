/* ----------------------------------------------------------------------------
 *               AVR GCC的字符串资源文件
 * --------------------------------------------------------------------------*/

#include "string.h"

#include "TMenuBase.h"

/*******************************************************************************
                      字符串定义
********************************************************************************/

/* 1 */static code const char LC_PassWord_Ch[]={"密码"};
/* 1 */static code const char LC_TopMenu_Ch[]={"系统菜单"};

/* 2 */static code const char LC_SprayPaintSet_Ch[]={"XXXX设置"};
/* 3 */static code const char LC_SprayPaintTemp_Ch[]={"XXXX温度"};

/*******************************************************************************
                 由字符串ID号得到字符串头函数回调函数实现
********************************************************************************/

const char *cbTM_pGetString(TStringSize_t StringID)
{
  switch(StringID){
  	case 0: return  NULL;    //固定空字符串
    case 1: return  LC_PassWord_Ch;
  	case 2: return  LC_TopMenu_Ch;

  	case 3: return  LC_SprayPaintSet_Ch;
  	case 4: return  LC_SprayPaintTemp_Ch;

  	default:break;
  }
  return NULL;
}




