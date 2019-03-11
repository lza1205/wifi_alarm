
#ifndef __SYS_TIME_H_
#define __SYS_TIME_H_

#ifdef __SYS_TIME_C_
#define __SYS_TIME_EXT_
#else
#define __SYS_TIME_EXT_ extern
#endif

#include "all.h"

__SYS_TIME_EXT_ u8 Timer5ms;
__SYS_TIME_EXT_ u8 Time10ms;
__SYS_TIME_EXT_ u8 time_x100ms;



void sys_time1_init(void);



#endif


