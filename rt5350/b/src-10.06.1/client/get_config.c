#include "all.h"
#include "client.h"


/*
���ܣ������ַ���s�ģ�unsigned int�ͣ����ȣ�������'\0'����
˵��������s�ĳ��ȣ�������������NULL��
*/
unsigned int STR_strlen(char *str) 
{ 
  unsigned int len = 0; 
  while((*str++) != '\0') 
  len++; 
  return len; 
}

/*
���ܣ��ҳ�str2�ַ�����str1�ַ����е�һ�γ��ֵ�λ�ã�������str2�Ĵ�����������
����ֵ�����ظ�λ�õ�ָ�룬���Ҳ��������ؿ�ָ�롣
*/
char *STR_strstr(char *s1, char *s2 )
{ 
  int len2; 
  if ( !(len2 = STR_strlen(s2)) ) 
  return s1; 
  for ( ; *s1; ++s1 )
  { 
    if ( *s1 == *s2 && strncmp((const char*) s1, (const char*)s2, len2 )==0 ) 
    return s1; 
  } 
  return NULL; 
} 

/*******************************************
������:	try_hostname
����:		���������ƽ��н���
*********************************************/

int try_hostname(void)
{
	struct hostent *host;

	if((host=gethostbyname(sys_cfg.serverip)) == NULL) {
		herror("gethostbyname err! >> ");
		return -1;
	}

	__strcpy(sys_cfg.serverip, inet_ntoa(*((struct in_addr *)host->h_addr)), IP_STRING_MAX);
	return 0;
}


#if 0
void str2int(char *str)
{
	char *s = str;
	int i = 0, j = 0;
	
	while((*s) ++){
		j ++;
	}
	
}
#endif

/*******************************************
������:	try_hostname
����:		������Ƿ��������ļ�
		��ȡ�������ļ���ֵ
*********************************************/

int check_config(void)
{
#if 0
	int fd;
	char buf[1024];
	int n;
	char *str;
	int i;
	char port_buf[10];
	int ret = 0;
	int port;

	if((fd = open("agent_config", O_RDONLY)) != -1){
		if((n = read(fd, buf, sizeof(buf))) <= 0){
			perror("read config :");
			goto scanf_port;
			
		}

		close(fd);
		
		buf[n] = 0;
//		app_printf("config : \n%s", buf);

		i = 0;
		if((str = STR_strstr(buf, "my port = ")) == NULL){
			app_printf("please input your port: ");
			ret = scanf("%d", &port);
			sys_cfg.my_port = port;
			
		}else{
			
			str = str + strnlen("my port = ", 30);
			while(*str != '\n'){
				port_buf[i] = *str;
				i ++;
				str ++;
			}
			port_buf[i] = 0;
			sys_cfg.my_port = atol(port_buf);
		}

		i = 0;
		if((str = STR_strstr(buf, "server port = ")) == NULL){
			app_printf("please input server port: ");
			ret = scanf("%d", &port);
			sys_cfg.ser_port = port;
			
		}else{
			
			str = str + strnlen("server port = ", 30);
			while(*str != '\n'){
				port_buf[i] = *str;
				i ++;
				str ++;
			}
			port_buf[i] = 0;
			sys_cfg.ser_port = atol(port_buf);
		}

		i = 0;
		if((str = STR_strstr(buf, "serverip = ")) == NULL){
			app_printf("please input server ip or Domain Name : ");
			ret = scanf("%s", sys_cfg.serverip);
			try_hostname();
		}else{
			
			str = str + strnlen("serverip = ", 30);
			while(*str != '\n'){
				sys_cfg.serverip[i] = *str;
				i ++;
				str ++;
			}
			try_hostname();
		}

		i = 0;
		if((str = STR_strstr(buf, "name = ")) == NULL){
			app_printf("please input name: ");
			ret = scanf("%s", sys_cfg.myname);
		}else{
			str = str + strnlen("name = ", 30);
			
			while(*str != '\n'){
				sys_cfg.myname[i] = *str;
				i ++;
				str ++;
			}
		}
		
		i = 0;
		if((str = STR_strstr(buf, "passwd = ")) == NULL){
			app_printf("please input your passwd: ");
			ret = scanf("%s", sys_cfg.passwd);
		}else{
			str = str + strnlen("passwd = ", 30);
			while(*str != '\n'){
				sys_cfg.passwd[i] = *str;
				i ++;
				str ++;
			}
		}
		
	}else{ 
		app_printf("no config\n");
scanf_port:		
		app_printf("please input your port: ");
		ret = scanf("%d", &port);
		sys_cfg.my_port = port;
//scanf_serip:
		/*	���������IP ���Լ�������*/
		app_printf("please input server ip or Domain Name : ");
		ret = scanf("%s", sys_cfg.serverip);
		try_hostname();
		
//scanf_name:
		app_printf("please input name: ");
		ret = scanf("%s", sys_cfg.myname);
		
//scanf_passwd:
		app_printf("please input passwd: ");
		ret = scanf("%s", sys_cfg.passwd);
	}

	app_printf("read config:\n%d\n%s\n%d\n%s\n%s\n", sys_cfg.my_port, sys_cfg.serverip, 
					sys_cfg.ser_port, sys_cfg.myname,sys_cfg.passwd);
	return ret;
#endif
}




