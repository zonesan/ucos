 
// demo.c : Defines the entry point for the console application.
//

/*
Demo.C is the core program for controlling the proSLIC.  It contains "main" as well as all of the major feature functions except
Caller ID which odly enough is in caller.c. An46 contains a discussion of this code and most of the functions are described
in the function description table.  ProSLIC.c contains the initializaion contents and interface functions to talk the the 
SPI bus of the proSLIC.
*/
#include "audio_cfg.h"

#ifdef INCLUDE_SI3210
//#include "Conio.h"
//#include "stdio.h"
#include "proslic.h"
#include "def_type.h"
#include "2risc_memory_map.h"
#include "two_risc_slic.h"
#include "risc.h"
#include "2risc_comm_protocol.h"
#include "m25p64.h"
#include "comm.h"
#include "bsp.h"

#include "os_cpu.h"
#include "os_cfg.h"
#include "uCOS_II.h"

#include "two_risc_comm.h"

//#include "time.h"
//#include "stdlib.h"
//#include <string.h>


PCHAR pcwcodeVersion= $VERSION; // Version STRING!
//static char share_queue[SLIC_MAX_DATABUF_SIZE];
static int slic_initialized = 0;
//int *gkey_addr = (int *)SLIC_UCOS_TO_LINUX_B_ADDR;
int reg_msg;

chipStruct chipData[CHANNEL_NUM] ; /* Represents a proslics state, cached information, and timers */
int channel_num;

void dumpregs(ULONG channelid)
{
	int i;
	printf("================== Dump Register:======================\n");
	for(i=0;i<=108;i++)
		printf("Direct register %3d = 0x%x\n",i,readDirectReg(i,channelid));
	printf("******indirect register**********\n");
	
	for(i=0;i<=12;i++)
		printf("DTMF indirect register %3d = 0x%x\n",i,readIndirectReg(i,channelid));
	for(i=13;i<=25;i++)
		printf("Oscillator indirect register %3d = 0x%x\n",i,readIndirectReg(i,channelid));
	for(i=26;i<=27;i++)
		printf("Digital Programmable indirect register %3d = 0x%x\n",i,readIndirectReg(i,channelid));
	for(i=28;i<=43;i++)
		printf("Slic Control indirect register %3d = 0x%x\n",i,readIndirectReg(i,channelid));
	for(i=99;i<=104;i++)
		printf("FSK Control indirect register %3d = 0x%x\n",i,readIndirectReg(i,channelid));
	
}


VOID Slic_Task(VOID *data)
{
	LONG i=0, j=0,channelid; 
	LONG startTime = 0;

	*(volatile unsigned int*)0xa8010038 |= 0x8; 		//reset sif
    	*(volatile unsigned int*)0xa8010038 = 0x0; 		// reset
	
//	*(volatile unsigned int*)0xa8010014 = 0x80000007; 		//set PCLK 
//    	*(volatile unsigned int*)0xa8070000 = 0xc800020f; 		// set FSK
    	*(volatile unsigned int *)0xa8010010 |= (0x8);

	//SifInit();
  	 printf("\n =======Slic Task==========\n"); 
	 BspStartTicker(100); 

	//chipData[0].cs=6;
	//chipData[0].reset=10;
	//chipData[1].cs=7;
	for(channelid = CHANNEL_START;channelid<channel_num;channelid++)
	//for(channelid = CHANNEL_START;channelid<CHANNEL_NUM;channelid++)
	{
		printf("***** init channel %d...",channelid);
		HwSlicInitialize(channelid);  /* initialize the ProSLIC */
		printf("*****done!\n");

		chipData[channelid].version = HwSlicChipVersion(channelid); 	
		chipData[channelid].type =HwSlicChipType(channelid); 
	
		HwSlicGoActive(channelid);
		HwSlicSetState(ONHOOK,channelid);
		if (HwSlicVerifyIndirectRegisters(channelid)) 
		{
			printf("Direct register verify error: \r\n");
			return;
		}

		chipData[channelid].eventEnable = 1;		//init chipData struture.
		Init_Queue(&chipData[channelid].workqueue);	//init workqueue for set register
	}
	//dumpregs(6);
	
	HwSlicPhoneSystemLoop();  /* pretty much never returns */
	
}

