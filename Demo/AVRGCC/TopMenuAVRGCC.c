/* ----------------------------------------------------------------------------
 *               顶层菜单结构文件
 * --------------------------------------------------------------------------*/


#include "TMenu_CheckBox.h"
#include "TMenu_MNumAdj.h"
//#include "TMenu_RadioBox.h"
#include "TMenu_SubMenu.h

#include "TMenuBase.h"
#include "string.h"


//----------------------用户权限管理---------------------
struct __TopMenuAVR{
    unsigned char SubMenuAmount;//子菜单数量
}_TopMenuAVR;
void GetPurview(unsigned short PassWord)
{
    //每次进入菜单前进行权限纠错?
    //if(_TopMenuAVR._eUser > Guest)_TopMenuAVR._eUser = Guest;
    if(PassWord == 5138)_TopMenuAVR.SubMenuAmount = 5;
    else if(PassWord == Static.PassWord)_TopMenuAVR.SubMenuAmount = 4;
    else _TopMenuAVR.SubMenuAmount = 2;
}

//============================设置菜单===========================
#include "ARelay.h"
void HandCtrlNotify(unsigned char Type, void *pv)//菜单结构函数
{
  unsigned char Item;
  struct _CheckboxUser *pUser = (struct _CheckboxUser*)pv;
  switch(Type){
  case TM_NOTIFY_GET_DATA://加载数据
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
  case TM_NOTIFY_SET_DATA://保存数据
  case TM_NOTIFY_CHANGED://某项被改变了
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
  //菜单类型
  TMTYPE_CHECKBOX, 
  8,              //由菜单类型决定的相关数据大小
  {19},            //多国语言支持的菜单头字附串ID号,下标对应语种
  2,              //自已的父菜单ID号
  7,              //存储pv阵列的ID号(pvAryID):
  TMENU_NOTIFY_PASS(HandCtrlNotify,TMENU_CBFUNID_NULL_MENU), //与用户空间交互的通报函数
};


//---------------------其他设置织菜单结构----------------------------
//#include "???.h"	//恢复出厂设置初始化

void AC220Notify(unsigned char Type, void *pv)//顶层菜单结构函数
{
    char *pBuf;
    switch(Type)
    {
        case TM_NOTIFY_GET_HEADER:
        
        pBuf = (char*)pv;
        strcpy_P(pBuf,LC_AC220_CH);
        pBuf += sizeof(LC_AC220_CH) - 1;

        pBuf += Value2StringMin(uRePairBuffer[CH5_V1],
                                pBuf,//接收缓冲
                                3);//保证的最小位数
        *pBuf++= 'V';
        *pBuf = '\0';

        break;
        default : break;
    }
}
//电压校准
code TMenu_t MAC220Set =
{
  //菜单类型
  TMTYPE_CMD,       //菜单类型为命令模式
  0,                //由菜单类型决定的相关数据大小
  {0},            //多国语言支持的菜单头字附串ID号,下标对应语种//为0时去回调函数中去查找菜单头
  6,              //自已的父菜单ID号
  0,              //存储pv阵列的ID号(pvAryID)://命令形菜单无 PV ID号
  TMENU_NOTIFY_PASS(AC220Notify,TMENU_CBFUNID_NULL_MENU), //与用户空间交互的通报函数
};

//1温度校准12
code TMenu_t MTCh12Adjust =
{
  //菜单类型
  TMTYPE_CMD,       //菜单类型为命令模式
  0,                //由菜单类型决定的相关数据大小
  {16},            //多国语言支持的菜单头字附串ID号,下标对应语种
  6,              //自已的父菜单ID号
  0,              //存储pv阵列的ID号(pvAryID):
  TMENU_NOTIFY_PASS(NULL,TMENU_CBFUNID_NULL_MENU), //与用户空间交互的通报函数
};

//2温度校准34
code TMenu_t MTCh34Adjust =
{
  //菜单类型
  TMTYPE_CMD,       //菜单类型为命令模式
  0,                //由菜单类型决定的相关数据大小
  {17},            //多国语言支持的菜单头字附串ID号,下标对应语种
  6,              //自已的父菜单ID号
  0,              //存储pv阵列的ID号(pvAryID):
  TMENU_NOTIFY_PASS(NULL,TMENU_CBFUNID_NULL_MENU), //与用户空间交互的通报函数
};

//3恢复出厂设置
code TMenu_t MRecoverSet =
{
  //菜单类型
  TMTYPE_CMD,       //菜单类型为命令模式
  0,                //由菜单类型决定的相关数据大小
  {18},            //多国语言支持的菜单头字附串ID号,下标对应语种
  6,              //自已的父菜单ID号
  0,              //存储pv阵列的ID号(pvAryID):
  TMENU_NOTIFY_PASS(NULL,TMENU_CBFUNID_NULL_MENU), //与用户空间交互的通报函数
};

void MOtherSetNotify(unsigned char Type, void *pv)//顶层菜单结构函数
{
    struct _SubMenuUser *pUser = (struct _SubMenuUser*)pv;
    //Type = Type;//通报类型
    switch(pUser->Sel)
    {
     case 1:    vTCH12Adj();	//温度校准
                break;
     case 2:    vTCH34Adj();    //温度校准
                break;
     case 3:    vReInitAll();   //恢复出厂设置
                break;
     case 0:    pUser->UserFlag = 1; return;//为命令；直接退出,不返回上级菜单   
     default:return;
     }
     pUser->UserFlag = 1;//为命令

     TMenu_GogoMenu(NULL,   //强制跳入的菜单结构
                    2,      //可菜单ID,无效时为0
                    0);     //该结构所在菜单内的层次
}

code TMenu_t MOtherSet =
{
  //菜单类型
  TMTYPE_SUBMENU, //菜单类型, 
  4,              //由菜单类型决定的相关数据大小
  {15},            //多国语言支持的菜单头字附串ID号,下标对应语种
  2,              //自已的父菜单ID号
  6,              //存储pv阵列的ID号(pvAryID):
  TMENU_NOTIFY_PASS(MOtherSetNotify,TMENU_CBFUNID_NULL_MENU), //与用户空间交互的通报函数
};

//---------------------多值调整菜单结构示例----------------------------
static code const struct _MNumDesc SpraySetDesc[] = {
  {MNUM_TYPE_DEC | MNUM_TYPE_GRP_MODE,0,50},//xxxx设定
  {MNUM_TYPE_DEC | MNUM_TYPE_GRP_MODE, 0,80},//xxxx2设定
};

#include "Apublic.h"  //喷烤温度等
void MSpraySetNotify(unsigned char Type, void *pv)//设置密码菜单结构函数
{
  struct _MNumAdjUser *pUser = (struct _MNumAdjUser*)pv;
  switch(Type)
  {
    case TM_NOTIFY_GET_DATA://加载数据
        pUser->Value[0] = Static.Prt_PQ.TempSet;
        pUser->Value[1] = Static.Prt_KQ.TempSet;
        break;
    case TM_NOTIFY_SET_DATA://保存数据
        Static.Prt_PQ.TempSet = pUser->Value[0];
        Static.Prt_KQ.TempSet = pUser->Value[1];

        //存储E2

        break;
    case TM_NOTIFY_MNUM_GET_DESC://得到数值描述
        memcpy_P(&pUser->Desc,&SpraySetDesc[pUser->CurItem],sizeof(struct _MNumDesc));
        break;
    default:break;
  }
}

code TMenu_t MSpraySet =
{
  //菜单类型
  TMTYPE_MNUMADJ | TM_MNUMADJ_WRITE | /*TM_MNUMADJ_LOOP_MODE | */TM_MNUMADJ_ENTER_KEY, 
  2,              //由菜单类型决定的相关数据大小
  {3},            //多国语言支持的菜单头字附串ID号,下标对应语种
  2,              //自已的父菜单ID号
  3,              //存储pv阵列的ID号(pvAryID):
  TMENU_NOTIFY_PASS(MSpraySetNotify,TMENU_CBFUNID_NULL_MENU), //与用户空间交互的通报函数
};


//---------------------------菜单结构----------------------------
code TMenu_t TopMenu =
{
  TMTYPE_SUBMENU, //菜单类型
  _TopMenuAVR.SubMenuAmount,              //由菜单类型决定的相关数据大小
  {2},            //多国语言支持的菜单头字附串ID号,下标对应语种
  0,              //自已的父菜单ID号
  2,              //存储pv阵列的ID号(pvAryID):
  TMENU_NOTIFY_PASS(NULL,TMENU_CBFUNID_NULL_MENU), //与用户空间交互的通报函数
};


//---------------------------顶层密码菜单结构----------------------------
static code const struct _MNumDesc PassWordDesc[] = {
  {MNUM_TYPE_DEC,0,9999},//密码设定范围
};

#include "Apublic.h"  //密码
void MPassWordNotify(unsigned char Type, void *pv)//设置密码菜单结构函数
{
  struct _MNumAdjUser *pUser = (struct _MNumAdjUser*)pv;
  switch(Type)
  {
    case TM_NOTIFY_GET_DATA://加载数据//预显示密码
        pUser->Value[0] = 6666;//Static.PassWord; 
        break;
    case TM_NOTIFY_SET_DATA://保存数据
        /*20100326前
        if((pUser->Value[0] == 5138) || (pUser->Value[0] == Static.PassWord))
        {
            TMenu_GogoMenu(NULL,   //强制跳入的菜单结构
                            2,      //可菜单ID,无效时为0
                            0);     //该结构所在菜单内的层次
        }
        else
        {
            //TMenu_ExitMenu();
            //ePKState = PKS_PWait;//密码错误返回到待机界面时 焦点区未清除!!

            TMenu_GogoMenu(NULL,   //强制跳入的菜单结构
                            1,      //可菜单ID,无效时为0
                            0);     //该结构所在菜单内的层次
        }
        */
        GetPurview(pUser->Value[0]);
        TMenu_GogoMenu(NULL,   //强制跳入的菜单结构
                        2,      //可菜单ID,无效时为0
                        0);     //该结构所在菜单内的层次
        break;
    case TM_NOTIFY_MNUM_GET_DESC://得到数值描述
        memcpy_P(&pUser->Desc,&PassWordDesc[pUser->CurItem],sizeof(struct _MNumDesc));
        break;
    default:break;
  }
}

code TMenu_t PassWord =
{
  TMTYPE_MNUMADJ | TM_MNUMADJ_WRITE | TM_MNUMADJ_LOOP_MODE | TM_MNUMADJ_ENTER_KEY, 
  1,              //由菜单类型决定的相关数据大小
  {1},            //多国语言支持的菜单头字附串ID号,下标对应语种
  0,              //自已的父菜单ID号
  1,              //存储pv阵列的ID号(pvAryID):
  TMENU_NOTIFY_PASS(MPassWordNotify,TMENU_CBFUNID_NULL_MENU), //与用户空间交互的通报函数
};

