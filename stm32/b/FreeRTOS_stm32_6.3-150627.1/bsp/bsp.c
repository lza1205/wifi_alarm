
#define __BSP_C_

#include "all.h"


/******************************************************************************
* ������	: RCC_Configuration
* ��������  : ����ϵͳ������ʱ��
* �������  : ��
* ������  : ��
* ����ֵ    : ��
******************************************************************************/
void RCC_Configuration(void)			    	//Use HSE
{
    // ����ö�����ͱ��� HSEStartUpStatus 
    ErrorStatus HSEStartUpStatus;    
    // ��λϵͳʱ������
    RCC_DeInit();
    // ����HSE
    RCC_HSEConfig(RCC_HSE_ON);
    // �ȴ�HSE�����ȶ�
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    // �ж�HSE���Ƿ���ɹ����������if()�ڲ� 
    if(HSEStartUpStatus == SUCCESS)
    {
		// ѡ��HCLK��AHB��ʱ��ԴΪSYSCLK 1��Ƶ 
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
		// ѡ��PCLK2ʱ��ԴΪ HCLK��AHB�� 1��Ƶ 
		RCC_PCLK2Config(RCC_HCLK_Div1); 
		// ѡ��PCLK1ʱ��ԴΪ HCLK��AHB�� 2��Ƶ 
		RCC_PCLK1Config(RCC_HCLK_Div2);
		// ����FLASH��ʱ������Ϊ2 
		FLASH_SetLatency(FLASH_Latency_2);
		// ʹ��FLASHԤȡ���� 
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		// ѡ�����໷��PLL��ʱ��ԴΪHSE 1��Ƶ����Ƶ��Ϊ9����PLL���Ƶ��Ϊ 8MHz * 9 = 72MHz 
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		// ʹ��PLL  
		RCC_PLLCmd(ENABLE);
		// �ȴ�PLL����ȶ� 
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		// ѡ��SYSCLKʱ��ԴΪPLL 
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		// �ȴ�PLL��ΪSYSCLKʱ��Դ 
		while(RCC_GetSYSCLKSource() != 0x08);
    }		  
   	// ��APB2�����ϵ�GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, and AFIOʱ��
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC 
    | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
}


//�򿪻�ر��ⲿ�ж�
void EXTIX_Switch(u8 EXTI_Line, u8 type)
{
	if(type== DISABLE)
	{//�ر�
		/* ���EXTI�жϺ��¼���·���� */
	    EXTI->IMR &= ~((u32)(1 << EXTI_Line ));
	    EXTI->EMR &= ~((u32)(1 << EXTI_Line ));
		
	}
	else
	{
		EXTI->IMR |= (u32)(1 << EXTI_Line );
	    EXTI->EMR |= (u32)(1 << EXTI_Line );

		InterEvent=0;
	}

}


//IO�ڳ�ʼ��
void IO_init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure; 	// ����GPIO��ʼ���ṹ�� GPIO_InitStructure 

	// ��APB2�����ϵ�GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, and AFIOʱ��
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC 
    | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

	// ���� GPIOB�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    port_SPI_CS=1;
	port_SPI_CLK=1;

	// ���� GPIOA�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 |  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//����PC�����
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//����PC�����
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	//����PB�����
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

} 




// static __debug_delay_us(void)
// {
// 	/*
// 	while(1)
// 	{
// 		PBout(12) = 1;
// 		delay_us(1);
// 		PBout(12) = 0;
// 		delay_us(1);
// 	}
// 	*/
// }

void bsp_init(void)
{
	/* ϵͳʱ�ӳ�ʼ�� */
	RCC_Configuration();

	/* GPIO ���ų�ʼ�� */
	IO_init();

	/* dbug */
//	__debug_delay_us();

	/* ��ʱ������ */
	TIM2_init();

	/* ���ڳ�ʼ�� */
	sys_uart1_init();
	sys_uart2_init();
	
}

