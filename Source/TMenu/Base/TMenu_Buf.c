/*******************************************************************************

                  TMenu�˵�֮�û��������˵��ؼ�ʵ��

*******************************************************************************/
#include "TMenu.h"
#include "TMenu_Buf.h"
#include <string.h>

//����:��־����Ϊ:
#define _FLAG_RETURN    0x01      //�ô˱�־��Ϊ���˳���һ��

//------------------------��������ʵ��----------------------------
//1:���������Ͳ˵�,���û���������ݲ�������ʾ
//���ش����ɹ�:
signed char TMenu_BufCreate(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                                 TWinHandle_t hWin,       //������˵��Ĵ���
                                 unsigned char Data,    //�������������,����������
                                 void *pv)      //�ɹ�ʹ�õ����ɻ�����
{
  struct _BufData *pBufData = (struct _BufData *)pv;
  TWin_t *pWin = TWM_pGetWin(hWin);

  pMenu = pMenu;
  Data = Data;
  pBufData->hWin = hWin;
  pBufData->Flag = 0;  
  pBufData->User.Notify = 0;
  pBufData->User.w = TWin_GetW(pWin);
  pBufData->User.h = TWin_GetH(pWin);
  pBufData->User.pBuf = TWin_pGetString(pWin,0);

  //���û��ռ�����ʾ
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_DATA,&pBufData->User);
  TWin_ClrFocus(pWin);//��ֹ��ʾ������
  TWin_Disp(pWin); //����������ʾ
  
  return -1;
}

//--------------------------ǿ��ˢ�º���ʵ��-----------------------------
void TMenu_BufRefresh(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                      void *pv)      //�ѱ�ʹ�õ��������ݻ�����
{
  struct _BufData *pBufData = (struct _BufData *)pv;
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_DATA,&pBufData->User);//������ʾ������
  TWin_Disp(TWM_pGetWin(pBufData->hWin));
}

//------------------------��Ӧ��������ʵ��----------------------------
//2:��Ӧ��������������������û��������ݽ���,��������ʾ
//����ֵ����Ϊ: ��ֵ:������Ӧ�Ӳ˵�,-1,����,-2:�����ϲ�˵�
signed char TMenu_BufKey(const TMenu_t *pMenu,    //ָ��Ĳ˵�ϵͳ
                              unsigned char Key,       //����ļ�ֵ
                              void *pv)      //�ѱ�ʹ�õ��������ݻ�����
{
  unsigned char Notify;
  struct _BufData *pBufData = (struct _BufData *)pv;

  //֪ͨ�û�����������
  pBufData->User.Key = Key;
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_BUF_KEY,&pBufData->User);
  Notify = pBufData->User.Notify;

  pBufData->User.Notify = 0;
  switch(Notify & TM_BUF_NOTIFY_MASK){
  //����������ʾ
  case TM_BUF_NOTIFY_DISP:
    TMenu_BufRefresh(pMenu,pv);
    break;

  //����������չ

  default:break;
  }

  //������˳��˲˵�
  if(Notify & TM_BUF_NOTIFY_EXIT_FLAG)
    return -2;
  


  return -1;
}

