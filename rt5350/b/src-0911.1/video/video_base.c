
/***********************************************************
video ��������
***********************************************************/

#include "all.h"
#include "client.h"


/*------------------------------------------------------------*/
/*
					ͨ����صĵײ㺯��
*/


/* ��name ��������ͨ������ */
void video_talk_request(char *name)
{
	video_head_t s_head;

	s_head.type = _video_talk_request_;

	sendto_node_reliable(name, _proto_c_audio_, (char *)&s_head, sizeof(video_head_t), 20);
}



/* ����Ӧ�� */
void send_video_talk_ack(char *name, int ack)
{
	video_head_t s_head;

	if(ack == 0)
		s_head.type = _video_talk_ack_;
	else
		s_head.type = _video_talk_noack_;
	
	
	sendto_node_reliable(name, _proto_c_audio_, (char *)&s_head, sizeof(video_head_t), 20);
}




int recv_video_talk_ack(listen_t *listen)
{
	int ret;
	char buf[512];
	struct sockaddr_in clientaddr;

	ret = recv_from_listen(listen, (struct sockaddr_in *)&clientaddr, buf, 1204, 10);

	if(ret == -1)
	{
		printf("%p recv is err \r\n", listen);
		return -1;		//ʱ�䵽���˳�
	}

	video_head_t *s_head = (video_head_t *)(buf + sizeof(struct check_head) + sizeof(struct proto_c_send_data));

	/* �Է��Ƿ�׼������ */
	if(s_head->type == _video_talk_ack_)
	{
		return 0;
	}else{
		return -1;
	}
}



int video_talk_cnt;
int send_video_talk(char *name, char *buf, int len)
{
	video_head_t s_head;
	char send_buf[len + 20];
	int send_len;

	s_head.type = _video_talk_data_;
	s_head.num = video_talk_cnt++;

	send_len = 0;
	memcpy(send_buf, &s_head, sizeof(video_head_t));
	memcpy(send_buf + sizeof(video_head_t), buf, len);

	send_len = sizeof(video_head_t) + len;

	sendto_node_unreliable(name, _proto_c_audio_, send_buf, send_len);
}


int recv_video_talk(listen_t *listen, char *recv_buf, int len)
{
	int ret;
//	char buf[512];

	struct sockaddr_in clientaddr;

	ret = recv_from_listen(listen, (struct sockaddr_in *)&clientaddr, recv_buf, len, 300);

	if(ret == -1)
	{
		printf("%p recv is err \r\n", listen);
		return -1;		//ʱ�䵽���˳�
	}

	return ret;
}


void send_video_talk_end(char *name)
{
	video_head_t s_head;

	s_head.type = _video_talk_stop_;

	sendto_node_reliable(name, _proto_c_audio_, (char *)&s_head, sizeof(video_head_t), 20);
}





/*-----------------------------------------------------------*/
/*
				��Ƶ�ļ�����ײ㺯��
*/

void video_file_request(char *name)
{
	video_head_t s_head;

	s_head.type = _video_file_request_;

	sendto_node_reliable(name, _proto_c_audio_, (char *)&s_head, sizeof(video_head_t), 20);
}

/* ����Ӧ�� */
void send_video_file_ack(char *name, int ack)
{
	video_head_t s_head;

	if(ack == 0)
		s_head.type = _video_file_ack_;
	else
		s_head.type = _video_file_noack_;
	
	
	sendto_node_reliable(name, _proto_c_audio_, (char *)&s_head, sizeof(video_head_t), 20);
}



int recv_video_file_ack(listen_t *listen)
{
	int ret;
	char buf[512];
	struct sockaddr_in clientaddr;

	ret = recv_from_listen(listen, (struct sockaddr_in *)&clientaddr, buf, 1204, 10);

	if(ret == -1)
	{
		printf("%p recv is err \r\n", listen);
		return -1;		//ʱ�䵽���˳�
	}

	video_head_t *s_head = (video_head_t *)(buf + sizeof(struct check_head) + sizeof(struct proto_c_send_data));

	/* �Է��Ƿ�׼������ */
	if(s_head->type == _video_file_ack_)
	{
		return 0;
	}else{
		return -1;
	}
}



int video_file_cnt;
int send_video_file(char *name, char *buf, int len)
{
	video_head_t s_head;
	char send_buf[len + 20];
	int send_len;

	s_head.type = _video_file_data_;
	s_head.num = video_file_cnt++;

	send_len = 0;
	memcpy(send_buf, &s_head, sizeof(video_head_t));
	memcpy(send_buf + sizeof(video_head_t), buf, len);

	send_len = sizeof(video_head_t) + len;

	sendto_node_unreliable(name, _proto_c_audio_, send_buf, send_len);
}


int recv_video_file(listen_t *listen, char *recv_buf, int len)
{
	int ret;
//	char buf[512];

	struct sockaddr_in clientaddr;

	ret = recv_from_listen(listen, (struct sockaddr_in *)&clientaddr, recv_buf, len, 300);

	if(ret == -1)
	{
		printf("%p recv is err \r\n", listen);
		return -1;		//ʱ�䵽���˳�
	}

	return ret;
}


void send_video_file_end(char *name)
{
	video_head_t s_head;

	s_head.type = _video_file_end_;

	sendto_node_reliable(name, _proto_c_audio_, (char *)&s_head, sizeof(video_head_t), 20);
}