void TwoRiscSetReg(int msg,int channelid)
{	
	typedef union
	{
		int message;
		struct 
		{
			char flag;
			char index;
			short value;
		}info;
	}tMsg;

	tMsg mm;

	mm.message = msg;
//	printf("Channel %d :Set %s[%d] to 0x%x\n",channelid,mm.info.flag ? "IDR":"DR",mm.info.index,mm.info.value);
	if(mm.info.flag)
		writeIndirectReg(mm.info.index, mm.info.value, channelid);
	else
		writeDirectReg(mm.info.index, (char)mm.info.value, channelid);

//	printf("The Valud you Set is 0x%x\n",mm.info.flag ? readIndirectReg(mm.info.index, channelid) : readDirectReg(mm.info.index, channelid));
	
}

BYTE GetKey(ULONG channelid)
{  
	CHAR rawDigit,asciiChar;
	rawDigit=digit( channelid);
	
	//asciiChar= '0' + digit;
	switch (rawDigit){
	case 0xA :
			asciiChar = '0';
			break;
	case 0xB:
			asciiChar = '*';
			break;
	case 0xC:
			asciiChar = '#';
			break;

	default:
		asciiChar= '0' + rawDigit;
		break;
	}

	return asciiChar;
	
}

VOID defered_isr(ULONG  channelid) {
	// Interrupt service  routine
	//printf("Interrupt = 0x%x\n",chipData[channelid].interrupt);
	unsigned long shiftMask=1, original_vec;
 	enum              // Declare enum type Days
	{

		OSC1_T1=0,  
		OSC1_T2=1,
		OSC2_T1=2,
		OSC2_T2=3,
		RING_T1=4,
		RING_T2=5,
		PULSE_T1=6,
		PULSE_T2=7,
		RING_TRIP=8,
		LOOP__STAT=9,
		PQ1=10,
		PQ2=11,
		PQ3=12,
		PQ4=13,
		PQ5=14,
		PQ6=15,
		DTMF=16, /* DTMF detected */
		INDIRECT=17, /* Indirect Reg Access ready */
		CAL_CM_BAL=18, /* Common Mode Calibration Error */
	} interruptCause;                // Variable today has type Days

	static CHAR * icause[]={
		"Osc1 Inactive",  
		"Osc1 Active",  
		"Osc2 Inactive",  
		"Osc2 Active",  
		"Ring Inactive", 
		"Ring Active" ,
		"Pulse Metering Inactive",
		"Pulse Metering Active",
		"Ring Trip",
		"Loop Status Change",
		"                           Pwr Alarm Q1",
		"                           Pwr Alarm Q2",
		"                           Pwr Alarm Q3",
		"                           Pwr Alarm Q4",
		"                           Pwr Alarm Q5",
		"                           Pwr Alarm Q6",
		"DTMF Decode", /* DTMF detected */
		"Indirect Access Complete", /* Indirect Reg Access ready */
		"Common mode balance fault",
 	};

	chipData[channelid].eventNumber++;
	original_vec=chipData[channelid].interrupt;

	for ( interruptCause=OSC1_T1 ; interruptCause <= CAL_CM_BAL ; interruptCause++)
	{
		if (shiftMask & chipData[channelid].interrupt)
		{
			chipData[channelid].interrupt &= ~shiftMask;   // clear interrupt cause
	                                 
			//printf((( interruptCause >=10) && (interruptCause<=11))?"\n %s":"\n(%s)  ", icause[interruptCause]);
			switch (interruptCause) {
			// Figure out what todo based on which one occured
				case OSC1_T1:
					break;
				case OSC1_T2:
					break;
				case OSC2_T1:
					break;
				case OSC2_T2:
					break;
				case RING_T1:
					chipData[channelid].ringCount++;
					if (chipData[channelid].state==FIRSTrING)
					{ 
						chipData[channelid].ringCount=1;
						if (chipData[channelid].version >2)
						{
							//callerid(channelid);
							//chipData[channelid].state = CALLERiD;
							HwSlicSetState(CALLERiD,channelid);
						}
						else
							HwSlicSetState(RINGING,channelid);
							//chipData[channelid].state = CALLBACK;
					}
					break;
				case RING_T2:
					break;
				case PULSE_T1:
					break;
				case PULSE_T2:
					break;
				case RING_TRIP:
					 if (chipData[channelid].version <=2 )  // if REVISION B  set to active.
					{
						HwSlicGoActive(channelid); // Rev B fix not needed 
					}
				case LOOP__STAT:
					 groundShort(channelid); /* Check for grounded Tip or Ring Leads*/
					 initializeLoopDebounceReg(channelid);
					//disableOscillators(channelid);
					loopAction(channelid);
					//disableOscillators(channelid);
					//TwoRiscSendMsg(GetStatus(channelid),channelid);
					HwSlicSetState(LOOPtRANSITION,channelid);
					break;
				case PQ1:
				case PQ2:
				case PQ3:
				case PQ4:
				case PQ5:
				case PQ6:
				{	
					static unsigned long lastEventNumber =1;
					if (lastEventNumber != chipData[channelid].eventNumber)  /*  We allow only one alarm per alarm event */
					{
						//printf("Q exception\n");
						LONG i = interruptCause - PQ1;
						lastEventNumber = chipData[channelid].eventNumber;
						HwSlicPowerLeakTest(channelid);
						HwSlicPowerUp(channelid);
						printf( "  Channel %d:%d time",channelid,chipData[channelid].qLog[i]);
						if (chipData[channelid].qLog[i]++>2)    exception(POWERaLARMQ1+i);
						if(chipData[channelid].qLog[i] >1) printf( "s");
						clearAlarmBits(channelid);
						HwSlicGoActive(channelid);
						HwSlicSetState(ONHOOK,channelid);
					}
				}
					break;
					
				case DTMF:
					//if(digit_valid(channelid))
					//{
						//printf("Channel %d ,\tget key:%c,\t",channelid,GetKey(channelid));
						TwoRiscSendMsg(GetKey(channelid),channelid);
						disableOscillators(channelid);
					//}
					//else
						//printf("Detect invalid key!\n");
					break;
				case INDIRECT:
					break;
			} //switch

	
		} //if
 		 shiftMask<<=1;
	} //for

} // function interruptCause

