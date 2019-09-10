/*****************************************************************************
                         TGUI�ϵ����뷨ģ��-�������뷨����
*****************************************************************************/

#include "TImePinYin.h"
#include "TImeBase.h"
#include <string.h>

/*****************************************************************************
                              ��غ���
*****************************************************************************/

//---------------------------��ʼ������---------------------------------
//�л���ƴ�����뷨���ϸ�����������ɺ����
void TImePinYin_Init(struct _TImePinYin *pPinYin) //���뷨�ṹ
{
  memset(pPinYin, 0, sizeof(struct _TImePinYin));//�ȳ�ʼ����0
}

//----------------------���ƴ����ʾ���ַ�������---------------------------
//���ʾ��:"pen ren sen", ������ʾʱ�磺ji jia li liu lin liu l...
static void _FullPinYinChar(const struct _TImePinYin *pPinYin,
                             char *pBuf)              //�������ַ�
{
  const struct _PyIndex *const *ppPyIndex;
  unsigned char FindCount;
  unsigned char PyCount;  
  unsigned char Pos = 0; //ƴ���ڲ�λ��
  char *pEndBuf = pBuf + TIME_PINGYING_DISP_W; //�н���λ��
  
  ppPyIndex = NumKeyPyIme_pGetCur(&pPinYin->Ime) + 
              pPinYin->PinYinStart;//ָ���������������Ч��ʼλ��
  FindCount = NumKeyPyIme_GetCurCount(&pPinYin->Ime) - pPinYin->PinYinStart;
  while(FindCount){//����ƴ��Ҫ��ʾʱ,һ�������ƴ��
    PyCount = strlen((const char*)(*ppPyIndex)->Py);//ƴ��
    if((PyCount + 1) <= (pEndBuf - pBuf)){//����+ƴ�����,ֱ��ƴ����ʾ����ʱΪֹ
      memcpy(pBuf, (*ppPyIndex)->Py, PyCount);
      pBuf += PyCount;   
      if(pBuf < (pEndBuf - 1)) *pBuf++ = ' '; //��λ��ʱ�ո���
      //Ϊ�´�����׼��
      FindCount--;
      ppPyIndex++;
      Pos++;
    }
    else break; //һ����ʾ�����¸�ƴ����
  }
  if(pEndBuf > pBuf){//�������ո�
    memset(pBuf, ' ', pEndBuf - pBuf);
  }  
  if(FindCount){//һҳ��ʾ����ʱ,���...��ʾ(���ܻᱻǿ�ƽض�ƴ��)
    memset(pEndBuf - 3, '.', 3);
  }
}

//-------------------------����̧ͷ�̶���ʾ-----------------------------------
static const char _chIdie1st[]   = {"ƴ��: ���ٰ�����Ϊ�ڶ���"};
static const char _chIdie2nd[]   = {"ȫ�������δ��ʾ����ѡ��"};
static const char _chPinYinSel[] = {"ƴ��: ƴ��ѡ��"};
static const char _chChSel[]     = {"ƴ��: ����ѡ��"};

//-------------------------����û���ʾ(̧ͷ)-----------------------------------
void TImePinYin_FullTitle(const struct _TImePinYin *pPinYin,//���뷨�ṹ
                           char *pStr)
{
  switch(pPinYin->eState){
    case eTImePinYin_Idie:
      if(pPinYin->Timer > (TIME_PINGYING_IDIE_DISP_OV / 2))//������ʾ
        strcpy(pStr, _chIdie1st); 
      else strcpy(pStr, _chIdie2nd); 
      break;
    case eTImePinYin_Input:
      _FullPinYinChar(pPinYin, pStr); 
      break;
    case eTImePinYin_PinYinSel: 
      strcpy(pStr, _chPinYinSel);
      break;
    case eTImePinYin_ChSel:      
      strcpy(pStr, _chChSel);    
      break;
    default: break;
  }
  return;  
}

