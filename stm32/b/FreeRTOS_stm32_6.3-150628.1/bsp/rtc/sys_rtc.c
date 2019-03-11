

#define __SYS_RTC_C_
#include "all.h"

/*******************************************************************************
* 函数名 		: RTC_Configuration
* 函数描述    	: 设置RTC
* 输入参数    	: 无
* 输出结果     	: 无
* 返回值       	: 无
*******************************************************************************/
void sys_rtc_init(void)
{	
	volatile u32 timeout = 0;

	// 定义 NVIC 初始化结构体 
	NVIC_InitTypeDef NVIC_InitStructure;
	// 使能 RTC 中断 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP|RCC_APB1Periph_PWR,ENABLE); //RTC是存在后备区域中的，所以是要开启后备区域和电源时钟的
	PWR_BackupAccessCmd(ENABLE);	   //电源控制寄存器(PWR_CR)的BPB位使能对后备区域和RTC的访问
	// 使能 RTC 时钟 
	RCC_RTCCLKCmd(ENABLE);

	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)		//判断是否是第一次配置时钟
	{	
		// 打开 APB1 总线上的 PWR，BKP 时钟 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);		
		// 使能 RTC 和后备寄存器访问 
		PWR_BackupAccessCmd(ENABLE);
		// 复位备份寄存器设置 
		BKP_DeInit();
		// 开启 LSE 
		RCC_LSEConfig(RCC_LSE_ON);
		// 等待LSE起振 
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{
			timeout ++;
			if(timeout > 0x1fffff)
			{
				Delay_10ms(100);
				printp("RTC ERROR\r\n"); 
				return ;							//超时退出
			}
		}

		// 选择 LSE 为 RTC 时钟源 
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		// 使能 RTC 时钟 
		RCC_RTCCLKCmd(ENABLE);
		// 等待 RTC 寄存器同步完成 
		RTC_WaitForSynchro();
		// 等待最近一次对 RTC 寄存器的写操作完成 
		RTC_WaitForLastTask();
		// 使能RTC秒中断   
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		//闹钟中断使能
		RTC_ITConfig(RTC_IT_ALR, ENABLE); 
		// 等待最近一次对 RTC 寄存器的写操作完成 
		RTC_WaitForLastTask();
		// 设置RTC时钟分频值为32767，则计数频率 = (32.768 KHz)/(32767+1) = 1Hz（~1s）
		RTC_SetPrescaler(32767); 
		// 等待最近一次对 RTC 寄存器的写操作完成 
		RTC_WaitForLastTask();
		
		// 设定初始时间 				
		RTC_SetDateTime((u8*)"2000/01/01,08:52:15+00");
		
		RTC_ExitConfigMode(); //退出配置模式 
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		// Wait for RTC registers synchronization 
		RTC_WaitForSynchro();
		// Enable the RTC Second
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		//闹钟中断使能
		RTC_ITConfig(RTC_IT_ALR, ENABLE);
		// Wait until last write operation on RTC registers has finished 
		RTC_WaitForLastTask();
	}
	RTC_GetCurrDateTime();

	//RTC_SetAlarmTime(30);//test
}


/*******************************************************************************
* 函数名  			: Time_Show
* 函数描述    	: 向串口打印当前时间 
* 输入参数     	: 无
* 输出结果     	: 无
* 返回值        : 无
******************************************************************************/
void RTC_GetCurrDateTime(void)
{

#ifdef __GSM_RTC_

#else
	u32 CurrenTime = 0;	
	
	if(TimeDisplay==1)
	{
		TimeDisplay = 0;
		if (RTC_GetCounter() == 0x0001517F)
		{
			RTC_SetCounter(0x0);
			// Wait until last write operation on RTC registers has finished 
			RTC_WaitForLastTask();
		}		
	
		// 读出当前RTC计数值（UNIX时间格式） 
		CurrenTime = Time_GetUnixTime();
		// 将UNIX时间格式转换为标准系统时间格式 
		time_now = Time_ConvUnixToCalendar(CurrenTime);
		time_now.tm_mon+=1;	
	}
#endif
}


