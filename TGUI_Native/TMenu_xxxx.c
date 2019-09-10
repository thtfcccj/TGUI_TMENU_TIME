/**************************************************************************

             TMenu中，关于xxxx应用相关的实现

**************************************************************************/

#include "TMenu.h"

/********************************************************************
                   TMenuMember_IARArm回调函数实现
********************************************************************/
//--------------------得到顶层菜单结构--------------------------
//各界面顶层菜单查找表

//各界面顶层菜单查找表,与UiDisp.TopMenuId对应
static const TMenu_t *const _pTopMenu[] = {
  &MainMenu1_Top,    //主界面1
  &MainMenu2_Top,    //主界面2
  &Menu_Top,    //菜单界面   
};

//-----------------------得到用户顶层菜单结构---------------------------
const TMenu_t *TM_pcbGetUserTopMenu(void)
{
  //根据提前指定进入
  const TMenu_t *pTopMenu = _pTopMenu[UiDisp.TopMenuId];
  return pTopMenu;
}

//---------------------进入菜单通报函数------------------------
void TMenu_cbEnterMenuNotify(void)
{
  TWin_Disp(TMenu_pGetTWin(UiMain_hGetMenuWin()));//置为显示
}

//---------------------退出菜单通报函数------------------------
void TMenu_cbExitMenuNotify(void)
{
  UiDisp_ExitMenu();
  TWin_Hidden(TMenu_pGetTWin(UiMain_hGetMenuWin()));//置为隐藏
}


