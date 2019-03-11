#define __INPUT_TASK_C_

#include "all.h"





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
				;
				break;
		}
	}
}

