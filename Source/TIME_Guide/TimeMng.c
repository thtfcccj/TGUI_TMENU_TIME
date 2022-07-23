/*****************************************************************************
                         TGUI�ϵ����뷨ģ����ʵ��
ע:�ݲ�֧���Դ��ֿ�ȫ�Ƕ���LCD��ʾ������ʾ
*****************************************************************************/

#include "TImeMng.h"
#include "ClipBoard.h"  //���а�
#include <string.h>

/*****************************************************************************
                           �ڲ���������
*****************************************************************************/

//---------------------------�״�ˢ������--------------------------------
//��ʼ������ʱ�ȵ���
static void _Refresh1st(struct _TImeMng *pIme);

//------------------ˢ�´�����ʾ����---------------------------------
//ˢ��pWin���ڵĲ�����Ҫʵʱ���µ�����
static void _Refresh(struct _TImeMng *pIme);

//----------------------------״̬�ı亯��---------------------------------
//ˢ��pWin���ڵĲ�����Ҫʵʱ���µ�����
#define _ChangeState(ime, nextState) do{(ime)->State = nextState; }while(0)

//----------------------��ֵת��Ϊ�ڲ�����ID�ź���-------------------------
//���ص�GuideKey����Ϊ��0��1��2ȷ��3��4��,5�˳���������Ч
static unsigned char _Key2GuideKey(char Key)
{
  //Ϊ�����ֵ���ܵ��޸�(������)���� CASEʵ��
  switch(Key){
    case TGUI_KEY_UP:      return TIME_MNG_KEY_UP;
    case TGUI_KEY_LEFT:    return TIME_MNG_KEY_LEFT;
    case TGUI_KEY_ENTER:   return TIME_MNG_KEY_ENTER;
    case TGUI_KEY_RIGHT:   return TIME_MNG_KEY_RIGHT;
    case TGUI_KEY_DOWN:    return TIME_MNG_KEY_DOWN;
    case TGUI_KEY_ESCAPE:  return TIME_MNG_KEY_EXIT;
  }
  return 0xff; 
}

//----------------------------�༭ģʽ����ʵ��---------------------------------
//����0:���˳�,��0�����˳�
static signed char _EditKey(struct _TImeMng *pIme,
                            unsigned char GuideKey)//������ֵ0-5
{
  switch(GuideKey){
    case TIME_MNG_KEY_UP: //�����,�����뷨ѡ��ģʽ
      _ChangeState(pIme, TIME_MNG_STATE_SEL); break;  
    case TIME_MNG_KEY_LEFT: //���ơ�
      TImeEdit_CursorLeft(&pIme->Edit); break;
    case TIME_MNG_KEY_ENTER: //�񱣴��˳� 
      return 1; 
    case TIME_MNG_KEY_RIGHT: //������
      TImeEdit_CursorRight(&pIme->Edit); break;   
    case TIME_MNG_KEY_DOWN: //ɾ����
      TImeEdit_Clr(&pIme->Edit); break;         
    case TIME_MNG_KEY_EXIT: // ��ֱ���˳�,����ʾ
      _ChangeState(pIme, TIME_MNG_STATE_FORCE_EXIT); break;
    default: break;
  }
  return 0; //0:���˳�
}

