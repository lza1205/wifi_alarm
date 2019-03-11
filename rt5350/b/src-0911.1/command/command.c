/***********************************************************
�ļ���	:	command.c
����		:	Faker
�汾��	:	1.0
����		:	2015.05.15

˵��:	
	�����еĺ��Ĵ���

***********************************************************/




#include "all.h"

char *gp_comman_tag[10];		//�����в���
int g_comman_num;		//�����в�������


/********************************************
������: __str_cmp
����:		�Ƚ������ַ����Ƿ���ȫ���
*********************************************/

int __str_cmp(char *str1, char *str2)
{
	int i = 0;
	while(((*str2) != '\0') && ((*str1) != '\0'))
	{
		if((*str1) != (*str2))
			return 0;
		str1 ++;
		str2 ++;
		i ++;
	}
	if(((*str2) == '\0') && (((*str1) == '\0')))
	{
		return i;
	}
	return 0;
}



/********************************************
������: __str_cmp
����:		���û�������ַ������и�ʽ��
			�ֽ����
*********************************************/

void command_format(char *str)
{
	char *tmp_p;
	int i;
	int err = -1;
	
	tmp_p = str;
	i = 0;

	if((*str) == '\n')
		return;
	if((*str) == '\r')
		return;
	
	g_comman_num = 0;
	gp_comman_tag[i++] = tmp_p;
	while((*tmp_p) != '\0')
	{
		if(((*tmp_p) == ' ') || ((*tmp_p) == '\r') || ((*tmp_p) == '\n'))
		{
			if((*tmp_p) == '\r')
				tmp_p ++;
			(*tmp_p) = '\0';
			gp_comman_tag[i++] = tmp_p + 1;
			g_comman_num ++;
		}
		tmp_p ++;
	}

	
	for(i = 0; gt_comman[i].name != NULL; i++)	//һ�����ȶ�
	{
		//dbg_printf("%s %s \r\n", gp_comman_tag[0], gt_comman[i].name);
		err = strcmp(gp_comman_tag[0], gt_comman[i].name);
		if(err == 0)
		{
			break;
		}
	}
	if(err == 0)
		gt_comman[i].com_fun();
	else
		app_printf("command err \r\n");
}


