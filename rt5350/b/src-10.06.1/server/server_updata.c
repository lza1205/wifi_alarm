
#include "all.h"
#include "server.h"

#include <wait.h>

#define TCP_BACKLOG 10


char updata_client_path[100] = "/work/agent/agent_client";	//��������ŵ�Ŀ¼

char updata_client_version[100] = CLIENT_VERSION;		//�ͻ��˰汾��

int check_update(char *recvbuf, struct sockaddr_in *cli)
{
#if 0
	int index;

	index = recvbuf->index;
	
	if(index > MAX_USER){
		printf("index > MAX %d\n", index);
		goto out;
	}
	
	if(userlist[index].username[0] == 0){
		my_printf("no name\n");
		goto out;
	}

	if(strcmp(userlist[index].username, recvbuf->username) != 0){
		my_printf("name err\n");
		goto out;
	}

	if(strcmp(userlist[index].passwd, recvbuf->password) != 0){
		my_printf("passwd err\n");
		goto out;
	}

	if(ntohl(cli->sin_addr.s_addr) != userlist[index].ip){
		my_printf("ip err\n");
		goto out;
	}

	printf("check update is ok\n");
	return 0;
out :
	printf("check update is err\n");
	return -1;
#else
	return 0;
#endif
}


int upda_handle(int sock_fd, struct sockaddr_in *cli_addr)
{
	ssize_t n, ret;
	char buf[512];
	int fd;
	char recvbuf[512];

	if((ret = recv(sock_fd, &recvbuf, sizeof(recvbuf), 0)) == -1){
		return -1;
	}

	if((ret = check_update(recvbuf, cli_addr)) == -1){
		return -1;
	}
	
	if((fd = open(updata_client_path, O_RDWR)) == -1){
		perror("open %s ");
		return -1;
	}

	while((ret = read(fd, buf, sizeof(buf))) > 0){


		n = send(sock_fd, buf, ret, 0);
/*		
		if(n < 0 && errno == EINTR)
			goto again;
		else if(n < 0)
			printf("str_echo: read err!\r\n");
		//printf("str_echo out!\r\n");
*/
	}
	
	printf("str_echo out!\r\n");
#if 0
again:
	while((n = read(sock_fd, buf, 512 - 1)) > 0){
		buf[n] = '\0';
		printf("recv data is %s\r\n", buf);
	}
	if(n < 0 && errno == EINTR)
		goto again;
	else if(n < 0)
		printf("str_echo: read err!\r\n");
	
	printf("str_echo out!\r\n");
#endif
	return 0;
}


void sig_chld(int signo)
{
	pid_t pid;
	int stat;

	while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\r\n", pid);
	return;
}



static int updata_flg = 0;		//�����ɹ���־


/* TCP ������ */
void *tcp_update_server(void *pdata)
{
	/* ��sock_fd ���м������� new_fd �����µ����� */
	int sock_fd, new_fd;
	/* �Լ��ĵ�ַ��Ϣ */
	struct sockaddr_in my_addr;
	/*	�����ߵĵ�ַ��Ϣ*/
	struct sockaddr_in their_addr;
	int sin_size;

	struct sockaddr_in *cli_addr;

	/* ������ */
	if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket is error\r\n");
		exit(1);
	}

	/* �����ֽ�˳�� */
	/* Э�� */
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(SERVER_PORT_TCP);
	/* ��ǰIP ��ַд�� */
	my_addr.sin_addr.s_addr = INADDR_ANY;

	/* ���ṹ������Ķ����� */
	bzero(&(my_addr.sin_zero), 8);

	/* bind ��*/
	if(bind(sock_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind is error\r\n");
		exit(1);
	}

	/* ��ʼ���� */
	if(listen(sock_fd, TCP_BACKLOG) == -1)
	{
		perror("listen is error\r\n");
		exit(1);
	}

	printf("start accept\n");

	/* ��Ϊ���Ǻ���ᴴ��������ӽ����������µ�����
		һ���ӽ����쳣��ֹ�ˣ���Ҫ��������������Դ����
	*/
	signal(SIGCHLD, sig_chld);			//�����ﴦ��������

	/* accept() ѭ�� */
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);

		if((new_fd = accept(sock_fd, (struct sockaddr *)&their_addr, (socklen_t *)&sin_size)) == -1)
		{
			perror("accept");
			continue;
		}

		cli_addr = malloc(sizeof(struct sockaddr));

		if(cli_addr != NULL)
			memcpy(cli_addr, &their_addr, sizeof(struct sockaddr));
		
		printf("accept addr is %s\r\n", inet_ntoa(their_addr.sin_addr));

		/*	���ｫ����һ���ӽ������͸ոս������׽��ֽ���ͨѶ */
		if(fork() == 0)
		{
			close(sock_fd);
			upda_handle(new_fd, cli_addr);
			close(new_fd);
			updata_flg = 1;
			exit(0);
		}
		close(new_fd);
	}
}




void server_updata_init(void)
{
	pthread_t thread;
    pthread_attr_t thread_attr;
	
	/* ����һ��TCP ���߳� */
	pthread_attr_init(&thread_attr);		//��ʼ����������
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
	if (pthread_create(&thread, &thread_attr, tcp_update_server, NULL) < 0)
	{
		perror("pthread_create");
	}
}



void __send_client_updata_flg(struct value *currentuser)
{
	
	struct sockaddr_in remote;
	struct check_head head;

	remote.sin_family = AF_INET;
	remote.sin_port = htons(currentuser->cli_info.port);
	remote.sin_addr.s_addr = inet_addr(currentuser->cli_info.ip);

	compages_head(&head, _aff_server_updata_);
	
    p2p_sendto(server_info.sockfd, &head, sizeof(head), 0, 
						(struct sockaddr *)&remote, sizeof(struct sockaddr));
}

void __updata_client_for_all(void)
{
	struct value *v;
    struct hashtable_itr *itr;
	char buf[512];
	int fd;
	int ret;
	
	
	/* ���ļ��ж����汾�� */
	
	if((fd = open("/work/agent/version", O_RDWR)) == -1){
		perror("open ");
		goto send_updata;
	}

	while((ret = read(fd, buf, sizeof(buf))) > 0){
		strcpy(updata_client_version, buf);
	}
	
send_updata:	
    itr = hashtable_iterator(h_table);
    if (hashtable_count(h_table) > 0)
    {
        do {
//            kk = hashtable_iterator_key(itr);
            v = hashtable_iterator_value(itr);
			if(strcmp(updata_client_version, v->cli_info.version) != 0)
			{
				updata_flg = 0;
				__send_client_updata_flg(v);
				while(updata_flg)
				{
					sleep(1);
				}
			}

        } while (hashtable_iterator_advance(itr));
    }
	free(itr);
}


