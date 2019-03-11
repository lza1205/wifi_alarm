
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
void TIM2_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	// ���� TIM_TimeBase ��ʼ���ṹ�� TIM_TimeBaseStructure 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_DeInit(TIM2);
	// �� TIM2 ʱ�� 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* 
	��ʱʱ����㣺
	T=Ftosc/(TIM_Period+1)*(TIM_Prescaler+1);��Ftosc�Ƕ�ʱ��ʱ�ӣ�
	*/																					  
	TIM_TimeBaseStructure.TIM_Period = 10000;  											//��������ֵΪ100, ���ϼ���ģʽֻ���Ǵ�0~��������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 72;               	 			//��Ƶ72
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;				  //ʱ�ӷָ� 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	  	//���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseStructure);
	
	TIM_SetAutoreload(TIM2, 5000); 
	TIM_ARRPreloadConfig(TIM2, ENABLE);															//5ms Interrupt

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  				
	TIM_Cmd(TIM2 , ENABLE);		// ����TIM2����

	// ���� TIM2 �ж�, 0����ռ���ȼ���0����ռ���ȼ� 	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


