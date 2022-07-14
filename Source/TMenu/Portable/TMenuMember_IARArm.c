/**************************************************************************
                        TMenu��س�Ա��������ʵ��
  ��û�ж���TMENU_USE_CONST_BUFʱ��غ�����ʵ��,�����û���ʵ�����к���
  Menu_t�ṹ�е�ʹ��ָ������г�Աͨ���˺�����������Ϣ
**************************************************************************/
#include "TMenuCfg.h"
#include "TMenu.h"
#include <string.h>

extern const TMenu_t MGetPower; //������code���Ķ���˵�

#ifdef TM_EN_LUT
static unsigned char _ItemLUT[TM_ITEM_MAX + 1];//���嵱ǰ���ұ�

#ifdef TM_EN_DYNC_LUT  //��̬���ұ�֧��ʱ
  static unsigned short _DyncLutSize; //��̬���ұ��С,��>=128ʱ����
  static unsigned short _DyncLutItem; //��̬���ұ���
#endif
  
#endif //TM_EN_LUT

//----------------------������ұ�----------------------------
#ifdef TM_EN_LUT
void _GetItemLUT(const TMenu_t *pMenu)
{
  if(!((pMenu)->Size & TM_TYPE_ITEM_LUT)) return;
  
  #ifdef TM_EN_DYNC_LUT  //��̬���ұ�֧��ʱ
    _DyncLutSize = 0; //�û�����Ӧʱ��֧��
    TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_GET_DLUT_SIZE, &_DyncLutSize);
    if(_DyncLutSize != 0) return; //ʹ�ö�̬���ұ��ȡ����
  #endif
   
  //���û��ռ�������
  TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_GET_LUT,_ItemLUT);
}
#else
  #define _GetItemLUT(pMenu) do{ }while(0)
#endif

//----------------------------�õ����ұ�ֵ--------------------------------
#ifdef TM_EN_DYNC_LUT  //��̬���ұ�֧��ʱ
unsigned short _GetLutItem(const TMenu_t *pMenu,
                            unsigned short Item)
{
  if(!(pMenu->Size & TM_TYPE_ITEM_LUT)) return Item;
  
  if(_DyncLutSize){ //��ǰΪ��̬���ұ�ʱ���û��ռ��ȡ����
    _DyncLutItem = Item;
    TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_GET_DLUT_ITEM, &_DyncLutItem);
    return _DyncLutItem;
  }
  return _ItemLUT[Item + 1];
}
  #define _GetItemVol(pmenu, item) _GetLutItem(pmenu, item);
#else
  #define _GetItemVol(pmenu, item) _ItemLUT[item + 1];
#endif

//-----------------------�õ�����˵�--------------------------
const TMenu_t *TM_pGetTopMenu(void)
{
  //���ݽ������������˵��Ľṹ
  const TMenu_t *pTopMenu = TM_pcbGetUserTopMenu();
      
  _GetItemLUT(pTopMenu); //������ұ�
  return pTopMenu;      //ָ�򻺴�
}

//---------------------�õ����˵��ṹ����----------------------------
const TMenu_t *TM_pGetParent(const TMenu_t *pMenu)
{
  #ifdef TM_DYNC_MENU   //��̬�˵�֧��ʱͨ�����¸��˵�
   if(pMenu->cbNotify)
     TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_UPDATE_PARENT,NULL);
  #endif
   
  pMenu = pMenu->pParent;
  
  if(pMenu != NULL) _GetItemLUT(pMenu); //û���˳��˵�ʱ�� ������ұ�
  return pMenu;
}

//-------------------------����Ӳ˵��ṹ����------------------------
const TMenu_t *TM_pGetSubMenu(const TMenu_t *pMenu,
                              unsigned char SubMenuID)
{
  TMenu_t const *const *PAry = (TMenu_t const *const *)pMenu->pv;  //C51�ķ�ʽ

  #ifdef TM_EN_LUT//�б�ʱ�Ȳ��
    SubMenuID = _GetItemVol(pMenu, SubMenuID);
  #endif
  
  #ifdef TM_DYNC_MENU   //��̬�˵�֧��ʱͨ�������Ӳ˵�
   if(pMenu->cbNotify)
    TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_UPDATE_SUB,&SubMenuID);
  #endif
    
  pMenu = *(PAry + SubMenuID); 
  _GetItemLUT(pMenu); //������ұ�
  return pMenu;
}

//-------------------------���ָ���˵��ṹ����------------------------
const TMenu_t *TM_pGetMenu(const TMenu_t *pMenu,   //ǿ����ת�Ĳ˵��ṹ
                           unsigned short MenuID) //���ɲ���,�����ڲ˵�ʶ��
{
  MenuID = MenuID;
  _GetItemLUT(pMenu); //������ұ�
  return pMenu;   //�ýṹ����ָ���Ĳ˵�
}

//-------------------�Ӳ˵��ṹ��ò˵�ͷ����-------------------
const char *TM_GetHeader(const TMenu_t *pMenu)
{
  char *pBuf;
  if(pMenu->pgHeader)
    return pLanCodeToChar(pMenu->pgHeader);

  //�˵���û��,���û��ռ��ȡ:
  pBuf = TMenu_pGetBuf();
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_HEADER,pBuf);
  return pBuf;
}

