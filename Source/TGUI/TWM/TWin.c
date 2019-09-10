/*******************************************************************************

                           TGUI�������ʵ��
TGUI���������������ؼ��Ļ��ര��
*******************************************************************************/
#include "TWin.h"
#include <string.h>

/**********************************************************************
                          ��Ϊ����ʵ��
**********************************************************************/

//-------------------------��ʼ�����ں���----------------------------
//����ʱ���ô˺�����ʼ��
void TWin_InitWin(TWin_t *pWin,       //���ʼ���Ĵ���
                  signed char w,    //ԭʼ���
                  signed char h,    //ԭʼ�߶�
                  char *pBuf,//������
                  unsigned short BufSize)//��������С
{
  memset(pWin,0,sizeof(TWin_t));
  pWin->Rect.w = w;
  pWin->Rect.h = h;
  pWin->pBuf = pBuf;
  memset(pBuf,' ',sizeof(TWin_t));  //��ʼ��Ϊ��
  pWin->BufSize = BufSize;
}

//-------------------------���ñ�־����----------------------------
static void _SetFlag(TWin_t *pWin,unsigned char Flag)
{
  pWin->Flag = Flag; 
  TWin_cbUpdateNotify(pWin->Rect.x,
                      pWin->Rect.y,
                      pWin->Rect.w,
                      pWin->Rect.h);//��Ҫ����������ʾ��
}



//-------------------------�������غ���----------------------------
void TWin_Hidden(TWin_t *pWin)
{
  _SetFlag(pWin,pWin->Flag | TWIN_HIDDEN); 
}

//-------------------------������ʾ����----------------------------
void TWin_Disp(TWin_t *pWin)
{
  _SetFlag(pWin,pWin->Flag & (~TWIN_HIDDEN)); 
}

//-------------------------�����ƶ�����----------------------------
//�����ƶ�,ʹ�þ�������
void TWin_Move(TWin_t *pWin, signed char x, signed char y)
{
  signed char PrvX = pWin->Rect.x;  
  signed char PrvY = pWin->Rect.y;
  pWin->Rect.x = x;
  pWin->Rect.y = y;
  //��Ҫ����ǰ��������ʾ��
  TWin_cbUpdateNotify(x,y,pWin->Rect.w,pWin->Rect.h);
  TWin_cbUpdateNotify(PrvX,PrvY,pWin->Rect.w,pWin->Rect.h);
}

//-------------------------���ڸı��С����----------------------------
void TWin_ReSize(TWin_t *pWin, signed char w, signed char h)
{
  signed char PrvW = pWin->Rect.w;  
  signed char PrvH = pWin->Rect.h;
  pWin->Rect.w = w;
  pWin->Rect.h = h;
  //��Ҫ����ǰ��������ʾ��
  TWin_cbUpdateNotify(pWin->Rect.x,pWin->Rect.y,w,h);
  TWin_cbUpdateNotify(pWin->Rect.x,pWin->Rect.y,PrvW,PrvH);
}

//-------------------------�õ�ָ���л���������----------------------------
//�˺�������ֱ�Ӹ����л���������,ע����³���С�ڴ�����
char *TWin_pGetString(TWin_t *pWin,
                               signed char y)//�ڴ����ڵ����������
{
  if(y >= pWin->Rect.h) return  NULL;//����������ʾ��
  //ͨ���������ػ�
  TWin_cbUpdateNotify(pWin->Rect.x,pWin->Rect.y + y,
                      pWin->Rect.w,1);      
  return  pWin->pBuf + pWin->Rect.w * y;
}

//-------------------------��õ�ָ��λ���ַ�����----------------------------
//x,yΪ�����ڵ��������
char TWin_GetChar(TWin_t *pWin,signed char x,signed char y)            
{
 return *(pWin->pBuf + pWin->Rect.w * y + x);
}


//-------------------------����һ���ַ�����----------------------------
//���ַ�������ʱ,���潫��Ϊ0,���ַ�������ʱ,���潫������ʾ
void TWin_SetString(TWin_t *pWin, 
                    signed char y,              //�ڴ����ڵ����������
                    const char *pString) //�ַ���
{
  signed char w;
  unsigned char String;
  char *pBuf;
  char *pEndBuf;
  if(y >= pWin->Rect.h) return; //���ڴ�����
  
  w = pWin->Rect.w;
  pBuf = pWin->pBuf;
  pEndBuf = pBuf + pWin->BufSize;
  pBuf += y * pWin->Rect.w;//��ǰ��ʼλ��
  if(pBuf >= pEndBuf) return;   //������!
  if((pBuf + w) < pEndBuf) pEndBuf = pBuf + w;
  //else //��ֹ�ڴ泬��
  //�������
  for(; pBuf < pEndBuf; pBuf++){
    String = *pString++;
    if(String < 0x20) break; //������
    *pBuf = String;
  }
  //����ʱ�������ո�
  for(; pBuf < pEndBuf; pBuf++)
    *pBuf = ' ';
  //ͨ���������ػ�
  TWin_cbUpdateNotify(pWin->Rect.x,pWin->Rect.y + y,
                      pWin->Rect.w,1);  
}

//-------------------------���ý��㺯��----------------------------
//�ƶ����ý���ʱ���ô˺���
//�����Ϊ0ʱ,��ʾȡ�����ڽ�����ʾ
void TWin_SetFocus(TWin_t *pWin,
                   signed char x, //�ڴ����е�xλ��
                   signed char y, //�ڴ����е�yλ��
                   signed char w) //���
{
  TWin_cbUpdateNotify(pWin->Rect.x + pWin->Focus.x,
                      pWin->Rect.y + pWin->Focus.y,
                      pWin->Focus.w,1);//ԭ�������ػ�
  //ע:δ��齹��������Ч��!
  pWin->Focus.x = x;
  pWin->Focus.y = y; 
  pWin->Focus.w = w; 
  TWin_cbUpdateNotify(pWin->Rect.x + x,
                      pWin->Rect.y + y,
                      w,1);//�½������ػ�
}
//-------------------------�ж�ĳ��λ���Ƿ��ڽ����Ϻ���----------------
//���ô˺���ȷ���þ���λ���ڸô�����
//���򷵻�1,���򷵻�0
unsigned char TWin_IsFocus(TWin_t *pWin,
                           signed char x,   //�����������x
                           signed char y)   //�����������y                
{
  signed char WinX;
  if(pWin->Focus.y == y){	//�ڽ�������
    WinX = pWin->Focus.x;
    if((x >= WinX) && (x < (WinX + pWin->Focus.w)))
      return 1;
  }
  return 0;
}

/**********************************************************************
                        ����ģʽ����
**********************************************************************/

//-------------------------�ﴰ����Ϊ����ģʽ����----------------------
void TWin_SetLattice(TWin_t *pWin)
{
  _SetFlag(pWin,pWin->Flag | TWIN_LATTICE); 
}

//-------------------------�ﴰ��ȡ������ģʽ����----------------------
void TWin_ClrLattice(TWin_t *pWin)
{
  _SetFlag(pWin,pWin->Flag & (~TWIN_LATTICE)); 
}

//---------------------------����������������---------------------------
//������������������ʾ,�����û���ͼ
void TWin_ClrLatticeDisp(TWin_t *pWin)
{
  if(pWin->Flag  & TWIN_LATTICE){  //��Ϊ����ģʽ��Ч
    TWin_cbClrNowNotify(pWin->Rect.x,
                        pWin->Rect.y,
                        pWin->Rect.w,
                        pWin->Rect.h);//��Ҫ����������ʾ��
	}
  _SetFlag(pWin,pWin->Flag);//�ػ�������
}
