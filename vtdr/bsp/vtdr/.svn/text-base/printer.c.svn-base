
//*----------------------------------------------------------------------------
//*      LCD�˵��л�
//*----------------------------------------------------------------------------
//* File Name           : printer.c
//* Object              : ʵ�ִ�ӡ����
//*
//* 1.0 10/04/03 myw : Creation
//*----------------------------------------------------------------------------
#include "printer.h"
#include "application.h"
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>

#define PRN_CMDE_CTR (GPIOE)
#define PRN_EN    GPIO_Pin_1
#define PRN_PWR   GPIO_Pin_7
#define PRN_STB1  GPIO_Pin_3
#define PRN_STB2  GPIO_Pin_4
#define PRN_PAPER GPIO_Pin_5

#define PRN_CMDB_CTR (GPIOB)
#define PRN_MCSTOP    GPIO_Pin_0
#define PRN_MC1       GPIO_Pin_8
#define PRN_MC2       GPIO_Pin_9
#define PRN_LAT       GPIO_Pin_1
#define PRN_MOSI      GPIO_Pin_15
#define PRN_SCK       GPIO_Pin_13
#define PRN_MISO      GPIO_Pin_14
#define PRN_CMDC_CRT (GPIOC)
#define PRN_THER      GPIO_PIN_0


extern CLOCK curTime;
extern StructPara Parameter;
extern PartitionTable pTable;
unsigned short print_buf[32][24];


//const unsigned short     line1[]={0xb5b3,0xc6c5,0xc5ba,0xebc2,0xbaa3};//���ƺ��룺
//const unsigned short     line2[]={0xb5b3,0xc6c5,0xd6b7,0xe0c0,0xbaa3};//���Ʒ��ࣺ
//const unsigned short	 line3[]={0xddbc,0xbbca,0xb1d4,0xfab4,0xebc2,0xbaa3};//��ʻԱ���룺
//const unsigned short	 line4[]={0xddbc,0xbbca,0xa4d6,0xc5ba,0xebc2,0xbaa3};//��ʻ֤����:
//const unsigned short	 line5[]={0xf2b4,0xa1d3,0xb1ca,0xe4bc,0xbaa3};//��ӡʱ��:
//const unsigned short	 line60[]={0xa3cd,0xb5b3,0xb0c7,0x3531,0xd6b7,0xbdc6,0xf9be,0xd9cb,0xc8b6,0xafa3,0xd6b7,0xbaa3};//ͣ��ǰ15��ƽ���ٶ�/�֣�
//const unsigned short	 line8[]={0xa3c6,0xcdc0,0xddbc,0xbbca,0xc7bc,0xbcc2,0xbaa3};//ƣ�ͼ�ʻ��¼��
//const unsigned short	 line8k[]={0xaabf,0xbcca,0xb1ca,0xe4bc,0xbaa3};//��ʼʱ�䣺
//const unsigned short	 line8j[]={0xe1bd,0xf8ca,0xb1ca,0xe4bc,0xbaa3};//����ʱ�䣺
//const unsigned short     unknown[]={0xb4ce,0xaad6};//δ֪

