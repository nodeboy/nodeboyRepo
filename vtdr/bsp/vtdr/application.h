#ifndef application_h
#define application_h

#define TRUE 1
#define FALSE 0


#define GetSpeedSelf    	0
#define POWERON_LINE_EN     0
#define StatusPol           0
#define StationStamp_EN     0
#define guizhoufile    	    0
#define guizhou             1
#define GPS                 0


#define OpenDoorDeal    	0
#define RTC8025             1     //ʱ��оƬѡ�񿪹�
#define GetSpeedStatusBy232 0     //ͨ��ں�������ͨѶ��ȡ�ٶȺ�ȫ��״̬����
//#define WATCH_DOG_EN    	1     // ���Ź�����
#define RPM_EN				0     //������ת�ٿ���
#define SectionAlarm_EN     0     //��·�α�������
#define OpenDoorAlarm		0     //������ʻ��������
#define Test                0	  //���Թ�װ
#define Status14			0     //����8��״̬
//**************************************//
#define AlarmRecord         0     //���ٺ�ƣ�ͼ�ʻ������¼
////////////////////////////////////////////////////////////////////////

//2004.03.02 panhui �人�������߸�Ϊ2Сʱ���п�����ʻ������
#if OpenDoorAlarm
#define GotoSleepTime 36000     //����ʱ�䣽120����
#else
#define GotoSleepTime 1500     //����ʱ�䣽300�룽5����
#endif

#define DoorNBLimit   6       //200ms�п����ź���Ч�ĸ�������

//�����ж����ȼ�
#define USART1_INT_PRI	4
#define IRQ0_INT_PRI	7       //USB
#define IRQ1_INT_PRI    3       //ENGINE SPEED
#define IRQ2_INT_PRI    6       //SPEED
#define TC0_INT_PRI     0       //TICK
#define Clock_PRI     	5       //CLOCK 
#define DriverCard_PRI 	5		//DRIVERCARD
#define TC2_INT_PRI     6       //��ʱ��
#define TC1_INT_PRI     5       //door��ʱ��


typedef struct
{

	unsigned char year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	
} CLOCK;
typedef struct
{

	int year;
	int month;
	int day;
	int time;
	
} DateTime;

typedef struct
{
	unsigned char year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char type;
	unsigned char reserved;
} UpDownCLOCK;

typedef struct
{

	unsigned short type;
	unsigned char year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	
} Record_CLOCK;

typedef struct
{
	unsigned char speed;
	unsigned long bound;
} RoadSection;
typedef struct
{
	unsigned char AutoVincode[17];
	unsigned char AutoCode[12];
	unsigned char AutoSort[12];

}AutoInfo;
typedef struct
{
	unsigned char d0[10];
	unsigned char d1[10];
	unsigned char d2[10];
	unsigned char d3[10];
	unsigned char d4[10];
	unsigned char d5[10];
	unsigned char d6[10];
	unsigned char d7[10];
}StatusSingal;
typedef struct
{
	unsigned char productccc[7];
	unsigned char type[16];
	unsigned char productdate[3];
	unsigned char productline[4];
	unsigned char reserve[5];
}ProductType;
typedef struct
{

	unsigned long  DriverDistace;
	unsigned long  StarDistance;
	unsigned long  LimitSpeed;
	unsigned short mark;//
	unsigned char  sn[22];//
	unsigned char standeryear;
	unsigned char modifyNb;
	unsigned char signalstatus;
	StatusSingal singalname;
	ProductType  typedata;
	unsigned char  AlarmSound;
	CLOCK   time;
	CLOCK   InstallTime;
	unsigned char  PulseNumber;
	unsigned short  PulseCoff;
	unsigned char  DriverLisenseCode[18];
	unsigned char SectionNumber;
	RoadSection section[20];

	unsigned char  reserved1[92];//
	unsigned char SectionNumber1;
	AutoInfo AutoInfodata;
	RoadSection section1[20];
} StructPara;

typedef struct
{
	unsigned long  DriverCode;
	unsigned char  DriverLisenseCode[18];
} DRIVER;

typedef struct
{

	unsigned long  BaseAddr;
	unsigned long  EndAddr;//�����ַ
	unsigned long  CurPoint;//��ǰָ���ַ,ָ����һ����ݿɴ�ŵ�λ��
	//unsigned long  BakPoint;
	unsigned long  finshflag;
	
} StructPT;

