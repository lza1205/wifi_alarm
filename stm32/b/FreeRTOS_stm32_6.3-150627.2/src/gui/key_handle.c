
#define __KEY_HANDLE_C_

#include "all.h"


void key_handle_idle(u16 key_val)
{
	switch(key_val)
	{
		case KEY_VAL_MENU:
			gui_change(INFA_ID_MENU);
			break;
	}
}


void key_handle_menu(u16 key_val)
{
	switch(key_val)
	{
		case KEY_VAL_MENU:
			gui_change(INFA_ID_IDLE);
			break;
	}
}