//-----------------------ƴ����ĸ����ַ���Դ----------------------------
static const char _ch_a2f[] =  {"abc def"};
static const char _ch_g2l[] =  {"ghi jkl"};
static const char _ch_m2s[] =  {"mno pqrs"};
static const char _ch_t2z[] =  {"tuv wxyz"};
static const char _chPinYinFinal[] = {"ƴ�����"};    
static const char *const _IdieAndInput[] = {
  _ch_a2f,        // TIME_MNG_KEY_UP 
  _ch_g2l,        //TIME_MNG_KEY_LEFT
  _chPinYinFinal, //TIME_MNG_KEY_ENTER
  _ch_m2s,       //TIME_MNG_KEY_RIGHT
  _ch_t2z,       //TIME_MNG_KEY_DOWN
};
static const char _chDeletel[] = {"ɾ��"};
static const char _chNextGrp[] = {"��һ��"};

//-------------------------�õ���Ӧ��ֵ�ַ�����-----------------------------------
//�磺�ϼ���Ӧ ����NULL��ʾ����Ҫ�ַ�
const char *TImePinYin_pGetArrowNotice(struct _TImePinYin *pPinYin,//���뷨�ṹ
                                         unsigned char GuideKey)//��Ӧ��ֵλ��
{
  //���������ؼ�
  if(GuideKey == TIME_MNG_KEY_EXIT){
    if(pPinYin->eState == eTImePinYin_Input)//��״̬Ϊɾ������
      return _chDeletel;
    return chReturn;
  }
  
  //ƴ������״̬��ʾ
  if(pPinYin->eState <= eTImePinYin_Input)
    return _IdieAndInput[GuideKey];
  
  //==================ƴ��ѡ��״̬����ѡ��״̬ʱ=============================
  if(GuideKey == TIME_MNG_KEY_ENTER){//��������ȷ�ϼ�Ϊ��һ��
    return _chNextGrp;
  }
  GuideKey = TIme_GuideKeyToNoEnterKey[GuideKey];

  //ƴ��ѡ��״̬��ʾƴ��
  if(pPinYin->eState == eTImePinYin_PinYinSel){
    unsigned char FindCount;
    volatile const struct _PyIndex *const *ppPyIndex;
    ppPyIndex = NumKeyPyIme_pGetCur(&pPinYin->Ime) + 
                 pPinYin->PinYinStart;//ָ���������������Ч��ʼλ��
    FindCount = NumKeyPyIme_GetCurCount(&pPinYin->Ime) - pPinYin->PinYinStart;
    if(GuideKey >= FindCount) return NULL; //ƴ�������ˡ�
    ppPyIndex += GuideKey;
    //unsigned char PyCount = strlen((const char*)(*ppPyIndex)->Py);//ƴ��
    //memcpy(pPinYin->ChBuf, (*ppPyIndex)->Py, PyCount);
    //pPinYin->ChBuf[PyCount] = '\0';
    //return pPinYin->ChBuf;
    return (const char*)(*ppPyIndex)->Py;
  }
  
  //����ѡ��״̬ʱ��ʾ����
  unsigned char ChOffset = GuideKey * 2;//ҳ��ƫҳ
  const char *pCh;
  pCh = (const char *)(*(NumKeyPyIme_pGetCur(&pPinYin->Ime) + 
                        pPinYin->CurPinYin))->Pymb;//ָ������ƴ�����λ��
    
  unsigned char CurLen = strlen((const char*)pCh) - pPinYin->ChStart;//��ǰҳ�ַ�����
  if(CurLen < ChOffset) return NULL; //���ֳ����ˡ�
      
  pCh += (pPinYin->ChStart + ChOffset); //ָ��ǰҳ�еĵ�ǰ��Ҫ���ĺ���
  //���������޽����ַ���ʹ�û�����
  pPinYin->ChBuf[0] = *pCh++;
  pPinYin->ChBuf[1] = *pCh;
  pPinYin->ChBuf[2] = '\0';
  return pPinYin->ChBuf;
}

