/*
           pyInput.c - ƴ�����뷨ģ��

           ԭ��:�� ��
           ��д:�� ǿ(mail2li@21cn.com)
           �ظ�д��dap_ccj
*/

#ifndef __PINYIN_IME_H 
#define __PINYIN_IME_H

struct _PyIndex
{
    const unsigned char *Py;      //ƴ��ƴд
    const unsigned char *Pymb;    //��Ӧ����
};

//���ּ�ƴ�����뷨�ṹ
typedef struct _PinYinIME{
  unsigned char NumCount;             //��ֵ�����������,����ֵ���1
  unsigned char CurPos;              //��ǰ����λ��
  unsigned char PerFindCount[6];      //ÿ�����������ָ���,ƴ�����6����ĸ
  const struct _PyIndex *pPyTblIndex[24];      //�����ҵ����ַ�������λ��
}PinYinIME_t;

//------------------------------������Ӧ����--------------------------------
//����ʱ���ô˺���
//�����ҵ�����ĸ��
//�����ؿ�,��ʾ�˴�û�ҵ�
//ע:����ֻ��Ӧ��ֵΪ: ' ':�˸�� '2'-'9'���ּ�,����������Ӧ
const struct _PyIndex **NumKeyPyIme_pKey(PinYinIME_t *pPy,
                                         unsigned char *pFindCount,//�����ҵ��Ĺ���      
                                         unsigned char Num);   //����İ���ֵ,��Ӧ2-9,�˸��

//------------------------------�õ���ǰ��ĸ��--------------------------------
//�����ҵ�����ĸ��
const struct _PyIndex **NumKeyPyIme_pGetCur(const PinYinIME_t *pPy);

//------------------------------�õ���ǰ��ĸ������--------------------------------
unsigned char NumKeyPyIme_GetCurCount(const PinYinIME_t *pPy);


#endif

