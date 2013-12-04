/*
 * lcd.c
 *
 *  Created on: Nov 21, 2013
 *      Author: konlese
 */
#include"lcd485.h"
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

///////define the gobal varible/////////
#define MAXBUF 255
int fd,keyfd;
DProcess_Var dprocessvar;
unsigned char Lcdsentbuf[MAXBUF];
unsigned char dir[12];

const MENU_NODE list0={1,-1,-1,6,8};
const MENU_NODE list1[6]={
		{2,0,0,8,1},{3,0,0,1,2},{4,0,0,1,3},
		{-1,0,0,-1,4},{-1,0,0,-1,5},{-1,0,0,-1,6}
};
const MENU_NODE list2[8]={
		{-1,1,0,-1,7},{-1,1,0,-1,10},{-1,1,0,-1,10},{-1,1,0,-1,11},
		{-1,1,0,-1,12},{-1,1,0,-1,13},{-1,-1,-1,-1,14},{-1,-1,-1,15}
};
const MENU_NODE list3={	-1,1,1,-1,16 };
const MENU_NODE list4={ -1,1,2,-1,16 };

const unsigned char icon[16][12]=
{
	{0x00,0x00,0x00,0x00,0x00,0x43,0x00,0x43,0x00,0x2d,0x00,0x0f},//gps icon
	{0x00,0x64,0x00,0x00,0x00,0xa7,0x00,0x43,0x00,0x2d,0x00,0x0f},//

    {0x00,0xc8,0x00,0x00,0x01,0x0b,0x00,0x43,0x00,0x8e,0x00,0x0f},//gprs icon
	{0x01,0x2c,0x00,0x00,0x01,0x6f,0x00,0x43,0x00,0x8e,0x00,0x0f},//

	{0x01,0x90,0x00,0x00,0x01,0xd3,0x00,0x43,0x00,0xef,0x00,0x0f},//net icon
	{0x01,0xf4,0x00,0x00,0x02,0x37,0x00,0x43,0x00,0xef,0x00,0x0f},//

	{0x02,0x58,0x00,0x00,0x02,0x9b,0x00,0x43,0x01,0x50,0x00,0x0f},//run icon
	{0x02,0xbc,0x00,0x00,0x02,0xff,0x00,0x43,0x01,0x50,0x00,0x0f},//

	{0x00,0x00,0x00,0x64,0x00,0x52,0x00,0xa7,0x01,0xb1,0x00,0x0f},//message icon
	{0x00,0x00,0x00,0x64,0x00,0x52,0x00,0xa7,0x01,0xb1,0x00,0x0f},//

	{0x00,0x00,0x00,0xc8,0x00,0x43,0x01,0x0b,0x02,0x21,0x00,0x0f},//warn icon
	{0x00,0x00,0x00,0xc8,0x00,0x43,0x01,0x0b,0x02,0x21,0x00,0x0f},//

	{0x00,0xc8,0x00,0x64,0x01,0x2c,0x00,0xa7,0x02,0x82,0x00,0x0f},//control icon
	{0x01,0x2c,0x00,0x64,0x01,0x90,0x00,0xa7,0x02,0x82,0x00,0x0f},//

	{0x00,0xc8,0x00,0xc8,0x01,0x1a,0x01,0x1a,0x00,0xbc,0x00,0x64},//control icon
	{0x01,0x2c,0x00,0xc8,0x01,0x7e,0x01,0x1a,0x00,0xbc,0x00,0x64}//
};
const unsigned char diricon[2][8] =
{

	{0x00,0x00,0x01,0x2c,0x00,0x3e,0x01,0x66},//dir
	{0x00,0x64,0x01,0x2c,0x00,0xa2,0x01,0x66} //clr

};
const unsigned char mainxy[8][4] =
{
	{0x01,0x50,0x00,0x64 },
	{0x01,0x50,0x00,0x96 },
	{0x01,0x50,0x00,0xc8 },
	{0x01,0x50,0x00,0xfa },
	{0x01,0x50,0x01,0x2c },
	{0x01,0x50,0x01,0x5e },
	{0x01,0x50,0x01,0x90 },
	{0x02,0xe4,0x00,0x64 },
};
const unsigned char keyxy[4]={0x01,0x36,0x00,0x46};

//const unsigned char textmainxy[][] = {};
const unsigned char gpsxy[6][4]={

	{0x00,0xaa,0x00,0x6e},
	{0x00,0xaa,0x00,0xb4},
	{0x00,0xaa,0x00,0xfa},
	{0x01,0xc2,0x00,0x8c},
	{0x01,0xc2,0x00,0xb4},
	{0x02,0x1c,0x00,0xfa},
};

