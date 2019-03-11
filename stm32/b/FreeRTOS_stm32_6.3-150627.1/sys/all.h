
#ifndef __ALL_H_
#define __ALL_H_


/* stm32 库 */
#include "stm32f10x_conf.h"


/* 数据结构定义 */
#include "datatype.h"


/* OS 相关 */
#define __OS__

#define _USE_MSG_LIST_		1


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


/* 消息队列 */
#include "msg_list.h"
#include "sys_msg.h"


/* 板级相关 */
#include "bsp.h"

/* 驱动 */
#include "sim900a.h"

/* 配置文件 */
#include "app_config.h"

/* 功能相关 */
#include "mystring.h"
#include "printp.h"

/* 系统任务 */
#include "main.h"
#include "input_task.h"
#include "back_task.h"

/* 头文件 */
#include "ImageDefine.h"

/* 应用层 */
#include "kwjxt.h"
#include "display.h"

/* GUI */
#include "gui.h"
#include "display.h"
#include "key_handle.h"


#endif



