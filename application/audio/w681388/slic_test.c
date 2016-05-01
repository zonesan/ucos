#include "audio_cfg.h"
#ifdef INCLUDE_WINBOND_SLIC
#include "slic.h"
#include "w681388.h"
#include "audio.h"
#include "slic_test.h"
#include <comm.h>
#include "2risc_memory_map.h"
//#include "two_risc_comm.h"
#include "2risc_comm_protocol.h"
//#include "../Slic.h"


#define TEST_CHANNEL 0

chipStruct chipData[CHANNEL_NUM];
int channel_num = 1;
static int slic_initialized = 0;
int reg_msg;

structCountryTones *Tones;
structRingTones  *RingTones;
structCountryTones  TonesOff[1] = { 0,0,0,0, 0,0,0,0};

structRingTones  WorldRingTones[8]=
{

	INIT_OS2C_USA,   	INIT_OS2IC_USA,		INIT_OS2AT_USA,   	INIT_OS2IT_USA,
	INIT_OS2C_JAPAN,   	INIT_OS2IC_JAPAN,	INIT_OS2AT_JAPAN,   INIT_OS2IT_JAPAN,
	INIT_OS2C_KOREA,   	INIT_OS2IC_KOREA,	INIT_OS2AT_KOREA,   INIT_OS2IT_KOREA,
	INIT_OS2C_CHINA,   	INIT_OS2IC_CHINA,	INIT_OS2AT_CHINA,   INIT_OS2IT_CHINA,
	INIT_OS2C_FRANCE,   INIT_OS2IC_FRANCE,	INIT_OS2AT_FRANCE,  INIT_OS2IT_FRANCE,
	INIT_OS2C_ITALY,   	INIT_OS2IC_ITALY,	INIT_OS2AT_ITALY,   INIT_OS2IT_ITALY,
	INIT_OS2C_GERMANY , INIT_OS2IC_GERMANY ,INIT_OS2AT_GERMANY ,INIT_OS2IT_GERMANY ,
	INIT_OS2C_UK,   	INIT_OS2IC_UK,		INIT_OS2AT_UK,   	INIT_OS2IT_UK
};

//International Signaling Tones
///

enum TONE_COUNTRY{
	TONE_USA,
	TONE_JAPAN,
	TONE_KOREA,
	TONE_CHINA,
	TONE_FRANCE,
	TONE_ITALY,
	TONE_GERMANY,
	TONE_UK,
	TONE_BELGUIM,
	TONE_ISREAL,
	TONE_NETHEALANDS,
	TONE_NORWAY,
	TONE_SINGAPORE,
	TONE_SWEDEN,
	TONE_SWITZERLAND,
	TONE_TAIWAN,
};

structCountryTones  WorldDialTones[16]=
{

	// OS1C(Freq)  OS1IC (Ampl)	OS1AT(OnTime)	OS1IT (OffTime)	OS2C (Freq)	OS2IC (Amplitude) OS2AT (OnTime) OS2IT (OffTime)		Comments
		0x786E	,	0x06B7	,	0x1F40	,	0	,	0x7B31	,	0x0561	,	0x1F40	,	0,  //USA	// 440+350 @ -13dBm
		0x79BC	,	0x0620	,	0x1F40	,	0	,	0	,	0	,	0	,	0	,			  // JAPAN	// 400 @ -13dBm
		0x786E	,	0x06B7	,	0x1F40	,	0	,	0x7B31	,	0x0561	,	0x1F40	,	0,	// KOREA 	// 440+350 @ -13dBm
		0x786E	,	0x06B7	,	0x1F40	,	0	,	0x7B31	,	0x0561	,	0x1F40	,	0,	// CHINA	// 440+350 @ -13dBm
		0x786E	,	0x06B7	,	0x1F40	,	0	,	0	,	0	,	0	,	0	,			  // FRANCE	// 440 @ -13dBm
		0x78EF	,	0x067E	,	0x12C0	,	0x0320,	0	,	0	,	0	,	0	,			  // ITALY	// 425 @ -13dBm
		0x78EF	,	0x067E	,	0x1F40	,	0	,	0	,	0	,	0	,	0	,			  // GERMANY	// 425 @ -13dBm
		0x786E	,	0x06B7	,	0x12C0	,	0x1F40,	0x7B31	,	0x0561	,	0x1F40	,	0,	// UK	// 440+350 @ -13dBm
		0x7816	,	0x06DC	,	0x1F40	,	0	,	0	,	0	,	0	,	0	,			  // BELGUIM 	// 450 @ -13dBm
		0x79BC	,	0x0620	,	0x1F40	,	0	,	0	,	0	,	0	,	0,			  // ISRAEL	//400 @ -13dBm
		0x78EF	,	0x067E	,	0x1F40	,	0	,	0	,	0	,	0	,	0	,			  // NETHERLANDS	//425 @ -13dBm
		0x78EF	,	0x067E	,	0x1F40	,	0	,	0	,	0	,	0	,	0	,			  // NORWAY	//425 @ -13dBm
		0x7BFA	,	0x04EE	,	0x1F40	,	0	,	0x7D22	,	0x042C	,	0x1F40	,	0,	// SINGAPORE	//320 + 270 @ -13dBm
		0x78EF	,	0x067E	,	0x1F40	,	0	,	0	,	0	,	0	,	0	,			  // SWEDEN	//425 @ -13dBm
		0x78EF	,	0x067E	,	0x1F40	,	0	,	0	,	0	,	0	,	0	,			  // SWITZERLAND	//425 @ -13dBm
		0x786E	,	0x06B7	,	0x1F40	,	0	,	0x7B31	,	0x0561	,	0x1F40	,	0,	// TAIWAN	// 440+350 @ -13dBm
	
};


