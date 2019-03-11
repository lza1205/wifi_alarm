
#include "all.h"
#include "client.h"


/* ����1 ���ӵ� RT5350������RT5350��ָ��
ָ���ʽ����
	SRC::TYPE::COMMAND::VAL

	SRC:	Դ��ַ
	type:	���ͣ�Ŀǰ��:
					GSM		GSM �¼�
	COMMAND	����
	VAL			����
*/

char *arg[4];
static void __decode(char *str, int len)
{
	char *p, *pp;

	arg[0] = str;
	p = strstr(str, "::");
	if(p != NULL)
	{
		*p = '\0';
		*(p + 1) = '\0';
	}else{
		return ;
	}

	pp = p + 2;

	arg[1] = pp;
	p = strstr(pp, "::");
	if(p != NULL)
	{
		*p = '\0';
		*(p + 1) = '\0';
	}else{
		return ;
	}

	pp = p + 2;

	arg[2] = pp;
	p = strstr(pp, "::");
	if(p != NULL)
	{
		*p = '\0';
		*(p + 1) = '\0';
	}else{
		return ;
	}

	arg[3] = p + 2;
}


/* �Խ��յĴ������������� */
void uart_recv_process(char *buf, int len)
{
	__decode(buf, strlen(buf));

	if(strcmp(arg[0], "STM32") != 0)
	{
		dbg_printf("uart data not STM32\r\n");
		return ;
	}

	if(strcmp(arg[1], "RF") == 0)
	{
		if(strcmp(arg[2], "CODE") == 0)
		{
			/* �յ��������ݡ��������ļ��Աȡ�������ʲô���� */
			rf_recv_process(arg[3]);
		}
	}else if(strcmp(arg[1], "GSM") == 0){
		;
	}else{
		;
	}
}


