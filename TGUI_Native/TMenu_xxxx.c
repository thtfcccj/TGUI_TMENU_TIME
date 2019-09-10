/**************************************************************************

             TMenu�У�����xxxxӦ����ص�ʵ��

**************************************************************************/

#include "TMenu.h"

/********************************************************************
                   TMenuMember_IARArm�ص�����ʵ��
********************************************************************/
//--------------------�õ�����˵��ṹ--------------------------
//�����涥��˵����ұ�

//�����涥��˵����ұ�,��UiDisp.TopMenuId��Ӧ
static const TMenu_t *const _pTopMenu[] = {
  &MainMenu1_Top,    //������1
  &MainMenu2_Top,    //������2
  &Menu_Top,    //�˵�����   
};

//-----------------------�õ��û�����˵��ṹ---------------------------
const TMenu_t *TM_pcbGetUserTopMenu(void)
{
  //������ǰָ������
  const TMenu_t *pTopMenu = _pTopMenu[UiDisp.TopMenuId];
  return pTopMenu;
}

//---------------------����˵�ͨ������------------------------
void TMenu_cbEnterMenuNotify(void)
{
  TWin_Disp(TMenu_pGetTWin(UiMain_hGetMenuWin()));//��Ϊ��ʾ
}

//---------------------�˳��˵�ͨ������------------------------
void TMenu_cbExitMenuNotify(void)
{
  UiDisp_ExitMenu();
  TWin_Hidden(TMenu_pGetTWin(UiMain_hGetMenuWin()));//��Ϊ����
}


