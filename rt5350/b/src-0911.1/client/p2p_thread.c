
/*
P2P��صĺ���
*/
#include "all.h"
#include "client.h"

#include <pthread.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <stdio.h>



/*
�����������С�
һ���Ǵ����ԵĶ���
һ���ǲ���ͨ���Ķ���
*/



LIST_HEAD(p2p_test_list);		//���岢��ʼ��һ������ͷ
volatile int p2p_test_node_num = 0;


//LIST_HEAD(p2p_sync_list);		//���岢��ʼ��һ������ͷ
//volatile int p2p_sync_node_num = 0;


/* ��Щ�����漰�����̲߳������⣬���ô��� */
/* ����ϻ����� */
static pthread_mutex_t p2p_test_list_lock = PTHREAD_MUTEX_INITIALIZER;
//static pthread_mutex_t p2p_sync_list_lock = PTHREAD_MUTEX_INITIALIZER;


void p2p_sendto_node(struct p2p_node_info *node, int aff)
{
	char send_buf[512];
	int send_len;
	struct sockaddr_in addr;
	struct check_head head;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(node->ip);
	addr.sin_port = htons(node->port);


	if(node->list_typpe == 0)
	{
		
		compages_head(&(head), _aff_client_p2p_test_);
		memcpy(send_buf, &head, sizeof(struct check_head));
		send_len = sizeof(struct check_head);

		if(node->test_cnt > 0)
		{
//			printf("send p2p test \r\n");
			p2p_sendto(sys_cfg.sockfd, (char *)send_buf, send_len, 0, (struct sockaddr *)&addr,
		   		sizeof(struct sockaddr));
			node->test_cnt --;
		}else{
			list_del(&(node->list));
			p2p_test_node_num --;
			free(node);
		}
	}else if(node->list_typpe == 1){
		compages_head(&(head), _aff_client_p2p_sync_);
		memcpy(send_buf, &head, sizeof(struct check_head));
		send_len = sizeof(struct check_head);

		if(node->sync_cnt > 0)
		{
//			printf("send p2p sync \r\n");
			p2p_sendto(sys_cfg.sockfd, (char *)send_buf, send_len, 0, (struct sockaddr *)&addr,
		   		sizeof(struct sockaddr));
			node->sync_cnt --;
		}else{
			list_del(&(node->list));
			p2p_test_node_num --;
			free(node);
		}
	}
}

struct p2p_node_info * __add_p2p_node
								(struct proto_s_client_info *info, struct list_head *list_head)
{
	struct p2p_node_info *node;

	node = malloc(sizeof(struct p2p_node_info));
	if (node != NULL)
	{		
		memcpy(node->name, info->name, USER_NAME_LEN);		
		strcpy(node->ip, info->ip);
		
		node->status = info->status;
		node->port = atoi(info->port);
		node->test_cnt = 3;
		node->list_typpe = 0;		/* ���Զ��� */

		pthread_mutex_lock(&p2p_test_list_lock);
		
		list_add(&node->list,list_head);
		p2p_test_node_num ++;
		pthread_mutex_unlock(&p2p_test_list_lock);
		return node;
	}
	return NULL;
}


int add_p2p_node(struct proto_s_client_info *info, struct list_head *list_head)
{
	struct p2p_node_info *node;
	struct p2p_node_info *pos,*n;


	
	/* ��Ҫ���̫��ڵ���Ϣ */
	pthread_mutex_lock(&p2p_test_list_lock);
	if(p2p_test_node_num > 50)
	{		
		list_for_each_entry_safe_reverse(pos,n,list_head,list, struct p2p_node_info)
		{
			list_del(&(pos->list));
			free(pos);
			p2p_test_node_num --;
			goto out1;
		}
	}
out1:
	pthread_mutex_unlock(&p2p_test_list_lock);

	node = __add_p2p_node(info, list_head);
	p2p_sendto_node(node, _aff_client_p2p_test_);
}


/*
���������ҵ�ĳ��Ԫ��
*/
struct p2p_node_info *find_from_list(unsigned char *name, struct list_head *list_head)
{
	struct p2p_node_info *pos,*n;
	int i = 0;

