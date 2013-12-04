/*
 * can1939.c
 *
 *  Created on: Oct 31, 2013
 *      Author: konlese
 */
#include     <stdio.h>      /*标准输入输出定义*/
#include     <stdlib.h>     /*标准函数库定义*/
#include     <unistd.h>     /*Unix 标准函数定义*/
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h>      /*文件控制定义*/
#include     <termios.h>    /*PPSIX 终端控制定义*/
#include     <errno.h>      /*错误号定义*/
#include	 <string.h>
#include	 <time.h>
#include	 <signal.h>
#include     "uartlib.h"
#define C_SND 1
#define C_RCV 2
#define C_ACK 7
#define C_END 9
#define C_BEG 11
#define C_ERR 63
struct PACKHEAD
{
	short cmd;
	int len;
}pkhead;
int speed_arr[] = {B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300,//
B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {115200,38400,19200,9600,4800,2400,1200,300,
38400,19200,9600, 4800, 2400, 1200,300, };
/**
*@brief   设置串口数据位，停止位和效验位
*@param  fd     类型  int  打开的串口文件句柄
*@param  databits 类型  int 数据位   取值 为 7 或者8
*@param  stopbits 类型  int 停止位   取值为 1 或者2
*@param  parity  类型  int  效验类型 取值为N,E,O,,S
*@param  mode   类型  int  效验类型 取值为gps，1：gprs：0
*/
int set_Parity(int fd,int databits,int stopbits,int parity,int mode)
{
	struct termios options;
	if  ( tcgetattr( fd,&options)  !=  0) {
		perror("SetupSerial 1");
		return 0;
	}
	options.c_cflag &= ~CSIZE;
	switch (databits) /*设置数据位数*/
	{
		case 7:
			options.c_cflag |= CS7;
			break;
		case 8:
			options.c_cflag |= CS8;
			break;
		default:
			fprintf(stderr,"Unsupported data size\n"); return FALSE;
	}
	switch (parity)
	{
		case 'n':
		case 'N':
			options.c_cflag &= ~PARENB;   /* Clear parity enable */
			options.c_iflag &= ~INPCK;     /* Enable parity checking */
			break;
		case 'o':
		case 'O':
			options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
			options.c_iflag |= INPCK;             /* Disnable parity checking */
			break;
		case 'e':
		case 'E':
			options.c_cflag |= PARENB;     /* Enable parity */
			options.c_cflag &= ~PARODD;   /* 转换为偶效验*/
			options.c_iflag |= INPCK;       /* Disnable parity checking */
			break;
		case 'S':
		case 's':  /*as no parity*/
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;break;
		default:
			fprintf(stderr,"Unsupported parity\n");
			return (FALSE);
	}
	/* 设置停止位*/
	switch (stopbits)
	{
		case 1:
			options.c_cflag &= ~CSTOPB;
			break;
		case 2:
			options.c_cflag |= CSTOPB;
		   break;
		default:
			 fprintf(stderr,"Unsupported stop bits\n");
			 return (FALSE);
	}
/* Set input parity option */
	if (parity != 'n')
		options.c_iflag |= INPCK;
	tcflush(fd,TCIFLUSH);
	options.c_cc[VTIME] = 0; /* 设置超时15 seconds*/
//	options.c_cc[VMIN] = 1; /* Update the options and do it NOW */
	if(mode)
	{
		options.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
		options.c_cc[VMIN] = 1; /* Update the options and do it NOW */
	}
	else
	{
		options.c_lflag |= ICANON;
		options.c_cc[VMIN] = 2; /* Update the options and do it NOW */
	}
	if (tcsetattr(fd,TCSANOW,&options) != 0)
	{
		perror("SetupSerial 3");
		return (FALSE);
	}
	return (TRUE);

}

/**********************************************************************
代码说明：使用串口二测试的，发送的数据是字符，
但是没有发送字符串结束符号，所以接收到后，后面加上了结束符号。
我测试使用的是单片机发送数据到第二个串口，测试通过。
**********************************************************************/
/*********************************************************************/
int OpenDev(char *Dev)
{
    int fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (-1 == fd)
    {
        perror("Can't Open Serial Port");
        return -1;
    }
    else
        return fd;
}
void set_speed(int fd, int speed)
{
	int i;
	int status;
	struct termios Opt; //定义了这样一个结构
	tcgetattr(fd, &Opt); //用来得到机器原端口的默认设置
	for ( i= 0;i < sizeof(speed_arr) / sizeof(int);i++)
	{
		if(speed == name_arr[i]) //判断传进来是否相等
		{
			tcflush(fd, TCIOFLUSH); //刷新输入输出缓冲
			cfsetispeed(&Opt, speed_arr[i]); //这里分别设置
			cfsetospeed(&Opt, speed_arr[i]);
			status = tcsetattr(fd, TCSANOW, &Opt); //这是立刻把bote rates设置真正写到串口中去
			if(status != 0)
			perror("tcsetattr fd1"); //设置错误
			return;
		}
		tcflush(fd,TCIOFLUSH); //同上
	}
}
void read_n(int fd, void *buf, int len)
{
	int cou, ret;
	ret=0;cou=0;
	while(len>0)
	{
		ret=read(fd,(char*)buf+cou,len);
		if(ret<1)
		{
		fprintf(stderr,"read device err %s\n",strerror(errno));
		break;
		}
		cou+=ret;
		len=len-ret;
	}
}
void write_n(int fd, void *buf, int len)
{
	int cou, ret;
	ret=0;cou=0;
	while(len>0)
	{
		ret=write(fd,(char*)buf+cou,len);
		if(ret<1)
		{
			fprintf(stderr,"write device err %s\n",strerror(errno));
			break;
		}
		cou+=ret;
		len=len-ret;
	}
}




/* 时间中断处理函数 */
void sig_alarm(int sig)
{
}

/* 设超时时间 */
static void (*old_sig)();
static int sig_flag=0;

int set_time_out(int sec)
{
	if(sec<1)
	return(1);
	sig_flag=1;
	old_sig=signal(SIGALRM,sig_alarm);
	alarm(sec);
	return(0);
}

/* 设超时时间 */
int reset_time()
{
	if(sig_flag==1)
	{
		signal(SIGALRM,old_sig);
		alarm(0);
	}
	return(0);
}




