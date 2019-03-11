/***********************************************************
�ļ���	:	client_net_data.c
����		:	Faker
�汾��	:	1.0
����		:	2015.06.05

˵��:	
	���ļ���Ϊ�ͻ����������ݰ��Ĵ���

***********************************************************/


#include "all.h"
#include "client.h"

#include "client_net_data.h"


#include <pthread.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <stdio.h>


/* ��Щ�����漰�����̲߳������⣬���ô��� */
/* ����ϻ����� */
static pthread_mutex_t net_data_lock = PTHREAD_MUTEX_INITIALIZER;

LIST_HEAD(net_data_list);		//���岢��ʼ��һ������ͷ
volatile int net_data_node_num = 0;


/* ����ͷ */



/********************************************
������:	__del_list
����:		����������������ɾ������
********************************************/

int __del_list(struct net_data *tmp, struct net_data *data)
{
	return 0;
}


/********************************************
������:	__printf_net_data_list
����:		��ӡ�����������ݰ�
********************************************/

void __printf_net_data_list(void)
{
	struct net_data *tmp;
	struct check_head *head;
	int i = 0;

	pthread_mutex_lock(&net_data_lock);

	dbg_printf("/********** printf list **********/\r\n");
	
	struct net_data *pos,*n;
	/* �Ƚ���Ӧ����Ƿ���һ���� */
	list_for_each_entry_safe_reverse(pos,n,&net_data_list, list, struct net_data)
	{
		head = (struct check_head *)(pos->data);
		dbg_printf("[%d] %d\r\n", i++, head->key);
	}

	pthread_mutex_unlock(&net_data_lock);
	
//	return 0;
}



/********************************************
������:	add_net_data_list
����:		��������ӵ���������������
********************************************/
/* ��ʱ���ṩ�ⲿ�ֵĹ��� */
int add_net_data_list(char *buf, int len, struct sockaddr_in *addr)
{

	struct net_data *new_data;
	char *data;
	
	new_data = (struct net_data*)malloc(sizeof(struct net_data));
	if(new_data == NULL)
		return -1;

	data = (char *)malloc(len);
	if(data == NULL){
		free(new_data);
		return -1;
	}

	/* �������� */
	if(addr != NULL)
		memcpy(&(new_data->serveraddr), addr, sizeof(struct sockaddr_in));

	memcpy(data, buf, len);
	new_data->data = data;
	new_data->data_len = len;

	new_data->cnt = 2;
	new_data->time = 3;

//	printf("add is ok 1 \r\n");

	pthread_mutex_lock(&net_data_lock);

	/* ��ӵ������� */
	list_add(&(new_data->list), &net_data_list);

	pthread_mutex_unlock(&net_data_lock);

//	printf("add is ok 2 \r\n");

//	__printf_net_data_list();

	return 0;
}



/********************************************
������:	del_net_data_list
����:		�����ݴ���������������ɾ��
********************************************/

int del_net_data_list(char *buf, int len)
{
#if 1
	struct check_head *head = (struct check_head *)buf;
	struct check_head *pos_head;

//	printf("__del key_cnt is %d \r\n", head->key);

	pthread_mutex_lock(&net_data_lock);
	struct net_data *pos,*n;
	/* �Ƚ���Ӧ����Ƿ���һ���� */
	list_for_each_entry_safe_reverse(pos,n,&net_data_list, list, struct net_data)
	{
		pos_head = (struct check_head *)(pos->data);
		if(pos_head->key == head->key)
		{
			free(pos->data);
			free(pos);
			list_del(&(pos->list));

//			printf("__del net data \r\n");
			
			pthread_mutex_unlock(&net_data_lock);
			return 0;
		}
	}
	pthread_mutex_unlock(&net_data_lock);
	return -1;
#endif
}





/********************************************
������:	__send_again
����:		�ٳ��Է���һ��
********************************************/

