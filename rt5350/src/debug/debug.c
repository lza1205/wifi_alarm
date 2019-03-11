
/***********************************************************
�ļ���	:	debug.c
����		:	Faker
�汾��	:	1.0
����		:	2015.05.13

˵��:
	��Ҫ�ĵ��Ժ���

***********************************************************/


#define __DEBUG_C_

#include "all.h"

#include "debug.h"

/***************************************
������: set_app_printf
����: �Ƿ���app ����Ĵ�ӡ��Ϣ
		1 ����
		0 �ر�
***************************************/
void set_app_printf(int mode)
{
	printf_app_flg = mode;
}


/***************************************
������: set_dbg_printf
����: �Ƿ���dbg����Ĵ�ӡ��Ϣ
		1 ����
		0 �ر�
***************************************/
void set_dbg_printf(int mode)
{
	printf_dbg_flg = mode;
}


/***************************************
������: set_sync_printf
����: �Ƿ���sync����Ĵ�ӡ��Ϣ
		1 ����
		0 �ر�
***************************************/
void set_sync_printf(int mode)
{
	printf_sync_flg = mode;
}

/***************************************
������: set_aes256
����:  �Ƿ���AES256 ���ܷ�ʽ
		1 ����
		0 �ر�
***************************************/
void set_aes256(int mode)
{
	aes256_flg = mode;
}

/***************************************
������: printf_client
����:  ��ӡ�ͻ�����Ϣ 
		1 ����
		0 �ر�
***************************************/
void printf_client(struct sockaddr_in *cli_addr)
{

	dbg_printf("client info : ");
	dbg_printf("%s : %d \n", inet_ntoa(cli_addr->sin_addr), 
                               ntohs(cli_addr->sin_port));

}

