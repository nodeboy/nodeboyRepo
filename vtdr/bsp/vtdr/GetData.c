
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include<stm32f10x_tim.h>
#include<application.h>
#include<DataManager.h>
#include<menu.h>
#include<RS232.h>
#include "serial.h"

extern void playVolWarm1();
extern void playVolWarm2();
extern void playVolWarm3();
//extern void playVolWarm4();
extern CLOCK curTime;
extern LCDTCB lcd_tcb;
extern SizeData location;
unsigned char CurStatus;
#define SpeedSpace 4

extern struct rt_device uart4_device;
extern struct rt_device uart2_device;
extern unsigned char TimeChange;
extern unsigned char DriverStatus ;
extern unsigned char AlarmFlag;
extern unsigned char DriverRegstatus;
extern Datastatus Datastatusdata;
extern StructPara Parameter;
extern PartitionTable pTable;
extern CMD_VER Verificationstatus;
extern unsigned char  DriverLisenseCode[18];
extern unsigned char  DriverFlag;
extern unsigned char CloseUSART1Time;
///////////////////////////////////gps data and speed data////////////
unsigned long CurPN = 0;
unsigned long LastPN20ms = 0;
unsigned long LastPN1s = 0;
unsigned long LastPN1min = 0;
unsigned long RsSpeed;
unsigned short CurSpeed = 0;	//当前速度（0.2秒平均速度）
unsigned long Curspeed1s = 0;//1s平均速度
unsigned long Curspeed1min = 0;//1分钟平均速度
unsigned char radionum = 0;
unsigned char gpsstatus=0;
unsigned long CurPulse = 0;

int DeltaSpeed = 0;


////////////////////////////gprs data and status///////////////////////

unsigned char  gprsflag;
unsigned char gprsflagbak;

unsigned char ipaddress[]="\"61.135.144.51\"";//ip dizhi
unsigned char depoint[]="\"9999\"";//duan kou hao
unsigned char GprsSendbuff[160];
unsigned char GprsSendleth;
const Message_Array  MessageArray[]={{0x0100,&devack},{0x0001,&devreg},{0x0201,&devauth},{0x0070,&devsendata},{0x0200,0}};

////////////////time///////////////////////////////////
unsigned char Time6sCnt;
unsigned char Time30mincnt1;
unsigned char Time30mincnt2;
unsigned char Time30mincnt3;
unsigned char Time30mincnt4;
unsigned char Time10sCnts;
unsigned char Time10sCnte;
unsigned char Time20sCnt1;
unsigned char Time20sCnt2;

unsigned long Distance = 0;


////////////////ic卡data///////////////////////
///////////////////////////////////////////////
unsigned short insertcount; //防止误动作
////////////////////////////////key data////////
//////////////////////////////////////////////
Timeflag timeflag;
TimeCnt  timecnt;
typedef enum KEY_STATUS
{
	KEY_NONE,
	MENU_KEY,
	SELE_UP_KEY,
	SELE_DOWN_KEY,
	ENTER_KEY,
	PRIN_KEY,

}Key_status;
Key_status keyval = KEY_NONE,keyvalbak = KEY_NONE;
unsigned short keyvalcount;
void Getthepluse()
{
	CurPN++;
	Parameter.PulseNumber++;

}
//*----------------------------------------------------------------------------
//* Function Name       : GetStatus
//* Object              : ��ȡ״̬
//* Input Parameters    : none
//* Output Parameters   : none
//* ���õ�ȫ�ֱ���      :
//* �޸ĵ�ȫ�ֱ���      : ״̬
//*----------------------------------------------------------------------------
void GetStatus()
{
	//PA0:点火信号；
	CurStatus |= (7<<GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12));//制动
	CurStatus |= (6<<GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_8));//turn left
	CurStatus |= (5<<GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10));//turn right
	CurStatus |= (4<<GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11));//high beam
	CurStatus |= (3<<(~GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)));//low beam
	CurStatus |= (2<<GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9));//door
	CurStatus |= (1<<GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13));//鸣笛
	CurStatus |= (0<<GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0));//点火
}


