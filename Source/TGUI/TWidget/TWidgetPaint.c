/*******************************************************************************

                           TGUI���ڻ�������֮��ͼ��غ���ʵ��

*******************************************************************************/
#include "TWidget.h"
#include <string.h>
#include "TWidgetPrv.h"  //�ڲ�����

//--------------------�õ��������ʾ�ռ�Y����-------------*
//���ؼ��к��λ��,��������ʾ�ռ���ʱ,������-1
static TItemSize_t _GetClipY(TWidget_t *pWidget,
                               TItemSize_t VPageSize,
                               TItemSize_t Item)
{
  TItemSize_t y =  pWidget->ItemsRect.y;
  //ȷ���������Ƿ�����ʾ��Χ
  if(Item < y) return (TItemSize_t)-1;//����ʾ���Ϸ�
  Item -= y;  //ת��Ϊ����������
  if(Item >= VPageSize)
    return (TItemSize_t)-1;//����ʾ���·�
  return Item;
}

//--------------------�õ��������ʾ�ռ�x����-------------*
//���ؼ��к��λ��
static TItemSize_t _GetClipX(TWidget_t *pWidget,
                               unsigned char HPageSize,
                               TItemSize_t ItemX,//�����X��λ��
                               TItemSize_t *pW) //������,���ؼ��к���
{
  TItemSize_t x =  pWidget->ItemsRect.x;
  TItemSize_t w = *pW;
  
  //������
  if(ItemX < x){  //������
    if(w >= x){//����ʾʱ
      w -= x;//���ƺ����¿��
      if(w > HPageSize)//����ʾһҳ
        w = HPageSize;
    }
    else w = 0;//�ƶ��ˣ�������ʾ��
    *pW = w;
    return 0;
  }
  
  ItemX -= x;  //ת��Ϊ����������
  if(ItemX > HPageSize) //����Ϊ��ʾ��Χ��
    w = 0;//������ʾ��
  else if((ItemX + w) > HPageSize)
    w = HPageSize - ItemX;
  
  *pW = w;
  return ItemX;
}

//-------------------������ͷ���ϱ߿���----------------------------*
//�˺������ϲ�߿�,������ͷ
void TWidget_PaintHeader(TWidget_t *pWidget,
                        const char *pHeader,   //��ʾͷ,��ʱΪNULL
                        unsigned char Len)    //��ʾͷ���ݳ���,��Ϊ0,��Ӧ�������ַ�
{
  TWin_t *pWin;
  char *pBuf;
  unsigned char w,i;
  unsigned char Flag = pWidget->Flag;
  if(!(Flag & TWIDGET_EN_HEADER)) return;//����Ҫ��ʾͷ
  
  pWin = TWM_pGetWin(pWidget->hWin);
  pBuf = TWin_pGetString(pWin,0);
  
  if(Flag & TWIDGET_EN_FRAME){*pBuf++ = 0xa9;*pBuf++ = 0xb0;}//����ʱ,���"��"
  else {*pBuf++ = 0xa9;*pBuf++ = 0xa4;}//������ʱ,���"��"
  if(pHeader == NULL) Len = 0;//��ͷ��ʾ
  else if(!Len) Len = strlen((const char*)pHeader);
  
  w = TWin_GetW(pWin) - 4;
  if(Len > w) {Len = w; w = 0;}//w�����ʾ���λ��
  else w = (w - Len) >> 2;//������ʾ,˫�ֽڶ���
  
  //���"��"
  for(i = 0; i < w; i++) {*pBuf++ = 0xa9;*pBuf++ = 0xa4;}//������"��"
  memcpy(pBuf,pHeader,Len);
  w = TWin_GetW(pWin) - (w << 1) - Len - 2;//�Ҳ���
  pBuf += Len;
  if(w & 0x01){w += 1; *pBuf++= ' ';}//��֤˫�ֽڶ���
  if(w < 2) return;//�쳣�˳�
  w -= 2;//����������Ԥ��
  w >>= 1;
  for(i = 0; i < w; i++) {*pBuf++ = 0xa9;*pBuf++ = 0xa4;}//�Ҳ����"��"
  
  if(Flag & TWIDGET_EN_FRAME){*pBuf++ = 0xa9;*pBuf++ = 0xb4;}//����ʱ,���"��"
  else {*pBuf++ = 0xa9;*pBuf = 0xa4;}//������ʱ,���"��"
}