structCountryTones  WorldRingBackTones[16]=
{

	//OS1C (Freq) OS1IC (Amplitude)	OS1AT (OnTime)	OS1IT (OffTime)	OS2C (Freq)	OS2IC (Amplitude)	OS2AT (OnTime)	OS2IT (OffTime)		Comments
		0x7703	,	0x03A8	,	0x3E80	,	0x7D00	,	0x786E	,	0x035D	,	0x3E80	,	0x7D00,	// USA	// 480+440 @ -19dBm
		0x793A	,	0x0330	,	0x1F40	,	0x3E80	,	0x7A39	,	0x02F3	,	0x1F40	,	0x3E80,// JAPAN	// 416+384 @ -19dBm
		0x7703	,	0x03A8	,	0x3E80	,	0x7D00	,	0x786E	,	0x035D	,	0x3E80	,	0x7D00,// KOREA	// 480+440 @ -19dBm
		0x7703	,	0x03A8	,	0x3E80	,	0x7D00	,	0x786E	,	0x035D	,	0x3E80	,	0x7D00,// CHINA	// 480+440 @ -19dBm
		0x79BC	,	0x0312	,	0x338F	,	0x68AF	,	0	,	0	,	0	,	0	,				  // FRANCE	// 400 @ -19dBm
		0x78EF	,	0x0341	,	0x1F40	,	0x7D00	,	0	,	0	,	0	,	0	,				  // ITALY	// 425 @ 19dBm
		0x78EF	,	0x0341	,	0x1F40	,	0x7D00	,	0	,	0	,	0	,	0	,				  // GERMANY// 425 @ 19dBm
		0x7816	,	0x0370	,	0x1F40	,	0x7D00	,	0x79BC,	0x0312	,	0x1F40	,	0x7D00	,	  // UK	// 450+400@ -19dBm
		0x7816	,	0x0370	,	0x1F40	,	0x5DC0	,	0	,	0	,	0x1F40	,	0	,			  // BELGUIM// 450 @ -19dBm
		0x7816	,	0x0370	,	0x1F40	,	0x5DC0	,	0x79BC,	0x0312	,	0x1F40	,	0x5DC0	,	  // ISRAEL// 450+400@ -19dBm
		0x78EF	,	0x0341	,	0x1F40	,	0x7D00	,	0	,	0	,	0	,	0	,				  // NETHERLANDS//425 @ -19dBm
		0x78EF	,	0x0341	,	0x1F40	,	0x7D00	,	0	,	0	,	0	,	0	,				  // NORWAY//425 @ -19dBm
		0x79BC	,	0x0312	,	0x1F40	,	0x3E80	,	0	,	0	,	0	,	0	,				  // SINGAPORE//400 @ -19dBm
		0x78EF	,	0x0341	,	0x1F40	,	0x7D00	,	0	,	0	,	0	,	0	,				  // SWEDEN	//425 @ -19dBm
		0x78EF	,	0x0341	,	0x1F40	,	0x7D00	,	0	,	0	,	0	,	0	,				  // SWITZERLAND//425 @ -19dBm
		0x7703	,	0x03A8	,	0x3E80	,	0x7D00	,	0x786E	,	0x035D	,	0x3E80	,	0x7D00	,// TAIWAN // 480+440 @ -19dBm
																	
};

structCountryTones  WorldBusyTones[16]=
{

	//	OS1C (Freq)		OS1IC (Amplitude)		OS1AT (OnTime)		OS1IT (OffTime)		OS2C (Freq)		OS2IC (Amplitude)		OS2AT (OnTime)		OS2IT (OffTime)		Comments
																		
		0x711F	,	0x04A6	,	0x0FA0	,	0x0FA0	,	0x7703	,	0x03A8	,	0x0FA0	,	0x0FA0	,// USA	// 440+620 @ -19dBm
		0x79BC	,	0x0312	,	0x0FA0	,	0x0FA0	,	0	,	0	,	0	,	0	,				  // JAPAN	// 400 @ -19dBm
		0x711F	,	0x04A6	,	0x0FA0	,	0x0FA0	,	0x7703	,0x03A8	,	0x0FA0	,	0x0FA0	,	  // KOREA	// 440+620 @ -19dBm
		0x711F	,	0x04A6	,	0x0FA0	,	0x0FA0	,	0x7703	,0x03A8	,	0x0FA0	,	0x0FA0	,	  // CHINA	// 440+620 @ -19dBm
		0x786E	,	0x035D	,	0x0FA0	,	0x0FA0	,	0	,	0	,	0	,	0	,				  // FRANCE// 440 @ -19dBm
		0x78EF	,	0x0341	,	0x0FA0	,	0x0FA0	,	0	,	0	,	0	,	0	,				  // ITALY// 425 @ -19dBm
		0x78EF	,	0x0341	,	0x0FA0	,	0x0FA0	,	0	,	0	,	0	,	0	,				  // GERMANY// 425 @ -19dBm
		0x79BC	,	0x0312	,	0x0C80	,	0x0C80	,	0	,	0	,	0	,	0	,				  // UK	// 440 @ -19dBm
		0x7816	,	0x06DC	,	0x04B0	,	0x04B0	,	0	,	0	,	0	,	0	,				  // BELGUIM// 450 @ -19dBm
		0x79BC	,	0x0312	,	0x0C80	,	0x0C80	,	0	,	0	,	0	,	0	,				  // ISRAEL// 440 @ -19dBm
		0x78EF	,	0x0341	,	0x0FA0	,	0x0FA0	,	0	,	0	,	0	,	0	,				  // NETHERLANDS// 425 @ -19dBm
		0x78EF	,	0x0341	,	0x0FA0	,	0x0FA0	,	0	,	0	,	0	,	0	,				  // NORWAY	// 425 @ -19dBm
		0x7BFA	,	0x0278	,	0x1770	,	0x1770	,	0x7D22	,0x0217	,0x1770	,	0x1770	,		  // SINGAPORE	//320 + 270 @ -19dBm
		0x78EF	,	0x0341	,	0x07D0	,	0x07D0	,	0	,	0	,	0	,	0	,				  // SWEDEN//425 @ -13dBm
		0x78EF	,	0x0341	,	0x0FA0	,	0x0FA0	,	0	,	0	,	0	,	0	,				  // SWITZERLAND// 425 @ -19dBm
		0x711F	,	0x04A6	,	0x0FA0	,	0x0FA0	,	0x7703	,	0x03A8	,	0x0FA0	,	0x0FA0,  // TAIWAN // 440+620 @ -19dBm


};

