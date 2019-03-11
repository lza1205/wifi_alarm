
#ifndef __DELAY_H
#define __DELAY_H

#ifdef __DELAY_C_
#define __DELAY_EXT_
#else
#define __DELAY_EXT_ extern
#endif

#include "all.h"


__DELAY_EXT_ u8 delay_flg;


void delay_init(void);

void delay_us(u32 us);
void delay_ms(u32 us);


void delay_xus(u32 nus);
void delay_xms(u16 nms);

void  Delay10us(void);
void  Delay15us(void);
void  Delay20us(void);
void  Delay40us(void);
void  Delay50us(void);
void  Delay60us(void);
void  Delay100us(void);
void  Delay150us(void);
void  Delay200us(void);
void  Delay_10us(unsigned char x_10us);
void  Delay_100us(unsigned char x_100us);
void  Delay_1ms(unsigned char x_1ms);
void  Delay_10ms(unsigned char x_10ms);


#endif

