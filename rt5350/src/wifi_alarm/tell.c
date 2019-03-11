
#define __TELL_C_

#include "all.h"
#include "client.h"

#include "string_define.h"
#include "alarm_conf.h"


int __tell_client(struct json_object *src, struct json_object *obj, char *command, char *type)
{
	struct json_object *tmp_obj;
	int ret;
	
	char send_buf[1024];
	int send_len;
	
	char name[20];
	char passwd[20];

	char str[100];
	char buf[8];
	
	ret = __json_key_to_string(src, _STRING_Name, name, 20);
	if(ret != 0)
		return -1;

	ret = __json_key_to_string(src, _STRING_Passwd, passwd, 20);
	if(ret != 0)
	{
		passwd[0] = 0;
	}

	app_printf("tell root\r\n");
	app_printf("tell %s\r\n", name);
	
	tmp_obj = json_object_new_object();

	if(tmp_obj == NULL)
	{
		printf("__tell_client err \r\n");
	}

	json_object_object_add(tmp_obj, _STRING_Command, 
								json_object_new_string(command));
	json_object_object_add(tmp_obj, _STRING_Type, json_object_new_string(type));
	
	__json_object_object_foreach(obj,key,val)
	{
		__json_key_to_string(obj, key, str, 50);
		json_object_object_add(tmp_obj, key, json_object_new_string(str));
	}

	__str_to_buf(buf, name);

	send_len = strlen(json_object_to_json_string(tmp_obj)) + 1;
	memcpy(send_buf, json_object_to_json_string(tmp_obj), send_len);
	sendto_node_unreliable(buf, _proto_c_msg_, send_buf, send_len);
	
	json_object_put(tmp_obj);
	
}


/***************************************************
函数名: 
作用，通知所有root 用户
***************************************************/
int tell_root(struct json_object *obj, char *command, char *type)
{
	
	struct json_object *tmp_obj = json_object_object_get(alarm_conf_json, _STRING_Root);

	app_printf("tell all root\r\n");
	__json_object_object_foreach(tmp_obj,key,val)
	{
		__tell_client(val, obj, command, type);
	}

	printf("tell_root end \r\n");
	return 0;
}

/***************************************************
函数名: 
作用，通知所有紧急联系人
***************************************************/
int tell_friend(struct json_object *obj, char *command, char *type)
{
	
	struct json_object *tmp_obj = json_object_object_get(alarm_conf_json, _STRING_Friends);

	app_printf("tell all friend\r\n");
	__json_object_object_foreach(tmp_obj,key,val)
	{
		if(strcmp(key, "Num") != 0)
			__tell_client(val, obj, command, type);
	}

	return 0;
}


