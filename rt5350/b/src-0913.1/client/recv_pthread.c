
#include "all.h"
#include "client.h"

#include "recv_pthread.h"
#include <sys/time.h>

/*
ģ��
*/
static pthread_mutex_t mb_mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t mb_cond = PTHREAD_COND_INITIALIZER;

int my_pthread_mutex_init(pthread_mutex_t *mtx, char *p)
{
	memcpy(mtx, &mb_mtx, sizeof(pthread_mutex_t));
	return 0;
}

int my_pthread_cond_init(pthread_cond_t *cond, char *p)
{
	memcpy(cond, &mb_cond, sizeof(pthread_cond_t));
	return 0;
}



LIST_HEAD(recv_pthread_list);		//���岢��ʼ��һ������ͷ

/* ����ϻ����� */
static pthread_mutex_t recv_pthread_list_lock = PTHREAD_MUTEX_INITIALIZER;


/* ������ص� */
static pthread_mutex_t listen_mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t listen_cond = PTHREAD_COND_INITIALIZER;

/* poll ������ص� */
static pthread_mutex_t poll_mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t poll_cond = PTHREAD_COND_INITIALIZER;


/* ��������poll �߳� */
static void poll_cond_broadcast(void)
{
	pthread_mutex_lock(&poll_mtx);//��Ҫ����head����ٽ���Դ���ȼ�����
	pthread_cond_broadcast(&poll_cond);		//��������
	pthread_mutex_unlock(&poll_mtx);//����
}



static int recv_list_num, recv_list_max;


void add_data_to_list(
	struct listen *recv_list, 
	struct sockaddr_in *addr,		/* ���ݰ���ַ��Ϣ */
	char *data,					/* ���ݰ����� */
	int len					/* ���ݰ����� */
);

void add_recv_pthread_list(
	struct sockaddr_in *addr,		/* ���ݰ���ַ��Ϣ */
	char *buf,					/* ���ݰ����� */
	int len					/* ���ݰ����� */
)
{
	struct proto_c_send_data *proto_c = (struct proto_c_send_data *)(buf + sizeof(struct check_head));
	
	int ret;
	struct listen *pos,*n;


	
//	printf("___ %d %d %d \r\n", (addr->sin_port), ntohs(addr->sin_port), htonl(addr->sin_port));

	/* ��Ҫ���̫��ڵ���Ϣ */
	pthread_mutex_lock(&recv_pthread_list_lock);
	
	list_for_each_entry_safe_reverse(pos, n, &recv_pthread_list, listen_list, struct listen)
	{
		//if(memcmp(addr, &(pos->addr), sizeof(struct sockaddr_in)) == 0)
		if((memcmp(proto_c->src_name, pos->name, USER_NAME_LEN) == 0) && (proto_c->c_proto == pos->proto))
		{
			/* ��ַ��ͬ���������ݵ��ö�����ȥ */
			add_data_to_list(pos, addr, buf, len);
			poll_cond_broadcast();			//���� poll �߳�

			pthread_mutex_unlock(&recv_pthread_list_lock);
			return ;
		}
	}
	if(recv_list_num < recv_list_max)
	{
		/* �����µĽ�������ͷ */
		struct listen *head;
		head = (struct listen *)malloc(sizeof(struct listen));
		if(head == NULL)
			goto out1;

		/* ��ʼ���߳��� */
		ret = my_pthread_mutex_init(&head->mutex, NULL);
		if(ret != 0)
		{
			free(head);
			goto out1;
		}

		/* ��ʼ������������ص� */
		ret = my_pthread_cond_init(&head->recv_cond, NULL);
		if(ret != 0)
		{
			free(head);
		}
		ret = my_pthread_mutex_init(&head->recv_mtx, NULL);
		if(ret != 0){
			free(head);
		}
		
		/* ��ʼ������ͷ */
		INIT_LIST_HEAD(&(head->head));
		
		head->data_num = 0;
		head->data_max_num = 10;	//�����10������

		/* ���Ƶ�ַ��Ϣ */
		memcpy(&(head->addr), addr, sizeof(struct sockaddr));		
//		printf("___ %d %d %d \r\n", (head->addr.sin_port), ntohs(head->addr.sin_port), htonl(head->addr.sin_port));

		/* �������֡�Э�� */
		memcpy(head->name, proto_c->src_name, USER_NAME_LEN);
		head->proto = proto_c->c_proto;

		/* �ӵ�������ȥ */
		list_add(&(head->listen_list), &recv_pthread_list);
		recv_list_num ++;

		add_data_to_list(head, addr, buf, len);
		poll_cond_broadcast();			//���� poll �߳�

		pthread_mutex_unlock(&recv_pthread_list_lock);

		/* ���Ѽ��� */
		pthread_mutex_lock(&listen_mtx);//��Ҫ����head����ٽ���Դ���ȼ�����

		head->list_flg = 1;		/* ��Ҫ�ϱ� */
		pthread_cond_signal(&listen_cond);		//��������

		pthread_mutex_unlock(&listen_mtx);//����

		return ;
		
	}


out1:
	pthread_mutex_unlock(&recv_pthread_list_lock);
	return ;
}