void slic_test_write(void)
{
    w681388Write(0, 1, TEST_CHANNEL);
    printf("x = 0x%x\n", (int)w681388Read(1, TEST_CHANNEL));
    w681388Write(1, 1, TEST_CHANNEL);
    printf("x = 0x%x\n", (int)w681388Read(1, TEST_CHANNEL));
    w681388Write(2, 1, TEST_CHANNEL);
    printf("x = 0x%x\n", (int)w681388Read(1, TEST_CHANNEL));
    w681388Write(3, 1, TEST_CHANNEL);
    printf("x = 0x%x\n", (int)w681388Read(1, TEST_CHANNEL));
    w681388Write(4, 1, TEST_CHANNEL);
    printf("x = 0x%x\n", (int)w681388Read(1, TEST_CHANNEL));
    w681388Write(5, 1, TEST_CHANNEL);
    printf("x = 0x%x\n", (int)w681388Read(1, TEST_CHANNEL));
    w681388Write(6, 1, TEST_CHANNEL);
    printf("x = 0x%x\n", (int)w681388Read(1, TEST_CHANNEL));
    w681388Write(7, 1, TEST_CHANNEL);
    printf("x = 0x%x\n", (int)w681388Read(1, TEST_CHANNEL));
    w681388Write(8, 1, TEST_CHANNEL);
    printf("x = 0x%x\n", (int)w681388Read(1, TEST_CHANNEL));
}


#ifdef SIMULATION
void Calculate(void)
{
    CalculateToneFReg(697);
    CalculateToneAReg(697, 0.31);
    CalculateToneFReg(770);
    CalculateToneAReg(770, 0.31);
    CalculateToneFReg(852);
    CalculateToneAReg(852, 0.31);
    CalculateToneFReg(941);
    CalculateToneAReg(941, 0.31);
    CalculateToneFReg(1209);
    CalculateToneAReg(1209, 0.55);
    CalculateToneFReg(1336);
    CalculateToneAReg(1336, 0.55);
    CalculateToneFReg(1477);
    CalculateToneAReg(1477, 0.55);
    CalculateToneFReg(1633);
    CalculateToneAReg(1633, 0.55);
}
#endif


int SlicInit(int channel)
{
	w681388Initialize(channel);
		//DumpReg(channel);
	 w681388Calibrate(channel);

	 w681388Write(0xff, W681388REG_INT1, channel); // Clear Status    
	 w681388Write(0xff, W681388REG_INT2, channel);
	 w681388Write(0x07, W681388REG_INT3, channel);

	 w681388Write(0xff, W681388REG_IE1, channel); // enable interrupt
	 w681388Write(0xff, W681388REG_IE2, channel);
	 w681388Write(0x06, W681388REG_IE3, channel);	//no temperature detect

	    //power up line
	 w681388AdjLineState(LS_RV_IDEL,channel);
    return SLIC_NOERROR;
}

int SlicSetPcmMode(struct tPcmMode *PcmMode, int channel)
{
    unsigned char temp;
    
    temp = 0;
    if(PcmMode->BitMode == PCM_CODEC_16BIT)
        temp |= PCMC_16BIT;
    if(PcmMode->GciFormat == PCM_GCI_2BIT)
        temp |= PCMC_GCI;
    if(PcmMode->TriMode == PCM_TRI_NEGATIVE)
        temp |= PCMC_TRI_N;
    printf("set pcm control 0x%x\n", temp);
    w681388Write(temp, W681388REG_PCMC, channel);
    w681388SetCodeMode((PcmMode->CodecMode) << 6, channel);
       
    SlicSetTimeSlot(PcmMode->TxTimeSlot, PcmMode->RxTimeSlot, channel);
    
    temp = 0;
    if(PcmMode->InternalFsync)
        temp |= PCMFS_INTERNAL_FS;
    if(PcmMode->FsType == PCM_LONG_FS)
        temp |= PCMFS_LFS;
    w681388Write(temp, W681388REG_PCMFS, channel);
    w681388SetFbclk(PcmMode->Fbclk, channel);
    //w681388PcmIfCtl(1, channel);       //should it be enable???????????????????
    //w681388PcmIfReset(channel);       //should it reset ??????????????????????????
    return 1;
}

