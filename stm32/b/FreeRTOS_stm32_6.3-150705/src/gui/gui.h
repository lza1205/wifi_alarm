
#ifndef __TOUCH_H_
#define __TOUCH_H_

#ifdef __TOUCH_C_
#define __GUI_EXT_
#else
#define __GUI_EXT_ extern
#endif


#include "display.h"
#include "key_handle.h"

/* �����ID ��� */
#define INFA_ID_IDLE		1
#define INFA_ID_MENU		2


struct gui_inteface{
	u8 id;				/* ID ��� */
	void (*display)(void);	/* ��ʾ���� */
	void (*key_handle)(u16 key_val);	/* ���������� */
};



void gui_refresh(u16 key_val);
void gui_change(u8 id);



#endif

