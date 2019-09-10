/*******************************************************************************

                           TEdit���ڲ���ز��ֽӿ�
��ģ�����:TWIN(����)
*******************************************************************************/
#ifndef __T_EDIT_PRV_H 
#define __T_EDIT_PRV_H

#include "TGUIMath.h"

//���ݵ������͵Ĺ����Բ�����,����ʵ��һ�����ĵ�������ʾ������ɳ�ʼ����,������ɴ˺���
struct _TEditFun{
  void(*UpdateBuf)(const struct _TEdit*);//���ݵ������͸������ݵ�������
  void(*Key)(struct _TEdit*,unsigned char Key);//����������
};

//---------------------�õ�ָ��λ��λ���ַ�����----------------
char TEdit_GetChar(const struct _TEdit *pEdit,unsigned char Pos);

//-----------------------�õ��༭λ���ַ�������-------------------
//�˺������������ַ�������ģʽ
const char* TEdit_pGetString(const struct _TEdit *pEdit);

//-----------------ʮ���Ƶ���ʱ���µ�������������------------------
void TEditDec_UpdateBuf(const struct _TEdit *pEdit);

//-------------------ʮ���Ƶ���ʱ����������--------------------
void TEditDec_KeyPro(struct _TEdit *pEdit,unsigned char Key);


#endif

