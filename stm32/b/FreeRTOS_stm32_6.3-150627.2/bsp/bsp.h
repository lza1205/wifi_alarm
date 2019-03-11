
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


//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
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
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define	UART1	1
#define	UART2	2




/**********************************************************/

//LED端口定义
#define port_Led1          PCout(9)// PC9
#define port_Led2          PCout(8)// PC8
#define port_Led_PWM       PAout(8)// PA8

#define Port_Key_ZDKG      PBin(5)// PB5
#define Port_Key_Reset     PBin(1)// PB5

#define Port_Buzzen        PAout(0)// PA0
#define Port_LCD_Back      PAout(6)// PA6 

#define Port_ChongDian     PAout(5)// PA5,充电端口 

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