//-------------------------ǿ���˳���ʾģʽ����ʵ��-----------------------------
//����0:���˳�, ��0�����˳�
static signed char _ForceExitKey(struct _TImeMng *pIme,
                                 unsigned char GuideKey)//������ֵ0-5
{
  if(GuideKey == TIME_MNG_KEY_ENTER) return -1;//ǿ���˳�
  if(GuideKey == TIME_MNG_KEY_EXIT){//���ر༭ģʽ
    _ChangeState(pIme, TIME_MNG_STATE_EDIT);
    return 0;//���˳�  
  }
  
  //���а����:ճ��
  if(GuideKey == TIME_MNG_KEY_RIGHT){
    const char *pClipBuf = ClipBoard.Buf;
    ClipBoardSizt_t StrLen = strlen(pClipBuf);
    for(; StrLen > 0; StrLen--){
      unsigned short Char = *pClipBuf++;
      if((Char >= 0x80) && (StrLen)){//ȫ��ʱ
        Char <<= 8;
        Char |= *pClipBuf++;
        StrLen--;
      }
      TImeEdit_Add(&pIme->Edit, Char);
    }
    return 0;//���˳�
  }
  
  //���а����:������أ�
  ClipBoardSizt_t Len =  TImeEdit_GetCurLen(&pIme->Edit);  //����
  ClipBoardSizt_t Cursor =  TImeEdit_GetCurCursor(&pIme->Edit);
  const char *pClipBuf = TImeEdit_pGetCurStr(&pIme->Edit);
  if(GuideKey == TIME_MNG_KEY_UP){//�������
    Len = Cursor;
  }
  else if(GuideKey == TIME_MNG_KEY_DOWN){//�����Ҳ�
    Len -= Cursor;
    pClipBuf += Cursor;
  }
    
  if(Len){//���ַ�ʱ
    if(Len > (CLIP_BOARD_BUF_SIZE - 1)) //��ֹ����
      Len = (CLIP_BOARD_BUF_SIZE - 1);
    memcpy(ClipBoard.Buf, pClipBuf, Len);
    ClipBoard.Buf[Len] = '\0';//ǿ�ƽ����ַ�
  }
  return 0;//���˳�  
}

//--------------------------���뷨ѡ��ģʽ����ʵ��------------------------------
static void _ImeSelKey(struct _TImeMng *pIme,
                       unsigned char GuideKey)//������ֵ0-5
{
  //��Ӧ�����δ������ʱ��������Ч
  switch(GuideKey){
    case TIME_MNG_KEY_UP: //A��
      if(!(pIme->TypeMask & (1 << TIME_MNG_TYPE_CAPITAL))) return;
      pIme->Type = TIME_MNG_TYPE_CAPITAL;
      TImeCapital_Init(&pIme->Data.Capital, 1); break;  
    case TIME_MNG_KEY_LEFT: //,.��
      if(!(pIme->TypeMask & (1 << TIME_MNG_TYPE_SIGN))) return;
      pIme->Type = TIME_MNG_TYPE_SIGN;
      TImeSign_Init(&pIme->Data.Sign, pIme->pSignTbl); break;
    case TIME_MNG_KEY_ENTER: //��ƴ��
      if(!(pIme->TypeMask & (1 << TIME_MNG_TYPE_PINYIN))) return;
      pIme->Type = TIME_MNG_TYPE_PINYIN;      
      TImePinYin_Init(&pIme->Data.PinYin); break; 
    case TIME_MNG_KEY_RIGHT: //��0-9
      if(!(pIme->TypeMask & (1 << TIME_MNG_TYPE_NUM))) return;   
      pIme->Type = TIME_MNG_TYPE_NUM;         
      TImeNum_Init(&pIme->Data.Num); break;   
    case TIME_MNG_KEY_DOWN: //a��
      if(!(pIme->TypeMask & (1 << TIME_MNG_TYPE_LOWERCASE))) return; 
      pIme->Type = TIME_MNG_TYPE_LOWERCASE;         
      TImeCapital_Init(&pIme->Data.Capital, 0); break;        
    case TIME_MNG_KEY_EXIT: // �𷵻ر༭ģʽ
      _ChangeState(pIme, TIME_MNG_STATE_EDIT); return;
    default: break;
  }
  
  _ChangeState(pIme, TIME_MNG_STATE_IME);//�����뷨ģʽ��
}

/*****************************************************************************
                           ���⺯��ʵ��
*****************************************************************************/

