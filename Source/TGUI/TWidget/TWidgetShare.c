/*******************************************************************************

              ��TWidgetΪ�����Ӧ����ع������ӿ�ʵ��

*******************************************************************************/
#include "TWidgetShare.h"
#include <string.h>

//----------------------�õ��������2�����ʼλ�ú���---------------------------
//��TWidget_tΪ����Ŀؼ���,����������:"   [��  ��]    [�� ��] "�Ľ�����ʾ
static TItemSize_t _Append2_GetX(//0x80:�ڸ��������־,0x01:�������־,0x02:�����ر�־
                                 unsigned char Flag,
                                 unsigned char Len1st,  //���ֳ���
                                 unsigned char Len2nd,  //���ֳ���
                                 unsigned char Space,   //��������ʱ,����
                                 TItemSize_t ItemLen)    //���ܳ�
                  
{
  unsigned char Len;
  //�õ�����λ��Pos
  if(Flag  & 0x01) Len = Len1st;
  else Len = 0;
  if(Flag  & 0x02) Len += Len2nd;
  if(Flag == 0x03) 
    Len += Space;//������ťʱ
  if(ItemLen < Len) return 0;//������ʾ
  return TGetAlignLenR((ItemLen - Len) >> 1,0); //����
}



//--------------------------�������2��ķ��Իص�ʵ�ֺ���-----------------------
//��TWidget_tΪ����Ŀؼ���,����������:"   [��  ��]    [�� ��] "�Ľ�����ʾ
void Append2_AutiNotify(TWidget_t *pWidget,//����
                           //0x80:�ڸ��������־,0x01:�������־,0x02:�����ر�־
                           unsigned char Flag,
                           unsigned char InReturn,//������ťʱ�����ڷ����ϱ�־
                           unsigned char Len1st,  //���ֳ���
                           unsigned char Len2nd)  //���ֳ���
{
  TItemSize_t Pos;
  unsigned char Len,Space;
  if(!(Flag & 0x80)) return; //������,Ҳ���ܵ���
  Flag &=  0x03;
  
  //�õ�����λ��Pos
  Space = cbGetBottomSpace(TWidget_GetW(pWidget));
  //�̶��Դ��ڿ�Ⱦ��ж��룬����ʾ���ڴ�������ʾ����Ϊ׼
  Pos = TWin_GetW(TWM_pGetWin(pWidget->hWin));
  if(pWidget->ItemsRect.w > Pos) Pos = pWidget->ItemsRect.w;
  Pos = _Append2_GetX(Flag, Len1st, Len2nd, Space, Pos);
  if(Pos == 0) Space = 0;//ǿ��ȥ���Ի���㹻���
  //�õ����㳤��
  if(Flag == 0x03){//ͬʱ������ʱ
    if(InReturn){//�����ڷ�����
      Pos += Len1st + Space;
      Len = Len2nd;
    }
    else Len = Len1st;
  }
  else if(Flag == 0x01) //�����水ť
    Len = Len1st;
  else //���ذ�ť
    Len = Len2nd;
  //�ý���
  TWidget_SetFocus(pWidget, TWidget_GetSel(pWidget), Pos, Len);
}

