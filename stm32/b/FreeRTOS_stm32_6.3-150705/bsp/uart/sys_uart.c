#define __SYS_UART_C_

#include "all.h"

/************************************************************************

	串口1初始化
	
**************************************************************************/
void sys_uart1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
 	NVIC_InitTypeDef NVIC_InitStructure; 


	/* 开启时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);

	/* 设置管教 */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	
	/* 设置寄存器 */
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_Init(USART1, &USART_InitStruct);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//使能接收中断
	USART_Cmd(USART1, ENABLE);//使能串口1 

	/* 中断配置 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 	//通道设置为串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //中断占先等级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //中断响应优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //打开中断
	NVIC_Init(&NVIC_InitStructure);							  //初始化
}


void sys_uart2_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USART2 GPIO config */
	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		  
	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure); 

	/*  配置中断优先级 */
	  /* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* 使能串口2接收中断 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);


	USART_Cmd(USART2, ENABLE);

}


void sys_uart1_sendchar(uint8_t data)
{
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
	USART_SendData(USART1,data);
}


void my_send_str(u8 *str)
{
	while((*str) != '\0')
	{
		sys_uart1_sendchar((*str++));
	}
}

/* 把数字用字符串显示出来 */
void my_send_buf(u8 *buf, u32 len)
{
	u32 i;
	u8 th,tl;
	for(i = 0; i < len; i++)
	{
		th = buf[i]/16;
		tl = buf[i]%16;
		if(th < 10)
			sys_uart1_sendchar(th + 0x30);
		else
			sys_uart1_sendchar(th - 10 + 97);
		
		if(tl < 10)
			sys_uart1_sendchar(tl + 0x30);
		else
			sys_uart1_sendchar(tl - 10 + 97);
	}
}

void my_send_hex(INT32U var)
{
	int i,temp = 1,j = 1, num_tmp;
	num_tmp = var;
	for(i = 0; i < 16; i++)
	{
		temp = temp * 16;
		if(var < temp)	break;
		j++;
	}
	my_send_str("0x");
	for(i = 0; i < j; i++)
	{
		temp = temp/16;
		num_tmp = var/temp;
		var = var % temp;
		if(num_tmp < 10)
			sys_uart1_sendchar(num_tmp + 0x30);	
		else
			sys_uart1_sendchar(num_tmp - 10 + 97);
	}
}


void send_var(s8 var)
{
	int i,temp = 1,j = 1;
	u8 num_tmp;
	u8 var_tmp;
	
	if(var < 0){
		my_send_str("-");
		var_tmp = var * -1;
	}else{
		var_tmp = (u8)var;
	}
	
	for(i = 0; i < 10; i++)
	{
		temp = temp * 10;
		if(var_tmp < temp)	break;
		j++;
	}
	for(i = 0; i < j; i++)
	{
		temp = temp/10;
		num_tmp = var_tmp/temp;
		var_tmp = var_tmp % temp;
		sys_uart1_sendchar(num_tmp + 0x30);	
	}
}



void USARTx_TimeCheck(void)
{
	if(COM1_RX_TimeOut > 0)COM1_RX_TimeOut--;		//超时计数

	
	if((FlagMainUartOK == 1) && (COM1_RX_TimeOut == 0))
	{
#ifdef __OS__
		msg_set_isr(MSG_GOURP_INPUT, MSG_INPUT_UART1);
#else
		;
#endif
	}
}



void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 		
		if(COM1_RX_Cnt >= COM1_RX_Lenth-1)	
		{	
			COM1_RX_Cnt = COM1_RX_Lenth-1;
		}
		COM1_RX_Buffer[COM1_RX_Cnt++] = USART_ReceiveData(USART1);
		COM1_RX_TimeOut = TimeOutSet1;
		FlagMainUartOK = 1;		//收到串口数据


		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{     
	   USART_ClearITPendingBit(USART1, USART_IT_TXE);
	}
}




/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART2_printf()调用
 */
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */


#if 1
//中断缓存串口数据

volatile    uint8_t uart_p = 0;
uint8_t     uart_buff[UART_BUFF_SIZE];

void bsp_USART2_IRQHandler(void)
{
    if(uart_p<UART_BUFF_SIZE)
    {
        if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
        {
            uart_buff[uart_p] = USART_ReceiveData(USART2);
            uart_p++;
        }
    }
}



//获取接收到的数据和长度
char *get_rebuff(uint8_t *len)
{
    *len = uart_p;
    return (char *)&uart_buff;
}

void clean_rebuff(void)
{
    uart_p = 0;
	memset(uart_buff, 0, UART_BUFF_SIZE);
}

#endif

/*
 * 函数名：USART2_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口2，即USART2
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART2_printf( USART2, "\r\n this is a demo \r\n" );
 *            		 USART2_printf( USART2, "\r\n %d \r\n", i );
 *            		 USART2_printf( USART2, "\r\n %s \r\n", j );
 */
void USART2_printf(USART_TypeDef* USARTx, char *Data,...)
{
	const char *s;
  int d;   
  char buf[16];

  va_list ap;
  va_start(ap, Data);

	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if ( *Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //字符串
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
          }
					Data++;
          break;

        case 'd':										//十进制
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
          }
					Data++;
          break;
				 default:
						Data++;
				    break;
			}		 
		} /* end of else if */
		else USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
	}
}


