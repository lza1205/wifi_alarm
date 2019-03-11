

#ifndef __DISPLAY_H_
#define __DISPLAY_H_

#ifdef __DISPLAY_C_
#define __DISPLAY_EXT_
#else
#define __DISPLAY_EXT_ extern
#endif

#include "all.h"

__DISPLAY_EXT_ u8 flag_daiji_main;
__DISPLAY_EXT_ u8 SysWorkModel;
__DISPLAY_EXT_ u8 FlagICO_TimerAway;
__DISPLAY_EXT_ u8 FlagICO_Alarm;

void display_idle(void);
void display_menu(void);


#endif


