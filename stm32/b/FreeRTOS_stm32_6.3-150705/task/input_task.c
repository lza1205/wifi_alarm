#define __INPUT_TASK_C_

#include "all.h"
#include "string.h"

/* 串口1 连接到 RT5350。处理RT5350的指令
指令格式如下
	SRC::TYPE::COMMAND::VAL

	SRC:	源地址
	type:	类型，目前有:
					GSM		GSM 事件
	COMMAND	命令
	VAL			参数
*/

u8 *arg[4];
static void __decode(u8 *str, u16 len)
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




void uart1_recv(void)
{
	if((FlagMainUartOK == 1) && (COM1_RX_TimeOut == 0))
	{
		FlagMainUartOK = 0;
		COM1_RX_Cnt = 0;
	}

	/* 测试 */
//	printp(COM1_RX_Buffer);

	__decode(COM1_RX_Buffer, COM1_RX_Cnt);

	if(strcmp(arg[0], "RT5350") != 0)
	{
		return ;
	}

	if(strcmp(arg[1], "GSM") == 0)
	{
		if(strcmp(arg[2], "Dial") == 0)
		{
			/* 拨打电话 */
			/* 为了响应速度。我们让另外一个线程去做这件事 */
			strcpy(call_phone, arg[3]);
			msg_set(MSG_GOURP_GSM, MSG_GSM_DIAL);
		}
	}else if(strcmp(arg[1], "RF") == 0){
		;
	}else{
		;
	}
}


void input_task(void *pdata)
{
	u8 ret;
	u32 msg;

	struct msg_head_list *p;

	printp("start input_task \r\n");

	p = msg_init_group();
	msg_set_group(p, MSG_GOURP_INPUT);		//监听输入信息

	__msg_ok = 1;

	/* 改变自身优先级 */
	vTaskPrioritySet(NULL, 4);
	printp("vTaskPrioritySet input_task \r\n");

	while(1)
	{
		ret = msg_get(p, &msg, 1000);

		if(ret != 0)
			continue;

		switch(msg){
			case IS_MSG(MSG_GOURP_INPUT, MSG_INPUT_EV1527):
				send_rf_data();
				break;
			case IS_MSG(MSG_GOURP_INPUT, MSG_INPUT_UART1):
				uart1_recv();
				break;
		}
	}
}

