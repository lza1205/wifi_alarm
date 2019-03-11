
#ifndef __RECV_PTHREAD_H__
#define __RECV_PTHREAD_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <pthread.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <stdio.h>

#include "my_list.h"



struct recv_data_node{
	struct sockaddr_in addr;		/* ���ݰ���ַ��Ϣ */
	char *data;					/* ���ݰ����� */
	int len;					/* ���ݰ����� */

	
	struct list_head data_list;		/* ���ݰ����� */
};


typedef struct listen{		
	struct sockaddr_in addr;		/* ���ݰ���ַ��Ϣ */
	char name[USER_NAME_LEN];	/* ���ݰ�Դ���� */
	unsigned int proto;			/* ���ݰ�ԴЭ�� */
	
	int data_num;				/* ���ݰ����� */
	int data_max_num;			/* ���ݰ�������� */
	
	int list_flg;				/* �Ƿ��Ѿ��������� */

	pthread_mutex_t mutex;	/* �߳��� */

	/* ����������������ص� */
	
	pthread_mutex_t recv_mtx;
	pthread_cond_t recv_cond;

	struct list_head head;		/* ���ݰ����� */
	
	struct list_head listen_list;		/*���յ��̶߳��� */

	struct list_head poll_list;			/* poll ���ƵĶ��� */
}listen_t;

/*--------------------------------------------------------------------------*/
//�ṩ���û��Ľӿ�
/*--------------------------------------------------------------------------*/


/*-------------------------------------------------------------------
���ýӿ�
-------------------------------------------------------------------*/

/* 
��һ��listen �л�ȡ����
*/
int recv_from_listen(	
	struct listen *recv_list, 
	struct sockaddr_in *addr,		/* ���ݰ���ַ��Ϣ */
	char *buf,					/* ���ݰ����� */
	int len,					/* ���ݰ����� */
	int delay_time				/* ����ʱ�䡣 -1��ʾ�������� */
);



/* ��ȡһ������ */
struct listen *server_accept(void);

/*
�ر�ĳ���ͻ���
*/
int listen_close(struct listen *listen);

/*
��ʼlisten ͨ��
*/
int listen_start(struct listen *listen);


/*
��ʼ����
*/
void server_listen(int num);

/*
���������߳�
*/
void listen_pthread(listen_t *listen, void *(*start_rtn)(void *));



/*-------------------------------------------------------------------
poll ���ƽӿ�
-------------------------------------------------------------------*/
	
/*
��ʼ��һ�� listen ����ͷ
*/
void listen_head_init(struct list_head *head);

/* 
��Ҫ������listen ���뵽���head ������
*/
int listen_add(struct list_head *head, listen_t *listen);

/* ��Ҫ������listen�����л�ȡ���� */
int recv_from_listen_head(
		struct list_head* head,
		struct listen **recv_listen, 	/* ָ��ָ���ָ�� */
		struct sockaddr_in *addr,		/* ���ݰ���ַ��Ϣ */
		char *buf,					/* ���ݰ����� */
		int len,					/* ���ݰ����� */
		int delay_time				/* ����ʱ�䡣 -1��ʾ�������� */
	);



/*-------------------------------------------------------------------
�Լ�����һ�� listen �ӿ�
-------------------------------------------------------------------*/
listen_t *create_listen(char *name, unsigned int proto);




#endif