//设置时间，格式："2014/12/31,23:35:15+00";
void RTC_SetDateTime(u8 *t)
{
   u16 i=0;
   u8 j,k=0,count=0;
   u8 y=0,m=0,d=0,h=0,m2=0,s=0;

   while((*t) != '\0')
   {
   	  j=*t;
	  if(j>='0' && j<='9')
	  {
		 
		 j-=0x30;
		 if(k==2)k=0;
		 k++;
		 if(count==0)
		 {//取出年
			i=i*10+j;
		 }
		 else if(count==1)
		 {//取出月
			if(y==0)
			{
			   y=1;
			   if(i<100)i=2000+i;
			   time_now.tm_year=i;
			   i=0;
			}
			i=i*10+j;
		 }
		 else if(count==2)
		 {//取出日
			if(m==0)
			{
			   m=1;
			   time_now.tm_mon=i;
			   i=0;
			}
			i=i*10+j;
		 }
		 else if(count==3)
		 {//取出小时
			if(d==0)
			{
			   d=1;
			   time_now.tm_mday=i;
			   i=0;
			}
			i=i*10+j;
		 }
		 else if(count==4)
		 {//取出分钟
			if(h==0)
			{
			   h=1;
			   time_now.tm_hour=i;
			   i=0;
			}
			i=i*10+j;
		 }
		 else if(count==5)
		 {//取出秒钟
			if(m2==0)
			{
			   m2=1;
			   time_now.tm_min=i;
			   i=0;
			}
			i=i*10+j;
			if(k==2)
			{
			   time_now.tm_sec=i;
			   break;
			}
		 }
	  }
	  else
	  {
		count++;
	  }
	  t++;
   }   

#ifdef __GSM_RTC_
	time_now.tm_wday = RTC_Get_Week(time_now.tm_year, time_now.tm_mon, time_now.tm_mday);
#else
   Time_SetCalendarTime(time_now);
#endif
   		
}


/*******************************************************************************
* Function Name  : Time_SetCalendarTime()
* Description    : 将给定的Calendar格式时间转换成UNIX时间戳写入RTC
* Input 		 : struct tm t
* Output		 : None
* Return		 : None
*******************************************************************************/
void Time_SetCalendarTime(struct tm t)
{
	Time_SetUnixTime(Time_ConvCalendarToUnix(t));
	return;
}

u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表

//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//输入参数：公历年月日
//返回值：星期号
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{
       u16 temp2;
       u8 yearH,yearL;

       yearH=year/100;     yearL=year%100;
       // 如果为21世纪,年份数加100
       if (yearH>19)yearL+=100;
       // 所过闰年数只算1900年之后的
       temp2=yearL+yearL/4;
       temp2=temp2%7;
       temp2=temp2+day+table_week[month-1];
       if (yearL%4==0&&month<3)temp2--;
       return(temp2%7);
}

/*******************************************************************************
* Function Name  : Time_SetCalendarTime()
* Description    : 将给定的Calendar格式时间转换成UNIX时间戳写入RTC
* Input 		 : struct tm t
* Output		 : None
* Return		 : None
*******************************************************************************/
void Time_SetCalendarTime(struct tm t)
{
	Time_SetUnixTime(Time_ConvCalendarToUnix(t));
	return;
}


/*******************************************************************************
* Function Name  : Time_ConvCalendarToUnix(struct tm t)
* Description    : 写入RTC时钟当前时间
* Input 		 : struct tm t
* Output		 : None
* Return		 : u32
*******************************************************************************/
u32 Time_ConvCalendarToUnix(struct tm t)
{
	t.tm_year -= 1900;  //外部tm结构体存储的年份为2014格式
						//而time.h中定义的年份格式为1900年开始的年份
						//所以，在日期转换时要考虑到这个因素。
	return mktime(&t); //mktime()-->将类型为struct tm结构体变量转换成UNIX的“秒”数据
}


