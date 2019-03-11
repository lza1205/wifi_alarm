
/***********************************************************
�ļ���	:	debug.h
����		:	Faker
�汾��	:	1.0
����		:	2015.05.13

˵��:
	��Ҫ�ĵ��Ժ���

***********************************************************/

#ifndef __DEBUG_H_
#define __DEBUG_H_

#ifdef __DEBUG_C_
#define __DEBUG_EXT_
#else
#define __DEBUG_EXT_ extern
#endif

#include "all.h"

__DEBUG_EXT_	int printf_app_flg;
__DEBUG_EXT_	int printf_dbg_flg;
__DEBUG_EXT_	int printf_sync_flg;

/* �Ƿ���AES256 ���� */
__DEBUG_EXT_	int aes256_flg;


void set_app_printf(int mode);
void set_dbg_printf(int mode);
void set_sync_printf(int mode);
void set_aes256(int mode);
void printf_client(struct sockaddr_in *cli_addr);


#endif