//*----------------------------------------------------------------------------
//* Function Name       : ComputeSpeed
//* Object              : �����ٶ�ֵ
//* Input Parameters    : ������
//* Output Parameters   : none
//* ���õ�ȫ�ֱ���      :
//* �޸ĵ�ȫ�ֱ���      : �������ٶ�ֵ��
//*----------------------------------------------------------------------------
unsigned long ComputeSpeed(unsigned long pulse)
{
	unsigned long p,t,temp_pulse;
	int i;
	unsigned long spe;
	unsigned long T;
	T = 360000/Parameter.PulseCoff;

	//spe = T*pulse/(2000);
	spe = T*pulse/2;

	if((spe % 10) >= 5)
		spe = spe/10 + 1;
	else
		spe = spe/10;
		
	if(spe>255)
		spe = 255;
	return spe;

}
//#if OpenDoorDeal
//*----------------------------------------------------------------------------
//* Function Name       : GetSpeed
//* Object              : ��ȡ�ٶȺͿ�����״̬
//* Input Parameters    : times��ʾ�ڼ��βɼ�
//* Output Parameters   : �ɼ��Ƿ�ɹ�
//*0.2s执行一次
//*----------------------------------------------------------------------------
void GetSpeedandTime(void)
{
 	unsigned long pulse,timer;
	unsigned long temp;
	unsigned long p,t;
	if(timeflag.Time200msflag == 1)
	{
		if(LastPN20ms == 0)
			pulse = CurPN;
		else
			pulse = CurPN-LastPN20ms;
		timeflag.Time200msflag = 0;
		LastPN20ms = CurPN;
		CurSpeed = ComputeSpeed(pulse);
		if(CurSpeed == 0)
		{
			CurSpeed = RsSpeed;
		}
		DoubltPointHandler();
		//rt_device_write(&uart4_device, 0,"AT/n", 3);

	}
	if(timeflag.Time1sflag == 1)
	{
		GetCurrentDateTime(&curTime);
		if(( lcd_tcb.ListNb == 0 ) && (lcd_tcb.mode == 0))
		{
			DisplayNormalUI();
		}
		TimeChange |= (0x01<<SECOND_CHANGE);
		if(LastPN1s == 0)
			pulse = CurPN;
		else
			pulse = CurPN-LastPN1s;
		LastPN1s = CurPN;
		timeflag.Time1sflag = 0;
		Curspeed1s = ComputeSpeed(pulse)/5;
		if(CloseUSART1Time ==1)
		{
			CloseUSART1Time = 0xff;
			WriteParameterTable(&Parameter);
		}
		JournalHandle();

	}
	if(timeflag.Time1minflag == 1)
	{
		if(LastPN1min == 0)
			pulse = CurPN;
		else
			pulse = CurPN-LastPN1min;
		LastPN1min = CurPN;
		timeflag.Time1minflag = 0;
		TimeChange |= (0x01<<MINUTE_CHANGE);
		Curspeed1min = ComputeSpeed(pulse)/300;
		temp = BCD2Char(Parameter.DriverDistace)+100*BCD2Char(Parameter.DriverDistace>>8);
		temp = 10000*BCD2Char(Parameter.DriverDistace>>16)+temp;
		temp = 1000000*BCD2Char(Parameter.DriverDistace>>24)+temp;
		temp = Curspeed1min/6+temp;
		Parameter.DriverDistace = Char2BCD(temp%100)+Char2BCD((temp%10000)/100)*256+Char2BCD((temp%1000000)/10000)*65536
								+Char2BCD((temp%100000000)/1000000)*16777216;

	}

}
//////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
///////////////////////////////////////key handler////////////////
/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
void Time3_irg_handler()
{
	extern void keyscanhandler(void);
	  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	  {
	      TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	      timecnt.Time1msCnt++;
	      if((timecnt.Time1msCnt )%10 ==0)//10ms扫描一次按键
	      {
	    	  keyscanhandler();
	      }
	      if( timecnt.Time1msCnt >199)
	      {
	    	  timecnt.Time1msCnt = 0;
	    	  timeflag.Time200msflag = 1;

	    	  timecnt.Time200msCnt++;
	    	  if(timecnt.Time200msCnt > 4)
	    	  {
	    		  timeflag.Time1sflag = 1;
	    		  timeflag.Ver1sflag = 1;
	    		  timecnt.Time200msCnt = 0;
	    		  timecnt.Time1sCnt ++;
				  if( MessagDevicIDflag.gprsAuthsucess == 1 )
				  {
					  timecnt.breaktimecout--;
					  if(timecnt.breaktimecout ==0 )
					  {
						  timecnt.breaktimecout = gprsparavalue.heartbreaktime;
						  MessagDevicIDflag.gprssenbreakheart=1;
					  }
				  }

	    		  if(timecnt.Time10sCnt)
	    		  {
	    			  timecnt.Time10sCnt--;
	    		  }
	    		  if(Time6sCnt)
	    		  {
	    			  Time6sCnt--;
	    			  if((Time6sCnt == 0)&&(Verificationstatus == 1))
	    			  {
	    				  Verificationstatus = 0;
	    			  }

	    		  }
	    		  if(Time10sCnts)
	    		  {
	    			  Time10sCnts--;
					  if(Time10sCnts == 0)
					  {
						  Datastatusdata.keepstart10s = 1;
					  }
	    		  }
	    		  if(Time10sCnte)
				  {
					  Time10sCnte--;
					  if(Time10sCnte == 0)
					  {
						  Datastatusdata.keepstop10s = 1;
					  }
				  }
	    		  if(Time20sCnt1)
				  {
					  Time20sCnt1--;
				  }
	    		  if(Time20sCnt2)
				  {
	    			  Time20sCnt2--;
				  }
	    		  if(timecnt.Time1sCnt >59)
	    		  {
	    			  timeflag.Time1minflag = 1;
	    			  timecnt.Time1sCnt = 0;
					  if(Time30mincnt4)//alarm status handle
					  {
						  if(Time30mincnt4 != 1)
						  {
							  Time30mincnt4--;
						  }
					  }
	    			  timecnt.Time5minCnt++;
	    			  if(timecnt.Time5minCnt>4)
	    			  {
	    				  timecnt.Time5minCnt = 0;
						  if(Time30mincnt1!= 0xff )
						  {
							  if(Time30mincnt1)//alarm status handle
							  {
								  Time30mincnt1--;
								  Time20sCnt1 =20;
								  playVolWarm1();
								  playVolWarm1();
								  playVolWarm1();
								  if(Time30mincnt1 == 0)
								  {

									  Time30mincnt1 = 0xff;

								  }


							  }
						  }
						  if(Time30mincnt2!= 0xff )
						  {
							  if(Time30mincnt2)//alarm status handle
							  {
								  Time30mincnt2--;
								  Time20sCnt2 =20;
								  playVolWarm2();
								  playVolWarm2();
								  playVolWarm2();
								  if(Time30mincnt2 == 0)
								  {
									  Time30mincnt2 = 0xff;

								  }

							  }
						  }
						  if(Time30mincnt3!= 0xff )
						  {
							  if(Time30mincnt3)//alarm status handle
							  {
								  Time30mincnt3--;

									  playVolWarm3();
									  playVolWarm3();
									  playVolWarm3();
									  if(Time30mincnt3 == 0)
									  {
										  Time30mincnt3 = 0xff;

									  }


							  }
						  }

	    			  }


	    		  }
	    	  }
	      }

	  }
}
//////////////////////////////////////////////
//////////////////////////////////////////////
///GPS data////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////
unsigned char ASCII2char(unsigned char number)
{
	if((number <58)&&(number>47))
	{
		number = number-48;
	}
	else
	{
		number = 0;
	}
	return number;
}
unsigned long CountTime(CLOCK ctime)
{
	unsigned long timesec;
	timesec = ctime.hour*3600+ctime.minute*60+ctime.second;
	return timesec;
}

