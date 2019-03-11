
/***********************************************************
�ļ���	:	aes256_data.c
����		:	Faker
�汾��	:	1.0
����		:	2015.05.15

˵��:	
	���ļ���ΪAES256 ���ܡ�key Ϊ aes256_key
	����� aes256_key_init ���г�ʼ��������
	aes256_data_enc		����
	aes256_data_dec		����

***********************************************************/

#ifndef _AES256_DATA_H_
#define _AES256_DATA_H_

#ifdef _AES256_DATA_C_
#define _AES256_DATA_EXT_
#else
#define _AES256_DATA_EXT_ extern
#endif


/* ��ʼ��aes256 key */
void aes256_key_init(void);

/* �������ݡ����ݳ��ȱ�����16 �ı��� */
void aes256_data(char *in, char *out, int len, char *rkey, 
	int rkey_len, int enc);

/* �ܳ� */
_AES256_DATA_EXT_ char aes256_key[32];






#endif



