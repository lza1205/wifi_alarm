
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

		/* ������Ϣ */
		msg_set_isr(MSG_GOURP_TIME, MSG_TIME_1S);
	}
}


//10ms��ʱ��
void sys_time1_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_truct;
	NVIC_InitTypeDef NVIC_truct;

	//������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

	TIM_truct.TIM_Period = 5000;		/* 5ms */
	TIM_truct.TIM_Prescaler = 72;
	TIM_truct.TIM_ClockDivision = 0;
	TIM_truct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_truct.TIM_RepetitionCounter = 0;		//ÿ��������������������¼�
	TIM_TimeBaseInit(TIM1,&TIM_truct);			//��ʼ��

	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);	//����ʱ���ж�
	TIM_Cmd(TIM1,ENABLE);						//�رն�ʱ��

	NVIC_truct.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_truct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_truct.NVIC_IRQChannelSubPriority = 1;
	NVIC_truct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_truct);	 
}