void ReadWriteTest(void)
{
    char key;
    int address;
    int data;
    int channel;
    printf("Read(r) , Write(w) or Dump(d)?\n");
  //  getchar(&key);
    if(key == 'r')
    {
        printf("channel =");
        //channel = getnumber();
        printf("address = ");
        //address = getnumber();
        data = w681388Read(address, channel);
        printf("chip[%d], reg[%d] r= 0x%2.2x(%d)\n", channel, address, data, data);
    }
    else if(key == 'w')
    {
        printf("channel =");
        //channel = getnumber();
        printf("address = ");
        //address = getnumber();
        printf("data = ");
        //data = getnumber();
        printf("chip[%d], reg[%d] w= %d\n", channel, address, data);
        w681388Write(data, address,channel);
    }
    else if(key == 'd')
    {
        printf("channel =");
       // channel = getnumber();
        DumpReg(channel);
    }
    else
        printf("command error!\n");
}

void DumpReg(unsigned int channel)
{
    int i;
    for(i = 0; i < 247; i++)
    {
        if(i%8 == 0)
            printf("\n");
        printf("r[%2.2x]=0x%2.2x  ", i, (int)w681388Read(i, channel));
    }
    printf("\n");
}

unsigned long interruptBits (unsigned long channelid) 
{
	// Determines which interrupt bit is set
	union {
		unsigned char reg_data[4];
		long interruptBits;
	} unionOfInterruptBits ;
	unionOfInterruptBits.interruptBits=0;
 
	//read interrupt state and clear ralated bit
	unionOfInterruptBits.reg_data[0] = w681388Read( W681388REG_INT2,channelid);
	w681388Write(unionOfInterruptBits.reg_data[0], W681388REG_INT2, channelid);		
	unionOfInterruptBits.reg_data[1] = w681388Read( W681388REG_INT1,channelid);
	w681388Write(unionOfInterruptBits.reg_data[1], W681388REG_INT1, channelid);	
	unionOfInterruptBits.reg_data[2] = w681388Read( W681388REG_INT3,channelid);
	w681388Write(unionOfInterruptBits.reg_data[2], W681388REG_INT3, channelid);	

	return unionOfInterruptBits.interruptBits ;
}

unsigned char interruptChannelData(unsigned long channelid)
{
	int i=0;
	char data;

	//data = Interrupt(channelid);

	if ( data != 0 ){
		chipData[channelid].interrupt = interruptBits(channelid); // Store which interrupt occured for which channel.
	}
	return data;
}

ULONG FSKSendData(BYTE *Buffer,ULONG channelid)
{
	BYTE status, count;
 
	//Load the FSK_LCR reg

	count=0;
	while(Buffer[count] != 0)
	{
		unsigned char tmp = 0;
		//Get FSK Status
		if(getLoopStatus(channelid))
		{
			tmp = w681388Read(W681388REG_INT1, channelid);
			if(tmp & 0x2)
			{
				TwoRiscSendMsg('f', channelid);
				w681388Write(0x2, W681388REG_INT1,channelid);
			}
			disableOscillators(channelid);
			return 1;
		}
		
		status = w681388Read(W681388REG_FSKS,channelid);

		while (status & 0x04){	//is FIFO Full?
			status = w681388Read(W681388REG_FSKS,channelid);

		}
		w681388Write(Buffer[count],W681388REG_FSKTD,channelid);
		//printf("FSK[%d] = 0x%x ",count,Buffer[count]);

		count++;
	}

	return 0;
}


/////////////////////////////////////////////////////////////////////////
// SendCallerID
// This routine Sends a Type I, Caller ID Mesaage. It should be called 	
//		during the OnHook Transmission state, between rings.
//		(Note: that the delay is only a placeholder. You must implement
//			  an accurate timer to do thid properly.)
//
//		Inputs:  Frequency, Amplitude, Cadence and Channel
//
/////////////////////////////////////////////////////////////////////////
ULONG SendCallerID (ULONG channelid)
{
	BYTE rmpc;
	int i;
	printf("caller id start\n");
	BYTE FSKString[31];

		unsigned char sum;
	//ProcessCallid(test, 19, 0);
	sum = checkSum(chipData[channelid].caller_number);

	w681388Write(0x0f,W681388REG_FSKLCR,channelid);
	//Load the FSK_FCR reg
	w681388Write(0x89,W681388REG_FSKC,channelid);
	
	for(i = 0;i<30;i++)
		FSKString[i] = 0x55;
	FSKString[i] = 0;

	//printf("1FSKLCR = 0x%x, FSKC = 0x%x\n",w681388Read(W681388REG_FSKLCR, channelid),w681388Read(W681388REG_FSKC, channelid));
	//OSTimeDlyHMSM(0, 0, 1, 0);
	//delay10us(500*100);	

	if(FSKSendData(FSKString,channelid))
		return 1;

	OSTimeDlyHMSM(0, 0, 0, 180);
	//delay10us(500*100);
	//Replace the dela above 
	// with a timer native to your system.
	BYTE status;
	status = w681388Read(W681388REG_FSKS, channelid);
	while(!(status &  0x1) )
		status = w681388Read(W681388REG_FSKS, channelid);
	
	if(FSKSendData(chipData[channelid].caller_number,channelid))
		return 1;
	//printf("2FSKLCR = 0x%x, FSKC = 0x%x\n",w681388Read(W681388REG_FSKLCR, channelid),w681388Read(W681388REG_FSKC, channelid));

	if(FSKSendData(&sum,channelid))
		return 1;
	//OSTimeDlyHMSM(0, 0, 0, 500);
	//Replace the dela above 
	// with a timer native to your system.

	//disable fsk
	//w681388Write(0x00,W681388REG_FSKC,channelid);	 

	return 0;

}
// Fcn

/*******************************************************************************
Below are functions needed for operation
********************************************************************************/

