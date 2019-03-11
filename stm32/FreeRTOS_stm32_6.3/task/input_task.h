
#ifndef __INPUT_TASK_H_
#define __INPUT_TASK_H_

#ifdef __INPUT_TASK_C_
#define __INPUT_TASK_EXT_
#else
#define __INPUT_TASK_EXT_ extern
#endif

#include "all.h"


/*ÕýÒª²¦ºÅµÄºÅÂë  */
__INPUT_TASK_EXT_ u8 call_phone[20];


void input_task(void *pdata);




#endif


