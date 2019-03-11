

#ifndef __SOUND_PROTO_H__
#define __SOUND_PROTO_H__


/*---------------------------------------------------------*/
/* Э�� */
#define _sound_talk_request_		0x0		/* ������*/
#define _sound_talk_ack_			0x1		/* ���� */
#define _sound_talk_noack_			0x2		/* �ܾ�ͨ�� */
#define _sound_talk_stop_			0x3		/* ֹͣͨ�� */
#define _sound_talk_data_			0x4	/* ���ݰ� */


#define _sound_file_request_		0x100	/* �ļ����� */
#define _sound_file_ack_			0x101		/* ���� */
#define _sound_file_noack_			0x102		/* �ܾ�ͨ�� */
#define _sound_file_end_			0x103	/* �ļ����� */
#define _sound_file_data_			0x104	/* �ļ����� */




typedef struct sound_head{
	unsigned int type;		/* ���ݰ����� */			
	unsigned int num;		/* ����� */
}sound_head_t;



#endif

