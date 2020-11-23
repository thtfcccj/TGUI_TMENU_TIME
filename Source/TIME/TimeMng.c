/*****************************************************************************
                         TGUI�ϵ����뷨ģ����ʵ��
ע:�ݲ�֧���Դ��ֿ�ȫ�Ƕ���LCD��ʾ������ʾ
*****************************************************************************/

#include "TImeMng.h"
#include "ClipBoard.h"  //���а�
#include <string.h>

/*****************************************************************************
                           �ڲ��궨��
*****************************************************************************/
#define _BORDER_MIN_W   ((128 / 8) + 4)  //���߿���С���
#define _BORDER_MIN_H   ((64 / 16) + 2)  //���߿���С�߶�

#ifdef SUPPORT_TIME_APPEND_NOTE
  #define _NOTE_MIN_W   (_BORDER_MIN_W + 1)  //��������ʾ����С���
  #define _NOTE_MIN_H   (_BORDER_MIN_H + 1)  //��������ʾ����С�߶�
  #define _SPACE_MIN_H   (_NOTE_MIN_H + 1)  //���������С�߶�

  #define _NOTE_ID_COPY   3  //copy��ʾID
  #define _NOTE_ID_PASTE  4  //ճ����ʾID
  #define _NOTE_ID_NSAVE  5  //��������ʾID
  #define _NOTE_ID_SAVE   6  //������ʾID
  #define _NoteId2Timer(id) ((0x10 * (id)) - 1) //��ʾתʱ��

  static void _UpdateAppendNote(struct _TImeMng *pIme);//���¸�����ʾ�к���
#else
  #define _SPACE_MIN_H   (_BORDER_MIN_H + 1)  //���������С�߶�
#endif

/*****************************************************************************
                           �ڲ���������
*****************************************************************************/

//---------------------------�״�ˢ������--------------------------------
//��ʼ������ʱ�ȵ���
static void _Refresh1st(struct _TImeMng *pIme);

//-------------------------�õ���һ����Ч���뷨����---------------------------------
//���ݿ��������л�����һ����Ч���뷨
//������һ����Ч���뷨
static unsigned char _SwitchType(const struct _TImeMng *pIme);

//--------------------------���뷨�л�����---------------------------------
//�л�����Ӧ���뷨����ʼ��
static void _SwitchIme(struct _TImeMng *pIme,
                       unsigned char Type);//���е������뷨,0-4

//-----------------------���뷨�ڲ�����������---------------------------------
//pIme->Flag�е�TIME_FLAG_STATE��λʱ���ô˺���
//����ֵ����Ϊ0δ��Ӧ
//2:���뷨�ڲ��˳����н������
//3:���뷨�ڲ��˳�����Ч����
static signed char _KeyInType(struct _TImeMng *pIme,
                              char Key);

//-----------------------�༭״̬���ּ�0��9������-----------------------------
//pIme->Flag�е�TIME_FLAG_STATEδ��λʱ���ô˺���
//����ֵ����Ϊ:
//0δ��Ӧ,
//1:�������뷨�ڲ����޽������
//2:δ�����뷨�ڲ����н������
static signed char _NumKeyInEdit(struct _TImeMng *pIme,
                                   char NumKey);//��ֵ'0'-'9'

//------------------�õ����뷨�ڲ����ź���------------------------------
//�����뷨�������ʱ���õ������±༭������
static void _GetImeChar(struct _TImeMng *pIme);

