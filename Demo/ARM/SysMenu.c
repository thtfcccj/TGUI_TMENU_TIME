/*******************************************************************************

                 TGUIϵͳ�˵�ʵ������Ҫ����غ���ʵ��

*******************************************************************************/
#include "TMenu.h"
#include "TMenu_CheckBox.h"
#include "TMenu_MNumAdj.h"
#include "TMenu_RadioBox.h"
#include "TMenu_SubMenu.h"

#include <string.h>

/**********************************************************************
                         TMenu�û��ص�����ʵ��
**********************************************************************/


/*/-----------------------�õ�ϵͳ��ǰ����------------------------
enum eLan_t GetLan(void)
{ 
  return (enum eLan_t)0; //һ������ 
}*/

//--------------TMenu����Ҫ���������ݻ���------------------
//,>һ���õ�����ַ�,���ֵ���ڲ����м�ֵ�Ļ���(��MNumAdj��)
unsigned long _Buf[(TLCD_WIDTH + 16) / 4];
void *TMenu_pGetBuf(void){
  return _Buf;
}

/**********************************************************************
                         TGUI��δʵ�ֵĻص�����
**********************************************************************/
//----------------------�õ���������ṹ����-----------------------
//��ĳЩ��Ƭ����֧��code��ָ��,�����ں��ѱ�����code��ָ������,
//�ں��賣��ʱ,ʹ�����г�����ʱ����泣������>= 3;
static char _ConstBuf[8]; 
extern char *TGUI_pGetConstBuf(void)
{
  return _ConstBuf;
}

//----------------------�õ����水ť��-----------------------
static const char LC_Save_Ch[]={"[ ȷ�� ]"};
//static const char LC_Enter_Ch[]={"[ ���� ]"};

//#include "MenuResorce.h"

const char *cbGetSaveBottom(void)
{
  const TMenu_t *pMenu = pTMenu_GetCurMenu();
  //if((pMenu == &MGetPower) || (pMenu == &MSelPoint))
  //  return LC_Enter_Ch;
  return LC_Save_Ch;
}

//----------------------�õ����ذ�ť��-----------------------
static const char LC_Exit_Ch[]={"[ �˳� ]"};

const char *cbGetReturnBottom(void)
{
  return LC_Exit_Ch;
}


/*/-------------------------��ʾ������һ�����ݺ���--------------------
void TWinMng_cbUpdateRow(unsigned char Lcd,    //��ǰ�����Ǹ���ʾ��
                         unsigned char x,      //��Ҫ���µ�����ʼλ��
                         unsigned char y,      //��Ҫ���µ���
                         unsigned char w,      //���³���
                         char *pBuf,    //������ַ�
                         //ÿ���ַ���Ӧλ�ñ�־,�����ж���
                         unsigned char *pFlagBuf)
{
  
  
}*/












