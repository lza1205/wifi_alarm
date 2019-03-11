#define __MSG_LIST_C_
//#include "app_config.h"
#include "msg_list.h"


u8 __msg_ok;

static xQueueHandle xQueue;
//static u16 msg_task_gourp = (u16)-1;

LIST_HEAD(msg_gourp_list);		//定义并初始化一个链表头
/* 总共有16 组 */
//struct list_head msg_gourp_list[16];



/* 发送消息 */
u8 msg_send(u16 type, u16 val, u8 mode)
{
#if (_USE_MSG_LIST_ == 1)

	u32 msg;
	portBASE_TYPE ret;

	portBASE_TYPE woken;

	if(__msg_ok == 0)
		return 0;

	msg = (type << 16) | (val);

	if(mode == MSG_MODE_NORMAL)
		ret = xQueueSendToBack(xQueue,&msg,0);
	else{
		woken = pdFALSE;
		ret = xQueueSendToBackFromISR(xQueue,&msg, &woken);
	}
	if(ret != pdPASS)
		return 1;
#endif
	return 0;
}


/* 接收消息 */
u8 msg_recv(u32 *val, u16 timeout)
{
	u32 msg;
	u16 t;
	u8 i;
	struct msg_head_list *pos; 
	
	const portTickType xTickToWait = timeout / portTICK_RATE_MS;
	portBASE_TYPE ret;

	ret = xQueueReceive(xQueue, &msg, xTickToWait);
	if(ret != pdPASS)
		return 1;

	t = (msg >> 16);
	list_for_each_entry_reverse(pos,&msg_gourp_list,list,struct msg_head_list)
	{
		for(i = 0; i < 16; i++){
			if((pos->type & (1 << i)) == t){
				xQueueSendToBack(pos->list_head, &msg, 0);
			}
		}
	}

	(*val) = msg;
	return 0;

}

/* 设置消息接收集 */
struct msg_head_list * msg_init_group(void)
{
#if (_USE_MSG_LIST_ == 1)

	struct msg_head_list *p;
	p = pvPortMalloc(sizeof(struct msg_head_list));
	if(p == NULL)
		return NULL;

	p->list_head = xQueueCreate(10, sizeof(long));
	if(p == NULL)
		return NULL;

	list_add(&p->list,&msg_gourp_list);
	
	return p;
#else
	return (struct msg_head_list *)0;
#endif
}

u8 msg_set_group(struct msg_head_list *p, u16 group)
{
#if (_USE_MSG_LIST_ == 1)

	p->type |= group;
#endif
	return 0;
}



u8 msg_get(struct msg_head_list *p, u32 *msg, u16 timeout)
{
//	u16 t;
//	u8 i;
//	struct msg_head_list *pos,*n; 

#if (_USE_MSG_LIST_ == 1)

	const portTickType xTickToWait = timeout / portTICK_RATE_MS;
	portBASE_TYPE ret;

	ret = xQueueReceive(p->list_head, msg, xTickToWait);
	if(ret != pdPASS)
		return 1;
#endif
	return 0;

}


/* 消息的守护进程 */
void msg_guard_task(void *pdata)
{
	u32 msg;
	while(1){
		msg_recv(&msg, 50000);
	}
}

/* 初始化消息队列 */
u8 sys_msg_init(void)
{

#if (_USE_MSG_LIST_ == 1)
	xQueue = xQueueCreate(10, sizeof(long));
	if(xQueue == NULL)
		return 1;

	xTaskCreate(msg_guard_task, "msg", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
#else

#endif
	return 0;
}