//------------------ˢ�´�����ʾ����---------------------------------
//ˢ��pWin���ڵ���ʾ����
static void _Refresh(struct _TImeMng *pIme);

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
                         const char *pSignTbl) //�ҽӵķ��ű�,Ϊ��ʱʹ��Ĭ��
{
  unsigned char w,h;
  //��鴰���Ƿ���ʾ
  if(TWin_GetW(pWin) < 16) return -1;//������ʾ
  if(TWin_GetH(pWin) < 4) return -1;//������ʾ
  
  memset(pIme, 0, sizeof(struct _TImeMng));
  ClipBoard.PrvSel = -1;//ѡ����Ч
  
  //������ʾƫ��
  w = TWin_GetW(pWin);
  if(w > TIME_MNG_MIX_W){
    pIme->DispOffsetX = (w - TIME_MNG_MIX_W) / 2;
    w = TIME_MNG_MIX_W;
  }
  h = TWin_GetH(pWin);
  if(h > TIME_MNG_MAX_H){
    pIme->DispOffsetY = (h - TIME_MNG_MAX_H) / 2;
    h = TIME_MNG_MAX_H;
  }
  pIme->w = w;
  pIme->h = h;
  //��ʼ����Data����ĳ�Ա
  pIme->pWin = pWin;
  TImeEdit_Init(&pIme->Edit,pString,Size,pIme->w);//��ʼ���༭��  
  pIme->pSignTbl = pSignTbl;  
  if(DefaultType >= 4) DefaultType = 0;//ǿ�ƾ���
  pIme->TypeMask = TypeMask;
  
  _SwitchIme(pIme,DefaultType);//����Ĭ�����뷨���ͳ�ʼ��Data��
  
  //�����ƴ�����ʾ
  //ȥ�����ܲ���Ҫ����ɫ
  TImeMng_cbFullStrColor(0xff,0,0,TWin_GetW(pWin));
  _Refresh1st(pIme); //��ʼ��ʱ����ˢ�¹̶����䲻����
  _Refresh(pIme);      //������ʾ
  return 0;
}

//----------------------------����������---------------------------------
//�û������ּ����뵼������ʱ���ô˺���
//����ֵ����Ϊ: 0����״̬,-1:�˳���ֱ���˳�;-2:ȷ�ϼ��˳�
signed char TImeMng_Key(struct _TImeMng *pIme,
                        char Key)          //��ֵ
{
  //ֻҪ�������ּ�����ǿ��ֹͣ��Сд��ʱ��
  if((Key < '0') || (Key > '9'))  pIme->CapitalTimer = 0;
  
  if(!(pIme->Flag & TIME_FLAG_STATE)){//�ڱ༭״̬,���б༭�ַ�������
    switch(Key){
    case TGUI_KEY_LEFT:  TImeEdit_CursorLeft(&pIme->Edit); break;
    case TGUI_KEY_RIGHT: TImeEdit_CursorRight(&pIme->Edit); break;
    case TGUI_KEY_UP:{  //�ϼ�����帴��
      ClipBoardSizt_t Cursor =  TImeEdit_GetCurCursor(&pIme->Edit);
      signed short PrvSel = ClipBoard.PrvSel;
      //���״�ѡ����������겻ͬʱ���ɸ���
      if((PrvSel >= 0) && (PrvSel != Cursor)){
        ClipBoardSizt_t Len;
        if(PrvSel > Cursor){//�������ʱ
          Len = PrvSel - Cursor;
          memcpy(ClipBoard.Buf, TImeEdit_pGetCurStr(&pIme->Edit) + Cursor, Len);
        }
        else{//���Һ���ʱ
          Len = Cursor - PrvSel;
          memcpy(ClipBoard.Buf, TImeEdit_pGetCurStr(&pIme->Edit) + PrvSel, Len);            
        }
        ClipBoard.Buf[Len] = '\0';//ǿ�ƽ����ַ�
      }
      ClipBoard.PrvSel = Cursor; //ǿ�Ƹ��������λ��
      #ifdef SUPPORT_TIME_APPEND_NOTE
        pIme->NoteTimer = _NoteId2Timer(_NOTE_ID_COPY); //������ʾ
      #endif
      break;
    }
    case TGUI_KEY_DOWN:{ //�¼����а�ճ��
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
      #ifdef SUPPORT_TIME_APPEND_NOTE
        pIme->NoteTimer = _NoteId2Timer(_NOTE_ID_COPY); //������ʾ
      #endif
      break;
    }
    case TGUI_KEY_DELETE:TImeEdit_Clr(&pIme->Edit); break;
    case TGUI_KEY_ENTER:  //ȷ�ϼ��˳��༭״̬
      TImeMng_Quit(pIme);
      return-2;
    case TGUI_KEY_ESCAPE://�˳���ֱ���˳��༭״̬
      TImeMng_Quit(pIme);
      return-1;
    case '*': //�л�����������ģʽ,���������ڲ�
      _SwitchIme(pIme, TIME_TYPE_SIGN);
      pIme->Flag |= TIME_FLAG_STATE;
      break;
    case '#'://���л����뷨
      _SwitchIme(pIme, _SwitchType(pIme));
    default:
      //���ּ����뷨��Ӧ
      if((Key >= '0') && (Key <= '9')){
        switch(_NumKeyInEdit(pIme, Key)){
        //case 0:break;// 0δ��Ӧ
        case 1://�������뷨�ڲ����ñ�־
          pIme->Flag |= TIME_FLAG_STATE;
          break;
        case 2: _GetImeChar(pIme);//�н������,ȡ�ý��
          break;
        default:break;
        }
      }
      break;
    }
  }
  else{//�����뷨�ڲ�ʱ
    
    switch(_KeyInType(pIme, Key)){
        //case 0:break;// 0δ��Ӧ
        case 1: //1:���뷨�ڲ����˳����н������
          _GetImeChar(pIme);
          break;
        case 2: //���뷨�ڲ��˳����н������
          _GetImeChar(pIme);
          #ifdef SUPPORT_TIME_APPEND_NOTE
            pIme->NoteTimer = _NoteId2Timer(_NOTE_ID_SAVE); //������ʾ�����˳�
          #endif  
          //����
        case 3: //���뷨�ڲ��˳�����Ч����
          pIme->Flag &= ~TIME_FLAG_STATE;
          #ifdef SUPPORT_TIME_APPEND_NOTE
            if(pIme->NoteTimer != _NoteId2Timer(_NOTE_ID_SAVE))//���Ǳ����˳�ʱ
              pIme->NoteTimer = 0;//��λ��ʾ
          #endif  
          //��Ϊ�������뷨�˳�����ǿ���˵�����ǰ���뷨״̬
          if(pIme->Type == TIME_TYPE_SIGN)
            _SwitchIme(pIme, pIme->Flag & TIME_PRV_TYPE_MASK);
          break;
        default:break;
    }
  }
  
  _Refresh(pIme);      //������ʾ
  return 0;
}


