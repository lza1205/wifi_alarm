
#ifndef __BACK_TASK_H_
#define __BACK_TASK_H_

#ifdef __BACK_TASK_C_
#define __BACK_TASK_EXT_
#else
#define __BACK_TASK_EXT_ extern
#endif

#include "all.h"


__BACK_TASK_EXT_ char *  rebuff;

void back_task(void *pdata);




#endif

