#include "Slic.h"
#include "2risc_comm_protocol.h"
#include "2risc_memory_map.h"

extern int channel_num;
extern chipStruct chipData[CHANNEL_NUM];

int *gkey_addr = (int *)SLIC_UCOS_TO_LINUX_B_ADDR;

char* strcat(char *str1,char *str2)
{
	char *p = str1;
	while(*str1++);
	--str1;
	while(*str2) *str1++ = *str2++;
	*str1 = '\0';

	return p;
}

void Init_Queue(Queue *sq)
{
	sq->front=0;
	sq->rear=0;
	sq->isNull=1;
	//printf("front = %d,rear = %d,isNull = %d\n",sq->front,sq->rear,sq->isNull);
}

int Ins_Queue(Queue *sq,int x)
{

	if(((sq->front)%QUEUE_LEN ==(sq->rear+1)%QUEUE_LEN) && sq->isNull !=1)
	{
		printf("\nFull squeue!New data in the old position!\n");

		sq->rear=(sq->rear+1)%QUEUE_LEN;
		sq->data[sq->rear]=x;
		sq->front=(sq->front+1)%QUEUE_LEN;
	}
	else
	{
		sq->rear=(sq->rear+1)%QUEUE_LEN;
		sq->data[sq->rear]=x;
		//printf("\nData in\n");
		sq->isNull=0;
		return 1;
	}

	sq->isNull=0;
	return x;
}

int Del_Queue(Queue *sq)
{
	if(sq->rear==sq->front && sq->isNull==1)
	{
		printf("the queue is empty\n");
		return 0;
	}
	else
	{
		if((sq->rear)%QUEUE_LEN==(sq->front+1)%QUEUE_LEN)
			sq->isNull=1;
		sq->front=(sq->front+1)%QUEUE_LEN;
		//printf("\nData out\n");
		return(sq->data[sq->front]);
	}

}

int Queue_Full(Queue *sq)
{
	if((sq->front)%QUEUE_LEN ==(sq->rear+1)%QUEUE_LEN && sq->isNull!=1)
	{
		return 1;
	}
	return 0;
}

int Queue_empty(Queue *sq)
{
	if(sq->rear==sq->front && sq->isNull==1)
	{
		return 1;
	}
	return 0;
}
//
VOID HwSlicSetState(LONG newState,LONG channelid)
{
	chipData[channelid].previousState=chipData[channelid].state;
	chipData[channelid].newState= newState;
	chipData[channelid].state=STATEcHANGE;
	switch (newState){

			 case CALLERiD:
				 chipData[channelid].eventEnable=0;
				 break;
			 case RINGING:
				 chipData[channelid].eventEnable=1;
				 break;
			 

	}
}

void TwoRiscSetPara(int *point)
{
	int i,value;

	channel_num = *point++;
	//channel_num  = 1;	//stone debug for single slic
	printf("channel num = %d\n",channel_num);
	for(i=0;i<channel_num;i++)
	{
		value = *point;
		chipData[i].cs = value & 0xff;
		chipData[i].reset = ((value >>16) &0xff);
		point++;
		printf("Channel %d : CS = %d,  RESET = %d\n",i,chipData[i].cs,chipData[i].reset);
	}
}

VOID HwSlicStartTone(ULONG type,ULONG channelid)
{
	int i;
	//disableOscillators(channelid);
	switch(type)
	{
		case e_SLIC_BUSY_TONE:
			//busyTone(channelid);
			HwSlicSetState(MAKEbUSY,channelid);
			break;
		case e_SLIC_RINGBACK_TONE:
			HwSlicSetState(MAKErINGbACK,channelid);
			//ringBackTone(channelid);
			break;
		case e_SLIC_DIAL_TONE:
			//ringBackTone();
			//dialTone(channelid);
			HwSlicSetState(MAKEoFFHOOK,channelid);
			break;
		case e_SLIC_RINGIN_TONE:
			//chipData[channelid].state = FIRSTrING;
			//standardRinging(channelid);
			//activateRinging(channelid);
			HwSlicSetState(CALLBACK,channelid);
			break;
		default:
			debug("No this Tone type:\n");
			//standardRinging();
			
	}
}


VOID TwoRiscSendMsg(char key,int channelid)
{
	Risc_Packet_t  tt, *packet;
	long *msg;

	packet = &tt;

	if(gkey_addr == SLIC_UCOS_TO_LINUX_E_ADDR)
		gkey_addr = SLIC_UCOS_TO_LINUX_B_ADDR;
	packet->start = RISC_PKT_START;
	packet->main_type = tAudio;
	packet->status =0x0;
	packet->end = RISC_PKT_END;
	packet->sub_type = 0xb;
	packet->length = 0xc;
	msg = (long *)packet->message;
	msg[0] = channelid;		//slic channel id
	msg[1] = gkey_addr;	// share address;
	msg[2] = 0x1;		//len
	*gkey_addr= key;
	
        // Send packet    
        if(TwoRiscCommSendPacket((unsigned int *)packet, RPACKET_LEN)) {            
		printf("Channel %d key_addr : 0x%x ,send key:%c\n",channelid,gkey_addr,*gkey_addr);	
		gkey_addr++;
            	RiscFlagSet();    // Set risc1 flag to interrupt risc1
        } else
            printf("SLIC send failed!\r\n");  
}

unsigned char checkSum( char * string )
{
	int i =0;
	unsigned char sum=0;
	while (string[i] !=0){
        	sum += string[i++];
        }

	return -sum ;
}

void ProcessCallid(char *pointer,int len,int channelid)
{
	printf("caller addr = 0x%x, len = %d",pointer,len);

	chipData[channelid].caller_number[0] = 0x80;
	chipData[channelid].caller_number[1] = 21 + len;
	chipData[channelid].caller_number[2] = 0x01;
	chipData[channelid].caller_number[3] = 0x08;

	memcpy(&chipData[channelid].caller_number[4],pointer,8);		//date & time

	chipData[channelid].caller_number[12] = 0x02;
	chipData[channelid].caller_number[13] = len -8;

	memcpy(&chipData[channelid].caller_number[14],pointer+8,len -8);	// telephone no.

	chipData[channelid].caller_number[6+len] = 0x07;
	chipData[channelid].caller_number[7+len] = 0x0f;
	chipData[channelid].caller_number[8+len] = '\0';

	strcat(chipData[channelid].caller_number,"VBRIDGE");
	strcat(chipData[channelid].caller_number,"\x20");
	strcat(chipData[channelid].caller_number,"CALLING");
/*
	int i = 0;
	for(i=0;i<50;i++)
		printf("0x%02x  ",caller_number[i]);
		*/
	
}