//--------------------------������---------------------------------
//�������뷨ʱ���ô˺���ʵ��ʱ��Ҫ��
void TImeMng_Task(struct _TImeMng *pIme)
{
  if(ClipBoard.PrvSel == -2) return; //ģ����Ч
  
  //��ʾ��ʱ��
  #ifdef SUPPORT_TIME_APPEND_NOTE
    pIme->NoteTimer++;
    if(!(pIme->NoteTimer & 0x0f)) _UpdateAppendNote(pIme);//2s�Զ�����һ����ʾ
  #endif
  
  //��Сд��ʱ��
  if(!pIme->CapitalTimer) return ;//��ʱ��ֹͣ״̬
  pIme->CapitalTimer--;
  if(pIme->CapitalTimer) return;//������
  //��ʱʱ�䵽ʱ:
  if((pIme->Type == TIME_TYPE_LOWERCASE) || //��ֹ�쳣����
     (pIme->Type == TIME_TYPE_CAPITAL)){
       TImeCapital_SampReset(&pIme->Data.Capital);//ͬ��ֵʱ��λ
       _Refresh(pIme);//����ˢ����ʾ
  }
}

//-----------------------------�˳�����---------------------------------
//�û������ַ�ȷ���˳�����ô˺���
void TImeMng_Quit(struct _TImeMng *pIme)
{
  //ȥ�����ӵ���ɫ
  TImeMng_cbFullStrColor(0xfe,
                         pIme->DispOffsetY,   //pWin��y����
                         pIme->DispOffsetX,   //pWin��x����
                         pIme->w);       //x����
  ClipBoard.PrvSel = -2;//ģ����Ч
}


/*****************************************************************************
                           �ڲ�����ʵ��
*****************************************************************************/

//-------------------------�õ���һ����Ч���뷨����---------------------------------
//���ݿ��������л�����һ����Ч���뷨
//������һ����Ч���뷨
static unsigned char _SwitchType(const struct _TImeMng *pIme)
{
  unsigned char Type = pIme->Type + 1;//��һ���뷨
  unsigned char TypeMask = 1 << Type;
  for(; Type < TIME_TYPE_SIGN; Type++,TypeMask <<= 1){
    if(TypeMask & pIme->TypeMask) return Type; //�ҵ���
  }
  //û�ҵ����ػ�����һ����ʼ��
  Type = 0;
  TypeMask = 1;
  for(; Type <= pIme->Type; Type++,TypeMask <<= 1){
    if(TypeMask & pIme->TypeMask) break;
  }
  return Type; //�ҵ���
}

