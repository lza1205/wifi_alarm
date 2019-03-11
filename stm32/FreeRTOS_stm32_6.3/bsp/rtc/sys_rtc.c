

#define __SYS_RTC_C_
#include "all.h"

/*******************************************************************************
* ������ 		: RTC_Configuration
* ��������    	: ����RTC
* �������    	: ��
* ������     	: ��
* ����ֵ       	: ��
*******************************************************************************/
void sys_rtc_init(void)
{	
	volatile u32 timeout = 0;

	// ���� NVIC ��ʼ���ṹ�� 
	NVIC_InitTypeDef NVIC_InitStructure;
	// ʹ�� RTC �ж� 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP|RCC_APB1Periph_PWR,ENABLE); //RTC�Ǵ��ں������еģ�������Ҫ����������͵�Դʱ�ӵ�
	PWR_BackupAccessCmd(ENABLE);	   //��Դ���ƼĴ���(PWR_CR)��BPBλʹ�ܶԺ������RTC�ķ���
	// ʹ�� RTC ʱ�� 
	RCC_RTCCLKCmd(ENABLE);

	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)		//�ж��Ƿ��ǵ�һ������ʱ��
	{	
		// �� APB1 �����ϵ� PWR��BKP ʱ�� 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);		
		// ʹ�� RTC �ͺ󱸼Ĵ������� 
		PWR_BackupAccessCmd(ENABLE);
		// ��λ���ݼĴ������� 
		BKP_DeInit();
		// ���� LSE 
		RCC_LSEConfig(RCC_LSE_ON);
		// �ȴ�LSE���� 
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{
			timeout ++;
			if(timeout > 0x1fffff)
			{
				Delay_10ms(100);
				printp("RTC ERROR\r\n"); 
				return ;							//��ʱ�˳�
			}
		}

		// ѡ�� LSE Ϊ RTC ʱ��Դ 
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		// ʹ�� RTC ʱ�� 
		RCC_RTCCLKCmd(ENABLE);
		// �ȴ� RTC �Ĵ���ͬ����� 
		RTC_WaitForSynchro();
		// �ȴ����һ�ζ� RTC �Ĵ�����д������� 
		RTC_WaitForLastTask();
		// ʹ��RTC���ж�   
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		//�����ж�ʹ��
		RTC_ITConfig(RTC_IT_ALR, ENABLE); 
		// �ȴ����һ�ζ� RTC �Ĵ�����д������� 
		RTC_WaitForLastTask();
		// ����RTCʱ�ӷ�ƵֵΪ32767�������Ƶ�� = (32.768 KHz)/(32767+1) = 1Hz��~1s��
		RTC_SetPrescaler(32767); 
		// �ȴ����һ�ζ� RTC �Ĵ�����д������� 
		RTC_WaitForLastTask();
		
		// �趨��ʼʱ�� 				
		RTC_SetDateTime((u8*)"2000/01/01,08:52:15+00");
		
		RTC_ExitConfigMode(); //�˳�����ģʽ 
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		// Wait for RTC registers synchronization 
		RTC_WaitForSynchro();
		// Enable the RTC Second
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		//�����ж�ʹ��
		RTC_ITConfig(RTC_IT_ALR, ENABLE);
		// Wait until last write operation on RTC registers has finished 
		RTC_WaitForLastTask();
	}
	RTC_GetCurrDateTime();

	//RTC_SetAlarmTime(30);//test
}


/*******************************************************************************
* ������  			: Time_Show
* ��������    	: �򴮿ڴ�ӡ��ǰʱ�� 
* �������     	: ��
* ������     	: ��
* ����ֵ        : ��
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
	
		// ������ǰRTC����ֵ��UNIXʱ���ʽ�� 
		CurrenTime = Time_GetUnixTime();
		// ��UNIXʱ���ʽת��Ϊ��׼ϵͳʱ���ʽ 
		time_now = Time_ConvUnixToCalendar(CurrenTime);
		time_now.tm_mon+=1;	
	}
#endif
}


//����ʱ�䣬��ʽ��"2014/12/31,23:35:15+00";
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
		 {//ȡ����
			i=i*10+j;
		 }
		 else if(count==1)
		 {//ȡ����
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
		 {//ȡ����
			if(m==0)
			{
			   m=1;
			   time_now.tm_mon=i;
			   i=0;
			}
			i=i*10+j;
		 }
		 else if(count==3)
		 {//ȡ��Сʱ
			if(d==0)
			{
			   d=1;
			   time_now.tm_mday=i;
			   i=0;
			}
			i=i*10+j;
		 }
		 else if(count==4)
		 {//ȡ������
			if(h==0)
			{
			   h=1;
			   time_now.tm_hour=i;
			   i=0;
			}
			i=i*10+j;
		 }
		 else if(count==5)
		 {//ȡ������
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
* Description    : ��������Calendar��ʽʱ��ת����UNIXʱ���д��RTC
* Input 		 : struct tm t
* Output		 : None
* Return		 : None
*******************************************************************************/
void Time_SetCalendarTime(struct tm t)
{
	Time_SetUnixTime(Time_ConvCalendarToUnix(t));
	return;
}

