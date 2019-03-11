
#ifndef __BSP_H_
#define __BSP_H_

#ifdef __BSP_C_
#define __BSP_EXT_
#else
#define __BSP_EXT_ extern
#endif

#include "all.h"

#include "sys_time.h"
#include "delay.h"
#include "sys_uart.h"


//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����

#define	UART1	1
#define	UART2	2




/**********************************************************/

//LED�˿ڶ���
#define port_Led1          PCout(9)// PC9
#define port_Led2          PCout(8)// PC8
#define port_Led_PWM       PAout(8)// PA8

#define Port_Key_ZDKG      PBin(5)// PB5
#define Port_Key_Reset     PBin(1)// PB5

#define Port_Buzzen        PAout(0)// PA0
#define Port_LCD_Back      PAout(6)// PA6 

#define Port_ChongDian     PAout(5)// PA5,���˿� 

#define Port_AC_DET        PCin(2)

#define port_SPI_CS        PBout(12)// 
#define port_SPI_CLK       PBout(13)// 

#define Port_Siren         PAout(11)// 
#define Port_386PWD        PAout(12)// 

#define Port_LedAway       PEout(3)// 
#define Port_LedHome       PEout(3)// 
#define Port_LedAlarm      PEout(3)// 

#define Port_SW_Audio      PBout(10)//
#define Port_SW_FMQ        PBout(3)// 
#define Port_SW_PwmSPK     PBout(4)// 
#define Port_VOL1          PBout(8)// 
#define Port_VOL2          PBout(9)// 
#define Port_SW_Duijiang   PCout(0)//

#define Port_2822_mute     PCout(3)//

#define Test_portA6        PAout(6)// PA6

#define Port_MIC_PWD       PAout(7)// PA0

#define port_TEST        PBout(7)//

/**********************************************************/


__BSP_EXT_ u8  InterEvent;


void EXTIX_Switch(u8 EXTI_Line, u8 type);
void bsp_init(void);




#endif


