
#include"uartlib.h"
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
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <pthread.h>

#define NUM 1024
int fd;
int main(int argc, char* argv[])
{
	int iret;
	int nread, i;
		int rcount = 0;
		unsigned char buf[100];
	pthread_t id1, id2;
	if((fd=open("/dev/ttyUSB0",O_RDWR|O_NOCTTY))==-1)//|O_NONBLOCK|O_NDELAY))==-1)
	{
		printf("Can't open serial1 port.");
		return -1;
	}
	if(set_Parity(fd,8,1,'o',1)==FALSE)
		printf("\nset_Parity is failed!\n");

	set_speed(fd,115200);

	while(1)
	{


				printf("\nnow it begain to read words.\n");
				nread = read(fd, buf,1);
				printf("\nnread=%d\n", nread);
				if(nread )
				{
					for(i = 0; i < nread; i++)
						printf("%c", buf[i]);
					rcount++;
					//sleep(1);

			}

	}


	close(fd);
return 0;
}


