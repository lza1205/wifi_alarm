#ifndef _PRINTP_H_
#define _PRINTP_H_

#include "all.h" 

void putchar1(u8 ch);
u8 getchar1(void);

//��ʽת��������ʮ������ת��Ϊ�ַ���  eg:����0x5A�����5A    
void PUT_INT8U_HexTOString(u8 Hex);
//��ʽת��������ʮ��������ת��Ϊ�ַ��� eg������001234���������1234(ȥ����Ч0)
void PUT_INT32U_DecTOString(u32 Dec);
//���������ַ���ת����ʮ���ƣ���"123456"--123456
u32 StringTODec(void);

#define PRINTP_EN  			
#define CR_as_CRLF	TRUE         
// if true , you can use "\n" to act as CR/LF, 
// if false , you have to use "\n\r",but can get a higher speed

#ifdef PRINTP_EN
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

/*
#define va_list void*
#define va_arg(arg, type) *(type*)arg; arg = (char*)arg + sizeof(type);
#define va_start(arg, start) arg = (va_list)(((char*)&(start)) + sizeof(start))
*/

void printp( char* ctrl, ...);
u8 USART_Scanf_Name(char * str);
u8 USART_Scanf_Cmd(char * str);
u8 Equal(void* A, void* B, u8 SizeA, u8 SizeB);

#endif


#endif  //_USART_PRINTP_H_



