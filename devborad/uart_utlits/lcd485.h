/*
 * lcd485.h
 *
 *  Created on: Nov 21, 2013
 *      Author: konlese
 */

#ifndef LCD485_H_
#define LCD485_H_
#define PIC_ID_MAX 20
typedef enum
{
	///文本显示命令
	TEXT_8 = 0x53,
	TEXT_16,
	TEXT_32,
	TEXT_12 = 0x6e,
	TEXT_24 ,
	//图片显示粘贴命令
	PIC_ALL,
	PIC_CUT1,
	PIC_CUT2 = 0x9c,
	//区域操作
    AREA_SET = 0x64,
    AREA_CLERA = 0x52
}LCD_CMD;

typedef enum
{
	PIC01 =1,
	PIC02,
	PIC03,
	PIC04,
	PIC05,
	PIC06,
	PIC07,
	PIC08,
	PIC09,
	PIC10,
	PIC11,
	PIC12,
	PIC13,
	PIC14,
	PIC15,
	PIC16,
	PIC17,
	PIC18
}PIC_ID;

typedef struct
{
	unsigned char GprsStatus:1;
	unsigned char NetStatus:1;
	unsigned char MessageStatus:1;
	unsigned char ControlStatus:1;
	unsigned char DirectioStatus:1;
	unsigned char WarnStatus:1;
	unsigned char GpsStatus:1;
	unsigned char RunStatus:1;
	unsigned char singalchange;
}Signal_Status;

////////////显示屏父子树///////////////////
typedef struct
{
	 char ChildrenList;//孩子链表
	 char FatherList;//父亲链表
	 char FatherNB;//父亲节点号
	 char ChildrenNB;//孩子节点数目
	 PIC_ID picId;//图片号

} MENU_NODE;
typedef struct
{
	MENU_NODE * ListPt;//父亲指针
	unsigned char NodeNumber;//节点号

} NODE_LIST;
typedef struct
{
	unsigned char mode;
	unsigned char ListNb;
	unsigned char NodeNb;
	unsigned char chiNb;
	unsigned char chiNbbak;

} LCDTCB;
typedef struct
{
	Signal_Status signalStatus;
	LCDTCB lcdtcb;
	unsigned char changePic;
	unsigned char changenode;
}DProcess_Status;
typedef struct
{
	unsigned char lcdcmd;
}Packet_Buf;
typedef struct
{
	DProcess_Status DProcessStatus;
	NODE_LIST NodeListTable[5];
	void (* handler)(DProcess_Status *pstatus,NODE_LIST *nodelist);

}DProcess_Var;




#endif /* LCD485_H_ */