u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�

//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//�������������������
//����ֵ�����ں�
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{
       u16 temp2;
       u8 yearH,yearL;

       yearH=year/100;     yearL=year%100;
       // ���Ϊ21����,�������100
       if (yearH>19)yearL+=100;
       // ����������ֻ��1900��֮���
       temp2=yearL+yearL/4;
       temp2=temp2%7;
       temp2=temp2+day+table_week[month-1];
       if (yearL%4==0&&month<3)temp2--;
       return(temp2%7);
}

/*******************************************************************************
* Function Name  : Time_SetCalendarTime()
* Description    : ��������Calendar��ʽʱ��ת����UNIXʱ���д��RTC
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
* Description    : д��RTCʱ�ӵ�ǰʱ��
* Input 		 : struct tm t
* Output		 : None
* Return		 : u32
*******************************************************************************/
u32 Time_ConvCalendarToUnix(struct tm t)
{
	t.tm_year -= 1900;  //�ⲿtm�ṹ��洢�����Ϊ2014��ʽ
						//��time.h�ж������ݸ�ʽΪ1900�꿪ʼ�����
						//���ԣ�������ת��ʱҪ���ǵ�������ء�
	return mktime(&t); //mktime()-->������Ϊstruct tm�ṹ�����ת����UNIX�ġ��롱����
}


/*******************************************************************************
* Function Name  : Time_GetUnixTime()
* Description    : ��RTCȡ��ǰʱ���Unixʱ���ֵ
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
* Description    : ת��UNIXʱ���Ϊ����ʱ��
* Input 		 : u32 t  ��ǰʱ���UNIXʱ���
* Output		 : None
* Return		 : struct tm
*******************************************************************************/
struct tm Time_ConvUnixToCalendar(time_t t)
{
	struct tm *t_tm;
	t_tm = localtime(&t);	//localtime�����Ĺ�����mktime���������෴
	t_tm->tm_year += 1900;	//localtimeת�������tm_year�����ֵ����Ҫת�ɾ���ֵ
	return *t_tm;
}


/*******************************************************************************
* Function Name  : Time_SetUnixTime()
* Description    : ��������Unixʱ���д��RTC
* Input 		 : time_t t
* Output		 : None
* Return		 : None
*******************************************************************************/
void Time_SetUnixTime(time_t t)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
	PWR_BackupAccessCmd(ENABLE);	//ʹ��RTC�ͺ󱸼Ĵ������� 	
	RTC_WaitForLastTask();
	RTC_SetCounter((u32)t);
	RTC_WaitForLastTask();
	return;
}

/*******************************************************************************
* ������  	   : set_alarm_time
* ��������     : ��������ʱ��
* �������     : Ҫ���õ�����ʱ��
* ������     : ��
* ����ֵ       : ��
******************************************************************************/
void RTC_SetAlarmTime(u32 AlarmTime)
{
	u32 Tcont;
	Tcont=RTC->CNTH;			//�õ��������е�ֵ(������)
	Tcont<<=16;
	Tcont+=RTC->CNTL;
	Tcont=Tcont+AlarmTime;
	RCC->APB1ENR|=1<<28;		//ʹ�ܵ�Դʱ��
    	RCC->APB1ENR|=1<<27;		//ʹ�ܱ���ʱ��
	PWR->CR|=1<<8;    			//ȡ��������д����
	RTC->CRL|=1<<4;   			//�������� 
	RTC->ALRL=Tcont&0xffff;
	RTC->ALRH=Tcont>>16;
	RTC->CRL&=~(1<<4);			//���ø���
	while(!(RTC->CRL&(1<<5)));	//�ȴ�RTC�Ĵ���������� 
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
		// ��� RTC ���ж� 
		RTC_ClearITPendingBit(RTC_IT_SEC);
	}
	
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)						//�����ж�
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);						//�������ж�				  	   		
		
		//USART_SendString(UART1, "rtc..\r\n",7);
		//RTC_SetAlarmTime(30);										//����30����һ��
	} 				  								 
	RTC_ClearITPendingBit(RTC_IT_ALR|RTC_IT_SEC|RTC_IT_OW);		//�жϱ��
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
		RTC_SetDateTime((u8*)GsmDateTime);//����ʱ��
	
		time_now.tm_wday = RTC_Get_Week(time_now.tm_year, time_now.tm_mon, time_now.tm_mday);
	}
	
	if(time_now.tm_mon > 12)
	{
		time_now.tm_mon = 1;
	}

}