void GetTheGPSTime(unsigned char *tbuf)
{
	CLOCK GPStime;
	unsigned long nwtime,gtime;
	unsigned char i,dtime;
	unsigned char *pbuf;
	unsigned char *buf;
	pbuf = &GPStime.hour;
	buf = &curTime.hour;
	for(i= 0;i<3;i++)
	{
		*(pbuf+i) = 16*ASCII2char(tbuf[i*2])+ ASCII2char(tbuf[i*2+1]);
	}
	nwtime =CountTime(curTime);
	gtime = CountTime(GPStime);
	if(gtime>nwtime)
	{
		dtime = gtime-nwtime;
	}
	else
	{
		dtime = nwtime-gtime;
	}
	if((dtime >10) )//&&(dtime<200))
	{
		for(i= 0;i<3;i++)
		{
			*(buf+i) = (*(pbuf+i));
		}
		//set the time

		SetCurrentDateTime(&curTime);
	}
	//SetCurrentDateTime(&curTime);

}
void GetGPSLocation1(unsigned char *tbuf)
{
	unsigned char i;
	unsigned long *pbuf = (unsigned long *)&location.longtitude;
	*pbuf = 0;
	for(i= 0;i<9;i++)
	{
		if(i!=4)
		{
			*pbuf =ASCII2char(tbuf[i])+10*(*pbuf);
		}
	}

}
void GetGPSLocation2(unsigned char *tbuf)
{
	unsigned char i,j= 1;
	unsigned long *pbuf = (unsigned long *)&location.latitude;
	*pbuf = 0;
	for(i= 0;i<10;i++)
	{
		if(i!=5)
		{
			*pbuf =ASCII2char(tbuf[i])+10*(*pbuf);
		}
	}

}
void GetGPSLocation3(unsigned char *tbuf,unsigned short lenth)
{
	unsigned char i,k=0;
	unsigned short *pbuf = (unsigned short *)&location.altitude;
	*pbuf = 0;
	if(tbuf[0] == '-')
	{
				k = 1;
    }
	for(i= k;i<lenth;i++)
	{
		  if(tbuf[i] !='.')
		  {
			*pbuf =ASCII2char(tbuf[i])+10*(*pbuf);
		  }
	}
	if(k== 1)
	{
		*pbuf = 0-*pbuf;
	}

}
//精度1km/h
void GetGPSSpeed(unsigned char *tbuf,unsigned lenth)
{
	unsigned char i;
	RsSpeed = 0;
	for(i = 0;i<lenth;i++)
	{
		if(tbuf[i] == '.')
			break;
		RsSpeed = ASCII2char(tbuf[i])+10*RsSpeed;
	}
}
void GettheSinaldata(unsigned char *tbuf,unsigned char num)
{
    if(num == 1)
    {
    	radionum = ASCII2char(tbuf[0]);
    }
    else
    {
    	radionum = 16*ASCII2char(tbuf[0])+ASCII2char(tbuf[1]);
    }

}
//////////////////////////////////////////////////////
////////////////////////key press handle//////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void KepPressHandler()
{
	switch(keyval)
	{
	case MENU_KEY:
		MenutKeyHandler();
		keyval = KEY_NONE;
		break;
	case SELE_UP_KEY:
		SelectKeyHandler(1);
		keyval = KEY_NONE;
		break;
	case SELE_DOWN_KEY:
		SelectKeyHandler(0);
		keyval = KEY_NONE;
		break;
	case ENTER_KEY:
		OKKeyHandler();
		keyval = KEY_NONE;
		break;
	case PRIN_KEY:
		if((lcd_tcb.ListNb == 0 )&&(lcd_tcb.mode ==0))
		{
			PrintAllData();
			printer();
			lcd_clear(lineall);
			DisplayNormalUI();
			keyval = KEY_NONE;
		}
	    break;
	default:
		break;
	}

}
void keyscanhandler(void)
{
	unsigned char tempkey = KEY_NONE;
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14) == 0)
	{
		tempkey = MENU_KEY;
	}
	else if(GPIO_ReadInputDataBit( GPIOD, GPIO_Pin_15) == 0 )
	{
		tempkey = SELE_UP_KEY;
	}
	else if(GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_6) == 0 )
	{
		tempkey = SELE_DOWN_KEY;
	}
	else if(GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_7) == 0 )
	{
		tempkey = ENTER_KEY;
	}
	if(tempkey)
	{
		if(tempkey == keyvalbak)
		{

			if(keyvalcount !=0xffff)
				keyvalcount++;
			if((keyvalcount ==300)&& (keyvalbak == ENTER_KEY))
			{
				keyval = PRIN_KEY;
			}
		}
		else
		{
			keyvalcount = 1;
			keyvalbak = tempkey;
		}
	}
	else
	{
		if(keyvalcount >5)
		{
			if((keyvalcount >300)&& (keyvalbak == ENTER_KEY))
			{
				if(keyval == KEY_NONE)
				{
					keyval = KEY_NONE;
				}
			}
			else
			{
				keyval = keyvalbak;
			}
			keyvalcount =0;
		}
		keyvalcount = 0;
		keyvalbak = 0;
	}

}

