/***********************************************************
�ļ���	:	p2p_hashtable.c
����		:	Faker
�汾��	:	1.0
����		:	2015.05.13

˵��:
		�������Ĺ�ϣ���ܺ���

***********************************************************/

#include "all.h"

#include "server.h"

/***********************************************************
����: ��ϣ�����
***********************************************************/
/*****************************************************************************/
DEFINE_HASHTABLE_INSERT(insert_some, struct key, struct value);
DEFINE_HASHTABLE_SEARCH(search_some, struct key, struct value);
DEFINE_HASHTABLE_REMOVE(remove_some, struct key, struct value);
DEFINE_HASHTABLE_ITERATOR_SEARCH(search_itr_some, struct key);
/*****************************************************************************/


/***************************************************
һ����������������ֲ��ϣ����
****************************************************/

/****************************************************************/

unsigned int
hashfromkey(void *ky)
{
    struct key *k = (struct key *)ky;
#if 0
    return (((k->one_ip << 17) | (k->one_ip >> 15)) ^ k->two_ip) +
            (k->one_port * 17) + (k->two_port * 13 * 29);
#else
//	printf("hashfromkey   %d\n", atoi(k->name));
	return atoi(k->name);
#endif
}

int
equalkeys(void *k1, void *k2)
{
	struct key *kk1, *kk2;
	kk1 = (struct key *)k1;
	kk2 = (struct key *)k2;
	
    return (0 == strcmp(kk1->name,kk2->name));
}
/****************************************************************/



/***************************************
������: __search_some
����:�ӹ�ϣ���������Ԫ��
***************************************/

struct value *__search_some(struct hashtable *h_table, struct key *k)
{
	return search_some(h_table,k);
}

/***************************************
������: __search_some
����:�ӹ�ϣ������ɾ��Ԫ��
***************************************/

struct value *__remove_some(struct hashtable *h_table, struct key *k)
{
	return remove_some(h_table,k);
}




/***************************************
������: find_hashtable_value
����:����KEY �ӹ�ϣ�������ҵ�val
		���û�����val �����
**************************************/

struct value *insert_hashtable_value(char *name)
{
	struct value *currentuser;
	struct value *found;
	struct key *k;

	/* ��ϣ�� */
	k = (struct key *)malloc(sizeof(struct key));
	if(k == NULL){
//		app_printf("malloc key err\n");
		return NULL;
	}

	__strcpy(k->name, name, USER_NAME_LEN);


	/* �鿴���Ƿ��е�½�� */
	if(NULL == (found = search_some(h_table,k)))
	{
//		app_printf("frist\n");
		currentuser = (struct value *)malloc(sizeof(struct value));
		if(currentuser == NULL){
			free(k);
//			app_printf("malloc err\n");
			return NULL;
		}
		/* ���뵽��ϣ�� */
		if (!insert_some(h_table,k,currentuser)){
			free(k);
			free(currentuser);
//			app_printf("insert_some err\n");
			return NULL;
		}
	}else{
		currentuser = found;
		free(k);		//�ͷ��ڴ�
//		dbg_printf("find\n");
	}

	if(NULL == currentuser)
	{
//		app_printf("search_some err \n");
		return NULL;
	}

	return currentuser;

}


/***************************************
������: fine_value
����:����KEY �ӹ�ϣ�������ҵ�val
		���û�����val �򷵻�NULL
**************************************/

struct value *fine_value(char *name)
{
	struct key *k;
	struct value *currentuser = NULL;
	struct value *found = NULL;
	
	k = (struct key *)malloc(sizeof(struct key));
	if(k == NULL){
		app_printf("malloc key err\n");
		return NULL;
	}

	__strcpy(k->name, name, USER_NAME_LEN);	


	/* �鿴���Ƿ��е�½�� */
	if(NULL == (found = search_some(h_table, k)))
	{
		printf("no login......\n");
		return NULL;		
	}else{
		currentuser = found;
	}

	return currentuser;
}