//-------------------�Ӳ˵��ṹ������ͺ���------------------------
unsigned char TM_GetType(const TMenu_t *pMenu)
{
  return pMenu->Type;
}

//-------------------�Ӳ˵��ṹ��ô�С����------------------------
#ifdef TM_EN_DYNC_LUT  //��̬���ұ�֧��ʱ
  unsigned short TM_GetSize(const TMenu_t *pMenu)
#else
  unsigned char TM_GetSize(const TMenu_t *pMenu)
#endif
{
  #ifdef TM_EN_LUT  //�в��ұ�ʱ�ض��򵽲��ұ��С:
  if(pMenu->Size & TM_TYPE_ITEM_LUT){
    #ifdef TM_EN_DYNC_LUT  //��̬���ұ�֧��ʱ
      if(_DyncLutSize) return _DyncLutSize; //Ϊ��̬���ұ�ʱ
    #endif
    return _ItemLUT[0];
  }
  #endif  

  return pMenu->Size;
}

//----------------�õ�ָ���Ӳ˵���ͷ����-------------------
const char *TM_GetSubMenuHeader(const TMenu_t *pMenu,
                                unsigned char Item)
{
  //�õ��Ӳ˵��ṹ
  TMenu_t const *const *PAry = (TMenu_t const *const *)pMenu->pv;  //C51�ķ�ʽ

  #ifdef TM_EN_LUT//�б�ʱ�Ȳ��
    #ifdef TM_EN_RPC
      unsigned char PrvItem = Item;//��ǰ
    #endif
    Item = _GetItemVol(pMenu, Item);
  #endif

  //RPCģʽʱ,�Ӳ˵����ܿ��ܲ�����, �ȴ��û��ռ����Ӳ˵�ͷ,�����ܻ�ȡ�ټ���
  #ifdef TM_EN_RPC   //�������Ӳ˵��л�ȡ 
    char *pBuf = TMenu_pGetBuf();
    *pBuf = Item;
    *(pBuf + 1) = '\0';//������־
    TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_SUB_HEADER, pBuf);
    if(*(pBuf + 1) != '\0') return pBuf; //���û��ռ�ֱ�ӻ����
    //�������
    #ifdef TM_EN_LUT//�б�ʱ,�ٳ�����ԭʼֵ���
      *pBuf = PrvItem;
      TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_SUB_HEADER_ORG, pBuf);
      if(*(pBuf + 1) != '\0') return pBuf; //���û��ռ�ֱ�ӻ����
    #endif //TM_EN_LUT
    //�������  
  #endif
  
  #ifdef TM_DYNC_MENU   //��̬�˵�֧��ʱͨ�������Ӳ˵�
    if(pMenu->cbNotify)
      TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_UPDATE_SUB,&Item);
  #endif
    
  pMenu = *(PAry + Item);
  return TM_GetHeader(pMenu);  
}

//-------------------�Ӳ˵��ṹ������ַ�������-------------------
const char *TM_GetItemString(const TMenu_t *pMenu,
                             unsigned char Item)
{
  char *pBuf;
  #ifdef TM_EN_LUT  //�в��ұ�ʱ�ض��򵽲��ұ�
    Item = _GetItemVol(pMenu, Item);
  #endif

  if(pMenu->pv){
    #ifdef TM_DYNC_MENU   //��̬�˵�֧��ʱͨ�������Ӳ˵�
     if(pMenu->cbNotify)
      TMENU_NOTIFY_RUN((pMenu)->cbNotify,TM_NOTIFY_USER_UPDATE_SUB, &Item);
    #endif
    const LanCode_t *pCode = *((const LanCode_t **)(pMenu->pv) + Item);
    return pLanCodeToChar(pCode);
  }

  //���û��ռ�������
  pBuf = TMenu_pGetBuf();
  *pBuf = Item;
  TMENU_NOTIFY_RUN(pMenu->cbNotify,TM_NOTIFY_GET_ITEM,pBuf);
  return pBuf;
}

//-------------------ͳ�Ʋ˵�������Ⱥ���-----------------------
TItemSize_t TM_GetItemMaxLen(const TMenu_t *pMenu)
{
  //���ұ�֧��
  unsigned char Size = TM_GetSize(pMenu);
  unsigned char i;
  unsigned char Type = pMenu->Type & TMTYPE_MASK;  
  TItemSize_t CurW, ItemW = 0;

  if((Type == TMTYPE_SUBMENU) || (Type == TMTYPE_EDITSEL)){//�Ӳ˵�ģʽ���Ӳ˵���ȡ����
    for(i = 0; i < Size; i++){
      CurW = strlen(TM_GetSubMenuHeader(pMenu,i));
      if(CurW > ItemW) ItemW = CurW;
    }
  }
  else{//ֱ����Ϊ��pv�Ｔ���ַ���
    for(i = 0; i < Size; i++){
      CurW = strlen(TM_GetItemString(pMenu,i));
      if(CurW > ItemW) ItemW = CurW;
    }
  }
  return ItemW;
}

//-------------------���ݵ�ǰ�˵����ұ�����ѡλ��---------------------
unsigned char TMenu_GetItemPosWithLUT(unsigned char Pos)
{
  return _ItemLUT[Pos + 1];
}
    