//disable oscillators and tones

void disableOscillators(unsigned long channelid) 
{ 
  	Tones = &TonesOff[0];
  	GenerateTones(Tones,channelid);

	goActive(channelid);
}

unsigned char getLoopStatus(unsigned long channelid)
{
	unsigned char LoopStatus;
	
	LoopStatus = w681388Read(W681388REG_LS,channelid);
	
	if(LoopStatus !=0x99)
		return	LoopStatus &1;
	else
		return 0;
}

BYTE GetKey(unsigned long channelid)
{
	BYTE digit = w681388Read(W681388REG_DTMFD,channelid) & 0x0f;
	BYTE key;

	switch(digit)
	{
		case 0xa:
			key = '0';
			break;
		case 0xb:
			key = '*';
			break;
		case 0xc:
			key = '#';
			break;
		case 0xd:
			key = 'A';
			break;
		case 0xe:
			key = 'B';
			break;
		case 0xf:
			key = 'C';
			break;
		case 0x0:
			key = 'D';
			break;
		default:
			key = '0' + digit;	
			break;
	}

	return key;
	
}
//return DTMF digit


////////////////////////////////////////////////////////////////////////
//  GenerateTones
//  Sets up Tone frequency, Amplitude and Cadence
//  ( i.e dial tone, RingBack, busy tone, etc.)
////////////////////////////////////////////////////////////////////////
WORD GenerateTones (structCountryTones *Tones,unsigned long channelid)
{
	unsigned char OscCntl, Temp; 

	OscCntl = w681388Read(W681388REG_OSNC,channelid);						//Get Oscillator Control
	OscCntl &= ~0x03;										//Clear Enables

	if(Tones->SlicOSC1_FREQ != 0)
	{
		OscCntl |= 0x01;					//Setup Oscillator Control

		w681388Write((BYTE)(Tones->SlicOSC1_AMP & 0xff),W681388REG_OS1ICL,channelid);				//Set Amplitude
		w681388Write((BYTE)((Tones->SlicOSC1_AMP>>8)& 0xff),W681388REG_OS1ICH,channelid);

		w681388Write((BYTE)(Tones->SlicOSC1_FREQ & 0xff),W681388REG_OS1CL,channelid);				//Set Frequency
		w681388Write((BYTE)((Tones->SlicOSC1_FREQ >>8)& 0xff),W681388REG_OS1CH,channelid);
			
		w681388Write((BYTE)(Tones->SlicOSC1_OnTime  & 0xff),W681388REG_OS1ATL,channelid);			//Set Active timer (On Time)
		w681388Write((BYTE)((Tones->SlicOSC1_OnTime>>8)& 0xff),W681388REG_OS1ATH,channelid);

		w681388Write((BYTE)(Tones->SlicOSC1_OffTime & 0xff),W681388REG_OS1ITL,channelid);			//Set Inactive timer  (Off Time)
		w681388Write((BYTE)((Tones->SlicOSC1_OffTime>>8)& 0xff),W681388REG_OS1ITH,channelid);											
	}

		
	if(Tones->SlicOSC2_FREQ != 0)
	{
		OscCntl |= 0x02;					//Setup Oscillator Control

		w681388Write((BYTE)(Tones->SlicOSC2_AMP & 0xff),W681388REG_OS2ICL,channelid);				//Set Amplitude
		w681388Write((BYTE)((Tones->SlicOSC2_AMP>>8)& 0xff),W681388REG_OS2ICH,channelid);

		w681388Write((BYTE)(Tones->SlicOSC2_FREQ & 0xff),W681388REG_OS2CL,channelid);				//Set Frequency
		w681388Write((BYTE)((Tones->SlicOSC2_FREQ>>8)& 0xff),W681388REG_OS2CH,channelid);
			
		w681388Write((BYTE)(Tones->SlicOSC2_OnTime & 0xff),W681388REG_OS2ATL,channelid);		    //Set Active timer (On Time)
		w681388Write((BYTE)((Tones->SlicOSC2_OnTime>>8)& 0xff),W681388REG_OS2ATH,channelid);

		w681388Write((BYTE)(Tones->SlicOSC2_OffTime & 0xff),W681388REG_OS2ITL,channelid);			//Set Inactive timer  (Off Time)
		w681388Write((BYTE)((Tones->SlicOSC2_OffTime>>8)& 0xff),W681388REG_OS2ITH,channelid);													
	}

	w681388Write(OscCntl,W681388REG_OSNC,channelid);								//Update Oscillator Control

	return 0;
}

////////////////////////////////////////////////////////////////////////
//  SetupRing  --> void initilaize_ringing(void) -->cadenceRingPhone(ringStruct ringRegs)
//  Sets up ring frequency, Amplitude and Cadence
////////////////////////////////////////////////////////////////////////
WORD SetupRing (structRingTones  *RingTones,unsigned long channelid)
{
	unsigned char OscCntl; 

	OscCntl |= 0x02;							//Setup Oscillator Control

	w681388Write((BYTE)((RingTones->SlicOSC2_AMP & 0xff)),W681388REG_OS2ICL,channelid);		//Set Amplitude
	w681388Write((BYTE)((RingTones->SlicOSC2_AMP>>8)& 0xff),W681388REG_OS2ICH,channelid);

	w681388Write((BYTE)((RingTones->SlicOSC2_FREQ & 0xff)),W681388REG_OS2CL,channelid);		//Set Frequency
	w681388Write((BYTE)((RingTones->SlicOSC2_FREQ>>8)& 0xff),W681388REG_OS2CH,channelid);
			
	w681388Write((BYTE)((RingTones->SlicOSC2_OnTime & 0xff)),W681388REG_OS2ATL,channelid);		//Set Active timer (On Time)
	w681388Write((BYTE)((RingTones->SlicOSC2_OnTime>>8)& 0xff),W681388REG_OS2ATH,channelid);

	w681388Write((BYTE)((RingTones->SlicOSC2_OffTime & 0xff)),W681388REG_OS2ITL,channelid);		//Set Inactive timer  (Off Time)
	w681388Write((BYTE)((RingTones->SlicOSC2_OffTime>>8)& 0xff),W681388REG_OS2ITH,channelid);													

	return 0;
}