//////////////////////////////////////////////////////
///////////////gprs数据处理/////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////



void FillGprsSendbuff(unsigned char devnum )
{
	unsigned char i,lenth;
	GprsSendbuff[0] = 0x7e;
	GprsSendbuff[1] = MessageArray[devnum].MeageDeviceId;
	GprsSendbuff[2] = MessageArray[devnum].MeageDeviceId>>8;
	GprsSendbuff[3] = gprsMessageheadsend.MessageAtributie.value>>8;
	GprsSendbuff[4] = gprsMessageheadsend.MessageAtributie.value;
	if(MessageArray[devnum].devptr == 0 )
	{
		lenth = 0;
		GprsSendbuff[3] = 0;
		GprsSendbuff[4] = 0;
	}
	else
	{
		lenth = gprsMessageheadsend.MessageAtributie.valuebit.lenth;
	}
	memcpy(&GprsSendbuff[5],&gprsMessageheadsend.phonenumber,6);
	gprsMessageheadsend.MessagelineID++;
	GprsSendbuff[11] = gprsMessageheadsend.MessagelineID>>8;
	GprsSendbuff[12] = gprsMessageheadsend.MessagelineID;
	GprsSendleth =13;
	if(MessageArray[devnum].devptr!=0)
	{
		if(gprsMessageheadsend.MessageAtributie.valuebit.packetbit == 1)
		{
			GprsSendbuff[GprsSendleth]= gprsMessageheadsend.packettoalnum>>8;
			GprsSendbuff[(GprsSendleth++)]= gprsMessageheadsend.packettoalnum;
			GprsSendbuff[(GprsSendleth++)]= gprsMessageheadsend.packetID>>8;
			GprsSendbuff[(GprsSendleth++)]= gprsMessageheadsend.packetID;
			GprsSendleth++;
		}
		memcpy(&GprsSendbuff[GprsSendleth],MessageArray[devnum].devptr,lenth);
	}
	GprsSendleth = lenth+GprsSendleth;
	GprsSendbuff[GprsSendleth] = GprsSendbuff[1];
	for(i=2;i<GprsSendleth;i++)
	{
		GprsSendbuff[GprsSendleth]^= GprsSendbuff[i];
	}
	GprsSendleth++;
	GprsSendbuff[GprsSendleth] = 0x7e;
	GprsSendleth++;
	GprsSendbuff[GprsSendleth] = 0x1a;
	GprsSendleth++;
}