void add_data_to_list(
	struct listen *recv_list, 
	struct sockaddr_in *addr,		/* ���ݰ���ַ��Ϣ */
	char *buf,					/* ���ݰ����� */
	int len					/* ���ݰ����� */
)
{
	char *new_data;

	/* ����������󳤶� */
	pthread_mutex_lock(&recv_list->mutex);

	if(recv_list->data_num >= recv_list->data_max_num)
	{
		pthread_mutex_unlock(&recv_list->mutex);
		return ;
	}

	pthread_mutex_unlock(&recv_list->mutex);


	/* �����µĽ�������ͷ */
	struct recv_data_node *node;
	node = (struct recv_data_node *)malloc(sizeof(struct recv_data_node));
	if(node == NULL)
		return ;


	new_data = (char *)malloc(len);
	if(new_data == NULL)
	{
		free(node);
		return ;
	}
	

	memcpy(&(node->addr), addr, sizeof(struct sockaddr));
	
	node->len = len;
	memcpy(new_data, buf, len);
	node->data = new_data;

	pthread_mutex_lock(&recv_list->mutex);

	/* �ӵ�������ȥ */
	list_add(&(node->data_list), &(recv_list->head));
	recv_list->data_num ++;

	pthread_mutex_unlock(&recv_list->mutex);

	
	/* �����߳� */
	pthread_mutex_lock(&recv_list->recv_mtx);
	pthread_cond_signal(&recv_list->recv_cond);		//��������
	pthread_mutex_unlock(&recv_list->recv_mtx);	
}


#define __max(a,b)        (((a) > (b)) ? (a) : (b))
#define __min(a,b)        (((a) < (b)) ? (a) : (b))

int recv_from_listen(	
	struct listen *recv_list, 
	struct sockaddr_in *addr,		/* ���ݰ���ַ��Ϣ */
	char *buf,					/* ���ݰ����� */
	int len,					/* ���ݰ����� */
	int delay_time				/* ����ʱ�䡣 -1��ʾ�������� */
)
{
	int ret;

	pthread_mutex_lock(&recv_list->recv_mtx);
	
	while(recv_list->data_num == 0)
	{
		if(delay_time == -1)
		{
			pthread_cond_wait(&recv_list->recv_cond, &recv_list->recv_mtx);	//�ȴ�����
		}else{
			struct timespec to;
			to.tv_sec = time(NULL) + delay_time;
			to.tv_nsec = 0;
			ret = pthread_cond_timedwait(&recv_list->recv_cond, &recv_list->recv_mtx, &to);	//�ȴ�����.��ʱ�˳�
			if(ret == ETIMEDOUT)
			{				
				pthread_mutex_unlock(&recv_list->recv_mtx);//�ٽ������ݲ�����ϣ��ͷŻ�����
				return -1;		//��ʱ�˳�
			}
		}
	}


	pthread_mutex_lock(&recv_list->mutex);

	/* �Ӷ�����ȡ������ */
	ret = -1;
	struct recv_data_node *pos,*n;
	
	list_for_each_entry_safe_reverse(pos,n,&(recv_list->head), data_list, struct recv_data_node)
	{
		/* �������� */
		ret = __min(len, pos->len);
		memcpy(buf, pos->data, ret);
		/* ���Ƶ�ַ */
		memcpy(addr, &(pos->addr), sizeof(struct sockaddr_in));

		/* ɾ������ */
		list_del(&(pos->data_list));

		/* �ͷ��ڴ� */
		free(pos->data);
		free(pos);

		recv_list->data_num --;

		goto out1;
	}

out1:
	pthread_mutex_unlock(&recv_list->mutex);

	
	pthread_mutex_unlock(&recv_list->recv_mtx);//�ٽ������ݲ�����ϣ��ͷŻ�����

	return ret;
}



/* ��ȡһ������ */
struct listen *server_accept(void)
{	
	struct listen *pos,*n;

	pthread_mutex_lock(&listen_mtx);	

loop:	
	/* �ҵ���δ�������� */
	