//-----------------------------������----------------------------------
//128ms����һ��, ���޸���ɣ����ض�Ӧ������ɵ����֣�����0������, 0xffff:ǿ��ˢ��
unsigned short TImePinYin_Task(struct _TImePinYin *pPinYin)
{
  //����״̬����ѭ����ʾ��ʾ��Ϣ
  if(pPinYin->eState == eTImePinYin_Idie){
    if(!pPinYin->Timer){ 
      pPinYin->Timer = TIME_PINGYING_IDIE_DISP_OV;
      return 0xffff;//ǿ��ˢ��
    }
    pPinYin->Timer--;
    if(pPinYin->Timer == (TIME_PINGYING_IDIE_DISP_OV / 2))
      return 0xffff;//ǿ��ˢ��
    return 0;
  }  
  
  //����������״̬
  if(pPinYin->eState != eTImePinYin_Input) return 0;
  if(!pPinYin->Timer) return 0; //������
  pPinYin->Timer--;
  if(pPinYin->Timer) return 0;//δ����
  //û��ѡ��ڶ����ַ�,ȷ��Ϊ��һ��ƴ����ĸ��
  unsigned char Count;
  NumKeyPyIme_pKey(&pPinYin->Ime, &Count, pPinYin->FirstKey * 2 + '2');
  return 0xffff;//ǿ��ˢ��
}

/*****************************************************************************
                        ��ֵ������غ���
*****************************************************************************/

//--------------------------ȷ�ϰ�����Ӧ����-----------------------------------
//���޸���ɣ��򷵻ض�Ӧ�ַ�������0: ���޸ķ���,  0xffff: �ڲ����������� 
static unsigned short _EnterKey(struct _TImePinYin *pPinYin)
{
 switch(pPinYin->eState){
    case eTImePinYin_Idie: //һ��ƴ����û���룬�Ӽ���Ч
      break;
   case eTImePinYin_Input:{ //ƴ��ȷ��
     if(pPinYin->Timer){//�������׸�ʱ,��ʾΪ��һ��
      unsigned char Count;
      NumKeyPyIme_pKey(&pPinYin->Ime, &Count, pPinYin->FirstKey * 2 + '2');
      pPinYin->Timer = 0; //�رն�ʱ��
     }
     pPinYin->eState = eTImePinYin_PinYinSel;//�л���ƴ��ѡ��״̬
     pPinYin->PinYinStart = 0;//��ʼ�����׸�(����Ҫ�򱣳��ϴβ���)
     break;
   }
   case eTImePinYin_PinYinSel:{ //ƴ��ѡ��,�л���һ��
      unsigned char Count = NumKeyPyIme_GetCurCount(&pPinYin->Ime) - 
                             pPinYin->PinYinStart;
      if(Count > 4) pPinYin->PinYinStart += 4; //4��Ϊһ��
      else pPinYin->PinYinStart = 0; //����ػ�
      pPinYin->ChStart = 0; //��λ������ѡ����
      break;    
    }
    case eTImePinYin_ChSel:{ //����ѡ��,�л���һ��
      //�ȵõ�����
      const char *pCh = (const char *)(*(NumKeyPyIme_pGetCur(&pPinYin->Ime) + 
                                       pPinYin->CurPinYin))->Pymb;//ָ������ƴ�����λ��
      unsigned char Count = strlen((const char*)pCh) - pPinYin->ChStart;//��ǰҳ�ַ�����(����)
      if(Count > 8) pPinYin->ChStart += 8; //4��Ϊһ��,����Ϊ��λ
      else pPinYin->ChStart = 0; //����ػ�
      break;    
    }
  }
  return 0xffff; //�ڲ����������� 
}

