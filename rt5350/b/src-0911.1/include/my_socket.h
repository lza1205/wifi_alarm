/***********************************************************
�ļ���	:	my_socket.h
����		:	Faker
�汾��	:	1.0
����		:	2015.05.13

˵��:
	��TCP/IP Э��ջ��һ����װ

***********************************************************/

#ifndef __MY_SOCKET_H_
#define __MY_SOCKET_H_

#include "all.h"

int aes256_recvfrom(int fd, void *buf, int len, int flags, struct sockaddr *addr, socklen_t *addr_len);

int aes256_sendto(int fd, void *buf, int len,
		       int flags, struct sockaddr *addr,
		       socklen_t addr_len);

int p2p_recvfrom(int fd, void *buf, int len, int flags, struct sockaddr *addr, socklen_t *addr_len);

int p2p_sendto(int fd, void *buf, int len,
		       int flags, struct sockaddr *addr,
		       socklen_t addr_len);

void create_server_socket(struct sockaddr_in *servaddr);

#endif



