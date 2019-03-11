
#ifndef __CONFIG_H_
#define __CONFIG_H_

/* ������Ĭ����ʼ�˿ں� */
#define SERVER_SYNC_PORT			8000

/* TCP �������˿ں� */
#define SERVER_PORT_TCP				8080

/* �汾�� */
#define CLIENT_VERSION		"client-15.07.27.1"	
#define SERVER_VERSION		"server-15.05.15"

/* ���Դ�ӡ��Ϣ */
#define app_printf(fmt, ...)	if(printf_app_flg)	\
									printf(fmt, ##__VA_ARGS__)

#define dbg_printf(fmt, ...)	if(printf_dbg_flg)	\
									printf("__debug :");	\
									printf(fmt, ##__VA_ARGS__)

#define sync_printf(fmt, ...)	if(printf_sync_flg)			\
									printf(fmt, ##__VA_ARGS__)


/* ͬ����� */
#define SYNC_TIME				30		
#define LOGIN_CNT_INIT			60		/* �û���Ծ״̬��ʼֵ */
#define LOGIN_CNT_DIF			30		/* �û���Ծֵ�ݼ�ֵ */


/* һ��UDP sendto recvfrom ������ֽ��� */
#define UDP_PACK_MAX_SIZE				(10*1024)

/* ID ������ */
#define USER_NAME_LEN	8

/* ���볤�� */
#define USER_PASSWD_LEN	10


/* IP ��󳤶� */
#define IP_STRING_MAX	20

/* �����Դ�Ŷ��ٸ����ѿͻ��� */
#define P2P_CLIENT_MAX	10




/* ����У�顢����ͷ */
/* 4 + 4 + 20 + 20 + 20 + 4 = 72 */
struct check_head{
	unsigned int affairs;	/* �������� */
//	unsigned int type;		/* ���� 0 ���� 1 APP*/
	unsigned char name[USER_NAME_LEN];
	unsigned int passwd;
//	char version[30];
//	int timecnt[6];			/* ʱ���: ������ʱ���� */
	unsigned int key;		/* ���ݰ�Ψһ��� */

	int crc;			/* ����У�� */
};

/* ������Ӧ���½��� */
struct server_ack_login{
	int ack;				/* ��¼�Ƿ�ɹ� */
	int master_sta;			/* �����Ƿ����� */
};





#define __max(a,b)        (((a) > (b)) ? (a) : (b))
#define __min(a,b)        (((a) < (b)) ? (a) : (b))


#endif

