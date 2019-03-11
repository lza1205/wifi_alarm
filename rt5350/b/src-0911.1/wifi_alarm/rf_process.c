
#include "all.h"
#include "client.h"

#include "alarm_conf.h"
#include "string_define.h"


int tell_root(struct json_object *obj, char *command);

int rf_recv_process(char *code)
{
	struct json_object *obj;
//	char buf[1024];
	unsigned char type = 0;
	char *p = code;

	while((*p) != 0)
	{
		if((*p) == '\r')
			*p = 0;
		if((*p) == '\n')
			*p = 0;
		p ++;
	}

	/* �ж��Ƿ�Ϊ�Ŵ� */
	struct json_object *door_obj = json_object_object_get(alarm_conf_json, _STRING_Doorsensor);

	if(door_obj == NULL)
		return NULL;
	
	obj = json_object_object_get(door_obj, code);
	if(obj != NULL)
	{
		/* �յ����Ŵ� */
		app_printf("recv rf for doorsensor \r\n");
		type = 1;
		goto out1;
	}

	/* �ж����Ƿ�Ϊ���� */
	struct json_object *infrared_obj = json_object_object_get(alarm_conf_json, _STRING_Infrared);

	if(infrared_obj == NULL)
		return NULL;
	
	obj = json_object_object_get(infrared_obj, code);
	if(obj != NULL)
	{
		/* �յ����Ǻ��� */
		app_printf("recv rf for infrared \r\n");
		type = 2;
		goto out1;
	}

	return -1;

	
out1:

	if(sys_state() == SYS_STATE_Idle)	/* ����ģʽ */
	{
		if((type == 1) || (type == 2))	/* �����ŴŲ��������� */
			return 0;
	}

#if 1
	/* ֪ͨ����root �û� */
	tell_root(obj, "Msg_RF");

	/* ֪ͨ������ϵ�� */
	tell_friend(obj, "Msg_RF");

	/* ֪ͨ�Ӿ����� */
	if((sys_state() == SYS_STATE_Urgent)
		|| (sys_state() == SYS_STATE_Threaten)) 	/* �����������Ҫ֪ͨ�Ӿ����� */
	{
		
	}

	if(sys_state() != SYS_STATE_Threaten)	/* ����Ю��ģʽ����Ҫ������ */
	{
		
	}
#endif
	return 0;
	
}