//--------------------------������Ӧ����-----------------------------------
//���޸���ɣ��򷵻ض�Ӧ�ַ�������0: ���޸ķ���,  0xffff: �ڲ����������� 
unsigned short TImePinYin_Key(struct _TImePinYin *pPinYin, 
                               unsigned char GuideKey)
{
  //���������˳���
  if(GuideKey == TIME_MNG_KEY_EXIT){
    if(pPinYin->eState == eTImePinYin_Idie)
      return 0;
    if(pPinYin->eState == eTImePinYin_Input){//ɾ��
      unsigned char FindCount;
      if(NumKeyPyIme_pKey(&pPinYin->Ime, &FindCount, ' ') == NULL)//�������
        pPinYin->eState = eTImePinYin_Idie;//������
    }
    else pPinYin->eState--; //һ��㷵��
    
    return 0xffff; //�ڲ����������� 
  }
  
  //��������ȷ�ϼ�
  if(GuideKey == TIME_MNG_KEY_ENTER) 
     return _EnterKey(pPinYin);
  
  //����4���Ӽ�ʱ
  GuideKey = TIme_GuideKeyToNoEnterKey[GuideKey];
  unsigned char Offset = GuideKey * 2;//ҳ��ƫ��
  
  //====================����������״̬����=====================================
  if(pPinYin->eState <= eTImePinYin_Input){
    unsigned char NumKey;  //�Ȼ�ü�ֵ,  '2'������NumKeyPyIme_pKey��Ӧ
    if((pPinYin->eState == eTImePinYin_Input) && (pPinYin->Timer)){//�ڶ�������ʱ
      if(pPinYin->FirstKey == GuideKey){//���μ�ֵ��ͬʱ,�ڶ���Ϊ�ڶ���
        NumKey = Offset + 1;
        pPinYin->Timer = 0; //ѡ�����
      }
      else{//���μ���ͬʱ
        NumKey = pPinYin->FirstKey * 2; //��ʾ�׸��ַ���ȷ��
        pPinYin->FirstKey = GuideKey; //ͬʱ������ַ���Ϊ�׸��ַ�
        pPinYin->Timer = TIME_NUM_TIMER_OV;
      }
    }
    else{ //�״�����ʱ,��סѡ�񼴿�
      pPinYin->FirstKey = GuideKey; //ͬʱ������ַ���Ϊ�׸��ַ�
      pPinYin->eState = eTImePinYin_Input; //���е�����״̬
      pPinYin->Timer = TIME_NUM_TIMER_OV;
      return 0xffff; //�ڲ�����������
    }
    NumKeyPyIme_pKey(&pPinYin->Ime, &NumKey, NumKey + '2');
  }
  //===========================ƴ���뺺��ѡ��״̬===============================
  else if(pPinYin->eState == eTImePinYin_PinYinSel){
    unsigned char FindCount = NumKeyPyIme_GetCurCount(&pPinYin->Ime) - pPinYin->PinYinStart;
    if(GuideKey >= FindCount) return 0xffff; //ƴ��������,ѡ����Ч
    pPinYin->CurPinYin = pPinYin->PinYinStart + GuideKey;
    pPinYin->eState = eTImePinYin_ChSel; //������ѡ��״̬
    pPinYin->ChStart = 0;//��ʼ�����׸�(����Ҫ�򱣳��ϴκ���ѡ�񲻱�)
  }
  else/* if(pPinYin->eState == eTImePinYin_ChSel)*/{//����ѡ��״̬ѡ���ַ��ؽ���
    const char *pCh = (const char *)(*(NumKeyPyIme_pGetCur(&pPinYin->Ime) + 
                          pPinYin->CurPinYin))->Pymb;//ָ������ƴ�����λ��
    unsigned char CurLen = strlen((const char*)pCh) - pPinYin->ChStart;//��ǰҳ�ַ�����
    if(CurLen <= Offset) return 0xffff; //���ֳ�����,ѡ����Ч
    pCh += (pPinYin->ChStart + Offset); //ָ��ǰҳ�еĵ�ǰ��Ҫ���ĺ���
    unsigned short Ch = ((unsigned short)*pCh << 8); //��λ��ǰ
    Ch |= *(pCh + 1);
    pPinYin->eState = eTImePinYin_Idie; //������ѡ��״̬
    return Ch;
  }
  return 0xffff; //�ڲ�����������  
}


