
/***********************************************************
�ļ���	:	server_sql.c
����		:	Faker
�汾��	:	1.0
����		:	2015.05.13

˵��:
		server �����ݿ�ӿ�

***********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "mysql.h"  //�ҵĻ����ϸ��ļ���/usr/include/mysql��


#include "all.h"
#include "server.h"


/* ���ݿ������ */
static pthread_mutex_t mysql_lock = PTHREAD_MUTEX_INITIALIZER;


#define SELECT_QUERY "select * from mytable where name like '%s'"
//#define ALL_QUERY "select * from mytable;"

MYSQL mysql, *sock; //�����������Ӿ��


/*******************************************
������:	service_sql_init
����:		���������ݿ��ʼ��
*********************************************/

int service_sql_init(void)
{
	mysql_init(&mysql);
	/* ���������ݿ� */
//	if(!(sock = mysql_real_connect(&mysql, "localhost", "root", "my-name-is?lza1205", "rt5350_user_list", 0, NULL, 0))){
	if(!(sock = mysql_real_connect(&mysql, "localhost", "root", "123456", "p2p_user_list", 0, NULL, 0))){
		fprintf(stderr, "Couldn't connect to engine!\n%s\n\n", mysql_error(&mysql));
		perror("");
		exit(1);
	}

	sql_table_update_all("device", "is_online", "offline");
	sql_table_update_all("network", "is_online", "offline");
	sql_table_update_all("system", "is_online", "offline");
	sql_table_update_all("other", "is_online", "offline");

	app_printf("mysql init is ok \r\n");
	return 0;
}

#if 0
void re_connect_mysql(void)
{
	  printf("mysql_close 1 \r\n");
	  mysql_close(&mysql);
	  printf("mysql_close 2 \r\n");
	
	 mysql_init(&mysql);
	
	 printf("mysql_close 3 \r\n");
	 if(!(sock = mysql_real_connect(&mysql, "localhost", "root", "123456", "p2p_user_list", 0, NULL, 0)))
	 {
		   fprintf(stderr,"Can't connect to mysql sever! ErrorCode:%s/r/n",mysql_error(&mysql));
	
	}
	 printf("ConnectDB/r/n");

}
#endif
/* �������ݿ� */
void re_connect_mysql(void)
{
#if 0
	printf("mysql_close 111 \r\n");
	if(mysql_ping(sock) != 0){	 //reconnect failed...Then try connect

		printf("mysql_close 1 \r\n");
		mysql_close(&mysql);
		printf("mysql_close 2 \r\n");

	   mysql_init(&mysql);

	   printf("mysql_close 3 \r\n");
	   if(!(sock = mysql_real_connect(&mysql, "localhost", "root", "123456", "p2p_user_list", 0, NULL, 0)))
	   {
			 fprintf(stderr,"Can't connect to mysql sever! ErrorCode:%s/r/n",mysql_error(&mysql));

	  }
	   printf("ConnectDB/r/n");
	 }
	printf("mysql_close 222 \r\n");
#endif
}





/*******************************************
������:	service_sql_select
����:		����name �ӷ��������ҳ�passwd
*********************************************/

int service_sql_select(char *name, char *passwd)
{
	MYSQL_RES	*res;	//��ѯ�����
//	MYSQL_FIELD	*fd;	//�����ֶ���Ϣ
	MYSQL_ROW	row;	//���һ�в�ѯ������ַ�������
	char qbuf[160];		//��Ų�ѯ sql ����ַ���

	sprintf(qbuf, SELECT_QUERY, name);
	printf("qbuf is %s\n", qbuf);
	/* ��ѯ���ݿ� */
	if(mysql_query(sock, qbuf)){
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		return -1;
	}

	if(!(res = mysql_store_result(sock))){
		fprintf(stderr, "Couldn't get result from %s\n", mysql_error(sock));
		return -1;
	}

	printf("number of fields returned : %d\n", mysql_num_fields(res));
	while((row = mysql_fetch_row(res))){
		printf("Ther name #%s 's passwd is: %s\n", (((row[0]==NULL)&&(!strlen(row[0]))) ? "NULL" : row[0]),
													(((row[1]==NULL)&&(!strlen(row[1]))) ? "NULL" : row[1])); 
        puts( "query ok !\n" ) ; 
		strcpy(passwd, (((row[1]==NULL)&&(!strlen(row[1]))) ? "NULL" : row[1]));
		mysql_free_result(res);
		return 0;
	}

	mysql_free_result(res);
	return -1;
//    mysql_close(sock);
}


/* �ͷŽ���� */
void __mysql_free_result(void)
{
	MYSQL_RES	*res;	//��ѯ�����
	do
	{
	  if (!(res= mysql_store_result(sock)))
	  {
			return;
	  }
	  mysql_free_result(res);
	
	} while (!mysql_next_result(sock));

}


