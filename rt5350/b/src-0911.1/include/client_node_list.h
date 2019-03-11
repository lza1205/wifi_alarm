#ifndef __CLIENT_INFO_LIST_H_
#define __CLIENT_INFO_LIST_H_

#ifdef __CLIENT_INFO_LIST_C_
#define __CLIENT_INFO_LIST_EXT_
#else
#define __CLIENT_INFO_LIST_EXT_
#endif 

#include "all.h"
#include "client.h"
#include "my_list.h"

struct client_node{
	int status;				//�Ƿ����� 0 ����
	int call_cnt;			//call ���
	char name[USER_NAME_LEN];
	char ip[IP_STRING_MAX];
	int port;
	struct list_head list;
};

int add_client_node(struct proto_s_client_info *info);




#endif

