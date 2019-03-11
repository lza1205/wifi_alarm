
#define __GUI_C_

#include "all.h"


struct gui_inteface gui_infa[] = {

	/* 空闲界面 */
	{
		INFA_ID_IDLE,
		display_idle,
		key_handle_idle,
	},
	{
		INFA_ID_MENU,
		display_menu,
		key_handle_menu,
	},


	/* 无效项 */
	{
		0xff,
		NULL,
		NULL,
	},
};


struct gui_inteface *cur_infa = &gui_infa[0];


/* 界面刷新 */
void gui_refresh(u16 key_val)
{
	cur_infa->key_handle(key_val);	//对传入的键值做处理
}

/* 界面切换 */
void gui_change(u8 id)
{
	u8 i;
	for(i = 0; gui_infa[i].id != 0xff; i++)
	{
		if(gui_infa[i].id == id)
		{
			cur_infa = &gui_infa[i];
			cur_infa->display();
		}
	}
}