//--------------------------�õ���Ч���---------------------------------
static unsigned char _GetValidW(const struct _TImeMng *pIme)
{
  unsigned char w = pIme->w;
  if(w >= _BORDER_MIN_W) w -= 4; //����ʾ���ұ߽�ʱ
  return w;
}

//--------------------------�õ����뷨�ڸ߶�---------------------------------
static unsigned char _GetImeH(const struct _TImeMng *pIme)
{
  unsigned char h = pIme->h - 1;           //�༭ռһ��;
  if(h >= _SPACE_MIN_H) return h - 4;       //���+��ʾ+���ұ߽�
  else if(h >= _SPACE_MIN_H) return h - 3; //��ʾ+���ұ߽�
  if(h >= _BORDER_MIN_W) return h - 2;      //�����ұ߽�
  return h;
}

//--------------------------���뷨�л�����---------------------------------
//�л�����Ӧ���뷨����ʼ��
static void _SwitchIme(struct _TImeMng *pIme,
                       unsigned char Type)//���е������뷨,0-4
{


  switch(Type){
  case TIME_TYPE_PINYIN:
    TImePinYin_Init(&pIme->Data.PinYin, _GetValidW(pIme));
    break;
  case TIME_TYPE_NUM:
    TImeNum_Init(&pIme->Data.Num);
    break;
  case TIME_TYPE_LOWERCASE:
    TImeCapital_Init(&pIme->Data.Capital, 0);
    break;
  case TIME_TYPE_CAPITAL:
    TImeCapital_Init(&pIme->Data.Capital, 1);
    break;
  default://�������뷨��
    pIme->Flag &= ~TIME_PRV_TYPE_MASK;//��ס�ϴ����뷨״̬
    pIme->Flag |= pIme->Type;
    TImeSign_Init(&pIme->Data.Sign, 
                  pIme->pSignTbl,
                  _GetValidW(pIme), _GetImeH(pIme));
    break;
  }
  pIme->Type = Type;
}

//-----------------------���뷨�ڲ�����������---------------------------------
//pIme->Flag�е�TIME_FLAG_STATE��λʱ���ô˺���
//����ֵ����Ϊ0δ��Ӧ
//1:���뷨�ڲ����˳����н������
//2:���뷨�ڲ��˳����н������
//3:���뷨�ڲ��˳�����Ч����
static signed char _KeyInType(struct _TImeMng *pIme,
                              char Key)
{
  if(pIme->Type == TIME_TYPE_PINYIN){
    switch(TImePinYin_eKey(&pIme->Data.PinYin, Key)){
    case eTImePinYin_Idie://�˳���ƴ�����뷨
       return 3;
    case eTImePinYin_Final://ƴ�����뷨���
       return 2;
    default: break;  //ƴ�����뷨����������
    }
  }
  else if(pIme->Type == TIME_TYPE_SIGN){
    if((Key >= '1') && (Key <= '9')){
      if(TImeSign_KeyNum(&pIme->Data.Sign, Key) == 1)//�������ʱ
        return 1;
    }
    else if((Key == TGUI_KEY_DELETE) || (Key == TGUI_KEY_ESCAPE)){
      if(TImeSign_KeyClr(&pIme->Data.Sign))//����Ӧʱ�˳����뷨
        return 3;
    }
    if((Key == TGUI_KEY_PAGEUP) || (Key == TGUI_KEY_UP))//��һҳ����
      TImeSign_KeyPage(&pIme->Data.Sign, 0);
    else if((Key == TGUI_KEY_PAGEDOWN) || (Key == TGUI_KEY_DOWN))//��һҳ����
      TImeSign_KeyPage(&pIme->Data.Sign, 1);
    else if(Key == TGUI_KEY_ENTER)//ȷ�ϼ��������˳�
      return 3;
  }
  //TIME_TYPE_NUM://�������뷨����������뷨�ڲ����޴�״̬
  //TIME_TYPE_LOWERCASE://Сд���뷨����������뷨�ڲ����޴�״̬
  //TIME_TYPE_CAPITAL://��д���뷨����������뷨�ڲ����޴�״̬
  return 0;
}

