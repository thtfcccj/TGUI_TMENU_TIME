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

/* 2 */static code const char LC_SprayPaintSet_Ch[]={"喷烤设置"};
/* 3 */static code const char LC_SprayPaintTemp_Ch[]={"喷漆温度"};

/* 4 */static code const char LC_DeviceIdSet_Ch[]={"机组地址"};
/* 5 */static code const char LC_ShirPaintTemp_Ch[]={"烤漆温度"};
/* 6 */static code const char LC_ReturnTemp_Ch[]={"温差设定"};
/* 7 */static code const char LC_HiTempProtect_Ch[]={"高温保护"};
/* 8 */static code const char LC_ShirTime_Ch[]={"烤漆时间"};

/* 9 */static code const char LC_DelaySet_Ch[]={"延时设置"};
/* 10 */static code const char LC_DoorTime_Ch[]={"风门转换"};
/* 11 */static code const char LC_ActDelay_Ch[]={"动作延时"};
/* 12 */static code const char LC_V_H_Ch[]={"电压上限"};
/* 13 */static code const char LC_V_L_Ch[]={"电压下限"};

/* 14 */static code const char LC_OtherSet_Ch[]={"出厂设置"};
/* 15 */static code const char LC_TCh1Adjust_Ch[]={"温度1 校准"};
/* 16 */static code const char LC_RecoverSet_Ch[]={"恢复出厂设置"};
/* 15 */static code const char LC_TCh2Adjust_Ch[]={"温度2 校准"};

/* 17 */static code const char LC_HandCtrl_Ch[]={"手动控制"};
/* 18 */static code const char LC_HandCtrl1_Ch[]={"送风1"};
/* 19 */static code const char LC_HandCtrl2_Ch[]={"送风2"};
/* 20 */static code const char LC_HandCtrl3_Ch[]={"加热1"};
/* 21 */static code const char LC_HandCtrl4_Ch[]={"加热2"};
/* 22 */static code const char LC_HandCtrl5_Ch[]={"排风"};
/* 23 */static code const char LC_HandCtrl6_Ch[]={"烤风门"};
/* 24 */static code const char LC_HandCtrl7_Ch[]={"照明"};
/* 25 */static code const char LC_HandCtrl8_Ch[]={"报警"};


/* 26 */static code const char LC_DeviceId_Ch[]={"机组号"};
/* 26 */static code const char LC_NewPassWord_Ch[]={"新密码"};


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

  	case 5: return  LC_DeviceIdSet_Ch;

  	case 6: return  LC_ShirPaintTemp_Ch;
  	case 7: return  LC_ReturnTemp_Ch;
  	case 8: return  LC_HiTempProtect_Ch;
	case 9: return  LC_ShirTime_Ch;

	case 10: return  LC_DelaySet_Ch;
	case 11: return  LC_DoorTime_Ch;
  	case 12: return  LC_ActDelay_Ch;
	case 13: return  LC_V_H_Ch;
    case 14: return  LC_V_L_Ch;

	case 15: return  LC_OtherSet_Ch;
    case 16: return  LC_TCh1Adjust_Ch;
    case 17: return  LC_TCh2Adjust_Ch;
    case 18: return  LC_RecoverSet_Ch;

  	case 19: return  LC_HandCtrl_Ch;
  	case 20: return  LC_HandCtrl1_Ch;
  	case 21: return  LC_HandCtrl2_Ch;
  	case 22: return  LC_HandCtrl3_Ch;
  	case 23: return  LC_HandCtrl4_Ch;
  	case 24: return  LC_HandCtrl5_Ch;
  	case 25: return  LC_HandCtrl6_Ch;
  	case 26: return  LC_HandCtrl7_Ch;
  	case 27: return  LC_HandCtrl8_Ch;

    case 28: return  LC_DeviceId_Ch;
    case 29: return  LC_NewPassWord_Ch;

  	default:break;
  }
  return NULL;
}




