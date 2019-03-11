
#include "all.h"
#include "client.h"

#include "alarm_conf.h"
#include "string_define.h"


//int tell_root(struct json_object *obj, char *command);

int rf_recv_process(char *code)
{
	struct json_object *obj;
//	char buf[1024];
	unsigned char type = 0;
	char *p = code;

	char type_str[50];

	while((*p) != 0)
	{
		if((*p) == '\r')
			*p = 0;
		if((*p) == '\n')
			*p = 0;
		p ++;
	}

	/* 判断是否为门磁 */
	struct json_object *door_obj = json_object_object_get(alarm_conf_json, _STRING_Doorsensor);

	if(door_obj == NULL)
		return NULL;
	
	obj = json_object_object_get(door_obj, code);
	if(obj != NULL)
	{
		/* 收到的门磁 */
		app_printf("recv rf for doorsensor \r\n");
		type = 1;
		strcpy(type_str, _STRING_Doorsensor);
		goto out1;
	}

	/* 判断下是否为红外 */
	struct json_object *infrared_obj = json_object_object_get(alarm_conf_json, _STRING_Infrared);

	if(infrared_obj == NULL)
		return NULL;
	
	obj = json_object_object_get(infrared_obj, code);
	if(obj != NULL)
	{
		/* 收到的是红外 */
		app_printf("recv rf for infrared \r\n");
		type = 2;
		strcpy(type_str, _STRING_Infrared);
		goto out1;
	}

	return -1;

	
out1:

	if(sys_state() == SYS_STATE_Idle)	/* 撤防模式 */
	{
		if((type == 1) || (type == 2))	/* 红外门磁不触发报警 */
			return 0;
	}

#if 1
	/* 通知所有root 用户 */
	tell_root(obj, _STRING_Msg_RF, type_str);

	/* 通知所有联系人 */
	tell_friend(obj, _STRING_Msg_RF, type_str);

	/* 通知接警中心 */
	if((sys_state() == SYS_STATE_Urgent)
		|| (sys_state() == SYS_STATE_Threaten)) 	/* 紧急情况，还要通知接警中心 */
	{
		
	}

	if(sys_state() != SYS_STATE_Threaten)	/* 不是挟持模式，需要响喇叭 */
	{
		
	}
#endif
	return 0;
	
}

