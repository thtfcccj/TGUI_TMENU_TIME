/*******************************************************************************

            TGUI��Ϣͨ��ģ��-��ֹʹ�ú���ָ��ʱʵ��

*******************************************************************************/
#include"TGUINotify.h"
#ifdef TGUI_DIS_P_FUN //��ֹʹ�ú���ָ��ʱ

#include "TGUI.h"
#include <string.h>

//�����Ļص������ļ�:
#include "TMenu_CheckBox.h"
#include "TMenu_MNumAdj.h"
#include "TMenu_RadioBox.h"
#include "TMenu_SubMenu.h"

//----------------TGUI��Ϣ���ݲ�֧�ֺ���ָ����滻����ʵ��------------------
const void* TGUIpFunReplace(void *pvRource,     //��˭������ͨ��
                            unsigned char Type, //ͨ������
                            //�����������������ͨ��
                            const void *pvData,
                            TGUINotify_t ID)   // �ص�����ID��
{
  //Ϊ��ֹ����������ֵ������,ֱ��ʹ��case:
  switch(ID){
  //�ڲ��ؼ��ص�����
  case TGUI_CBFUNID_LISTBOX_EX: return TListBox_Notify(pvRource,Type,pvData);
  #ifndef TGUI_DIS_TEXT
  case TGUI_CBFUNID_TEXT: return TText_Notify(pvRource,Type,pvData);
  #endif
  #ifndef TGUI_DIS_CHECKBOX
  case TGUI_CBFUNID_CHECKBOX: return TCheckBox_Notify(pvRource,Type,pvData);
  #endif
  #ifndef TGUI_DIS_RADIO_BUTTON
  case TGUI_CBFUNID_RADIO_BUTTON: return TRadioButtom_Notify(pvRource,Type,pvData);
  #endif
  //�ⲿ�ؼ��ص�����
  case TGUI_CBFUNID_TMENU_CHECKBOX: return TMenu_CheckBoxNotify(pvRource,Type,pvData);
  case TGUI_CBFUNID_TMENU_MNUM_LISTBOX: return TMenu_MNumAdjListBoxNotify(pvRource,Type,pvData);//��ֵ�������б��
  case TGUI_CBFUNID_TMENU_MNUM_EDIT: return TMenu_MNumAdjEditNotify(pvRource,Type,pvData);//��ֵ�����ı༭��
  case TGUI_CBFUNID_TMENU_RADIO_BUTTON: return TMenu_RadioBoxNotify(pvRource,Type,pvData);
  case TGUI_CBFUNID_TMENU_SUB_MENU: return TMenu_SubMenu_Notify(pvRource,Type,pvData);
  default: break;
  }
  return NULL;
}







#endif //#ifdef TGUI_DIS_P_FUN //��ֹʹ�ú���ָ��ʱ