#define UPDATE_STRING_QUERY "update mytable set %s=\'%s\' where name=\'%s\'"
#define UPDATE_INT_QUERY "update mytable set %s=%d where name=\'%s\'"

/* ���������޸����ݿ��е�ĳ�� */
int server_sql_update_string(char *name, char *key, char *val)
{
	char qbuf[160];		//��Ų�ѯ sql ����ַ���

	sprintf(qbuf, UPDATE_STRING_QUERY, key, val, name);
	printf("qbuf is %s\n", qbuf);
	/* ��ѯ���ݿ� */
	if(mysql_query(sock, qbuf)){
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		re_connect_mysql();
		return -1;
	}
	return 0;
}

int server_sql_update_int(char *name, char *key, unsigned int val)
{
	char qbuf[160];		//��Ų�ѯ sql ����ַ���

	sprintf(qbuf, UPDATE_INT_QUERY, key, val, name);
	printf("qbuf is %s\n", qbuf);
	/* ��ѯ���ݿ� */
	if(mysql_query(sock, qbuf)){
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		re_connect_mysql();
		return -1;
	}
	return 0;
}











/*****************************************************************************/

/*
	����ĺ�������ָ��ĳ�����
*/



#define UPDATE_TABLE_STRING_QUERY "update %s set %s=\'%s\' where %s=\'%s\'"
#define UPDATE_TABLE_INT_QUERY "update %s set %s=%d where %s='%s'"

#define UPDATE_TABLE_ALL_QUERY "update %s set %s=\'%s\'"


/*
	table :����
	name: ����
	key:	�޸ĵ���
	val:	�޸�ֵ
*/
/* ���������޸����ݿ��е�ĳ�� */
int sql_table_update_string(char *table, char *name_key, char *name_val, char *key, char *val)
{
	char qbuf[160];		//��Ų�ѯ sql ����ַ���
	
	pthread_mutex_lock(&mysql_lock);

	re_connect_mysql();

	sprintf(qbuf, UPDATE_TABLE_STRING_QUERY, table, key, val, name_key, name_val);
	
	/* ��ѯ���ݿ� */
	if(mysql_query(sock, qbuf)){
		printf("qbuf is %s\n", qbuf);
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		re_connect_mysql();

		pthread_mutex_unlock(&mysql_lock);
		return -1;
	}

	pthread_mutex_unlock(&mysql_lock);
	return 0;
}

int sql_table_update_int(char *table, char *name_key, char *name_val, char *key, int val)
{
	char qbuf[160];		//��Ų�ѯ sql ����ַ���

	pthread_mutex_lock(&mysql_lock);
	re_connect_mysql();

	sprintf(qbuf, UPDATE_TABLE_INT_QUERY, table, key, val, name_key, name_val);
	
	/* ��ѯ���ݿ� */
	if(mysql_query(sock, qbuf)){
		printf("qbuf is %s\n", qbuf);
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		re_connect_mysql();

		pthread_mutex_unlock(&mysql_lock);
		return -1;
	}
	pthread_mutex_unlock(&mysql_lock);
	return 0;
}


/* �޸������е�ĳ�� */
int sql_table_update_all(char *table, char *key, char *val)
{
	char qbuf[160];		//��Ų�ѯ sql ����ַ���

	pthread_mutex_lock(&mysql_lock);
	re_connect_mysql();

	sprintf(qbuf, UPDATE_TABLE_ALL_QUERY, table, key, val);
	
	/* ��ѯ���ݿ� */
	if(mysql_query(sock, qbuf)){
		printf("qbuf is %s\n", qbuf);
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		re_connect_mysql();
		pthread_mutex_unlock(&mysql_lock);
		return -1;
	}
	pthread_mutex_unlock(&mysql_lock);
	return 0;
}


/* ��ѯ���ݿ����Ƿ������Ԫ�� */
#define SELECT_STRING_QUERY	"select * from %s where %s like \'%s\'"
int sql_table_select_string(char *table, char *key, char *val)
{
	char qbuf[160];		//��Ų�ѯ sql ����ַ���
	MYSQL_RES	*res;	//��ѯ�����
//	MYSQL_FIELD *fd;	//�����ֶ���Ϣ
	MYSQL_ROW	row;	//���һ�в�ѯ������ַ�������

	pthread_mutex_lock(&mysql_lock);
	
	re_connect_mysql();

	sprintf(qbuf, SELECT_STRING_QUERY, table, key, val);
	//printf("qbuf is %s\n", qbuf);
	/* ��ѯ���ݿ� */
	if(mysql_query(sock, qbuf)){
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		re_connect_mysql();

		pthread_mutex_unlock(&mysql_lock);
		return -1;
	}
	
	if(!(res = mysql_store_result(sock))){
		fprintf(stderr, "Couldn't get result from %s\n", mysql_error(sock));
		re_connect_mysql();

		pthread_mutex_unlock(&mysql_lock);
		return -1;
	}

	//printf("number of fields returned : %d\n", mysql_num_fields(res));
	while((row = mysql_fetch_row(res))){

		mysql_free_result(res);

		pthread_mutex_unlock(&mysql_lock);
		return 0;
	}

	mysql_free_result(res);

	pthread_mutex_unlock(&mysql_lock);
	return -1;
}