//----------------------------------��ʼ������---------------------------------
//�����Ƿ�ɹ���ʼ��
signed char TImeMng_Init(struct _TImeMng *pIme,  //��������뷨�ṹ����
                         TWin_t *pWin,             //��ǰ�ҽӵĴ���
                         char *pString,            //�û��Ѵ��ڵ��ַ���,������'\0'��β
                         TIme_String_t Size,     //�ַ��������������С+1
                         unsigned char DefaultType,   //Ĭ�����뷨<4
                         unsigned char TypeMask,      //��ʹ�õ����뷨���� 
                         const char *pSignTbl,        //�ҽӵķ��ű�,Ϊ��ʱʹ��Ĭ��
                         TIme_String_t Cursor)//Ĭ�Ϲ��λ��,-1ʱΪ���
{
  //��鴰���Ƿ���ʾ
  if(TWin_GetW(pWin) < TIME_MNG_DISP_W) return -1;//������ʾ
  if(TWin_GetH(pWin) < TIME_MNG_DISP_H) return -1;//������ʾ
  memset(pIme, 0,sizeof(struct _TImeMng)); 
  pIme->State = TIME_MNG_STATE_EDIT; //�������ģʽ
  
  //������ʾƫ��
  if(TWin_GetW(pWin) > TIME_MNG_DISP_W)
    pIme->DispOffsetX = (TWin_GetW(pWin) - TIME_MNG_DISP_W) / 2;
  if(TWin_GetH(pWin)> TIME_MNG_DISP_H)
    pIme->DispOffsetY = (TWin_GetH(pWin) - TIME_MNG_DISP_H) / 2;
  //��ʼ����Data����ĳ�Ա
  pIme->pWin = pWin;
  TImeEdit_Init(&pIme->Edit,pString ,Size, //��ʼ���༭��  
                TIME_MNG_DISP_W - 4, Cursor);  //ȥ��ǰ�����Ʊ��(ȫ��)
  pIme->pSignTbl = pSignTbl;
  pIme->TypeMask = TypeMask;
  
  //ȥ�����ܲ���Ҫ����ɫ
  TImeMng_cbFullStrColor(0xff,0,0,TWin_GetW(pWin));
  _Refresh1st(pIme); //��ʼ��ʱ����ˢ�¹̶����䲻����
  _Refresh(pIme);    //�䶯�����״�ˢ��
  return 0;
}

//-----------------------��pIme->Typeת��ΪGuideKeyֵ--------------------------
static const unsigned char _TypeGuideKey[] = {
  TIME_MNG_KEY_ENTER,    //ƴ�����뷨
  TIME_MNG_KEY_RIGHT,    //�������뷨
  TIME_MNG_KEY_DOWN,    //Сд��ĸ���뷨
  TIME_MNG_KEY_UP,     //��д��ĸ���뷨
  TIME_MNG_KEY_LEFT,    //�������뷨
};

//----------------------------����������---------------------------------
//����ֵ����Ϊ: 0����״̬,-1:�˳���ֱ���˳�;-2:ȷ�ϼ��˳�
signed char TImeMng_Key(struct _TImeMng *pIme,
                         char Key)          //��ֵ,����Ӧ�������ҽ���6��
{
  unsigned char GuideKey = _Key2GuideKey(Key);
  if(GuideKey >= TIME_MNG_KEY_COUNT) return 0; //�ǵ�����ֵ
  
  unsigned char State = pIme->State;
  if(State == TIME_MNG_STATE_EDIT){//�༭ģʽʱ
    if(_EditKey(pIme,GuideKey)){//�����˳�
      TImeMng_Quit(pIme);
      return -2; 
    }
  }
  else if(State == TIME_MNG_STATE_FORCE_EXIT){//ǿ���˳�����
    if(_ForceExitKey(pIme, GuideKey)){//�˳���ֱ���˳�
      TImeMng_Quit(pIme);
      return -1;
    }
  }
  else if(State == TIME_MNG_STATE_SEL)//���뷨ѡ��ģʽ
    _ImeSelKey(pIme, GuideKey);
  else{
    //===================���뷨ģʽ�ɼ�ֵ��������========================
    //����ģ�鰴������
    unsigned short ResumeString;
    switch(pIme->Type){
      case TIME_MNG_TYPE_CAPITAL: 
      case TIME_MNG_TYPE_LOWERCASE:         
        ResumeString = TImeCapital_Key(&pIme->Data.Capital, GuideKey); break;
      case TIME_MNG_TYPE_NUM:         
        ResumeString = TImeNum_Key(&pIme->Data.Num, GuideKey); break;        
      case TIME_MNG_TYPE_SIGN:         
        ResumeString = TImeSign_Key(&pIme->Data.Sign, GuideKey); break;  
      case TIME_MNG_TYPE_PINYIN:         
        ResumeString = TImePinYin_Key(&pIme->Data.PinYin, GuideKey); break;          
      default:break;
    }
    //���ؽ������
    if(ResumeString == 0)//���޸ķ���ʱ,�����ص���һ��(���뷨ѡ��״̬)
      _ChangeState(pIme, TIME_MNG_STATE_SEL);
    else 
      if(ResumeString == 0xffff){}//�ڲ�δ���
    else{//�����ַ����ʱ
      TImeEdit_Add(&pIme->Edit, ResumeString); //����������ַ�
      if(pIme->Type != TIME_MNG_TYPE_NUM)//����ʱ��������
        _ImeSelKey(pIme, _TypeGuideKey[pIme->Type]); //�ٴν����������
    }
  }
  _Refresh(pIme);      //������ʾ
  return 0;
}

