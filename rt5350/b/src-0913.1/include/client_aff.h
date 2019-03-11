
#ifndef __CLIENT_AFF_H_
#define __CLIENT_AFF_H_

#ifdef __CLIENT_AFF_C_
#define __CLIENT_AFF_EXT_
#else
#define __CLIENT_AFF_EXT_	extern
#endif

#include "all.h"

#include "my_list.h"

/* ���յ����������� */
struct client_aff_info{
	int aff;		//��������
	void (*aff_fun)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr);
};



struct recv_head_node{
	struct check_head head;
	
	struct list_head list;
	
};

int check_recv_head(struct check_head *head);


__CLIENT_AFF_EXT_ struct client_aff_info client_aff_table[];

__CLIENT_AFF_EXT_ int recv_list_flg;		//�Ƿ����ͬ���������ݵķ�ʽ


#endif

