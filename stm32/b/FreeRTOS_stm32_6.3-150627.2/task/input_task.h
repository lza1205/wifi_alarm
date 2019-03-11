
#ifndef __INPUT_TASK_H_
#define __INPUT_TASK_H_

#ifdef __INPUT_TASK_C_
#define __INPUT_TASK_EXT_
#else
#define __INPUT_TASK_EXT_ extern
#endif

#include "all.h"


__INPUT_TASK_EXT_ u8 systemid[3];
__INPUT_TASK_EXT_ u8 Flag7185ConnOK;
__INPUT_TASK_EXT_ u8 Flag_OLD_zBUS;


void input_task(void *pdata);




#endif