//--------------------------������---------------------------------
//�������뷨ʱ���ô˺���ʵ��ʱ��Ҫ��
void TImeMng_Task(struct _TImeMng *pIme)
{
  unsigned char State = pIme->State;
  if(State != TIME_MNG_STATE_IME) return;
  if(pIme->Type == TIME_MNG_TYPE_SIGN) return; //�������뷨����Ҫ

  unsigned short ResumeString;
  switch(pIme->Type){
      case TIME_MNG_TYPE_CAPITAL: 
      case TIME_MNG_TYPE_LOWERCASE:         
       ResumeString = TImeCapital_Task(&pIme->Data.Capital); 
       break;
      case TIME_MNG_TYPE_NUM:         
       ResumeString = TImeNum_Task(&pIme->Data.Num);     
       break;  
      case TIME_MNG_TYPE_PINYIN:         
       ResumeString = TImePinYin_Task(&pIme->Data.PinYin);     
       break;         
      default:break;
  }
  if(ResumeString == 0xffff)//ǿ�Ƹ�����ʾ
    _Refresh(pIme);   
  else if(ResumeString != 0){//���Զ�ȷ�Ͻ���ˣ���ʾ�����ַ����
    TImeEdit_Add(&pIme->Edit, ResumeString); //����������ַ�
    _ImeSelKey(pIme, _TypeGuideKey[pIme->Type]); //�ٴν����������
    _Refresh(pIme);      //������ʾ
  }
}

//-----------------------------�˳�����---------------------------------
//�û������ַ�ȷ���˳�����ô˺���
void TImeMng_Quit(struct _TImeMng *pIme)
{
  if(pIme->State == TIME_MNG_STATE_IDIE) return; //����ģʽ
  //ȥ�����ӵ���ɫ
  TImeMng_cbFullStrColor(0xfe,
                         pIme->DispOffsetY + 4,   //pWin��y����,4Ϊ��ͷ��ʼ��
                         pIme->DispOffsetX,       //pWin��x����
                         TIME_MNG_DISP_W);       //x����
  pIme->State = TIME_MNG_STATE_IDIE; //������
}

/*****************************************************************************
                           �ڲ�ˢ���������ʵ��
*****************************************************************************/

//---------------------------Ĭ���ַ����------------------------------------
  //ʾ��:
  //  �������������������������������� ����ʼ��
  //  ���������豸| ������������������ | �༭��
  //  �����������������������������������м���
  //  ���༭ģʽ: ������������        | �û���ʾ
  //  �������������� �� �񱣴��˳�    | �ϼ�ͷ��
  //  �����������ơ�������ơ�        | ���Ҽ�ͷ��
  //  ����������ɾ���� ��ֱ���˳�     | �¼�ͷ��
  //  ������������������������������������ĩ��
//��ʼ��
static const char _TopLine[] = \
  {"������������������������������"};
//�м���
static const char _MiddleLine[] = \
  {"������������������������������"};
//�ϼ�ͷ��
static const char _UpArrowLine[] = \
  {"��           ����           ��"};
//���Ҽ�ͷ��
static const char _LeftRightArrowLine[] = \
  {"��         �����           ��"};