	char tmp[30];
//	__buf_to_str(tmp, name, 8);
//	printf("name [%s]\r\n", tmp);
	
	pthread_mutex_lock(&p2p_test_list_lock);
	list_for_each_entry_safe_reverse(pos,n,list_head,list, struct p2p_node_info)
	{
//		__buf_to_str(tmp, pos->name, 8);
//		printf("find pos name [%s]\r\n", tmp);
		if(memcmp(pos->name, name, USER_NAME_LEN) == 0)
		{
//			printf("find 2\r\n");
			pthread_mutex_unlock(&p2p_test_list_lock);
			return pos;
		}
	}
	pthread_mutex_unlock(&p2p_test_list_lock);

	return NULL;
}

void printf_p2p_node_list(struct list_head *list_head)
{
	struct p2p_node_info *pos,*n;
	int i = 0;
	
	pthread_mutex_lock(&p2p_test_list_lock);
	list_for_each_entry_safe_reverse(pos,n,list_head,list, struct p2p_node_info)
	{
		i ++;
		printf("[%d] name %s\n",i, pos->name);
		printf("[%d] status %d\n",i, pos->status);
		printf("[%d] ip %s\n",i, pos->ip);
		printf("[%d] port %d\n",i, pos->port);
		printf("[%d] test cnt %d\n",i, pos->test_cnt);
		printf("\r\n");
	}
	pthread_mutex_unlock(&p2p_test_list_lock);
}

void printf_p2p_test_node_list(void)
{
	printf_p2p_node_list(&p2p_test_list);
}

void printf_p2p_sync_node_list(void)
{
	printf_p2p_node_list(&p2p_test_list);
}


/* ��������P2P �ڵ� */
void p2p_sendto_test(struct list_head *list_head)
{	
	struct p2p_node_info *pos,*n;
	
	pthread_mutex_lock(&p2p_test_list_lock);
	list_for_each_entry_safe_reverse(pos,n,list_head,list, struct p2p_node_info)
	{
		p2p_sendto_node(pos, _aff_client_p2p_test_);
	}
	pthread_mutex_unlock(&p2p_test_list_lock);
}

/* ����ͬ���ڵ� */
void p2p_sendto_sync(struct list_head *list_head)
{	
	struct p2p_node_info *pos,*n;
	
	pthread_mutex_lock(&p2p_test_list_lock);
	list_for_each_entry_safe_reverse(pos,n,list_head,list, struct p2p_node_info)
	{
		p2p_sendto_node(pos, _aff_client_p2p_sync_);
	}
	pthread_mutex_unlock(&p2p_test_list_lock);
}


/*************************************************************************/
void add_p2p_test_node(struct proto_s_client_info *info)
{
	add_p2p_node(info, &p2p_test_list);
}


/* ��ӵ�ͬ������ */
void add_p2p_sync_node(struct check_head* head)
{
	struct p2p_node_info *node;
	node = find_from_list(head->name, &p2p_test_list);
	if(node == NULL)
		return ;

	node->list_typpe = 1;		/* ͬ������ */
	node->sync_cnt = 3;			/* ͬ������ */
}

/* �Ӷ�����ɾ�� */
void del_p2p_sync_node(char *name)
{
	struct p2p_node_info *pos,*n;
	int i = 0;
	
	pthread_mutex_lock(&p2p_test_list_lock);
	list_for_each_entry_safe_reverse(pos,n, &p2p_test_list, list, struct p2p_node_info)
	{
		if(memcmp(pos->name, name, USER_NAME_LEN) == 0)
		{
			list_del(&(pos->list));
			free(pos);
			pthread_mutex_unlock(&p2p_test_list_lock);
			return ;
		}
	}
	pthread_mutex_unlock(&p2p_test_list_lock);

}



/* �յ�ͬ��Ӧ�𡣸��� */
void update_p2p_sync_node(struct check_head* head)
{
	struct p2p_node_info *node;
	node = find_from_list(head->name, &p2p_test_list);
	if(node != NULL)
	{
		node->sync_cnt = 3;
	}
}

void *p2p_thread_sync(void *pdata)
{
	while(1)
	{
		p2p_sendto_test(&p2p_test_list);
//		p2p_sendto_sync(&p2p_sync_list);
		sleep(15);
	}
}



