/* ----------------------------------------------------------------------------
 *               ����˵��ṹ�ļ�
 * --------------------------------------------------------------------------*/


#include "TMenu_CheckBox.h"
#include "TMenu_MNumAdj.h"
//#include "TMenu_RadioBox.h"
#include "TMenu_SubMenu.h

#include "TMenuBase.h"
#include "string.h"


//----------------------�û�Ȩ�޹���---------------------
struct __TopMenuAVR{
    unsigned char SubMenuAmount;//�Ӳ˵�����
}_TopMenuAVR;
void GetPurview(unsigned short PassWord)
{
    //ÿ�ν���˵�ǰ����Ȩ�޾���?
    //if(_TopMenuAVR._eUser > Guest)_TopMenuAVR._eUser = Guest;
    if(PassWord == 5138)_TopMenuAVR.SubMenuAmount = 5;
    else if(PassWord == Static.PassWord)_TopMenuAVR.SubMenuAmount = 4;
    else _TopMenuAVR.SubMenuAmount = 2;
}

//============================���ò˵�===========================
#include "ARelay.h"
void HandCtrlNotify(unsigned char Type, void *pv)//�˵��ṹ����
{
  unsigned char Item;
  struct _CheckboxUser *pUser = (struct _CheckboxUser*)pv;
  switch(Type){
  case TM_NOTIFY_GET_DATA://��������
    Item = 0;
    if(RL1_SF1_IS_ON()) Item |= 0x01;
    if(RL1_SF2_IS_ON()) Item |= 0x02;
    if(RL1_JR1_IS_ON()) Item |= 0x04;
    if(RL1_JR2_IS_ON()) Item |= 0x08;  
    if(RL1_PF_IS_ON()) Item |= 0x10;
    if(RL1_KFM_IS_ON()) Item |= 0x20;
    if(RL1_ZM_IS_ON()) Item |= 0x40;
    if(RL1_BJ_IS_ON()) Item |= 0x80; 
    pUser->SelMask[0] = Item;
    break;
  case TM_NOTIFY_SET_DATA://��������
  case TM_NOTIFY_CHANGED://ĳ��ı���
    Item = pUser->SelMask[0];
    if(Item & 0x01) RL1_SF1_ON(); 
    else RL1_SF1_OFF();
    if(Item & 0x02) RL1_SF2_ON(); 
    else RL1_SF2_OFF();
    if(Item & 0x04) RL1_JR1_ON(); 
    else RL1_JR1_OFF();
    if(Item & 0x08) RL1_JR2_ON(); 
    else RL1_JR2_OFF();
    if(Item & 0x10) RL1_PF_ON(); 
    else RL1_PF_OFF();
    if(Item & 0x20) RL1_KFM_ON(); 
    else RL1_KFM_OFF();
    if(Item & 0x40) RL1_ZM_ON(); 
    else RL1_ZM_OFF();
    if(Item & 0x80) RL1_BJ_ON(); 
    else RL1_BJ_OFF();
    break;
  default:break;
  }
}

code TMenu_t MHandCtrl =
{
  //�˵�����
  TMTYPE_CHECKBOX, 
  8,              //�ɲ˵����;�����������ݴ�С
  {19},            //�������֧�ֵĲ˵�ͷ�ָ���ID��,�±��Ӧ����
  2,              //���ѵĸ��˵�ID��
  7,              //�洢pv���е�ID��(pvAryID):
  TMENU_NOTIFY_PASS(HandCtrlNotify,TMENU_CBFUNID_NULL_MENU), //���û��ռ佻����ͨ������
};


//---------------------��������֯�˵��ṹ----------------------------
//#include "???.h"	//�ָ��������ó�ʼ��

void AC220Notify(unsigned char Type, void *pv)//����˵��ṹ����
{
    char *pBuf;
    switch(Type)
    {
        case TM_NOTIFY_GET_HEADER:
        
        pBuf = (char*)pv;
        strcpy_P(pBuf,LC_AC220_CH);
        pBuf += sizeof(LC_AC220_CH) - 1;

        pBuf += Value2StringMin(uRePairBuffer[CH5_V1],
                                pBuf,//���ջ���
                                3);//��֤����Сλ��
        *pBuf++= 'V';
        *pBuf = '\0';

        break;
        default : break;
    }
}
//��ѹУ׼
code TMenu_t MAC220Set =
{
  //�˵�����
  TMTYPE_CMD,       //�˵�����Ϊ����ģʽ
  0,                //�ɲ˵����;�����������ݴ�С
  {0},            //�������֧�ֵĲ˵�ͷ�ָ���ID��,�±��Ӧ����//Ϊ0ʱȥ�ص�������ȥ���Ҳ˵�ͷ
  6,              //���ѵĸ��˵�ID��
  0,              //�洢pv���е�ID��(pvAryID)://�����β˵��� PV ID��
  TMENU_NOTIFY_PASS(AC220Notify,TMENU_CBFUNID_NULL_MENU), //���û��ռ佻����ͨ������
};