VOID HwSlicStateMachine(int channelid)
{
	static int ringCount=0;

	switch (chipData[channelid].state)
	{
	
		case STATEcHANGE:
		 	chipData[channelid].state=chipData[channelid].newState;
		  	break;			
		case CALLBACK :
			disableOscillators(channelid);
			standardRinging(channelid);
			activateRinging(channelid);
			chipData[channelid].state = FIRSTrING;
			break; 

		case CALLBACKpROGRAMMED:
			disableOscillators(channelid);
	
 			activateRinging(channelid);
			chipData[channelid].state = PROGRAMMEDrING;
		break; 
		case MAKEoFFHOOK: // Make dialtone 
			dialTone(channelid);
			HwSlicSetState(DIALtONE,channelid);
			//chipData.digit_count=0;
			break;
		case LOOPtRANSITION:
			if (chipData[channelid].version <= 2)
				initializeLoopDebounceReg(channelid);
			
				switch (chipData[channelid].previousState){
												
						case RINGING:
						case FIRSTrING:
							HwSlicSetState(OFFHOOK,channelid);
				 			break;
						case DEFEREDcALLBACKpROGRAMMED:
							HwSlicSetState (CALLBACKpROGRAMMED,channelid);
						break;

						case PRENEON:
							setupNeon(channelid);
							HwSlicSetState(NEON,channelid);
							break;
						default:
							;
							//printf("test loop off\n");
							//loopAction(channelid);
				}
			break;
		case DTMFtRANISTION:
			switch (chipData[channelid].previousState){
						case OFFHOOK:
						case BUSY:
						case RINGbACK:
						break;
						case DIALtONE:
							disableOscillators(channelid);
							//printf("Press Key: %c\r\n",GetKey());
							//dtmfAction();
							break;
						case DIGITDECODING:
							//printf("Press Key: %c\r\n",GetKey());
							//dtmfAction();
							break;
			}
			break;
		case CALLERiD:
			callerid(channelid);
			HwSlicSetState(RINGING,channelid);
			break;
		case RINGING:
			if (chipData[channelid].ringCount>10){
				stopRinging(channelid);
				chipData[channelid].ringCount=0;

				HwSlicSetState(ONHOOK,channelid);
			}
			break;
		case MAKEbUSY:
			//PlayMusic();
			busyTone(channelid);
			HwSlicSetState(BUSY,channelid);
			//chipData[channelid].digit_count=0;
			break;
		case MAKErINGbACK:
			ringBackTone(channelid);
			HwSlicSetState(RINGbACK,channelid);
			//chipData[channelid].digit_count=0;
			break;
		case MAKErEORDER:
			reorderTone(channelid);
			HwSlicSetState(REORDER,channelid);
			//chipData.digit_count=0;
			break;
		case MAKEcONGESTION:
			congestionTone(channelid);
			HwSlicSetState(CONGESTION,channelid);
			//chipData.digit_count=0;
			break;
	
		}

		// set audio encode type
		if(chipData[channelid].code_type != 0)
		{
			if(AUDIO_ENCODING_ULAW == chipData[channelid].code_type)
				writeDirectReg(0x1, 0x28,channelid); 
			else if(AUDIO_ENCODING_ALAW == chipData[channelid].code_type)
				writeDirectReg(0x1, 0x20,channelid); 
			else if(AUDIO_ENCODING_LINEAR== chipData[channelid].code_type)
				writeDirectReg(0x1, 0x3c,channelid);

			chipData[channelid].code_type = 0;
		}
		// stop oscillators
		if(chipData[channelid].stoptone)
		{
			printf("detect stop tone\n");
			disableOscillators(channelid);
			chipData[channelid].stoptone = 0;
		}

		while(!Queue_empty(&chipData[channelid].workqueue))
		{
			int data;
			data = Del_Queue(&chipData[channelid].workqueue);
			TwoRiscSetReg(data,channelid);
		}
		/*
		if(1 == chipData[channelid].setreg)
		{
			TwoRiscSetReg(reg_msg,channelid);
			chipData[channelid].setreg = 0;
		}*/
		
}

