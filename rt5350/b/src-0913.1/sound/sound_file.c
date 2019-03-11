
/*******************************************************
��Ƶ�ļ�����
*******************************************************/
#include "all.h"
#include "client.h"


void sound_file(listen_t *listen)
{	
	char buf[4096];
	sound_head_t *s_head;

	int ret;

	printf("fopen \r\n");

	FILE *fp = fopen("sound_file", "w+");
	if(fp == NULL)
	{
		perror("fopen ");
		return ;
	}
	
	printf("fopen \r\n");
	while(1)
	{
		ret = recv_sound_file(listen, buf, sizeof(buf));
		if(ret == -1)
		{
			printf("%p recv is err \r\n", listen);
			return ;		//ʱ�䵽���˳�
		}

		s_head = (sound_head_t *)(buf + sizeof(struct check_head) + sizeof(struct proto_c_send_data));

		/* �Է��Ƿ�׼������ */
		switch(s_head->type)
		{
			case _sound_file_data_:
			{
				fwrite(buf + sizeof(struct check_head) + sizeof(struct proto_c_send_data) + sizeof(sound_head_t), 
						ret - (sizeof(struct check_head) + sizeof(struct proto_c_send_data) + sizeof(sound_head_t)), 1, fp);
				break;
			}
			case _sound_file_end_:
			{
				printf("recv new sound file ! \r\n");
				fclose(fp);
				return ;
			}
			printf("??????????????????? \r\n");
		}
	}
}


/* ����һ���߳���������Ƶ�ļ� */
void *sound_file_phread(listen_t *listen)
{
	int ret;
	char buf[1204];
	struct sockaddr_in clientaddr;

	ret = 0;
	/* ���ûص����� */
	if(client_inface.listen_audio != NULL)
		client_inface.listen_audio(listen);

	printf("send_sound_file_ack \r\n");
	/* �ظ��Է��ܾ�ͨ�� */
	if(ret != 0)
	{
		send_sound_file_ack(listen->name, 1);
		listen_close(listen);
		return 0;
	}

	/* �ظ��Է������Կ�ʼͨ���� */
	send_sound_file_ack(listen->name, 0);

	printf("sound_file \r\n");

	/* ��ʽ������Ƶ�ļ� */
	sound_file(listen);

	listen_close(listen);
	return 0;
}



/*---------------------------------------------------------*/
/*
����������Ƶ�ļ���������
*/


int sound_file_end_flg;

extern int frames_size;
/*
��lasa �л�ȡ��Ƶ���ݲ����ͳ�ȥ
*/
void sound_file_send(listen_t *listen)
{
	//char buff[3760];
//	char buff[340];
	char *buff;


	/* ¼����ʼ�� */
	alsa_capture_init();

	buff = malloc(frames_size);
	if(buff == NULL)
		goto out1 ;
	
	sound_file_end_flg = 0;

	printf("You can speak \r\n");
	while(1)
	{
		if(alsa_capture(buff) > 0)
			send_sound_file(listen->name, buff, frames_size);

		/* ���ͽ��� */
		if(sound_file_end_flg == 1)
		{
			send_sound_file_end(listen->name);
			printf("send_sound_file_end \r\n");
			break;
		}
	}

	free(buff);
out1:
	alsa_capture_close();
}

/*
����һ���߳�������˴�ͨ��
*/
void *sound_file_to_phread(void *pdata)
{
	int ret;
	char buf[1204];
	struct sockaddr_in clientaddr;
	listen_t *listen;
	listen = (struct listen *)pdata;

	/* �������listen  */
	listen_start(listen);

	printf("sound_file_request \r\n");

	/* ���ͻỰ���� */
	sound_file_request(listen->name);

	/* �ȴ��Է�ȷ�� */
	if(recv_sound_file_ack(listen) != 0)
	{
		printf("recv_sound_file_noack \r\n");
		goto out1;
	}

	printf("sound_file_send \r\n");
	
	/* ��ʽ������Ƶ�ļ� */
	sound_file_send(listen);

out1:
	listen_close(listen);
	return 0;
}



void sound_file_to(char *name)
{
	listen_t *listen;
	listen = create_listen(name, _proto_c_audio_);
	if(listen == NULL)
		printf("NULL !\r\n");
	listen_pthread(listen, sound_file_to_phread);
}




/*---------------------------------------------------------*/
/*
			������Ƶ�ļ�
*/

void play_sound_file(char *fname)
{
//	char buf[3760];
//	char buf[340];
	char *buf;	
	int ret;
	
	FILE *fp = fopen(fname, "r+");

	if(fp == NULL){
		perror("fopen ");
		return ;
	}
	
	alsa_playback_init();
	
	buf = malloc(frames_size);
	if(buf == NULL)
		goto out1 ;

	printf("frames_size %d \r\n", frames_size);

	while((ret = fread(buf, frames_size, 1, fp)) > 0)
	{
		alsa_playback(buf);
	}
	
	free(buf);
	
out1:
	alsa_playback_close();

	fclose(fp);
}

