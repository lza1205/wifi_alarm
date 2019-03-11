
#define __CLIENT_C_

#include "all.h"

#include "client.h"


#if 0
/* ͬ������ */
void *my_thread_recv(void *pdata)
{
	int ret;
	char buf[1024];
	struct sockaddr_in recv_addr;
	while(1)
	{
		ret = recv_node_data(buf, 1024, &recv_addr, -1);
		if(ret < 0)
		{
			printf("my recv is err \r\n");
		}else{
			char *recv_data;
			struct check_head *head = (struct check_head *)buf;
			struct proto_c_send_data *recv_proto = (struct proto_c_send_data *)(buf + sizeof(struct check_head));
			recv_data = (char *)(buf + sizeof(struct check_head) + sizeof(struct proto_c_send_data));

			printf("recv from %s 's %s data : [%s] \r\n", recv_proto->src_name, 
												((head->affairs == _aff_client_p2p_data_)?"stun":"turn"),
												recv_data);
		}
	}
}

void my_thread_init(void)
{
    pthread_t p2p_thread;
    pthread_attr_t p2p_thread_attr;

	/* ����һ���߳�ȥ����P2P ���� */
	pthread_attr_init(&p2p_thread_attr);		//��ʼ����������
	pthread_attr_setdetachstate(&p2p_thread_attr, PTHREAD_CREATE_DETACHED);
	if (pthread_create(&p2p_thread, &p2p_thread_attr, my_thread_recv, NULL) < 0)
	{
		perror("pthread_create");
	}
}
#endif

void *listen_phread(void *pdata)
{
	int ret;
	char buf[1204];
	struct sockaddr_in clientaddr;
	struct listen *_listen;
	_listen = (struct listen *)pdata;

	/* �������listen  */
	listen_start(_listen);

	while(1)
	{
		ret = recv_from_listen(_listen, (struct sockaddr *)&clientaddr, buf, 1204, 300);
		if(ret == -1)
		{
			printf("%p recv is err \r\n", _listen);
			break;		//ʱ�䵽���˳�
		}else{
			
			char *recv_data;
			char name[30];
			struct check_head *head = (struct check_head *)buf;
			struct proto_c_send_data *recv_proto = (struct proto_c_send_data *)(buf + sizeof(struct check_head));
			recv_data = (char *)(buf + sizeof(struct check_head) + sizeof(struct proto_c_send_data));

			__buf_to_str(name, recv_proto->src_name, USER_NAME_LEN);
			printf("recv from %s 's %s data : [%s] \r\n", name, 
												((head->affairs == _aff_client_p2p_data_)?"stun":"turn"),
												recv_data);		
			/* ���մ��� */
			recv_process(recv_proto->src_name, recv_data, ret, clientaddr);
		}
	}
	/* �ر����� */
	listen_close(_listen);
}


void my_listen_msg(listen_t *listen)
{
	listen_pthread(listen, listen_phread);
}


int main(int argc, char *argv[])
{
	char command[1024];
	char *str;

	
	printf("head byte : %d \r\n", sizeof(struct check_head));
	printf("proto byte : %d\r\n", sizeof(struct proto_c_send_data));
	//debug_buf();

	p2p_client_init();

	set_recv_list(1);
//	my_thread_init();

	/* ����listen msg �Ļص����� */
	set_listen_msg(my_listen_msg);

	/* wifi ��������ʼ�� */
	wifi_alarm_init();

	while(1)
	{
        str = fgets(command, 1024, stdin);
		if(str != NULL)
		{
	        command_format(command);
			printf("client @%s: $ ", sys_cfg.name_str);
		}

	}
	return 0;
}


