
#define __CLIENT_INTERFACE_C__


#include "all.h"
#include "client.h"



void default_recv_stun(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
{
	struct proto_c_send_data *recv_proto;
	recv_proto = (struct proto_c_send_data *)(buf + sizeof(struct check_head));
	
	printf("recv data : \r\n");
	printf("%s send to %s stun data is [%s] \r\n", recv_proto->src_name, recv_proto->dest_name,
							buf + sizeof(struct check_head) + sizeof(struct proto_c_send_data));


	/* ��������ӵ����������ȥ */
	add_recv_data_list(buf, len, clientaddr);
#if 0
	switch(recv_proto->c_proto)
	{
		/* ���������Ϣ */
		case _proto_c_msg_:
		{
			/* ��������ӵ����������ȥ */
			add_recv_data_list(buf, len, clientaddr);
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

void default_recv_node_info(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
{
	//printf("get client's info is ok \r\n");
	struct proto_s_client_info *recv_proto;
	recv_proto = (struct proto_s_client_info *)(buf + sizeof(struct check_head));
	
	if(recv_proto->status != 0)
	{
		printf("%s is offline \r\n", recv_proto->name);
	}else{
		printf("%s is online \r\n", recv_proto->name);
	}
}

void default_recv_stun_ack(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
{
	printf("recv ack, send is ok \r\n");
	
	struct proto_c_send_data *recv_proto;
	recv_proto = (struct proto_c_send_data *)(buf + sizeof(struct check_head));
	
	printf("%s send to %s ack \r\n", recv_proto->src_name, recv_proto->dest_name);
}


void default_data_send_err(struct net_data *net_dat)
{
	printf("send failed! \r\n");
}

void default_recv_turn(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
{	
	struct proto_c_send_data *recv_proto;
	recv_proto = (struct proto_c_send_data *)(buf + sizeof(struct check_head));
	
	printf("recv data : \r\n");
	printf("%s send to %s turn data is [%s] \r\n", recv_proto->src_name, recv_proto->dest_name,
							buf + sizeof(struct check_head) + sizeof(struct proto_c_send_data));

	/* ��������ӵ����������ȥ */
	add_recv_data_list(buf, len, clientaddr);
	
#if 0
	switch(recv_proto->c_proto)
	{
		/* ���������Ϣ */
		case _proto_c_msg_:
		{
			/* ��������ӵ����������ȥ */
			add_recv_data_list(buf, len, clientaddr);
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

void default_recv_turn_ack(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
{
	printf("recv turn ack, send is ok \r\n");
	
	struct proto_c_send_data *recv_proto;
	recv_proto = (struct proto_c_send_data *)(buf + sizeof(struct check_head));
	
}


void default_listen_msg(listen_t *listen)
{
	printf("new msg listen \r\n");
}

struct client_interface_ops client_inface = {
	.recv_stun = default_recv_stun,
	.recv_node_info = default_recv_node_info,
	.recv_stun_ack = default_recv_stun_ack,
	.data_send_err = default_data_send_err,

	.recv_turn = default_recv_turn,	
	.recv_turn_ack = default_recv_turn_ack,

	.listen_msg = default_listen_msg,
};





/* �����Լ��Ĵ����� */
void set_recv_turn(
	void (*recv_turn)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
)
{
	client_inface.recv_turn = recv_turn;
}


/* �����Լ��յ���תack �ķ��� */
void set_recv_turn_ack(
	void (*recv_turn_ack)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
)
{
	client_inface.recv_turn_ack = recv_turn_ack;
}

/* �����Լ���ȡ�ڵ���Ϣ�Ĵ����� */
void set_recv_node_info(
	void (*recv_node_info)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
)
{
	client_inface.recv_node_info = recv_node_info;
}

/* �����Լ��ķ���ʧ�ܴ����� */
void set_data_send_err(
	void (*data_send_err)(struct net_data *net_dat)
)
{
	client_inface.data_send_err = data_send_err;
}


/* ���ý��յ�stun(P2P���ݰ�)�Ĵ����� */
void set_recv_stun(
	void (*recv_stun)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
)
{
	client_inface.recv_stun = recv_stun;
}


/* ���ý��յ�stun ack(P2P Ӧ���)�Ĵ����� */
void set_recv_stun_ack(
	void (*recv_stun_ack)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
)
{
	client_inface.recv_stun_ack = recv_stun_ack;
}


/* ����listen_msg �Ļص����� */
void set_listen_msg(void (*listen_msg)(listen_t *listen))
{
	client_inface.listen_msg = listen_msg;
}


/* �����Ƿ���ý��ն��� */
void set_recv_list(int mode)
{
	recv_list_flg = mode;
}


/* 
ȫ�ֳ�ʼ�� 
*/
int p2p_client_init(void)
{
	/* ������ӡ��Ϣ */
	set_app_printf(1);
	set_dbg_printf(1);
	set_sync_printf(1);

	/* AES256 ���ܳ�ʼ�� */
//	aes256_key_init();
	set_aes256(1);			//���� AES 256 ���ܷ�ʽ
	
	/* ����������ļ� */
	get_config_for_json();

	/* ��ʼ��socket */
    init_client_socket((unsigned short)sys_cfg.my_port);

	memcpy(sys_cfg.now_version, CLIENT_VERSION, sizeof(sys_cfg.now_version));
	printf("\nclient version :         %s\n\n", sys_cfg.now_version);
	
	/* ��ʼ���������ݶ��� */
	init_net_data_list();

	/* ����udp ͨ���߳� */
	init_network_pthread();

	/* P2P �̳߳�ʼ�� */
	p2p_thread_init();

	/* ��ʼ�������� */
	client_command_init();

	/* listen ��ʼ���� */
	server_listen(50);
}


/*
���ͺ���
ֻ�ܷ��͡����ܶԷ��Ƿ����յ�
*/
int sendto_node_unreliable(char *name, unsigned int proto, char *buf, int len)
{
	int ret;
	struct client_node *cn;

	ret = p2p_send_data(name, proto, buf, len);
	if(ret <= 0)
	{
		/* ����ʧ�ܵġ�Ҫ�����û���P2P����ɾ�� */

		del_p2p_sync_node(name);
		ret = send_client_data(name, proto, buf, len);
		cn = find_client_info(name);
		if(cn != NULL)
		{
			if(cn->call_cnt == 0)
			{
				get_client_info(name);
				cn->call_cnt = 10;
			}
		}else{
			get_client_info(name);
		}
	}
	return ret;
}


/*
�ɿ��ķ��͡����������
*/
int __sendto_node_reliable(char *name, unsigned int proto, char *buf, int len, int delay_time)
{
	int ret;
	int r;
	struct client_node *cn;

	ret = p2p_send_data_2(name, proto, buf, len, &r, delay_time);

	if(ret <= 0)
	{
		/* ����ʧ�ܵġ�Ҫ�����û���P2P����ɾ�� */
		del_p2p_sync_node(name);
		
		ret = send_client_data_2(name, proto, buf, len, delay_time);
		cn = find_client_info(name);
		if(cn != NULL)
		{
			if(cn->call_cnt == 0)
			{
				get_client_info(name);
				cn->call_cnt = 10;
			}
		}else{
			get_client_info(name);
		}
	}
	return ret;
}


/*
�ɿ��ķ��͡����������
*/
int sendto_node_reliable(char *name, unsigned int proto, char *buf, int len, int delay_time)
{
	__sendto_node_reliable(name, proto, buf, len, 20);
}

