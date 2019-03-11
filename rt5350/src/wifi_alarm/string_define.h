
#ifndef __STRING_DEFINE_H__
#define __STRING_DEFINE_H__


#define _STRING_Command		"Command"

/*
Command 的可选参数
*/
#define _STRING_Root_Login	"Root_Login"
#define _STRING_Add_Door	"Add_Door"
#define _STRING_Del_Door	"Del_Door"
#define _STRING_Add_Remote	"Add_Remote"
#define _STRING_Del_Remote	"Del_Remote"
#define _STRING_Add_Infrared	"Add_Infrared"
#define _STRING_Del_Infrared	"Del_Infrared"
#define _STRING_Add_Friend	"Add_Friend"
#define _STRING_Del_Friend	"Del_Friend"
#define _STRING_Set			"Set"
#define _STRING_Get			"Get"
#define _STRING_Scan		"Scan"		/* 扫描 */


#define _STRING_Type		"Type"

/* 通用 */
#define _STRING_Num		"Num"	/* 数量 */
#define _STRING_Name		"Name"		/* 名称 */
#define _STRING_ID			"ID"		/* ID */


/* 管理员相关 */
#define _STRING_Root		"Root"		/* 管理员 */
#define _STRING_Phone		"Phone"		/* 收集号码 */
#define _STRING_Passwd		"Passwd"	/* 管理员密码 */


/* 紧急联系人 */
#define _STRING_Friends	"Friends"	/* 紧急联系人 */


/* 接警中心 */
#define _STRING_Alarm_Center	"Alarm_Center"	/* 接警中心 */


/* 系统状态 */
#define _STRING_System_State	"System_State"	/* 系统状态 */
#define _STRING_State_Idle		"Idle"			/* 撤防状态 */
#define _STRING_State_Arm			"Arm"			/* 布防状态 */
#define _STRING_State_Home_Arm		"Home_Arm"		/* 在家布防 */
#define _STRING_State_Urgent			"Urgent"		/* 紧急状态 */
#define _STRING_State_Threaten		"Threaten"		/* 被挟持状态 */


/* 定时策略 */
#define _STRING_Timing			"Timing"		/* 定时策略 */
#define _STRING_Time			"Time"			/* 时间 */
#define _STRING_Cycle			"Cycle"			/* 周期 一次每天或者某个星期 */
#define _STRING_Cycle_One		0xff			/* 仅此一次 */
#define _STRING_Cycle_W1		(unsigned char)(1<<0)	/* 周一 */
#define _STRING_Cycle_W2		(unsigned char)(1<<1)	/* 周二 */
#define _STRING_Cycle_W3		(unsigned char)(1<<2)	/* 周三 */
#define _STRING_Cycle_W4		(unsigned char)(1<<3)	/* 周四 */
#define _STRING_Cycle_W5		(unsigned char)(1<<4)	/* 周五 */
#define _STRING_Cycle_W6		(unsigned char)(1<<5)	/* 周六 */
#define _STRING_Cycle_W7		(unsigned char)(1<<6)	/* 周日 */




#define _STRING_Msg_RF			"Msg_RF"

/* 杂项 */
#define _STRING_Ringing_Time	"Ringing_Time"		/* 喇叭鸣响时间 */

/* 无线门磁 */
#define _STRING_Doorsensor	"Doorsensor"	/* 门磁 */


/* 无线红外 */
#define _STRING_Infrared	"Infrared"		/* 红外 */

/* 遥控器 */
#define _STRING_Remote		"Remote"


#endif

