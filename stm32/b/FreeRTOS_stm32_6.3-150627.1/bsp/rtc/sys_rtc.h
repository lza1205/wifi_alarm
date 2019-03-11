
#ifndef __SYS_RTC_H_
#define __SYS_RTC_H_

#ifdef __SYS_RTC_C_
#define __SYS_RTC_EXT_
#else
#define __SYS_RTC_EXT_ extern
#endif

#include "all.h"


__SYS_RTC_EXT_ struct tm time_now;		// 定义UNIX时间结构体


#endif



