// caller.c : Defines the entry point for the console application.
//
#include "audio_cfg.h"

#ifdef INCLUDE_SI3210
#include "proslic.h"
#include "two_risc_slic.h"
#include "comm.h"
#include "libc_s.h"
#include "Slic.h"

//#include "string.h"
//#include "stdlib.h"
//#include "Conio.h"
//#include "stdio.h"
//#include "time.h"
  


unsigned char arrayOfU [] =
{
   'U','U','U',
   'U','U','U',
   'U','U','U',
   'U','U','U',
   'U','U','U',
   'U','U','U',
   'U','U','U', //
   'U','U','U',
   'U','U','U', //27
   'U','U','U', //30
0,  } ;

// Notice that the 0x24 is the length of the caller ID frame.
// You would need to change this if you wanted to change the length of the
// message


unsigned char init ;// this will indicate if there is a missed interrupt
extern chipStruct chipData[CHANNEL_NUM];
//char caller_number[50];
char test[] = "0208085613450529495";

unsigned char  Interrupt(ULONG channelid)
{
	return (readDirectReg(18,channelid) & 0x1);
}

void fskInitialization (ULONG channelid)

{
	init =0 ;  // gloabal variable used to detect critical timing violation
           // if init =2 => more than 1/1200th second passed between interrupt
	//writeDirectReg( REVC,0x40); // set to revision C FSK mode on  Now this is done ealier
	writeDirectReg(36,19,channelid);  // 19 is twenty ticks  20/24000 sec = 1/1200 sec
	writeDirectReg(37,0x0,channelid); // 0 is zero MSB of timer



	writeDirectReg(FSK_DATA,1,channelid);  /* Mark is the default value */
	writeDirectReg(FSK_DATA,1,channelid);  /* Mark is the default value */
	/* writen twice to fill double buffer which has logic to detect
  	 bit transition
	*/

	writeDirectReg(21,0x01,channelid); /*  Mask Register #1  Active Interrupt*/

/*

INFO: case0 = 0.997673,case1 = 0.998714,case2 = 1.001328,case3 = 1.002373
case1 < case0

INFO: Settings for  2200 as initial frequency are - OSCn = 0x6b60, OSCnX = 0x01b4
INFO: Settings for  1200 as initial frequency are - OSCn = 0x79c0, OSCnX = 0x00e9
INFO: Settings for  2200 to  1200  transition are - OSCn = 0x79c0, OSCnX = 0x1110
INFO: Settings for  1200 to  2200  transition are - OSCn = 0x6b60, OSCnX = 0x3c00
INFO: Compound gain variation                          = 0.999756


*/

	writeIndirectReg(FSK_X_0,0x01b4,channelid); // See above
	writeIndirectReg(FSK_COEFF_0	,0x6b60,channelid);
	writeIndirectReg(FSK_X_1,0x00e9,channelid);
	writeIndirectReg(FSK_COEFF_1	,0x79c0,channelid);
	writeIndirectReg(FSK_X_01,0x1110,channelid);
	writeIndirectReg(FSK_X_10,0x3c00,channelid);
	writeDirectReg(OSC1,0x56,channelid);  // FSK mode receiver
}

int waitForInterrupt (ULONG channelid)
{
/* Wait for an Interrupt from the ProSLIC => oscillator loaded */
	unsigned char data = readDirectReg(18,channelid),tmp;

	 if (data && init !=0){
		 printf(" %1.1x",init) ;
		 init = 2;
	}
	 if (init == 0) init=1; /* init has 3 states 0 => fsk initialized

				*/
	//printf("intertupt:0x%x\n",data);
	while (!data) 
	{
		//printf("reg[19]]=0x%x\n",readDirectReg(19,channelid));
		data =  readDirectReg(18,channelid);

		if(loopStatus(channelid))
		{
			//printf("reg[19]]=0x%x\n",readDirectReg(19,channelid));
			tmp = readDirectReg(19,channelid);
			if(tmp & 0x2)
			{
				printf("reg[19]]=0x%x\n",tmp);
				disableOscillators(channelid);
				TwoRiscSendMsg('f', channelid);
				writeDirectReg(19, 0x2,channelid);
				//printf("reg[19]]=0x%x\n",readDirectReg(19,channelid));
			}
			return 1;
		}
	}
	
	writeDirectReg(18,0x01,channelid); /*  Status Register #1  clear interrupt*/

	return 0;
}

int  fskByte(unsigned char c,ULONG channelid)
{
	unsigned int i;

	for(i = 0;i<1;i++)
	{
		writeDirectReg(FSK_DATA,1,channelid); // Send Stop bit
		if(waitForInterrupt(channelid))     // start bit  STARTS
			return 1;
	}

	writeDirectReg(FSK_DATA,0,channelid); // Send Stop bit
	if(waitForInterrupt(channelid))     // start bit  STARTS
		return 1;

	for (i=0;i<8;i++)
	{
		writeDirectReg(FSK_DATA,c,channelid);
		c>>=1;
		if(waitForInterrupt(channelid))
			return 1;
	} // for

	writeDirectReg(FSK_DATA,1,channelid);

	if(waitForInterrupt(channelid))
		return 1;

	return 0;

//writeDirectReg(FSK_DATA,1);

//waitForInterrupt();
}// fskByte()

int sendProSLICID (ULONG channelid)
{   
	static char c ='0',modulo=0;
	int i; 
	unsigned char sum;
	//ProcessCallid(test, 19, 0);
	sum = checkSum(chipData[channelid].caller_number);
	printf("CallerID....");
	fskInitialization (channelid);
	
	OSTimeDlyHMSM(0,0,1,0);
	for ( i=0 ; i<30; i++) 
		if(fskByte('U',channelid))
			return 1;

	for ( i=0 ; i<150 ; i++ ) 
		if(waitForInterrupt(channelid))  // wait 180 bits worth
			return 1;
	i=0;
	while  (chipData[channelid].caller_number[i] != 0) 
		if(fskByte(chipData[channelid].caller_number[i++],channelid))
			return 1;
	if(fskByte(sum,channelid))
		return 1;
	printf("OK\n");
	OSTimeDlyHMSM(0,0,0,500);
	return 0;
}

#endif

