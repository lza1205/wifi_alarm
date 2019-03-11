
#ifndef __ALL_H_
#define __ALL_H_


/* stm32 �� */
#include "stm32f10x_conf.h"


/* ���ݽṹ���� */
#include "datatype.h"


/* OS ��� */
#define __OS__

#define _USE_MSG_LIST_		1


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


/* ��Ϣ���� */
#include "msg_list.h"
#include "sys_msg.h"


/* �弶��� */
#include "bsp.h"

/* ���� */
#include "sim900a.h"

/* �����ļ� */
#include "app_config.h"

/* ������� */
#include "mystring.h"
#include "printp.h"

/* ϵͳ���� */
#include "main.h"
#include "input_task.h"
#include "back_task.h"

/* ͷ�ļ� */
#include "ImageDefine.h"

/* Ӧ�ò� */
#include "kwjxt.h"
#include "display.h"

/* GUI */
#include "gui.h"
#include "display.h"
#include "key_handle.h"


#endif