void lcd485sentCmd(unsigned char displaycmd,char picid,unsigned char *buf,unsigned char sentsize)
{
	unsigned char size,i;
	Lcdsentbuf[0] = 0xaa;
	Lcdsentbuf[1] = displaycmd;
	if(displaycmd <PIC_ALL)
	{
		size =sentsize+6;
		for(i =0;i<sentsize;i++)
		{
			Lcdsentbuf[i+2] = buf[i];
		}
	}
	else
	{
		Lcdsentbuf[2] = picid;
		for(i =0;i<sentsize;i++)
		{
			Lcdsentbuf[i+3] = buf[i];
		}
		size =sentsize+7;
	}
	Lcdsentbuf[size-1]= 0x3c;
	Lcdsentbuf[size-2]= 0xc3;
	Lcdsentbuf[size-3]= 0x33;
	Lcdsentbuf[size-4]= 0xcc;

	write (fd, &Lcdsentbuf ,size);
}
void diplayPIC1(DProcess_Status *pstatus,unsigned char picid)
{
	unsigned char i;
	if(pstatus->changePic ==1)
	{
		pstatus->changePic = 0;
		pstatus->signalStatus.singalchange = 0;
		//刷新图片
		lcd485sentCmd(0x70,picid,NULL,0);
		//刷新图标
		for(i = 0;i<8;i++)
		{
			lcd485sentCmd(0x71,9,(unsigned char *)icon[2*i+1],12);
		}
		//刷新内容
	}
	else if(pstatus->signalStatus.singalchange ==1)
	{
		pstatus->signalStatus.singalchange =0;
		//刷新图表
	}
	else
	{
		//刷新内容
	}

}
void diplayPIC2to6(DProcess_Status *pstatus,unsigned char picid)
{
	unsigned char i;
	if(pstatus->changePic ==1)
	{
		pstatus->changePic = 0;
		pstatus->changenode =0;
		//刷新图片
		lcd485sentCmd(0x70,picid,NULL,0);
		//刷新指示图标
		if(picid == PIC01)
		{
			memcpy(dir,diricon[0],8);
			for(i=0;i<4;i++)
			{
				dir[8+i]= mainxy[pstatus->lcdtcb.chiNb][i];
			}
			lcd485sentCmd(0x71,9,dir,12);
		}

	}
	else if(pstatus->changenode ==1)
	{
		pstatus->changenode =0;
		if(picid == PIC01)
		{
			memcpy(dir,diricon[1],8);
			for(i=0;i<4;i++)
			{
				dir[8+i]= mainxy[pstatus->lcdtcb.chiNbbak][i];
			}
			lcd485sentCmd(0x71,9,dir,12);
			memcpy(dir,diricon[0],8);
			for(i=0;i<4;i++)
			{
				dir[8+i]= mainxy[pstatus->lcdtcb.chiNb][i];
			}
			lcd485sentCmd(0x71,9,dir,12);

		}
		//刷新指示图表
	}

}
void diplayPICa6(DProcess_Status *pstatus,unsigned char picid)
{
	if(pstatus->changePic ==1)
	{
		pstatus->changePic = 0;
		lcd485sentCmd(0x70,picid,NULL,0);
		//刷新图片
	}
	else
	{
		//刷新内容
	}


}

void DispalyHandler(DProcess_Status *pstatus,NODE_LIST *nodelist)
{
	unsigned char picid;
	picid = nodelist[pstatus->lcdtcb.ListNb].ListPt[pstatus->lcdtcb.NodeNb].picId;
	switch(picid)
	{
		case PIC08:
			diplayPIC1(pstatus,picid);
			break;
		case PIC01:
		case PIC02:
		case PIC03:
		case PIC04:
		case PIC05:
		case PIC06:
			diplayPIC2to6(pstatus,picid);
			break;
		case PIC07:
		case PIC09:
		case PIC10:
		case PIC11:
		case PIC12:
		case PIC13:
		case PIC14:
		case PIC15:
		case PIC16:
		case PIC17:
		case PIC18:
			diplayPICa6(pstatus,picid);
			break;
		default:
			break;
	}
}