/* �����ݿ��в���Ԫ�� */
#define INSERT_STRING_QUERY	"insert into %s (%s) values (\'%s\')"
int sql_table_insert(char *table, char *key, char *val)
{
	char qbuf[160];		//��Ų�ѯ sql ����ַ���

	pthread_mutex_lock(&mysql_lock);
	re_connect_mysql();

	sprintf(qbuf, INSERT_STRING_QUERY, table, key, val);
	
	/* ��ѯ���ݿ� */
	if(mysql_query(sock, qbuf)){
		printf("qbuf is %s\n", qbuf);
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		re_connect_mysql();

		pthread_mutex_unlock(&mysql_lock);
		return -1;
	}
	pthread_mutex_unlock(&mysql_lock);
	return 0;
}

#define SQL_SELECT_QUERY "select %s from %s where name like '%s'"

/* ���������ҵ�ĳ��ֵ�ҵ�ĳ��ֵ */
int sql_table_select(char *table, char *name, char *key, char *my_buf)
{
	MYSQL_RES	*res;	//��ѯ�����
//	MYSQL_FIELD	*fd;	//�����ֶ���Ϣ
	MYSQL_ROW	row;	//���һ�в�ѯ������ַ�������
	char qbuf[160];		//��Ų�ѯ sql ����ַ���

	pthread_mutex_lock(&mysql_lock);
	
	re_connect_mysql();

	sprintf(qbuf, SQL_SELECT_QUERY, key, table, name);
//	printf("qbuf is %s\n", qbuf);
	/* ��ѯ���ݿ� */
	if(mysql_query(sock, qbuf)){
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		re_connect_mysql();

		pthread_mutex_unlock(&mysql_lock);
		return -1;
	}

	if(!(res = mysql_store_result(sock))){
		fprintf(stderr, "Couldn't get result from %s\n", mysql_error(sock));
		re_connect_mysql();
		pthread_mutex_unlock(&mysql_lock);
		return -1;
	}

//	printf("number of fields returned : %d\n", mysql_num_fields(res));
	while((row = mysql_fetch_row(res))){		
		int n;
		strcpy(my_buf, (((row[0]==NULL)&&(!strlen(row[0]))) ? "NULL" : row[0]));
		/* ������и��ո�?????? */
		n = strlen(my_buf);
//		my_buf[n - 2] = 0;
//		strcpy(val, (((row[1]==NULL)&&(!strlen(row[1]))) ? "NULL" : row[1]));
		mysql_free_result(res);
//		 puts( "query ok !\n" ) ; 

		 pthread_mutex_unlock(&mysql_lock);
		return 0;
	}

	mysql_free_result(res);

	pthread_mutex_unlock(&mysql_lock);
	return -1;
//    mysql_close(sock);
}


#define _SQL_TABLE_SUB_	"update %s set %s=(%s-%d) where %s>0"
/* ��ĳһ�м�ȥĳ������ */
int sql_table_sub(char *table, char *key, int val)
{
	char qbuf[160];		//��Ų�ѯ sql ����ַ���

	pthread_mutex_lock(&mysql_lock);
	re_connect_mysql();

	sprintf(qbuf, _SQL_TABLE_SUB_, table, key, key, val, key);
	
	/* ��ѯ���ݿ� */
	if(mysql_query(sock, qbuf)){
//		printf("qbuf is %s\n", qbuf);
		fprintf(stderr, "Query failed (%s)\n", mysql_error(sock));
		re_connect_mysql();

		pthread_mutex_unlock(&mysql_lock);
		return -1;
	}

	pthread_mutex_unlock(&mysql_lock);
	return 0;
}





void __mysql_debug(void)
{
	int ret;
	char tmp_buf[100];

	
	sql_table_select("device", "YTVY-00:16:3E:00:19:B4", "version", tmp_buf);
	printf("[sql_table_select is %s]\r\n", tmp_buf);

	server_sql_update_string("lza3", "passwd", "123456");

	server_sql_update_string("lza1", "passwd", "654321");

#if 1
	ret = sql_table_select_string("device", "code", "qwer");
	if(ret != 0)
		printf("sql_table_select_string err \r\n");
#endif

	ret = sql_table_insert("device", "code", "faker");
	if(ret != 0)
		printf("sql_table_insert err \r\n");

	ret = sql_table_select_string("device", "code", "faker");
	if(ret != 0)
		printf("sql_table_select_string err \r\n");

	sql_table_update_string("device", "name", "faker", "mac", "aabbccdd");
	if(ret != 0)
		printf("sql_table_update_string err \r\n");

	ret = sql_table_select_string("device", "code", "faker");
	if(ret != 0)
		printf("sql_table_select_string err \r\n");


}