//-----------------------�༭״̬���ּ�0��9������-----------------------------
//pIme->Flag�е�TIME_FLAG_STATEδ��λʱ���ô˺���
//����ֵ����Ϊ:
//0δ��Ӧ,
//1:�������뷨�ڲ����޽������
//2:δ�����뷨�ڲ����н������
static signed char _NumKeyInEdit(struct _TImeMng *pIme,
                                   char NumKey)//��ֵ'0'-'9'
{
  signed char Resume = 0;
  switch(pIme->Type){
  case TIME_TYPE_PINYIN:
    if(NumKey >= '2'){//����ƴ���ڲ�����Ӧ�׸�����
       TImePinYin_eKey(&pIme->Data.PinYin, NumKey);
       Resume =  1;
    }
    break;
  case TIME_TYPE_NUM://ȫ��Ӧ,�������ڲ�����Ӧ����
    TImeNum_KeyNum(&pIme->Data.Num, NumKey);
    Resume =  2;
    break;
  case TIME_TYPE_LOWERCASE:
  case TIME_TYPE_CAPITAL:
    TImeCapital_KeyNum(&pIme->Data.Capital, NumKey);
    pIme->CapitalTimer = TIME_CAPITAL_TIME_OV;//���¸�λ��������ʱ��
    Resume =  2; //�������Ȼ���ַ���
    break;
  default:
    break;
  }
  return Resume;
}

//------------------�õ����뷨�ڲ����ź���------------------------------
//�����뷨�������ʱ���õ������±༭������
static void _GetImeChar(struct _TImeMng *pIme)
{
  const char *pChar;
  unsigned short String;
  
  switch(pIme->Type){
  case TIME_TYPE_PINYIN://ƴ����������뷨ʱ
  case TIME_TYPE_SIGN:
    if(pIme->Type == TIME_TYPE_PINYIN)
      pChar = TImePinYin_pGetChar(&pIme->Data.PinYin);
    else
      pChar = TImeSign_pGetChar(&pIme->Data.Sign);
    if(*pChar >= 0x80)//ȫ���ַ�ʱ
      String = (unsigned short)(*pChar << 8) | *(pChar + 1);
    else
      String = (unsigned short)*(pChar + 1);
    break;
  case TIME_TYPE_NUM:  //����ʱ��ֱ�ӻ��
    String = (unsigned short)TImeNum_GetNum(&pIme->Data.Num);
    break;
  case TIME_TYPE_LOWERCASE://��Сдʱ,ֱ�ӻ��
  case TIME_TYPE_CAPITAL:
    String = (unsigned short)TImeCapital_GetChar(&pIme->Data.Capital);
    //ͬһ�����ж��ʱ�������ַ��滻��ǰλ���ַ�(�����֮ǰ)
    if(TImeCapital_GetSameKeyIndex(&pIme->Data.Capital) >= 2)
      TImeEdit_Clr(&pIme->Edit);
    break;
  default: break;
  }
  TImeEdit_Add(&pIme->Edit, String);//���Ӽ�ֵ
}

//------------------ˢ�´�����ʾ�����ڲ���Դ---------------------------------
//��ʾ���ַ���
static const char *const _pTypeDisp[] = {
 "ƴ��",
 "123 ",
 "abc ",
 "ABC ",
};

//ƴ��������ʾ�������ַ���
static const char *const _pPinYinTypeDisp[] = {
 "->ƴ������",
 "->ƴ��ѡ��",
 "->����ѡ��",
};

static const char _Line[] = {"��"};
static const char _TopLeft[] = {"��"};
static const char _TopRight[] = {"��"};
static const char _BottomLeft[] = {"��"};
static const char _BottomRight[] = {"��"};
static const char _CenterLeft[] = {"��"};
static const char _CenterRight[] = {"��"};
static const char *const pLeft[] = {
  _Line,
  _TopLeft,
  _BottomLeft,
  _CenterLeft,
};
static const char *const pRight[] = {
  _Line,
  _TopRight,
  _BottomRight,
  _CenterRight,
};

