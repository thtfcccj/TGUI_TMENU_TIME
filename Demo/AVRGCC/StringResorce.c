/* ----------------------------------------------------------------------------
 *               AVR GCC���ַ�����Դ�ļ�
 * --------------------------------------------------------------------------*/

#include "string.h"

#include "TMenuBase.h"

/*******************************************************************************
                      �ַ�������
********************************************************************************/

/* 1 */static code const char LC_PassWord_Ch[]={"����"};
/* 1 */static code const char LC_TopMenu_Ch[]={"ϵͳ�˵�"};

/* 2 */static code const char LC_SprayPaintSet_Ch[]={"XXXX����"};
/* 3 */static code const char LC_SprayPaintTemp_Ch[]={"XXXX�¶�"};

/*******************************************************************************
                 ���ַ���ID�ŵõ��ַ���ͷ�����ص�����ʵ��
********************************************************************************/

const char *cbTM_pGetString(TStringSize_t StringID)
{
  switch(StringID){
  	case 0: return  NULL;    //�̶����ַ���
    case 1: return  LC_PassWord_Ch;
  	case 2: return  LC_TopMenu_Ch;

  	case 3: return  LC_SprayPaintSet_Ch;
  	case 4: return  LC_SprayPaintTemp_Ch;

  	default:break;
  }
  return NULL;
}




