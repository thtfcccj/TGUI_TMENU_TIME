/*******************************************************************************

                           �˵��༭��ʱ������ʵ��

*******************************************************************************/

#include "TMenuBuf.h"
#include <string.h>

struct _TMenuBuf TMenuBuf;

//-----------------------------ͷ��������ұ���--------------------------------
//��ԭ�в��ұ�����Ѵ��ڵ�TMenuBuf.Lut�У�������TMenuBuf.Lut
const unsigned char *TMenuBuf_pLutInsertH(unsigned char NewCount,//.Lut���д�С
                                           const unsigned char *pLut)//ԭ��
{
  if(NewCount == 0) return pLut;//û�в���
  memcpy(&TMenuBuf.Lut[1 + NewCount], pLut + 1, *pLut);
  TMenuBuf.Lut[0] = NewCount + *pLut;//��������
  return TMenuBuf.Lut;
}