//-------------------------������ұ߽纯��---------------------------------
//���������ʼ�߽�λ��
static char* _pFullVLine(unsigned char w, char *pStr)
{
  if(w >= _BORDER_MIN_W){//����ʾ���ұ߽�ʱ
    *pStr++ = 0xa9; //�����
    *pStr++ = 0xa6;//���Ұ�
    memset(pStr, ' ', w - 4);//����м䲿��
    *(pStr + w - 4)= 0xa9; //�����
    *(pStr + w - 3) = 0xa6;//���Ұ�
  }
  return pStr;
}

//-------------------------���ˮƽ�߿���---------------------------------
//���������ʼ�߽�λ��
static void _FullHBorder(unsigned char w,//����ܿ��
                          char *pStr,      //���λ��
                          unsigned char BorderType)//�߽�����,0�м�1��2��3��
{
  //�����߽�
  memcpy(pStr, pLeft[BorderType], 2);
  pStr += 2;
  //����м�
  w = (w - 4) & 0xfe;//˫�ֶ���
  for(; w > 0; w-= 2){
    *pStr++= 0xa9; //�����
    *pStr++ = 0xa4;//���Ұ�
  }
  //����ұ߽�
  memcpy(pStr, pRight[BorderType], 2);
}

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
  unsigned char w = pIme->w;
  
  //����ʾ���±߽�ʱ
  if(pIme->h >= _BORDER_MIN_H){
    if(w >= _BORDER_MIN_W){//����ʾ���ұ߽�ʱ
      _FullHBorder(w, TWin_pGetString(pWin, y + 0) + x, 1);
      _FullHBorder(w, TWin_pGetString(pWin, y + pIme->h - 1) + x, 2);
      if(pIme->h >= _SPACE_MIN_H)//�������ʱ
        _FullHBorder(w, TWin_pGetString(pWin, y + 2) + x, 3);
    }
    else{//�����ұ߽�
      _FullHBorder(w, TWin_pGetString(pWin, y + 0) + x, 0);
      _FullHBorder(w, TWin_pGetString(pWin, y + pIme->h - 1) + x, 0);
      if(pIme->h >= _SPACE_MIN_H)//�������ʱ
        _FullHBorder(w, TWin_pGetString(pWin, y + 2) + x, 0);
    }
  }
}

//--------------------------���¸�����ʾ�к���--------------------------------
#ifdef SUPPORT_TIME_APPEND_NOTE
//16���ַ�Ϊ��С��С��������������

//�༭״̬ʱ����ʾ
static const char _ImeSwitchNote[] =   {"*��#���л����뷨"};
static const char _ImeKeyNumNote[] =   {"�����ּ���ʼ����"};
static const char _ImeLenNote[] =      {"ע���ַ���������"};
static const char _ImeCopyStartNote[] ={"�������ø������"};
static const char _ImeCopyEndNote[] =  {"�ƶ�����������"};
static const char _ImePasteNote[] =    {"����ճ������괦"};
static const char _ImeNoPasteNote[] =  {"ճ������������!"};
static const char _ImeExitNote[] =     {"���ؼ��������˳�"};
static const char _ImeSaveNote[] =     {"ȷ�ϼ����沢�˳�"};

static const char * const _ImeNote[] = {
  _ImeSwitchNote,
  _ImeKeyNumNote,
  _ImeLenNote,
  _ImeCopyEndNote,
  _ImePasteNote,
  _ImeExitNote,  
  _ImeSaveNote, //�����
};

//ƴ�����뷨��ʾ
static const char _PinYinEnterNote[] =  {"ȷ�ϼ�ƴ�����"};
static const char _PinYinPyNote[] =     {"1-9��,������ҳ"};
static const char _PinYinChNote[] =     {"1-9��,������ҳ"};
static const char * const _PinYinNote[] = {
  _PinYinEnterNote,
  _PinYinPyNote,
  _PinYinChNote,
};

//�������뷨��ʾ
static const char _SignSelRowPageNote[] =  {"��ѡ,������ҳ!"};
static const char _SignSelRowNote[] =  {"1-9��ѡ����!"};
static const char _SignSelColNote[] =  {"1-9��ѡ����!"};

//�������뷨��ʾ
static const char _NumNote[] =     {"0-9����������"};
static const char _CapitalNote[] = {"���Ӱ��л���ĸ"};

static void _UpdateAppendNote(struct _TImeMng *pIme)
                                  