//---------------------�õ��л���������---------------------------*
//�˺��������߿�,������л���λ�ü����ȹ��ⲿ�������
//�����ؿ�,��ʾ���ɽ����ַ�
//ע:�ɽ������ݳ��ȿɵ���TWidget_pGetPaintRowLen()�õ�
//���ַ���ʼλ�ÿɵ���TWidget_pGetPaintRowItemStart()�õ�
char *TWidget_pGetPaintRow(TWidget_t *pWidget,
                                    TItemSize_t Item,    //��Ҫ��ʾ����
                                    TItemSize_t PrvLen) //��һ���û��ַ�����
{
  char *pBuf;
  TWin_t *pWin = TWM_pGetWin(pWidget->hWin);
  unsigned char Flag = pWidget->Flag;
  TItemSize_t w;
  TItemSize_t VPageSize = TWidget_GetDispH(pWin,Flag);
  //ͳ���������
  w = pWidget->ItemsRect.w;
  if(PrvLen > w) pWidget->ItemsRect.w = PrvLen;
  //Y�����  
  Item = _GetClipY(pWidget,VPageSize,Item);
  if(Item == (TItemSize_t)-1) return NULL;//������ʾ��Χ

  //��ʾ������
  pBuf = TWin_pGetString(pWin,Item + TWidget_GetDispY(pWin,Flag));
  if(pBuf == NULL) return NULL;//��ʾ�������
  
  //�ޱ߿�ʱ,���߿�λ�ÿ�ʡ��
  if(!(pWidget->Flag & TWIDGET_EN_FRAME))
    return pBuf;
  
  //�б߿�ʱ��ǰ���"��"
  *pBuf++= 0xa9;
  *pBuf++= 0xa6;
  return pBuf;
}

//---------------------���ý���������------------------------*
//�˺����������ý���(һ�㱻ѡ���ý���)
void TWidget_SetFocus(TWidget_t *pWidget,
                      TItemSize_t y,     //����ռ�����λ��
                      TItemSize_t x,     //����ռ����λ��,>��ռ�ʱ������ʾ����
                      TItemSize_t w)     //������,0��ʾx���������ȫΪ����
{

  
  unsigned char Flag;
  unsigned char VPageSize;
  TWin_t *pWin = TWM_pGetWin(pWidget->hWin);
  
  TItemSize_t ItemW = pWidget->ItemsRect.w; 
  //������ʾ��Χ,���Բ�����
  if(x > ItemW){
    TWin_ClrFocus(pWin);
    return;
  }
  
  Flag = pWidget->Flag;
  pWin = TWM_pGetWin(pWidget->hWin);
  VPageSize = TWidget_GetDispH(pWin,Flag);

  //Y�����
  y = _GetClipY(pWidget,VPageSize,y); 
  if(y == (unsigned char)-1){//���㲻����ʾ��Χ
    TWin_ClrFocus(pWin);
    return;
  }
  //������Χ�Ϸ���
  if(!w) w = ItemW - x;
  else if((w + x) > ItemW) w = ItemW - x;
  
  //X�����
  x = _GetClipX(pWidget,TWidget_GetDispW(pWin,Flag),x,&w);

  if(w) TWin_SetFocus(pWin,x + TWidget_GetDispX(pWin,pWidget->Flag),
                       y + TWidget_GetDispY(pWin,pWidget->Flag),w);
  else TWin_ClrFocus(pWin); //�ڼ���������,������ʾ
}