//*----------------------------------------------------------------------------
//* Function Name       : TimeToASCII()  //myw
//* Object              : ��ǰʱ��ת����ASCII
//* Input Parameters    : curTime only to print time;
//* Output Parameters   : <ASCII[]>��ŵ�ǰʱ��ת����ASCII
//*���õ�ȫ�ֱ���       : curTime
//*�޸ĵ�ȫ�ֱ���       : None
//*----------------------------------------------------------------------------
void TimeToASCII(unsigned char *ASCII)
{

	ASCII[0]=0x32;
	ASCII[1]=0x30;//20
	
	ASCII[2]=((curTime.year&0xf0)>>4)+0x30;//shiwei
	ASCII[3]=(curTime.year&0x0f)+0x30;//gewei
	ASCII[4]=0x2f;// /

	ASCII[5]=((curTime.month&0xf0)>>4)+0x30;//shiwei
	ASCII[6]=(curTime.month&0x0f)+0x30;//gewei
	ASCII[7]=0x2f;// /

	ASCII[8]=((curTime.day&0xf0)>>4)+0x30;//shiwei
	ASCII[9]=(curTime.day&0x0f)+0x30;//gewei
	ASCII[10]=0x20;// /
	
	ASCII[11]=((curTime.hour&0xf0)>>4)+0x30;//shiwei
	ASCII[12]=(curTime.hour&0x0f)+0x30;//gewei
	ASCII[13]=0x3a;//:
	
	ASCII[14]=((curTime.minute&0xf0)>>4)+0x30;//shiwei
	ASCII[15]=(curTime.minute&0x0f)+0x30;//gewei
	ASCII[16]=0x3a;//:
	
	ASCII[17]=((curTime.second&0xf0)>>4)+0x30;//shiwei
	ASCII[18]=(curTime.second&0x0f)+0x30;//gewei

}
//*----------------------------------------------------------------------------
//* Function Name       : TiredToASCII()  //myw
//* Object              :buf[]���ת����ASCII
//* Input Parameters    : ƣ��ʱ�� only to print time;
//* Output Parameters   : <ASCII[]>���buf[]ת����ASCII
//*���õ�ȫ�ֱ���       : none
//*�޸ĵ�ȫ�ֱ���       : None
//*----------------------------------------------------------------------------
void TiredToASCII(unsigned char *ASCII,unsigned char *buf)
{
	ASCII[0]=0x32;
	ASCII[1]=0x30;//20
	//  / ��
	ASCII[2]=((buf[0]&0xf0)>>4)+0x30;//shiwei
	ASCII[3]=(buf[0]&0x0f)+0x30;//gewei
	ASCII[4]=0x2f;
//  /��	
	ASCII[5]=((buf[1]&0xf0)>>4)+0x30;//shiwei
	ASCII[6]=(buf[1]&0x0f)+0x30;//gewei
	ASCII[7]=0x2f;
//��	
	ASCII[8]=((buf[2]&0xf0)>>4)+0x30;//shiwei
	ASCII[9]=(buf[2]&0x0f)+0x30;//gewei
	ASCII[10]=0x1b;  //��2��
	ASCII[11]=0x66;//shiwei
	ASCII[12]=0x0;
	ASCII[13]=0x2;//gewei
//  :	
	ASCII[14]=((buf[3]&0xf0)>>4)+0x30;//shiwei
	ASCII[15]=(buf[3]&0x0f)+0x30;//gewei
	ASCII[16]=0x3a;
	
	ASCII[17]=((buf[4]&0xf0)>>4)+0x30;//shiwei
	ASCII[18]=(buf[4]&0x0f)+0x30;//gewei
}
//*----------------------------------------------------------------------------
//* Function Name       : IntToASCII() //myw
//* Object              :��ʻԱ����ת����ASCII
//* Input Parameters    : int DriverCode
//* Output Parameters   : <buf[]>��ż�ʻԱ����ת����ASCII���������鳤��
//*���õ�ȫ�ֱ���       : none
//*�޸ĵ�ȫ�ֱ���       : None
//*----------------------------------------------------------------------------
unsigned char IntToASCII(unsigned char *buf,unsigned long drivercode)
{
	//unsigned char gew;unsigned char shiw;unsigned char baiw;
	// unsigned long 16--10����,
	unsigned int y;
	y=drivercode;
	unsigned int x=y/10;
	unsigned char i=0;
	do{
	    buf[i] =(y-x*10)+0x30;
		y=y/10;
		x=y/10;
		i=i+1;
	}while(y!=0);
     return i;
}
//*----------------------------------------------------------------------------
//* Function Name       : CharToASCII() //myw
//* Object              :һ���ֽڵ����ת���ɹ�꺺����
//* Input Parameters    : charһ���ֽڵ����
//* Output Parameters   :< ASCII[]>���һ���ֽڵ����ת���ɹ�꺺���룬ͬʱ���ظ����鳤��
//*���õ�ȫ�ֱ���       : none
//*�޸ĵ�ȫ�ֱ���       : None
//*----------------------------------------------------------------------------
unsigned char CharToASCII(unsigned char Char,unsigned char *ASCII)
{
	
	//unsigned char gew;unsigned char shiw;unsigned char baiw;
	// unsigned char16--10����,
	unsigned char geshu;
	if((Char/100)==0)
	{
	   if((Char/10)==0)
	   {
	      ASCII[0]=Char%10+0x30;//gew
	      geshu=1;
	   }
	   else
	   {
			 ASCII[0]=Char/10+0x30;//shiw
			 ASCII[1]=Char%10+0x30;//gew
			 geshu=2;
	    }
	}
	else
	{    	 
	 ASCII[0]=Char/100+0x30;//baiw
	 ASCII[1]=(Char/10)%10+0x30;//shiw
	 ASCII[2]=Char%10+0x30;//gew
	 geshu=3;
	 
	 } 
     return geshu;
}

//*----------------------------------------------------------------------------
//* Function Name       : PrintReady
//* Object              : �Ƿ���Դ�ӡ
//* Input Parameters    : none
//* Output Parameters   : none
//*----------------------------------------------------------------------------
unsigned char PrintReady()
{ 
	return 1;
}
unsigned char printOneOTDRRecord(unsigned char number,OTDR *record)
{
	return 1;
}
//*----------------------------------------------------------------------------
//* Function Name       : printline8() //myw
//* Object              :��ӡƣ�ͼ�ʻ��¼
//* Input Parameters    : none
//* Output Parameters   : �ɹ�����1��ʧ�ܷ���0
//*���õ�ȫ�ֱ���       : none
//*�޸ĵ�ȫ�ֱ���       : None
//*----------------------------------------------------------------------------
void printsent2byte(unsigned short *tbyte )
{
	rt_public_pin_init(3);
	SPI_I2S_SendData(SPI2, *tbyte);

}
void print1stb(unsigned short *tbyte)
{
	unsigned char i;
	for(i = 0;i<4;i++)
	{

	}
}


