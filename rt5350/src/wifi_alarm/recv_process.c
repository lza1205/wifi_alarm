
#include "all.h"
#include "client.h"

#include "string_define.h"
#include "alarm_conf.h"

void recv_process(char *name, char *buf, int len, struct sockaddr *clientaddr)
{
	struct json_object *new_obj = NULL;
	char command[50];

	new_obj = json_tokener_parse(buf);

	if(is_error(new_obj))
	{
		dbg_printf("json_tokener_parse err \n");
		return -1;
	}

	__json_key_to_string(new_obj, _STRING_Command, command, 50);

	app_printf("Command is (%s)\r\n", command);
#if 1
	if(strcmp(command, _STRING_Root_Login) == 0)	/* 管理员登陆 */
	{
		__root_login(name, new_obj);
	}else if(strcmp(command, _STRING_Add_Door) == 0){		/* 添加门磁 */						/* 添加配件 */

		__add_parts(new_obj, _STRING_Doorsensor);

	}else if(strcmp(command, _STRING_Del_Door) == 0){		/* 删除门磁 */						/* 添加配件 */

		__del_parts(new_obj, _STRING_Doorsensor);

	}else if(strcmp(command, _STRING_Add_Infrared) == 0){		/* 添加红外 */						/* 添加配件 */

		__add_parts(new_obj, _STRING_Infrared);

	}else if(strcmp(command, _STRING_Del_Infrared) == 0){		/* 删除红外 */				/* 添加配件 */

		__del_parts(new_obj, _STRING_Infrared);

	}else if(strcmp(command, _STRING_Add_Remote) == 0){		/* 添加遥控 */						/* 添加配件 */

		__add_parts(new_obj, _STRING_Remote);

	}else if(strcmp(command, _STRING_Del_Remote) == 0){		/* 删除遥控 */						/* 添加配件 */

		__del_parts(new_obj, _STRING_Remote);

	}else if(strcmp(command, _STRING_Add_Friend) == 0){		/* 添加好友 */						/* 添加配件 */

		__add_friend(new_obj);

	}else if(strcmp(command, _STRING_Del_Friend) == 0){		/* 删除好友 */						/* 添加配件 */

		__del_friend(new_obj);

	}else if(strcmp(command, _STRING_Set) == 0){		/* 设置参数 */						/* 添加配件 */

		__set(new_obj);

	}else if(strcmp(command, _STRING_Get) == 0){		/* 获取参数 */						/* 添加配件 */

		__get(new_obj);

	}else if(strcmp(command, _STRING_Scan) == 0){		/*扫描 */						/* 添加配件 */

		__scan(name, new_obj, clientaddr);

	}else{
		printf("command err \r\n");
	}
#endif
	json_object_put(new_obj);

}


/* 管理员登陆 */
int __root_login(char *name, struct json_object *obj)
{
//	char name[20];
	char passwd[20];
	char name_str[50];
	
	__buf_to_str(name_str, name, USER_NAME_LEN);
//	__json_key_to_string(obj, _STRING_Name, name, 20);
	__json_key_to_string(obj, _STRING_Passwd, passwd, 20);

	struct json_object *sub1obj = json_object_new_object();

	
	json_object_object_add(sub1obj, _STRING_Name,
							json_object_new_string(name_str));

	json_object_object_add(sub1obj, _STRING_Passwd, 
							json_object_new_string(passwd));


	struct json_object *root_obj = json_object_object_get(alarm_conf_json, _STRING_Root);
	
	json_object_object_add(root_obj, name_str, sub1obj);
	save_json_to_file(json_object_to_json_string(alarm_conf_json), "alarm_conf.json");
	printf_json(json_object_to_json_string(alarm_conf_json));


	return 0;
}

#if 0
int __add_door(struct json_object *obj)
{
	char ID[20];
	char name[20];
	int num;
	
	__json_key_to_string(obj, "ID", ID, 20);
	__json_key_to_string(obj, _STRING_Name, name, 20);

	struct json_object *sub1obj = json_object_new_object();

	json_object_object_add(sub1obj, _STRING_ID,
							json_object_new_string(ID));

	json_object_object_add(sub1obj, _STRING_Name, 
							json_object_new_string(name));

	struct json_object *door_obj = json_object_object_get(alarm_conf_json, _STRING_Doorsensor);

	struct json_object *sub_obj = json_object_object_get(door_obj, ID);

	if(sub_obj == NULL)
	{
		/* 修改总数量 */
		__json_key_to_int(door_obj, _STRING_Num, &num);
		num ++;
		json_object_object_add(door_obj, _STRING_Num, json_object_new_int(num));
	}
	
	json_object_object_add(door_obj, ID, sub1obj);

	/* 同步到系统配置文件 */
	save_json_to_file(json_object_to_json_string(alarm_conf_json), "alarm_conf.json");
	printf_json(json_object_to_json_string(alarm_conf_json));

	return 0;
}


