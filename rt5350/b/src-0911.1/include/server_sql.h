/***********************************************************
�ļ���	:	server_sql.h
����		:	Faker
�汾��	:	1.0
����		:	2015.05.13

˵��:
		server �����ݿ�ӿ�

***********************************************************/

#ifndef _SERVICE_SQL_H_
#define _SERVICE_SQL_H_


int login_passwd(char *name, char *passwd);
int service_sql_init(void);


int server_sql_update_string(char *name, char *key, char *val);
int server_sql_update_int(char *name, char *key, unsigned int val);



int sql_table_update_string(char *table, char *name_key, char *name_val, char *key, char *val);
int sql_table_update_int(char *table, char *name_key, char *name_val, char *key, int val);
int sql_table_update_all(char *table, char *key, char *val);


int sql_table_select_string(char *table, char *key, char *val);
int sql_table_insert(char *table, char *key, char *val);


#endif


