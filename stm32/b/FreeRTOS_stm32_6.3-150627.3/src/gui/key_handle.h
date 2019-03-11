
#ifndef __KEY_HANDLE_H_
#define __KEY_HANDLE_H_

#ifdef __KEY_HANDLE_C_
#define __KEY_HANDLE_EXT_
#else
#define __KEY_HANDLE_EXT extern
#endif

#include "all.h"


/* 按键值对应表 */
#define KEY_VAL_BACK		1
#define KEY_VAL_UP			2
#define KEY_VAL_MENU		3
#define KEY_VAL_DOWN		4
#define KEY_VAL_OK			5
#define KEY_VAL_LOGO		6



void key_handle_menu(u16 key_val);
void key_handle_idle(u16 key_val);



#endif


