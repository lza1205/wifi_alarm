#define __SYS_UART_C_

#include "all.h"

/************************************************************************

	����1��ʼ��
	
**************************************************************************/
void sys_uart1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
 	NVIC_InitTypeDef NVIC_InitStructure; 


	/* ����ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);

	/* ���ùܽ� */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	
	/* ���üĴ��� */
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	
	USART_Init(USART1, &USART_InitStruct);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//ʹ�ܽ����ж�
	USART_Cmd(USART1, ENABLE);//ʹ�ܴ���1 

	/* �ж����� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 	//ͨ������Ϊ����1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //�ж�ռ�ȵȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //�ж���Ӧ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //���ж�
	NVIC_Init(&NVIC_InitStructure);							  //��ʼ��
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

	/*  �����ж����ȼ� */
	  /* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* ʹ�ܴ���2�����ж� */
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

/* ���������ַ�����ʾ���� */
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
	if(COM1_RX_TimeOut > 0)COM1_RX_TimeOut--;		//��ʱ����

	
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
		FlagMainUartOK = 1;		//�յ���������


		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{     
	   USART_ClearITPendingBit(USART1, USART_IT_TXE);
	}
}




/*
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
 * ����  ����USART2_printf()����
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
//�жϻ��洮������

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



//��ȡ���յ������ݺͳ���
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
 * ��������USART2_printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ��������ֻ�õ��˴���2����USART2
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��USART2_printf( USART2, "\r\n this is a demo \r\n" );
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

	while ( *Data != 0)     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //�س���
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //���з�
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
				case 's':										  //�ַ���
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
          }
					Data++;
          break;

        case 'd':										//ʮ����
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


