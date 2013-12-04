/*
 * selecta.c
 *
 *  Created on: Nov 12, 2013
 *      Author: konlese
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
int main ()
{
	int fd;
	int ret,i;
	char buf[20];
	fd_set readfd;
	struct timeval timeout;
	fd = open("/home/konlese/abc.txt",O_RDONLY | O_NONBLOCK);
	//fd = open("/dev/tty",O_RDONLY|O_NONBLOCK);
	assert(fd>0);
	while(1)
	{
		  timeout.tv_sec=5;
		  timeout.tv_usec=0;
		  FD_ZERO(&readfd);
		  FD_SET(fd,&readfd);
		  ret=select(fd+1,&readfd,NULL,NULL,&timeout);
		  printf("fd = %d/n",fd);
		  //select error when ret = -1
		  printf("ret= %d/n",ret);
		  if (ret == -1)
			  perror("select error");
		  //data coming when ret>0
		  else if (ret)
		  {
			  if(FD_ISSET(fd,&readfd))
			  {
				  i=read(fd,buf,1);
				  printf("hehethe input is %d/n",i);

			  }
		  }
		  //time out when ret = 0
		  else if (ret == 0)
			  printf("time out/n");
	}
}