//---------------���ڼ�����������������Ⱥ���------------------------
//����UCGUI->Scollbar.c
unsigned short _DivideRound(unsigned short a, unsigned short b)
{
  unsigned short r = 0;
  if (b) {
    r = ((a + b/2) / b);
  }
  return r;
}

//---------------------��ˮƽ����������------------------------
//ʾ��:���߿�:"������������������������������"
//�����߿�:"  ��������������������������  "
static void _FullHScoll(TWin_t *pWin,
                        unsigned char Flag,   //��־
                        TItemSize_t ItemW,  //��ڿ��
                        TItemSize_t ItemX,  //���λ��
                        TItemSize_t DispW)  //��ʾ���ڿ��,>=10
{
  //�����һ�л�ͼ
  TItemSize_t ThumbSize,Thumb,Moveable,i;
  char *pBuf = TWin_pGetString(pWin,TWin_GetH(pWin) - 1);
  //�����੸��
  if(Flag & TWIDGET_EN_FRAME) {*pBuf++= 0xa9; *pBuf++= 0xb8;} //��"��"
  *pBuf++= 0xa1; *pBuf++= 0xfb;//��"��"

  //�õ��������ѱ��᳤�������ɹ�������
  DispW >>= 1;  //ˮƽ�������������ֿ���һ����
  
  Moveable = DispW - 2; //�õ��ɹ���������
    
  if(ItemW > DispW){  //��ҳ��ʾʱ
    //ˮƽ�������������ֿ���һ����
    ItemW >>= 1; ItemX >>= 1;
    ThumbSize = _DivideRound(Moveable * DispW,ItemW);//�����ѱ��᳤��
    if(!ThumbSize) ThumbSize = 1;
    Thumb = Moveable - ThumbSize;//�õ����ɹ��������С
    Thumb = _DivideRound(Thumb * ItemX,ItemW - DispW);//�õ���ʼλ��
  }
  else{//��ҳ��ʾʱ,����ʾ��ȫ
    ThumbSize = Moveable;
    Thumb = 0;
  }
  //������������
  for(i = 0; i < Thumb; i++) {*pBuf++= 0xa9; *pBuf++= 0xa4;} //��"��"
  //�������ѱ���
  for(i = 0; i < ThumbSize; i++) {*pBuf++= 0xa9; *pBuf++= 0xa5;} //��"��"
  //����ҹ�������
  Thumb += ThumbSize;
  if(Moveable > Thumb){
    Thumb = Moveable - Thumb;
    for(i = 0; i < Thumb; i++) {*pBuf++= 0xa9; *pBuf++= 0xa4;} //��"��"
  }
  //����Ҳ����
  *pBuf++= 0xa1; *pBuf++= 0xfa;//��"��"
  if(Flag & TWIDGET_EN_FRAME) {*pBuf++= 0xa9; *pBuf++= 0xbc;} //��"��"
}

//-------------����ˮƽ���������±߿���------------------------
//ʾ��:����,��������: "����������������������������
static void _FullHFrame(TWin_t *pWin,
                        TItemSize_t ItemW,  //��ڿ��
                        TItemSize_t ItemX,  //���λ��
                        TItemSize_t DispW)  //��ʾ���ڿ��,>=10
{
  //�����һ�л�ͼ
  TItemSize_t i,w;
  char *pBuf = TWin_pGetString(pWin,TWin_GetH(pWin) - 1);
  
  *pBuf++= 0xa9; *pBuf++= 0xb8; //��"��"

  if(!ItemX){*pBuf++= 0xa9; *pBuf++= 0xa5;} //��"��"��ʾ���������
  else{*pBuf++= 0xa1; *pBuf++= 0xfb;}//��"��"��ʾ���������

  w = DispW >> 1;
  for(i = (4/2); i < w; i++) {*pBuf++= 0xa9; *pBuf++= 0xa5;} //��"��"

  if((ItemX + DispW) >= ItemW){//��"��"��ʾ�Ҳ�������
    *pBuf++= 0xa9; *pBuf++= 0xa5;} 
  else{*pBuf++= 0xa1; *pBuf++= 0xfa;}//��"��"��ʾ�Ҳ�������

  *pBuf++= 0xa9; *pBuf++= 0xbc; //��"��"
}