//1�¶�У׼12
code TMenu_t MTCh12Adjust =
{
  //�˵�����
  TMTYPE_CMD,       //�˵�����Ϊ����ģʽ
  0,                //�ɲ˵����;�����������ݴ�С
  {16},            //�������֧�ֵĲ˵�ͷ�ָ���ID��,�±��Ӧ����
  6,              //���ѵĸ��˵�ID��
  0,              //�洢pv���е�ID��(pvAryID):
  TMENU_NOTIFY_PASS(NULL,TMENU_CBFUNID_NULL_MENU), //���û��ռ佻����ͨ������
};

//2�¶�У׼34
code TMenu_t MTCh34Adjust =
{
  //�˵�����
  TMTYPE_CMD,       //�˵�����Ϊ����ģʽ
  0,                //�ɲ˵����;�����������ݴ�С
  {17},            //�������֧�ֵĲ˵�ͷ�ָ���ID��,�±��Ӧ����
  6,              //���ѵĸ��˵�ID��
  0,              //�洢pv���е�ID��(pvAryID):
  TMENU_NOTIFY_PASS(NULL,TMENU_CBFUNID_NULL_MENU), //���û��ռ佻����ͨ������
};

//3�ָ���������
code TMenu_t MRecoverSet =
{
  //�˵�����
  TMTYPE_CMD,       //�˵�����Ϊ����ģʽ
  0,                //�ɲ˵����;�����������ݴ�С
  {18},            //�������֧�ֵĲ˵�ͷ�ָ���ID��,�±��Ӧ����
  6,              //���ѵĸ��˵�ID��
  0,              //�洢pv���е�ID��(pvAryID):
  TMENU_NOTIFY_PASS(NULL,TMENU_CBFUNID_NULL_MENU), //���û��ռ佻����ͨ������
};

void MOtherSetNotify(unsigned char Type, void *pv)//����˵��ṹ����
{
    struct _SubMenuUser *pUser = (struct _SubMenuUser*)pv;
    //Type = Type;//ͨ������
    switch(pUser->Sel)
    {
     case 1:    vTCH12Adj();	//�¶�У׼
                break;
     case 2:    vTCH34Adj();    //�¶�У׼
                break;
     case 3:    vReInitAll();   //�ָ���������
                break;
     case 0:    pUser->UserFlag = 1; return;//Ϊ���ֱ���˳�,�������ϼ��˵�   
     default:return;
     }
     pUser->UserFlag = 1;//Ϊ����

     TMenu_GogoMenu(NULL,   //ǿ������Ĳ˵��ṹ
                    2,      //�ɲ˵�ID,��ЧʱΪ0
                    0);     //�ýṹ���ڲ˵��ڵĲ��
}

code TMenu_t MOtherSet =
{
  //�˵�����
  TMTYPE_SUBMENU, //�˵�����, 
  4,              //�ɲ˵����;�����������ݴ�С
  {15},            //�������֧�ֵĲ˵�ͷ�ָ���ID��,�±��Ӧ����
  2,              //���ѵĸ��˵�ID��
  6,              //�洢pv���е�ID��(pvAryID):
  TMENU_NOTIFY_PASS(MOtherSetNotify,TMENU_CBFUNID_NULL_MENU), //���û��ռ佻����ͨ������
};

//---------------------��ֵ�����˵��ṹʾ��----------------------------
static code const struct _MNumDesc SpraySetDesc[] = {
  {MNUM_TYPE_DEC | MNUM_TYPE_GRP_MODE,0,50},//xxxx�趨
  {MNUM_TYPE_DEC | MNUM_TYPE_GRP_MODE, 0,80},//xxxx2�趨
};

