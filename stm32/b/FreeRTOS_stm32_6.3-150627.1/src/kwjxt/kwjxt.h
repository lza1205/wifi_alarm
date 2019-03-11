#ifndef __KWJXT_H_
#define __KWJXT_H_

#ifdef __KWJXT_C_
#define __KWJXT_EXT_
#else
#define __KWJXT_EXT_ extern
#endif


/*
块文件系统结构

					起始			占用扇区数
系统配置		4096 * 1		1
字库				4096 * 2		20
铃声				4096 * (22)		146
公共图片		4096 * (174)		92
										合计284

 第一个语言 						
 											起始300
短信				4096 * 300		3		1228800
语言				4096 * 303		214		1236992
图片				4096 * 517		200		2117632

											合计 417

短信				4096 * 717		2		1228800
语言				4096 * 719		215		1236992
图片				4096 * 934		200		2117632

短信				4096 * 1134		2		1228800
语言				4096 * 1136		215		1236992
图片				4096 * 1351		200		2117632

短信				4096 * 1551		2		1228800
语言				4096 * 1553		215		1236992
图片				4096 * 1768		200		2117632

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


/* 系统公共部分 */
#define	SYSTEM_IMAGE_TYPE		0x0003
#define SYSTEM_AUDIO_TYPE		0x0004

/* 第一个语种 */
#define LANG_1_SMS_TYPE			0x0102
#define LANG_1_IMAGE_TYPE		0x0103
#define LANG_1_AUDIO_TYPE		0x0104


/* 第二个语种 */
#define LANG_2_SMS_TYPE			0x0202
#define LANG_2_IMAGE_TYPE		0x0203
#define LANG_2_AUDIO_TYPE		0x0204

/* 第三个语种 */
#define LANG_3_SMS_TYPE			0x0302
#define LANG_3_IMAGE_TYPE		0x0303
#define LANG_3_AUDIO_TYPE		0x0304


/* 第四个语种 */
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