//-------------ˮƽ�޹������ޱ߿�ʱ����ʾ����------------------------*
//ʾ��:����,��������: "������5. ��������    ����  "
static void _FullHHint(TWin_t *pWin,
                        TItemSize_t ItemW,  //��ڿ��
                        TItemSize_t ItemX,  //���λ��
                        TItemSize_t DispW)  //��ʾ���ڿ��,>=10
{
  //�����һ�л�ͼ(��ʱӦ��ʾ�û�����)
  char *pBuf;
  if(ItemW <= DispW) return;//����,����ʾ

  pBuf = TWin_pGetString(pWin,TWin_GetH(pWin) - 1);
  //pBuf += TWidget_GetDispX(pWin,Flag);//�б߿�Ͳ�������ʾ��

  if(!ItemX){*pBuf= 0xa9; *(pBuf + 1)= 0xc0;} //��"��"��ʾ���������
  else{*pBuf= 0xa1; *(pBuf + 1)= 0xfb;}//��"��"��ʾ���������

  pBuf += DispW - 4;
  if((ItemX + DispW) >= ItemW){//��"  ��"��ʾ�Ҳ�������
    *pBuf++= 0xa9; *pBuf= 0xc8;} 
  else{*pBuf++= 0xa1; *pBuf= 0xfa;}//��"��"��ʾ���������
  
}

//---------------------����ֱ����������------------------------
//ʾ��:��ˮƽ��������ʱ��ת90����ͬ
static void _FullVScoll(TWin_t *pWin,
                        unsigned char Flag,   //��־
                        TItemSize_t ItemH,  //��ڸ߶�
                        TItemSize_t ItemY,  //���λ��
                        TItemSize_t DispH)  //��ʾ���ڸ߶�,>=3
{
  TItemSize_t ThumbSize,Thumb,Moveable,i;
  unsigned char y;//��ǰ����λ��,�����������
  char *pBuf;
  
  if(DispH < 3) return;//�ϲ������,����
  if(Flag & (TWIDGET_EN_FRAME | TWIDGET_EN_HEADER)) y = 1;
  else y = 0;
  
  pBuf = TWin_pGetString(pWin,y++);
  pBuf += pWin->Rect.w - 2;//�����һ�л�
  if(DispH < 2){//ֻ��һ��ʱֻ��"��"
    *pBuf++= 0xa9; *pBuf= 0xa7;//��"��"
    return;
  }
  *pBuf++= 0xa1; *pBuf = 0xfc;//���ϲ�"��"
  
  //�õ��������ѱ��᳤�������ɹ�������
  Moveable = DispH - 2; //�ɹ���������
  if(ItemH > DispH){  //��ҳ��ʾʱ
    ThumbSize = _DivideRound(Moveable * DispH,ItemH);//�����ѱ��᳤��
    if(!ThumbSize) ThumbSize = 1;
    Thumb = Moveable - ThumbSize;//�õ����ɹ��������С
    Thumb = _DivideRound(Thumb * ItemY,ItemH - DispH);//�õ���ʼλ��
  }
  else{//��ҳ��ʾʱ,����ʾ��ȫ
    ThumbSize = Moveable;
    Thumb = 0;
  }
  //����Ϲ�������
  for(i = 0; i < Thumb; i++){
    pBuf = TWin_pGetString(pWin,y++);
    pBuf += pWin->Rect.w - 2;//�����һ�л�
    *pBuf++= 0xa9; *pBuf= 0xa6;;//��"��"
  }
  //�������ѱ���
  for(i = 0; i < ThumbSize; i++) {
    pBuf = TWin_pGetString(pWin,y++);
    pBuf += pWin->Rect.w - 2;//�����һ�л�
    *pBuf++= 0xa9; *pBuf= 0xa7;//��"��"
  } 
  //����¹�������
  Thumb += ThumbSize;
  if(Moveable > Thumb){
    Thumb = Moveable - Thumb;
    for(i = 0; i < Thumb; i++){
      pBuf = TWin_pGetString(pWin,y++);
      pBuf += pWin->Rect.w - 2;//�����һ�л�
      *pBuf++= 0xa9; *pBuf= 0xa6;//��"��"
    } 
  }
  pBuf = TWin_pGetString(pWin,y);
  pBuf += pWin->Rect.w - 2;//�����һ�л�
  *pBuf++= 0xa1; *pBuf= 0xfd;//��"��"
  
}

