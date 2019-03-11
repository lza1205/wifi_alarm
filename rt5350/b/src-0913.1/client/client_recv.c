#define __CLIENT_RECV_C_

#include "all.h"
#include "client.h"

#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include <sys/time.h>



/********************************************
������: client_recv_handle
����:		�ͻ���udp���յ����ݺ�ĵ�һ��������
*********************************************/

void client_recv_handle(void)
{
	int i;
	struct sockaddr_in clientaddr;
	char recvbuf[UDP_PACK_MAX_SIZE];
	struct check_head *head;
	int ret;
	int clilen;

	while(1)
	{
		
		clilen = sizeof(clientaddr);
		ret = p2p_recvfrom(sys_cfg.sockfd, (char *)recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&clientaddr, (socklen_t *)&clilen);


		if (ret < 0)
		{
			perror("my_recvfrom_handle");
			continue;
		}
		else
		{
			head = (struct check_head *)recvbuf;

			/* ��鳤�� */
			if(ret < sizeof(struct check_head))
				continue;

			/* �����ݰ�ͷ������У�� */
			if(check_head_crc(head) != 0)
				continue;			

//			printf_client(&clientaddr);
//			printf("aff is %d \r\n", head->affairs);
			
			for(i = 0; client_aff_table[i].aff_fun != NULL; i++)
			{
				if(client_aff_table[i].aff == head->affairs)
				{
					client_aff_table[i].aff_fun(sys_cfg.sockfd, recvbuf, ret, &clientaddr);
					break ;
				}
			}
		}
	}

}




