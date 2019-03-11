

#define __ALARM_CONF_C__

#include "all.h"
#include "client.h"

#include "string_define.h"
#include "alarm_conf.h"

struct json_object *alarm_conf_json;


/* 默认配置 */
int alarm_conf_default(void)
{
	alarm_conf_json = json_object_new_object();

	if(NULL == alarm_conf_json)
		return -1;

	{
	/* 管理员账户 */
		struct json_object *root_obj;
		root_obj = json_object_new_object();
		if(root_obj == NULL)
			return -1;
		/*json_object_object_add(root_obj, "Name", 
							json_object_new_string(""));
							*/
		json_object_object_add(alarm_conf_json, _STRING_Root, 
							root_obj);
	}
	
	/* 紧急联系人 */
	{
		struct json_object *Friends_obj;
		Friends_obj = json_object_new_object();
		if(Friends_obj == NULL)
			return -1;

		/* 总共有多少个子项 */
		json_object_object_add(Friends_obj, _STRING_Num, 
							json_object_new_int(0));
		
		json_object_object_add(alarm_conf_json, _STRING_Friends, 
							Friends_obj);
	}

	/* 接警中心 */
	{
		struct json_object *alarm_obj;
		alarm_obj = json_object_new_object();
		if(alarm_obj == NULL)
			return -1;
		json_object_object_add(alarm_obj, _STRING_Num, 
							json_object_new_int(0));
		json_object_object_add(alarm_conf_json, _STRING_Alarm_Center, 
							alarm_obj);
	}

	/* 系统状态 */
	{
		json_object_object_add(alarm_conf_json, _STRING_System_State,
							json_object_new_string(_STRING_State_Idle));
	}

	/* 杂项 */
	{
		/* 喇叭鸣响时间 */
		json_object_object_add(alarm_conf_json, _STRING_Ringing_Time,
							json_object_new_int(30));
	}

	/* 定时策略 */
	{
		struct json_object *time_obj;
		time_obj = json_object_new_object();
		if(time_obj == NULL)
			return -1;
		json_object_object_add(time_obj, _STRING_Num, 
							json_object_new_int(0));
		json_object_object_add(alarm_conf_json, _STRING_Timing, 
							time_obj);
	}
	
	/* 门磁配件 */
	{
		struct json_object *mc_obj;
		mc_obj = json_object_new_object();
		if(mc_obj == NULL)
			return -1;

		/* 总共有多少个子项 */
		json_object_object_add(mc_obj, _STRING_Num, 
							json_object_new_int(0));
		
		json_object_object_add(alarm_conf_json, _STRING_Doorsensor, 
							mc_obj);
	}
	
	/* 红外配件 */
	{
		struct json_object *hw_obj;
		hw_obj = json_object_new_object();
		if(hw_obj == NULL)
			return -1;

		/* 总共有多少个子项 */
		json_object_object_add(hw_obj, _STRING_Num, 
							json_object_new_int(0));
		
		json_object_object_add(alarm_conf_json, _STRING_Infrared, 
							hw_obj);
	}

	printf_json((char *)json_object_to_json_string(alarm_conf_json));

	return 0;
}


#if 0
/* 增加更新配置文件子项 */
void alarm_conf_add(char *key, struct json_object *sub1obj)
{
	struct json_object *root_obj = json_object_object_get(alarm_conf_json, "Root");
	
	json_object_object_add(root_obj, key, sub1obj);
	json_object_to_file("alarm_conf", alarm_conf_json);
	printf_json(json_object_to_json_string(alarm_conf_json));
}
#endif


/* 打开系统配置信息文件 */
int init_alarm_conf(void)
{
	int fd;
	
	if((fd = open("alarm_conf.json", O_RDWR)) == -1){
		perror("open alarm_conf.json err \r\n");
		alarm_conf_default();
		save_json_to_file(json_object_to_json_string(alarm_conf_json), "alarm_conf.json");
		return 1;
	}else{
		alarm_conf_json = json_object_from_file("alarm_conf.json");
		app_printf("alarm_conf.json : \r\n");
		printf_json((char *)json_object_to_json_string(alarm_conf_json));
		return 0;
	}
}



unsigned char sys_state(void)
{
	char state[20];
	__json_key_to_string(alarm_conf_json, _STRING_System_State, state, 20);

	if(strcmp(_STRING_State_Idle, state) == 0)
	{
		return SYS_STATE_Idle;
	}else if(strcmp(_STRING_State_Arm, state) == 0)
	{
		return SYS_STATE_Arm;
	}else if(strcmp(_STRING_State_Home_Arm, state) == 0)
	{
		return SYS_STATE_Home_Arm;
	}else if(strcmp(_STRING_State_Urgent, state) == 0)
	{
		return SYS_STATE_Urgent;
	}else if(strcmp(_STRING_State_Threaten, state) == 0)
	{
		return SYS_STATE_Threaten;
	}else {
		return _STRING_State_Arm;
	}
}