typedef struct
{

	unsigned long	 TotalDistance;
	unsigned long	 DriverCode;
	unsigned short  Available;//=0��ʾû��ʹ�ô������>0��ʾ�������Ч
	StructPT DoubtPointData;// 疑点数据
	StructPT LocationData;
	StructPT OverSpeedRecord;//超速记录
	StructPT PowerOffRunRecord;
	StructPT ModifyRecord;
	StructPT BaseData;//行驶速度记录
	StructPT DriverReRecord;
	StructPT journalRecord;
	CLOCK	 LastUploadTime;
	unsigned char   DriverLisenseCode[18];//��ʻ֤����
} PartitionTable;
typedef struct
{
	unsigned char Time200msflag:1;
	unsigned char Time1sflag:1;
	unsigned char Ver1sflag:1;
	unsigned char Time1minflag:1;
}Timeflag;
typedef struct
{
	unsigned char Time1msCnt;
	unsigned char Time200msCnt:3;
	unsigned char Time1sCnt:6;
	unsigned char Time10sCnt:4;
	unsigned char Time5minCnt:3;
	unsigned long breaktimecout;
}TimeCnt;


//////////////////gprs data////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
typedef enum GPRS_process
{
	NONEPOWER,
	CIPSTART,
	CIPCONNECT,
	CIPTRANSFERE,
	ENDPOWER,
	PROCEIDEL
}gprshandleflag;
typedef enum GPRS_AUTH_process
{
	REGISTER,
	DOGPRSWORK,
	AUTHIDLE,
}gprsAuth;
typedef enum GPRS_CMD_process
{
	SEND,
	SENDDATA,
	WAIT,
	RECEIVE,
}gprsCMD;
typedef struct
{
	unsigned short MessageID;//消息ID
	union
	{
	    unsigned short value;
		struct
		{
			unsigned short lenth:10;//长度
			unsigned char addpassword:3;//数据加密方式
			unsigned char packetbit:1;//是否分包
			unsigned char reserve:2;//保留
		}valuebit;
	}MessageAtributie;
	unsigned char phonenumber[6];
	unsigned short MessagelineID;
	unsigned short packettoalnum;
	unsigned short packetID;
}GPRS_Message_head;
typedef struct
{
	unsigned short lineAck;
	unsigned short lineId;
	unsigned short result;
}Devic_ACK;
Devic_ACK devack;
typedef struct
{
	unsigned short proviceId;
	unsigned short cityId;
	unsigned char  ManufactoryID[5];
}Devic_Reg;
Devic_Reg devreg;
typedef struct
{
	unsigned char authword[10];
	unsigned short lenth;
}Dev_AUTH;
Dev_AUTH devauth;
typedef struct
{
	unsigned short MessagelineID;
	unsigned char updatacmd;
	unsigned char updata[137];
	unsigned short lenth;
}Dev_SENDDATA;
Dev_SENDDATA devsendata;
typedef struct
{
	unsigned short MeageDeviceId;
	void *devptr;
}Message_Array;

typedef struct
{
	unsigned char platformgernelAckflag:1;//0x8001
	unsigned char devregAckflag:1;        //0x8100
	unsigned char setdevparaflag:1;//0x8103
	unsigned char getdevparaflag:1;//0x8104
	unsigned char devcontrolflag:1;//0x8105
	unsigned char getlocationflag:1;//0x8201
	unsigned char traceloactiontempflag:1;//0x8202
	unsigned char textdownsentflag:1;//0x8300
	unsigned char eventsetflag:1;//0x8301
	unsigned char issuesentdownflag:1;//0x8302
	unsigned char infomenuplayflag:1;//0x8303
	unsigned char infoserviceflag:1;//0x8304
	unsigned char celluarcallbackflag:1;//0x8400
	unsigned char phonetextsetflag:1;//0x8401
	unsigned char verticalcontraolflag:1;//0x08500
	unsigned char setcylezoonflag:1;//0x8600
	unsigned char delecylezoonflag:1;//0x8601
	unsigned char settanglezoonflag:1;//0x8602
	unsigned char delettanglezoonflag:1;//0x8603
	unsigned char setmultsizezoonflag:1;//0x8604
	unsigned char delemultsizezoonflag:1;//0x8605
	unsigned char setrouteflag:1;//0x8606
	unsigned char delrouteflag:1;//0x8607
	unsigned char updriverdataflag:1;//0x08700
	unsigned char downdriverparaflag:1;//0x8701
	unsigned char multdataupflag:1;//0x8800
	unsigned char imageupflag:1;//0x8801
	unsigned char gprsAuthsucess:1;//add by needed
	unsigned char gprssenbreakheart:1;//add by needed

}MessagDevicID_FLAG;
MessagDevicID_FLAG MessagDevicIDflag;
gprsAuth gprsAuthflag;
gprsCMD gprsCmdflag;
GPRS_Message_head gprsMessageheadsend;
GPRS_Message_head gprsMessageheadrev;

