#ifndef _SYS_MSG_H_
#define _SYS_MSG_H_




#define MSG_GOURP_0			(u16)(1<<0)
#define MSG_GOURP_1			(u16)(1<<1)
#define MSG_GOURP_2			(u16)(1<<2)
#define MSG_GOURP_3			(u16)(1<<3)
#define MSG_GOURP_4			(u16)(1<<4)
#define MSG_GOURP_5			(u16)(1<<5)
#define MSG_GOURP_6			(u16)(1<<6)
#define MSG_GOURP_7			(u16)(1<<7)
#define MSG_GOURP_8			(u16)(1<<8)
#define MSG_GOURP_9			(u16)(1<<9)
#define MSG_GOURP_10		(u16)(1<<10)
#define MSG_GOURP_11		(u16)(1<<11)
#define MSG_GOURP_12		(u16)(1<<12)
#define MSG_GOURP_13		(u16)(1<<13)
#define MSG_GOURP_14		(u16)(1<<14)
#define MSG_GOURP_15		(u16)(1<<15)



#define MSG_INPUT_RFID		(u16)(1<<15)

#define MSG_INPUT_EV1527		(u16)(1<<14)
#define MSG_INPUT_UART1		(u16)(1<<13)
#define	MSG_INPUT_SMS		(u16)(1<<12)


#define MSG_TIME_LOGO		(u16)(1<<15)
#define MSG_TIME_1S			(u16)(1<<14)	/* 1s  */

#define MSG_GOURP_INPUT		MSG_GOURP_0
#define MSG_GOURP_TIME		MSG_GOURP_1





#define IS_MSG(type, val)	((type << 16) | val)





#endif



