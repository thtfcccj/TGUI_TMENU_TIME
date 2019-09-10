/*******************************************************************************

                           TWidget�ؼ�����ʵ��
��ģ�����:TWIN(����)
*******************************************************************************/
#include "TWidget.h"
#include <string.h>
#include "TWidgetPrv.h"  //�ڲ�����,�����ڲ�ʹ��
/**********************************************************************
                         ��Ա��������
**********************************************************************/
//-----------------------�����û�����������ʼλ��y---------------------
void TWidget_SetY(TWidget_t *pWidget,TItemSize_t Y)
{
  unsigned char Page = TWidget_GetVPageSize(pWidget);
  TItemSize_t MaxSel = pWidget->ItemsRect.h - 1;
  if(MaxSel <= Page) Y = 0;//һҳ����ʾ��
  else{
    //MaxY -= Page;//���һҳ����ʾȫʱ
    if(Y > MaxSel) Y = MaxSel;//���һҳ
  }
  pWidget->ItemsRect.y = Y;
}


//-----------------------���õ�ǰ��ѡ��------------------------
void TWidget_SetSel(TWidget_t *pWidget,TItemSize_t Sel)
{
  unsigned char Page = TWidget_GetVPageSize(pWidget);
  TItemSize_t MaxSel = pWidget->ItemsRect.h - 1;
  if(Sel > MaxSel) Sel = MaxSel;
  pWidget->Sel = Sel;
  //�����ܹ��ڵ�ǰҳ��ʾ
  if(Sel >= (Page + pWidget->ItemsRect.y))
    pWidget->ItemsRect.y = Sel - Page + 1;
  //�ڵ�һҳʱ,�����ܹ���ʾ
  else if(Sel < pWidget->ItemsRect.y)
    pWidget->ItemsRect.y = Sel;
}
//------------------��ѡ����һ,���ص������ѡ����---------------
TItemSize_t TWidget_IncSel(TWidget_t *pWidget)
{
  TItemSize_t  Sel = pWidget->Sel;
  TItemSize_t MaxSel = pWidget->ItemsRect.h - 1;
  if(Sel >= MaxSel){  //�����������
    if(pWidget->Flag & TWIDGET_EN_RETURN)
      Sel = 0;
    else
      Sel = MaxSel; //���ػ�
  }
  else Sel++;
  pWidget->Sel = Sel;
  return Sel;
}
//----------------��ѡ���һ,���ص������ѡ����--------------
TItemSize_t TWidget_DecSel(TWidget_t *pWidget)
{
  TItemSize_t  Sel = pWidget->Sel;
  if(!Sel){
    if(pWidget->Flag & TWIDGET_EN_RETURN)
      Sel = pWidget->ItemsRect.h - 1; //�����һ��
  }
  else Sel--;
  pWidget->Sel = Sel;
  return Sel;
}

//-------------------------����������-----------------------------
void TWidget_SetItems(TWidget_t *pWidget,TItemSize_t Items)
{
  //if(!Items) return;  //��ֹΪ0
  pWidget->ItemsRect.h = Items;
  //����ѡ��
  if(pWidget->Sel > Items) pWidget->Sel = Items;
}

//--------------------Ԥ�����,������----------------------
void  TWidget_SetW(TWidget_t *pWidget,TItemSize_t w)
{
  //if(pWidget->ItemsRect.w < w)
     pWidget->ItemsRect.w = w;
}

//----------------------�õ���ʾ����x--------------------------------
unsigned char TWidget_GetDispX(const TWin_t *pWin,
                               unsigned char Flag)
{
  pWin = pWin;
  if(Flag & TWIDGET_EN_FRAME) return 2;
  return 0;
}

//----------------------�õ���ʾ������--------------------------------
unsigned char TWidget_GetDispW(const TWin_t *pWin,
                               unsigned char Flag)
{
  unsigned char w = TWin_GetW(pWin);
  if(Flag & TWIDGET_EN_FRAME) return w - 4;
  if(Flag & TWIDGET_EN_VSCOLL) return w - 2;
  return w; 
}

//----------------------�õ���ʾ����y--------------------------------
unsigned char TWidget_GetDispY(const TWin_t *pWin,
                               unsigned char Flag)
{
  pWin = pWin;
  if(Flag & TWIDGET_EN_HEADER) return 1;
  return 0;
}

//----------------------�õ���ʾ����߶�--------------------------------
unsigned char TWidget_GetDispH(const TWin_t *pWin,
                               unsigned char Flag)
{
  unsigned char h = TWin_GetH(pWin);
  if(Flag & TWIDGET_EN_HEADER) h--;
  if(Flag & (TWIDGET_EN_FRAME | TWIDGET_EN_HSCOLL)) h--;
  return h;
}

//----------------------�õ�ˮƽҳ��С--------------------------------
unsigned char TWidget_GetHPageSize(TWidget_t *pWidget)
{
  return TWidget_GetDispW(TWM_pGetWin(pWidget->hWin),pWidget->Flag);
}