typedef enum
{
	HATR_BREAK= 1,
	TCP_ACK

}GPRS_PARA_ID;
typedef struct
{
	unsigned long heartbreaktime;//second for the unit
	unsigned long tcpacktime;//second for the unit
	unsigned long tcpresendtime;//second for the unit
}GPRS_PARA_VALUE;
GPRS_PARA_VALUE gprsparavalue;

/*handler define*/
#define DOUBTPOINTDATA  	0
#define OVERSPEEDRECORD    	1
#define POWEROFFRUNRECORD   2
#define DOUBLTPOINT 		3
#define BASEDATA 			4
#define LOCATIONDATA  		5
#define RPMDATA  			6
#define JOURNALDATA  	   7
#define OILPRESSDATA 		8
#define STATUS14DATA        9
/*  */
#define DoubtDataBlockSize 210

/* ������ݷ���� */
#define DOUBTPOINTDATA_EN  		1

#if AlarmRecord
#define OVERSPEEDRECORD_EN    	1
#else
#define OVERSPEEDRECORD_EN    	0
#endif

#define POWEROFFRUNRECORD_EN   	0	
#define RUNRECORD360h_EN 		1
#define BASEDATA_EN 			1
#define OILDATA_EN  			0

#if RPM_EN
#define RPMDATA_EN  			1
#else
#define RPMDATA_EN				0
#endif

#if Status14
#define Status14DATA_EN  			1
#else
#define Status14DATA_EN				0
#endif

#define TEMPERATUREDATA_EN  	0
#define OILPRESSDATA_EN 		0

//#define PartitionTableFlag  (DOUBTPOINTDATA_EN << DOUBTPOINTDATA)|(OVERSPEEDRECORD_EN << OVERSPEEDRECORD)|(RUNRECORD360h_EN << DOUBLTPOINT)|(BASEDATA_EN << BASEDATA)|(RPMDATA_EN << RPMDATA)|(Status14DATA_EN << STATUS14DATA)
#define PartitionTableFlag 25
#define DATAFLASH_BASE    0x00000000
#define PARAMETER_BASE    (DATAFLASH_BASE+0x00000000)
#define PartitionTable_BASE (DATAFLASH_BASE+0x00001000)

#define BASEDATA_BASE DATAFLASH_BASE+0x02000 //48h data 126byte/block
#define BASEDATA_END  DATAFLASH_BASE+0x5afff

#define DPD_BASE      DATAFLASH_BASE+0x5b000 //100条
#define DPD_END       DATAFLASH_BASE+0x60fff

#define OVERDRV_BASE  DATAFLASH_BASE+0x61000 //100条
#define OVERDRV_END   DATAFLASH_BASE+0x62fff

#define LOCATION_BASE DATAFLASH_BASE+0x63000//360个小时
#define LOCATION_END  DATAFLASH_BASE+0x9dfff

#define DRVRG_BASE    DATAFLASH_BASE+0x9e000//200条
#define DRVRG_END     DATAFLASH_BASE+0x9ffff

#define POWER_BASE    DATAFLASH_BASE+0xa0000//100条
#define POWER_END     DATAFLASH_BASE+0xa0fff

#define PARA_BASE     DATAFLASH_BASE+0xa1000//100条
#define PARA_END      DATAFLASH_BASE+0xa1fff

#define JN_BASE       DATAFLASH_BASE+0xa2000//10条
#define JN_END    	  DATAFLASH_BASE+0xa2fff

//2004.07.23
#define DOWNLOADTIME_BASE DATAFLASH_BASE+0x00400
#define DOWNLOADTIME_END DATAFLASH_BASE+0x007f4
#define UPLOADTIME_BASE DATAFLASH_BASE+0x00800
#define UPLOADTIME_END DATAFLASH_BASE+0x00bf4
#define SETCHCOTIME_BASE DATAFLASH_BASE+0x00c00
#define SETCHCOTIME_END DATAFLASH_BASE+0x00ff4
//2004.07.23

#define DoubtDataSpace 110 

/*�����ɵ���ݸ�ʽ*/
typedef struct{
	unsigned char speed;//ÿ0.2���ٶ�
	unsigned char status;//ÿ0.2��8λ״̬
}DoubtData;
typedef struct{
	unsigned char longtitude[4];//经度
	unsigned char latitude[4];//纬度
	short  altitude;
}SizeData;
typedef struct{
	unsigned char  DriverLisenseCode[18];
	CLOCK   StopTime;
	DoubtData data[100];
	SizeData vaildlocation;
}DoubtDataBlock;
typedef struct{
	CLOCK basedataclk;
	unsigned char speed[60];
	unsigned char status[60];
}BaseDataBlock;
typedef struct{
	CLOCK lclk;
	SizeData Lsizedata[60];
    unsigned char speed1min[60];
}LocationBlock;
typedef struct{
	unsigned char  DriverLisenseCode[18];
	CLOCK startdrivertime;
	CLOCK enddrivertime;
	SizeData startlocation;
	SizeData endlocation;
}OverDriverBlock;
typedef struct{
	unsigned char  DriverLisenseCode[18];
	CLOCK happentime;
	unsigned char registerstatus;

}DriverRegisterBlock;
typedef struct{
	CLOCK powertime;
	unsigned char powerstatus;
}PowerBlock;
typedef struct{
	CLOCK modifytime;
	unsigned char modifystatus;
}ParaModifyBlock;
typedef struct{
	unsigned char speedstatus;
	CLOCK journalstartime;
	CLOCK jouranlendtime;
	unsigned char speed[60];
	unsigned char rspeed[60];
}JournalBlock;