VOID HwSlicPhoneSystemLoop() 
{
	LONG i=0, chip=0,channelid;
	LONG timeout = 200;

	chipData[channelid].eventEnable = 1;
	//chipData[channelid].interrupt = 0;
	printf("Slic System Loop start ******************************************************\n");
	while(1) {
		for(channelid = CHANNEL_START;channelid<channel_num;channelid++)
		//for(channelid = CHANNEL_START;channelid<CHANNEL_NUM;channelid++)
		{
			//printf("in while loop\n");
			//if (chipData[channelid].eventEnable) {
				chipData[channelid].interrupt = interruptBits(channelid);
				if (chipData[channelid].interrupt)
					defered_isr(channelid);   // isr = interrrupt service routine

				if(0 == timeout--)
				{
					//printf("channelid %d eventenale = %d..\n",channelid,chipData[channelid].eventEnable);
					timeout = 400;
				}
			//}
			HwSlicStateMachine(channelid);
		
		}
/*		if(0 == timeout--)
		{
			printf("Slic Task Alive.. eventenable = %d\n",chipData[channelid].eventEnable);
			timeout = 400;
		}*/
             OSTimeDlyHMSM(0, 0, 0, 50); 	
	}
}

LONG OffHook_Valid(LONG channelid)
{

	if(((readDirectReg(78,channelid) & 0x3f) <= 24) && ((readDirectReg(79,channelid) & 0x3f) >= 15))
	{
		OSTimeDlyHMSM(0, 0, 0, 100); 
	}
	else
		return 0;

	if(((readDirectReg(78,channelid) & 0x3f) <= 24) && ((readDirectReg(79,channelid) & 0x3f) >= 15))
	{
		return 1; 
	}
	else
		return 0;

}

LONG OnHook_Valid(LONG channelid)
{

	if(((readDirectReg(78,channelid) & 0x3f) > 24) && ((readDirectReg(79,channelid) & 0x3f) < 15))
	{
		OSTimeDlyHMSM(0, 0, 0, 50); 
	}
	else
		return 0;

	if(((readDirectReg(78,channelid) & 0x3f) > 24) && ((readDirectReg(79,channelid) & 0x3f) < 15))
	{
		return 1; 
	}
	else
		return 0;

}

VOID loopAction(ULONG channelid)
{ 

	//printLoopState();
	//initializeLoopDebounceReg(channelid);
		printf("reg[68] = 0x%x\n",readDirectReg(68,channelid));

	if((chipData[channelid].state != MAKEoFFHOOK)  && (loopStatus(channelid))) //Checking for offhook with debounce
	{
		 if (chipData[channelid].version <=2 )  // if REVISION B  set to active.
		{
			HwSlicGoActive(channelid);  // Rev B fix not needed 
		}
		//HwSlicSetState(MAKEoFFHOOK,channelid);
		//if(OffHook_Valid(channelid))	//for videophone
			TwoRiscSendMsg('f', channelid);
		//printf("DR[78] = %d	DR[79] = %d\n",readDirectReg(78,channelid),readDirectReg(79,channelid));
		return;		
	}

	if ((chipData[channelid].state != ONHOOK) && ((loopStatus(channelid))==0) )//Checking for onHook with debounce
	{		
		HwSlicSetState(ONHOOK,channelid);
		//if(OnHook_Valid(channelid))	//for videophone
			TwoRiscSendMsg('n', channelid);
		printf("DR[78] = %d	DR[79] = %d\n",readDirectReg(78,channelid),readDirectReg(79,channelid));
		disableOscillators(channelid);
		return;
	}

}

#endif