//----------------------�õ���������ʾ���ˮƽҳ��С--------------------------------
unsigned char TWidget_GetHPageSizeEnNotify(TWidget_t *pWidget)
{
  unsigned char w = TWidget_GetDispW(TWM_pGetWin(pWidget->hWin), 
                                     pWidget->Flag);
  //��ֻ����ʾ,�ų���ʾ
  if((w == pWidget->ItemsRect.w) && (pWidget->Flag & TWIDGET_EN_NOTIFY))
    w -= 2;
  return w;
}

//----------------------�õ���ֱҳ��С--------------------------------
unsigned char TWidget_GetVPageSize(TWidget_t *pWidget)
{
  return TWidget_GetDispH(TWM_pGetWin(pWidget->hWin),pWidget->Flag);
}

//----------------------�õ��û���������������x--------------------------------
unsigned char TWidget_GetClipX(TWidget_t *pWidget)
{
  TWin_t *pWin = TWM_pGetWin(pWidget->hWin);
  return TWin_GetX(pWin) + TWidget_GetDispX(pWin,pWidget->Flag);
}

//----------------------�õ��û���������������y--------------------------------
unsigned char TWidget_GetClipY(TWidget_t *pWidget)
{
  TWin_t *pWin = TWM_pGetWin(pWidget->hWin);
  return TWin_GetY(pWin) + TWidget_GetDispY(pWin,pWidget->Flag);
}


//�����û���־
void TWidget_SetUserFlag(TWidget_t *pWidget,unsigned char Flag)
{
  pWidget->Flag &= ~TWIDGET_USER;
  pWidget->Flag |= (Flag & TWIDGET_USER);
}


/*/----------------------�õ���ʾ����������-------------------------
//���ݴ��ڴ�С�ͱ�־ȷ���ڴ����ڵ���ʾʽ����
void _GetDispClip(TRect_t *pClip,
                  const TWin_t *pWin,
                  unsigned char Flag)
{
  unsigned char Data = TWin_GetY(pWin);
  unsigned char Len = TWin_GetH(pWin);
  if(Flag & TWIDGET_EN_FRAME){
    Len -= 2; //�߿�ռ����
    Data += 1;
  }
  else if(Flag & TWIDGET_EN_HEADER){
    Len -= 1; //ͷռһ��
    Data += 1;
  }
  pClip->h = Len;
  pClip->y = Data;
  Data = TWin_GetX(pWin);
  Len = TWin_GetW(pWin);
  if(Flag & TWIDGET_EN_FRAME){
    Data += 2;
    Len -= 4; //��߿�Ҫռ4��
  }
  pClip->w = Len;
  pClip->x = Data;
}*/

//--------------------------��С�ı�ͨ������-------------------------
//���ı�TWidget_t��Ӧ��Win��С�ı�ʱ,�����Զ��ı���ʾ��ʽ
void TWidget_WinReSizeNotify(TWidget_t *pWidget,
                             unsigned char w,
                             unsigned char h)
{
  unsigned char Flag = pWidget->Flag;
  //���ڹ�Сʱʹ�ñ߿���Ч
  if((w < 10) || (h < 4)) Flag &= ~TWIDGET_EN_FRAME;
  //ˮƽ��Сʱʹ��ˮƽ��������Ч
  if(w < 10) Flag &= ~TWIDGET_EN_HSCOLL;
  //�߶ȹ�Сʱ��ʾͷ��Ч
  if(h < 2) Flag &= ~TWIDGET_EN_HEADER;
  //�߶ȹ�Сʱ��ֱ��������Ч
  if(h < 5) Flag &= ~TWIDGET_EN_VSCOLL;
  pWidget->Flag = Flag;
}

//--------------------------���Ƴ�ʼ������-------------------------
//���������ؼ�ʱ���ô˺�����ʼ������
//�˺����Զ�������־����־,��������ʾ������Ϊ�����ڴ����ڵ������
//��ʼ��Ϊ����������,������0��
void TWidget_Init(TWidget_t *pWidget,
                  TWinHandle_t hWin,
                  unsigned char Flag)
{
  TWin_t *pWin;
  memset(pWidget,0,sizeof(TWidget_t));
  pWidget->hWin = hWin;
  pWidget->Sel = 0;
  pWidget->Flag = Flag;
  
  pWin = TWM_pGetWin(hWin);
  TWidget_WinReSizeNotify(pWidget,
                          TWin_GetW(pWin),
                          TWin_GetH(pWin));
  
  //������Ĭ��ռ����ʾ����
  pWidget->ItemsRect.x = 0;//TWidget_GetDispX(pWin,Flag);
  pWidget->ItemsRect.y = 0;//TWidget_GetDispY(pWin,Flag);
  pWidget->ItemsRect.w = TWidget_GetDispW(pWin,Flag);//Ĭ�Ͽ��
  pWidget->ItemsRect.h = 2; //����Ĭ��Ϊ2��
}

