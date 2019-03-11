
#ifndef __SYS_UART_H_
#define __SYS_UART_H_

#ifdef __SYS_UART_C_
#define __SYS_UART_EXT_
#else
#define __SYS_UART_EXT_ extern
#endif


#define	TimeOutSet1		2
#define	TimeOutSet2		2

#define COM2_RX_Lenth	1024
#define COM1_RX_Lenth	1024

#define UART_BUFF_SIZE      255


__SYS_UART_EXT_ u16 COM1_RX_Cnt;
__SYS_UART_EXT_ u8 COM1_RX_Buffer[COM1_RX_Lenth];
__SYS_UART_EXT_ u8 COM1_RX_TimeOut;
__SYS_UART_EXT_ u8 FlagMainUartOK;


__SYS_UART_EXT_ volatile    uint8_t uart_p;
__SYS_UART_EXT_ uint8_t     uart_buff[UART_BUFF_SIZE];


void sys_uart1_init(void);

void sys_uart1_sendchar(uint8_t data);
void my_send_str(u8 *str);
void send_var(s8 var);
void sys_uart2_init(void);
void USARTx_TimeCheck(void);



void clean_rebuff(void);
char *get_rebuff(uint8_t *len);
void USART2_printf(USART_TypeDef* USARTx, char *Data,...);


#endif