//���Ҽ�ͷ��,��Ҫ�ַ��϶�ʱ(�������뷨ʹ��)
static const char _LeftRightArrowLine2[] = \
  {"��          ����            ��"};
//�¼�ͷ��
static const char _DownArrowLne[] = \
  {"��           ����           ��"};
//��ĩ��
static const char _BottomLine[] = \
  {"������������������������������"};
//��ͷ��ʶ���ַ��е�λ��
static const unsigned char _ArrowPosLUT[] = {
  13,17,
  11,17,
  13,17,
};

static const char *const _ArrowLine[] = {
  _UpArrowLine,
  _LeftRightArrowLine,
  _DownArrowLne,
};

//---------------------------�״�ˢ������--------------------------------
//��ʼ������ʱ�ȵ���
static void _Refresh1st(struct _TImeMng *pIme)
{
  TWin_t *pWin = pIme->pWin;  //��ǰ�ҽӵĴ���
  //����(���ո�)
  for(unsigned char Line = 0; Line < TWin_GetH(pWin); Line++){
    memset(TWin_pGetString(pWin, Line),' ',TWin_GetW(pWin));
  }
  unsigned char x = pIme->DispOffsetX;
  unsigned char y = pIme->DispOffsetY;
  //����һ��
  memcpy(TWin_pGetString(pWin, y + 0) + x, _TopLine, TIME_MNG_DISP_W);  
  //��������
  memcpy(TWin_pGetString(pWin, y + 2) + x, _MiddleLine, TIME_MNG_DISP_W);
  //���ڰ���: ��ĩ��
  memcpy(TWin_pGetString(pWin, y + 7) + x, _BottomLine, TIME_MNG_DISP_W);
}

//-------------------------������ұ߽纯��---------------------------------
//���������ʼ�߽�λ��
static char* _pFullVLine(char *pStr)
{
  *pStr++ = 0xa9; //�����
  *pStr++ = 0xa6;//���Ұ�
  memset(pStr, ' ', TIME_MNG_DISP_W - 4);//����м䲿��
  *(pStr + TIME_MNG_DISP_W - 4)= 0xa9; //�����
  *(pStr + TIME_MNG_DISP_W - 3) = 0xa6;//���Ұ�
  return pStr;
}

//------------------------------�༭ģʽ����ַ���Դ--------------------------
static const char _chEditMode[] = {"�༭ģʽ:"};
static const char _chEditUp[] =    {"����"};
static const char _chEditSave[] =  {"�����˳�"};
static const char _chEditLeft[] =  {"����"};
static const char _chEditRight[] = {"����"};
static const char _chEditDown[] =  {"ɾ��"};
static const char _chEditQuit[] =  {"ֱ���˳�"};
static const char *const _EditArrow[] = {
  _chEditUp, _chEditSave,
  _chEditLeft, _chEditRight,
  _chEditDown, _chEditQuit,
};

//------------------------------ǿ���˳���ʾģʽ�ַ���Դ--------------------------
static const char _chFourceExitMode[] = {"���а����:"};// �������˳���?
static const char _chFourceExitEnter[] = {"�������˳�!"};
static const char _chFourceExitReturn[] =  {"���ؼ���"};
//���в���
static const char _chCopyLeft[] =       {"���ƹ��ǰ"};
static const char _chCopyRight[] =      {"���ƹ���"};
static const char _chCopyAll[] =        {"��������"};
static const char _chPease[] =          {"ճ��"};

static const char *const _FourceExitArrow[] = {
  _chCopyLeft, _chFourceExitEnter,  //��һ��
  _chCopyAll, _chPease,             //�ڶ���
  _chCopyRight, _chFourceExitReturn,//������
};

//---------------------------���뷨ѡ��ģʽ����ַ���Դ--------------------------
static const char _chImeSel[] = {"���뷨ѡ��:"};
static const char _chImeSelUp[] =    {"ABC"};
static const char _chImeSelSave[] =  {"ƴ��"};
static const char _chImeSelLeft[] =  {"����"};
static const char _chImeSelRight[] = {"0-9"};
static const char _chImeSelDown[] =  {"abc"};
static const char *const _ImeSelArrow[] = {
  _chImeSelUp, _chImeSelSave,
  _chImeSelLeft, _chImeSelRight,
  _chImeSelDown, chReturn,
};