#include "Apublic.h"  //�翾�¶ȵ�
void MSpraySetNotify(unsigned char Type, void *pv)//��������˵��ṹ����
{
  struct _MNumAdjUser *pUser = (struct _MNumAdjUser*)pv;
  switch(Type)
  {
    case TM_NOTIFY_GET_DATA://��������
        pUser->Value[0] = Static.Prt_PQ.TempSet;
        pUser->Value[1] = Static.Prt_KQ.TempSet;
        break;
    case TM_NOTIFY_SET_DATA://��������
        Static.Prt_PQ.TempSet = pUser->Value[0];
        Static.Prt_KQ.TempSet = pUser->Value[1];

        //�洢E2

        break;
    case TM_NOTIFY_MNUM_GET_DESC://�õ���ֵ����
        memcpy_P(&pUser->Desc,&SpraySetDesc[pUser->CurItem],sizeof(struct _MNumDesc));
        break;
    default:break;
  }
}

code TMenu_t MSpraySet =
{
  //�˵�����
  TMTYPE_MNUMADJ | TM_MNUMADJ_WRITE | /*TM_MNUMADJ_LOOP_MODE | */TM_MNUMADJ_ENTER_KEY, 
  2,              //�ɲ˵����;�����������ݴ�С
  {3},            //�������֧�ֵĲ˵�ͷ�ָ���ID��,�±��Ӧ����
  2,              //���ѵĸ��˵�ID��
  3,              //�洢pv���е�ID��(pvAryID):
  TMENU_NOTIFY_PASS(MSpraySetNotify,TMENU_CBFUNID_NULL_MENU), //���û��ռ佻����ͨ������
};


//---------------------------�˵��ṹ----------------------------
code TMenu_t TopMenu =
{
  TMTYPE_SUBMENU, //�˵�����
  _TopMenuAVR.SubMenuAmount,              //�ɲ˵����;�����������ݴ�С
  {2},            //�������֧�ֵĲ˵�ͷ�ָ���ID��,�±��Ӧ����
  0,              //���ѵĸ��˵�ID��
  2,              //�洢pv���е�ID��(pvAryID):
  TMENU_NOTIFY_PASS(NULL,TMENU_CBFUNID_NULL_MENU), //���û��ռ佻����ͨ������
};


//---------------------------��������˵��ṹ----------------------------
static code const struct _MNumDesc PassWordDesc[] = {
  {MNUM_TYPE_DEC,0,9999},//�����趨��Χ
};

#include "Apublic.h"  //����
void MPassWordNotify(unsigned char Type, void *pv)//��������˵��ṹ����
{
  struct _MNumAdjUser *pUser = (struct _MNumAdjUser*)pv;
  switch(Type)
  {
    case TM_NOTIFY_GET_DATA://��������//Ԥ��ʾ����
        pUser->Value[0] = 6666;//Static.PassWord; 
        break;
    case TM_NOTIFY_SET_DATA://��������
        /*20100326ǰ
        if((pUser->Value[0] == 5138) || (pUser->Value[0] == Static.PassWord))
        {
            TMenu_GogoMenu(NULL,   //ǿ������Ĳ˵��ṹ
                            2,      //�ɲ˵�ID,��ЧʱΪ0
                            0);     //�ýṹ���ڲ˵��ڵĲ��
        }
        else
        {
            //TMenu_ExitMenu();
            //ePKState = PKS_PWait;//������󷵻ص���������ʱ ������δ���!!

            TMenu_GogoMenu(NULL,   //ǿ������Ĳ˵��ṹ
                            1,      //�ɲ˵�ID,��ЧʱΪ0
                            0);     //�ýṹ���ڲ˵��ڵĲ��
        }
        */
        GetPurview(pUser->Value[0]);
        TMenu_GogoMenu(NULL,   //ǿ������Ĳ˵��ṹ
                        2,      //�ɲ˵�ID,��ЧʱΪ0
                        0);     //�ýṹ���ڲ˵��ڵĲ��
        break;
    case TM_NOTIFY_MNUM_GET_DESC://�õ���ֵ����
        memcpy_P(&pUser->Desc,&PassWordDesc[pUser->CurItem],sizeof(struct _MNumDesc));
        break;
    default:break;
  }
}

code TMenu_t PassWord =
{
  TMTYPE_MNUMADJ | TM_MNUMADJ_WRITE | TM_MNUMADJ_LOOP_MODE | TM_MNUMADJ_ENTER_KEY, 
  1,              //�ɲ˵����;�����������ݴ�С
  {1},            //�������֧�ֵĲ˵�ͷ�ָ���ID��,�±��Ӧ����
  0,              //���ѵĸ��˵�ID��
  1,              //�洢pv���е�ID��(pvAryID):
  TMENU_NOTIFY_PASS(MPassWordNotify,TMENU_CBFUNID_NULL_MENU), //���û��ռ佻����ͨ������
};