/*******************************************************************************
* Function Name  : Time_GetUnixTime()
* Description    : 从RTC取当前时间的Unix时间戳值
* Input 		 : None
* Output		 : None
* Return		 : u32 t
*******************************************************************************/
u32 Time_GetUnixTime(void)
{
	return (u32)RTC_GetCounter();
}


/*******************************************************************************
* Function Name  : Time_ConvUnixToCalendar(time_t t)
* Description    : 转换UNIX时间戳为日历时间
* Input 		 : u32 t  当前时间的UNIX时间戳
* Output		 : None
* Return		 : struct tm
*******************************************************************************/
struct tm Time_ConvUnixToCalendar(time_t t)
{
	struct tm *t_tm;
	t_tm = localtime(&t);	//localtime（）的功能与mktime（）正好相反
	t_tm->tm_year += 1900;	//localtime转换结果的tm_year是相对值，需要转成绝对值
	return *t_tm;
}


/*******************************************************************************
* Function Name  : Time_SetUnixTime()
* Description    : 将给定的Unix时间戳写入RTC
* Input 		 : time_t t
* Output		 : None
* Return		 : None
*******************************************************************************/
void Time_SetUnixTime(time_t t)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 	
	RTC_WaitForLastTask();
	RTC_SetCounter((u32)t);
	RTC_WaitForLastTask();
	return;
}

/*******************************************************************************
* 函数名  	   : set_alarm_time
* 函数描述     : 设置闹钟时间
* 输入参数     : 要设置的闹钟时间
* 输出结果     : 无
* 返回值       : 无
******************************************************************************/
void RTC_SetAlarmTime(u32 AlarmTime)
{
	u32 Tcont;
	Tcont=RTC->CNTH;			//得到计数器中的值(秒钟数)
	Tcont<<=16;
	Tcont+=RTC->CNTL;
	Tcont=Tcont+AlarmTime;
	RCC->APB1ENR|=1<<28;		//使能电源时钟
    	RCC->APB1ENR|=1<<27;		//使能备份时钟
	PWR->CR|=1<<8;    			//取消备份区写保护
	RTC->CRL|=1<<4;   			//允许配置 
	RTC->ALRL=Tcont&0xffff;
	RTC->ALRH=Tcont>>16;
	RTC->CRL&=~(1<<4);			//配置更新
	while(!(RTC->CRL&(1<<5)));	//等待RTC寄存器操作完成 
}

//void USART_SendString(u8 USARTx, u8 *puts,u16 Length);

/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
		// 清除 RTC 秒中断 
		RTC_ClearITPendingBit(RTC_IT_SEC);
	}
	
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)						//闹钟中断
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);						//清闹钟中断				  	   		
		
		//USART_SendString(UART1, "rtc..\r\n",7);
		//RTC_SetAlarmTime(30);										//设置30秒闹一次
	} 				  								 
	RTC_ClearITPendingBit(RTC_IT_ALR|RTC_IT_SEC|RTC_IT_OW);		//中断标记
	RTC_WaitForLastTask();	
}


void GSM_RTC_Get_Time()
{
	u8 mon_day[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	time_now.tm_sec ++;
	if(time_now.tm_sec > 60)
	{
		time_now.tm_sec = 0;
		time_now.tm_min ++;
		if(ClockTimeFactor > 0)
			ClockTimeFactor --;
	}
	if(time_now.tm_min > 60)
	{
		time_now.tm_min = 0;
		time_now.tm_hour ++;
	}
	if(time_now.tm_hour > 24)
	{
		time_now.tm_hour = 0;
		time_now.tm_mday ++;
		if(time_now.tm_mday > mon_day[time_now.tm_mon])
		{
			time_now.tm_mday = 1;
			time_now.tm_mon ++;
		}
		GSM_GetCLOCK(GsmDateTime);
		RTC_SetDateTime((u8*)GsmDateTime);//设置时间
	
		time_now.tm_wday = RTC_Get_Week(time_now.tm_year, time_now.tm_mon, time_now.tm_mday);
	}
	
	if(time_now.tm_mon > 12)
	{
		time_now.tm_mon = 1;
	}

}