////


/////////////////////////////////////////////////////////////////////////
// GenerateMeterPulse
// This routine generates a metering pulse for a line	 
//		Inputs:  Frequency, Amplitude, cadence and channel
//
/////////////////////////////////////////////////////////////////////////

WORD GenerateMeterPulse(WORD Frequency, WORD Amplitude, WORD OnTime, WORD OffTime,unsigned long channelid)
{
	BYTE Rmpc;
	
	Rmpc = w681388Read(W681388REG_RMPC,channelid);
	Rmpc |= 0x05;


	w681388Write(OnTime & 0xff,W681388REG_MPATL,channelid);				//Set Active Timer
	w681388Write((OnTime >>8)& 0xff,W681388REG_MPATH,channelid);
	
	w681388Write(OffTime & 0xff,W681388REG_MPITL,channelid);				//Set InActive Timer
	w681388Write((OffTime >>8)& 0xff,W681388REG_MPITH,channelid);
	
	w681388Write(Frequency & 0xff,W681388REG_MPCL,channelid);			//Set Frequency
	w681388Write((Frequency >>8)& 0xff,W681388REG_MPCH,channelid);
								
	w681388Write(Amplitude & 0xff,W681388REG_MPICL,channelid);			//Set Amplitude
	w681388Write((Amplitude >>8)& 0xff,W681388REG_MPICH,channelid);	
											
	w681388Write(0x08,W681388REG_MPADS,channelid);				//Metering Attack Rate
	
	w681388Write(0x40,W681388REG_MPMX,channelid);
	
	w681388Write(Rmpc,W681388REG_RMPC,channelid);						//Enabale Pulse Metering
	
	return 0;  

}


/**********************************************************************
Below are functions used for setting the linefeed states for operation.
***********************************************************************/

//Set LINEFEED to ACTIVE
void goActive(unsigned long channelid)
{
	/* SET LOOP STATUS REGISTER TO ACTIVE */
	w681388Write(LS_FW_ACTIVE,W681388REG_LS,channelid);

	/* The phone hook-switch sets the off-hook and on-hook substate*/
}

//set LINEFEED to OPEN
void goOpen(unsigned long channelid)
{
	w681388Write(LS_OPEN,W681388REG_LS,channelid);
}


//set Line Feed to Ringing
void activateRinging(unsigned long channelid)
{
	w681388Write(LS_RINGING,W681388REG_LS,channelid);
}

void stopRinging(unsigned long channelid)
{   
	goActive(channelid);
}

void pulseMeter(unsigned long channelid)
{
	GenerateMeterPulse(0x5a83, 0x6134, 0x1f40, 0x3e80,channelid);
}	


