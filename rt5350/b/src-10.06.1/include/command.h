/***********************************************************
�ļ���	:	command.h
����		:	Faker
�汾��	:	1.0
����		:	2015.05.15

˵��:	
	�����еĺ��Ĵ���

***********************************************************/

#ifndef __COMMAND_H_
#define __COMMAND_H_

#include "all.h"

/* �����нṹ�� */
struct command_t{
	char *name;
	void (*com_fun)(void);
	char tag_num;
	char *tag_p;
	char *help;
//	struct list_head list;
};

struct command_t *gt_comman;


void command_format(char *str);


#endif


