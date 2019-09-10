/*******************************************************************************

                  TMenu��������

*******************************************************************************/
#ifndef __T_MENU_BASE_H 
#define __T_MENU_BASE_H

#include "TMenuCfg.h"
#include "TGUI.h"       //������TGUI֮��
#include "TMenuNotify.h"  //��Ϣͨ������


/****************************************************************
                  �˵����ṹ����˵��������
*****************************************************************/

//����ʵ��Ӧ����Ҫ,�˵��ṹ�����û�����,�������������:
//struct _TMenu{
//  unsigned char Type;      //�˵����ͼ���ر�־
//  unsigned char Size;       //�ɲ˵����;�����������ݴ�С
//  const LanguageCode_t *pgHeader; //�˵�ͷָ��,ΪNULLʱ�ӻص����ȡ
//  const struct _TMenu *pParent;   //���ѵĸ��˵�
//  const void *pv;           //���ݲ˵�����ȷ�������ɲ���ָ��
//  TMenuNotify_t cbNotify;     //���û��ռ佻����ͨ������
//};
//����:LanguageCode_t  ����Ϊchar*�ṹ
//Ҳ����Ϊ�����ṹ,��:�������֧��ʱ�����ڱ��治ͬ���ֶ�Ӧ���ַ�������ָ�롣


//�˽ṹ�ĳ�Ա���ʽ�����ͨ��TMenu.h����ĺ�������
typedef struct _TMenu TMenu_t;
//TMenu_t���û�������Ҫ��������壺

#include "TMenuMember_IARArm.h" //ͨ����(ARM)����
//#include "TMenuMember_KeilC51.h" //C51���Ͷ���
//#include "TMenuMember_AVRGCC.h" //AVRGCC���Ͷ���

/****************************************************************
                  �˵����ṹ�ڣ����ͼ���ر�־����
*****************************************************************/
//�ڸýṹ����,�˵����ͼ���ر�־����Ϊ��
#define   TMTYPE_MASK       0x0f //��4λΪ�˵�����,����Ϊ:
  #define   TMTYPE_SUBMENU  0   //�Ӳ˵�ģʽ
  #define   TMTYPE_RADIOBOX 1   //��ѡ��ģʽ
  #define   TMTYPE_MNUMADJ  2   //��ֵ����ģʽ
  #define   TMTYPE_CMD      3   //����ģʽ
  #define   TMTYPE_BUF      4   //�û�������ģʽ,������ʾ�밴��(���˳�)�������û����� 
  #define   TMTYPE_CHECKBOX 5   //��ѡ��ģʽ
  #define   TMTYPE_LISTBOX  6   //�б��ģʽ(��δʵ��)
  #define   TMTYPE_EDITSEL  7   //�༭ѡ��ģʽ
  #define   TMTYPE_EDITSTRING 8   //�༭�ַ���ģʽ

  #define	 TMTYPE_MAX    8	  //����˵����Ϳ�ʹ����Ч����

//�������ڲ�ͬ�˵�����ģʽ���в�ͬ�Ķ���
#define   TMTYPE_USER_SHIFT 4
#define   TMTYPE_USER_MASK  0xf0 

/****************************************************************
        �˵����ṹ��Ӧ�˵���������ɵĲ�����������
*****************************************************************/
typedef struct{
  //1:���������Ͳ˵�,���û���������ݲ�������ʾ
  //���ش����ɹ�:
  signed char (*Create)(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                        TWinHandle_t hWin,       //������˵��Ĵ���
                        unsigned char Data,    //�������������
                        void *pv);      //�ɹ�ʹ�õ����ɻ�����
  //2:��Ӧ��������������������û��������ݽ���,��������ʾ
  //����ֵ����Ϊ: ��ֵ:������Ӧ�Ӳ˵�,-1,����,-2:�����ϲ�˵�
  signed char(*Key)(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                    unsigned char Key,       //����ļ�ֵ
                    void *pv);              //�ѱ�ʹ�õ��������ݻ�����
}TMenuOperate_t;

/****************************************************************
                        ��������
*****************************************************************/

//----------------------������˵���ʱ�������----------------
//�ڸ����ְ���������,ʹ�õĻص������޷�ֱ�ӻ��
//�������,�Ұ���������̲���ִ����������,��ʹ�����в���
//���˵��ṹ��ջ
//void PushMenu(const TMenu_t *);
#define	PushMenu(pTMenu) do{pTMenu = pTMenu; }while(0)

//���˵��ṹ��ջ
const TMenu_t *pPopMenu(void);

//��ǰָ������ݻ�������ջ
//void PushData(const TMenu_t *);
#define	PushData(pv) do{pv = pv;}while(0)

//����ǰָ������ݻ�������ջ
void *pPopData(void);

//-------------------�õ���������������----------------------
//TWinHandle_t  TM_hGetMWin(void);
#define  TM_hGetMWin()  (TMENU_FIX_WIN_COUNT) 

//-------------------�õ��˵�������������----------------------
//TWinHandle_t  TM_hGetMenuWin(void);
#define  TM_hGetMenuWin()  (TMENU_FIX_WIN_COUNT + TMENU_MWIN_SIGNAL) 

//----------------------�õ��༭��������������---------------------
//TWinHandle_t  TM_hGetEditWin(void);
#define  TM_hGetEditWin() \
  (TMENU_FIX_WIN_COUNT + TMENU_MWIN_SIGNAL + TMENU_MSG_BOX + 1) 

//----------------------�õ��Ի�����ʼ������������---------------------
//TWinHandle_t  TM_hGetMsgBoxWin(void);
#define  TM_hGetMsgBoxWin()  \
  (TMENU_FIX_WIN_COUNT + TMENU_MWIN_SIGNAL + TMENU_MSG_BOX + 2) 

#endif
