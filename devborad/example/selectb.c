/*
 * selectb.c
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
	int fd,keyboard;
	int ret,i;
	char wbuf[8]={"12345678"};
	char c;
	fd_set readfd;
	struct timeval timeout;
	keyboard = open("/dev/tty",O_RDONLY|O_NONBLOCK);
	fd = open("/home/konlese/abc.txt", O_RDWR|O_NONBLOCK, 0777);
	assert(fd>0);
	assert(keyboard>0);
	while(1)
	{
		i=read(keyboard,&c,1);
		if('w'==c)
		{
			printf("hehethe input is %c/n",c);
			ret = write(fd,wbuf,8);
			printf("ret= %d/n",ret);
		}
	    if ('q'==c)
			 break;
	}
}