/****************************************************************
performs necessary routines of each state and changes to next state
******************************************************************/
void HwSlicStateMachine(unsigned long channelid)
{

	int j;
	switch (chipData[channelid].state)
	{
	/*  Below are only the meta/transient states (transitions).
	The actual precursor states are changed durring the event/interrupt handler.
	I know this is unconventional but it does shrink down the code considerably.  	
	Latter when I fold in the wait states and the delays I will put the stable states back here.	
		*/
	
	case STATEcHANGE:
		 chipData[channelid].state=chipData[channelid].newState;
		  break;			
	case CALLBACK :
		disableOscillators(channelid);
		Tones = &WorldRingTones[TONE_CHINA];
		SetupRing(Tones,channelid);
		activateRinging(channelid);
		chipData[channelid].state = FIRSTrING;
		break; 
	//case MAKEpULSE:
		//disableOscillators(channelid);
		//pulseMeter (channelid);
		//HwSlicSetState (PULSING,channelid);
		//break;
	case CALLBACKpROGRAMMED:
		disableOscillators(channelid);
	
 		activateRinging(channelid);
		chipData[channelid].state = PROGRAMMEDrING;
		break; 

	case MAKEoFFHOOK: // Make dialtone 
	  	Tones = &WorldDialTones[TONE_CHINA];
  		GenerateTones(Tones,channelid);	
		HwSlicSetState(DIALtONE,channelid);
		break;
	case LOOPtRANSITION://handles loop transitions.
		switch (chipData[channelid].previousState)
		{
				
			case RINGING:
			case FIRSTrING:
			case CALLERiD:
				HwSlicSetState(OFFHOOK,channelid);
				break;
			case DEFEREDcALLBACKpROGRAMMED:
				HwSlicSetState(CALLBACKpROGRAMMED,channelid);
				break;
			//case DIALtONE:
				//chipData[channelid].currentPulsedigit=0;
				//disableOscillators(channelid);
			//case DIALpULSE://checks if in on-hook transition was really a pulse digit.
			//case DIGITDECODING:
				//break;
			//case PULSING:
				//w681388Write (0x00,PMCON,channelid); //disable pulse metering
				//break;
			default:
				;//loopAction(channelid);
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
						//dtmfAction();
						break;
					case DIGITDECODING:
						//dtmfAction();
						break;
			}
		break;

	case CALLERiD:
		SendCallerID(channelid);
		HwSlicSetState(RINGING,channelid);
		break;
		
	case FIRSTrING:
	case RINGING:
		/*{int k = ONHOOK;}
	if (chipData[channelid].connectionWith != currentChannel )
			j=channelData[chipData[channelid].connectionWith].state;
		else
			j=0xFF;
		

		if ((chipData[channelid].ringCount>6)|| (j==ONHOOK))
			
		{ 
			breakConnection(currentChannel,chipData[channelid].connectionWith );
			stopRinging();
			chipData[channelid].ringCount=0;

		HwSlicSetState(ONHOOK);
		}*/
		break;

	case ONHOOK:

/*  The marker for a non-connection is that connectionWidth element is connection to its self */
/*
		if (chipData[channelid].connectionWith != currentChannel ){
	breakConnection(currentChannel,chipData[channelid].connectionWith );
	channelData[chipData[channelid].connectionWith].connectionWith=chipData[channelid].connectionWith;
	if(channelData[chipData[channelid].connectionWith].state == OFFHOOK2) channelData[chipData[channelid].connectionWith].state = MAKEcONGESTION;
	chipData[channelid].connectionWith =currentChannel;
		}*/
		break;
	case MAKEbUSY:
  		Tones = &WorldBusyTones[TONE_CHINA];
  		GenerateTones(Tones,channelid);
		HwSlicSetState(BUSY,channelid);
		break;
	case MAKErINGbACK:
  		Tones = &WorldRingBackTones[TONE_CHINA];
  		GenerateTones(Tones,channelid);
  			
		HwSlicSetState(RINGbACK,channelid);
		break;

	case RINGbACK:
		/*
	if (chipData[channelid].connectionWith != currentChannel )
		{
		if (channelData[chipData[channelid].connectionWith].state==OFFHOOK)
		
			HwSlicSetState(OFFHOOK);
		}*/
		break;
/*
	case OFFHOOK:
		disableOscillators();
		if (connectionTable[currentChannel].destination== currentChannel )
			`makeConnection(currentChannel,chipData[channelid].connectionWith);
		
		HwSlicSetState(OFFHOOK2);
		break;
	case MAKErEORDER:
		HwSlicSetState(REORDER);
		chipData[channelid].digit_count=0;
		break;
	case MAKEcONGESTION:
  		Tones = &WorldBusyTones[data];
  		GenerateTones2(Tones);
		HwSlicSetState(CONGESTION);
		chipData[channelid].digit_count=0;
		break;*/
	}

	if(chipData[channelid].code_type != 0)
	{
		BYTE code;

		code = w681388Read(W681388REG_PCMC, channelid) & 0x3f;
		//code |= 0x4;
		
		if(AUDIO_ENCODING_ULAW == chipData[channelid].code_type)
			w681388Write((code | (1 << 6)), W681388REG_PCMC, channelid);
			
		else if(AUDIO_ENCODING_ALAW == chipData[channelid].code_type)
			w681388Write((code), W681388REG_PCMC, channelid);
		else if(AUDIO_ENCODING_LINEAR== chipData[channelid].code_type)
			w681388Write((code | (2 << 6)), W681388REG_PCMC, channelid);

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
	}
*/

}

void slic_test_init(unsigned long channelid)
{
    struct tPcmMode PCMMode;
    int i;
	
    PCMMode.CodecMode = PCM_CODEC_ALOW;
    PCMMode.BitMode = PCM_CODEC_8BIT;
    // pcm if set
    PCMMode.InternalFsync = PCM_EXFS;
    PCMMode.FsType = PCM_SHORT_FS;
    PCMMode.GciFormat = PCM_GCI_1BIT;
    PCMMode.TriMode = PCM_TRI_NEGATIVE;
    PCMMode.Fbclk = 2048;
    // time slot set
    PCMMode.TxTimeSlot = 33;
    PCMMode.RxTimeSlot = 33;
    SlicSetPcmMode(&PCMMode, channelid);
}

/*******************************************************************
This function combined with the function is ISR.c accomplish the entire
Interrupt Service Routine of the program.  Here the  program finds what caused
the interupt and changes the state of that channel as necessary.
*********************************************************************/
 