/********************************************************
������:	p2p_thread_init
����:		��ʼ��ͬ���߳�
********************************************************/
void p2p_thread_init(void)
{
    pthread_t p2p_thread;
    pthread_attr_t p2p_thread_attr;

	/* ����һ���߳�ȥ����P2P ���� */
	pthread_attr_init(&p2p_thread_attr);		//��ʼ����������
	pthread_attr_setdetachstate(&p2p_thread_attr, PTHREAD_CREATE_DETACHED);
	if (pthread_create(&p2p_thread, &p2p_thread_attr, p2p_thread_sync, NULL) < 0)
	{
		perror("pthread_create");
	}
}




/* ----------------------------------------------------------------------------------- */
#if 0
int p2p_send_data(char *name, char *buf, int len)
{
	
	int ret;
	char send_buf[len + 200];
	int send_len;
	struct sockaddr_in addr;
	struct check_head head;
	struct p2p_node_info *node;

	node = find_from_list(name, &p2p_sync_list);
	if(node == NULL)
	{
		return -1;
	}


	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(node->ip);
	addr.sin_port = htons(node->port);

	compages_head(&(head), _aff_client_p2p_data_);
	memcpy(send_buf, &head, sizeof(struct check_head));
	send_len = sizeof(struct check_head);
	memcpy(send_buf + send_len, buf, len);
	send_len += len;

	
	printf("p2p_send_data \r\n");
	ret = p2p_sendto(sys_cfg.sockfd, (char *)send_buf, send_len, 0, (struct sockaddr *)&addr,
		sizeof(struct sockaddr));

	/* ��������ӵ�������ȥ */
	add_net_data_list(send_buf, send_len, &addr);

	return ret;
}
#endif



/* �������� */
/*
	r ���Ƿ��� -1 ���ʾ���û���֧��P2P ͨ��
*/
int __p2p_send_data(unsigned char *name, unsigned int c_proto, char *buf, int len, int *r, int wait_flg, int delay_time)
{
	
	int ret;
	char send_buf[len + 200];
	int send_len;
	struct sockaddr_in addr;
	struct check_head head;
	struct p2p_node_info *node;

	node = find_from_list(name, &p2p_test_list);
	if(node == NULL)
	{
		(*r) = -1;
		return -1;
	}

	(*r) = 0;
	
	/* �ж��Ƿ��Ǵ���ͬ��״̬ */
	if((node->list_typpe != 1) || (node->sync_cnt <= 0))
		return -1;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(node->ip);
	addr.sin_port = htons(node->port);

	struct proto_c_send_data proto;
	memcpy((proto.dest_name), name, USER_NAME_LEN);
	memcpy((proto.src_name), sys_cfg.myname, USER_NAME_LEN);
	proto.c_proto = c_proto;		/* Э�� */

	compages_head(&(head), _aff_client_p2p_data_);
	
	memcpy(send_buf, &head, sizeof(head));
	memcpy(send_buf + sizeof(head), &proto, sizeof(proto));
	send_len = sizeof(head) + sizeof(proto);
	memcpy(send_buf + send_len, buf, len);
	send_len += len;

	
	printf("p2p_send_data \r\n");
	ret = p2p_sendto(sys_cfg.sockfd, (char *)send_buf, send_len, 0, (struct sockaddr *)&addr,
		sizeof(struct sockaddr));
	
	if(wait_flg == 0)
		return ret;

	/* ��������ӵ�������ȥ */
	
	add_net_data_list(send_buf, send_len, &addr);		//�ɿ��������Ĵ������Ҫ

	if(wait_ack_head(&head, delay_time) == 0)
	{
		return ret;
	}else{
		return -1;
	}
}



/* ���������� */
int p2p_send_data(char *name, unsigned int proto, char *buf, int len)
{
	int r;
	int ret;
	
	ret = __p2p_send_data(name, proto, buf, len, &r, 0, 0);

	return ret;
}


int p2p_send_data_2(char *name, unsigned int proto, char *buf, int len, int *r, int delay_time)
{
	int ret;
	ret = __p2p_send_data(name, proto, buf, len, r, 1, delay_time);

	return ret;
}


