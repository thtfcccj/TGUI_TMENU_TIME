/*******************************************************************************

			                  T6963C�ײ���������ʵ��

*******************************************************************************/
#include "T6963C_Command.h"

//--------------------------�ײ�IO�ڳ�ʼ��-------------------------------
/*/���û���ʼ����IO�ڵ�Ĭ��״̬
void T6963C_HwInit(void)
{

  
  
  
}*/

//--------------------------�ȴ�����------------------------------------
__arm static void _WaitIdie(unsigned char WaitType)
{
  unsigned char Data;
  T6963C_SelCmd();		    //ѡ��ָ��
  T6963C_SetIn();			//����Ϊ����  
  //T6963C_CsEn();	        //ʹ��(ѡͨ)
  for(unsigned char i = 10; i > 0; i--){
    T6963C_ClrRd();	        //��ѡ��
    T6963C_RdDelay();
    Data = T6963C_GetData();
    T6963C_SetRd();         //��ȡ��
    if((Data & WaitType) != WaitType) break;
  };
  //T6963C_CsDis();         //ȡ��Ƭѡ  
} 

//------------------------------д����------------------------------
__arm void T6963C_cbWrCmd(unsigned char Cmd)
{
  T6963C_CsEn();    
  _WaitIdie(T6963C_CMD_BUSY);//�ȴ�ָ�����
  //T6963C_SelCmd();         //ѡ��ָ��,(��Ϊָ��״̬)
  T6963C_SetOut();           //��Ϊ��� 
  T6963C_SetData(Cmd);       //������  
  T6963C_ClrWr();            //д����״̬
  T6963C_WrDelay();	         //��ʱ�ȴ�����д��
  T6963C_SetWr();            //ȡ��д����״̬  
  T6963C_CsDis();
}

//--------------------------˳��д����----------------------------------
//����T6963_StartAutoWr()�ɵ��ô˺�������д����
__arm void T6963C_SqureWrData(const unsigned char *pData,
                        unsigned short Len) 
{
  T6963C_CsEn();    
  _WaitIdie(T6963C_CMD_BUSY);//�ȴ�ָ�����
  T6963C_SelData();         //д���ݷ�ʽ  
  T6963C_SetOut();           //��Ϊ���
  while(Len--){              //дָ�����ݳ���
    T6963C_SetData(*pData++);   //������
    T6963C_ClrWr();            //д����״̬
    T6963C_WrDelay();	      //��ʱ�ȴ�����д��
    T6963C_SetWr();            //ȡ��д����״̬
  }
  T6963C_CsDis();
}

//---------------------------------д����----------------------------------
__arm void T6963C_cbWrData(unsigned char Data)
{
  T6963C_CsEn();    
  _WaitIdie(T6963C_DATA_BUSY);//�ȴ����ݿ���
  T6963C_SelData();         //д���ݷ�ʽ
  T6963C_SetOut();           //��Ϊ���   
  T6963C_SetData(Data);     //������
  T6963C_ClrWr();            //д����״̬
  T6963C_WrDelay();	      //��ʱ�ȴ�����д��
  T6963C_SetWr();            //ȡ��д����״̬
  T6963C_CsDis();
}