unsigned char  GetAuthData(rt_device_t device)
{
	unsigned char i,lenth,result;
	struct stm32_serial_device* uart = (struct stm32_serial_device*) device->user_data;
	lenth =  uart->int_rx->rx_buffer[3];
	lenth = lenth-3;
	for(i =0;i<lenth;i++)
	{
		devauth.authword[i] = uart->int_rx->rx_buffer[i+15];
	}
	devauth.lenth = lenth;
	if( uart->int_rx->rx_buffer[14] == 2||uart->int_rx->rx_buffer[14] == 3)
	{
		return 0;
	}
	return 1;

}
void filedevsenddata(unsigned char cmdnum)
{
	unsigned char i;
	devsendata.MessagelineID = gprsMessageheadrev.MessagelineID;
	devsendata.updatacmd = cmdnum;

	switch(cmdnum)
	{

	case 0:
		devsendata.updata[0] = Parameter.standeryear;
		devsendata.updata[1] = Parameter.modifyNb;
		devsendata.lenth =5;
		break;
	case 1:
		for(i = 0;i<18;i++)
		{
			devsendata.updata[i] = pTable.DriverLisenseCode[i];
		}
		devsendata.lenth =21;
		break;
	case 2:
		for(i = 0;i<6;i++)
		{
			devsendata.updata[i] = *((unsigned char *)&curTime+i);
		}
		devsendata.lenth =9;
		break;
	case 3:
		for(i = 0;i<6;i++)
		{
			devsendata.updata[i] = *((unsigned char *)&curTime+i);
		}
		for(i = 6;i<12;i++)
		{
			devsendata.updata[i] = *((unsigned char *)&Parameter.InstallTime+i-6);
		}
		for(i = 12;i<16;i++)
		{
			devsendata.updata[i] = *((unsigned char *)&Parameter.DriverDistace+i-12);
		}
		for(i = 16;i<20;i++)
		{
			devsendata.updata[i] = *((unsigned char *)&Parameter.StarDistance+i-16);
		}
		devsendata.lenth =23;
		break;
	case 4:
		for(i = 0;i<6;i++)
		{
			devsendata.updata[i]  = *((unsigned char *)&curTime+i);
		}
		for(i = 6;i<8;i++)
		{
			devsendata.updata[i]  = *((unsigned char *)&Parameter.PulseCoff+i-6);
		}
		devsendata.lenth =11;
		break;
	case 5:
		for(i = 0;i<41;i++)
		{
			devsendata.updata[i]  = *((unsigned char *)&Parameter.AutoInfodata+i);
		}
		devsendata.lenth =44;
		break;
	case 6:
		for(i = 0;i<6;i++)
		{
			devsendata.updata[i]  = *((unsigned char *)&curTime+i-0);
		}
		devsendata.updata[i]  = CurStatus;
		for(i = 7;i<87;i++)
		{
			devsendata.updata[i]  = *((unsigned char *)&Parameter.singalname+i-7);
		}
		devsendata.lenth =90;
		break;
	case 7:
		for(i = 0;i<6;i++)
		{
			devsendata.updata[i]  = *((unsigned char *)&Parameter.typedata+i-23);
		}
		devsendata.lenth =9;
		break;
	default:
		break;

	}
}
unsigned short FillthegprssendBlock(unsigned char *buf,unsigned char block,unsigned short *count)
{
	static unsigned long readnum;
	static unsigned long readaddr;
	unsigned short renum;
	unsigned short BlockSize;
	unsigned long STOPp,STOPb,startbase,endbase;
	unsigned long flag;
	switch (block)
	{
		case 0x08:
			BlockSize = DRV_SPEED_BLOCK;
			STOPp = pTable.BaseData.CurPoint;
			flag = pTable.BaseData.finshflag;
			startbase = BASEDATA_BASE;
			endbase = BASEDATA_END;
			break;
		case 0x09:
			BlockSize = LOCATION_BLOCK;
			STOPp = pTable.LocationData.CurPoint;
			flag = pTable.LocationData.finshflag;
			startbase = LOCATION_BASE;
			endbase = LOCATION_END;
			break;
		case 0x10:
			BlockSize = DOUBLT_BLOCK;
			STOPp = pTable.OverSpeedRecord.CurPoint;
			flag = pTable.OverSpeedRecord.finshflag;
			startbase = DPD_BASE;
			endbase = DPD_END;
			break;
		case 0x11:
			BlockSize = OVERDRV_BLOCK;
			STOPp = pTable.DoubtPointData.CurPoint;
			flag = pTable.BaseData.finshflag;
			startbase = OVERDRV_BASE;
			endbase = OVERDRV_END;
			break;
		case 0x12:
			BlockSize = DRV_RG_BLOCK;
			STOPp = pTable.DriverReRecord.CurPoint;
			flag = pTable.DriverReRecord.finshflag;
			startbase = DRVRG_BASE;
			endbase = DRVRG_END;
			break;
		case 0x13:
			BlockSize = POW_BLOCK;
			STOPp = pTable.PowerOffRunRecord.CurPoint;
			flag = pTable.PowerOffRunRecord.finshflag;
			startbase = POWER_BASE;
			endbase = POWER_END;
			break;
		case 0x14:
			BlockSize = PARA_BLOCK;
			STOPp = pTable.ModifyRecord.CurPoint;
			flag = pTable.ModifyRecord.finshflag;
			startbase = PARA_BASE;
			endbase = PARA_END;
			break;
		case 0x15:
			BlockSize = JN_BLOCK;
			STOPp = pTable.journalRecord.CurPoint;
			flag = pTable.journalRecord.finshflag;
			startbase = JN_BASE;
			endbase =  JN_END;
			break;
		default:
			break;

	}
	if( flag == 0xeaea)
	{
		if((*count)==0)
		{
			readaddr = STOPp;
			if(readaddr > startbase)
			{
				readaddr = readaddr-BlockSize;
			}
			else
			{
				readaddr = (endbase-startbase+1)/BlockSize;
				readaddr = readaddr*BlockSize+startbase;
			}
			readnum = (endbase -startbase+1)/BlockSize;
			readnum = readnum*BlockSize;
			renum = 137;
			if(readnum%renum)
			{
				*count = readnum/renum;
			}
			else
			{
				*count = readnum/renum-1;
			}
			readnum = readnum-renum;
			SPI_FLASH_BufferRead(SPI1 ,&buf[0] ,readaddr, renum);

		}
		else
		{
			if(readaddr > startbase)
			{
				readaddr = readaddr-BlockSize;
			}
			else
			{
				readaddr = (endbase-startbase+1)/BlockSize;
				readaddr = readaddr*BlockSize+startbase;
			}
			(*count)--;
			if((*count)!= 0)
			{
				renum = 137;
				readnum = readnum-renum;
			}
			else
			{
				renum = readnum;
			}
			SPI_FLASH_BufferRead(SPI1 ,buf ,STOPp, renum);
		}
	}
	else
	{
		if((*count)==0)
		{
			readaddr = STOPp;
			readnum = (STOPp -startbase)/BlockSize;
			readnum = readnum*BlockSize;
			if(readnum  == 0)
			{
				renum = 0;
				GettheBlock(buf,block,0);
				return renum;
			}
			else
			{
				readaddr = readaddr-BlockSize;
				GettheBlock(buf,block,readnum);
				if(readnum%137)
				{
					*count = readnum/137;
					if((*count)!= 0)
					{
						renum = 137;
						readnum = readnum-renum;
					}
					else
					{
						renum = readnum;
					}

				}
				else
				{
					*count = readnum/renum-1;
					renum = 137;
					readnum = readnum-renum;
				}
				SPI_FLASH_BufferRead(SPI1 ,&buf[0] ,STOPp, renum);
			}

		}
		else
		{
			readaddr = readaddr-BlockSize;
			(*count)--;
			if((*count)!= 0)
			{
				renum = 137;
				readnum = readnum-renum;
			}
			else
			{
				renum = readnum;
			}
			SPI_FLASH_BufferRead(SPI1 ,buf ,STOPp, renum);
		}
	}
	return renum;

}

