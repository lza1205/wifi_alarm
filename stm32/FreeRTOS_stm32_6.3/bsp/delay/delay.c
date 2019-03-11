
#define __DELAY_C_

#include "stm32f10x.h"
#include "delay.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


static __IO uint16_t TimingDelay;


//static u8  fac_us=0;//us延时倍乘数			   
//static u16 fac_ms=0;//ms延时倍乘数,在ucos下,代表每个节拍的ms数

#define SYSTEM_CLOCK	72		   //系统主频

//初始化延迟函数
void delay_init(void)
{
#if 0
    u8 SYSCLK=(u8)SYSTEM_CLOCK;

 	SysTick->CTRL&=~(1<<2);	//SYSTICK使用外部时钟源	 
	fac_us=SYSCLK/8;		//不论是否使用ucos,fac_us都需要使用
	fac_ms=(u16)fac_us*1000;//非ucos下,代表每个ms需要的systick时钟数
#endif
}					

#if 0
//nus为要延时的us数.		    								   
void delay_xus(u32 nus)
{
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL=0x01 ;      //开始倒数 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void delay_xms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL=0x01 ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
} 

void  Delay10us(void)
{
   delay_xus(10);
}
void  Delay15us(void)
{
   delay_xus(15);
}
void  Delay20us(void)
{
   delay_xus(20);
}
void  Delay40us(void)
{
   delay_xus(40);
}
void  Delay50us(void)
{
   delay_xus(50);
}
void  Delay60us(void)
{
   delay_xus(60);
}
void  Delay100us(void)
{
   delay_xus(100);
}
void  Delay150us(void)
{
   delay_xus(150);
}
void  Delay200us(void)
{
   delay_xus(200);
}
void  Delay_10us(unsigned char x_10us)
{
   delay_xus((u32)x_10us*10);
}
void  Delay_100us(unsigned char x_100us)
{
   delay_xus((u32)x_100us*10);
}
void  Delay_1ms(unsigned char x_1ms)
{
   delay_xms((u32)x_1ms);
}
void  Delay_10ms(unsigned char x_10ms)
{
   u8 i=x_10ms;

   while(i--)delay_xms(10);
}
#else

void delay_us(u32 us)
{
/*
	u32 i, j;
	for(i = 0; i < us; i++)
		for(j = 0; j < 10; j++);
*/
	u16 i=0;
	while(us--)
	{
		i=4;  //自己定义       
		while(i--);         
	} 
}


void delay_ms(u32 us)
{
	u32 i, j;
	if(delay_flg == 1)
		vTaskDelay(us / portTICK_RATE_MS);
	else {
		for(i = 0; i < us; i++)
			for(j = 0; j < 10000; j++);
	}
}


void delay_xus(u32 nus)
{
	delay_us(nus);
}

void delay_xms(u16 nms)
{
	delay_ms(nms);
}
void Delay_10ms(u8 ms)
{
	delay_ms(10 * ms);
}

#endif