void Initvarible(void *funhandler)
{

		dprocessvar.NodeListTable[0].ListPt = (MENU_NODE *)&list0;
		dprocessvar.NodeListTable[0].NodeNumber =1;
		dprocessvar.NodeListTable[1].ListPt = (MENU_NODE *)list1;
		dprocessvar.NodeListTable[1].NodeNumber = 6;
		dprocessvar.NodeListTable[2].ListPt = (MENU_NODE *)list2;
		dprocessvar.NodeListTable[2].NodeNumber = 8;
		dprocessvar.NodeListTable[3].ListPt = (MENU_NODE *)&list3;
		dprocessvar.NodeListTable[3].NodeNumber = 1;
		dprocessvar.NodeListTable[4].ListPt = (MENU_NODE *)&list4;
		dprocessvar.NodeListTable[4].NodeNumber = 1;
		dprocessvar.handler = funhandler;
		dprocessvar.DProcessStatus.changePic = 1;

}
void DisplayThread(void* arg)
{
	while(1)
	{
		dprocessvar.handler(&dprocessvar.DProcessStatus,dprocessvar.NodeListTable);
		sleep(1);
	}
}
void init_Lcd(int fd)
{
	//write (int __fd, __const void *__buf, size_t __n);
}
int main(int argc, char* argv[])
{
	int iret,nread;
	char c;
	pthread_t id1;
	LCDTCB lcdtcb;
	unsigned char upicid;
	keyfd = open("/dev/tty",O_RDONLY|O_NONBLOCK);//模拟按键
	if((fd=open("/dev/ttyUSB0",O_RDWR|O_NOCTTY))==-1)//|O_NONBLOCK|O_NDELAY))==-1)
	{
		printf("Can't open serial1 port.");
		return -1;
	}
	else
	{
		if(set_Parity(fd,8,1,'n',1)==FALSE)
			printf("\nset_Parity is failed!\n");

		set_speed(fd,115200);

		//getthestatus();
		Initvarible(DispalyHandler);
	//	init_Lcd(fd);
		iret = pthread_create(&id1, NULL, (void*)DisplayThread, NULL);
		if(iret != 0) {
			printf("\nthread1_create is failed!\n");
		}
	}

	while(1)
	{

		//dprocessvar.handler(dprocessvar.DProcessStatus,dprocessvar.NodeListTable);
		nread=read(keyfd,&c,1);
		switch (c)
		{
			case 'c'://cancel
				if(dprocessvar.DProcessStatus.lcdtcb.ListNb!= 0)
				{
					lcdtcb.ListNb  = dprocessvar.DProcessStatus.lcdtcb.ListNb;
					lcdtcb.NodeNb = dprocessvar.DProcessStatus.lcdtcb.NodeNb;
					dprocessvar.DProcessStatus.lcdtcb.chiNb = lcdtcb.NodeNb;
					dprocessvar.DProcessStatus.lcdtcb.ListNb =
					dprocessvar.NodeListTable[lcdtcb.ListNb].ListPt[lcdtcb.NodeNb].FatherList;
					dprocessvar.DProcessStatus.lcdtcb.NodeNb =
										dprocessvar.NodeListTable[lcdtcb.ListNb].ListPt[lcdtcb.NodeNb].FatherNB;
					dprocessvar.DProcessStatus.changePic = 1;
					dprocessvar.handler(&dprocessvar.DProcessStatus,dprocessvar.NodeListTable);
				}
				break;
			case 'e'://enter
				if(dprocessvar.DProcessStatus.lcdtcb.ListNb!= 0)
				{
					lcdtcb.ListNb  = dprocessvar.DProcessStatus.lcdtcb.ListNb;
					lcdtcb.NodeNb = dprocessvar.DProcessStatus.lcdtcb.NodeNb;
					if(dprocessvar.NodeListTable[lcdtcb.ListNb].ListPt[lcdtcb.NodeNb].ChildrenList!=-1)
					{
						dprocessvar.DProcessStatus.lcdtcb.ListNb =
						dprocessvar.NodeListTable[lcdtcb.ListNb].ListPt[lcdtcb.NodeNb].ChildrenList;
						dprocessvar.DProcessStatus.lcdtcb.NodeNb = dprocessvar.DProcessStatus.lcdtcb.chiNb;
						dprocessvar.DProcessStatus.lcdtcb.chiNb=0;
						dprocessvar.DProcessStatus.changePic = 1;
						dprocessvar.handler(&dprocessvar.DProcessStatus,dprocessvar.NodeListTable);
					}
				}
				break;
			case 'u'://uper
				if(dprocessvar.DProcessStatus.lcdtcb.ListNb!= 0)
				{
					upicid = dprocessvar.NodeListTable[ dprocessvar.DProcessStatus.lcdtcb.ListNb].ListPt[dprocessvar.DProcessStatus.lcdtcb.NodeNb].picId;
					switch (upicid)
					{
						case PIC01:
							if((dprocessvar.DProcessStatus.lcdtcb.chiNb !=0)&&(dprocessvar.DProcessStatus.lcdtcb.chiNb !=7))
							{
								dprocessvar.DProcessStatus.lcdtcb.chiNbbak = dprocessvar.DProcessStatus.lcdtcb.chiNb;
								dprocessvar.DProcessStatus.lcdtcb.chiNb--;
								dprocessvar.DProcessStatus.changenode = 1;
								dprocessvar.handler(&dprocessvar.DProcessStatus,dprocessvar.NodeListTable);
							}
							break;
						default:
							break;

					}
				}
				break;
			case 'l'://left
				if(dprocessvar.DProcessStatus.lcdtcb.ListNb!= 0)
				{
					upicid = dprocessvar.NodeListTable[ dprocessvar.DProcessStatus.lcdtcb.ListNb].ListPt[dprocessvar.DProcessStatus.lcdtcb.NodeNb].picId;
					switch (upicid)
					{
						case PIC01:
							if(dprocessvar.DProcessStatus.lcdtcb.chiNb ==7)
							{
								dprocessvar.DProcessStatus.lcdtcb.chiNbbak = dprocessvar.DProcessStatus.lcdtcb.chiNb;
								dprocessvar.DProcessStatus.changenode = 1;
								dprocessvar.DProcessStatus.lcdtcb.chiNb=0;
								dprocessvar.handler(&dprocessvar.DProcessStatus,dprocessvar.NodeListTable);
							}
							break;
						default:
							break;

					}
				}
				break;
			case 'd'://down
				if(dprocessvar.DProcessStatus.lcdtcb.ListNb!= 6)
				{
					upicid = dprocessvar.NodeListTable[ dprocessvar.DProcessStatus.lcdtcb.ListNb].ListPt[dprocessvar.DProcessStatus.lcdtcb.NodeNb].picId;
					switch (upicid)
					{
						case PIC01:
							if((dprocessvar.DProcessStatus.lcdtcb.chiNb !=7)&&(dprocessvar.DProcessStatus.lcdtcb.chiNb !=6))
							{
								dprocessvar.DProcessStatus.lcdtcb.chiNbbak = dprocessvar.DProcessStatus.lcdtcb.chiNb;
								dprocessvar.DProcessStatus.lcdtcb.chiNb++;
								dprocessvar.DProcessStatus.changenode = 1;
								dprocessvar.handler(&dprocessvar.DProcessStatus,dprocessvar.NodeListTable);
							}
							break;
						default:
							break;

					}
				}
				break;
			case 'r'://right
				if(dprocessvar.DProcessStatus.lcdtcb.ListNb!= 6)
				{
					upicid = dprocessvar.NodeListTable[ dprocessvar.DProcessStatus.lcdtcb.ListNb].ListPt[dprocessvar.DProcessStatus.lcdtcb.NodeNb].picId;
					switch (upicid)
					{
						case PIC01:
							if(dprocessvar.DProcessStatus.lcdtcb.chiNb ==0)
							{
								dprocessvar.DProcessStatus.lcdtcb.chiNbbak = dprocessvar.DProcessStatus.lcdtcb.chiNb;
								dprocessvar.DProcessStatus.lcdtcb.chiNb=7;
								dprocessvar.DProcessStatus.changenode = 1;
								dprocessvar.handler(&dprocessvar.DProcessStatus,dprocessvar.NodeListTable);
							}
							break;
						default:
							break;

					}
				}
				break;
			case '1'://f1
			case '2'://f2
			case '3'://f3
			case '4'://f4
			case '5'://f5
			case '6'://f6
				if(dprocessvar.DProcessStatus.lcdtcb.ListNb== 0)
				{
					dprocessvar.DProcessStatus.lcdtcb.chiNb = 0;
					dprocessvar.DProcessStatus.changePic = 1;
					dprocessvar.DProcessStatus.lcdtcb.ListNb = 1;
					switch (c)
					{
						case '1'://f1
							dprocessvar.DProcessStatus.lcdtcb.NodeNb = 0;
							break;
						case '2'://f2
							dprocessvar.DProcessStatus.lcdtcb.NodeNb = 1;
							break;
						case '3'://f3
							dprocessvar.DProcessStatus.lcdtcb.NodeNb = 2;
							break;
						case '4'://f4
							dprocessvar.DProcessStatus.lcdtcb.NodeNb = 3;
							break;
						case '5'://f5
							dprocessvar.DProcessStatus.lcdtcb.NodeNb = 4;
							break;
						case '6'://f6
							dprocessvar.DProcessStatus.lcdtcb.NodeNb = 5;
							break;
						default:
							break;
					}
					dprocessvar.handler(&dprocessvar.DProcessStatus,dprocessvar.NodeListTable);
				}
				break;

			default:
				break;

		}


	}


	close(fd);
return 0;
}