//*----------------------------------------------------------------------------
//* Function Name       : Printer
//* Object              : ��ӡ�������
//* Input Parameters    : none
//* Output Parameters   : none
//*----------------------------------------------------------------------------
#if 0
void PrinterOneline()
{
	unsigned long i,j;
	GPIO_SetBits(PRN_CMDB_CTR,PRN_LAT);
	GPIO_ResetBits(PRN_CMDE_CTR,PRN_STB1);
	GPIO_ResetBits(PRN_CMDE_CTR,PRN_STB2);
	rt_public_pin_init(3);
	lcd_delay(1000);
	GPIO_ResetBits(PRN_CMDB_CTR,PRN_SCK);
	GPIO_SetBits(PRN_CMDB_CTR,PRN_MOSI);
	lcd_delay(1);
	for(i = 0;i<384;i++)
	{

		//if((i%4)==0)
			//GPIO_SetBits(PRN_CMDB_CTR,PRN_MOSI);
		//else
			//GPIO_ResetBits(PRN_CMDB_CTR,PRN_MOSI);
		//GPIO_SetBits(PRN_CMDB_CTR,PRN_SCK);
		//lcd_delay(1);

		//GPIO_ResetBits(PRN_CMDB_CTR,PRN_SCK);
		//GPIO_ResetBits(PRN_CMDB_CTR,PRN_LAT);
		//lcd_delay(1);
		SPI_I2S_SendData(SPI2, 0xaaaa);
	}
	GPIO_ResetBits(PRN_CMDB_CTR,PRN_LAT);
	lcd_delay(3);
	GPIO_SetBits(PRN_CMDB_CTR,PRN_LAT);
	lcd_delay(1);
	GPIO_SetBits(PRN_CMDE_CTR,PRN_STB1);
	lcd_delay(2000);
	GPIO_ResetBits(PRN_CMDE_CTR,PRN_STB1);
	lcd_delay(1);
	GPIO_SetBits(PRN_CMDE_CTR,PRN_STB2);
	lcd_delay(2000);
	GPIO_ResetBits(PRN_CMDE_CTR,PRN_STB2);

}
void Printtest()
{
	unsigned long i,j,k,count;
	j=0;
	k=0;
	GPIO_SetBits(PRN_CMDB_CTR,PRN_MCSTOP);
	GPIO_ResetBits(PRN_CMDB_CTR,PRN_MC1);
	GPIO_ResetBits(PRN_CMDB_CTR,PRN_MC2);
	for(i=0;i<500;i++)
	{
		if((i%2)==0)
		{
			if(k)
			{
				GPIO_ResetBits(PRN_CMDB_CTR,PRN_MC2);
				k= 0;
			}
			else
			{
				GPIO_SetBits(PRN_CMDB_CTR,PRN_MC2);

				k= 1;
			}

		}
		else
		{
			if(j)
			{
				GPIO_ResetBits(PRN_CMDB_CTR,PRN_MC1);
				j = 0;
			}
			else
			{
				GPIO_SetBits(PRN_CMDB_CTR,PRN_MC1);
				j= 1;
			}
		}
		PrinterOneline();
		for(count = 0;count<0xfff;count++);

		}
	GPIO_ResetBits(PRN_CMDB_CTR,PRN_MCSTOP);
}
#endif
unsigned short AutoCodeHZ2Print(unsigned short data,unsigned char num)
{
	unsigned short ret;
	switch(data)
	{
		case 0xbea9:
			ret = Pri_Ejing[num];//{""}
			break;
		case 0xbba6:
			ret = Pri_Ehu[num];//{"��"}
			break;
		case 0xbdf2:
			ret = Pri_Ejin[num];//{"��"}
			break;
		case 0xcbd5:
			ret = Pri_Esu[num];//{"��"}
			break;
		case 0xcdee:
			ret = Pri_Ewan[num];//{"��"}
			break;
		case 0xb8d3:
			ret = Pri_Egan[num];//{"��"}
			break;
		case 0xc3f6:
			ret = Pri_Emin[num];//{"��"}
			break;
		case 0xc2b3:
			ret = Pri_Elu[num];//{"³"}
			break;
		case 0xd5e3:
			ret = Pri_Ezhe[num];//{"��"}
			break;
		case 0xbdfa:
			ret = Pri_Ejin1[num];//{"��"}
			break;
		case 0xbcbd:
			ret = Pri_Eji[num];//{"��"}
			break;
		case 0xd4a5:
			ret = Pri_Eyu[num];//{"ԥ"}
			break;
		case 0xc3c9:
			ret = Pri_Emeng[num];//{"��"}
			break;
		case 0xd0c2:
			ret = Pri_Exin[num];//{"��"}
			break;
		case 0xc4fe:
			ret = Pri_Ening[num];//{"��"}
			break;
		case 0xc1c9:
			ret = Pri_Eliao[num];//{"��"}
			break;
		case 0xbada:
			ret = Pri_Ehei[num];//{"��"}
			break;
		case 0xbcaa:
			ret = Pri_Eji1[num];//{"��"}
			break;
		case 0xcfe6:
			ret = Pri_Exiang[num];//{"��"}
			break;
		case 0xb6f5:
			ret = Pri_Ee[num];//{"��"}
			break;
		case 0xb9f0:
			ret = Pri_Egui[num];//{"��"}
			break;
		case 0xd4c1:
			ret = Pri_Eyue[num];//{"��"}
			break;
		case 0xc7ed:
			ret = Pri_Eqiong[num];//{"��"}
			break;
		case 0xb2d8:
			ret = Pri_Ezang[num];//{"��"}
			break;
		case 0xc9c2:
			ret = Pri_Eshan[num];//{"��"}
			break;
		case 0xb8ca:
			ret = Pri_Egan1[num];//{"��"}
			break;
		case 0xc7e0:
			ret = Pri_Eqing[num];//{"��"}
			break;
		case 0xb4a8:
			ret = Pri_Echuan[num];//{"��"}
			break;
		case 0xc7ad:
			ret =  Pri_Eqian[num];//{"ǭ"}
			break;
		case 0xd4c6:
			ret = Pri_Eyun[num];//{"��"}
			break;
		case 0xbaa3:
			ret =  Pri_Ehai[num];//{"��"}
			break;
		case 0xcca8:
			ret =  Pri_Etai[num];//{"̨"}
			break;
		case 0xd3e5:
			ret = Pri_Eyu1[num];//{"��"}
			break;
		case 0xb8db:
			ret =  Pri_Egang[num];//{"��"}
			break;
		case 0xb0c4:
			ret =  Pri_Eao[num];//{"��"}
			break;
		case 0xcab9:
			ret =  Pri_Eshi[num];//{"ʹ"}
			break;
		case 0xbcd7:
			ret = Pri_Ejia[num];//{"��"}
			break;
		case 0xd2d2:
			ret = Pri_Eyi[num];//{"��"}
			break;
		case 0xb1fb:
			ret = Pri_Ebing[num];//{"��"}
			break;
		case 0xb6a1:
			ret = Pri_Eding[num];//{"��"}
			break;
		case 0xceec:
			ret = Pri_Exu[num];//{"��"}
			break;
		case 0xbcba:
			ret = Pri_Eji3[num];//{"��"}
			break;
		case 0xb8fd:
			ret = Pri_Egeng[num];//{"��"}
			break;
		case 0xd0c1:
			ret =Pri_Exin1[num];//{"��"}
			break;
		case 0xd7d3:
			ret = Pri_Ezi[num];//{"��"}
			break;
		case 0xb3f3:
			ret = Pri_Echou[num];//{"��"}
			break;
		case 0xd2fa:
			ret = Pri_Eyin[num];//{"��"}
			break;
		case 0xc3ae:
			ret = Pri_Emou[num];//{"î"}
			break;
		case 0xb3bd:
			ret = Pri_Echen[num];//{"��"}
			break;
		case 0xcee7:
			ret =Pri_Ewu[num];//{"��"}
			break;
		case 0xceb4:
			ret = Pri_Ewei[num];//{"δ"}
			break;
		case 0xc9ea:
			ret = Pri_Eshen[num];//{"��"}
			break;
		case 0xd3cf:
			ret = Pri_Eyou[num];//{"��"}
			break;
		case 0xbaa5:
			ret = Pri_Ehai1[num];//{"��"}
			break;
		case 0xc8c9:
			ret = Pri_Eren[num];//{"��"}
			break;
		case 0xbeaf:
			ret = Pri_Ejing1[num];//{"��"}
			break;
		case 0xb9f3:
			ret = Pri_Egui1[num];//{"��"}
			break;
		case 0xc1ec:
			ret = Pri_Eling[num];//{"��"}
			break;
		case 0xd1a7:
			ret = Pri_Exue[num];//{"ѧ"}
			break;
		case 0xcad4:
			ret = Pri_Eshi3[num];//{"��"}
			break;
		case 0xbeb3:
			ret = Pri_Ejing2[num];//{"��"}
			break;
		default:
			ret = 0x0000;
			break;
	}
	return ret;
}
unsigned short ASCII2Print(unsigned char data,unsigned char num)
{
	unsigned short ret;
	switch(data)
	{
		case 48:
			ret=Pri_zimu0[num];break;
		case 49:
			ret=Pri_zimu1[num];break;
		case 50:
			ret=Pri_zimu2[num];break;
		case 51:
			ret=Pri_zimu3[num];break;
		case 52:
			ret=Pri_zimu4[num];break;
		case 53:
			ret=Pri_zimu5[num];break;
		case 54:
			ret=Pri_zimu6[num];break;
		case 55:
			ret=Pri_zimu7[num];break;
		case 56:
			ret=Pri_zimu8[num];break;
		case 57:
			ret=Pri_zimu9[num];break;

		case 65:
			ret=Pri_zimuA[num];break;
		case 66:
			ret=Pri_zimuB[num];break;
		case 67:
			ret=Pri_zimuC[num];break;
		case 68:
			ret=Pri_zimuD[num];break;
		case 69:
			ret=Pri_zimuE[num];break;
		case 70:
			ret=Pri_zimuF[num];break;
		case 71:
			ret=Pri_zimuG[num];break;
		case 72:
			ret=Pri_zimuH[num];break;
		case 73:
			ret=Pri_zimuI[num];break;
		case 74:
			ret=Pri_zimuJ[num];break;
		case 75:
			ret=Pri_zimuK[num];break;
		case 76:
			ret=Pri_zimuL[num];break;
		case 77:
			ret=Pri_zimuM[num];break;
		case 78:
			ret=Pri_zimuN[num];break;
		case 79:
			ret=Pri_zimuO[num];break;
		case 80:
			ret=Pri_zimuP[num];break;
		case 81:
			ret=Pri_zimuQ[num];break;
		case 82:
			ret=Pri_zimuR[num];break;
		case 83:
			ret=Pri_zimuS[num];break;
		case 84:
			ret=Pri_zimuT[num];break;
		case 85:
			ret=Pri_zimuU[num];break;
		case 86:
			ret=Pri_zimuV[num];break;
		case 87:
			ret=Pri_zimuW[num];break;
		case 88:
			ret=Pri_zimuX[num];break;
		case 89:
			ret=Pri_zimuY[num];break;
		case 90:
			ret=Pri_zimuZ[num];break;
		default:
			ret=0;break;
	}
	return ret;
}
void PrinAutoCode(unsigned short *pbuf ,unsigned char num)
{
	unsigned short hz =0;
	unsigned char j=0,type=0,count;
	unsigned char buf =Parameter.AutoInfodata.AutoCode[0];
	while((buf!='\0')&&(j<17))
	{
		if(buf>127)
		{
			hz = hz+buf;
		    if(count == 1)
		    {
		    	pbuf[type] = AutoCodeHZ2Print(hz,num);
		    	type++;
		    	count = 0;
		    	hz =0;
		    }
		    else
		    {
		    	hz = hz<<8;
		    	count++;
		    }
		    j++;
			buf=Parameter.AutoInfodata.AutoCode[j];


		}
		else
		{
			pbuf[type] = ASCII2Print(buf,num);

			j++;
			type++;
			buf=Parameter.AutoInfodata.AutoCode[j];
		}
	}

}
void PrinAutoCodeDiff(unsigned short *pbuf ,unsigned char num)
{
	unsigned short hz=0;
	unsigned char j=0,type=0,count;
	unsigned char buf =Parameter.AutoInfodata.AutoSort[0];
	while((buf!='\0')&&(j<17))
	{
		if(buf>127)
		{
			hz = hz+buf;
			if(count == 1)
			{
				pbuf[type] = AutoCodeHZ2Print(hz,num);
				type++;
				count = 0;
				hz =0;
			}
			else
			{
				hz = hz<<8;
				count++;
			}
			j++;
			buf=Parameter.AutoInfodata.AutoCode[j];

		}
		else
		{
			pbuf[type] = ASCII2Print(buf,num);

			j++;
			type++;
			buf=Parameter.AutoInfodata.AutoSort[j];
		}
	}

}
void PrinterOnelinePoint(unsigned short *prtbuf)
{
	unsigned long i,j;
	GPIO_SetBits(PRN_CMDB_CTR,PRN_LAT);
	GPIO_ResetBits(PRN_CMDE_CTR,PRN_STB1);
	GPIO_ResetBits(PRN_CMDE_CTR,PRN_STB2);
	rt_public_pin_init(3);
	lcd_delay(1);
    for(i = 0;i<24;i++)
    {
    	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    	SPI_I2S_SendData(SPI2,  *(prtbuf+i));
    }
	GPIO_ResetBits(PRN_CMDB_CTR,PRN_LAT);
	lcd_delay(1);
	GPIO_SetBits(PRN_CMDB_CTR,PRN_LAT);
	lcd_delay(1);
	GPIO_SetBits(PRN_CMDE_CTR,PRN_STB1);
	GPIO_SetBits(PRN_CMDE_CTR,PRN_STB2);
	lcd_delay(1200);
	GPIO_ResetBits(PRN_CMDE_CTR,PRN_STB1);
	GPIO_ResetBits(PRN_CMDE_CTR,PRN_STB2);

//	lcd_delay(1);
//	GPIO_SetBits(PRN_CMDE_CTR,PRN_STB2);
	//lcd_delay(2000);
	//GPIO_ResetBits(PRN_CMDE_CTR,PRN_STB2);

}
void Print1line(unsigned short *buf)
{
	unsigned long i,j,k,count;
	j=0;
	k=0;
	GPIO_SetBits(PRN_CMDB_CTR,PRN_MCSTOP);
	GPIO_ResetBits(PRN_CMDB_CTR,PRN_MC1);
	GPIO_ResetBits(PRN_CMDB_CTR,PRN_MC2);
	for(i=0;i<90;i++)
	{
		if((i%2)==0)
		{
			if(k)
			{
				GPIO_ResetBits(PRN_CMDB_CTR,PRN_MC2);
				k= 0;
			}
			else
			{
				GPIO_SetBits(PRN_CMDB_CTR,PRN_MC2);

				k= 1;
			}

		}
		else
		{
			if(j)
			{
				GPIO_ResetBits(PRN_CMDB_CTR,PRN_MC1);
				j = 0;
			}
			else
			{
				GPIO_SetBits(PRN_CMDB_CTR,PRN_MC1);
				j= 1;
			}
		}
		if(i<64)
		{
			PrinterOnelinePoint((buf+(i/2)*24));
		}
		else
		{
			for(count = 0;count<0xfff;count++);
		}
		for(count = 0;count<0xff;count++);

	}
	GPIO_ResetBits(PRN_CMDB_CTR,PRN_MCSTOP);
}
void printhaoma()
{
	memset(print_buf,0,sizeof(print_buf));
	unsigned char i;
	for(i=0;i<32;i++)
	{
		print_buf[i][0] = Pri_ji[i];
		print_buf[i][1] = Pri_dong[i];
		print_buf[i][2] = Pri_che[i];
		print_buf[i][3] = Pri_hao[i];
		print_buf[i][4] = Pri_pai[i];
		print_buf[i][5] = Pri_hao[i];
		print_buf[i][6] = Pri_ma[i];
		print_buf[i][7] = Pri_maohao[i];
		PrinAutoCode(&print_buf[i][8],i/2);

	}
	Print1line(&print_buf[0][0]);

}
void printfenlei()
{
	unsigned char i;
	memset(print_buf,0,sizeof(print_buf));
	for(i=0;i<32;i++)
	{
		print_buf[i][0] = Pri_ji[i];
		print_buf[i][1] = Pri_dong[i];
		print_buf[i][2] = Pri_che[i];
		print_buf[i][3] = Pri_hao[i];
		print_buf[i][4] = Pri_pai[i];
		print_buf[i][5] = Pri_fen[i];
		print_buf[i][6] = Pri_lei[i];
		print_buf[i][7] = Pri_maohao[i];
		PrinAutoCodeDiff(&print_buf[i][8],i);

	}
	Print1line(&print_buf[0][0]);
}
void printJSZ()
{
	unsigned char i,j;
	memset(print_buf,0,sizeof(print_buf));
	for(i=0;i<32;i++)
	{
		print_buf[i][0] = Pri_ji[i];
		print_buf[i][1] = Pri_dong[i];
		print_buf[i][2] = Pri_che[i];
		print_buf[i][3] = Pri_jia[i];
		print_buf[i][4] = Pri_shi[i];
		print_buf[i][5] = Pri_zheng[i];
		print_buf[i][6] = Pri_hao[i];
		print_buf[i][7] = Pri_ma[i];
		print_buf[i][8] = Pri_maohao[i];

	}
	Print1line(&print_buf[0][0]);
	memset(print_buf,0,sizeof(print_buf));
	///////dayin jiashizheng
	for(i = 0;i<32;i++)
	{
		for(j=0;j<18;j++)
		{
			print_buf[i][j+2] =ASCII2Print(Parameter.DriverLisenseCode[j],i/2);
		}
	}
	Print1line(&print_buf[0][0]);

}
void printspeed()
{
	unsigned char i;
	memset(print_buf,0,sizeof(print_buf));
	for(i=0;i<32;i++)
	{
		print_buf[i][0] = Pri_su[i];
		print_buf[i][1] = Pri_du[i];
		print_buf[i][2] = Pri_zhuang[i];
		print_buf[i][3] = Pri_tai[i];
		print_buf[i][4] = Pri_maohao[i];
		//print_buf[i][19] = ASCII2Print(CurStatus,i);


	}
	Print1line(&print_buf[0][0]);
}
void FTimebuff(CLOCK ctime)
{
	unsigned char i,j;
	for(i=0;i<32;i++)
	{
		j= i/2;
		print_buf[i][2] = Pri_zimu2[j];
		print_buf[i][3] = Pri_zimu0[j];
		print_buf[i][4] =ASCII2Print(((ctime.year>>4)+48),j);
		print_buf[i][5] =ASCII2Print(((ctime.year&0x0f)+48),j);
		print_buf[i][6] =Pri_zhongheng[j];
		print_buf[i][7] =ASCII2Print(((ctime.month>>4)+48),j);
		print_buf[i][8] =ASCII2Print(((ctime.month&0x0f)+48),j);
		print_buf[i][9] =Pri_zhongheng[j];
		print_buf[i][10] =ASCII2Print(((ctime.day>>4)+48),j);
		print_buf[i][11] =ASCII2Print(((ctime.day&0x0f)+48),j);

		print_buf[i][14] =ASCII2Print(((ctime.hour>>4)+48),j);
		print_buf[i][15] =ASCII2Print(((ctime.hour&0x0f)+48),j);
		print_buf[i][16] =Pri_maohao[i];
		print_buf[i][17] =ASCII2Print(((ctime.minute>>4)+48),j);
		print_buf[i][18] =ASCII2Print(((ctime.minute&0x0f)+48),j);
		print_buf[i][19] =Pri_maohao[i];
		print_buf[i][20] =ASCII2Print(((ctime.second>>4)+48),j);
		print_buf[i][21] =ASCII2Print(((ctime.second&0x0f)+48),j);

	}

}
void printTime()
{
	unsigned char i;
	memset(print_buf,0,sizeof(print_buf));
	for(i=0;i<32;i++)
	{
		print_buf[i][0] = Pri_da[i];
		print_buf[i][1] = Pri_yin[i];
		print_buf[i][2] = Pri_shi2[i];
		print_buf[i][3] = Pri_jian[i];
		print_buf[i][4] = Pri_maohao[i];

	}
	Print1line(&print_buf[0][0]);
	memset(print_buf,0,sizeof(print_buf));
	FTimebuff(curTime);
	Print1line(&print_buf[0][0]);
}
void printRecord(void)
{
	unsigned long tdaytime,tsave,STOPp,STOPb,flag;
	unsigned char num = 0,i,j;
	unsigned char Readdata[50];
	CLOCK timecount;
	STOPp = pTable.BaseData.CurPoint;
	flag = pTable.BaseData.finshflag;
	timecount.day = curTime.day;
	timecount.year = curTime.year;
	timecount.month = curTime.month;
	timecount.hour = 0;
	timecount.minute =0;
	timecount.second = 0;
	tdaytime = timechange(timecount)-86400;
	if((flag &0xea)==0xea)
	{
		do
		{
			if(STOPp > OVERDRV_BASE)
			{
				STOPp = STOPp-50;
			}
			else
			{
				STOPb = (OVERDRV_END-OVERDRV_BASE+1)/50;
				STOPp = STOPb*50+OVERDRV_BASE;
			}
			SPI_FLASH_BufferRead(SPI1 ,(uint8_t *)&timecount ,STOPp+18, 6);
			tsave = timechange(timecount);
			if(tsave > tdaytime )
			{
				num++;
				for(i=0;i<32;i++)
				{
					print_buf[i][0] = Pri_ji1[i];
					print_buf[i][1] = Pri_lu[i];
					print_buf[i][2] = ASCII2Print(num,i);
					print_buf[i][3] = Pri_maohao[i];
				}
				Print1line(&print_buf[0][0]);
				memset(print_buf,0,sizeof(print_buf));
				SPI_FLASH_BufferRead(SPI1 ,(uint8_t *)&Readdata ,STOPp, 50);
				for(i=0;i<32;i++)
				{
					print_buf[i][0] = Pri_ji[i];
					print_buf[i][1] = Pri_dong[i];
					print_buf[i][2] = Pri_che[i];
					print_buf[i][3] = Pri_jia[i];
					print_buf[i][4] = Pri_shi[i];
					print_buf[i][5] = Pri_zheng[i];
					print_buf[i][6] = Pri_hao[i];
					print_buf[i][7] = Pri_ma[i];
					print_buf[i][8] = Pri_maohao[i];

				}
				Print1line(&print_buf[0][0]);
				memset(print_buf,0,sizeof(print_buf));
				for(i=0;i<32;i++)
				{
					for(j=0;j<18;j++)
					{
						print_buf[i][21-j] =ASCII2Print(Readdata[j],i/2);
					}
				}
				Print1line(&print_buf[0][0]);
				memset(print_buf,0,sizeof(print_buf));
				for(i=0;i<32;i++)
				{
					print_buf[i][0] = Pri_lian[i];
					print_buf[i][1] = Pri_xu[i];
					print_buf[i][2] = Pri_jia[i];
					print_buf[i][3] = Pri_shi[i];
					print_buf[i][4] = Pri_kai[i];
					print_buf[i][5] = Pri_shi1[i];
					print_buf[i][6] = Pri_shi2[i];
					print_buf[i][7] = Pri_jian[i];
					print_buf[i][8] = Pri_maohao[i];

				}
				Print1line(&print_buf[0][0]);
				memset(print_buf,0,sizeof(print_buf));
				for(i=0;i<32;i++)
				{

					print_buf[i][1] = ASCII2Print(2,i);
					print_buf[i][2] = ASCII2Print(0,i);
					print_buf[i][3] = ASCII2Print(Readdata[18]>>4,i);
					print_buf[i][4] = ASCII2Print((Readdata[18]&0x0f),i);
					print_buf[i][5] = Pri_Ehx2[i];
					print_buf[i][6] = ASCII2Print(Readdata[19]>>4,i);
					print_buf[i][7] = ASCII2Print((Readdata[19]&0x0f),i);
					print_buf[i][8] = Pri_Ehx2[i];
					print_buf[i][9] = ASCII2Print(Readdata[20]>>4,i);
					print_buf[i][10] = ASCII2Print((Readdata[20]&0x0f),i);

					print_buf[i][12] = ASCII2Print(Readdata[21]>>4,i);
					print_buf[i][13] = ASCII2Print((Readdata[21]&0x0f),i);
					print_buf[i][14] = Pri_Ehx2[i];
					print_buf[i][15] = ASCII2Print(Readdata[22]>>4,i);
					print_buf[i][16] = ASCII2Print((Readdata[22]&0x0f),i);
					print_buf[i][17] = Pri_Ehx2[i];
					print_buf[i][18] = ASCII2Print(Readdata[23]>>4,i);
					print_buf[i][19] = ASCII2Print((Readdata[23]&0x0f),i);

				}
				Print1line(&print_buf[0][0]);
				memset(print_buf,0,sizeof(print_buf));
				for(i=0;i<32;i++)
				{
					print_buf[i][0] = Pri_lian[i];
					print_buf[i][1] = Pri_xu[i];
					print_buf[i][2] = Pri_jia[i];
					print_buf[i][3] = Pri_shi[i];
					print_buf[i][4] = Pri_jie[i];
					print_buf[i][5] = Pri_shu[i];
					print_buf[i][6] = Pri_shi2[i];
					print_buf[i][7] = Pri_jian[i];
					print_buf[i][8] = Pri_maohao[i];
				}
				Print1line(&print_buf[0][0]);
				memset(print_buf,0,sizeof(print_buf));
				for(i=0;i<32;i++)
				{

					print_buf[i][1] = ASCII2Print(2,i);
					print_buf[i][2] = ASCII2Print(0,i);
					print_buf[i][3] = ASCII2Print(Readdata[24]>>4,i);
					print_buf[i][4] = ASCII2Print((Readdata[24]&0x0f),i);
					print_buf[i][5] = Pri_Ehx2[i];
					print_buf[i][6] = ASCII2Print(Readdata[25]>>4,i);
					print_buf[i][7] = ASCII2Print((Readdata[25]&0x0f),i);
					print_buf[i][8] = Pri_Ehx2[i];
					print_buf[i][9] = ASCII2Print(Readdata[26]>>4,i);
					print_buf[i][10] = ASCII2Print((Readdata[26]&0x0f),i);

					print_buf[i][12] = ASCII2Print(Readdata[27]>>4,i);
					print_buf[i][13] = ASCII2Print((Readdata[27]&0x0f),i);
					print_buf[i][14] = Pri_Ehx2[i];
					print_buf[i][15] = ASCII2Print(Readdata[28]>>4,i);
					print_buf[i][16] = ASCII2Print((Readdata[28]&0x0f),i);
					print_buf[i][17] = Pri_Ehx2[i];
					print_buf[i][18] = ASCII2Print(Readdata[29]>>4,i);
					print_buf[i][19] = ASCII2Print((Readdata[29]&0x0f),i);

				}
				Print1line(&print_buf[0][0]);
				memset(print_buf,0,sizeof(print_buf));

			}

		}while((tsave < tdaytime) );
	}

}
void printOverRecord()
{
	unsigned char i;
	memset(print_buf,0,sizeof(print_buf));
	for(i=0;i<32;i++)
	{
		print_buf[i][0] = Pri_zimu2[i/2];
		print_buf[i][1] = Pri_ge[i];
		print_buf[i][2] = Pri_ri[i];
		print_buf[i][3] = Pri_li[i];
		print_buf[i][4] = Pri_tian[i];
		print_buf[i][5] = Pri_nei[i];
		print_buf[i][6] = Pri_chao[i];
		print_buf[i][7] = Pri_shi2[i];
		print_buf[i][8] = Pri_jia[i];
		print_buf[i][9] = Pri_shi[i];
		print_buf[i][10] = Pri_ji1[i];
		print_buf[i][11] = Pri_lu[i];
		print_buf[i][12] = Pri_maohao[i];

	}
	Print1line(&print_buf[0][0]);
	memset(print_buf,0,sizeof(print_buf));
	printRecord();
}
void printsign()
{
	unsigned char i;
	memset(print_buf,0,sizeof(print_buf));
	for(i=0;i<32;i++)
	{
		print_buf[i][0] = Pri_qian[i];
		print_buf[i][1] = Pri_ming[i];
		print_buf[i][2] = Pri_maohao[i];

	}
	Print1line(&print_buf[0][0]);
}