//----------------���ܸ����������������ĵõ��ַ�����ʵ��--------------------------
//��TWidget_tΪ�����,���һ�������������߽���ĸ߼��ؼ��ɵ��ô˺������й���
//���ؽ������Ǹ���ť��
const char* Append2_GetString(TWidget_t *pWidget,//����
                              //0x01:�׸���ť��־,0x02:�ڶ�����ť��־
                              unsigned char Flag,
                              const char *p1st,
                              const char *p2nd)
{
  unsigned char Space;
  unsigned char Pos;
  char *pBuf;
  //�õ�����λ��Pos
  Space = cbGetBottomSpace(TWidget_GetW(pWidget));
  //�̶��Դ��ڿ�Ⱦ��ж��룬����ʾ���ڴ�������ʾ����Ϊ׼
  Pos = TWin_GetW(TWM_pGetWin(pWidget->hWin));
  if(pWidget->ItemsRect.w > Pos) Pos = pWidget->ItemsRect.w;
  Pos = _Append2_GetX(Flag,strlen(p1st),strlen(p2nd),Space, Pos);
  if(Pos == 0) Space = 0;//ǿ��ȥ���Ի���㹻���
  
  pBuf = TGUI_pGetBuf();       //ʹ�ù�����
  memset(pBuf,' ', Pos); pBuf += Pos;   //ǰ�����ո�
  if(Flag & 0x01) pBuf = Tstrcpy_ex(pBuf,p1st);//�������
  if(Flag == 0x03){//������ո�
    memset(pBuf,' ', Space);  
    pBuf += Space;
  }
  if(Flag & 0x02)pBuf = Tstrcpy_ex(pBuf,p2nd);//ĩ�����
  
  //β�ո����
  if((TGUI_pGetBuf() + TWidget_GetW(pWidget)) > pBuf){
    Space = TGUI_pGetBuf() + TWidget_GetW(pWidget) - pBuf;
    memset(pBuf,' ', Space);
    pBuf += Space;  
  }
  *pBuf = '\0'; //�����ַ�
  
  return TGUI_pGetBuf();
}

//----------------���ܸ����������������ļ�������ʵ��--------------------------
//��TWidget_tΪ�����,���һ�������������߽���ĸ߼��ؼ��ɵ��ô˺������й���
//����-1����ִ�б�׼�������������ٵ��ô˺���
//��������0:�����ڵ�һ����ť��
//1:�����ڵڶ�����ť��
//2:����δ�����仯
//���򷵻ؽ������Ǹ���ť��,
unsigned char Append2_Key(TWidget_t *pWidget,//����,
                          unsigned char Key,//��ֵ
                          //0x01:�׸���ť��־,0x02:�ڶ�����ť��־
                          unsigned char Flag,
                          //-1:�ٴν���˺���
                          //����:��ǰ�����ڵڶ�����ť�ϱ�־
                          unsigned char In2)
{
  TItemSize_t MaxItem = TWidget_GetItems(pWidget) - 1;
  static TItemSize_t Sel;//�ϴ�ѡ��

  if(In2 == (unsigned char)-1){ //�ٴν���
    //��ѡ��δ�����仯ʱ
    if(Sel == TWidget_GetSel(pWidget))
      return 2;
    Sel = TWidget_GetSel(pWidget);
    //û��������,������
    if(Sel != MaxItem) return 2;
    //�����ط���������ʱ,ֻ�а��¼����ܵ��������
    if(Key == TGUI_KEY_DOWN) 
      return 0;
    else 
      return 1;
  }

  //�״ν���:
  Sel = TWidget_GetSel(pWidget);//��ס�ϴ�ѡ��
  //һ����ť���޸���ʱ,�������¼�ʱ,�ײ�ֱ�ӾͿ��Ը㶨��
  if(Flag != 0x03) 
    return (unsigned char)-1;

  //�������һ��ʱ,�ɵײ�㶨
  if(TWidget_GetSel(pWidget) != MaxItem)
    return (unsigned char)-1;

  //���һ��ʱ,���Ҽ��������¼�����
  if(Key == TGUI_KEY_LEFT) Key = TGUI_KEY_UP;
  else if(Key == TGUI_KEY_RIGHT) Key = TGUI_KEY_DOWN;
  //������ť�������¼�,�ɵײ�㶨
  if((Key != TGUI_KEY_UP) && (Key != TGUI_KEY_DOWN))
    return (unsigned char)-1;
  
  //������ť���ϼ����¼�
  //�����������һ��,������ťʱ���ϼ����¼��Ĵ���:

  //�����ڵڶ�����ť��,���ϰ�����,����ת������
  if(In2 && (Key == TGUI_KEY_UP))
    In2 = 0;
  //�����ڱ���,���°�����,����ת������
  else if((!In2) && (Key == TGUI_KEY_DOWN))
    In2 = 1;
  else return (unsigned char)-1; //�ײ㴦��

  return In2;
}
