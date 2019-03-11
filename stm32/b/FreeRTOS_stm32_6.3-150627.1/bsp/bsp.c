
#define __BSP_C_

#include "all.h"


/******************************************************************************
* 函数名	: RCC_Configuration
* 函数描述  : 设置系统各部分时钟
* 输入参数  : 无
* 输出结果  : 无
* 返回值    : 无
******************************************************************************/
void RCC_Configuration(void)			    	//Use HSE
{
    // 定义枚举类型变量 HSEStartUpStatus 
    ErrorStatus HSEStartUpStatus;    
    // 复位系统时钟设置
    RCC_DeInit();
    // 开启HSE
    RCC_HSEConfig(RCC_HSE_ON);
    // 等待HSE起振并稳定
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    // 判断HSE起是否振成功，是则进入if()内部 
    if(HSEStartUpStatus == SUCCESS)
    {
		// 选择HCLK（AHB）时钟源为SYSCLK 1分频 
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
		// 选择PCLK2时钟源为 HCLK（AHB） 1分频 
		RCC_PCLK2Config(RCC_HCLK_Div1); 
		// 选择PCLK1时钟源为 HCLK（AHB） 2分频 
		RCC_PCLK1Config(RCC_HCLK_Div2);
		// 设置FLASH延时周期数为2 
		FLASH_SetLatency(FLASH_Latency_2);
		// 使能FLASH预取缓存 
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		// 选择锁相环（PLL）时钟源为HSE 1分频，倍频数为9，则PLL输出频率为 8MHz * 9 = 72MHz 
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		// 使能PLL  
		RCC_PLLCmd(ENABLE);
		// 等待PLL输出稳定 
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		// 选择SYSCLK时钟源为PLL 
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		// 等待PLL成为SYSCLK时钟源 
		while(RCC_GetSYSCLKSource() != 0x08);
    }		  
   	// 打开APB2总线上的GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, and AFIO时钟
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC 
    | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
}


//打开或关闭外部中断
void EXTIX_Switch(u8 EXTI_Line, u8 type)
{
	if(type== DISABLE)
	{//关闭
		/* 清除EXTI中断和事件线路配置 */
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


//IO口初始化
void IO_init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure; 	// 定义GPIO初始化结构体 GPIO_InitStructure 

	// 打开APB2总线上的GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, and AFIO时钟
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC 
    | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);

	// 设置 GPIOB输出口
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    port_SPI_CS=1;
	port_SPI_CLK=1;

	// 设置 GPIOA输出口
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 |  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//设置PC输出口
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//设置PC输入口
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	//设置PB输入口
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
	/* 系统时钟初始化 */
	RCC_Configuration();

	/* GPIO 引脚初始化 */
	IO_init();

	/* dbug */
//	__debug_delay_us();

	/* 定时器功能 */
	TIM2_init();

	/* 串口初始化 */
	sys_uart1_init();
	sys_uart2_init();
	
}

