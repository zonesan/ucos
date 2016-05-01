#ifndef SLIC_H
#define SLIC_H

#include "def_type.h"

#define CHANNEL_START	0
#define CHANNEL_NUM	4
#define STK_SIZE_TwoRiscSlicTA 4000


unsigned int StkTwoRiscSlicTA[STK_SIZE_TwoRiscSlicTA];

#define	AUDIO_ENCODING_ULAW	(1)	/* ISDN u-law */
#define	AUDIO_ENCODING_ALAW	(2)	/* ISDN A-law */
#define	AUDIO_ENCODING_LINEAR	(3)	/* PCM 2's-complement (0-center) */

#define QUEUE_LEN 50

typedef struct
{
	int data[QUEUE_LEN];
	int front, rear;
	int isNull;
}Queue;

void Init_Queue(Queue *sq);
int Ins_Queue(Queue *sq,int x);
int Del_Queue(Queue *sq);
int Queue_Full(Queue *sq);
int Queue_empty(Queue *sq);

typedef struct {
	enum{ MAKEbUSY, STATEcHANGE,DIALtONE,INITIALIZING,POWERuP,CALIBRATE,PROGRAMMEDrING,POWERdOWN,POWERlEAKtEST,
		MAKErINGbACKjAPAN, MAKEbUSYjAPAN, RINGbACKjAPAN, MAKErINGbACK,RINGbACK,MAKErEORDER,REORDER,MAKEcONGESTION,CONGESTION,PRENEON,NEON,
		CALLBACKpROGRAMMED, BUSY, CALLBACK, CALLING, MAKEoFFHOOK,ONHOOK, OFFHOOK, DIGITDECODING,
	LOOPtRANSITION, FIRSTrING, DEFEREDcALLBACKpROGRAMMED,CALLERiD,RINGING,DTMFtRANISTION} state,newState,previousState;
	ULONG cs;
	ULONG reset;
	ULONG interrupt;
	CHAR	version,type;
	BYTE 	eventEnable;
	BYTE ringCount;
	LONG qLog[6];
	ULONG eventNumber;
	CHAR caller_number[50];
	CHAR code_type,stoptone,setreg;
	Queue workqueue;
} chipStruct;

typedef enum
{
	tSlicInit		= 0x1,	
	tSlicStartTone		= 0x2, 
	tSlicStopTone	= 0x3,	
	tSlicSendKey	= 0x4,
	tSlicClose		= 0x5,
}tSlicSubType;

typedef enum
{
	e_SLIC_BUSY_TONE=1,
	e_SLIC_RINGBACK_TONE=2,
	e_SLIC_DIAL_TONE=3,
	e_SLIC_RINGIN_TONE=4,	
	e_SLIC_CONGESTION_TONE=5,
	e_SLIC_PLAY_DHCP_TONE=6,
	e_SLIC_PLAY_IP_TONE=7,
	e_SLIC_PLAY_NETMASK_TONE=8,
	e_SLIC_PLAY_GATEWAY_TONE=9,
	e_SLIC_PLAY_DNS_TONE=10,			
	e_SLIC_KEY_ZERO_TONE=11,
	e_SLIC_KEY_ONE_TONE=12,
	e_SLIC_KEY_TWO_TONE=13,
	e_SLIC_KEY_THREE_TONE=14,
	e_SLIC_KEY_FOUR_TONE=15,
	e_SLIC_KEY_FIVE_TONE=16,
	e_SLIC_KEY_SIX_TONE=17,
	e_SLIC_KEY_SEVEN_TONE=18,
	e_SLIC_KEY_EIGHT_TONE=19,
	e_SLIC_KEY_NINE_TONE=20,
	e_SLIC_KEY_STAR_TONE=21,
	e_SLIC_KEY_POUND_TONE=22,
	e_SLIC_KEY_A_TONE=23,
	e_SLIC_KEY_B_TONE=24,
	e_SLIC_KEY_C_TONE=25,
	e_SLIC_KEY_D_TONE=26,
}tSlicToneType;

VOID HwSlicSetState(LONG newState,LONG channelid);
VOID HwSlicStartTone(ULONG type,ULONG channelid);
void TwoRiscSetPara(int *point);
unsigned char checkSum( char * string );
void ProcessCallid(char *pointer,int len,int channelid);

#endif