int __del_door(struct json_object *obj)
{
	char ID[20];
	char name[20];
	int num;
	
	__json_key_to_string(obj, "ID", ID, 20);
	__json_key_to_string(obj, _STRING_Name, name, 20);

	struct json_object *door_obj = json_object_object_get(alarm_conf_json, _STRING_Doorsensor);

	struct json_object *sub_obj = json_object_object_get(door_obj, ID);
	if(sub_obj != NULL)
	{
		dbg_printf("find door\r\n");
		
		/* 修改总数量 */
		__json_key_to_int(door_obj, _STRING_Num, &num);
		if(num > 0)
			num --;
		json_object_object_add(door_obj, _STRING_Num, json_object_new_int(num));
	}
	
	json_object_object_del(door_obj, ID);

	/* 同步到系统配置文件 */
	save_json_to_file(json_object_to_json_string(alarm_conf_json), "alarm_conf.json");
	printf_json(json_object_to_json_string(alarm_conf_json));

	return 0;
}


int __add_Infrared(struct json_object *obj)
{
	char ID[20];
	char name[20];
	int num;
	
	__json_key_to_string(obj, "ID", ID, 20);
	__json_key_to_string(obj, _STRING_Name, name, 20);

	struct json_object *sub1obj = json_object_new_object();

	json_object_object_add(sub1obj, _STRING_ID,
							json_object_new_string(ID));

	json_object_object_add(sub1obj, _STRING_Name, 
							json_object_new_string(name));

	struct json_object *door_obj = json_object_object_get(alarm_conf_json, _STRING_Infrared);

	struct json_object *sub_obj = json_object_object_get(door_obj, ID);

	if(sub_obj == NULL)
	{
		/* 修改总数量 */
		__json_key_to_int(door_obj, _STRING_Num, &num);
		num ++;
		json_object_object_add(door_obj, _STRING_Num, json_object_new_int(num));
	}
	
	json_object_object_add(door_obj, ID, sub1obj);

	/* 同步到系统配置文件 */
	save_json_to_file(json_object_to_json_string(alarm_conf_json), "alarm_conf.json");
	printf_json(json_object_to_json_string(alarm_conf_json));

	return 0;
}


int __del_Infrared(struct json_object *obj)
{
	char ID[20];
	char name[20];
	int num;
	
	__json_key_to_string(obj, "ID", ID, 20);
	__json_key_to_string(obj, _STRING_Name, name, 20);

	struct json_object *door_obj = json_object_object_get(alarm_conf_json, _STRING_Infrared);

	struct json_object *sub_obj = json_object_object_get(door_obj, ID);
	if(sub_obj != NULL)
	{
		dbg_printf("find door\r\n");
		
		/* 修改总数量 */
		__json_key_to_int(door_obj, _STRING_Num, &num);
		if(num > 0)
			num --;
		json_object_object_add(door_obj, _STRING_Num, json_object_new_int(num));
	}
	
	json_object_object_del(door_obj, ID);

	/* 同步到系统配置文件 */
	save_json_to_file(json_object_to_json_string(alarm_conf_json), "alarm_conf.json");
	printf_json(json_object_to_json_string(alarm_conf_json));

	return 0;
}

#endif
int __add_parts(struct json_object *obj, char *parts)
{
	char ID[20];
	char name[20];
	int num;
	
	__json_key_to_string(obj, "ID", ID, 20);
	__json_key_to_string(obj, _STRING_Name, name, 20);

	struct json_object *sub1obj = json_object_new_object();

	json_object_object_add(sub1obj, _STRING_ID,
							json_object_new_string(ID));

	json_object_object_add(sub1obj, _STRING_Name, 
							json_object_new_string(name));

	struct json_object *door_obj = json_object_object_get(alarm_conf_json, parts);

	struct json_object *sub_obj = json_object_object_get(door_obj, ID);

	if(sub_obj == NULL)
	{
		/* 修改总数量 */
		__json_key_to_int(door_obj, _STRING_Num, &num);
		num ++;
		json_object_object_add(door_obj, _STRING_Num, json_object_new_int(num));
	}
	
	json_object_object_add(door_obj, ID, sub1obj);

	/* 同步到系统配置文件 */
	save_json_to_file(json_object_to_json_string(alarm_conf_json), "alarm_conf.json");
	printf_json(json_object_to_json_string(alarm_conf_json));

	return 0;
}


