
#ifndef __ALARM_CONF_H__
#define __ALARM_CONF_H__

#ifdef __ALARM_CONF_C__
#define __ALARM_CONF_EXT__
#else
#define __ALARM_CONF_EXT__	extern
#endif

#include "all.h"

#define SYS_STATE_Idle			1
#define SYS_STATE_Arm			2
#define SYS_STATE_Home_Arm		3
#define SYS_STATE_Urgent		4
#define SYS_STATE_Threaten		5




__ALARM_CONF_EXT__ struct json_object *alarm_conf_json;



int init_sys_info(void);

void sys_info_add(char *key, struct json_object *sub1obj);


unsigned char sys_state(void);


#endif

