
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
void TIM2_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	// 定义 TIM_TimeBase 初始化结构体 TIM_TimeBaseStructure 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_DeInit(TIM2);
	// 打开 TIM2 时钟 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* 
	定时时间计算：
	T=Ftosc/(TIM_Period+1)*(TIM_Prescaler+1);（Ftosc是定时器时钟）
	*/																					  
	TIM_TimeBaseStructure.TIM_Period = 10000;  											//计数重载值为100, 向上计数模式只能是从0~计数重载值
	TIM_TimeBaseStructure.TIM_Prescaler = 72;               	 			//分频72
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;				  //时钟分割 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	  	//向上计数模式
	TIM_TimeBaseInit(TIM2 , &TIM_TimeBaseStructure);
	
	TIM_SetAutoreload(TIM2, 5000); 
	TIM_ARRPreloadConfig(TIM2, ENABLE);															//5ms Interrupt

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  				
	TIM_Cmd(TIM2 , ENABLE);		// 启动TIM2计数

	// 开启 TIM2 中断, 0级先占优先级，0级后占优先级 	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