void printer()
{
	printhaoma();
	printfenlei();
	printJSZ();
	printspeed();
	printTime();
	printOverRecord();
	printsign();
	memset(print_buf,0,sizeof(print_buf));
	Print1line(&print_buf[0][0]);
	Print1line(&print_buf[0][0]);
	Print1line(&print_buf[0][0]);
}
void printer_pwr_ctrl(unsigned char ctldr)
{
	if(ctldr)
	{
		GPIO_SetBits(PRN_CMDE_CTR,PRN_EN);
		lcd_delay(5);
		GPIO_SetBits(PRN_CMDE_CTR,PRN_PWR);
	}
	else
	{
		GPIO_ResetBits(PRN_CMDE_CTR,PRN_EN);
		lcd_delay(5);
		GPIO_ResetBits(PRN_CMDE_CTR,PRN_PWR);
	}
}
void rt_hw_printer_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin   = PRN_PWR|PRN_EN;
	//PRN_PWR，PRN_EN，SRTOB12。
	GPIO_Init(PRN_CMDE_CTR, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = PRN_STB1|PRN_STB2;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(PRN_CMDE_CTR, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin   = PRN_MCSTOP |PRN_LAT|PRN_MC1|PRN_MC2;
	GPIO_Init(PRN_CMDB_CTR, &GPIO_InitStructure);

	GPIO_Init(GPIOE, &GPIO_InitStructure);

	printer_pwr_ctrl(0);

}


