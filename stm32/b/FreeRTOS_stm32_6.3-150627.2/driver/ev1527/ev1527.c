

#include "all.h"

#define uchar unsigned char
#define uint unsigned int


#define RF	GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)

u8 LED;	   //ѧϰָʾ��						  
u8 D0;	  //�������
u8 D1;
u8 D2;
u8 D3;
u8 VT;				


u8  decode_ok;		 //����ɹ�
u8 rf_ok;	       //�յ���Ч����
u8 study;         //ѧϰ��־
u8 jmnx;  //�������� 0��2262��1��1527

u8  decode_ok;		 //����ɹ�
uchar  hh_w,ll_w;           //��,�͵�ƽ���
uchar  ma_x;                //���յ��ڼ�λ������

uchar  bma1,bma2,bma3, bma4; //���ڽ��չ��̴��ң�ر��룬����Ƚ����Σ����ǵ�һ��
uchar  mmb1,mmb2,mmb3, mmb4; //	���ڽ��չ��̴��ң�ر��룬�ڶ���
uchar  mma1,mma2,mma3, mma4; //���ڽ������

u8 rf_ok1,rf_ok2;		 //��������е���ʱ���ճɹ���־
u8 old_rc5;             //������һ�β�ѯ���ĵ�ƽ״̬
u8 tb_ok;               //���յ�ͬ������ʱ��1   
u8 rf_ok;               //���յ�һ��������ң���������1,֪ͨ���������Խ�����

uchar xplx;
uchar key_data;
 		
uchar temp_h,temp_l;

u8 bt_auto;     //�Զ�����ң�ؽ��ղ����ʱ�־

u8 short_k;
uchar da1527[2][3];  //�����������ʱ����

uint s,s1;


u8 ev1527_data[3];

volatile u8 _dbg;

/*
static void delay(uint ms)//�˴����ò�ͬ��Ƭ�����������
{
	delay(ms);
}
*/

/**********************************************************
	��ʱ��2
***********************************************************/
void sys_timer2_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_truct;
	NVIC_InitTypeDef NVIC_truct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	//������ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_truct.TIM_Period = 7;	//70us �ж�һ��
	TIM_truct.TIM_Prescaler = 719;	//720��Ƶ
	TIM_truct.TIM_ClockDivision = 0;
	TIM_truct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2,&TIM_truct);			//��ʼ��
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);	//����ʱ���ж�
	TIM_Cmd(TIM2,ENABLE);						//ʹ�ܶ�ʱ��

	NVIC_truct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_truct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_truct.NVIC_IRQChannelSubPriority = 0;
	NVIC_truct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_truct);
}


void __dbg_ev1527(void)
{
	 if (!RF)  // ��⵽�͵�ƽ �͵�ƽʱ���1,��¼���ε�ƽ״̬
	 {
		 _dbg++;
	 }else{
		 _dbg--;
	 }
}


void rf_io_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//ѡ��PX.0 8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����Ϊ��������
	GPIO_Init(GPIOC , &GPIO_InitStructure);//��ʼ��GPIOA�Ĵ���

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


//============================���ս��벿��========================================//

void RF_decode(void) 	 
{ 
	   if (!RF) { ll_w++;old_rc5=0; }			// ��⵽�͵�ƽ �͵�ƽʱ���1,��¼���ε�ƽ״̬
		 else									   // ��⵽�ߵ�ƽ
		 { hh_w++;
		  if (!old_rc5) 						 // ��⵽�ӵ͵��ߵ�����,�Ѽ�⵽һ������(��-��)��ƽ����
		   { if (((hh_w>=2)&&(hh_w<=8))&&((ll_w>=50)&&(ll_w<=190))) 		 //��ͬ����    2/5 110/140
			  { 
				 LED=!LED;
				 if((ll_w>=110)&&(ll_w<=140)){ma_x=0;tb_ok=1;bma1=0; bma2=0; bma3=0; bt_auto=0;}  //���ݲ�ͬ ͬ���� ��ȣ�T0���ز�ͬ��ֵ
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
					 ma_x++;			 //�Ѿ����յ�ͬ����,��0
					 if(ma_x>23)
						 {
							if(!rf_ok1)
							 {
							   mma1=bma1;mma2=bma2;mma3=bma3;mma4=bma4;//�����յ��ı��븴�Ƶ�����Ĵ�����							  
							   rf_ok1=1;								// ֪ͨ�����ӳ�����Խ�����
							   tb_ok=0;
							   bt_auto=0;
							   s=1000;								 
							 }
							 else
								 {
								   mmb1=bma1;mmb2=bma2;mmb3=bma3;mmb4=bma4;//�����յ��ı��븴�Ƶ�����Ĵ�����							  
								   rf_ok2=1;								// ֪ͨ�����ӳ�����Խ�����
								   tb_ok=0;
								   bt_auto=0;																			 
								 }
						 }
				  }  
			 else if ((tb_ok)&&((ll_w>=2)&&(ll_w<=8)))		  //   3/5
				   { switch (ma_x)
					  { 
						case 0 : { bma1=bma1 | 0x80; break; }//ң�ر����1λ
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
						case 20: { bma4=bma4 | 0x80; break; }//����״̬��1λ
						case 21: { bma4=bma4 | 0x40; break; }
						case 22: { bma4=bma4 | 0x20; break; }
						case 23: { bma4=bma4 | 0x10;
						
								   if(!rf_ok1)
									 {
									   mma1=bma1;mma2=bma2;mma3=bma3;mma4=bma4;//�����յ��ı��븴�Ƶ�����Ĵ�����							  
									   rf_ok1=1;								// ֪ͨ�����ӳ�����Խ�����
									   tb_ok=0;
									   bt_auto=0;
									   s=1000;
									   break;									 
									 }
									 else
										 {
										   mmb1=bma1;mmb2=bma2;mmb3=bma3;mmb4=bma4;//���ٴν��յ��ı��븴�Ƶ�����Ĵ����У� 							
										   rf_ok2=1;								// ֪ͨ�����ӳ�����Խ�����
										   tb_ok=0;
										   bt_auto=0;
										   break;																			 
										 } 
									
								 }
					  } 
					 ma_x++;
					 
				   }
			 else {ma_x=0; tb_ok=0; bt_auto=0;bma1=0; bma2=0; bma3=0; bma4=0;hh_w=1;ll_w=0;}	  //���յ������ϵĸ�-�͵�ƽ����
			ll_w=0;hh_w=1; 
			  } 		 
		  old_rc5=1;	  // ��¼���ε�ƽ״̬
		}
	 if(rf_ok1) 	 //�涨ʱ���ڽ��ܵ�2֡��ͬ�ı������ݲ���Ч
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
	
	 if((rf_ok))	  //�ж��Ƿ���ճɹ�
			 {	 
				 //EA=0;
				 rf_ok=0;
												 
				 D3=!(mma4 & 0x80);    //ȡ������
				 D2=!(mma4 & 0x40);
				 D1=!(mma4 & 0x20);
				 D0=!(mma4 & 0x10);
	
				 decode_ok=1;
				 VT=0;		 
				 s1=1000;

				 /* ��ȡEV1527 �� */
				 ev1527_data[0] = mma1;
				 ev1527_data[1] = mma2;
				 ev1527_data[2] = mma3 | (mma4 >> 4);
			#ifdef __OS__
				msg_set_isr(MSG_GOURP_INPUT, MSG_INPUT_EV1527);
				// EA=1;
			#endif
	 	}
	
#if 0
	if(decode_ok)	//������Ч�źţ�����2272 PT��
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


