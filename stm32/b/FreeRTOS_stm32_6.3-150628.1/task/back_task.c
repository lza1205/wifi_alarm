
#define __BACK_TASK_C_

#include "all.h"



/* 系统1S 后台处理时间 */
void __back_1s_handle(void)
{
	/* 7185 初始化尚未成功 */
	
}

char *  rebuff;

void __test_gsm(void)
{
    printp("\r\n野火WF-SIM900A模块拨号例程\r\n"); 
    
    //检测模块响应是否正常
    while(sim900a_cmd("AT\r","OK",1000) != SIM900A_TRUE)
    {
      printp("\r\n模块响应测试不正常！！\r\n");
      printp("\r\n若模块响应测试一直不正常，请检查模块的连接或是否已开启电源开关\r\n");
    }
			
		while(sim900a_cmd("ATE0\r","OK",1000) != SIM900A_TRUE)
    {
      printp("\r\n模块响应测试不正常！！\r\n");
      printp("\r\n若模块响应测试一直不正常，请检查模块的连接或是否已开启电源开关\r\n");
    }
		
	while(sim900a_cmd("AT+COLP=1\r","OK",1000) != SIM900A_TRUE)
    {
      printp("\r\n模块响应测试不正常！！\r\n");
      printp("\r\n若模块响应测试一直不正常，请检查模块的连接或是否已开启电源开关\r\n");
    }
		
    printp("\r\n通过了模块响应测试，5秒后开始拨号测试...\r\n");
    
    /* 延时3秒再发送命令到模块 */
    SIM900A_DELAY(5000);
    
    //拨打电话
  
    sim900a_call("13510979604");                      //拨打112电话测试
    rebuff = sim900a_waitask(0);

	if(strstr(rebuff,"ATD") != NULL)
	{
		/* sim900a 会返回 ATDxxxxxxx 。先过滤掉 */
		SIM900A_CLEAN_RX();
		rebuff = sim900a_waitask(0);
	}
   
    if(strstr(rebuff,"OK") != NULL)           //响应OK，表示模块正常接收到命令
    {
      printp("\r\n正在呼叫\r\n");
      SIM900A_CLEAN_RX();                     //清除接收缓冲区
      rebuff = sim900a_waitask(0);            //重新等待消息
     
      if(strstr(rebuff,"NO CARRIER") != NULL) //响应NO CARRIER,通话结束
       {
         printp("\r\n通话结束\r\n");
       }
      else if(strstr(rebuff,"NO ANSWER") != NULL)   //响应NO ANSWER，无人接听
      {
        printp("\r\n你拨打的电话暂时无人接听，请稍后再拨\r\n");
      }  

    } 

    

}


void __test_sms(void)
{    //检测模块响应是否正常
    while(sim900a_cmd("AT\r","OK",1000) != SIM900A_TRUE)
    {
      printp("\r\n模块响应测试不正常！！\r\n");
      printp("\r\n若模块响应测试一直不正常，请检查模块的连接或是否已开启电源开关\r\n");
    }
    
    printp("\r\n通过了模块响应测试，5秒后开始发送短信测试... \r\n");
   
    /* 延时5秒再发送命令到模块 */
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
	msg_set_group(p, MSG_GOURP_TIME);		//监听输入信息

//	__test_gsm();

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

