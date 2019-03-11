

#include "all.h"

#define uchar unsigned char
#define uint unsigned int


#define RF	GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)

u8 LED;	   //学习指示灯						  
u8 D0;	  //解码输出
u8 D1;
u8 D2;
u8 D3;
u8 VT;				


u8  decode_ok;		 //解码成功
u8 rf_ok;	       //收到有效数据
u8 study;         //学习标志
u8 jmnx;  //编码类型 0是2262，1是1527

u8  decode_ok;		 //解码成功
uchar  hh_w,ll_w;           //高,低电平宽度
uchar  ma_x;                //接收到第几位编码了

uchar  bma1,bma2,bma3, bma4; //用于接收过程存放遥控编码，编码比较两次，这是第一次
uchar  mmb1,mmb2,mmb3, mmb4; //	用于接收过程存放遥控编码，第二次
uchar  mma1,mma2,mma3, mma4; //用于解码过程

u8 rf_ok1,rf_ok2;		 //解码过程中的临时接收成功标志
u8 old_rc5;             //保存上一次查询到的电平状态
u8 tb_ok;               //接收到同步的马时置1   
u8 rf_ok;               //接收到一个完整的遥控命令后置1,通知解码程序可以解码了

uchar xplx;
uchar key_data;
 		
uchar temp_h,temp_l;

u8 bt_auto;     //自动设置遥控接收波特率标志

u8 short_k;
uchar da1527[2][3];  //解码过程中临时数组

uint s,s1;


u8 ev1527_data[3];

volatile u8 _dbg;

/*
static void delay(uint ms)//此处换用不同单片机需调整参数
{
	delay(ms);
}
*/

/**********************************************************
	定时器2
***********************************************************/
void sys_timer2_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_truct;
	NVIC_InitTypeDef NVIC_truct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	//打开外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_truct.TIM_Period = 7;	//70us 中断一次
	TIM_truct.TIM_Prescaler = 719;	//720分频
	TIM_truct.TIM_ClockDivision = 0;
	TIM_truct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2,&TIM_truct);			//初始化
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);	//开定时器中断
	TIM_Cmd(TIM2,ENABLE);						//使能定时器

	NVIC_truct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_truct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_truct.NVIC_IRQChannelSubPriority = 0;
	NVIC_truct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_truct);
}


void __dbg_ev1527(void)
{
	 if (!RF)  // 检测到低电平 低电平时间加1,记录本次电平状态
	 {
		 _dbg++;
	 }else{
		 _dbg--;
	 }
}


void rf_io_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//选择PX.0 8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//设置为下拉输入
	GPIO_Init(GPIOC , &GPIO_InitStructure);//初始化GPIOA寄存器

}

void chick1527(uchar d)
{
	uchar u;
	for(u=0;u<4;u++) 
		{
			if(((d>>(u*2)) & 3)==2)
				 {
					 xplx=0x55;
				  }
		}
}


//============================接收解码部分========================================//