typedef struct{
	Record_CLOCK  dt;
}OTDR_start;
typedef struct{
	Record_CLOCK  dt;
	unsigned long  TotalDistance;
	unsigned long MinuteNb;
	DRIVER driver;
}OTDR_end;
typedef struct{
	OTDR_start start;
	OTDR_end end;
}OTDR;

typedef struct{
	Record_CLOCK  dt;
	DRIVER driver;
}AnAlarmData;

typedef struct{
	unsigned char datacode;
	unsigned char blockname[18];
	unsigned long blocklenth;
}UsbBlock;
typedef struct{
    unsigned short blocknum;
	UsbBlock UsbBlock0;
	unsigned char standeryear;
	unsigned char modifynum;
	UsbBlock UsbBlock1;
	unsigned char drivercode[18];
	UsbBlock UsbBlock2;
	CLOCK usbtime;
	UsbBlock UsbBlock3;
	CLOCK drivertime;
	CLOCK installtime;
	unsigned long startdistance;
	unsigned long totaldistance;
	UsbBlock UsbBlock4;
	UsbBlock UsbBlock5;
	UsbBlock UsbBlock6;
	UsbBlock UsbBlock7;
	UsbBlock UsbBlock8;
	UsbBlock UsbBlock9;
	UsbBlock UsbBlock10;
	UsbBlock UsbBlock11;
	UsbBlock UsbBlock12;
	UsbBlock UsbBlock13;
	UsbBlock UsbBlock14;
	UsbBlock UsbBlock15;
    unsigned char checksum;
}UsbBlockData;

/*����״̬�ֽ���ÿλ�ĺ���*/
#define DOOR	   8
#define AIRPRESS   7
#define STATION    6
#define HEADLIGHT  5
#define RIGHTLIGHT 4
#define HORN	   3
#define BREAK      2
#define LEFTLIGHT  1
#define POWERON	   0
/* ����TIMECHANGE�� */
#define YEAR_CHANGE		0
#define MONTH_CHANGE	1
#define DAY_CHANGE		2
#define HOUR_CHANGE		3
#define MINUTE_CHANGE	4
#define SECOND_CHANGE	5
//
#define DRIVING_STAR  0x01
#define DRIVING_STOP  0X02
#define DRIVING_STOP_DRIVER 0x04
#define DRIVING_OVERTIME 0x08

//
#define DRIVER_REG_IN   0x01
#define DRIVER_REG_OUT  0x02

//
#define POWER_ON   0x01
#define POWER_OFF  0x02

//Alarmstatus
#define ALARM_OVER_TIME 0x01
#define ALARM_NOT_RE    0x02
#define ALARM_OVER_SPEED 0x04
#define ALARM_SPEED_ABOR  0x08

#define SpeedPowerOff    0
enum
{
	NONE_OPR, //木有操作
	NORMAL,   //速度正常
	ABORT,    //速度异常
	RECORD_STARTTIME,   //记录数据
	RECORD_DATA
}CMD_JOURNAL;
enum
{
	NONE_DB,
	DBRECORD_START,
	DBRECORD_END
}CMD_DB;

typedef struct
{
	Record_CLOCK dt;
}RecordData_start;
typedef struct
{//��ʻ��¼
	Record_CLOCK dt;
	unsigned long DistancePulse;
	unsigned long DriverCode;
}RecordData_end;

//�����ӡ����ʾ��ÿ����ƽ���ٶ�
typedef struct
{//����ʱ��

	unsigned char hour;
	unsigned char minute;
	unsigned char speed;
	unsigned long  DriverCode;
	
} PrintSpeed;


#define MAX_BUFF 2048
#define RECVHEADSIZE 4
#define SENDHEADSIZE 8
#define SENDFILEBUFF 1000
#define SENDFILESUM	 2

//232
#define BAUDS4800 427
#define BAUDS9600 213
#define BAUDS38400 53
#define DataLength  256
#define CmdLength   17


#endif
