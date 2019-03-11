


/***********************************************************
文件名	:	uart.c
作者		:	Faker
版本号	:	1.0
日期		:	2015.05.13

说明:
	串口，用于跟副处理器(STM32等单片机)通信

***********************************************************/


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

#include "all.h"
#include "client.h"

int tty_fd;


int max(int a,int b)
{
	return a>b?a:b;
}


int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
    if  ( tcgetattr( fd,&oldtio)  !=  0) 
    { 
        perror("SetupSerial 1");
        return -1;
    }
    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag  |=  CLOCAL | CREAD; 
    newtio.c_cflag &= ~CSIZE; 

    switch( nBits )
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }

    switch( nEvent )
    {
    case 'O':                     //奇校验
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':                     //偶校验
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':                    //无校验
        newtio.c_cflag &= ~PARENB;
        break;
    }

	switch( nSpeed )
    {
    case 2400:
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }
    if( nStop == 1 )
    {
        newtio.c_cflag &=  ~CSTOPB;
    }
    else if ( nStop == 2 )
    {
        newtio.c_cflag |=  CSTOPB;
    }
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;
    }
    printf("set done!\n");
    return 0;
}

int open_port(int fd,int comport)
{
//    char *dev[]={"/dev/ttyS0","/dev/ttyS1","/dev/ttyS2"};
//    long  vdisable;
    if (comport==1)
    {   
    	printf("open /dev/ttyS0");
    	fd = open( "/dev/ttyS0", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd)
        {
            perror("Can't Open Serial Port");
            return(-1);
        }
        else 
        {
            printf("open ttyS0 .....\n");
        }
    }
    else if(comport==2)
    {    fd = open( "/dev/ttyS1", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd)
        {
            perror("Can't Open Serial Port");
            return(-1);
        }
        else 
        {
            printf("open ttyS1 .....\n");
        }    
    }
    else if (comport==3)
    {
        fd = open( "/dev/ttyS2", O_RDWR|O_NOCTTY|O_NDELAY);
        if (-1 == fd)
        {
            perror("Can't Open Serial Port");
            return(-1);
        }
        else 
        {
            printf("open ttyS2 .....\n");
        }
    }
    if(fcntl(fd, F_SETFL, 0)<0)
    {
        printf("fcntl failed!\n");
    }
    else
    {
        printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
    }
    if(isatty(STDIN_FILENO)==0)
    {
        printf("standard input is not a terminal device\n");
    }
    else
    {
        printf("isatty success!\n");
    }
    printf("fd-open=%d\n",fd);
    return fd;
}


void *uart_thread(void *pdata)
{
    int maxfd;
	fd_set rd; 
    int nread,i;
    char tmp_buff[8];
	int offset;
	char recvbuf[1024];
	
	struct timeval timeout={0,50000}; 
	int timeout_flg = 0;
	int recv_flg = 0;

    if((tty_fd=open_port(tty_fd,1))<0)
    {
        perror("open_port error");
        return NULL;
    }
    if((i=set_opt(tty_fd,9600,8,'N',1))<0)
    {
        perror("set_opt error");
        return NULL;
    }
    printf("fd=%d\n",tty_fd);
	
	FD_ZERO(&rd);

	offset = 0;


	for(;;){
		FD_SET(tty_fd, &rd);

		maxfd = tty_fd + 1;

		timeout.tv_sec = 0;
		timeout.tv_usec = 20000;
		select(maxfd, &rd, NULL, NULL, &timeout);
		timeout_flg = 1;
		
		//offset = 0;
		if(FD_ISSET(tty_fd, &rd)){
			while((nread = read(tty_fd, tmp_buff, sizeof(tmp_buff))) > 0)  
			{
//				printf("%s", tmp_buff);
				if((offset + nread) > 1024)
					offset = 0;

				memcpy(&recvbuf[offset], tmp_buff, nread);
				offset = offset + nread;
				memset(tmp_buff, 0 , sizeof(tmp_buff));
				timeout_flg = 0;
				recv_flg = 1;
			}
			//printf("%s\n", recvbuf);
		}
		else if(timeout_flg == 1){
			if(recv_flg == 1){
				
				printf("%s\r\n", recvbuf);

				/* 处理事件 */
				uart_recv_process(recvbuf, offset);

				recv_flg = 0;
				offset = 0;
				memset(recvbuf, 0 , sizeof(recvbuf));
			}
		}
	}

	close (tty_fd);  
	return NULL;  

}


int uart_init(void)
{
	pthread_t uart_thread_t;
    pthread_attr_t uart_thread_attr;

    pthread_attr_init(&uart_thread_attr);		//初始化进程属性
    pthread_attr_setdetachstate(&uart_thread_attr, PTHREAD_CREATE_DETACHED);
    if (pthread_create(&uart_thread_t, &uart_thread_attr, uart_thread, NULL) < 0)
    {
        perror("pthread_create uart_thread");
		return -1;
	}
	return 0;
}

