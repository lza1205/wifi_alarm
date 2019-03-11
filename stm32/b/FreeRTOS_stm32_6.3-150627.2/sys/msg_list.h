
#ifndef _MSG_LIST_H_
#define _MSG_LIST_H_


#ifdef __MSG_LIST_C_
#define __MSG_LIST_EXT_
#else
#define __MSG_LIST_EXT_	extern
#endif

#include "all.h"
#include "my_list.h"

//#include "app_config.h"

//#include "config.h"


struct msg_head_list{
	xQueueHandle list_head;
	u32 type;

	struct list_head list;
};


#define MSG_MODE_NORMAL	0
#define MSG_MODE_ISR	1

#define msg_set(type, val)		msg_send(type, val, MSG_MODE_NORMAL)
#define msg_set_isr(type, val)	msg_send(type, val, MSG_MODE_ISR)



__MSG_LIST_EXT_ u8 __msg_ok;

u8 sys_msg_init(void);
struct msg_head_list * msg_init_group(void);
u8 msg_set_group(struct msg_head_list *p, u16 group);

u8 msg_send(u16 type, u16 val, u8 mode);
u8 msg_recv(u32 *val, u16 timeout);
u8 msg_get(struct msg_head_list *p, u32 *msg, u16 timeout);




#endif