void UpdataWebDataHandle(rt_device_t device)
{
	unsigned char i,lenth,updatacmd;
	unsigned short renum;
	static unsigned short gcount=0;
	static unsigned char flagpag;
	static unsigned char packnum =0;
	struct stm32_serial_device* uart = (struct stm32_serial_device*) device->user_data;
	updatacmd = uart->int_rx->rx_buffer[12];
	switch(updatacmd)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			memset(&gprsMessageheadsend.MessageAtributie,0,2);
			filedevsenddata(updatacmd);
			gprsMessageheadsend.MessageAtributie.valuebit.lenth= devsendata.lenth;
			FillGprsSendbuff(3);
			MessagDevicIDflag.updriverdataflag = 0;

			break;
		case 8:
		case 9:
		case 0x10:
		case 0x11:
		case 0x12:
		case 0x13:
		case 0x14:
		case 0x15:
			memset(&gprsMessageheadsend.MessageAtributie,0,2);
			devsendata.MessagelineID = gprsMessageheadrev.MessagelineID;
			renum = FillthegprssendBlock(devsendata.updata,updatacmd,&gcount);
			gprsMessageheadsend.MessageAtributie.valuebit.lenth = renum;
			if(gcount == 0)
			{
				if(flagpag)
				{
					gprsMessageheadsend.MessageAtributie.valuebit.packetbit = 1;
				}
				MessagDevicIDflag.updriverdataflag = 0;
				flagpag =0;

			}
			else
			{
				if(!flagpag)
				{
					packnum = 1;
					gprsMessageheadsend.packettoalnum = gcount+1;
					devsendata.MessagelineID = gprsMessageheadrev.MessagelineID;
					gprsMessageheadsend.packetID = packnum;
				}
				gprsMessageheadsend.MessageAtributie.valuebit.packetbit = 1;
				flagpag = 1;

			}
			FillGprsSendbuff(3);
			break;
		default:
			break;

	}
	if((updatacmd<0x0A)||((updatacmd>0x0F)&& (updatacmd<0X16)))
	{
		gprsflag = CIPTRANSFERE;
		gprsCmdflag = SEND;
	}
}
void GetServerDatahandle(rt_device_t device)
{
	if(MessagDevicIDflag.devregAckflag == 1)//已经注册成功
	{
		MessagDevicIDflag.devregAckflag =0;
		if(GetAuthData(device)==1)
		{
			memset(&gprsMessageheadsend.MessageAtributie,0,2);
			gprsMessageheadsend.MessageAtributie.valuebit.lenth= devauth.lenth;
			gprsflagbak = gprsflag;
			FillGprsSendbuff(2);
			gprsflag = CIPTRANSFERE;
			gprsCmdflag = SEND;
		}
	}
	else if(MessagDevicIDflag.gprssenbreakheart==1)
	{
		MessagDevicIDflag.gprssenbreakheart=0;
		FillGprsSendbuff(4);
		gprsflag = CIPTRANSFERE;
		gprsCmdflag = SEND;

	}
	else if(MessagDevicIDflag.updriverdataflag == 1)//上传数据
	{
		UpdataWebDataHandle(device);

	}

}
void Gprs_Cn_handle()
{
	switch (gprsAuthflag)
	{
		case REGISTER:
			gprsflagbak = gprsflag;
			memset(&gprsMessageheadsend.MessageAtributie,0,2);
			gprsMessageheadsend.MessageAtributie.valuebit.lenth= 5;
			FillGprsSendbuff(1);
			gprsflag = CIPTRANSFERE;
			gprsAuthflag = DOGPRSWORK;
			break;
		case DOGPRSWORK:
			GetServerDatahandle(&uart4_device);
			break;
		default:
			break;
	}
}
void Gprs_tranfer()
{
	switch(gprsCmdflag)
	{
		case SEND:
			rt_device_write(&uart4_device, 0,"AT+CIPSEND\r", 11);
			rt_device_write(&uart2_device, 0,"AT+CIPSEND\r\n", 12);
			gprsCmdflag = WAIT;
			break;
		case SENDDATA:
			if(timecnt.Time10sCnt == 0)
			{
				rt_device_write(&uart4_device, 0,GprsSendbuff, GprsSendleth);
				rt_device_write(&uart2_device, 0,GprsSendbuff, GprsSendleth);
				gprsCmdflag = WAIT;
			}
			break;
		case WAIT:
			break;
		case RECEIVE:
			gprsflag = gprsflagbak;
			break;
		default:
			break;
	}
}
void GPRS_Handle()
{
	switch(gprsflag)
	{
		case CIPSTART:
			if(timecnt.Time10sCnt == 0)
			{
				rt_device_write(&uart4_device, 0,"AT+CIPSTART=", 12);
				rt_device_write(&uart4_device, 0,"\"TCP\",", 6);
				rt_device_write(&uart4_device, 0,ipaddress, sizeof(ipaddress)-1);
				rt_device_write(&uart4_device, 0,",", 1);
				rt_device_write(&uart4_device, 0,depoint, sizeof(depoint)-1);
				rt_device_write(&uart4_device, 0,"\r", 1);
				gprsflag = PROCEIDEL;
			}
			break;
		case CIPCONNECT:
			Gprs_Cn_handle();
			break;
		case CIPTRANSFERE:
			Gprs_tranfer();
			break;
		default:
			break;
	}
}
//////////////////////////////////////////////////////
///////////////////////////////////////////////////////
/////////IC 卡数据处理//////////////////////////////////
void GetTheDriverNumber()
{
	I2C_LowLevel_Init(I2C1);

	I2C_Master_BufferRead(I2C1,
								  0xa0,
	                              32,
	                              18,
	                              DriverLisenseCode);
}
void IckaHandler()
{
	unsigned char i;
	if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4) == 1)&&(Datastatusdata.keeprestatus ==0))
	{
		insertcount++;
		if(insertcount == 100)
		{
			insertcount = 0;
			Datastatusdata.keeprestatus = 1;
			DriverRegstatus = DRIVER_REG_IN;
			GetTheDriverNumber();
			if(!memcmp(Parameter.DriverLisenseCode,DriverLisenseCode,18))
			{
				DriverFlag = 1;
			}
			else
			{
				DriverFlag = 0;
			}
			memcpy(Parameter.DriverLisenseCode,DriverLisenseCode,18);
			AlarmFlag &=~ ALARM_NOT_RE;
			Time30mincnt2 = 0;
		}
	}
	else if((Datastatusdata.keeprestatus ==  1) &&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4) ==0))
	{
		Datastatusdata.keeprestatus = 0;
		DriverRegstatus = DRIVER_REG_OUT;
		DriverStatus &= ~DRIVING_STAR;
		DriverStatus |= DRIVING_STOP_DRIVER;//连续驾驶结束
		DriverStatus &= ~DRIVING_OVERTIME;
		AlarmFlag &= ~ALARM_OVER_TIME;
		Time30mincnt1 = 0;

	}
	else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4) ==0)
	{
		insertcount = 0;
		if((AlarmFlag&ALARM_NOT_RE)!= ALARM_NOT_RE)
			AlarmFlag |= ALARM_NOT_RE;
	}

}


 
