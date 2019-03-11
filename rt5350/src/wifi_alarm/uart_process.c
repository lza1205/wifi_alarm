
#include "all.h"
#include "client.h"


/* 串口1 连接到 RT5350。处理RT5350的指令
指令格式如下
	SRC::TYPE::COMMAND::VAL

	SRC:	源地址
	type:	类型，目前有:
					GSM		GSM 事件
	COMMAND	命令
	VAL			参数
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


/* 对接收的串口数据做处理 */
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
			/* 收到无线数据。跟配置文件对比。看下是什么数据 */
			rf_recv_process(arg[3]);
		}
	}else if(strcmp(arg[1], "GSM") == 0){
		;
	}else{
		;
	}
}