void RF_decode(void) 	 
{ 
	   if (!RF) { ll_w++;old_rc5=0; }			// 检测到低电平 低电平时间加1,记录本次电平状态
		 else									   // 检测到高电平
		 { hh_w++;
		  if (!old_rc5) 						 // 检测到从低到高的跳变,已检测到一个完整(高-低)电平周期
		   { if (((hh_w>=2)&&(hh_w<=8))&&((ll_w>=50)&&(ll_w<=190))) 		 //判同步码    2/5 110/140
			  { 
				 LED=!LED;
				 if((ll_w>=110)&&(ll_w<=140)){ma_x=0;tb_ok=1;bma1=0; bma2=0; bma3=0; bt_auto=0;}  //根据不同 同步码 宽度，T0加载不同初值
				   else
					 {	
					   if((ll_w>140)&&(ll_w<=160))
						 { ma_x=0;tb_ok=1;bma1=0; bma2=0; bma3=0; bt_auto=1;temp_h=0xfe;temp_l=0x6b; }				   
						else
						  {  
						   if((ll_w>160)&&(ll_w<=190))
							 { ma_x=0;tb_ok=1;bma1=0; bma2=0; bma3=0; bt_auto=1;temp_h=0xfd;temp_l=0xdc; }
							 else
								 {					   
								   if((ll_w>70)&&(ll_w<=90))
									 {ma_x=0;tb_ok=1;bma1=0; bma2=0; bma3=0; bt_auto=1;temp_h=0xff;temp_l=0x30; }
									 else
										 {
										   if((ll_w>90)&&(ll_w<=110))
											 {ma_x=0;tb_ok=1;bma1=0; bma2=0; bma3=0; bt_auto=1;temp_h=0xff;temp_l=0x00; }
											 else
											   {
												  if((ll_w>=50)&&(ll_w<=70))
													 {ma_x=0;tb_ok=1;bma1=0; bma2=0; bma3=0; bt_auto=1;temp_h=0xff;temp_l=0x7a; }									 
											   }
			 
										 }
								 }							   
						  }
					   }		 
			  }
			 else if ((tb_ok)&&((ll_w>=10)&&(ll_w<=14)))	 //   10/14
				 {	 
					 ma_x++;			 //已经接收到同步码,判0
					 if(ma_x>23)
						 {
							if(!rf_ok1)
							 {
							   mma1=bma1;mma2=bma2;mma3=bma3;mma4=bma4;//将接收到的编码复制到解码寄存器中							  
							   rf_ok1=1;								// 通知解码子程序可以解码了
							   tb_ok=0;
							   bt_auto=0;
							   s=1000;								 
							 }
							 else
								 {
								   mmb1=bma1;mmb2=bma2;mmb3=bma3;mmb4=bma4;//将接收到的编码复制到解码寄存器中							  
								   rf_ok2=1;								// 通知解码子程序可以解码了
								   tb_ok=0;
								   bt_auto=0;																			 
								 }
						 }
				  }  
			 else if ((tb_ok)&&((ll_w>=2)&&(ll_w<=8)))		  //   3/5
				   { switch (ma_x)
					  { 
						case 0 : { bma1=bma1 | 0x80; break; }//遥控编码第1位
						case 1 : { bma1=bma1 | 0x40; break; }
						case 2 : { bma1=bma1 | 0x20; break; }
						case 3 : { bma1=bma1 | 0x10; break; }
						case 4 : { bma1=bma1 | 0x08; break; }
						case 5 : { bma1=bma1 | 0x04; break; }
						case 6 : { bma1=bma1 | 0x02; break; }
						case 7 : { bma1=bma1 | 0x01; break; }
						case 8 : { bma2=bma2 | 0x80; break; }
						case 9 : { bma2=bma2 | 0x40; break; }
						case 10: { bma2=bma2 | 0x20; break; }
						case 11: { bma2=bma2 | 0x10; break; }
						case 12: { bma2=bma2 | 0x08; break; }
						case 13: { bma2=bma2 | 0x04; break; }
						case 14: { bma2=bma2 | 0x02; break; }
						case 15: { bma2=bma2 | 0x01; break; }
						case 16: { bma3=bma3 | 0x80; break; }
						case 17: { bma3=bma3 | 0x40; break; }
						case 18: { bma3=bma3 | 0x20; break; }
						case 19: { bma3=bma3 | 0x10; break; }
						case 20: { bma4=bma4 | 0x80; break; }//按键状态第1位
						case 21: { bma4=bma4 | 0x40; break; }
						case 22: { bma4=bma4 | 0x20; break; }
						case 23: { bma4=bma4 | 0x10;
						
								   if(!rf_ok1)
									 {
									   mma1=bma1;mma2=bma2;mma3=bma3;mma4=bma4;//将接收到的编码复制到解码寄存器中							  
									   rf_ok1=1;								// 通知解码子程序可以解码了
									   tb_ok=0;
									   bt_auto=0;
									   s=1000;
									   break;									 
									 }
									 else
										 {
										   mmb1=bma1;mmb2=bma2;mmb3=bma3;mmb4=bma4;//将再次接收到的编码复制到解码寄存器中， 							
										   rf_ok2=1;								// 通知解码子程序可以解码了
										   tb_ok=0;
										   bt_auto=0;
										   break;																			 
										 } 
									
								 }
					  } 
					 ma_x++;
					 
				   }
			 else {ma_x=0; tb_ok=0; bt_auto=0;bma1=0; bma2=0; bma3=0; bma4=0;hh_w=1;ll_w=0;}	  //接收到不符合的高-低电平序列
			ll_w=0;hh_w=1; 
			  } 		 
		  old_rc5=1;	  // 记录本次电平状态
		}
	 if(rf_ok1) 	 //规定时间内接受到2帧相同的编码数据才有效
		 {
			 s--;
			 if(!s) rf_ok1=0;
			 if(rf_ok2) 
				 {
				   if((mma1==mmb1)&&(mma2==mmb2)&&(mma3==mmb3)&&(mma4==mmb4))
					  {
					   rf_ok=1;
					   rf_ok1=0;
					   rf_ok2=0;				  
					  }
					  else
						{
						   rf_ok=0;
						   rf_ok1=0;
						   rf_ok2=0;
					  
						}
								 
				 }					 
		 }
	
	 if((rf_ok))	  //判断是否接收成功
			 {	 
				 //EA=0;
				 rf_ok=0;
												 
				 D3=!(mma4 & 0x80);    //取按键码
				 D2=!(mma4 & 0x40);
				 D1=!(mma4 & 0x20);
				 D0=!(mma4 & 0x10);
	
				 decode_ok=1;
				 VT=0;		 
				 s1=1000;

				 /* 获取EV1527 码 */
				 ev1527_data[0] = mma1;
				 ev1527_data[1] = mma2;
				 ev1527_data[2] = mma3 | (mma4 >> 4);
			#ifdef __OS__
				msg_set_isr(MSG_GOURP_INPUT, MSG_INPUT_EV1527);
				// EA=1;
			#endif
	 	}
	
#if 0
	if(decode_ok)	//解码有效信号，类似2272 PT脚
		 {
			 s1--;
			 if(!s1)
				 {
					 VT=1;
					 decode_ok=0;
				 }				   
		 }
#endif
	}



void send_rf_data(void)
{
	static u32 __cnt = 0;
	if(decode_ok)
	{
		s1--;
		if(!s1)
		{
			decode_ok=0;
//			my_send_hex(__cnt++);
			my_send_str(" RF:CODE=");
			my_send_buf(ev1527_data, 3);
			my_send_str("\r\n");
		}
	}
}


void ev1527_init(void)
{
	sys_timer2_init();
	rf_io_init();
}