//--------------------------------���������û���ʾ-----------------------------
static void _FullNotice(struct _TImeMng *pIme, 
                        char *pStr)
{
  unsigned char State = pIme->State;
  if(State == TIME_MNG_STATE_EDIT)//�༭ģʽʱ
    memcpy(pStr, _chEditMode, sizeof(_chEditMode) - 1);
  else if(State == TIME_MNG_STATE_FORCE_EXIT)//ǿ���˳�ģʽʱ
    memcpy(pStr, _chFourceExitMode, sizeof(_chFourceExitMode) - 1);
  else if(State == TIME_MNG_STATE_SEL)//���뷨ѡ��ģʽ
    memcpy(pStr, _chImeSel, sizeof(_chImeSel) - 1);
  else{//���뷨ģʽ�ɸ���ģ�����(���ģ��ʵ�ֵ���)
    switch(pIme->Type){
      case TIME_MNG_TYPE_CAPITAL:    //��д��ĸ���뷨
      case TIME_MNG_TYPE_LOWERCASE:  //Сд��ĸ���뷨      
        TImeCapital_FullTitle(&pIme->Data.Capital, pStr); break;
      case TIME_MNG_TYPE_NUM:        //�������뷨
        TImeNum_FullTitle(&pIme->Data.Num, pStr); break;
      case TIME_MNG_TYPE_SIGN:       //�������뷨
        TImeSign_FullTitle(&pIme->Data.Sign, pStr); break;
      case TIME_MNG_TYPE_PINYIN:     //ƴ�����뷨 
        TImePinYin_FullTitle(&pIme->Data.PinYin, pStr); break; 
      default:break;
    }
  }
}

//-----------------------�ɼ�ͷ��ʾ��תת��ΪGuideKeyId--------------------------
static const unsigned char _Arrow2GuideKey[] = {
  TIME_MNG_KEY_UP, TIME_MNG_KEY_ENTER,  
  TIME_MNG_KEY_LEFT, TIME_MNG_KEY_RIGHT,
  TIME_MNG_KEY_DOWN, TIME_MNG_KEY_EXIT
};

//--------------------------��GuideKey���Ҷ�Ӧ���뷨����-----------------------
static const unsigned char _GuideKey2TypeMask[] = {
  (1 <<TIME_MNG_TYPE_CAPITAL),  // TIME_MNG_KEY_UP 
  (1 <<TIME_MNG_TYPE_SIGN),     //TIME_MNG_KEY_LEFT
  (1 <<TIME_MNG_TYPE_PINYIN),   //TIME_MNG_KEY_ENTER
  (1 <<TIME_MNG_TYPE_NUM),      //TIME_MNG_KEY_RIGHT
  (1 <<TIME_MNG_TYPE_LOWERCASE), //TIME_MNG_KEY_DOWN
};

