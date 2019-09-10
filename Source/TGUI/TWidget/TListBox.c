/*******************************************************************************

                           TWidget֮�б��ؼ�ʵ��

*******************************************************************************/

#include "TListbox.h"
#include <string.h>

//-------------------------���ڻ�ͼ����-----------------------------
void ListBox_Paint(TListbox_t *pListbox,    //����ؼ�
                   //���Ʊ�־,���������������ض���,0xff��δ��������
                   unsigned char PaintFlag) 
{
  char *pBuf;
  const char *pString;
  TItemSize_t Items,Item;
  TItemSize_t StringStart,FullLen,CpyLen;
  TGUINotify_t cbGUINotify = pListbox->cbGUINotify;
  TItemSize_t  StringLen = 0;
  
  //1:��ͷ�ı�,�����TWidget_PaintHeader����ͷ
  if(PaintFlag & TGUI_PAINT_HEADER){
    pString = TGUI_NOTIFY_RUN(cbGUINotify,pListbox,TGUI_NOTIFY_GET_CAPTION,&TListbox_GetSel(pListbox));
    TWidget_PaintHeader((TWidget_t *)pListbox,pString, 0);
  }
  //2:������(ˮƽ�봹ֱ����)�����仯,Ӧ����TWidget_pGetPaintRow���ڻ�������������
  if(PaintFlag & (TGUI_PAINT_HCONTEXT | TGUI_PAINT_VCONTEXT | TGUI_PAINT_DATA)){
    Items = TListbox_GetItems(pListbox);
    for(Item = TWidget_GetPaintRowItemStart((TWidget_t *)pListbox); Item < Items; Item++){
      pBuf = TWidget_pGetPaintRow((TWidget_t *)pListbox,Item,StringLen);
      if(!pBuf) break;//������
      pString = TGUI_NOTIFY_RUN(cbGUINotify,pListbox,TGUI_NOTIFY_GET_ITEM,&Item);
      StringLen = strlen((const char*)pString);
      StringStart = TWidget_GetPaintRowStringStart((TWidget_t *)pListbox);
      FullLen = TWidget_GetPaintRowLen((TWidget_t *)pListbox);//�ٶ�ȫ�����
      if(StringLen >= StringStart){              //������Ҫ������
        CpyLen = StringLen - StringStart;
        if(CpyLen > FullLen){CpyLen = FullLen; FullLen = 0;}//һ����ʾ����
        else FullLen -=  CpyLen;    //���һ����,���油�ո�
        memcpy(pBuf, pString + StringStart,CpyLen); 
        pBuf += CpyLen;
      }
      //else //��Ч���ݲ��ܹ���ʾ��,ȫ�����
      //������ո�
      memset(pBuf,' ',FullLen);
    }
  }
  //3:�����㷢���仯,���ϲ㴦��
  if(PaintFlag & TGUI_PAINT_FOCUS){
    TGUI_NOTIFY_RUN(cbGUINotify,pListbox,TGUI_NOTIFY_GET_FOCUS,&TListbox_GetSel(pListbox));
  }
  //4:������ˮƽ�봹ֱ����仯�ı�־����TWidget_PaintEnd,��ͼ����
  TWidget_PaintEnd((TWidget_t *)pListbox,PaintFlag,StringLen);
}

//-------------------------��ʼ������-----------------------------
//ע:���Ǻܶ�ϵͳ��ʹ�þ�̬�ڲ����,�ʽ�����������������һ��,��
//���ڴ˻������������Ĵ�������
void TListbox_Init(TListbox_t *pListbox,  //���û�����Ĵ洢�ռ�
                   TWinHandle_t hWin,     //�ѽ�������ʼ���õĴ���
                   unsigned char Flag,    //�ؼ���־
                   TItemSize_t Items,         //Ԥ������Ŀ����
                   TGUINotify_t cbGUINotify) //ͨ������
{
  pListbox->cbGUINotify = cbGUINotify;
  TWidget_Init((TWidget_t*)pListbox,hWin,Flag);
  TListbox_SetItems(pListbox,Items);
}

//-------------------------����������-----------------------------
void TListbox_Key(TListbox_t *pListbox,unsigned char Key)
{
  unsigned char PaintFlag = 0;
  //�ȼ���˳���,0��Ҳ�����˳�������
  if((Key == TGUI_KEY_ESCAPE) || (Key == '0')){
    TGUI_NOTIFY_RUN(pListbox->cbGUINotify,pListbox,TGUI_NOTIFY_KEY_ESCAPE,&TListbox_GetSel(pListbox));
    return;
  }
  if(Key != TGUI_KEY_ENTER){
    PaintFlag = TWidget_Key((TWidget_t*)pListbox,Key);
    //if((Key >= '1') && (Key <= '9')) //���ּ������,���赱�����������
    //  Key = TGUI_KEY_ENTER; 
 }
 if(Key == TGUI_KEY_ENTER) 
    PaintFlag = *(unsigned char*)TGUI_NOTIFY_RUN(pListbox->cbGUINotify,pListbox,TGUI_NOTIFY_KEY_ENTER,
                                                &TListbox_GetSel(pListbox));

  //�ػ����
  ListBox_Paint(pListbox,PaintFlag);
}
