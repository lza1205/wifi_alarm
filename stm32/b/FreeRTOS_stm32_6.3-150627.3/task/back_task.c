
#define __BACK_TASK_C_

#include "all.h"



/* ϵͳ1S ��̨����ʱ�� */
void __back_1s_handle(void)
{
	/* 7185 ��ʼ����δ�ɹ� */
	
}

char *  rebuff;

void __test_gsm(void)
{
    my_send_str("\r\nҰ��WF-SIM900Aģ�鲦������\r\n"); 
    
    //���ģ����Ӧ�Ƿ�����
    while(sim900a_cmd("AT\r","OK",1000) != SIM900A_TRUE)
    {
      my_send_str("\r\nģ����Ӧ���Բ���������\r\n");
      my_send_str("\r\n��ģ����Ӧ����һֱ������������ģ������ӻ��Ƿ��ѿ�����Դ����\r\n");
    }
			
		while(sim900a_cmd("ATE1\r","OK",1000) != SIM900A_TRUE)
    {
      my_send_str("\r\nģ����Ӧ���Բ���������\r\n");
      my_send_str("\r\n��ģ����Ӧ����һֱ������������ģ������ӻ��Ƿ��ѿ�����Դ����\r\n");
    }
		
		while(sim900a_cmd("AT+CPIN?\r","OK",1000) != SIM900A_TRUE)
    {
      my_send_str("\r\nģ����Ӧ���Բ���������\r\n");
      my_send_str("\r\n��ģ����Ӧ����һֱ������������ģ������ӻ��Ƿ��ѿ�����Դ����\r\n");
    }
		
		while(sim900a_cmd("AT+COLP=1\r","OK",1000) != SIM900A_TRUE)
    {
      my_send_str("\r\nģ����Ӧ���Բ���������\r\n");
      my_send_str("\r\n��ģ����Ӧ����һֱ������������ģ������ӻ��Ƿ��ѿ�����Դ����\r\n");
    }
		
    my_send_str("\r\nͨ����ģ����Ӧ���ԣ�5���ʼ���Ų���...\r\n");
    
    /* ��ʱ3���ٷ������ģ�� */
    SIM900A_DELAY(5000);
    
    //����绰
  
    sim900a_call("13510979604");                      //����112�绰����
    rebuff = sim900a_waitask(0);

	if(strstr(rebuff,"ATD") != NULL)
	{
		/* sim900a �᷵�� ATDxxxxxxx ���ȹ��˵� */
		SIM900A_CLEAN_RX();
		rebuff = sim900a_waitask(0);
	}
   
    if(strstr(rebuff,"OK") != NULL)           //��ӦOK����ʾģ���������յ�����
    {
      my_send_str("\r\n���ں���\r\n");
      SIM900A_CLEAN_RX();                     //������ջ�����
      rebuff = sim900a_waitask(0);            //���µȴ���Ϣ
     
      if(strstr(rebuff,"NO CARRIER") != NULL) //��ӦNO CARRIER,ͨ������
       {
         my_send_str("\r\nͨ������\r\n");
       }
      else if(strstr(rebuff,"NO ANSWER") != NULL)   //��ӦNO ANSWER�����˽���
      {
        my_send_str("\r\n�㲦��ĵ绰��ʱ���˽��������Ժ��ٲ�\r\n");
      }  

    } 

    

}


void __test_sms(void)
{    //���ģ����Ӧ�Ƿ�����
    while(sim900a_cmd("AT\r","OK",1000) != SIM900A_TRUE)
    {
      my_send_str("\r\nģ����Ӧ���Բ���������\r\n");
      my_send_str("\r\n��ģ����Ӧ����һֱ������������ģ������ӻ��Ƿ��ѿ�����Դ����\r\n");
    }
    
    my_send_str("\r\nͨ����ģ����Ӧ���ԣ�5���ʼ���Ͷ��Ų���... \r\n");
   
    /* ��ʱ5���ٷ������ģ�� */
    SIM900A_DELAY(5000);
 
	sim900a_sms("13510979604","hellow world");
}

void back_task(void *pdata)
{
	u8 ret;
	u32 msg;
	struct msg_head_list *p;

	printp("start back_task \r\n");
	
	p = msg_init_group();
	msg_set_group(p, MSG_GOURP_TIME);		//����������Ϣ

	__test_gsm();

	for( ; ; )
	{
		ret = msg_get(p, &msg, 1000);

		if(ret != 0)
			continue;

		switch(msg){
			case IS_MSG(MSG_GOURP_TIME, MSG_TIME_1S):
				__back_1s_handle();
				break;
		}
	}
}

