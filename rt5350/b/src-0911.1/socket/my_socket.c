
/***********************************************************
�ļ���	:	my_socket.c
����		:	Faker
�汾��	:	1.0
����		:	2015.05.13

˵��:
	��TCP/IP Э��ջ��һ����װ

***********************************************************/


#include "all.h"


#include "aes256_data.h"

#include <openssl/aes.h>  
#include <openssl/rand.h>  




/***************************************
������: aes256_recvfrom
����: ��TCP/IP Э��ջ���ȡ UDP ���ݣ�ͬʱ
		�����ݽ���AES256 ����
***************************************/
int aes256_recvfrom(int fd, void *buf, int len, int flags, struct sockaddr *addr, socklen_t *addr_len)
{
	int ret;

	int ss,len_s;
	char buff_in[UDP_PACK_MAX_SIZE];

	len_s = len;
	if((ss = (len % 16)) != 0){
		len_s = len + (16 - ss);
	}

	if(len_s > UDP_PACK_MAX_SIZE){
		printf("recvfrom len_s > UDP_PACK_MAX_SIZE %d", len_s);
		return -1;
	}

	ret = recvfrom(fd, buf, len_s, flags, addr, addr_len);
	
	if((ss = (ret % 16)) != 0){
		printf("ret no 16x != 0\n");
		return -1;
	}
	
	memcpy(buff_in, buf, ret);
	aes256_data(buff_in, buf, ret, 
				aes256_key, sizeof(aes256_key), AES_DECRYPT);

	return ret;
}


/***************************************
������: aes256_sendto
����: ��TCP/IP Э��ջ�﷢�� UDP ���ݣ�ͬʱ
		�����ݽ���AES256 ���ܣ����Զ�����
		Ϊ 16 �ı���
***************************************/
int aes256_sendto(int fd, void *buf, int len,
		       int flags, struct sockaddr *addr,
		       socklen_t addr_len)
{
	int ret;
	int ss, len_s;
	char buff_in[UDP_PACK_MAX_SIZE];
	char buff_out[UDP_PACK_MAX_SIZE];

	len_s = len;

	memcpy(buff_in, buf, len);
	
	if((ss = (len % 16)) != 0){
		memset(&buff_in[len], 0, 16);
		len_s = len + (16 - ss);
	}
	
	if(len_s > UDP_PACK_MAX_SIZE){
		printf("recvfrom len_s > UDP_PACK_MAX_SIZE %d", len_s);
		return -1;
	}
	
	aes256_data(buff_in, buff_out, len_s, 
				aes256_key, sizeof(aes256_key), AES_ENCRYPT);	
	ret = sendto(fd, buff_out, len_s, flags, addr, addr_len);
//	printf("sendto len %d ret %d\n", len_s, ret);

	return ret;
}


/***************************************
������: p2p_recvfrom
����: ��TCP/IP Э��ջ�﷢�� P2P ���ݣ�
		ͬʱ�������þ����Ƿ���Ҫ����
***************************************/
int p2p_recvfrom(int fd, void *buf, int len, int flags, struct sockaddr *addr, socklen_t *addr_len)
{
	int ret;
	if(aes256_flg)
	{
		ret = aes256_recvfrom(fd, buf, len, flags, addr, addr_len);
	}
	else
	{
		ret = recvfrom(fd, buf, len, flags, addr, addr_len);
	}
	if(ret == -1){
		perror("sendto ");
	}
	return ret;
}

/***************************************
������: p2p_sendto
����: ��TCP/IP Э��ջ�﷢�� P2P ���ݣ�
		ͬʱ�������þ����Ƿ���Ҫ����
***************************************/
int p2p_sendto(int fd, void *buf, int len,
		       int flags, struct sockaddr *addr,
		       socklen_t addr_len)
{
	int ret;
	
	if(aes256_flg)
	{
		ret = aes256_sendto(fd, buf, len, flags, addr, addr_len);
	}
	else
	{
		ret = sendto(fd, buf, len, flags, addr, addr_len);
	}
	if(ret == -1){
		perror("sendto ");
	}
	return ret;
}


/***************************************
������: p2p_sendto
����: ����������socket ��ǩ��
***************************************/
#if 0
void create_server_socket(struct sockaddr_in *servaddr)
{
	memset(servaddr, 0, sizeof(struct sockaddr_in));
	
	servaddr->sin_family = AF_INET;
    servaddr->sin_addr.s_addr = inet_addr(sys_cfg.serverip);
    servaddr->sin_port = htons(sys_cfg.ser_port);

}
#endif


