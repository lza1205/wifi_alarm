
#define __SYS_TIME_C_

#include "all.h"


void Time_10ms_Interrupt(void)
{
	
//	u8 i;

	Timer5ms++;	

	/* 10ms */
	if(Timer5ms>=2)
	{
		Timer5ms=0;
		Time10ms ++;

		USARTx_TimeCheck();
	}

	/* 100ms */
	if(Time10ms>=10)
	{//100ms
		Time10ms = 0;
		time_x100ms ++;
	}

	/* 1S */
	if(time_x100ms >= 10)
	{
		time_x100ms = 0;

		/* 发送消息 */
		msg_set_isr(MSG_GOURP_TIME, MSG_TIME_1S);
	}
}


//10ms定时器
void sys_time1_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_truct;
	NVIC_InitTypeDef NVIC_truct;

	//打开外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

	TIM_truct.TIM_Period = 5000;		/* 5ms */
	TIM_truct.TIM_Prescaler = 72;
	TIM_truct.TIM_ClockDivision = 0;
	TIM_truct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_truct.TIM_RepetitionCounter = 0;		//每次向上溢出都产生更新事件
	TIM_TimeBaseInit(TIM1,&TIM_truct);			//初始化

	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);	//开定时器中断
	TIM_Cmd(TIM1,ENABLE);						//关闭定时器

	NVIC_truct.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_truct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_truct.NVIC_IRQChannelSubPriority = 1;
	NVIC_truct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_truct);	 
}