void defered_isr(unsigned long channelid) 
{

	unsigned long shiftMask=1, original_vec;

	enum               
	{
	    OSC1_ON=0,  
	    OSC1_OFF=1,
	    OSC2_ON=2,
	    OSC2_OFF=3,
	    RING_ON=4,
	    RING_OFF=5,
	    DTMF=6,			// DTMF detected  
	    FSK=7,			// FSK FIFO Empty 
	    RING_TRIP=8,
	    LOOP_CLOSURE=9,
	    PQ1=10,
	    PQ2=11,
	    PQ3=12,
	    PQ4=13,
	    PQ5=14,
	    PQ6=15,
	    TEMP=16,		// Temperature INT detected  
	    PULSE_ON=17,
	    PULSE_OFF=18,
	} interruptCause;               

	static char *icause[]={
		"Osc1 Active",  
		"Osc1 Inactive",  
		"Osc2 Active",  
		"Osc2 Inactive",  
		"Ring Active", 
		"Ring Inactive" ,
		"DTMF Decode",		// DTMF detected 
		"FSK FIFO Empty ",  // FSK FIFO Empty 
		"Ring Trip",
		"Loop Closure",
		"				Pwr Alarm QR2",
		"				Pwr Alarm QT2",
		"				Pwr Alarm QR1",
		"               Pwr Alarm QT1",
		"               Pwr Alarm QR3",
		"               Pwr Alarm QT3",
		"Temperature Alarm",
		"Pulse Metering Active",
		"Pulse Metering Inactive",

	 };

	chipData[channelid].eventNumber++;
	original_vec = chipData[channelid].interrupt;

	for(interruptCause = OSC1_ON ; interruptCause <= PULSE_OFF ; interruptCause++)
	{
		if (shiftMask & chipData[channelid].interrupt)
		{
			chipData[channelid].interrupt &= ~shiftMask;   // clear interrupt cause
			 
			switch (interruptCause) {
				// Figure out what todo based on which one occured
			
				case RING_TRIP:
				case LOOP_CLOSURE:
					printf("%s\n",getLoopStatus(channelid) ? "Off Hook" : "On Hook");
					if(getLoopStatus(channelid))
					{
						BYTE pcmcontrol = w681388Read(W681388REG_PCMC, channelid);
						pcmcontrol |= 0x1;	//enable pcm
						w681388Write(pcmcontrol, W681388REG_PCMC,channelid);
						TwoRiscSendMsg('f', channelid);
						
						//w681388AdjLineState(LS_RV_ACITVE, channelid);
					}
					else
					{
						TwoRiscSendMsg('n', channelid);
						disableOscillators(channelid);
						HwSlicSetState(ONHOOK,channelid);
						//w681388AdjLineState(LS_RV_OH_TX, channelid);
					}
					//HwSlicSetState(LOOPtRANSITION,channelid);
					break;			
			
				case RING_ON:

					chipData[channelid].ringCount++;
					if (chipData[channelid].state==FIRSTrING)//run caller ID after first ring.
					{
						chipData[channelid].ringCount=1;
						HwSlicSetState(CALLERiD,channelid);
					}
					break;
					
				case RING_OFF:
					break;
					
				case PULSE_ON:
					break;

				case PULSE_OFF:
					break;
				case PQ1:
				case PQ2:
				case PQ3:
				case PQ4:
				case PQ5:
				case PQ6:
				/*	{	
						static unsigned long lastEventNumber[8];
						{
							int lcv;		
							for (lcv=0;lcv<8;lcv++)
								lastEventNumber[lcv] =1;  
						}
						if (lastEventNumber[currentChannel] != chipData[channelid].eventNumber)  
							int i = interruptCause - PQ1;
							lastEventNumber[currentChannel] = chipData[channelid].eventNumber;

							printf( "  %d time",chipData[channelid].qLog[i]);
							if (chipData[channelid].qLog[i]++>2) 
								exception(POWERaLARMQ1+i);
							if(chipData[channelid].qLog[i] >1) printf( "s");
							
							goActive(channelid);
							HwSlicSetState(ONHOOK,channelid);
						}
							 
					}
*/
					break;
				case DTMF:
					disableOscillators(channelid);
					printf("GetKey: %c\n",GetKey(channelid));
					TwoRiscSendMsg(GetKey(channelid), channelid);
					
					//HwSlicSetState(DTMFtRANISTION,channelid); 
					break;

				case OSC1_ON:
					{

						//if (count==20)
							//count=2; //reset counter

					}
					break;
				case OSC1_OFF:
					break;
				case OSC2_ON:
					break;
				case OSC2_OFF:
					break;
				case FSK:		// FSK FIFO Empty 
					w681388Write(0x00,W681388REG_FSKC,channelid);	 //disable FSK
					printf("FSK empty\n");
					break;		
				case TEMP:		// Temperature INT detected  
					printf("Temperature too high,temperature = %dC\n",w681388Read(0x8b,channelid));
					break;
					

			} //switch

			
		} //if
		  shiftMask<<=1;
	} //for


} // function interruptCause

VOID HwSlicPhoneSystemLoop() 
{
	LONG i=0, chip=0,channelid;
	LONG timeout = 200;

	printf("Slic System Loop start ******************************************************\n");
	while(1) {
		//printf("in while ");
		for(channelid = CHANNEL_START;channelid<channel_num;channelid++)
		{	
				chipData[channelid].interrupt = interruptBits(channelid);
				//printf("interrupt = 0x%x\n",chipData[channelid].interrupt);
				if (chipData[channelid].interrupt)
					defered_isr(channelid);   // isr = interrrupt service routine

			HwSlicStateMachine(channelid);
		
		}

		if(0 == timeout--)
		{
			printf("loop status = 0x%x\n",w681388Read(W681388REG_LS, 0));
			timeout = 400;
		}
           OSTimeDlyHMSM(0, 0, 0, 50); 	
	}
}

void Slic_Task(void *data)
{
    	int i,channelid;
    	char key;
    
    	AppAudioClkOpen();	//Set bclk
	HwSifSeting();		//reset sif interface and gpio
//	RiscSemUnLock(5);
//	chipData[0].cs = 1;
//	chipData[1].cs =6;
 	 for(channelid = CHANNEL_START;channelid<channel_num;channelid++)
	{
		if(!w681388Reset(channelid))
			printf("Channel %d PLL clock is unstable,exit\n",channelid);
		
		printf("channel %d initialize...",channelid);
		SlicInit(channelid);  /* initialize the ProSLIC */
		printf("done!\n");

		HwSlicSetState(ONHOOK,channelid);

		//slic_test_init(channelid);
		
		chipData[channelid].eventEnable = 1;		//init chipdata struture.
		Init_Queue(&chipData[channelid].workqueue);	//init workqueue for set register
	}
	 	//slic_test_init(0);
    	//DumpReg(TEST_CHANNEL);
	HwSlicPhoneSystemLoop();
}
#endif

