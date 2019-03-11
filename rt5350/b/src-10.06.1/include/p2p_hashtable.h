/***********************************************************
�ļ���	:	p2p_hashtable.h
����		:	Faker
�汾��	:	1.0
����		:	2015.05.13

˵��:
		�������Ĺ�ϣ���ܺ���

***********************************************************/

#ifndef __P2P_HASHTABLE_H_
#define __P2P_HASHTABLE_H_


#include "all.h"
#include "server.h"



/* �ͻ�����Ϣ */
struct __client_info{
	char name[USER_NAME_LEN];
	unsigned int passwd;
	char version[USER_NAME_LEN];			//�ͻ��˵İ汾��
    char ip[IP_STRING_MAX];
    unsigned int port;
	char lan_ip[IP_STRING_MAX];		//������IP
	int lan_port;	//�������˿�
	
	int login_cnt;			/* �����ж��û��Ƿ��Ծ */
};

/* ��ϣ�� */
struct key
{
    char name[USER_NAME_LEN];
};

/* ��ϣ�� */
struct value
{
	struct __client_info cli_info;
};

unsigned int hashfromkey(void *ky);

int equalkeys(void *k1, void *k2);

struct value *__search_some(struct hashtable *h_table, struct key *k);
struct value *__remove_some(struct hashtable *h_table, struct key *k);


struct value *insert_hashtable_value(char *name);

int alloc_cli_ipport(struct value *val);


struct value *fine_value(char *name);




#endif



