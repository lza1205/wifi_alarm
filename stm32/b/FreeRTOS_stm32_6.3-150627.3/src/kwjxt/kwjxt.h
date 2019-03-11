#ifndef __KWJXT_H_
#define __KWJXT_H_

#ifdef __KWJXT_C_
#define __KWJXT_EXT_
#else
#define __KWJXT_EXT_ extern
#endif


/*
���ļ�ϵͳ�ṹ

					��ʼ			ռ��������
ϵͳ����		4096 * 1		1
�ֿ�				4096 * 2		20
����				4096 * (22)		146
����ͼƬ		4096 * (174)		92
										�ϼ�284

 ��һ������ 						
 											��ʼ300
����				4096 * 300		3		1228800
����				4096 * 303		214		1236992
ͼƬ				4096 * 517		200		2117632

											�ϼ� 417

����				4096 * 717		2		1228800
����				4096 * 719		215		1236992
ͼƬ				4096 * 934		200		2117632

����				4096 * 1134		2		1228800
����				4096 * 1136		215		1236992
ͼƬ				4096 * 1351		200		2117632

����				4096 * 1551		2		1228800
����				4096 * 1553		215		1236992
ͼƬ				4096 * 1768		200		2117632

*/

#define SYSTEM_AUDIO_ADDR			22 * 4096		/* 90112 */
#define SYSTEM_IMAGE_ADDR			174*4096		/* 712704 */

#define LANG_OFFSET_ADDR			(417 * 4096)	/* 1708032 */

#define LANG_1_SMS_ADDR				(300 * 4096)	/* 1228800 */
#define LANG_1_AUDIO_ADDR				(302 * 4096)	/* 1241088 */
#define LANG_1_IMAGE_ADDR			(517 * 4096)	/* 2117632 */

/* 2936832 */
#define LANG_2_SMS_ADDR				((300 * 4096) + LANG_OFFSET_ADDR * 1)	/* 2936832 */
/* 2949120 */
#define LANG_2_AUDIO_ADDR				((303 * 4096) + LANG_OFFSET_ADDR * 1)	/* 2945024 */
/* 3825664 */
#define LANG_2_IMAGE_ADDR			((517 * 4096) + LANG_OFFSET_ADDR * 1)	/* 3825664 */

/* 4644864 */
#define LANG_3_SMS_ADDR				((300 * 4096) + LANG_OFFSET_ADDR * 2)	/*  */
/* 4657152 */
#define LANG_3_AUDIO_ADDR				((303 * 4096) + LANG_OFFSET_ADDR * 2)	/*  */
/* 5533696 */
#define LANG_3_IMAGE_ADDR			((517 * 4096) + LANG_OFFSET_ADDR * 2)	/*  */

#define LANG_4_SMS_ADDR				((300 * 4096) + LANG_OFFSET_ADDR * 3)	/*  */
#define LANG_4_AUDIO_ADDR				((303 * 4096) + LANG_OFFSET_ADDR * 3)	/*  */
#define LANG_4_IMAGE_ADDR			((517 * 4096) + LANG_OFFSET_ADDR * 3)	/*  */


/* ϵͳ�������� */
#define	SYSTEM_IMAGE_TYPE		0x0003
#define SYSTEM_AUDIO_TYPE		0x0004

/* ��һ������ */
#define LANG_1_SMS_TYPE			0x0102
#define LANG_1_IMAGE_TYPE		0x0103
#define LANG_1_AUDIO_TYPE		0x0104


/* �ڶ������� */
#define LANG_2_SMS_TYPE			0x0202
#define LANG_2_IMAGE_TYPE		0x0203
#define LANG_2_AUDIO_TYPE		0x0204

/* ���������� */
#define LANG_3_SMS_TYPE			0x0302
#define LANG_3_IMAGE_TYPE		0x0303
#define LANG_3_AUDIO_TYPE		0x0304


/* ���ĸ����� */
#define LANG_4_SMS_TYPE			0x0402
#define LANG_4_IMAGE_TYPE		0x0403
#define LANG_4_AUDIO_TYPE		0x0404


struct Kwjxt_Head{
	u16 type;
	u32 addr;
};



//__KWJXT_EXT_ u8  FlagUartE2Command;
__KWJXT_EXT_ u16 sms_Length_e2;
__KWJXT_EXT_ u8 sms_lange_ucs2;



u32 kwjxt_find_image(u16 type, u16 item);
u8 kwjxt_read(u16 type, u16 item, u8 *buf);
u32 kwjxt_find_audio(u16 type, u16 item);


#endif




