#ifndef __AUDIO_API_H__
#define __AUDIO_API_H__

//#include  <asm/mach-au1x00/vb1000_controller.h>
#include  <def_type.h>


typedef struct AUDIO_PARAMETER
{
	LONG lwSampleBit;
	LONG lwSampleRate;
	LONG lwSampleMethod;
	LONG lwTimeSlot;
}tAUDIO_PARAMETER, *ptAUDIO_PARAMETER;

typedef struct AUDIO_DATA_INFO
{
	LONG lwChannelId;
	LONG lwAddr;
	LONG lwLen;
}tAUDIO_DATA_INFO,*ptAUDIO_DATA_INFO;

/*
func: open audio
In para: pConfig, pConfig[0]:sampling bit;pConfig[1]:sampling frequency;pConfig[2]:sampling method;
return: TRUE/FALSE
*/
//BOOL AppAudioOpen(LONG lwChannelId);

/*
func: close audio
return: TRUE/FALSE
*/
//BOOL AppAudioClose(LONG lwChannelId);

/*
func: modify parameter of Audio
In para: pConfig, pConfig[0]:sampling bit;pConfig[1]:sampling frequency;pConfig[2]:sampling method;
return TRUE/FALSE
*/
//BOOL AppAudioModifyPara(LONG lwChannelId);

/*
func:Audio start voice sampling
In para: pConfig, pConfig[0]:sampling bit;pConfig[1]:sampling frequency;pConfig[2]:sampling method;
return: TRUE/FALSE
*/
//BOOL AppAudioRun(LONG lwChannelId);

/*
func:Audio Stop voice sampling
return: TRUE/FALSE
*/
//BOOL AppAudioStop(LONG lwChannelId);

/*
func:Sip receive data from Audio 
In para: data's len  required 
return: >=0 : data's len received; -1: no data
*/
//LONG AppAudioReceiveData(LONG lwChannelId,PCHAR pcwData,LONG lwLen);

/*
func:Sip send data to Audio
return:len
*/
//LONG AppAudioSendData(LONG lwChannelId,PCHAR pcwData,LONG lwLen);


typedef enum tAudioSubType
{
	tAudioOpen		= 0x1,	
	tAudioClose		= 0x2, 
	tAudioSetAudioCodec	= 0x3,	
	tAudioStartVoice	= 0x4,
	tAudioStopVoice		= 0x5,
	tAudioTransferLinuxToUcos = 0x6,
	tAudioTransferUcosToLinux = 0x7,
	tAudioStartTone		= 0x8,
	tAudioStopTone		= 0x9,
	tAudioPlayPCMData	= 0xa,
	tAudioTransmitKeyFromUcosToLinux	= 0xb,
	tSendCallerID		= 0xc,
	tAddOneTalker		= 0xd,
	tDelOneTalker		= 0xe,
	tSetSlicRegister	= 0xf,
	tGetSlicRegister	= 0x10,
	tSetEnAudioMode     =0x11,
	tSetVoiceUP         =0x31,
	tSetVoiceDown       =0x32,
}tAudioSubType;

typedef enum tAudioCodecEncodingType
{
    tG711U	= 0,
    tG711A	= 8,
    tG726_16K	= 98,
    tG726_24K	= 97,
    tG726_32K	= 2,
    tG726_40K	= 96,
    tG729A	= 18,
    tG729B	= 72,
    tG723_1_24	= 4,
    tG723_1_20	= 3,
}tAudioCodecEncodingType;


#endif
