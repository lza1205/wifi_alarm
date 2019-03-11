
#ifndef __EV1527_H_
#define __EV1527_H_

#ifdef __EV1527_C_
#define __EV1527_EXT_
#else
#define __EV1527_EXT_ extern
#endif

#include "all.h"

void ev1527_init(void);
void send_rf_data(void);


#endif