//------------------------------����ͷ������----------------------------------
//�˺���ֻ���һ��
static void _FullArrowLine(struct _TImeMng *pIme, 
                            unsigned char Arrow,//0-2��
                            unsigned char DispOffset,//ƫ��λ��
                            char *pStr)
{
  unsigned char State = pIme->State;
  //������Ҳ��ַ����磺"������񱣴��˳�",��: ����   ��:�����˳�
  unsigned char y = pIme->DispOffsetY + Arrow + 4; //������
  Arrow *= 2;
  //====================���뷨ģʽ�ɸ���ģ��Ҫ��Ӧλ���ַ�======================
  for(unsigned char ArrowR = Arrow + 1; Arrow <= ArrowR; Arrow++){
    unsigned char GuideKey = _Arrow2GuideKey[Arrow];
    const char *pRdStr = NULL;
    if(State == TIME_MNG_STATE_EDIT){//�༭ģʽʱ
      pRdStr = _EditArrow[Arrow];     
    }
    else if(State == TIME_MNG_STATE_FORCE_EXIT){//ǿ���˳�ģʽʱ
      pRdStr = _FourceExitArrow[Arrow];     
    }
    else if(State == TIME_MNG_STATE_SEL){//���뷨ѡ��ģʽ
      //���˳���ʱ�������뷨������ʱ������ѡ��
      if((GuideKey == TIME_MNG_KEY_EXIT) || 
        (pIme->TypeMask & _GuideKey2TypeMask[GuideKey]))
          pRdStr = _ImeSelArrow[Arrow];
    }
    else{//���뷨ģʽʱ
      switch(pIme->Type){
      case TIME_MNG_TYPE_CAPITAL:
      case TIME_MNG_TYPE_LOWERCASE: 
        pRdStr = TImeCapital_pGetArrowNotice(&pIme->Data.Capital, GuideKey); 
        break;
      case TIME_MNG_TYPE_NUM: 
        pRdStr = TImeNum_pGetArrowNotice(&pIme->Data.Num, GuideKey); 
        break;      
      case TIME_MNG_TYPE_SIGN: 
        pRdStr = TImeSign_pGetArrowNotice(&pIme->Data.Sign, GuideKey); 
        break;
      case TIME_MNG_TYPE_PINYIN:{ 
        pRdStr = TImePinYin_pGetArrowNotice(&pIme->Data.PinYin, GuideKey); 
        break; 
       }
       default:break;
      }
    }
    if(pRdStr == NULL) continue; //�������
    
    if(Arrow < ArrowR){//������
      if(pRdStr != NULL){
        unsigned char StrLLen = strlen(pRdStr);
        unsigned char Pos = _ArrowPosLUT[Arrow] + DispOffset - StrLLen;
        memcpy(pStr + Pos, pRdStr, StrLLen);
        TImeMng_cbFullStrColor(GuideKey,
                               y, pIme->DispOffsetX + Pos, StrLLen + 2);//��ͷһ��
      }
    }
    else if(pRdStr != NULL){//�Ҳ����
      unsigned char StrLLen = strlen(pRdStr);
      unsigned char Pos = _ArrowPosLUT[Arrow] - DispOffset;
      memcpy(pStr + Pos, pRdStr, strlen(pRdStr));
      TImeMng_cbFullStrColor(GuideKey, 
                             y, pIme->DispOffsetX + Pos - 2, StrLLen + 2);//��ͷһ��
    }
  }
}

//-----------------------------ˢ�´�����ʾ����---------------------------------
//ˢ��pWin���ڵĲ�����Ҫʵʱ���µ�����(��ֻ���±仯����)
static void _Refresh(struct _TImeMng *pIme)
{
  TWin_t *pWin = pIme->pWin;  //��ǰ�ҽӵĴ���
  unsigned char x = pIme->DispOffsetX;
  unsigned char y = pIme->DispOffsetY;

  //���ڶ���:�༭��
  TImeEdit_FullBuf(&pIme->Edit, _pFullVLine(TWin_pGetString(pWin, y + 1) + x));
  //���λ��
  TWin_SetFocus(pWin,
                  x + TImeEdit_GetCursor(&pIme->Edit) + 2,
                  y + 1, 1);//����ռ1��

  //��������: �û���ʾ��
  _FullNotice(pIme, _pFullVLine(TWin_pGetString(pWin, y + 3) + x));
  //��������:�ϼ�ͷ�С�������:���Ҽ�ͷ���������:�¼�ͷ��
  y += 4;  
  for(unsigned char ArrowLne = 0; ArrowLne < 3; ArrowLne++){
    char *pStr = TWin_pGetString(pWin, y + ArrowLne) + x;
    if((ArrowLne == 1) && //���Ҽ�ͷ��ʱ���ڷ������뷨ģʽ�辫������ʾ�·���
       (pIme->State == TIME_MNG_STATE_IME) && (pIme->Type == TIME_MNG_TYPE_SIGN)){
      memcpy(pStr, _LeftRightArrowLine2, TIME_MNG_DISP_W);
      _FullArrowLine(pIme, ArrowLne, 1, pStr);
    }
    else{
      memcpy(pStr, _ArrowLine[ArrowLne], TIME_MNG_DISP_W);//���Ĭ��
      _FullArrowLine(pIme, ArrowLne, 0, pStr);
    }
    
  }
}
