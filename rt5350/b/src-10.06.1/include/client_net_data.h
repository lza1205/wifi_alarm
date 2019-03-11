/***********************************************************
�ļ���	:	client_net_data.h
����		:	Faker
�汾��	:	1.0
����		:	2015.06.05

˵��:	
	���ļ���Ϊ�ͻ����������ݰ��Ĵ���

***********************************************************/

#ifndef __CLIENT_NET_DATA_H_
#define __CLIENT_NET_DATA_H_

#include "all.h"

/* �������ݰ� */
struct net_data{

	struct sockaddr_in serveraddr;	/* Ŀ�� */
	
	int cnt;				/* ʣ�෢�ʹ��� */
	int time;				/* �ظ����͵���ʱ */
	
	char *data;				/* ���� */
	int data_len;				/* ���ݳ��� */

	struct list_head list;	/* ���� */
};


int add_net_data_list(char *buf, int len, struct sockaddr_in *addr);
void __printf_net_data_list(void);


int del_net_data_list(char *buf, int len);

void __test(void);

int init_net_data_list(void);


#endif