	pthread_mutex_lock(&recv_pthread_list_lock);

	
	list_for_each_entry_safe_reverse(pos, n, &recv_pthread_list, listen_list, struct listen)
	{
		if(pos->list_flg == 1)		
		{
			/* ��Ҫ������ */
			pos->list_flg = 0;
			pthread_mutex_unlock(&recv_pthread_list_lock);

			pthread_mutex_unlock(&listen_mtx);
			return pos;
		}
	}
	pthread_mutex_unlock(&recv_pthread_list_lock);

	/* �ȴ��������� */

	pthread_cond_wait(&listen_cond, &listen_mtx);	//�ȴ�����

	goto loop;

	pthread_mutex_unlock(&listen_mtx);	
	return NULL;
}



/*
�ر�ĳ���ͻ���
*/
int listen_close(struct listen *listen)
{
	/* ɾ����P2P �ڵ� */
	del_p2p_sync_node(listen->name);

	pthread_mutex_lock(&recv_pthread_list_lock);

	/* �ͷ��ڴ� */	
	pthread_mutex_lock(&listen->mutex);
	struct recv_data_node *pos,*n;

	list_for_each_entry_safe_reverse(pos,n,&listen->head, data_list, struct recv_data_node)
	{
		/* ɾ������ */
		list_del(&(pos->data_list));

		/* �ͷ��ڴ� */
		free(pos->data);
		free(pos);
	}
	pthread_mutex_unlock(&listen->mutex);

	/* �ӽ��ն�����ɾ�� */
	list_del(&(listen->listen_list));
	recv_list_num --;

	free(listen);
	
	pthread_mutex_unlock(&recv_pthread_list_lock);

}


/* ����ĳ��listen */
/*
ʵ�����ǽ��ͻ�����ӵ� P2P ������
*/
int listen_start(struct listen *listen)
{
#if 0
	struct proto_s_client_info info;
	char str_port[IP_STRING_MAX];
	
	printf("___ %d %d %d \r\n", (listen->addr.sin_port), ntohs(listen->addr.sin_port), htonl(listen->addr.sin_port));
	sprintf(str_port, "%d", (listen->addr.sin_port));
	
	memcpy(info.ip, inet_ntoa(listen->addr.sin_addr), IP_STRING_MAX);
	memcpy(info.port, str_port, IP_STRING_MAX);
	memcpy(info.name, listen->name, USER_NAME_LEN);
	
	add_p2p_test_node(&info);
#endif
	get_client_info(listen->name);
}



/*--------------------------------------------------------------------*/
/*
����Ϊ��չ����
*/
/*--------------------------------------------------------------------*/


/**********************************************************************
ʵ��ͬʱ���ն�� listen ������
**********************************************************************/

/*
��ʼ��һ�� listen ����ͷ
*/
void listen_head_init(struct list_head *head)
{
	INIT_LIST_HEAD(head);
}


/* 
��Ҫ������listen ���뵽���head ������
*/
int listen_add(struct list_head *head, listen_t *listen)
{
	list_add(&(listen->poll_list), head);
}


int recv_from_listen_head(
		struct list_head* head,
		struct listen **recv_listen, 
		struct sockaddr_in *addr,		/* ���ݰ���ַ��Ϣ */
		char *buf,					/* ���ݰ����� */
		int len,					/* ���ݰ����� */
		int delay_time				/* ����ʱ�䡣 -1��ʾ�������� */
	)
{
	int ret;
	
	/* �ȿ�����û�����ݡ�û�вŽ���ȴ� */
	/* ���������ҵ����� */
	listen_t *pos,*n;
	
	list_for_each_entry_safe_reverse(pos,n, head, poll_list, listen_t)
	{
		
		(*recv_listen) = pos;
		ret = recv_from_listen(pos, addr, buf, len, 0);
		if(ret >= 0)
		{
			return ret;
		}
	}

	//printf("poll 3 \r\n");

	pthread_mutex_lock(&poll_mtx);

loop:
	if(delay_time == -1)
	{
		pthread_cond_wait(&poll_cond, &poll_mtx); //�ȴ�����
	}else{
		struct timespec to;
		to.tv_sec = time(NULL) + delay_time;
		to.tv_nsec = 0;
		ret = pthread_cond_timedwait(&poll_cond, &poll_mtx, &to); //�ȴ�����.��ʱ�˳�
		if(ret == ETIMEDOUT)
		{				
			pthread_mutex_unlock(&poll_mtx);//�ٽ������ݲ�����ϣ��ͷŻ�����
			return -1;		//��ʱ�˳�
		}
	}

	
	list_for_each_entry_safe_reverse(pos,n, head, poll_list, listen_t)
	{
		ret = recv_from_listen(pos, addr, buf, len, 0);
		if(ret >= 0)
		{
			(*recv_listen) = pos;
			pthread_mutex_unlock(&poll_mtx);
			return ret;
		}
	}

goto loop;

	pthread_mutex_unlock(&poll_mtx);

	return -1;
}