{
  if(pIme->h < _NOTE_MIN_H) return; //���β�����ʾ
  
  //�ҵ�������ʾ��λ��
  unsigned char y = pIme->DispOffsetY + 1; //�༭��������
  if(pIme->h >= _BORDER_MIN_H)  y++;        //����ʾ���±߽�ʱ 
  if(pIme->h >= _SPACE_MIN_H) y++;          //�����������
  char *pBuf = _pFullVLine(pIme->w, 
                           TWin_pGetString(pIme->pWin, y) + pIme->DispOffsetX);
  
  const char *pStr;
  unsigned char StrColorState;
  if(!(pIme->Flag & TIME_FLAG_STATE)){//�ڱ༭״̬ʱ����ʾ
    StrColorState = (pIme->NoteTimer >> 4) % (sizeof(_ImeNote) / 4);//��ʾ����
    if(StrColorState == _NOTE_ID_COPY){//���а帴����ʾ
      if(ClipBoard.PrvSel < 0) //û��ѡ��ʼ��
        pStr = _ImeCopyStartNote;
      else pStr = _ImeCopyEndNote;
    }
    else if(StrColorState == _NOTE_ID_PASTE){//���а�ճ����ʾ
      if(ClipBoard.Buf[0] == '\0') //������
        pStr = _ImeNoPasteNote;
      else pStr = _ImePasteNote;
    }
    else pStr = _ImeNote[StrColorState];
    StrColorState += 0x80;
  }
  else{//�����뷨״̬�ڲ�������ʾ
    switch(pIme->Type){
    case TIME_TYPE_PINYIN://ƴ����������뷨ʱ
      StrColorState = pIme->Data.PinYin.eState;
      if(StrColorState) StrColorState--;
      pStr = _PinYinNote[StrColorState];
      StrColorState += 0x90;
      break;
    case TIME_TYPE_SIGN:
      StrColorState = pIme->Data.Sign.eState;
      if(!StrColorState){//��ҳ��ʾ
        if(TImeSign_IsOnePage(&pIme->Data.Sign))//һҳʱ
          pStr = _SignSelRowNote;
        else pStr = _SignSelRowPageNote;
      }
      else pStr = _SignSelColNote;//��ѡ����ʾ
      StrColorState += 0xa0;
      break;
    case TIME_TYPE_NUM:  //����ʱ
      pStr = _NumNote;
      StrColorState = 0xb0;
      break;    
    case TIME_TYPE_LOWERCASE://Сдʱ
      pStr = _CapitalNote;
      StrColorState = 0xb0;      
    case TIME_TYPE_CAPITAL://��дʱ
      pStr = _CapitalNote;
      StrColorState = 0xc0;  
      break;
    default: return; //�쳣
    }
  }
  unsigned char StrLen = strlen(pStr);
  memcpy(pBuf, pStr, StrLen); //���������ַ�
  //������ʾ����ɫ
  unsigned char xColorStart = pIme->DispOffsetX;
  if(pIme->w >= _BORDER_MIN_W) xColorStart += 2;//����ʾ���ұ߽�ʱ,��߿�ʼ
  TImeMng_cbFullStrColor(StrColorState, y, xColorStart, StrLen);
}
#endif

