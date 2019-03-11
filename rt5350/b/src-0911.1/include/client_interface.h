
#ifndef __CLIENT_INTERFACE_H__
#define __CLIENT_INTERFACE_H__

#ifdef __CLIENT_INTERFACE_C__
#define __CLIENT_INTERFACE_EXT__
#else
#define __CLIENT_INTERFACE_EXT__
#endif

#include "all.h"
#include "client.h"

struct client_interface_ops{
	/* ���յ���������ת������ */
	void (*recv_turn)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr);	
	/* ���յ���������ת��Ӧ��� */
	void (*recv_turn_ack)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr); 
	/* ���������ؽڵ���Ϣ */
	void (*recv_node_info)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr);

	/* ����ʧ�� */	
	void (*data_send_err)(struct net_data *net_dat);

	
	/* ���յ�P2P������ */
	void (*recv_stun)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr);	
	/* ���յ�P2P��Ӧ��� */
	void (*recv_stun_ack)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr);

	/* ���������ݵĴ��� */
	void (*listen_msg)(listen_t *listen);
	void (*listen_file)(listen_t *listen);
	void (*listen_audio)(listen_t *listen);
	void (*listen_voide)(listen_t *listen);
	
};


__CLIENT_INTERFACE_EXT__ struct client_interface_ops client_inface;



/*---------------------------------------------------------
p2p_client_init
	��ʼ��
---------------------------------------------------------*/
int p2p_client_init(void);


/*---------------------------------------------------------
client_command_init
	client �򵥵������г�ʼ�����ú����Ѿ��� 
	p2p_client_init �б�����
---------------------------------------------------------*/
void client_command_init(void);



/*---------------------------------------------------------
set_data_send_err
	���÷�������ʧ�ܵĻص�����
---------------------------------------------------------*/
void set_data_send_err(
	void (*data_send_err)(struct net_data *net_dat)
);



/*---------------------------------------------------------
set_recv_node_info
	���ý��յ��ͻ��˽ڵ�Ļص�����
	�ӷ������ϻ��P2P�ڵ���Ϣ�󡣻�ʹ�ø�
	�ص�����
---------------------------------------------------------*/
void set_recv_node_info(
	void (*recv_node_info)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
);



/*---------------------------------------------------------
set_recv_stun_ack
	�յ�turn ��Ӧ����Ļص�����
---------------------------------------------------------*/
void set_recv_turn_ack(
	void (*recv_stun_ack)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
);



/*---------------------------------------------------------
set_recv_stun_ack
	�յ�turn ���ݰ��Ļص�����
---------------------------------------------------------*/
void set_recv_turn(
	void (*recv_stun)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
);




/*---------------------------------------------------------
set_recv_stun
	���ý��յ�stun(P2P���ݰ�)�Ĵ����� 
---------------------------------------------------------*/
void set_recv_stun(
	void (*recv_stun)(int sockfd, char *buf, int len, struct sockaddr_in *clientaddr)
);



/*---------------------------------------------------------
sendto_node_unreliable 
	���ɿ������ݴ��䡣����ֵ����0  ��ʾ���ͳɹ�
	���ǲ���֤�Է�һ�����յ�������UDP
	��������

����
	name	Ŀ������
	buf		����ָ��
	len		���ݳ���
---------------------------------------------------------*/
int sendto_node_unreliable(char *name, unsigned proto, char *buf, int len);

/*---------------------------------------------------------
sendto_node_reliable 
	��Կɿ������ݴ��䡣����ֵ����0  ��ʾ���ͳɹ�
	���ҶԷ�һ�����յ���
	�ú�����ȴ��Է���Ӧ���źš����������ֵ����0
	����ζ�ŶԷ�һ���յ���
	����Է���Ӧ������10��󽫻᷵��-1

����
	name	Ŀ������
	buf		����ָ��
	len		���ݳ���
	delay_time	����ʱ�䡣����ʵ��������ʱ����20�벻��
---------------------------------------------------------*/
int sendto_node_reliable(char *name, unsigned proto, char *buf, int len, int delay_time);



/*---------------------------------------------------------
set_recv_list

	��ʼ�����ն���
	
����:
	mode 	1 ��ʼ�����ն���
			0 ���ٽ��ն���
---------------------------------------------------------*/
void set_recv_list(int mode);


/*---------------------------------------------------------
recv_node_data
	���������ݶ�����ȡ��һ�����ݰ�
	
����:
	buf		����ָ��
	len		׼�����ն����ֽ�
	addr	�Է���ַ��Ϣ
	delay_time	Ϊ -1 ���ʾ������һֱ����
			����0 ��ʾ������������һ��ʱ�䡣Ȼ�󷵻�
---------------------------------------------------------*/
int recv_node_data(char *buf, int len, struct sockaddr_in *addr, int delay_time);


#endif