int __del_parts(struct json_object *obj, char *parts)
{
	char ID[20];
	char name[20];
	int num;
	
	__json_key_to_string(obj, "ID", ID, 20);
	__json_key_to_string(obj, _STRING_Name, name, 20);

	struct json_object *door_obj = json_object_object_get(alarm_conf_json, parts);

	struct json_object *sub_obj = json_object_object_get(door_obj, ID);
	if(sub_obj != NULL)
	{
		dbg_printf("find door\r\n");
		
		/* 修改总数量 */
		__json_key_to_int(door_obj, _STRING_Num, &num);
		if(num > 0)
			num --;
		json_object_object_add(door_obj, _STRING_Num, json_object_new_int(num));
	}
	
	json_object_object_del(door_obj, ID);

	/* 同步到系统配置文件 */
	save_json_to_file(json_object_to_json_string(alarm_conf_json), "alarm_conf.json");
	printf_json(json_object_to_json_string(alarm_conf_json));

	return 0;
}


int __add_friend(struct json_object *obj)
{
	char Phone[20];
	char name[20];
	int num;
	
	__json_key_to_string(obj, "Phone", Phone, 20);
	__json_key_to_string(obj, _STRING_Name, name, 20);

	struct json_object *sub1obj = json_object_new_object();

	json_object_object_add(sub1obj, _STRING_Phone,
							json_object_new_string(Phone));

	json_object_object_add(sub1obj, _STRING_Name, 
							json_object_new_string(name));

	struct json_object *t_obj = json_object_object_get(alarm_conf_json, _STRING_Friends);

	struct json_object *sub_obj = json_object_object_get(t_obj, name);

	if(sub_obj == NULL)
	{
		/* 修改总数量 */
		__json_key_to_int(t_obj, _STRING_Num, &num);
		num ++;
		json_object_object_add(t_obj, _STRING_Num, json_object_new_int(num));
	}
	
	json_object_object_add(t_obj, name, sub1obj);

	/* 同步到系统配置文件 */
	save_json_to_file(json_object_to_json_string(alarm_conf_json), "alarm_conf.json");
	printf_json(json_object_to_json_string(alarm_conf_json));

	return 0;
}


int __del_friend(struct json_object *obj)
{
	char Phone[20];
	char name[20];
	int num;
	
	__json_key_to_string(obj, "Phone", Phone, 20);
	__json_key_to_string(obj, _STRING_Name, name, 20);

	struct json_object *t_obj = json_object_object_get(alarm_conf_json, _STRING_Friends);

	struct json_object *sub_obj = json_object_object_get(t_obj, name);
	if(sub_obj != NULL)
	{
		dbg_printf("find door\r\n");
		
		/* 修改总数量 */
		__json_key_to_int(t_obj, _STRING_Num, &num);
		if(num > 0)
			num --;
		json_object_object_add(t_obj, _STRING_Num, json_object_new_int(num));
	}
	
	json_object_object_del(t_obj, name);

	/* 同步到系统配置文件 */
	save_json_to_file(json_object_to_json_string(alarm_conf_json), "alarm_conf.json");
	printf_json(json_object_to_json_string(alarm_conf_json));

	return 0;
}


int __set(struct json_object *obj)
{
	int ret = 0;
	char str[50];
	struct json_object *sub_obj = NULL;
	__json_object_object_foreach(obj, key, val){
		sub_obj = json_object_object_get(alarm_conf_json, key);
		if(sub_obj != NULL)
		{
			__json_key_to_string(obj, key, str, 50);
			
			json_object_object_add(alarm_conf_json, key, json_object_new_string(str));
			ret ++;
		}
	}
	dbg_printf("set %d done \r\n", ret);

	/* 同步到系统配置文件 */
	save_json_to_file(json_object_to_json_string(alarm_conf_json), "alarm_conf.json");
	printf_json(json_object_to_json_string(alarm_conf_json));
	return ret;
}

int __get(struct json_object *obj)
{
	return 0;
}

void __scan(char *name, struct json_object *obj, struct sockaddr *clientaddr)
{
	struct json_object *tmp_obj = json_object_new_object();
	if(tmp_obj == NULL)
	{
		printf("json_object_new_object err \r\n");
		return ;
	}

	json_object_object_add(tmp_obj, _STRING_Command, 
								json_object_new_string(_STRING_Scan));

	char myname[30];
	__buf_to_str(myname, sys_cfg.myname, USER_NAME_LEN);
	json_object_object_add(tmp_obj, _STRING_Name, 
								json_object_new_string(myname));

	sendto_node_unreliable(name, _proto_c_msg_, json_object_to_json_string(tmp_obj),
									strlen(json_object_to_json_string(tmp_obj)) + 1);

	json_object_put(tmp_obj);
}

void wifi_alarm_init(void)
{
	/* 配置文件初始化 */
	init_alarm_conf();

	/* 串口初始化 */
	uart_init();
}