void __send_again(struct net_data *net_dat)
{
//	struct check_head *head;

//	head = (struct check_head *)(net_dat->data);
//	dbg_printf("__send_again to %s get %s\r\n", 
//					inet_ntoa((net_dat->serveraddr).sin_addr),
//					head->AID);
	p2p_sendto(sys_cfg.sockfd, (char *)(net_dat->data), net_dat->data_len, 0, (struct sockaddr *)&(net_dat->serveraddr),
		   sizeof(struct sockaddr_in));
}


/********************************************
������:	net_data_send_err
����:		���ظ����ζ�����ʧ�ܵ��������ݽ��д���
		����Ƿ�������ת:

		�����P2P ͨ��ʧ�ܣ����߷�������ת��
			ͬʱ��Ǹÿͻ���P2P������
********************************************/

void net_data_send_err(struct net_data *net_dat)
{
//	struct check_head *head;
//	struct net_data *new_data;
//	char *data;

//	head = (struct check_head *)(net_dat->data);

//	dbg_printf("net_data_send_err\r\n");

	client_inface.data_send_err(net_dat);

#if 0
	if(head->affairs== AFF_SEND_CLIENT)		/* ��������תʧ�� */
	{
		
	}else if(head->affairs == AFF_CLIENT_DATA_P2P)	/* P2Pʧ�� */
	{
		struct sockaddr_in serveraddr;
	
		serveraddr.sin_family = AF_INET;
	    serveraddr.sin_addr.s_addr = inet_addr(sys_cfg.handle_ip);
	    serveraddr.sin_port = htons(sys_cfg.handle_port);

		p2p_send_err(net_dat->data, net_dat->data_len);
		/* ����ͨ��������ת�� */
		compages_head(head, AFF_SEND_CLIENT);

		new_data = (struct net_data*)malloc(sizeof(struct net_data));
		if(new_data == NULL)
			return ;

		data = (char *)malloc(net_dat->data_len);
		if(data == NULL){
			free(new_data);
			return ;
		}

		memcpy(new_data, net_dat, sizeof(struct net_data));
		memcpy(data, net_dat->data, net_dat->data_len);
		memcpy(&(new_data->serveraddr), &serveraddr, sizeof(struct sockaddr_in));

		new_data->data = data;
		new_data->cnt = 3;
		new_data->time = 0;

		dbg_printf("net_data_send_err 2\r\n");
		/* ��ӵ������� */
		__add_list(new_data);
	}
#endif
}


/********************************************
������:	net_data_list_pthread
����:		�̡߳����ڴ����������ݰ����ط�����
********************************************/

void *net_data_list_pthread(void *pdata)
{
	struct net_data *now_data = NULL, *tmp_data = NULL, *t_data;

	/* ��ʼ���� */
	
//	__test();

	while(1)
	{

//		now_data = NULL;
		tmp_data = NULL;
		t_data 	= NULL;

		pthread_mutex_lock(&net_data_lock);
		struct net_data *now_data,*n;
		/* �Ƚ���Ӧ����Ƿ���һ���� */
		list_for_each_entry_safe_reverse(now_data,n,&net_data_list, list, struct net_data)
		{
			if(now_data->time > 0){
				now_data->time --;
				goto out1;
			}
			if(now_data->cnt > 0){
				now_data->cnt --;
				now_data->time = 3;
				/* ���·���һ�� */
				__send_again(now_data);
				goto out1;
			}else if(now_data->cnt == 0){		//�ظ���3�λ�����Ӧ�������ʧ����
				/* ɾ�� */
				list_del(&(now_data->list));
				free(now_data->data);
				free(now_data);
				net_data_send_err(now_data);
			}
			continue;
out1:
			;
		}

		pthread_mutex_unlock(&net_data_lock);
		sleep(2);
	}
}


/********************************************
������:	net_data_list_pthread
����:		��ʼ���������ݶ��е��߳�
********************************************/

int init_net_data_list(void)
{
	pthread_t thread;
    pthread_attr_t thread_attr;

	/* �����ӽ��� */
	pthread_attr_init(&thread_attr);		//��ʼ����������
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	if(pthread_create(&thread, &thread_attr, net_data_list_pthread, NULL) < 0)
	{
		perror("pthread_create");
	}
	return 0;
}