/********************************************
������: connet_to_server
����:		�����������UDP ��������
*********************************************/
int connet_to_server(char *serverip, int port)
{
	char sendbuf[1024];
	int send_len;
	struct sockaddr_in servaddr;
	struct sockaddr_in recvaddr;
	int nread;
	int ret;
	int fromlen;
	char recvbuf[1024];
    fd_set readfds;
    fd_set writefds;
	struct timeval tv;

	struct check_head head, *recv_head;;
	char *str;

	struct proto_s_login_ack *ack;


	printf("connet to %s:%d.....\r\n", serverip, port);

	/* ����������socket ��ǩ�� */
	memset(&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(serverip);
    servaddr.sin_port = htons(port);

	/* �����ͷ */
	compages_head(&head, _aff_client_login_);

	/* �ϲ����� */	
	memcpy(sendbuf, &head, sizeof(head));
	send_len = sizeof(head);
	
    ret = p2p_sendto(sys_cfg.sockfd, sendbuf, send_len, 0, (struct sockaddr *)&servaddr, sizeof(struct sockaddr));
	if(ret == -1){
		return -1;
	}

   while(1)
   {
	   
	   int maxfd = sys_cfg.sockfd;
	   
	   FD_ZERO(&readfds);
	   FD_ZERO(&writefds);
	   FD_SET(sys_cfg.sockfd, &readfds);

	   /* �ȴ�5 �룬���������û�з��أ���ôʧ�� */
	   tv.tv_sec = 5;
	   tv.tv_usec = 0;
	   ret = select(maxfd + 1, &readfds, &writefds, NULL, &tv);
	   
	   if (ret > 0)
	   {
		   if (FD_ISSET(sys_cfg.sockfd, &readfds)) 
		   {
			   fromlen = sizeof(servaddr);
			   nread = p2p_recvfrom(sys_cfg.sockfd,(char *)&recvbuf,sizeof(recvbuf),0,(struct sockaddr *)&recvaddr, (socklen_t *)&fromlen);
			   if(nread <= 0)
				   return -1;
			   
//			   printf_client(&recvaddr);

			   if(recvaddr.sin_addr.s_addr != inet_addr(sys_cfg.serverip))
				   continue;

			   recv_head = (struct check_head *)recvbuf;
			   if((recv_head->affairs == _aff_server_login_ack_))
				   goto login_ack;
		   }
	   }
	   ret = p2p_sendto(sys_cfg.sockfd, sendbuf, send_len, 0, (struct sockaddr *)&servaddr, sizeof(struct sockaddr));
	   if(ret == -1){
		   perror("sendto ");
	   }
   }

login_ack:
	
	ack = (struct proto_s_login_ack *)(recvbuf + sizeof(struct check_head));

	if(ack->ack == 0)
	{
		printf("connet is ok!\r\n");
	}else{
		printf("connet is err!\r\n");
	}
	return ack->ack;		//���ص�½���

}



/********************************************
������: sync_service
����:		������ͬ�����ӽ���
*********************************************/

void *sync_thread_fun(void *pdata)
{
	while(1)
	{
		sync_service(); 			//ͬ��������
		update_client_node_callcnt();	//����client�ڵ����Ϣ
		sleep(SYNC_TIME);
	}
}



/************************************************
������: client_recv_pthread
����:		�ͻ��˽��մ����߳�
************************************************/
void *client_recv_pthread(void *pdata)
{
	
	pthread_t sync_thread;
    pthread_attr_t sync_thread_attr;

	/* ���ӵ������� */
	connet_to_server(sys_cfg.serverip, sys_cfg.ser_port);

	/* ����һ��ͬ�����߳� */
	pthread_attr_init(&sync_thread_attr);		//��ʼ����������
    pthread_attr_setdetachstate(&sync_thread_attr, PTHREAD_CREATE_DETACHED);
    if (pthread_create(&sync_thread, &sync_thread_attr, sync_thread_fun, NULL) < 0)
    {
        perror("pthread_create");
    }


	client_recv_handle();

	while(1);
}




/********************************************
������: sync_service
����:		������������������
*********************************************/

void sync_service(void)
{
	int ret;
	struct sockaddr_in servaddr;
	struct check_head head;
	char sendbuf[1024];
	int send_len;

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(sys_cfg.serverip);
    servaddr.sin_port = htons(sys_cfg.ser_port);

	compages_head(&(head), _aff_client_sync_);

	memcpy(sendbuf, &head, sizeof(head));
	send_len = sizeof(head);

	ret = p2p_sendto(sys_cfg.sockfd, sendbuf, send_len, 0, (struct sockaddr *)&servaddr, sizeof(struct sockaddr));
	if(ret == -1){
		perror("sendto ");
	}
}



void init_network_pthread(void)
{
	pthread_t thread;
    pthread_attr_t thread_attr;
	
	pthread_t aff_thread;
    pthread_attr_t aff_thread_attr;

	/* �����ӽ��� */
	pthread_attr_init(&thread_attr);		//��ʼ����������
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	if(pthread_create(&thread, &thread_attr, client_recv_pthread, NULL) < 0)
	{
		perror("pthread_create");
	}
}




/* ----------------------------------------------------------------------*/
/*
	���յ����ݵ�һ�δ���
*/
void resolve_recv_data(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
{
	struct proto_c_send_data *recv_proto;
	recv_proto = (struct proto_c_send_data *)(buf + sizeof(struct check_head));

//	printf("___ %d %d %d \r\n", (clientaddr->sin_port), ntohs(clientaddr->sin_port), htonl(clientaddr->sin_port));

	add_recv_pthread_list(clientaddr, buf, len);

#if 0
	switch(recv_proto->c_proto)
	{
		/* ���������Ϣ */
		case _proto_c_msg_:
		{
			/* ��������ӵ����������ȥ */
			//add_recv_data_list(buf, len, clientaddr);
			add_recv_pthread_list(clientaddr, buf, len);
			break;
		}
		/* ��������ļ� */
		case _proto_c_file_:
		{
			break;
		}
	}
#endif
}




/* ---------------------------------------------------------------------- */

/* �ṩ���û���ͬ���������� */
static pthread_mutex_t recv_mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t recv_cond = PTHREAD_COND_INITIALIZER;

/* ������ */
static pthread_mutex_t recv_list_lock = PTHREAD_MUTEX_INITIALIZER;


LIST_HEAD(recv_data_list);		//���岢��ʼ��һ������ͷ
volatile int recv_data_num = 0;



void __add_recv_data(struct recv_data *r_data)
{
	struct recv_data *pos,*n;
	/* ��Ҫ���̫��ڵ���Ϣ */
	pthread_mutex_lock(&recv_list_lock);
	if(recv_data_num > 50)
	{		
		list_for_each_entry_safe_reverse(pos,n,&recv_data_list,list, struct recv_data)
		{
			list_del(&(pos->list));
			free(pos);
			recv_data_num --;
			goto out1;
		}
	}
out1:
	list_add(&r_data->list, &recv_data_list);
	recv_data_num ++;
	
	pthread_mutex_unlock(&recv_list_lock);
}

/* �ѽ��յ����������ݰ��ŵ�������ȥ */
int add_recv_data_list(char *buf, int len, struct sockaddr_in *addr)
{
	struct recv_data *new_data;
	char *data;
	
	new_data = (struct recv_data*)malloc(sizeof(struct recv_data));
	if(new_data == NULL)
		return -1;

	data = (char *)malloc(len);
	if(data == NULL){
		free(new_data);
		return -1;
	}

	/* �������� */
	if(addr != NULL)
		memcpy(&(new_data->recv_addr), addr, sizeof(struct sockaddr_in));

	memcpy(data, buf, len);
	new_data->data = data;
	new_data->data_len = len;

	pthread_mutex_lock(&recv_mtx);//��Ҫ����head����ٽ���Դ���ȼ�����

	__add_recv_data(new_data);
	pthread_cond_signal(&recv_cond);		//��������

	pthread_mutex_unlock(&recv_mtx);//����
	return 0;
}



/*
��������ڵ������
time ��ʾ�ȴ�ʱ�䡣��λ��
-1 ��ʾ���õȴ�
*/
int recv_node_data(char *buf, int len, struct sockaddr_in *addr, int delay_time)
{
	int ret = -1;

	pthread_mutex_lock(&recv_mtx);

	while(recv_data_num == 0){
		if(delay_time == -1)
		{
			pthread_cond_wait(&recv_cond,&recv_mtx);	//�ȴ�����
		}else{
			struct timespec to;
			to.tv_sec = time(NULL) + delay_time;
			to.tv_nsec = 0;
			ret = pthread_cond_timedwait(&recv_cond,&recv_mtx, &to);	//�ȴ�����.��ʱ�˳�
			if(ret == ETIMEDOUT)
			{				
				pthread_mutex_unlock(&recv_mtx);//�ٽ������ݲ�����ϣ��ͷŻ�����
				return -1;		//��ʱ�˳�
			}
		}
		
	}

	ret = -1;
	struct recv_data *pos,*n;

	pthread_mutex_lock(&recv_list_lock);
	list_for_each_entry_safe_reverse(pos,n,&recv_data_list,list, struct recv_data)
	{
		/* �������� */
		ret = __min(len, pos->data_len);
		memcpy(buf, pos->data, ret);
		/* ���Ƶ�ַ */
		memcpy(addr, &(pos->recv_addr), sizeof(struct sockaddr_in));
		
		list_del(&(pos->list));

		/* �ͷ��ڴ� */
		free(pos->data);
		free(pos);
		
		recv_data_num --;
		goto out1;
	}
out1:
	pthread_mutex_unlock(&recv_list_lock);

	pthread_mutex_unlock(&recv_mtx);//�ٽ������ݲ�����ϣ��ͷŻ�����

	return ret;
}


