
#define __DELAY_C_

#include "stm32f10x.h"
#include "delay.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


static __IO uint16_t TimingDelay;


//static u8  fac_us=0;//us��ʱ������			   
//static u16 fac_ms=0;//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��

#define SYSTEM_CLOCK	72		   //ϵͳ��Ƶ

//��ʼ���ӳٺ���
void delay_init(void)
{
#if 0
    u8 SYSCLK=(u8)SYSTEM_CLOCK;

 	SysTick->CTRL&=~(1<<2);	//SYSTICKʹ���ⲿʱ��Դ	 
	fac_us=SYSCLK/8;		//�����Ƿ�ʹ��ucos,fac_us����Ҫʹ��
	fac_ms=(u16)fac_us*1000;//��ucos��,����ÿ��ms��Ҫ��systickʱ����
#endif
}					

#if 0
//nusΪҪ��ʱ��us��.		    								   
void delay_xus(u32 nus)
{
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL=0x01 ;      //��ʼ���� 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void delay_xms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL=0x01 ;          //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
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
		i=4;  //�Լ�����       
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