//-----------------------����������---------------------------
//���¼�������л�,���Ҽ�ˮƽ�ƶ�����,���·�ҳ����ҳ,���ּ�ѡ����Ŀ
//���ر�־λ����Ϊ��λʱ�����仯(������)
unsigned int TWidget_Key(TWidget_t *pWidget,unsigned char Key)
{
  TItemSize_t  Sel;
  TItemSize_t PrvSel = TWidget_GetSel(pWidget);//��ס�ϴ�λ��
  const TWin_t *pWin = TWM_pGetWin(pWidget->hWin);
  unsigned char Flag = pWidget->Flag;
  unsigned char PageSize;
  
  Sel = 0;//��ʼ����ֹ���ֱ���������
  
  switch(Key){
  case TGUI_KEY_LEFT:{  //�����ƶ�������
    Sel = pWidget->ItemsRect.x;
    if(!Sel) return 0;  //���������
    if(Sel >= TGUI_HORI) Sel-= TGUI_HORI;
    else Sel = 0;
    pWidget->ItemsRect.x = Sel;
    return TGUI_PAINT_HCONTEXT | TGUI_PAINT_FOCUS;
  }
  case TGUI_KEY_RIGHT:{//����ʾ����ȫ,����������ƶ���
    Sel = pWidget->ItemsRect.x;
    if((pWidget->ItemsRect.w - Sel) <= TWidget_GetDispW(pWin,Flag))
      return 0;
    pWidget->ItemsRect.x += TGUI_HORI;
    return TGUI_PAINT_HCONTEXT | TGUI_PAINT_FOCUS;
  }
  case TGUI_KEY_DOWN:{
    Sel = TWidget_IncSel(pWidget);
    if(Sel == PrvSel) return 0;   //δ�仯
    if(Sel > PrvSel){//������,��鴰���ƶ�
      if(Sel >= (pWidget->ItemsRect.y + TWidget_GetDispH(pWin,Flag))){
        pWidget->ItemsRect.y++;
        return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;
      }
      return  TGUI_PAINT_FOCUS;//����仯
    }
    pWidget->ItemsRect.y = 0;   //�ػ���
    return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;
  }
  case TGUI_KEY_UP:{
    Sel = TWidget_DecSel(pWidget);
    if(Sel == PrvSel) return 0;   //δ�仯
    if(Sel < PrvSel){//������,��鴰���ƶ�
      if(Sel < pWidget->ItemsRect.y){
        pWidget->ItemsRect.y--;
        return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;
      }
      return  TGUI_PAINT_FOCUS;//����仯
    }
    PageSize = TWidget_GetDispH(pWin,Flag);
    if(pWidget->ItemsRect.h > PageSize)
      pWidget->ItemsRect.y = pWidget->ItemsRect.h - PageSize;   //�ػ��������
    return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;  
  }
  case TGUI_KEY_PAGEUP:{
    PageSize = TWidget_GetDispH(pWin,Flag);
    if(pWidget->ItemsRect.h <= PageSize) return 0;//һҳ����ʾ��
    if(!PrvSel){//�ڵ�һ��
      if(Flag & TWIDGET_EN_RETURN){//�ػ������һ��
        TWidget_SetSel(pWidget,pWidget->ItemsRect.h - 1);
        return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;
      }
      return 0;
    }
    //ͬʱY��ı�
    if(pWidget->ItemsRect.y > PageSize)
      TWidget_SetY(pWidget,pWidget->ItemsRect.y - PageSize);
    //������
    if(PrvSel > PageSize) Sel = PrvSel - PageSize;
    else Sel = 0;
    TWidget_SetSel(pWidget,Sel);

    return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;
  }
  case TGUI_KEY_PAGEDOWN:{     //���,��һҳ
    PageSize = TWidget_GetDispH(pWin,Flag);
    //�������һҳ��
    if((pWidget->ItemsRect.y + PageSize) >= pWidget->ItemsRect.h){
      if(Flag & TWIDGET_EN_RETURN){//�ػ�����һҳ
        TWidget_SetSel(pWidget,0);
        return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;
      }
      return 0;
    }
    //��һҳ,ͬʱY��ı�
    Sel = PrvSel + PageSize;       
    TWidget_SetY(pWidget,pWidget->ItemsRect.y + PageSize);
    TWidget_SetSel(pWidget,Sel);
    return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;
  } 
  default:{
    if((Key >= '1') && (Key <= '9')){
      TWidget_SetSel(pWidget, Key - '1');
      //ȷ���ܹ���ʾ��ѡ��
      Sel = TWidget_GetSel(pWidget);
      if(PrvSel == Sel) break;
      //ѡ��ı�ʱ
      PrvSel = pWidget->ItemsRect.y;//�ݴ�
      if(Sel < PrvSel){
        pWidget->ItemsRect.y = Sel;
      }
      else{
        PrvSel += TWidget_GetDispH(pWin,Flag);
        if(Sel >= PrvSel)//������ҳʱ��ʾ�����һ��
         pWidget->ItemsRect.y = PrvSel;
      }
      return TGUI_PAINT_VCONTEXT | TGUI_PAINT_FOCUS;
    }
  }
  }
  return 0;
}