struct json_object *sys_info_json;

char log_path[100] = "./";		//��־�ļ�·��

void __str_mh(char *dest, char *src, char h)
{
	while((*src))
	{
		if((*src) != h)
		{
			*dest = *src;
			dest ++;
		}
		src ++;
	}
}

/* ������� */
void rand_name(void)
{
#if 0
	char mac[20];
	int i;
	srand((int)time(NULL));     //ÿ��ִ�����Ӳ�ͬ�����ɲ�ͬ�������
	for(i = 0; i < 4; i++)
	{
		sys_cfg.myname[i] = 'A'+rand()%26;
	}
	sys_cfg.myname[i] = '-';
	get_mac("eth0", mac, &(sys_cfg.myname[i + 1]));
#else
	/* ���ֹ���
		���ֳ���8 ���ֽ�
		0 1 �ֽ�Ϊ�������
		2 3 4 5 6 7 8 �ֽ�Ϊ����mac��ַ
	*/
	int i;
	char mac[20], mac_str[20];

	get_mac("eth0", &(sys_cfg.myname[2]), mac_str);

	srand((int)time(NULL));     //ÿ��ִ�����Ӳ�ͬ�����ɲ�ͬ�������
	for(i = 0; i < 2; i++)
	{
		sys_cfg.myname[i] = rand()%0xff;
	}
#endif
}




void debug_buf(void)
{
	int i;
	char *str = "8aA1bEBf";
	unsigned char str_bug[100];
	unsigned char tmp[100];
	printf(".....\r\n");
	__str_to_buf(str_bug, str);
	printf("wocao \r\n");
	for(i = 0; i < 4; i++)
	{
		printf("[%x]", str_bug[i]);
	}
	printf("\r\n");

	__buf_to_str(tmp, str_bug, 8);
	tmp[8] = 0;
	printf("[%s]\r\n", tmp);
}


/* Ĭ������ */
int sys_info_default(void)
{
	char name[30];
	sys_info_json = json_object_new_object();

	if(NULL == sys_info_json)
		return -1;

	rand_name();
	sys_cfg.passwd = 0x12345678;
	strcpy(sys_cfg.serverip, "112.74.197.88");
	sys_cfg.ser_port = 8000;
	sys_cfg.tcp_port = 8080;
	sys_cfg.my_port = 3333;

	json_object_object_add(sys_info_json, "log_path", 
									json_object_new_string("./"));


	__buf_to_str(sys_cfg.name_str, sys_cfg.myname, USER_NAME_LEN);
	json_object_object_add(sys_info_json, "name", 
							json_object_new_string(sys_cfg.name_str));
	
	json_object_object_add(sys_info_json, "passwd", 
							json_object_new_int(sys_cfg.passwd));
	
	json_object_object_add(sys_info_json, "server_ip", 
								json_object_new_string(sys_cfg.serverip));

	json_object_object_add(sys_info_json, "server_port", 
							json_object_new_int(sys_cfg.ser_port));

	json_object_object_add(sys_info_json, "tcp_port", 
							json_object_new_int(sys_cfg.tcp_port));

	json_object_object_add(sys_info_json, "my_port", 
							json_object_new_int(sys_cfg.my_port));

	json_object_object_add(sys_info_json, "up_total_flow", 
							json_object_new_string("0"));

	json_object_object_add(sys_info_json, "down_total_flow", 
							json_object_new_string("0"));

	printf_json((char *)json_object_to_json_string(sys_info_json));

	return 0;
}



void __get_config_for_json()
{
	char __path[100];
	unsigned char name[100];
	
	__json_key_to_string(sys_info_json, "name", sys_cfg.name_str, 30);
//	printf("my name is %s\r\n", sys_cfg.myname);
	__str_to_buf(sys_cfg.myname, sys_cfg.name_str);

	__json_key_to_int(sys_info_json, "passwd", &(sys_cfg.passwd));
	__json_key_to_string(sys_info_json, "server_ip", sys_cfg.serverip, IP_STRING_MAX);
	__json_key_to_int(sys_info_json, "server_port", &(sys_cfg.ser_port));
	__json_key_to_int(sys_info_json, "tcp_port", &(sys_cfg.tcp_port));
	__json_key_to_int(sys_info_json, "my_port", &(sys_cfg.my_port));

	__json_key_to_string(sys_info_json, "log_path", log_path, 100);
	
	sys_cfg.log_path = log_path;
}





/*
��json �ṹ���л�ȡϵͳ��Ϣ
*/
int get_config_for_json(void)
{
	int fd;
	
	if((fd = open("sys_info.json", O_RDWR)) == -1){
		perror("open sys_info err \r\n");
		sys_info_default();

		//json_object_to_file("sys_info.json", sys_info_json);
		save_json_to_file((char *)json_object_to_json_string(sys_info_json), "sys_info.json");

		return 1;
	}else{
		sys_info_json = json_object_from_file("sys_info.json");
		app_printf("sys_info.json : \r\n");
		printf_json((char *)json_object_to_json_string(sys_info_json));

		__get_config_for_json();
		return 0;
	}
}