//-------------���޴�ֱ���������ұ߿���------------------------
//ʾ��:�뻭��ˮƽ���������±߿���ʱ��ת90����ͬ
static void _FullVFrame(TWin_t *pWin,
                        unsigned char Flag,   //��־
                        TItemSize_t ItemH,  //��ڸ߶�
                        TItemSize_t ItemY,  //���λ��
                        TItemSize_t DispH)  //��ʾ���ڸ߶�,>=2
{
  TItemSize_t i;
  unsigned char y;//��ǰ����λ��,�����������
  char *pBuf;
  
  if(Flag & TWIDGET_EN_FRAME) y = 1;
  else y = 0;
  
  pBuf = TWin_pGetString(pWin,y++);
  pBuf += pWin->Rect.w - 2;//�����һ�л�
  if(!ItemY){*pBuf++= 0xa9; *pBuf++= 0xa7;} //��"��"��ʾ�ϲ�������
  else{*pBuf++= 0xa1; *pBuf++= 0xfc;}//��"��"��ʾ�ϲ�������

  for(i = 2; i < DispH; i++){
    pBuf = TWin_pGetString(pWin,y++);
    pBuf += pWin->Rect.w - 2;//�����һ�л�
    *pBuf++= 0xa9; *pBuf= 0xa7;//��"��"
  }
  
  pBuf = TWin_pGetString(pWin,y++);
  pBuf += pWin->Rect.w - 2;//�����һ�л�
  if((ItemY + DispH) >= ItemH){//��"��"��ʾ�²�������
    *pBuf++= 0xa9; *pBuf++= 0xa7;} 
  else{*pBuf++= 0xa1; *pBuf++= 0xfd;}//��"��"��ʾ�Ҳ�������
}


//-------------��ֱ�޹������ޱ߿�ʱ����ʾ����------------------------*
//ʾ��:��ˮƽ�޹������ޱ߿�ʱ��ʱ��ת90����ͬ
static void _FullVHint(TWin_t *pWin,
                        unsigned char Flag, 
                        TItemSize_t ItemH,  //��ڸ߶�
                        TItemSize_t ItemY,  //���λ��
                        TItemSize_t DispH)  //��ʾ���ڸ߶�,>=2
{
  char *pBuf;
  TItemSize_t DispW =  TWidget_GetDispW(pWin,Flag);
  if(ItemH <= DispH) return;//����,����ʾ
  pBuf = TWin_pGetString(pWin,TWidget_GetDispY(pWin,Flag)) + DispW - 2;

  if(!ItemY){*pBuf= 0xa9; *(pBuf + 1)= 0xd0;} //��"��"��ʾ�ϲ�������
  else{*pBuf= 0xa1; *(pBuf + 1) = 0xfc;}//��"��"��ʾ�ϲ�������
  

  pBuf = TWin_pGetString(pWin, 
                  TWidget_GetDispY(pWin,Flag) + DispH - 1) + DispW - 2;
  if((ItemY + DispH) >= ItemH){//��"��"��ʾ�²�������
    *pBuf++= 0xa9; *pBuf++= 0xd8;} 
  else{*pBuf++= 0xa1; *pBuf++= 0xfd;}//��"��"��ʾ�Ҳ�������
}

