#ifndef __P2P_THREAD_H_
#define __P2P_THREAD_H_

#include "all.h"
#include "client.h"

struct p2p_node_info{
	int status;				//�Ƿ����� 0 ����
	unsigned char name[USER_NAME_LEN];
	char ip[IP_STRING_MAX];
	int port;

	int list_typpe;		/* 0 �ǲ��Զ���  1 ��ͬ������ */
	int sync_cnt;		/* ͬ������ֵ */
	int test_cnt;			/* ���Դ��� */
	
	struct list_head list;
};

int p2p_send_data(char *name, unsigned int proto, char *buf, int len);



#endif