//------------------ˢ�´�����ʾ����---------------------------------
//ˢ��pWin���ڵ���ʾ����
static void _Refresh(struct _TImeMng *pIme)
{
  char *pBuf;
  TWin_t *pWin = pIme->pWin;  //��ǰ�ҽӵĴ���
  unsigned char w = pIme->w;
  unsigned char h = pIme->h;
  unsigned char x = pIme->DispOffsetX;
  unsigned char y = pIme->DispOffsetY; //��ǰ��
  unsigned char xColorStart = x;
  if(w >= _BORDER_MIN_W){//����ʾ���ұ߽�ʱ
    xColorStart += 2; //��߿�ʼ
  }
  if(h >= _BORDER_MIN_H){//����ʾ���±߽�ʱȥ��
    h -= 2;//ȥ���±߿�
    y += 1;//�߿���һ�п�ʼ
  }

  //����һ��:�༭��
  TImeEdit_FullBuf(&pIme->Edit, _pFullVLine(w, TWin_pGetString(pWin, y) + x));
  if(pIme->CapitalTimer){
    //��Сд���뷨ʱ,����������״̬,����ռ������ʾ�������������
    TWin_SetFocus(pWin,
                  TImeEdit_GetCursor(&pIme->Edit) + xColorStart - 1,
                  y, 2);//����ռ2��
  }
  else{
    TWin_SetFocus(pWin,
                  TImeEdit_GetCursor(&pIme->Edit) + xColorStart,
                  y, 1);//����ռ1��
  }
  h--; //ȥ������
  y++;//���ڶ�����
  if(pIme->h >= _SPACE_MIN_H){//�������ʱ�����Դ���
    h--;
    y++;//��������    
  }
  #ifdef SUPPORT_TIME_APPEND_NOTE
  if(pIme->h >= _NOTE_MIN_H){//��������ʾ��ʱ���
    _UpdateAppendNote(pIme);
    h--;
    y++;//��������    
  }
  #endif
  
  //�������뷨ʱ������������������
  if(pIme->Type == TIME_TYPE_SIGN){
    for(unsigned char VNum = 0; VNum  < h; VNum++, y++){
      unsigned char Len = TImeSign_GetDispChar(&pIme->Data.Sign, 
                              _pFullVLine(w, TWin_pGetString(pWin, y) + x), VNum);
      unsigned char Mode; //�б�ѡ����
      if((VNum == pIme->Data.Sign.PosV) && (pIme->Data.Sign.eState == eTImeSign_SelV))
        Mode = TIME_TYPE_SIGN | 0x10;
      else Mode = TIME_TYPE_SIGN | 0;
      TImeMng_cbFullStrColor(Mode, y, xColorStart,  Len);//���û������֧�ֱַ���ɫ
    }
    return;
  }
  //���̶���ʾ��: ���ݵ�ǰ���뷨״̬���
  pBuf = _pFullVLine(w, TWin_pGetString(pWin, y) + x);
  memcpy(pBuf, _pTypeDisp[pIme->Type], 4);
  TImeMng_cbFullStrColor(0xf0, y, xColorStart,  4);//�̶���ʾ����ɫ
  h--;//ȥ���ڶ���  
  y++;//���༭���ڲ���(������)

  //���༭���ڲ�: �ɾ���ģ��������򲹳�
  if(pIme->Flag & TIME_FLAG_STATE){
    if(pIme->Type == TIME_TYPE_PINYIN){
      //����ڶ��е���ʾ���ַ�
      enum _eTImePinYin ePinYin = TImePinYin_eGetState(&pIme->Data.PinYin);
      if((ePinYin >= eTImePinYin_Input) && (ePinYin <= eTImePinYin_ChSel)){
        memcpy(pBuf + 4, _pPinYinTypeDisp[ePinYin - 1], 10);
        TImeMng_cbFullStrColor(0xf1, y - 1 , xColorStart + 4,  10);//������ʾ��
      }
      unsigned char Len;
      //�ڲ��и��£�ƴ��ѡ����    
      Len = TImePinYin_GetPinYinChar(&pIme->Data.PinYin, 
                                       _pFullVLine(w, TWin_pGetString(pWin, y) + x));
      TImeMng_cbFullStrColor(TIME_TYPE_PINYIN | (0 << 3), y, xColorStart, Len);
      //����ѡ��ʱ����ɫ��ѡ���ƴ��
      unsigned char ChSel;
      if(pIme->Data.PinYin.eState == eTImePinYin_ChSel){
        TImeMng_cbFullStrColor(TIME_TYPE_PINYIN | (1 << 3), y, 
                               xColorStart + pIme->Data.PinYin.CurPinYinStrStart, 
                               pIme->Data.PinYin.CurPinYinStrCount);
        ChSel = (3 << 3);
      }
      else ChSel = (2 << 3);//ƴ��ѡ��״̬
      y++;//���������� ƴ��ѡ����
      Len = TImePinYin_GetChChar(&pIme->Data.PinYin,
                            _pFullVLine(w, TWin_pGetString(pWin, y) + x));
      TImeMng_cbFullStrColor(TIME_TYPE_PINYIN | ChSel, y, xColorStart,  Len);//���û������֧�ֱַ���ɫ
      h -= 2; //���������
      y++;//����������
    }
  }
  //����������δ����
  for(; h > 0; h--, y++){
    _pFullVLine(w, TWin_pGetString(pWin, y) + x);
  }
}