/*-------------------------------------------------------------------
�Լ�����һ�� listen �ӿ�
-------------------------------------------------------------------*/
//int __init_listen(listen_t *listen, struct sockaddr_in *addr)
int __init_listen(listen_t *listen, char *name, unsigned int proto)
{
	int ret;
	/* ��ʼ���߳��� */
	ret = my_pthread_mutex_init(&listen->mutex, NULL);
	if(ret != 0)
	{
		free(listen);
		return -1;
	}
	
	/* ��ʼ������������ص� */
	ret = my_pthread_cond_init(&listen->recv_cond, NULL);
	if(ret != 0)
	{
		free(listen);
	}
	ret = my_pthread_mutex_init(&listen->recv_mtx, NULL);
	if(ret != 0){
		free(listen);
	}
	INIT_LIST_HEAD(&(listen->head));
	
	listen->data_num = 0;
	listen->data_max_num = 10;	//�����10������
	
	/* ��������Э�� */
//	memcpy(&(listen->addr), addr, sizeof(struct sockaddr));
	memcpy(listen->name, name, USER_NAME_LEN);
	listen->proto = proto;
	
	/* �ӵ�������ȥ */
	list_add(&(listen->listen_list), &recv_pthread_list);

	return 0;
}

/* ����һ�� listen */
listen_t *create_listen(char *name, unsigned int proto)
{
	listen_t *listen = NULL;
	pthread_mutex_lock(&recv_pthread_list_lock);
	if(recv_list_num < recv_list_max)
	{
		/* �����µĽ�������ͷ */
		listen = (struct listen *)malloc(sizeof(struct listen));
		if(listen == NULL)
			goto out1;

		/* ��ʼ�� */
		__init_listen(listen, name, proto);
		recv_list_num ++;
	}

out1:
	pthread_mutex_unlock(&recv_pthread_list_lock);
	return listen;
}




/*-------------------------------------------------------------------
listen ����������
-------------------------------------------------------------------*/
/*
����listen ������������
*/
void set_listen_maxdata(listen_t *listen, int max)
{
	listen->data_max_num = max;
}





/*--------------------------------------------------------------------------------*/
/*
����һ���߳����������µ�listen
*/



void *server_listen_pthread(void *pdata)
{
	listen_t *listen;
	while(1)
	{
		listen = server_accept();

		if(listen == NULL){
			continue;
		}

		printf("new client \r\n");
		/* ����һ���̣߳����ң��ɸ��߳�ȥ�����������
			������TCP ��fork
		*/
		switch(listen->proto)
		{
			/* ��ϢЭ�� */
			case _proto_c_msg_:
			{
				client_inface.listen_msg(listen);
				break;
			}
			/* �ļ����� */
			case _proto_c_file_:
			{
				if(client_inface.listen_file != NULL)
					client_inface.listen_file(listen);
				break;
			}
			/* ��Ƶ����Э�� */
			case _proto_c_audio_:
			{
				sound_listen(listen);
				break;
			}
			/* ��Ƶ����Э�� */
			case _proto_c_voide_:
			{
				if(client_inface.listen_voide != NULL)
					client_inface.listen_voide(listen);

				break;
			}
		}
	}
}

/*
��ʼ����
*/
void server_listen(int num)
{	
	pthread_t recv_thread;
    pthread_attr_t recv_thread_attr;

	recv_list_max = num;

	/* �����ӽ��� */
	pthread_attr_init(&recv_thread_attr);		//��ʼ����������
	pthread_attr_setdetachstate(&recv_thread_attr, PTHREAD_CREATE_DETACHED);
	if (pthread_create(&recv_thread, &recv_thread_attr, server_listen_pthread, NULL) < 0)
	{
		perror("pthread_create");
		exit(1);
	}
}




/*
���������߳�
*/
void listen_pthread(listen_t *listen, void *(*start_rtn)(void *))
{	
	pthread_t recv_thread;
    pthread_attr_t recv_thread_attr;

	/* �����ӽ��� */
	pthread_attr_init(&recv_thread_attr);		//��ʼ����������
	pthread_attr_setdetachstate(&recv_thread_attr, PTHREAD_CREATE_DETACHED);
	if (pthread_create(&recv_thread, &recv_thread_attr, start_rtn, listen) < 0)
	{
		perror("pthread_create");
		exit(1);
	}
}


