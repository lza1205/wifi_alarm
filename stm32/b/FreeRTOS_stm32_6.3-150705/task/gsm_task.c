
#define __GSM_TASK_C_

#include "all.h"

void dail_phone(void)
{
	//拨打电话
	printp("开始拨号 : [%s]\r\n", call_phone);
	
    sim900a_call(call_phone);                      //拨打电话测试
    rebuff = sim900a_waitask(0);
#if 0
	if(strstr(rebuff,"ATD") != NULL)
	{
		/* sim900a 会返回 ATDxxxxxxx 。先过滤掉 */
		printp(rebuff);
		SIM900A_CLEAN_RX();
		rebuff = sim900a_waitask(0);
	}
 #endif
    if(strstr(rebuff,"OK") != NULL)           //响应OK，表示模块正常接收到命令
    {
    	printp(rebuff);
		printp("\r\n正在呼叫\r\n");
		SIM900A_CLEAN_RX();                     //清除接收缓冲区
		rebuff = sim900a_waitask(0);            //重新等待消息

		printp(rebuff);
		if(strstr(rebuff,"NO CARRIER") != NULL) //响应NO CARRIER,通话结束
		{
			printp("\r\n通话结束\r\n");
		}
		else if(strstr(rebuff,"NO ANSWER") != NULL)   //响应NO ANSWER，无人接听
		{
			printp("\r\n你拨打的电话暂时无人接听，请稍后再拨\r\n");
		}
    }else{
    	printp(rebuff);
		printp("拨号失败\r\n");
	}
}


void gsm_init(void)
{
	printp("gsm init...\r\n");
	
    //检测模块响应是否正常
    while(sim900a_cmd("AT\r","OK",1000) != SIM900A_TRUE)
    {

    }
	printp("GSM AT is ok \r\n");

	/* 不要回显 */
	while(sim900a_cmd("ATE0\r","OK",1000) != SIM900A_TRUE)
    {

    }
	printp("GSM ATE0 is ok \r\n");

	
	/* 检测卡是否存在 */
	while(sim900a_cmd("AT+CPIN?\r","+CPIN: READY",1000) != SIM900A_TRUE)
    {
    }
	printp("AT+CPIN? READY\r\n");


	/* 设置被叫号码显示 */	
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
	msg_set_group(p, MSG_GOURP_GSM);		//监听GSM 事件

	/* GSM 模块初始化 */
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