//---------------------������ͼ����---------------------------*?
//�˺��������Ҳ����²�߿�
void TWidget_PaintEnd(TWidget_t *pWidget,
                      unsigned char PaintFlag,//����Ļ�ͼ��־
                      TItemSize_t LastW)  //���һ�����ݳ���
{
  TItemSize_t ItemLen,ItemPos;//��߻��,x���y��
  TItemSize_t DispLen;//��ʾ���߻��
  unsigned char Flag;
  TWin_t *pWin;
  char *pBuf;
  
  //============��ͳ���������=========
  ItemLen = pWidget->ItemsRect.w;
  if(LastW > ItemLen){
    ItemLen = LastW;
    pWidget->ItemsRect.w = ItemLen;
  }

  Flag = pWidget->Flag;
  pWin = TWM_pGetWin(pWidget->hWin);

  //����ռ�����С����ʾ����,Ӱ�������ʱ�������������
  DispLen = TWidget_GetDispH(pWin,Flag);
  ItemLen = TWidget_GetDispY(pWin,Flag); //��������ʾ��x
  ItemPos = pWidget->ItemsRect.h - pWidget->ItemsRect.y;
  if(PaintFlag & (TGUI_PAINT_VCONTEXT | TGUI_PAINT_DATA)){
    for(; ItemPos < DispLen; ItemPos++){
      pBuf = TWin_pGetString(pWin,ItemPos + ItemLen);
      memset(pBuf, ' ',TWin_GetW(pWin));
      if(Flag & TWIDGET_EN_FRAME){//������߿��б߿�ʱ��ǰ����"��
        *pBuf++= 0xa9;
        *pBuf = 0xa6;
      }
    } 
  }
  
  //�ޱ߿���ˮƽ������ʱ,��ˮƽ��ʾ���������һ������,��ǿ���ػ���ʾ
  if(!(Flag & (TWIDGET_EN_FRAME | TWIDGET_EN_HSCOLL)))
    PaintFlag |= TGUI_PAINT_HCONTEXT;
  if(!(Flag & (TWIDGET_EN_FRAME | TWIDGET_EN_VSCOLL)))
    PaintFlag |= TGUI_PAINT_VCONTEXT;
  

  
  //==============���ˮƽ�߿�===========
  if(PaintFlag & TGUI_PAINT_HCONTEXT){
    ItemPos = pWidget->ItemsRect.x;         //��Xλ��  
    DispLen = TWidget_GetDispW(pWin,Flag);//��ʾ����
    if(Flag & TWIDGET_EN_HSCOLL)//��ˮƽ������
      _FullHScoll(pWin,Flag,ItemLen,ItemPos,DispLen);
    else if(Flag & TWIDGET_EN_FRAME)//��ˮƽ���������±߿�
      _FullHFrame(pWin,ItemLen,ItemPos,DispLen);
    else if(Flag & TWIDGET_EN_NOTIFY)  //�޹������ޱ߿�ʱ,�û��ռ���ʾ
      _FullHHint(pWin,ItemLen,ItemPos,DispLen);
  }
  //==============����Ҳ�߿�===========
  if(PaintFlag & TGUI_PAINT_VCONTEXT){
    ItemLen = pWidget->ItemsRect.h; //��߶�
    ItemPos = pWidget->ItemsRect.y; //��Yλ��
    DispLen = TWidget_GetDispH(pWin,Flag);//��ʾ��߶�  
    if(Flag & TWIDGET_EN_VSCOLL)//����ֱ������
      _FullVScoll(pWin,Flag,ItemLen,ItemPos,DispLen);
    else if(Flag & TWIDGET_EN_FRAME)//���ұ߿�
      _FullVFrame(pWin,Flag,ItemLen,ItemPos,DispLen);
    else if(Flag & TWIDGET_EN_NOTIFY)  //�޹������ޱ߿�ʱ,�û��ռ���ʾ
      _FullVHint(pWin,Flag,ItemLen,ItemPos,DispLen);
  }
}


