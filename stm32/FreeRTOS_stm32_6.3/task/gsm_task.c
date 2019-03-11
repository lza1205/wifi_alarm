
#define __GSM_TASK_C_

#include "all.h"

void dail_phone(void)
{
	//����绰
	printp("��ʼ���� : [%s]\r\n", call_phone);
	
    sim900a_call(call_phone);                      //����绰����
    rebuff = sim900a_waitask(0);
#if 0
	if(strstr(rebuff,"ATD") != NULL)
	{
		/* sim900a �᷵�� ATDxxxxxxx ���ȹ��˵� */
		printp(rebuff);
		SIM900A_CLEAN_RX();
		rebuff = sim900a_waitask(0);
	}
 #endif
    if(strstr(rebuff,"OK") != NULL)           //��ӦOK����ʾģ���������յ�����
    {
    	printp(rebuff);
		printp("\r\n���ں���\r\n");
		SIM900A_CLEAN_RX();                     //������ջ�����
		rebuff = sim900a_waitask(0);            //���µȴ���Ϣ

		printp(rebuff);
		if(strstr(rebuff,"NO CARRIER") != NULL) //��ӦNO CARRIER,ͨ������
		{
			printp("\r\nͨ������\r\n");
		}
		else if(strstr(rebuff,"NO ANSWER") != NULL)   //��ӦNO ANSWER�����˽���
		{
			printp("\r\n�㲦��ĵ绰��ʱ���˽��������Ժ��ٲ�\r\n");
		}
    }else{
    	printp(rebuff);
		printp("����ʧ��\r\n");
	}
}


void gsm_init(void)
{
	printp("gsm init...\r\n");
	
    //���ģ����Ӧ�Ƿ�����
    while(sim900a_cmd("AT\r","OK",1000) != SIM900A_TRUE)
    {

    }
	printp("GSM AT is ok \r\n");

	/* ��Ҫ���� */
	while(sim900a_cmd("ATE0\r","OK",1000) != SIM900A_TRUE)
    {

    }
	printp("GSM ATE0 is ok \r\n");

	
	/* ��⿨�Ƿ���� */
	while(sim900a_cmd("AT+CPIN?\r","+CPIN: READY",1000) != SIM900A_TRUE)
    {
    }
	printp("AT+CPIN? READY\r\n");


	/* ���ñ��к�����ʾ */	
	while(sim900a_cmd("AT+COLP=1\r","OK",1000) != SIM900A_TRUE)
    {
    }
	printp("AT+COLP=1 READY\r\n");
}

void gsm_task(void *pdata)
{
	u8 ret;
	u32 msg;
	struct msg_head_list *p;

	printp("start gsm_task \r\n");
	
	p = msg_init_group();
	msg_set_group(p, MSG_GOURP_GSM);		//����GSM �¼�

	/* GSM ģ���ʼ�� */
	gsm_init();
	printp("gsm init is ok\r\n");
	
	for(; ; )
	{
		ret = msg_get(p, &msg, 1000);

		if(ret != 0)
			continue;

		switch(msg){
			case IS_MSG(MSG_GOURP_GSM, MSG_GSM_DIAL):
				dail_phone();
				break;
		}
	}
}

